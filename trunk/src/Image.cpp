// -*-c++-*- osgCairo - Copyright (C) 2006 Jeremy Moles

#include <cstring>
#include <osg/Notify>
#include <osgCairo/Image>

namespace osgCairo {

Image::Image(CairoFormat format):
_allocated (false),
_format    (format) {
}

Image::Image(
	unsigned int         width,
	unsigned int         height,
	CairoFormat          format,
	const unsigned char* data
):
_allocated (false),
_format    (format) {
	allocateSurface(width, height, data);
}

Image::Image(const Image& si, const osg::CopyOp& co):
osg::Image (si, co),
_allocated (si._allocated),
_format    (si._format) {
}

CairoSurface* Image::_createSurfaceImplementation() {
	return cairo_image_surface_create_for_data(
		_data,
		_format,
		_s,
		_t,
		cairo_format_stride_for_width(_format, _s)
	);
}

bool Image::allocateSurface(
	unsigned int         width,
	unsigned int         height,
	const unsigned char* data
) {
	GLenum pf1 = GL_RGBA;
	GLenum pf2 = GL_UNSIGNED_INT_8_8_8_8_REV;

	if(_format == CAIRO_FORMAT_A8) {
		pf1 = GL_ALPHA;
		pf2 = GL_UNSIGNED_BYTE;
	}

	// Call the osg::Image allocation method.
	osg::Image::allocateImage(width, height, 1, pf1, pf2);

	if(!osg::Image::valid()) {
		osg::notify(osg::WARN)
			<< "osgCairo::Image::allocateImage failed!"
			<< std::endl
		;

		return false;
	}

	unsigned int i = 0;

	// This will have to do for now. :)
	if(_format == CAIRO_FORMAT_A8) i = 1;

	else i = 4;
	
	if(data) std::memcpy(_data, data, (width * height) * i);

	else std::memset(_data, 0, (width * height) * i);

	_allocated = true;

	if(_format == CAIRO_FORMAT_A8) _pixelFormat = GL_ALPHA;

	else _pixelFormat = GL_BGRA;

	dirty();

	return true;
}

void Image::setOriginBottomLeft() {
	setMatrix(Matrix::translate(0.0f, -_t) * Matrix::scale(1.0f, -1.0f));
}

unsigned int Image::getImageSizeInBytes() const {
	return computeRowWidthInBytes(_s, _pixelFormat, _dataType, _packing) * _t * _r;
}

// Thank you to Riccardo Corsi <riccardo.corsi@vrmmp.it> for pointing out the necessity
// of this routine. :)
unsigned char* convertImageDataToCairoFormat(osg::Image* image, CairoFormat cairoFormat) {
	unsigned char* data   = image->data();
	GLenum         format = image->getPixelFormat();

	if(cairoFormat == CAIRO_FORMAT_ARGB32) {
		if(format != GL_RGB && format != GL_RGBA) return 0;

		unsigned int   numPixel = image->s() * image->t();
		unsigned char* newData  = new unsigned char[numPixel * 4];
		unsigned int   offset   = 4;

		if(format == GL_RGB) offset = 3;

		for(unsigned int i = 0; i < numPixel; ++i) {
			newData[i * 4]     = data[i * offset + 2];
			newData[i * 4 + 1] = data[i * offset + 1];
			newData[i * 4 + 2] = data[i * offset];

			if(format == GL_RGBA) newData[i * 4 + 3] = data[i * offset + 3];

			else newData[i * 4 + 3] = 255;
		}

		return newData;
	}

	return 0;
}

} // namespace osgCairo
