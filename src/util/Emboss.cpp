// Adapted from "Fast Embossing Effects on Raster Image Data"
// by John Schlag, <jfs@kerner.com>
// in "Graphics Gems IV", Academic Press, 1994

#include <cmath>
#include <cstring>
#include <osg/Notify>
#include <osgCairo/Util>

// TODO: Eventually, make these two values variable. For now, these
// give what I consider to be the best results, but a WIDTH_45 or 2
// or 3 isn't bad, either. Perhaps they could be enum'd in osgCairo.
#define PIXEL_SCALE 255
#define WIDTH_45    1

namespace osgCairo {
namespace util {

cairo_surface_t* createEmbossedSurface(
	cairo_surface_t* surface,
	double           azimuth,
	double           elevation
) {
	if(cairo_image_surface_get_format(surface) != CAIRO_FORMAT_A8) {
		osg::notify(osg::WARN)
			<< "Cannot call osgCairo::util::createEmbossedSurface with a non-A8 "
			<< "surface type; returning NULL."
			<< std::endl
		;

		return 0;
	}
	
	unsigned char* src    = cairo_image_surface_get_data(surface);
	unsigned int   width  = cairo_image_surface_get_width(surface);
	unsigned int   stride = cairo_image_surface_get_stride(surface);
	unsigned int   height = cairo_image_surface_get_height(surface);

	cairo_surface_t* dstSurface = cairo_image_surface_create(CAIRO_FORMAT_A8, width, height);

	unsigned char* dst = cairo_image_surface_get_data(dstSurface);

	// Compute the light vector from the input parameters.
	// Normalize the length to PIXEL_SCALE for fast shading calculation.
	long Lx   = std::cos(azimuth) * std::cos(elevation) * PIXEL_SCALE;
	long Ly   = std::sin(azimuth) * std::cos(elevation) * PIXEL_SCALE;
	long Lz   = std::sin(elevation) * PIXEL_SCALE;
	long Nz   = (6 * 255) / WIDTH_45;
	long Nz2  = Nz * Nz;
	long NzLz = Nz * Lz;

	std::memset(dst, Lz, stride * height);

	for(unsigned int y = 1; y < height - 2; y++) {
		for(unsigned int x = 1; x < stride - 1; x++) {
			unsigned int   offset = (y * stride) + x;
			unsigned char* s1     = src + offset;
			unsigned char* s2     = s1 + stride;
			unsigned char* s3     = s2 + stride;
			unsigned char  shade  = 0;

			// Compute the normal from the source. The type of the expression
			// before the cast is compiler dependent. In some cases the sum is
			// unsigned, in others it is signed. Ergo, cast to signed.
			long Nx = static_cast<long>(s1[-1] + s2[-1] + s3[-1] - s1[1] - s2[1] - s3[1]);
			long Ny = static_cast<long>(s3[-1] + s3[0] + s3[1] - s1[-1] - s1[0] - s1[1]);
			
			long NdotL = Nx * Lx + Ny * Ly + NzLz;

			// Shade with distant light source.
			if(!Nx && !Ny) shade = Lz;

			else if(NdotL < 0) shade = 0;

			else shade = NdotL / std::sqrt(Nx * Nx + Ny * Ny + Nz2);
			
			*(dst + offset) = shade;
		}
	}

	return dstSurface;
}

}
}
