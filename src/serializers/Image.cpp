// -*-c++-*- osgCairo - Copyright (C) 2011 osgCairo Development Team
// $Id$

#include <osgDB/Registry>
#include <osgDB/ObjectWrapper>
#include <osgDB/InputStream>
#include <osgDB/OutputStream>
#include <osgCairo/Image>
#include <osgCairo/Util>

// ---------------------------------------------------------------------------------------- Surface
static bool checkSurface(const osgCairo::Image& image) {
	return image.getSurface() != 0;
}

static bool readSurface(osgDB::InputStream& is, osgCairo::Image& image) {
	return true;
}

static bool writeSurface(osgDB::OutputStream& os, const osgCairo::Image& image) {
	const std::string format = osgCairo::util::cairoFormatAsString(image.getSurfaceFormat());

	os << format << image.getSurfaceWidth() << image.getSurfaceHeight() << std::endl;
	
	os.setWriteImageHint(osgDB::OutputStream::WRITE_EXTERNAL_FILE);

	return true;
}

REGISTER_OBJECT_WRAPPER(
	osgCairo_Image,
	new osgCairo::Image(),
	osgCairo::Image,
	"osg::Object osg::Image osgCairo::Image"
) {
	ADD_USER_SERIALIZER(Surface);
}

