// -*-c++-*- Copyright (C) 2011 osgCairo Development Team
// $Id$

#include <osgDB/Registry>
#include <osgDB/ObjectWrapper>
#include <osgCairo/Image>

extern "C" void wrapper_serializer_library_osgCairo(void) {
}

REGISTER_OBJECT_WRAPPER(
	osgCairo_Image,
	new osgCairo::Image(),
	osgCairo::Image,
	"osg::Object osg::Image osgCairo::Image"
) {
}

