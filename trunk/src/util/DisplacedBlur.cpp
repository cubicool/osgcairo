// -*-c++-*- osgCairo - Copyright (C) 2010 Jeremy Moles
// $Id$

#include <osgCairo/Util>

namespace osgCairo {
namespace util {

// This code was given to me by M. Joonas Pihlaja <jpihlaja@cc.helsinki.fi>.

cairo_pattern_t* displaced_blend(
	cairo_t*         cr,
	cairo_pattern_t* pat,
	double           dx,
	double           dy
) {
	cairo_push_group(cr);
	cairo_set_operator(cr, CAIRO_OPERATOR_ADD);
	cairo_set_source(cr, pat);
	cairo_paint_with_alpha(cr, 0.5);
	cairo_translate(cr, dx, dy);
	cairo_set_source(cr, pat);
	cairo_paint_with_alpha(cr, 0.5);
	cairo_pattern_destroy(pat);
	
	return cairo_pop_group(cr);
}

cairo_pattern_t* blur_step(cairo_t* cr, cairo_pattern_t* pat) {
	pat = displaced_blend(cr, pat, 0, 1);
	pat = displaced_blend(cr, pat, 1, 0);

	return pat;
}

cairo_pattern_t* displacedBlur(cairo_t* cr, cairo_pattern_t* pat, unsigned int n) {
	while(n-- > 0) pat = blur_step(cr, pat);

	return pat;
}

/*
	pat = blurred_pattern(cr, pat, num_blurs);

	cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
	cairo_set_source(cr, pat);
	cairo_paint(cr);

	cairo_pattern_destroy(pat);
*/

}
}

