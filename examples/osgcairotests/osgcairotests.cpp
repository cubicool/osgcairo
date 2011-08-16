#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgCairo/Image>
#include <osgCairo/ReadFile>
#include <osgCairo/Util>

const unsigned int IMG_WIDTH  = 512;
const unsigned int IMG_HEIGHT = 256;

void textOnImage(osgCairo::Image* image, cairo_format_t format, const std::string& name) {
	cairo_t* c = image->createContext();

	if(!cairo_status(c)) {
		cairo_save(c);
		cairo_set_operator(c, CAIRO_OPERATOR_CLEAR);
		cairo_paint(c);
		cairo_restore(c);

		cairo_text_extents_t extents;

		cairo_set_font_size(c, 50.0f);
		cairo_select_font_face(
			c,
			"Sans",
			CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_BOLD
		);

		cairo_text_extents(c, name.c_str(), &extents);

		cairo_translate(
			c,
			(IMG_WIDTH - extents.width) / 2.0f,
			((IMG_HEIGHT - extents.height) / 2.0f) + (extents.height / 2.0f)
		);

		cairo_text_path(c, name.c_str());

		if(format != CAIRO_FORMAT_A8) cairo_set_source_rgb(c, 1.0f, 0.0f, 0.0f);

		cairo_fill(c);
		cairo_destroy(c);
	}
}

osgCairo::Image* createImage(cairo_format_t format, const std::string& name) {
	osgCairo::Image* image = new osgCairo::Image();
	
	if(image->allocateSurface(IMG_WIDTH, IMG_HEIGHT, format)) {
		textOnImage(image, format, name);

		// image->flipVertical();
		image->dirty();
	}

	return image;
}

void writeImage(osgCairo::Image* image, const std::string& path) {
	if(!image) {
		OSG_NOTICE << "Warning: " << path << ".png couldn't be written." << std::endl;

		return;
	}

	OSG_NOTICE << "Writing: " << path << ".png" << std::endl;
	
	// This could be use on ARGB32 surfaces. However, osgDB cannot CURRENTLY be used to
	// dump images, so we stick with osgCairo::util.
	// image->unPreMultiply();
	// osgDB::writeImageFile(*image, path + ".png");

	osgCairo::util::writeToPNG(image->getSurface(), path + ".png");
}

osgCairo::Image* readImage(const std::string& path) {
	OSG_NOTICE << "Reading: " << path << ".png" << std::endl;

	// return osgCairo::readImageFile(path + ".png");

	return osgCairo::util::readFromPNG(path + ".png");
}

void imageStats(osgCairo::Image* image) {
	cairo_surface_t* surface = image->getSurface();

	OSG_NOTICE
		<< "Surface: " << surface << std::endl
		<< "RefCount: " << cairo_surface_get_reference_count(surface) << std::endl
	;
}

int main(int argc, char** argv) {
	osg::ref_ptr<osgCairo::Image> imageA8     = createImage(CAIRO_FORMAT_A8, "imageA8");
	osg::ref_ptr<osgCairo::Image> imageRGB24  = createImage(CAIRO_FORMAT_RGB24, "imageRGB24");
	osg::ref_ptr<osgCairo::Image> imageARGB32 = createImage(CAIRO_FORMAT_ARGB32, "imageARGB32");

	writeImage(imageA8, "imageA8");
	writeImage(imageRGB24, "imageRGB24");
	writeImage(imageARGB32, "imageARGB32");

	imageA8     = readImage("imageA8");
	imageRGB24  = readImage("imageRGB24");
	imageARGB32 = readImage("imageARGB32");

	writeImage(imageA8, "read-imageA8");
	writeImage(imageRGB24, "read-imageRGB24");
	writeImage(imageARGB32, "read-imageARGB32");

	/*
	osg::ref_ptr<osgCairo::Image> orig = createImage(CAIRO_FORMAT_A8, "orig");

	// Test SHALLOW copying...
	OSG_WARN << "Copying: SHALLOW_COPY" << std::endl;

	imageStats(orig);

	osg::ref_ptr<osgCairo::Image> copy = new osgCairo::Image(*orig);
	
	imageStats(copy);

	// Test DEEP copying...
	OSG_WARN << "Copying: DEEP_COPY_IMAGES" << std::endl;
	
	imageStats(orig);
	
	copy = new osgCairo::Image(*orig, osg::CopyOp::DEEP_COPY_IMAGES);
	
	imageStats(copy);

	writeImage(orig, "orig");
	writeImage(copy, "copy");
	*/

	return 0;
}

