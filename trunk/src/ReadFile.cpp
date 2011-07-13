// -*-c++-*- osgCairo - Copyright (C) 2011 osgCairo Development Team
// $Id$

#include <osg/Notify>
#include <osgCairo/ReadFile>
#include <osgCairo/Util>

namespace osgCairo {

Image* readImageFile(const std::string& path, osgDB::ReaderWriter::Options* options) {
	osg::ref_ptr<osg::Image> image = osgDB::readImageFile(path, options);

	if(!image) return 0;

	GLenum         format      = image->getPixelFormat();
	cairo_format_t cairoFormat = CAIRO_FORMAT_ARGB32;

	if(format != GL_RGB && format != GL_RGBA && format != GL_ALPHA) return 0;

	if(format == GL_ALPHA) cairoFormat = CAIRO_FORMAT_A8;

	else if(format == GL_RGB) {
		// Check the options string to see if the want to add an alpha channel
		// to RGB images; this can be really useful sometimes.
		std::string ops = options ? options->getOptionString() : std::string();

		if(ops.find("addAlphaToRGB") != std::string::npos) {
			osg::notify()
				<< "Adding alpha channel to GL_RGB image at the "
				<< "request of the user." << std::endl
			;

			cairoFormat = CAIRO_FORMAT_ARGB32;
		}

		else cairoFormat = CAIRO_FORMAT_RGB24;
	}

	unsigned char* newData = createNewImageDataAsCairoFormat(image, cairoFormat);

	if(!newData) {
		osg::notify(osg::WARN)
			<< "Failed to convert " << path << " to an appropriate Cairo format ("
			<< util::cairoFormatAsString(cairoFormat) << ")." << std::endl
		;

		return 0;
	}

	Image* cairoImage = new Image(image->s(), image->t(), cairoFormat, newData);

	osg::notify()
		<< "Loaded osgCairo::Image file " << path
		<< " using format: " << util::cairoFormatAsString(cairoFormat)
		<< std::endl
	;

	delete[] newData;

	return cairoImage;
}

}
