// -*-c++-*- osgCairo - Copyright (C) 2006 Jeremy Moles

#include <cstring>
#include <sstream>
#include <osg/Notify>
#include <osgCairo/SurfaceImage>

namespace osgCairo {

SurfaceImage::SurfaceImage():
_allocated(false) {
}

SurfaceImage::SurfaceImage(
	unsigned int         width,
	unsigned int         height,
	const unsigned char* data	
):
_allocated(false) {
	allocateImage(width, height, data);
}

SurfaceImage::SurfaceImage(const SurfaceImage& si, const osg::CopyOp& co):
osg::Image (si, co),
_allocated (si._allocated) {
}

CairoSurface* SurfaceImage::_createSurfaceImplementation() {
	return cairo_image_surface_create_for_data(
		_data,
		CAIRO_FORMAT_ARGB32,
		_s,
		_t,
		_t * 4
	);
}

bool SurfaceImage::allocateImage(
	unsigned int         width,
	unsigned int         height,
	const unsigned char* data
) {
	// Call the osg::Image allocation method.
	osg::Image::allocateImage(width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE);

	if(!osg::Image::valid()) {
		osg::notify(osg::WARN)
			<< "osgCairo::SurfaceImage::allocateImage failed!"
			<< std::endl
		;

		return false;
	}

	// This will have to do for now. :)
	if(data) std::memcpy(_data, data, (width * height) * 4);

	else std::memset(_data, 0, (width * height) * 4);

	_allocated   = true;
	_pixelFormat = GL_BGRA;

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

} // namespace osgCairo
