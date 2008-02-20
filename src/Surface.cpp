// -*-c++-*- osgCairo - Copyright (C) 2006 Jeremy Moles

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

//void Surface::closePath() {
//	cairo_close_path(_context);
//}

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

void Surface::roundedRectangle(double x, double y, double w, double h, double r) {
	moveTo(x + r, y);

	lineTo(x + w - r, y);
	curveTo(x + w, y, x + w, y, x + w, y + r);
	
	lineTo(x + w, y + h - r);
	curveTo(x + w, y + h, x + w, y + h, x + w - r, y + h);
	
	lineTo(x + r, y + h) ;
	curveTo(x, y + h, x, y + h, x, y + h - r);
	
	lineTo(x, y + r);
	curveTo(x, y, x, y, x + r, y);
}

} // namespace osgCairo
