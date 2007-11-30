// -*-c++-*- osgCairo - Copyright (C) 2006 Jeremy Moles

#include <cstring>
#include <sstream>
#include <osg/Notify>
#include <osgCairo/SurfaceImage>

namespace osgCairo {

unsigned int SurfaceImage::_count = 0;

std::string SurfaceImage::_generateName() {
	std::stringstream ss;

	ss << "SurfaceImage" << _count;
	
	_count++;

	return ss.str();
}

SurfaceImage::SurfaceImage(const std::string& name):
_allocated(false) {
	_name     = name.size() ? name : _generateName();
	_fileName = name;
}

bool SurfaceImage::allocateCairo(
	unsigned int         width,
	unsigned int         height,
	const unsigned char* data,
	bool                 flip
) {
	allocateImage(width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE);

	if(not osg::Image::valid()) {
		osg::notify(osg::WARN)
			<< "osgCairo::SurfaceImage::allocateImage failed!"
			<< std::endl
		;

		return false;
	}

	// This will have to do for now. :)
	if(data) std::memcpy(_data, data, (width * height) * 4);

	else std::memset(_data, 0, (width * height) * 4);

	if(not createContext(width, height, _data)) {
		osg::notify(osg::WARN)
			<< "osgCairo::Surface::createContext failed!"
			<< std::endl
		;

		return false;
	}

	_allocated   = true;
	_pixelFormat = GL_BGRA;

	if(flip) flipVertical();
	
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

} 
