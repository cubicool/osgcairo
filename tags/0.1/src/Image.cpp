// -*-c++-*- osgCairo - Copyright (C) 2009 Jeremy Moles
// $Id$

#include <cstring>
#include <cmath>
#include <osg/Notify>
#include <osgCairo/Image>

namespace osgCairo {

Image::Image() {
}

Image::Image(
	unsigned int   width,
	unsigned int   height,
	CairoFormat    format,
	unsigned char* data
) {
	allocateSurface(width, height, format, data);
}

Image::Image(const Image& si, const osg::CopyOp& co):
osg::Image(si, co) {
}

bool Image::allocateSurface(
	unsigned int   width,
	unsigned int   height,
	CairoFormat    format,
	unsigned char* data
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

	dirty();

	return Surface::allocateSurface(width, height, format, _data);
}

bool Image::valid() const {
	return Surface::valid() && osg::Image::valid();
}

void Image::setOriginBottomLeft() {
	setMatrix(Matrix::translate(0.0f, -_t) * Matrix::scale(1.0f, -1.0f));
}

unsigned int Image::getImageSizeInBytes() const {
	return computeRowWidthInBytes(_s, _pixelFormat, _dataType, _packing) * _t * _r;
}

// Thank you to Riccardo Corsi <riccardo.corsi@vrmmp.it> for pointing out the necessity
// of this routine. :)
// TODO: If this routine gets called a lot, it may make sense to change the data
// IN PLACE instead of doing a new allocation every time.
unsigned char* createNewImageDataAsCairoFormat(osg::Image* image, CairoFormat cairoFormat) {
	unsigned char* data   = image->data();
	GLenum         format = image->getPixelFormat();

	if(cairoFormat == CAIRO_FORMAT_ARGB32 || cairoFormat == CAIRO_FORMAT_RGB24) {
		if(format != GL_RGB && format != GL_RGBA) return 0;

		unsigned int   numPixel = image->s() * image->t();
		unsigned char* newData  = new unsigned char[numPixel * 4];
		unsigned int   offset   = 4;

		if(format == GL_RGB) offset = 3;

		for(unsigned int i = 0; i < numPixel; ++i) {
			newData[i * 4]     = data[i * offset + 2];
			newData[i * 4 + 1] = data[i * offset + 1];
			newData[i * 4 + 2] = data[i * offset];

			if(cairoFormat == CAIRO_FORMAT_ARGB32) {
				if(format == GL_RGBA) newData[i * 4 + 3] = data[i * offset + 3];

				else newData[i * 4 + 3] = 255;
			}

			else newData[i * 4 + 3] = 0;
		}

		return newData;
	}

	return 0;
}

}