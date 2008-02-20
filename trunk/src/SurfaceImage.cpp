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

	/*
	osg::notify() 
		<< "- Name:        " << _name                        << std::endl
		<< "- Size:        " << _s << " " << _t << " " << _r << std::endl
		<< "- TexFormat:   " << _internalTextureFormat       << std::endl
		<< "- PixelFormat: " << _pixelFormat                 << std::endl
		<< "- DataType:    " << _dataType                    << std::endl
		<< "- Packing:     " << _packing                     << std::endl
		<< "- Allocation:  " << _allocationMode              << std::endl
		<< "- Modified:    " << _modifiedCount               << std::endl
	;
	*/

	return true;
}

} 
