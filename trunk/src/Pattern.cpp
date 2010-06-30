// -*-c++-*- osgCairo - Copyright (C) 2009 Jeremy Moles
// $Id$

#include <osgCairo/Pattern>

namespace osgCairo {

Pattern::Pattern(CairoPattern* pattern):
_pattern(pattern) {
}

Pattern::~Pattern() {
	cairo_pattern_destroy(_pattern);
}

CairoPattern* Pattern::getPattern() {
	return _pattern;
}

void Pattern::addColorStopRGBA(double offset, double r, double g, double b, double a) {
	cairo_pattern_add_color_stop_rgba(_pattern, offset, r, g, b, a);
}

void Pattern::setExtend(CairoExtend extend) {
	cairo_pattern_set_extend(_pattern, extend);
}

void Pattern::setFilter(CairoFilter filter) {
	cairo_pattern_set_filter(_pattern, filter);
}

CairoStatus Pattern::status() {
	return cairo_pattern_status(_pattern);
}

CairoExtend Pattern::getExtend() {
	return cairo_pattern_get_extend(_pattern);
}

CairoFilter Pattern::getFilter() {
	return cairo_pattern_get_filter(_pattern);
}

SolidPattern::SolidPattern(double r, double g, double b, double a):
Pattern(cairo_pattern_create_rgba(r, g, b, a)) {
}

SurfacePattern::SurfacePattern(CairoSurface* surface):
Pattern(cairo_pattern_create_for_surface(surface)) {
}

LinearPattern::LinearPattern(double x0, double y0, double x1, double y1):
Pattern(cairo_pattern_create_linear(x0, y0, x1, y1)) {
}

RadialPattern::RadialPattern(double x0, double y0, double r0, double x1, double y1, double r1):
Pattern(cairo_pattern_create_radial(x0, y0, r0, x1, y1, r1)) {
}

}
