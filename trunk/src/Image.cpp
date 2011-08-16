// -*-c++-*- osgCairo - Copyright (C) 2011 osgCairo Development Team
// $Id$

#include <cstring>
#include <cmath>
#include <osgCairo/Notify>
#include <osgCairo/Image>

namespace osgCairo {

Image::Image():
_surface(0) {
}

Image::Image(
	unsigned int         width,
	unsigned int         height,
	cairo_format_t       format,
	const unsigned char* data
):
_surface(0) {
	allocateSurface(width, height, format, data);
}

Image::Image(const Image& si, const osg::CopyOp& co):
osg::Image(si, co),
_surface(si._surface) {
	// If the user wants to do a full copy...
	if(co.getCopyFlags() & osg::CopyOp::DEEP_COPY_IMAGES) {
		_surface = cairo_image_surface_create_for_data(
			_data,
			si.getSurfaceFormat(),
			si.getSurfaceWidth(),
			si.getSurfaceHeight(),
			si.getSurfaceStride()
		);
	}

	// Otherwise, bump the reference count.
	else cairo_surface_reference(_surface);
}

Image::~Image() {
	if(_surface) cairo_surface_destroy(_surface);
}

bool Image::allocateSurface(
	unsigned int         width,
	unsigned int         height,
	cairo_format_t       format,
	const unsigned char* data
) {
	// The default is for CAIRO_FORMAT_ARGB32.
	GLenum pf1 = GL_RGBA;
	GLenum pf2 = GL_UNSIGNED_INT_8_8_8_8_REV;

	if(format == CAIRO_FORMAT_A8) {
		pf1 = GL_ALPHA;
		pf2 = GL_UNSIGNED_BYTE;
	}

	else if(format == CAIRO_FORMAT_RGB24) pf1 = GL_RGB;

	// Call the osg::Image allocation method.
	allocateImage(width, height, 1, pf1, pf2);

	if(!osg::Image::valid()) return false;

	unsigned int i = 0;

	// This will have to do for now. :)
	if(format == CAIRO_FORMAT_A8) i = 1;

	else i = 4;
	
	if(data) std::memcpy(_data, data, (width * height) * i);

	else std::memset(_data, 0, (width * height) * i);

	if(format == CAIRO_FORMAT_A8) _pixelFormat = GL_ALPHA;

	else _pixelFormat = GL_BGRA;

	_surface = cairo_image_surface_create_for_data(
		_data,
		format,
		width,
		height,
		cairo_format_stride_for_width(format, width)
	);

	if(!valid()) return false;

	dirty();

	return true;
}

cairo_t* Image::createContext() const {
	return cairo_create(_surface);
}

bool Image::valid() const {
	return !cairo_surface_status(_surface) && osg::Image::valid();
}

bool Image::preMultiply() {
	if(getSurfaceFormat() != CAIRO_FORMAT_ARGB32) return false;

	for(int p = 0; p < _s * _t; p++) {
		unsigned int  offset = p * 4;
		unsigned char alpha  = _data[offset + 3];

		_data[offset]     = (_data[offset] * alpha) / 255;
		_data[offset + 1] = (_data[offset + 1] * alpha) / 255;
		_data[offset + 2] = (_data[offset + 2] * alpha) / 255;
	}

	dirty();

	return true;
}

bool Image::unPreMultiply() {
	if(getSurfaceFormat() != CAIRO_FORMAT_ARGB32) return false;

	for(int p = 0; p < _s * _t; p++) {
		unsigned int  offset = p * 4;
		unsigned char alpha  = _data[offset + 3];

		if(!alpha) continue;

		_data[offset]     = (_data[offset] * 255) / alpha;
		_data[offset + 1] = (_data[offset + 1] * 255) / alpha;
		_data[offset + 2] = (_data[offset + 2] * 255) / alpha;
	}

	dirty();

	return true;
}

cairo_surface_t* Image::getSurface() const {
	return _surface;
}

cairo_format_t Image::getSurfaceFormat() const {
	return cairo_image_surface_get_format(_surface);
}

int Image::getSurfaceWidth() const {
	return cairo_image_surface_get_width(_surface);
}

int Image::getSurfaceHeight() const {
	return cairo_image_surface_get_height(_surface);
}

int Image::getSurfaceStride() const {
	return cairo_image_surface_get_height(_surface);
}

unsigned char* Image::getSurfaceData() const {
	return cairo_image_surface_get_data(_surface);
}

cairo_status_t Image::getSurfaceStatus() const {
	return cairo_surface_status(_surface);
}

unsigned int Image::getImageSizeInBytes() const {
	return computeRowWidthInBytes(_s, _pixelFormat, _dataType, _packing) * _t * _r;
}

// Thank you to Riccardo Corsi <riccardo.corsi@vrmmp.it> for pointing out the necessity
// of this routine. :)
// TODO: If this routine gets called a lot, it may make sense to change the data
// IN PLACE instead of doing a new allocation every time.
unsigned char* createNewImageDataAsCairoFormat(osg::Image* image, cairo_format_t cairoFormat) {
	unsigned char* data     = image->data();
	unsigned char* newData  = 0;
	GLenum         format   = image->getPixelFormat();
	unsigned int   numPixel = image->s() * image->t();

	if(cairoFormat == CAIRO_FORMAT_ARGB32 || cairoFormat == CAIRO_FORMAT_RGB24) {
		if(format != GL_RGB && format != GL_RGBA) return 0;

		unsigned int offset = 4;

		newData = new unsigned char[numPixel * 4];

		if(format == GL_RGB) offset = 3;

		for(unsigned int i = 0; i < numPixel; i++) {
			unsigned char a = data[i * offset + 3];
			unsigned char r = data[i * offset + 2];
			unsigned char g = data[i * offset + 1];
			unsigned char b = data[i * offset];

			// Requested format is RGB24, which doesn't use the alpha byte.
			if(cairoFormat == CAIRO_FORMAT_RGB24) {
				newData[i * 4]     = r;
				newData[i * 4 + 1] = g;
				newData[i * 4 + 2] = b;
				newData[i * 4 + 3] = 0;
			}

			else {
				// We want ARGB32 from a _SOURCE_ that _DOES_ have an alpha
				// channel; therefore, we need to premultiply.
				if(format == GL_RGBA) {			
					newData[i * 4]     = (r * a) / 255;
					newData[i * 4 + 1] = (g * a) / 255;
					newData[i * 4 + 2] = (b * a) / 255;
					newData[i * 4 + 3] = a;
				}

				// Otherwise, we're loading from a source with no alpha
				// channel. This doesn't make a lot of sense to do, unless
				// the user is planning on using CAIRO_OPERATOR_CLEAR at
				// some point during their drawing.
				else {
					newData[i * 4]     = r;
					newData[i * 4 + 1] = g;
					newData[i * 4 + 2] = b;
					newData[i * 4 + 3] = 255;
				}
			}
		}

	}

	// The user wants CAIRO_FORMAT_A8...
	else {
		if(format != GL_ALPHA && format != GL_LUMINANCE) {
			OSGCAIRO_WARN("createNewImageDataAsCairoFormat")
				<< "Couldn't understand GL format enum '" << format
				<< "' for CAIRO_FORMAT_A8."
				<< std::endl
			;

			return 0;
		}

		newData = new unsigned char[numPixel];
		
		std::memcpy(newData, data, numPixel);
	}

	return newData;
}

}
