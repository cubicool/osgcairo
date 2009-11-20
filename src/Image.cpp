// -*-c++-*- osgCairo - Copyright (C) 2009 Jeremy Moles

#include <cstring>
#include <osg/Notify>
#include <osgCairo/Image>

namespace osgCairo {

Image::Image(CairoFormat format):
_allocated (false),
_format    (format) {
}

Image::Image(
	unsigned int         width,
	unsigned int         height,
	CairoFormat          format,
	const unsigned char* data
):
_allocated (false),
_format    (format) {
	allocateSurface(width, height, data);
}

Image::Image(const Image& si, const osg::CopyOp& co):
osg::Image (si, co),
_allocated (si._allocated),
_format    (si._format) {
}

CairoSurface* Image::_createSurfaceImplementation() {
	return cairo_image_surface_create_for_data(
		_data,
		_format,
		_s,
		_t,
		cairo_format_stride_for_width(_format, _s)
	);
}

bool Image::allocateSurface(
	unsigned int         width,
	unsigned int         height,
	const unsigned char* data
) {
	// The default is for CAIRO_FORMAT_ARGB32.
	GLenum pf1 = GL_RGBA;
	GLenum pf2 = GL_UNSIGNED_INT_8_8_8_8_REV;

	if(_format == CAIRO_FORMAT_A8) {
		pf1 = GL_ALPHA;
		pf2 = GL_UNSIGNED_BYTE;
	}

	else if(_format == CAIRO_FORMAT_RGB24) pf1 = GL_RGB;

	// Call the osg::Image allocation method.
	allocateImage(width, height, 1, pf1, pf2);

	if(!osg::Image::valid()) return false;

	unsigned int i = 0;

	// This will have to do for now. :)
	if(_format == CAIRO_FORMAT_A8) i = 1;

	else i = 4;
	
	if(data) std::memcpy(_data, data, (width * height) * i);

	else std::memset(_data, 0, (width * height) * i);

	_allocated = true;

	if(_format == CAIRO_FORMAT_A8) _pixelFormat = GL_ALPHA;

	else _pixelFormat = GL_BGRA;

	dirty();

	return true;
}

void Image::setOriginBottomLeft() {
	setMatrix(Matrix::translate(0.0f, -_t) * Matrix::scale(1.0f, -1.0f));
}

double* createKernel(double radius, double deviation) {
	int     size   = 2 * static_cast<int>(radius) + 1;
	double* kernel = new double[size + 1];
	
	if(!kernel) return 0;

	double radiusf = fabs(radius) + 1.0f;

	if(deviation == 0.0f) deviation = sqrt(-(radiusf * radiusf) / (2.0f * log(1.0f / 255.0f)));

	kernel[0] = size;

	double value = -radius;
	double sum   = 0.0f;	

	for(int i = 0; i < size; i++) {
		kernel[1 + i] = 
			1.0f / (2.506628275f * deviation) *
			expf(-((value * value) /
			(2.0f * (deviation * deviation))))
		;

		sum   += kernel[1 + i];
		value += 1.0f;
	}

	for(int i = 0; i < size; i++) kernel[1 + i] /= sum;

	return kernel;
}

void Image::gaussianBlur(unsigned int radius) {
	if(!_surface) return;

	unsigned int channels = 0;

	if(_format == CAIRO_FORMAT_ARGB32) channels = 4;
	
	else if(_format == CAIRO_FORMAT_RGB24) channels = 3;
	
	else if(_format == CAIRO_FORMAT_A8) channels = 1;

	else return;

	unsigned int stride = _s * channels;

	// create buffers to hold the blur-passes
	double* horzBlur = new double[_t * stride];
	double* vertBlur = new double[_t * stride];
	double* kernel   = createKernel(radius, 0.0f);

	if(!kernel) return;

	// horizontal pass
	for(int iY = 0; iY < _t; iY++) {
		for(int iX = 0; iX < _s; iX++) {
			double red   = 0.0f;
			double green = 0.0f;
			double blue  = 0.0f;
			double alpha = 0.0f;

			int offset = static_cast<int>(kernel[0]) / -2;
			
			for(int i = 0; i < static_cast<int>(kernel[0]); i++) {
				int x = iX + offset;

				if(x < 0 || x >= _s) continue;

				unsigned char* dataPtr = &_data[iY * stride + x * channels];
				
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

	// vertical pass
	for(int iY = 0; iY < _t; iY++) {
		for(int iX = 0; iX < _s; iX++) {
			double red   = 0.0f;
			double green = 0.0f;
			double blue  = 0.0f;
			double alpha = 0.0f;

			int offset = static_cast<int>(kernel[0]) / -2;
			
			for(int i = 0; i < static_cast<int>(kernel[0]); i++) {
				int y = iY + offset;

				if(y < 0 || y >= _t) {
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

	for(int iY = 0; iY < _t; iY++) {
		for(int iX = 0; iX < _s; iX++) {
			int offset = iY * stride + iX * channels;

			if(channels == 1) _data[offset] = (unsigned char)vertBlur[offset];

			else {
				if(channels == 4) _data[offset + 3] = (unsigned char)vertBlur[offset + 3];

				_data[offset + 2] = (unsigned char)vertBlur[offset + 2];
				_data[offset + 1] = (unsigned char)vertBlur[offset + 1];
				_data[offset + 0] = (unsigned char)vertBlur[offset + 0];
			}
		}
	}

	delete[] horzBlur;
	delete[] vertBlur;
}

unsigned int Image::getImageSizeInBytes() const {
	return computeRowWidthInBytes(_s, _pixelFormat, _dataType, _packing) * _t * _r;
}

// Thank you to Riccardo Corsi <riccardo.corsi@vrmmp.it> for pointing out the necessity
// of this routine. :)
// TODO: If this routine gets called a lot, it may make sense to change the data
// IN PLACE instead of doing a new allocation every time.
unsigned char* createNewImageDataAsCairoFormat(osg::Image* image, CairoFormat cairoFormat) {
	unsigned char* data   = image->data();
	GLenum         format = image->getPixelFormat();

	if(cairoFormat == CAIRO_FORMAT_ARGB32 || cairoFormat == CAIRO_FORMAT_RGB24) {
		if(format != GL_RGB && format != GL_RGBA) return 0;

		unsigned int   numPixel = image->s() * image->t();
		unsigned char* newData  = new unsigned char[numPixel * 4];
		unsigned int   offset   = 4;

		if(format == GL_RGB) offset = 3;

		for(unsigned int i = 0; i < numPixel; ++i) {
			newData[i * 4]     = data[i * offset + 2];
			newData[i * 4 + 1] = data[i * offset + 1];
			newData[i * 4 + 2] = data[i * offset];

			if(cairoFormat == CAIRO_FORMAT_ARGB32) {
				if(format == GL_RGBA) newData[i * 4 + 3] = data[i * offset + 3];

				else newData[i * 4 + 3] = 255;
			}

			else newData[i * 4 + 3] = 0;
		}

		return newData;
	}

	return 0;
}

}
