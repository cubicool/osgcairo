import osgcairoutil

	cairo_push_group(c);

	cairo_set_line_width(c, lineWidth);
	cairo_move_to(c, S / 2.0f, 0.0f);
	cairo_line_to(c, S / 2.0f, S);
	cairo_stroke(c);

	cairo_pattern_t* pattern = displacedBlur(c, cairo_pop_group(c), numBlurs);

	cairo_set_source(c, pattern);
	cairo_paint(c);

	cairo_set_line_width(c, 1.0f);
	cairo_set_source_rgb(c, 1.0f, 0.0f, 0.0f);
	cairo_move_to(c, S / 2.0f, 0.0f);
	cairo_line_to(c, S / 2.0f, S);
	cairo_stroke(c);


