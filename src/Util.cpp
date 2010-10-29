// -*-c++-*- osgCairo - Copyright (C) 2010 Jeremy Moles
// $Id$

#include <osg/Math>
#include <osg/Notify>
#include <osgCairo/Util>

#include "cairocks.h"

namespace osgCairo {
namespace util {

bool writeToPNG(cairo_surface_t* surface, const std::string& path) {
#ifdef CAIRO_HAS_PNG_FUNCTIONS
	cairo_surface_write_to_png(surface, path.c_str());

	return true;
#else
	osg::notify(osg::WARN) << "Your version of Cairo does not support PNG writing!" << std::endl;

	return false;
#endif
}

bool roundedRectangle(
	cairo_t*       c,
	double         x,
	double         y,
	double         width,
	double         height,
	double         radius,
	const Corners& corners
) {
	return cairocks_rounded_rectangle(c, x, y, width, height, radius, corners.corners) != FALSE;
}

bool roundedRectangleApply(
	cairo_t*       c,
	double         x,
	double         y,
	double         width,
	double         height,
	double         radius,
	const Corners& corners
) {
	return cairocks_rounded_rectangle_apply(c, x, y, width, height, radius, corners.corners) != FALSE;
}

bool gaussianBlur(cairo_surface_t* surface, double radius, double deviation) {
	return cairocks_gaussian_blur(surface, radius, deviation) != FALSE;
}

bool mapPathOnto(cairo_t* c, cairo_path_t* path) {
	return cairocks_map_path_onto(c, path) != FALSE;
}

cairo_surface_t* createEmbossedSurface(
	cairo_surface_t* surface,
	double           azimuth,
	double           elevation
) {
	return cairocks_create_embossed_surface(surface, azimuth, elevation);
}

std::string cairoFormatAsString(cairo_format_t format) {
	if(format == CAIRO_FORMAT_ARGB32) return "CAIRO_FORMAT_ARGB32";

	else if(format == CAIRO_FORMAT_RGB24) return "CAIRO_FORMAT_RGB24";

	else if(format == CAIRO_FORMAT_A8) return "CAIRO_FORMAT_A8";

	else return "CAIRO_FORMAT_A1";
}

}
}

