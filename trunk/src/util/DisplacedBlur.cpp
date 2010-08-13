// -*-c++-*- osgCairo - Copyright (C) 2010 Jeremy Moles
// $Id$

#include <osgCairo/Util>

namespace osgCairo {
namespace util {

// This code was given to me by M. Joonas Pihlaja <jpihlaja@cc.helsinki.fi>.

cairo_pattern_t* displaced_blend(
	cairo_t*         c,
	cairo_pattern_t* pat,
	double           x,
	double           y
) {
	cairo_push_group(c);
	cairo_set_operator(c, CAIRO_OPERATOR_ADD);
	cairo_set_source(c, pat);
	cairo_paint_with_alpha(c, 0.5f);
	cairo_translate(c, x, y);
	cairo_set_source(c, pat);
	cairo_paint_with_alpha(c, 0.5f);
	
	cairo_pattern_destroy(pat);
	
	return cairo_pop_group(c);
}

cairo_pattern_t* blur_step(cairo_t* c, cairo_pattern_t* pat) {
	pat = displaced_blend(c, pat, 0, 1);
	pat = displaced_blend(c, pat, 1, 0);

	return pat;
}

cairo_pattern_t* displacedBlur(cairo_t* c, cairo_pattern_t* pat, unsigned int n) {
	cairo_matrix_t matrix;
	
	cairo_matrix_init_translate(&matrix, n / 2.0f, n / 2.0f);
	cairo_pattern_set_matrix(pat, &matrix);
	
	while(n-- > 0) pat = blur_step(c, pat);

	return pat;
}

}
}

