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
	cairo_paint_with_alpha(cr, 0.5f);
	cairo_translate(cr, dx, dy);
	cairo_set_source(cr, pat);
	cairo_paint_with_alpha(cr, 0.5f);
	
	cairo_pattern_destroy(pat);
	
	return cairo_pop_group(cr);
}

cairo_pattern_t* blur_step(cairo_t* cr, cairo_pattern_t* pat, unsigned int r) {
	pat = displaced_blend(cr, pat, 0, r);
	pat = displaced_blend(cr, pat, r, 0);

	return pat;
}

cairo_pattern_t* displacedBlur(cairo_t* cr, cairo_pattern_t* pat, unsigned int n, unsigned int r) {
	/*
	cairo_matrix_t matrix;

	cairo_matrix_init_translate(&matrix, n / 2.0f, n / 2.0f);
	
	cairo_pattern_set_matrix(pat, &matrix);
	*/

	while(n-- > 0) pat = blur_step(cr, pat, r);

	return pat;
}

}
}

