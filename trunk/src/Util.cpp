// -*-c++-*- osgCairo - Copyright (C) 2010 Jeremy Moles
// $Id$

#include <cmath>
#include <osgCairo/Util>

namespace osgCairo {
namespace util {

void roundedRectangle(
	Surface* surface,
	double   x,
	double   y,
	double   width,
	double   height,
	double   radius
) {
	if(!surface || surface->status()) return;

	surface->moveTo(x + radius, y);
	surface->lineTo(x + width - radius, y);

	surface->arc(
		x + width - radius,
		y + radius,
		radius,
		-90.0f * osg::PI / 180.0f,
		0.0f * osg::PI / 180.0f
	);

	surface->lineTo(x + width, y + height - radius);

	surface->arc(
		x + width - radius,
		y + height - radius,
		radius,
		0.0f * osg::PI / 180.0f,
		90.0f * osg::PI / 180.0f
	);

	surface->lineTo(x + radius, y + height);

	surface->arc(
		x + radius,
		y + height - radius,
		radius,
		90.0f * osg::PI / 180.0f,
		180.0f * osg::PI / 180.0f
	);

	surface->lineTo(x, y + radius);

	surface->arc(
		x + radius,
		y + radius,
		radius,
		180.0f * osg::PI / 180.0f,
		270.0f * osg::PI / 180.0f
	);
}

void roundedCorners(Surface* surface, double size, double radius) {
	if(!surface || surface->status()) return;

	SolidPattern p(0.0f, 0.0f, 0.0f, 1.0f);

	surface->save();
	surface->scale(surface->getWidth(), surface->getHeight());
	surface->setOperator(CAIRO_OPERATOR_DEST_IN);
	surface->setSource(&p);
	
	roundedRectangle(
		surface,
		size,
		size,
		1.0f - (size * 2.0f),
		1.0f - (size * 2.0f),
		radius
	);
	
	surface->fill();
	surface->restore();
}

// This blur function was originally created my MacSlow and published on his website:
// http://macslow.thepimp.net
// My modifications were purely cosmetic to bring it more in line with what C++
// programmers expect to look at.

double* createKernel(double radius, double deviation) {
	int     size   = 2 * static_cast<int>(radius) + 1;
	double* kernel = new double[size + 1];
	
	if(!kernel) return 0;

	double radiusf = std::fabs(radius) + 1.0f;

	if(deviation == 0.0f) deviation = std::sqrt(
		-(radiusf * radiusf) / (2.0f * std::log(1.0f / 255.0f))
	);

	kernel[0] = size;

	double value = -radius;
	double sum   = 0.0f;	

	for(int i = 0; i < size; i++) {
		kernel[1 + i] = 
			1.0f / (2.506628275f * deviation) *
			expf(-((value * value) / (2.0f * (deviation * deviation))))
		;

		sum   += kernel[1 + i];
		value += 1.0f;
	}

	for(int i = 0; i < size; i++) kernel[1 + i] /= sum;

	return kernel;
}

void gaussianBlur(Surface* surface, double radius) {
	if(!surface || surface->status()) return;

	unsigned char* data   = surface->getData();
	CairoFormat    format = surface->getFormat();
	int            width  = surface->getWidth();
	int            height = surface->getHeight();
	
	// TODO: When channels == 1, this entire function fails in some way. :(
	unsigned int channels = 0;

	if(format == CAIRO_FORMAT_ARGB32) channels = 4;
	
	else if(format == CAIRO_FORMAT_RGB24) channels = 3;
	
	else if(format == CAIRO_FORMAT_A8) channels = 1;

	else return;

	unsigned int stride = width * channels;

	double* horzBlur = new double[height * stride];
	double* vertBlur = new double[height * stride];
	double* kernel   = createKernel(radius, 0.0f);

	if(!kernel) return;

	// Horizontal pass.
	for(int iY = 0; iY < height; iY++) {
		for(int iX = 0; iX < width; iX++) {
			double red   = 0.0f;
			double green = 0.0f;
			double blue  = 0.0f;
			double alpha = 0.0f;

			int offset = static_cast<int>(kernel[0]) / -2;
			
			for(int i = 0; i < static_cast<int>(kernel[0]); i++) {
				int x = iX + offset;

				if(x < 0 || x >= width) continue;

				unsigned char* dataPtr = &data[iY * stride + x * channels];
				
				double kernip1 = kernel[i + 1];

				if(channels == 1) alpha += kernip1 * dataPtr[0];

				else {
					if(channels == 4) alpha += kernip1 * dataPtr[3];

					red   += kernip1 * dataPtr[2];
					green += kernip1 * dataPtr[1];
					blue  += kernip1 * dataPtr[0];
				}
				
				offset++;
			}

			int baseOffset = iY * stride + iX * channels;

			if(channels == 1) horzBlur[baseOffset] = alpha;

			else {
				if(channels == 4) horzBlur[baseOffset + 3] = alpha;

				horzBlur[baseOffset + 2] = red;
				horzBlur[baseOffset + 1] = green;
				horzBlur[baseOffset]     = blue;
			}
		}
	}

	// Vertical pass.
	for(int iY = 0; iY < height; iY++) {
		for(int iX = 0; iX < width; iX++) {
			double red   = 0.0f;
			double green = 0.0f;
			double blue  = 0.0f;
			double alpha = 0.0f;

			int offset = static_cast<int>(kernel[0]) / -2;
			
			for(int i = 0; i < static_cast<int>(kernel[0]); i++) {
				int y = iY + offset;

				if(y < 0 || y >= height) {
					offset++;

					continue;
				}
				
				double* dataPtr = &horzBlur[y * stride + iX * channels];

				double kernip1 = kernel[i + 1];

				if(channels == 1) alpha += kernip1 * dataPtr[0];

				else {
					if(channels == 4) alpha += kernip1 * dataPtr[3];

					red   += kernip1 * dataPtr[2];
					green += kernip1 * dataPtr[1];
					blue  += kernip1 * dataPtr[0];
				}

				offset++;
			}

			int baseOffset = iY * stride + iX * channels;

			if(channels == 1) vertBlur[baseOffset] = alpha;

			else {
				if(channels == 4) vertBlur[baseOffset + 3] = alpha;

				vertBlur[baseOffset + 2] = red;
				vertBlur[baseOffset + 1] = green;
				vertBlur[baseOffset]     = blue;
			}
		}
	}

	// We're done with the blurring.
	delete[] kernel;

	for(int iY = 0; iY < height; iY++) {
		for(int iX = 0; iX < width; iX++) {
			int i = iY * stride + iX * channels;

			if(channels == 1) data[i] = static_cast<unsigned char>(vertBlur[i]);

			else {
				if(channels == 4) data[i + 3] = static_cast<unsigned char>(
					vertBlur[i + 3]
				);

				data[i + 2] = static_cast<unsigned char>(vertBlur[i + 2]);
				data[i + 1] = static_cast<unsigned char>(vertBlur[i + 1]);
				data[i]     = static_cast<unsigned char>(vertBlur[i]);
			}
		}
	}

	delete[] horzBlur;
	delete[] vertBlur;
}

/*
cairo_pattern_t* displaced_blend(
	cairo_t *cr,
	cairo_pattern_t *pat,
	double dx,
	double dy
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

cairo_pattern_t * blur_step(cairo_t *cr, cairo_pattern_t *pat) {
	pat = displaced_blend(cr, pat, 0, 1);
	pat = displaced_blend(cr, pat, 1, 0);
	
	return pat;
}

cairo_pattern_t* blurred_pattern(cairo_t *cr, cairo_pattern_t *pat, unsigned n) {
	while (n-->0) pat = blur_step(cr, pat);

	return pat;
}

void cairoBlur(CairoContext* cr, cairo_pattern_t* pat) {
	// Blur the pattern up.
	pat = blurred_pattern(cr, pat, 10);

	// Paint the pattern.
	cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
	cairo_set_source(cr, pat);
	cairo_paint(cr);

	// Clean up.
	// cairo_pattern_destroy(pat);
	// cairo_destroy(cr);
	// cairo_surface_destroy(surf);
}
*/

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
// void mapPathOnto(Surface* surface, cairo_path_t* path) {
void mapPathOnto(Surface* surface, Path& path) {
	/*
	parametrized_path_t param;
	
	param.path            = path->_path;
	param.parametrization = parametrize_path(path->_path);

	Path^ cPath = surface->CopyPath();

	surface->NewPath();

	transform_path(cPath->_path, (transform_point_func_t)(point_on_path), &param);

	surface->AppendPath(cPath);
	*/

	parametrized_path_t param;

	param.path            = path.getPath();
	param.parametrization = parametrize_path(path.getPath());

	Path current_path = surface->copyPath();

	surface->newPath();

	transform_path(current_path.getPath(), (transform_point_func_t)(point_on_path), &param);
	
	surface->appendPath(current_path);

	delete[] param.parametrization;
}

}
}
