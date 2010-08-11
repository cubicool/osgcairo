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

void roundedRectangle(
	cairo_t* c,
	double   x,
	double   y,
	double   width,
	double   height,
	double   radius
) {
	if(cairo_status(c)) return;

	cairo_move_to(c, x + radius, y);
	cairo_line_to(c, x + width - radius, y);

	cairo_arc(
		c,
		x + width - radius,
		y + radius,
		radius,
		-90.0f * osg::PI / 180.0f,
		0.0f * osg::PI / 180.0f
	);

	cairo_line_to(c, x + width, y + height - radius);

	cairo_arc(
		c,
		x + width - radius,
		y + height - radius,
		radius,
		0.0f * osg::PI / 180.0f,
		90.0f * osg::PI / 180.0f
	);

	cairo_line_to(c, x + radius, y + height);

	cairo_arc(
		c,
		x + radius,
		y + height - radius,
		radius,
		90.0f * osg::PI / 180.0f,
		180.0f * osg::PI / 180.0f
	);

	cairo_line_to(c, x, y + radius);

	cairo_arc(
		c,
		x + radius,
		y + radius,
		radius,
		180.0f * osg::PI / 180.0f,
		270.0f * osg::PI / 180.0f
	);
}

void roundedCorners(cairo_t* c, int w, int h, double size, double radius) {
	if(cairo_status(c)) return;

	cairo_pattern_t* p = cairo_pattern_create_rgba(0.0f, 0.0f, 0.0f, 1.0f);

	if(cairo_pattern_status(p)) return;

	cairo_save(c);
	cairo_scale(c, w, h);
	cairo_set_operator(c, CAIRO_OPERATOR_DEST_IN);
	cairo_set_source(c, p);
	
	roundedRectangle(
		c,
		size,
		size,
		1.0f - (size * 2.0f),
		1.0f - (size * 2.0f),
		radius
	);
	
	cairo_fill(c);
	cairo_restore(c);
	cairo_pattern_destroy(p);
}

std::string cairoFormatAsString(cairo_format_t format) {
	if(format == CAIRO_FORMAT_ARGB32) return "CAIRO_FORMAT_ARGB32";

	else if(format == CAIRO_FORMAT_RGB24) return "CAIRO_FORMAT_RGB24";

	else if(format == CAIRO_FORMAT_A8) return "CAIRO_FORMAT_A8";

	else return "CAIRO_FORMAT_A1";
}

}
}

