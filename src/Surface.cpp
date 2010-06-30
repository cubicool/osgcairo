// -*-c++-*- osgCairo - Copyright (C) 2009 Jeremy Moles
// $Id$

#include <osgCairo/Surface>

namespace osgCairo {

Surface::Surface():
_surface (0),
_context (0) {
}

Surface::~Surface() {
	cairo_surface_destroy(_surface);
	cairo_destroy(_context);
}

bool Surface::createContext() {
	_surface = _createSurfaceImplementation();
	
	if(!_surface) return false;

	_context = cairo_create(_surface);

	if(!_context || status()) return false;
	
	return true;
}

CairoSurface* Surface::getSurface() {
	return _surface;
}

CairoContext* Surface::getContext() {
	return _context;
}

CairoStatus Surface::status() {
	return cairo_status(_context);
}

const char* Surface::statusToString() {
	return cairo_status_to_string(status());
}

CairoFillRule Surface::getFillRule() {
	return cairo_get_fill_rule(_context);
}

CairoLineCap Surface::getLineCap() {
	return cairo_get_line_cap(_context);
}

CairoLineJoin Surface::getLineJoin() {
	return cairo_get_line_join(_context);
}

CairoOperator Surface::getOperator() {
	return cairo_get_operator(_context);
}

double Surface::getLineWidth() {
	return cairo_get_line_width(_context);
}

double Surface::getMiterLimit() {
	return cairo_get_miter_limit(_context);
}

double Surface::getTolerance() {
	return cairo_get_tolerance(_context);
}

CairoScaledFont* Surface::getScaledFont() {
	return cairo_get_scaled_font(_context);
}

bool Surface::inFill(double x, double y) {
	return cairo_in_fill(_context, x, y);
}

bool Surface::inStroke(double x, double y) {
	return cairo_in_stroke(_context, x, y);
}

void Surface::save() {
	cairo_save(_context);
}

void Surface::restore() {
	cairo_restore(_context);
}

void Surface::setSource(Pattern* pattern) {
	cairo_set_source(_context, pattern->getPattern());
}

void Surface::setSourceRGBA(double r, double g, double b, double a) {
	cairo_set_source_rgba(_context, r, g, b, a);
}

void Surface::setSourceSurface(Surface* surface, double x, double y) {
	cairo_set_source_surface(_context, surface->getSurface(), x, y);
}

void Surface::setAntialias(CairoAntialias aa) {
	cairo_set_antialias(_context, aa);
}

//void Surface::setDash(double* dashes, int num, double offset) {
//	cairo_set_dash(_context, dashes, num, offset);
//}

void Surface::setFillRule(CairoFillRule rule) {
	cairo_set_fill_rule(_context, rule);
}

void Surface::setLineCap(CairoLineCap cap) {
	cairo_set_line_cap(_context, cap);
}

void Surface::setLineJoin(CairoLineJoin join) {
	cairo_set_line_join(_context, join);
}

void Surface::setLineWidth(double width) {
	cairo_set_line_width(_context, width);
}

void Surface::setMiterLimit(double limit) {
	cairo_set_miter_limit(_context, limit);
}

void Surface::setOperator(CairoOperator op) {
	cairo_set_operator(_context, op);
}

void Surface::setTolerance(double tolerance) {
	cairo_set_tolerance(_context, tolerance);
}

void Surface::setScaledFont(CairoScaledFont* scaledFont) {
	cairo_set_scaled_font(_context, scaledFont);
}

void Surface::clip() {
	cairo_clip(_context);
}

void Surface::clipPreserve() {
	cairo_clip_preserve(_context);
}

void Surface::resetClip() {
	cairo_reset_clip(_context);
}

void Surface::fill() {
	cairo_fill(_context);
}

void Surface::fillPreserve() {
	cairo_fill_preserve(_context);
}

void Surface::fillExtents(double& x1, double& y1, double& x2, double& y2) {
	cairo_fill_extents(_context, &x1, &y1, &x2, &y2);
}

//void Surface::mask(CairoPattern* mask) {
//	cairo_mask(_context, mask);
//}

//void Surface::maskSurface(CairoSurface* surface, double x, double y) {
//	cairo_mask_surface(_context, surface, x, y);
//}

void Surface::paint() {
	cairo_paint(_context);
}

void Surface::paintWithAlpha(double alpha) {
	cairo_paint_with_alpha(_context, alpha);
}

void Surface::stroke() {
	cairo_stroke(_context);
}

void Surface::strokePreserve() {
	cairo_stroke_preserve(_context);
}

void Surface::strokeExtents(double& x1, double& y1, double& x2, double& y2) {
	cairo_stroke_extents(_context, &x1, &y1, &x2, &y2);
}

void Surface::showGlyphs(const GlyphList& gl) {
	cairo_show_glyphs(_context, &gl.front(), gl.size());
}

void Surface::showGlyphs(const Glyph& glyph) {
	cairo_show_glyphs(_context, &glyph, 1);
}

void Surface::glyphPath(const GlyphList& gl) {
	cairo_glyph_path(_context, &gl.front(), gl.size());
}

void Surface::glyphPath(const Glyph& glyph) {
	cairo_glyph_path(_context, &glyph, 1);
}

void Surface::writeToPNG(const std::string& filename) {
	cairo_surface_write_to_png(_surface, filename.c_str());
}

// Path stuff...

Path Surface::copyPath() {
	return cairo_copy_path(_context);
}

Path Surface::copyPathFlat() {
	return cairo_copy_path_flat(_context);
}

void Surface::appendPath(Path path) {
	cairo_append_path(_context, path._path);
}

void Surface::getCurrentPoint(double& x, double& y) {
	cairo_get_current_point(_context, &x, &y);
}

//void Surface::newPath() {
//	cairo_new_path(_context);
//}

//void Surface::newSubPath() {
//	cairo_new_sub_path(_context);
//}

void Surface::closePath() {
	cairo_close_path(_context);
}

void Surface::arc(double x, double y, double rad, double a1, double a2) {
	cairo_arc(_context, x, y, rad, a1, a2);
}

void Surface::arcNegative(double x, double y, double rad, double a1, double a2) {
	cairo_arc_negative(_context, x, y, rad, a1, a2);
}

void Surface::curveTo(double x1, double y1, double x2, double y2, double x3, double y3) {
	cairo_curve_to(_context, x1, y1, x2, y2, x3, y3);
}

void Surface::lineTo(double x, double y) {
	cairo_line_to(_context, x, y);
}

void Surface::moveTo(double x, double  y) {
	cairo_move_to(_context, x, y);
}

void Surface::rectangle(double x, double y, double width, double height) {
	cairo_rectangle(_context, x, y, width, height);
}

//void Surface::glyphPath(CairoGlyph* glyphs, int num) {
//	cairo_glyph_path(_context, glyphs, num);
//}

void Surface::textPath(const std::string& path) {
	cairo_text_path(_context, path.c_str());
}

void Surface::relCurveTo(double x1, double y1, double x2, double y2, double x3, double y3) {
	cairo_rel_curve_to(_context, x1, y1, x2, y2, x3, y3);
}

void Surface::relLineTo(double x, double y) {
	cairo_rel_line_to(_context, x, y);
}

void Surface::relMoveTo(double x, double y) {
	cairo_rel_move_to(_context, x, y);
}

void Surface::translate(double tx, double ty) {
	cairo_translate(_context, tx, ty);
}

void Surface::scale(double sx, double sy) {
	cairo_scale(_context, sx, sy);
}

void Surface::rotate(double rad) {
	cairo_rotate(_context, rad);
}

void Surface::transform(const Matrix& mat) {
	cairo_transform(_context, &mat);
}

void Surface::setMatrix(const Matrix& mat) {
	cairo_set_matrix(_context, &mat);
}

void Surface::setFontMatrix(const Matrix& mat) {
	cairo_set_font_matrix(_context, &mat);
}

void Surface::identityMatrix() {
	cairo_identity_matrix(_context);
}

void Surface::userToDevice(double& x, double& y) {
	cairo_user_to_device(_context, &x, &y);
}

void Surface::userToDeviceDistance(double& x, double& y) {
	cairo_user_to_device_distance(_context, &x, &y);
}

void Surface::deviceToUser(double& x, double& y) {
	cairo_device_to_user(_context, &x, &y);
}

void Surface::deviceToUserDistance(double& x, double& y) {
	cairo_device_to_user_distance(_context, &x, &y);
}

Matrix Surface::getMatrix() {
	Matrix m;

	cairo_get_matrix(_context, &m);

	return m;
}

void Surface::roundedRectangle(
	double x,
	double y,
	double width,
	double height,
	double radius
) {
	moveTo(x + radius, y);
	lineTo(x + width - radius, y);

	arc(
		x + width - radius,
		y + radius,
		radius,
		-90.0f * osg::PI / 180.0f,
		0.0f * osg::PI / 180.0f
	);

	lineTo(x + width, y + height - radius);

	arc(
		x + width - radius,
		y + height - radius,
		radius,
		0.0f * osg::PI / 180.0f,
		90.0f * osg::PI / 180.0f
	);

	lineTo(x + radius, y + height);

	arc(
		x + radius,
		y + height - radius,
		radius,
		90.0f * osg::PI / 180.0f,
		180.0f * osg::PI / 180.0f
	);

	lineTo(x, y + radius);

	arc(
		x + radius,
		y + radius,
		radius,
		180.0f * osg::PI / 180.0f,
		270.0f * osg::PI / 180.0f
	);
}

void Surface::roundedCorners(double width, double height) {
	SolidPattern p(0.0f, 0.0f, 0.0f, 1.0f);

	save();
	scale(width, height);
	setOperator(CAIRO_OPERATOR_DEST_IN);
	setSource(&p);
	roundedRectangle(0.01f, 0.01f, 0.98f, 0.98f, 0.075f);
	fill();
	restore();
}

ImageSurface::ImageSurface(CairoFormat format, int width, int height):
_format (format),
_width  (width),
_height (height) {
}

CairoSurface* ImageSurface::_createSurfaceImplementation() {
	return createImageSurface(_format, _width, _height);
}

bool ImageSurface::valid() const {
	return(cairo_surface_status(_surface) == CAIRO_STATUS_SUCCESS);
}

unsigned char* ImageSurface::getData() {
	if(valid()) return cairo_image_surface_get_data(_surface);

	else return 0;
}

CairoSurface* createImageSurface(CairoFormat format, int width, int height) {
	return cairo_image_surface_create(format, width, height);
}

CairoSurface* createImageSurfaceForData(
	unsigned char* data,
	CairoFormat    format,
	int            width,
	int            height
) {
	return cairo_image_surface_create_for_data(
		data,
		format,
		width,
		height,
		cairo_format_stride_for_width(format, width)
	);
}

// This code was written by the fantastic Behdad Esfahbod and adapated by Jeremy Moles
// from the cairotwisted.c file in Pango. Thanks Behdad!

// Returns Euclidean distance between two points
double two_points_distance(cairo_path_data_t* a, cairo_path_data_t* b) {
	double dx, dy;

	dx = b->point.x - a->point.x;
	dy = b->point.y - a->point.y;

	return sqrt(dx * dx + dy * dy);
}

// Returns length of a Bezier curve. Seems like computing that analytically is not easy. The
// code just flattens the curve using cairo and adds the length of segments.
double curve_length(
	double x0, double y0,
	double x1, double y1,
	double x2, double y2,
	double x3, double y3
) {
	cairo_surface_t*   surface = cairo_image_surface_create(CAIRO_FORMAT_A8, 0, 0);
	cairo_t*           cr      = cairo_create(surface);
	cairo_path_t*      path;
	cairo_path_data_t* data, current_point;
	double             length = 0;

	cairo_surface_destroy(surface);
	cairo_move_to(cr, x0, y0);
	cairo_curve_to(cr, x1, y1, x2, y2, x3, y3);

	path = cairo_copy_path_flat(cr);

	for(int i = 0; i < path->num_data; i += path->data[i].header.length) {
		data = &path->data[i];

		switch (data->header.type) {
		case CAIRO_PATH_MOVE_TO:
			current_point = data[1];

			break;

		case CAIRO_PATH_LINE_TO:
			length        += two_points_distance(&current_point, &data[1]);
			current_point  = data[1];

			break;

		default:
			break;
		}
	}

	cairo_path_destroy(path);
	cairo_destroy(cr);

	return length;
}

typedef double parametrization_t;

// Compute parametrization info.  That is, for each part of the cairo path, tags it with
// its length.
parametrization_t* parametrize_path(cairo_path_t* path) {
	cairo_path_data_t* data, last_move_to, current_point;
	parametrization_t* parametrization;

	// parametrization = (parametrization_t*)malloc(path->num_data * sizeof(parametrization[0]));
	// parametrization = new parametrization_t[(path->num_data * sizeof(parametrization[0]))];
	
	parametrization = new parametrization_t[path->num_data];

	for (int i = 0; i < path->num_data; i += path->data[i].header.length) {
		data               = &path->data[i];
		parametrization[i] = 0.0;

		switch(data->header.type) {
		case CAIRO_PATH_MOVE_TO:
			last_move_to  = data[1];
			current_point = data[1];

			break;

		case CAIRO_PATH_CLOSE_PATH:
			// Make it look like it's a line_to to last_move_to
			data = (&last_move_to) - 1;
			
			// fall through

		case CAIRO_PATH_LINE_TO:
			parametrization[i] = two_points_distance(&current_point, &data[1]);
			current_point      = data[1];

			break;

		case CAIRO_PATH_CURVE_TO:
			// naive curve-length, treating bezier as three line segments:
			// parametrization[i] = two_points_distance (&current_point, &data[1])
			// + two_points_distance (&data[1], &data[2])
			// + two_points_distance (&data[2], &data[3]);
			parametrization[i] = curve_length(
				current_point.point.x, current_point.point.x,
				data[1].point.x, data[1].point.y,
				data[2].point.x, data[2].point.y,
				data[3].point.x, data[3].point.y
			);

			current_point = data[3];

			break;
		}
	}

	return parametrization;
}

typedef void (*transform_point_func_t) (void *closure, double *x, double *y);

// Project a path using a function.  Each point of the path (including  Bezier control
// points) is passed to the function for transformation.
void transform_path(cairo_path_t* path, transform_point_func_t f, void* closure) {
	cairo_path_data_t* data;

	for(int i = 0; i < path->num_data; i += path->data[i].header.length) {
		data = &path->data[i];

		switch(data->header.type) {
		case CAIRO_PATH_CURVE_TO:
			f(closure, &data[3].point.x, &data[3].point.y);
			f(closure, &data[2].point.x, &data[2].point.y);

		case CAIRO_PATH_MOVE_TO:
		case CAIRO_PATH_LINE_TO:
			f(closure, &data[1].point.x, &data[1].point.y);

			break;

		case CAIRO_PATH_CLOSE_PATH:
			break;
		}
	}
}

// Simple struct to hold a path and its parametrization
struct parametrized_path_t {
	cairo_path_t*      path;
	parametrization_t* parametrization;
};

// Project a point X,Y onto a parameterized path.  The final point is
// where you get if you walk on the path forward from the beginning for X
// units, then stop there and walk another Y units perpendicular to the
// path at that point.  In more detail:
//
// There's three pieces of math involved:
//
//   - The parametric form of the Line equation
//     http://en.wikipedia.org/wiki/Line
//
//   - The parametric form of the Cubic Bézier curve equation
//     http://en.wikipedia.org/wiki/B%C3%A9zier_curve
//
//   - The Gradient (aka multi-dimensional derivative) of the above
//     http://en.wikipedia.org/wiki/Gradient
//
// The parametric forms are used to answer the question of "where will I be
// if I walk a distance of X on this path".  The Gradient is used to answer
// the question of "where will I be if then I stop, rotate left for 90
// degrees and walk straight for a distance of Y".
void point_on_path(parametrized_path_t* param, double* x, double* y) {
	double ratio, the_y = *y, the_x = *x, dx, dy;
	cairo_path_data_t* data, last_move_to, current_point;
	cairo_path_t*      path = param->path;
	parametrization_t* parametrization = param->parametrization;

	int i;

	for(i = 0; i + path->data[i].header.length < path->num_data &&
			(the_x > parametrization[i] ||
			 path->data[i].header.type == CAIRO_PATH_MOVE_TO);
			i += path->data[i].header.length
		) {
		
		the_x -= parametrization[i];
		data   = &path->data[i];

		switch(data->header.type) {
		case CAIRO_PATH_MOVE_TO:
			current_point = data[1];
			last_move_to  = data[1];

			break;

		case CAIRO_PATH_LINE_TO:
			current_point = data[1];

			break;

		case CAIRO_PATH_CURVE_TO:
			current_point = data[3];

			break;

		case CAIRO_PATH_CLOSE_PATH:
			break;
		}
	}

	data = &path->data[i];

	switch(data->header.type) {
	case CAIRO_PATH_MOVE_TO:
		break;

	case CAIRO_PATH_CLOSE_PATH:
		// Make it look like it's a line_to to last_move_to
		data = (&last_move_to) - 1;

		// fall through

	case CAIRO_PATH_LINE_TO:
		ratio = the_x / parametrization[i];
		// Line polynomial
		*x = current_point.point.x * (1 - ratio) + data[1].point.x * ratio;
		*y = current_point.point.y * (1 - ratio) + data[1].point.y * ratio;

		// Line gradient
		dx = -(current_point.point.x - data[1].point.x);
		dy = -(current_point.point.y - data[1].point.y);

		// optimization for: ratio = the_y / sqrt (dx * dx + dy * dy);
		ratio  = the_y / parametrization[i];
		*x    += -dy * ratio;
		*y    += dx * ratio;

		break;

	case CAIRO_PATH_CURVE_TO:
		// FIXME the formulas here are not exactly what we want, because the
		// Bezier parametrization is not uniform.  But I don't know how to do
		// better.  The caller can do slightly better though, by flattening the
		// Bezier and avoiding this branch completely.  That has its own cost
		// though, as large y values magnify the flattening error drastically.

		double ratio_1_0, ratio_0_1;
		double ratio_2_0, ratio_0_2;
		double ratio_3_0, ratio_2_1, ratio_1_2, ratio_0_3;
		double _1__4ratio_1_0_3ratio_2_0, _2ratio_1_0_3ratio_2_0;

		ratio = the_x / parametrization[i];

		ratio_1_0 = ratio;
		ratio_0_1 = 1 - ratio;

		ratio_2_0 = ratio_1_0 * ratio_1_0; //      ratio  *      ratio
		ratio_0_2 = ratio_0_1 * ratio_0_1; // (1 - ratio) * (1 - ratio)

		ratio_3_0 = ratio_2_0 * ratio_1_0; //      ratio  *      ratio  *      ratio
		ratio_2_1 = ratio_2_0 * ratio_0_1; //      ratio  *      ratio  * (1 - ratio)
		ratio_1_2 = ratio_1_0 * ratio_0_2; //      ratio  * (1 - ratio) * (1 - ratio)
		ratio_0_3 = ratio_0_1 * ratio_0_2; // (1 - ratio) * (1 - ratio) * (1 - ratio)

		_1__4ratio_1_0_3ratio_2_0 = 1 - 4 * ratio_1_0 + 3 * ratio_2_0;
		_2ratio_1_0_3ratio_2_0    =     2 * ratio_1_0 - 3 * ratio_2_0;

		// Bezier polynomial
		*x = current_point.point.x * ratio_0_3
			+ 3 * data[1].point.x * ratio_1_2
			+ 3 * data[2].point.x * ratio_2_1
			+     data[3].point.x * ratio_3_0
		;

		*y = current_point.point.y * ratio_0_3
			+ 3 * data[1].point.y * ratio_1_2
			+ 3 * data[2].point.y * ratio_2_1
			+     data[3].point.y * ratio_3_0
		;

		// Bezier gradient
		dx =-3 * current_point.point.x * ratio_0_2
			+ 3 * data[1].point.x * _1__4ratio_1_0_3ratio_2_0
			+ 3 * data[2].point.x * _2ratio_1_0_3ratio_2_0
			+ 3 * data[3].point.x * ratio_2_0
		;

		dy =-3 * current_point.point.y * ratio_0_2
			+ 3 * data[1].point.y * _1__4ratio_1_0_3ratio_2_0
			+ 3 * data[2].point.y * _2ratio_1_0_3ratio_2_0
			+ 3 * data[3].point.y * ratio_2_0
		;

		ratio  = the_y / sqrt(dx * dx + dy * dy);
		*x    += -dy * ratio;
		*y    += dx * ratio;
		
		break;
	}
}

// Projects the current path of cr onto the provided path.
void map_path_onto(cairo_t* cr, cairo_path_t* path) {
	parametrized_path_t param;

	param.path            = path;
	param.parametrization = parametrize_path(path);

	cairo_path_t* current_path = cairo_copy_path(cr);

	cairo_new_path(cr);
	transform_path(current_path, (transform_point_func_t)(point_on_path), &param);
	cairo_append_path(cr, current_path);

	delete[] param.parametrization;

	cairo_path_destroy(current_path);
}

}
