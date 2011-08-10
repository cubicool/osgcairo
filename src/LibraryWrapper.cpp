// -*-c++-*- osgCairo - Copyright (C) 2011 osgCairo Development Team
// $Id$

#include <osgDB/Registry>
#include <osgDB/ObjectWrapper>
#include <osgDB/InputStream>
#include <osgDB/OutputStream>
#include <osgCairo/Image>

USE_SERIALIZER_WRAPPER(osgCairo_Image)

extern "C" void wrapper_serializer_library_osgCairo(void) {}

REGISTER_OBJECT_WRAPPER(
	osgCairo_Image,
	new osgCairo::Image(),
	osgCairo::Image,
	"osg::Image osgCairo::Image"
) {
}

