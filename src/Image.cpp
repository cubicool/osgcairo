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

		// In A8 surfaces, the width must be evenly divisible by 4; if not,
		// this will catch that failure and resize the image appropriately.
		width = cairo_format_stride_for_width(format, width);
	}

	else if(format != CAIRO_FORMAT_ARGB32) {
		OSGCAIRO_WARN 
			<< "The format you've chosen is not supported in osgCairo; there is no "
			"compelling reason (speed or otherwise) to use a format other than "
			"ARGB32 or A8 in an OpenGL setting."
			<< std::endl
		;

		return false;
	}

	// Call the osg::Image allocation method.
	allocateImage(width, height, 1, pf1, pf2);

	if(!osg::Image::valid()) return false;

	unsigned int i = 4;

	// This will have to do for now. :)
	if(format == CAIRO_FORMAT_A8) i = 1;

	if(data) std::memcpy(_data, data, (width * height) * i);

	else std::memset(_data, 0, (width * height) * i);

	if(format == CAIRO_FORMAT_A8) _pixelFormat = GL_ALPHA;

	else _pixelFormat = GL_BGRA;

	// Remove any previous surface allocation.
	if(_surface) cairo_surface_destroy(_surface);

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

bool Image::allocateSurface(const osg::Image* image) {
	if(!image) return false;

	if(_surface) return false;

	const osgCairo::Image* test = dynamic_cast<const osgCairo::Image*>(image);

	if(test) {
		OSGCAIRO_WARN
			<< "Cannot allocate from an existing osgCairo::Image; "
			"use copy construction instead; this method is only intended "
			"for allocating from a traditional osg::Image."
			<< std::endl
		;

		return false;
	}

	GLenum format = image->getPixelFormat();

	if(format == GL_ALPHA || format == GL_LUMINANCE) return allocateSurface(
		image->s(),
		image->t(),
		CAIRO_FORMAT_A8,
		static_cast<const unsigned char*>(image->getDataPointer())
	);

	else if(format != GL_RGB && format != GL_RGBA) {
		OSGCAIRO_WARN
			<< "Can only allocate ARGB32 surfaces from GL_RGB/GL_RGBA Image sources."
			<< std::endl
		;

		return false;
	}

	allocateImage(image->s(), image->t(), 1, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV);

	if(!osg::Image::valid()) return false;

	_pixelFormat = GL_BGRA;

	const unsigned char* data      = image->data();
	unsigned int         numPixels = image->s() * image->t();
	unsigned int         offset    = 4;

	if(format == GL_RGB) offset = 3;

	for(unsigned int i = 0; i < numPixels; i++) {
		unsigned char a = data[i * offset + 3];
		unsigned char r = data[i * offset + 2];
		unsigned char g = data[i * offset + 1];
		unsigned char b = data[i * offset];

		// We want ARGB32 from a _SOURCE_ that _DOES_ have an alpha
		// channel; therefore, we need to premultiply.
		if(format == GL_RGBA) {			
			_data[i * 4]     = (r * a) / 255;
			_data[i * 4 + 1] = (g * a) / 255;
			_data[i * 4 + 2] = (b * a) / 255;
			_data[i * 4 + 3] = a;
		}

		// Otherwise, we're loading from a source with no alpha
		// channel. This doesn't make a lot of sense to do, unless
		// the user is planning on using CAIRO_OPERATOR_CLEAR at
		// some point during their drawing.
		else {
			_data[i * 4]     = r;
			_data[i * 4 + 1] = g;
			_data[i * 4 + 2] = b;
			_data[i * 4 + 3] = 255;
		}
	}

	// Remove any previous surface allocation.
	if(_surface) cairo_surface_destroy(_surface);

	_surface = cairo_image_surface_create_for_data(
		_data,
		CAIRO_FORMAT_ARGB32,
		image->s(),
		image->t(),
		cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, image->s())
	);

	if(!valid()) return false;

	dirty();

	return true;
}

cairo_t* Image::createContext(bool adjustOrigin) const {
	cairo_t* c = cairo_create(_surface);

	if(adjustOrigin && c) {
		cairo_translate(c, 0.0f, getSurfaceHeight());
		cairo_scale(c, 1.0f, -1.0f);
	}

	return c;
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
	return cairo_image_surface_get_stride(_surface);
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

}

