// -*-c++-*- osgCairo - Copyright (C) 2006 Jeremy Moles

#include <cstring>
#include <sstream>
#include <osg/Notify>
#include <osgCairo/SurfaceImage>

namespace osgCairo {

SurfaceImage::SurfaceImage(const std::string& name):
_allocated(false) {
	_name     = name;
	_fileName = name;
}

SurfaceImage::SurfaceImage(const SurfaceImage& si, const osg::CopyOp& co):
osg::Image (si, co),
_allocated (si._allocated) {
}

bool SurfaceImage::allocateCairo(
	unsigned int         width,
	unsigned int         height,
	const unsigned char* data
) {
	allocateImage(width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE);

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

	if(!createContext(width, height, _data)) {
		osg::notify(osg::WARN)
			<< "osgCairo::Surface::createContext failed!"
			<< std::endl
		;

		return false;
	}

	_allocated   = true;
	_pixelFormat = GL_BGRA;

	flipVertical();
	dirty();
	
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

	return true;
}

bool SurfaceImage::roundedRectangle(double x, double y, double w, double h, double r) {
	if(!_allocated) return false;

	moveTo(x + r, y);

	lineTo(x + w - r, y);
	curveTo(x + w, y, x + w, y, x + w, y + r);
	
	lineTo(x + w, y + h - r);
	curveTo(x + w, y + h, x + w, y + h, x + w - r, y + h);
	
	lineTo(x + r, y + h) ;
	curveTo(x, y + h, x, y + h, x, y + h - r);
	
	lineTo(x, y + r);
	curveTo(x, y, x, y, x + r, y);

	return true;
}

} 
