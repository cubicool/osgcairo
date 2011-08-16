// -*-c++-*- osgCairo - Copyright (C) 2011 osgCairo Development Team
// $Id$

#include <osg/Math>
#include <osgDB/FileUtils>
#include <osgCairo/Notify>
#include <osgCairo/Image>
#include <osgCairo/Util>

#include "cairocks.h"

namespace osgCairo {
namespace util {

std::string cairoFormatAsString(cairo_format_t format) {
	if(format == CAIRO_FORMAT_ARGB32) return "CAIRO_FORMAT_ARGB32";

	else if(format == CAIRO_FORMAT_RGB24) return "CAIRO_FORMAT_RGB24";

	else if(format == CAIRO_FORMAT_A8) return "CAIRO_FORMAT_A8";

	else return "CAIRO_FORMAT_A1";
}

cairo_format_t stringAsCairoFormat(const std::string& format) {
	if(format == "CAIRO_FORMAT_ARGB32") return CAIRO_FORMAT_ARGB32;

	else if(format == "CAIRO_FORMAT_RGB24") return CAIRO_FORMAT_RGB24;

	else if(format == "CAIRO_FORMAT_A8") return CAIRO_FORMAT_A8;

	else return CAIRO_FORMAT_A1;
}

bool writeToPNG(cairo_surface_t* surface, const std::string& path) {
#ifdef CAIRO_HAS_PNG_FUNCTIONS
	cairo_surface_write_to_png(surface, path.c_str());

	return true;
#else
	OSGCAIRO_WARN("util::writeToPNG")
		<< "Your version of Cairo does not support PNG writing!"
		<< std::endl
	;

	return false;
#endif
}

Image* readFromPNG(const std::string& path) {
	osgCairo::Image* image = 0;

#ifdef CAIRO_HAS_PNG_FUNCTIONS
	if(!osgDB::fileExists(path)) return image;

	cairo_surface_t* surface = cairo_image_surface_create_from_png(path.c_str());

	if(!surface) return image;

	image = new osgCairo::Image();

	if(!image->allocateSurface(
		cairo_image_surface_get_width(surface),
		cairo_image_surface_get_height(surface),
		cairo_image_surface_get_format(surface),
		cairo_image_surface_get_data(surface)
	)) {
		OSGCAIRO_WARN("util::readFromPNG")
			<< "Couldn't allocate new Image."
			<< std::endl
		;
	}

	cairo_surface_destroy(surface);

#else
	OSGCAIRO_WARN("util::readFromPNG")
		<< "Your version of Cairo does not support PNG reading!"
		<< std::endl
	;
#endif

	return image;
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
	double           elevation,
	double           height,
	double           ambient,
	double           diffuse
) {
	return cairocks_emboss_create(surface, azimuth, elevation, height, ambient, diffuse);
}

cairo_surface_t* createDistanceField(
	cairo_surface_t* surface,
	int              scan_size,
	int              block_size
) {
	return cairocks_distance_field_create(surface, scan_size, block_size);
}

}
}

