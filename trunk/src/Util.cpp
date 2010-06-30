// -*-c++-*- osgCairo - Copyright (C) 2010 Jeremy Moles
// $Id$

#include <cmath>
#include <osgCairo/Util>

namespace osgCairo {

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

void gaussianBlur(
	unsigned char* data,
	CairoFormat    format,
	int            width,
	int            height,
	double         radius
) {
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

}
