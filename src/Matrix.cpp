// -*-c++-*- osgCairo - Copyright (C) 2009 Jeremy Moles

#include <osgCairo/Matrix>

namespace osgCairo {

Matrix::Matrix(double xx, double yx, double xy, double yy, double x0, double y0) {
	cairo_matrix_init(this, xx, yx, xy, yy, x0, y0);
}

Matrix::Matrix(const CairoMatrix& cm):
CairoMatrix(cm) {
}

Matrix::~Matrix() {
}

Matrix Matrix::operator=(const Matrix& rhs) {
	if(&rhs == this) return *this;

	cairo_matrix_init(this, rhs.xx, rhs.yx, rhs.xy, rhs.yy, rhs.x0, rhs.y0);
	
	return *this;
}

Matrix Matrix::operator*(const Matrix& rhs) {
	Matrix m;

	cairo_matrix_multiply(&m, this, &rhs);

	return m;
}

void Matrix::setTranslate(double tx, double ty) {
	cairo_matrix_translate(this, tx, ty);
}

void Matrix::setScale(double tx, double ty) {
	cairo_matrix_scale(this, tx, ty);
}

void Matrix::setRotate(double rad) {
	cairo_matrix_rotate(this, rad);
}

void Matrix::invert() {
	cairo_matrix_invert(this);
}

void Matrix::transformDistance(double* dx, double* dy) {
	cairo_matrix_transform_distance(this, dx, dy);
}

void Matrix::transformPoint(double* x, double* y) {
	cairo_matrix_transform_point(this, x, y);
}

Matrix Matrix::identity() {
	Matrix m;

	cairo_matrix_init_identity(&m);

	return m;
}

Matrix Matrix::translate(double tx, double ty) {
	Matrix m;

	cairo_matrix_init_translate(&m, tx, ty);

	return m;
}

Matrix Matrix::scale(double sx, double sy) {
	Matrix m;

	cairo_matrix_init_scale(&m, sx, sy);

	return m;
}

Matrix Matrix::rotate(double rad) {
	Matrix m;

	cairo_matrix_init_rotate(&m, rad);

	return m;
}

}
