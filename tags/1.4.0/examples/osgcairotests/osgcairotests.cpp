#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgCairo/Image>
#include <osgCairo/ReadFile>
#include <osgCairo/Util>
#include <osgCairo/Notify>

const unsigned int IMG_WIDTH  = 512;
const unsigned int IMG_HEIGHT = 256;
const char*        FILE_EXT   = ".png";

void textOnImage(osgCairo::Image* image, const std::string& name) {
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

		if(image->getSurfaceFormat() != CAIRO_FORMAT_A8) {
			cairo_set_source_rgb(c, 1.0f, 0.0f, 0.0f);
		}

		cairo_fill(c);
		cairo_destroy(c);
	}
}

osgCairo::Image* createImage(cairo_format_t format, const std::string& name) {
	osgCairo::Image* image = new osgCairo::Image();
	
	if(image->allocateSurface(IMG_WIDTH, IMG_HEIGHT, format)) {
		textOnImage(image, name);

		image->flipVertical();
		image->dirty();
	}

	return image;
}

void writeImage(osgCairo::Image* image, const std::string& path) {
	if(!image) {
		OSG_NOTICE << "Warning: " << path << " couldn't be written." << std::endl;

		return;
	}

	OSG_NOTICE << "Writing: " << path << FILE_EXT << std::endl;

	// Calling this function will convert the alpha levels internally so that they 
	// are not premultiplied, resulting in a "proper" PNG. However, cairo uses
	// premultiplied alpha internally.
	// if(image->getSurfaceFormat() == CAIRO_FORMAT_ARGB32) image->unPreMultiply();
	
	osgDB::writeImageFile(*image, path + FILE_EXT);
}

osgCairo::Image* readImage(const std::string& path) {
	OSG_NOTICE << "Reading: " << path << FILE_EXT << std::endl;

	return osgCairo::readImageFile(path + FILE_EXT);
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
	osg::ref_ptr<osgCairo::Image> imageARGB32 = createImage(CAIRO_FORMAT_ARGB32, "imageARGB32");

	writeImage(imageA8, "imageA8");
	writeImage(imageARGB32, "imageARGB32");

	imageA8     = readImage("imageA8");
	imageARGB32 = readImage("imageARGB32");

	writeImage(imageA8, "read-imageA8");
	writeImage(imageARGB32, "read-imageARGB32");

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

	textOnImage(copy, "copy-from-orig");

	copy->dirty();

	writeImage(orig, "orig");
	writeImage(copy, "copy");

	return 0;
}

