/*
	ANSI C code from the article
	"Fast Embossing Effects on Raster Image Data"
	by John Schlag, jfs@kerner.com
	in "Graphics Gems IV", Academic Press, 1994

	Emboss - shade 24-bit pixels using a single distant light source.
	Normals are obtained by differentiating a monochrome 'bump' image.
	The unary case ('texture' == NULL) uses the shading result as output.
	The binary case multiples the optional 'texture' image by the shade.
	Images are in row major order with interleaved color components (rgbrgb...).
	E.g., component c of pixel x,y of 'dst' is dst[3*(y*xSize + x) + c].
*/

#include <math.h>
#include <sys/types.h>

#include <osgCairo/Util>

#define pixelScale 255.9f

namespace osgCairo {
namespace util {

cairo_surface_t* createEmbossedSurface(
	cairo_surface_t* surface,
	double           azimuth,
	double           elevation,
	unsigned short   width45
) {
	long Nx, Ny, Nz, Lx, Ly, Lz, Nz2, NzLz, NdotL;
	unsigned short x, y;

	unsigned char* bump  = cairo_image_surface_get_data(surface);
	unsigned short xSize = cairo_image_surface_get_width(surface);
	unsigned short ySize = cairo_image_surface_get_height(surface);

	cairo_surface_t* dstSurface = cairo_image_surface_create(CAIRO_FORMAT_A8, xSize, ySize);

	unsigned char* dst = cairo_image_surface_get_data(dstSurface);

	// compute the light vector from the input parameters.
	// normalize the length to pixelScale for fast shading calculation.
	Lx = cos(azimuth) * cos(elevation) * pixelScale;
	Ly = sin(azimuth) * cos(elevation) * pixelScale;
	Lz = sin(elevation) * pixelScale;

	// constant z component of image surface normal - this depends on the
	// image slope we wish to associate with an angle of 45 degrees, which
	// depends on the width of the filter used to produce the source image.
	Nz   = (6 * 255) / width45;
	Nz2  = Nz * Nz;
	NzLz = Nz * Lz;

	// mung pixels, avoiding edge pixels
	dst += xSize * 1;

	for(y = 1; y < ySize - 1; y++, bump += xSize, dst += 1) {
		unsigned char* s1 = bump + 1;
		unsigned char* s2 = s1 + xSize;
		unsigned char* s3 = s2 + xSize;
		
		dst += 1;

		for(x = 1; x < xSize-1; x++, s1++, s2++, s3++) {
			unsigned char shade = 0;

			// compute the normal from the bump map. the type of the expression
			// before the cast is compiler dependent. in some cases the sum is
			// unsigned, in others it is signed. ergo, cast to signed.
			Nx = (int)(s1[-1] + s2[-1] + s3[-1] - s1[1] - s2[1] - s3[1]);
			Ny = (int)(s3[-1] + s3[0] + s3[1] - s1[-1] - s1[0] - s1[1]);

			// shade with distant light source

			if(!Nx && !Ny) shade = Lz;

			else if((NdotL = Nx * Lx + Ny * Ly + NzLz) < 0) shade = 0;

			else shade = NdotL / sqrt(Nx * Nx + Ny * Ny + Nz2);

			// do something with the shading result

			/*
			if (shade >= 127) shade -= 127;
			else shade = 0;
			*/

			*dst++ = shade;
		}
	}

	return dstSurface;
}

}
}
