// -*-c++-*- osgCairo - Copyright (C) 2006 Jeremy Moles

#include <cstring>
#include <sstream>
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
	GLenum pf1 = GL_RGBA;
	GLenum pf2 = GL_UNSIGNED_INT_8_8_8_8_REV;

	if(_format == CAIRO_FORMAT_A8) {
		pf1 = GL_ALPHA;
		pf2 = GL_UNSIGNED_BYTE;
	}

	// Call the osg::Image allocation method.
	osg::Image::allocateImage(width, height, 1, pf1, pf2);

	if(!osg::Image::valid()) {
		osg::notify(osg::WARN)
			<< "osgCairo::Image::allocateImage failed!"
			<< std::endl
		;

		return false;
	}

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

void Image::roundedRectangle(
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

void Image::roundedCorners() {
	SolidPattern p(0.0f, 0.0f, 0.0f, 1.0f);

	save();
	scale(_s, _t);
	setOperator(CAIRO_OPERATOR_DEST_IN);
	setSource(&p);
	roundedRectangle(0.01f, 0.01f, 0.98f, 0.98f, 0.075f);
	fill();
	restore();
}

void Image::setOriginBottomLeft() {
	setMatrix(Matrix::translate(0.0f, -_t) * Matrix::scale(1.0f, -1.0f));
}

unsigned int Image::getImageSizeInBytes() const {
	return computeRowWidthInBytes(_s, _pixelFormat, _dataType, _packing) * _t * _r;
}

double* createKernel(
	int radius,
	double deviation
) {
	double* kernel = 0;
	double  sum    = 0.0f;
	double  value  = 0.0f;
	int     i;
	int     size   = 2 * radius + 1;
	double  radiusf;

	if(radius <= 0) return 0;

	kernel = new double[size + 1];
	
	if(!kernel) return 0;

	radiusf = fabs(double(radius)) + 1.0f;

	if(deviation == 0.0f) 
		deviation = sqrt (-(radiusf * radiusf) / (2.0f * log (1.0f / 255.0f)))
	;

	kernel[0] = size;
	value     = (double)-radius;
	
	for (i = 0; i < size; i++) {
		kernel[1 + i] = 
			1.0f / (2.506628275f * deviation) *
			expf (-((value * value) /
			(2.0f * deviation * deviation)))
		;

		sum   += kernel[1 + i];
		value += 1.0f;
	}

	for(i = 0; i < size; i++) kernel[1 + i] /= sum;

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

			int offset = ((int) kernel[0]) / -2;
			
			for(int i = 0; i < (int) kernel[0]; i++) {
				int x = iX + offset;

				// TODO: THIS
				if (x >= 0 && x < _s) {
					int baseOffset = iY * stride + x * channels;

					if(channels == 1) alpha += (
						kernel[1 + i] *
						(double)_data[baseOffset]
					);

					else {
						if (channels == 4)
							alpha += (kernel[1+i] *
							(double)_data[baseOffset + 3]);

						red += (kernel[1+i] *
							(double)_data[baseOffset + 2]);

						green += (kernel[1+i] *
							(double)_data[baseOffset + 1]);

						blue += (kernel[1+i] *
							(double)_data[baseOffset + 0]);
					}
				}

				offset++;
			}

			int baseOffset = iY * stride + iX * channels;

			if(channels == 1) horzBlur[baseOffset] = alpha;

			else {
				if(channels == 4) horzBlur[baseOffset + 3] = alpha;

				horzBlur[baseOffset + 2] = red;
				horzBlur[baseOffset + 1] = green;
				horzBlur[baseOffset + 0] = blue;
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

			int offset = ((int) kernel[0]) / -2;
			
			for(int i = 0; i < (int) kernel[0]; i++) {
				int y = iY + offset;

				if (y >= 0 && y < _t) {
					int baseOffset = y * stride + iX * channels;

					if(channels ==1) alpha += (
						kernel[1 + i] *
						horzBlur[baseOffset]
					);

					else {

						if (channels == 4)
							alpha += (kernel[1+i] *
							horzBlur[baseOffset + 3]);

						red   += (kernel[1+i]  *
							horzBlur[baseOffset + 2]);

						green += (kernel[1+i]  *
							horzBlur[baseOffset + 1]);

						blue  += (kernel[1+i]  *
							horzBlur[baseOffset + 0]);
					}
				}

				offset++;
			}

			int baseOffset = iY * stride + iX * channels;

			if(channels == 1) vertBlur[baseOffset] = alpha;

			else {
				if(channels == 4) vertBlur[baseOffset + 3] = alpha;

				vertBlur[baseOffset + 2] = red;
				vertBlur[baseOffset + 1] = green;
				vertBlur[baseOffset + 0] = blue;
			}
		}
	}

	// We're done with the blurring.
	delete[] kernel;

	for (int iY = 0; iY < _t; iY++) {
		for (int iX = 0; iX < _s; iX++) {
			int offset = iY * stride + iX * channels;

			if(channels == 1) _data[offset] = (unsigned char)vertBlur[offset];

			else {
				if (channels == 4) _data[offset + 3] = (unsigned char)vertBlur[offset + 3];

				_data[offset + 2] = (unsigned char)vertBlur[offset + 2];
				_data[offset + 1] = (unsigned char)vertBlur[offset + 1];
				_data[offset + 0] = (unsigned char)vertBlur[offset + 0];
			}
		}
	}

	/*
	cairo_surface_t* hb = cairo_image_surface_create_for_data((unsigned char*)horzBlur, _format, _s, _t, 
		cairo_format_stride_for_width(_format, _s)
	);
	cairo_surface_t* vb = cairo_image_surface_create_for_data((unsigned char*)vertBlur, _format, _s, _t,
		cairo_format_stride_for_width(_format, _s)
	);

	cairo_surface_write_to_png(hb, "hb.png");
	cairo_surface_write_to_png(vb, "vb.png");
	*/

	delete[] horzBlur;
	delete[] vertBlur;
}

// Thank you to Riccardo Corsi <riccardo.corsi@vrmmp.it> for pointing out the necessity
// of this routine. :)
unsigned char* convertImageDataToCairoFormat(osg::Image* image, CairoFormat cairoFormat) {
	unsigned char* data   = image->data();
	GLenum         format = image->getPixelFormat();

	if(cairoFormat == CAIRO_FORMAT_ARGB32) {
		if(format != GL_RGB && format != GL_RGBA) return 0;

		unsigned int   numPixel = image->s() * image->t();
		unsigned char* newData  = new unsigned char[numPixel * 4];
		unsigned int   offset   = 4;

		if(format == GL_RGB) offset = 3;

		for(unsigned int i = 0; i < numPixel; ++i) {
			newData[i * 4]     = data[i * offset + 2];
			newData[i * 4 + 1] = data[i * offset + 1];
			newData[i * 4 + 2] = data[i * offset];

			if(format == GL_RGBA) newData[i * 4 + 3] = data[i * offset + 3];

			else newData[i * 4 + 3] = 255;
		}

		return newData;
	}

	return 0;
}

} // namespace osgCairo
