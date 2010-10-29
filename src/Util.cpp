// -*-c++-*- osgCairo - Copyright (C) 2010 Jeremy Moles
// $Id$

#include <osg/Math>
#include <osg/Notify>
#include <osgCairo/Util>

namespace osgCairo {
namespace util {

void writeToPNG(cairo_surface_t* surface, const std::string& path) {
#ifdef CAIRO_HAS_PNG_FUNCTIONS
	cairo_surface_write_to_png(surface, path.c_str());
#else
	osg::notify(osg::WARN) << "Your version of Cairo does not support PNG writing!" << std::endl;
#endif
}

bool roundedRectangle(
	cairo_t*            c,
	double              x,
	double              y,
	double              width,
	double              height,
	double              radius
	const cairo_bool_t* corners
) {
	return cairocks_rounded_rectangle(c, x, y, width, height, radius, corners);
}

bool roundedRectangleApply(
	cairo_t*            c,
	double              x,
	double              y,
	double              width,
	double              height,
	double              radius
	const cairo_bool_t* corners
) {
	return cairocks_rounded_rectangle_apply(c, x, y, width, height, radius, corners);
}

std::string cairoFormatAsString(cairo_format_t format) {
	if(format == CAIRO_FORMAT_ARGB32) return "CAIRO_FORMAT_ARGB32";

	else if(format == CAIRO_FORMAT_RGB24) return "CAIRO_FORMAT_RGB24";

	else if(format == CAIRO_FORMAT_A8) return "CAIRO_FORMAT_A8";

	else return "CAIRO_FORMAT_A1";
}

}
}

