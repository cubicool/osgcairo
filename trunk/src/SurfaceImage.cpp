// -*-c++-*- osgCairo - Copyright (C) 2006 Jeremy Moles

#include <cstring>
#include <sstream>
#include <osg/Notify>
#include <osgCairo/SurfaceImage>

namespace osgCairo {

SurfaceImage::SurfaceImage(CairoFormat format):
_allocated (false),
_format    (format) {
}

SurfaceImage::SurfaceImage(
	unsigned int         width,
	unsigned int         height,
	const unsigned char* data,
	CairoFormat          format
):
_allocated (false),
_format    (format) {
	allocateSurface(width, height, data);
}

SurfaceImage::SurfaceImage(const SurfaceImage& si, const osg::CopyOp& co):
osg::Image (si, co),
_allocated (si._allocated),
_format    (si._format) {
}

CairoSurface* SurfaceImage::_createSurfaceImplementation() {
	return cairo_image_surface_create_for_data(
		_data,
		_format,
		_s,
		_t,
		cairo_format_stride_for_width(_format, _s)
	);
}

bool SurfaceImage::allocateSurface(
	unsigned int         width,
	unsigned int         height,
	const unsigned char* data
) {
	GLenum pf = 0;

	if(_format == CAIRO_FORMAT_A8) {
		pf           = GL_ALPHA;
		_pixelFormat = GL_ALPHA;
	}

	else {
		pf           = GL_RGBA;
		_format      = CAIRO_FORMAT_ARGB32;
		_pixelFormat = GL_BGRA;
	}

	// Call the osg::Image allocation method.
	osg::Image::allocateImage(width, height, 1, pf, GL_UNSIGNED_BYTE);

	if(!osg::Image::valid()) {
		osg::notify(osg::WARN)
			<< "osgCairo::SurfaceImage::allocateImage failed!"
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

	dirty();

	return true;
}

void SurfaceImage::roundedCorners() {	
	Pattern* p = new SolidPattern(0.0f, 0.0f, 0.0f, 1.0f);

	if(!p) return;

	scale(_s, _t);
	setOperator(CAIRO_OPERATOR_DEST_IN);
	setSource(p);
	roundedRectangle(0.01f, 0.01f, 0.98f, 0.98f, 0.075f);
	fill();

	delete p;
}

void SurfaceImage::setOriginBottomLeft() {
	setMatrix(Matrix::translate(0.0f, -_t) * Matrix::scale(1.0f, -1.0f));
}

unsigned int SurfaceImage::getImageSizeInBytes() const {
	return computeRowWidthInBytes(_s, _pixelFormat, _dataType, _packing) * _t * _r;
}

} // namespace osgCairo
