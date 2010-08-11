// -*-c++-*- osgCairo - Copyright (C) 2010 Jeremy Moles
// $Id$

#include <cmath>
#include <osg/Math>
#include <osgCairo/Util>

namespace osgCairo {
namespace util {

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

void gaussianBlur(cairo_surface_t* surface, double radius, double deviation) {
	if(cairo_surface_status(surface)) return;

	unsigned char* data   = cairo_image_surface_get_data(surface);
	cairo_format_t format = cairo_image_surface_get_format(surface);
	int            width  = cairo_image_surface_get_width(surface);
	int            height = cairo_image_surface_get_height(surface);
	int            stride = cairo_image_surface_get_stride(surface);
	
	unsigned int channels = 0;

	if(format == CAIRO_FORMAT_ARGB32) channels = 4;
	
	else if(format == CAIRO_FORMAT_RGB24) channels = 3;
	
	else if(format == CAIRO_FORMAT_A8) channels = 1;

	else return;

	double* horzBlur = new double[height * stride];
	double* vertBlur = new double[height * stride];
	double* kernel   = createKernel(radius, deviation);

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

}
}
