// -*-c++-*- osgCairo - Copyright (C) 2006 Jeremy Moles

#ifdef OSGCAIRO_POPPLER
#include <poppler.h>
#endif

#include <osg/Notify>
#include <osgCairo/ReadFile>

namespace osgCairo {

bool readFilePDF(const std::string& uri, ImageVector& pages, const PDFOptions& opts) {
#ifdef OSGCAIRO_POPPLER
	static bool gTypeInit = false;

	if(!gTypeInit) {
		g_type_init();

		gTypeInit = true;
	}

	PopplerDocument* doc = poppler_document_new_from_file(uri.c_str(), NULL, NULL);

	if(!doc) {
		osg::notify(osg::WARN)
			<< "osgCairo::readFilePDF - Couldn't find file: " << uri << "."
			<< std::endl
		;

		return false;
	}

	int numPages = poppler_document_get_n_pages(doc);

	if(numPages <= 0) {
		osg::notify(osg::WARN)
			<< "osgCairo::readFilePDF - file has no pages." << uri
			<< std::endl
		;

		return false;
	}

	if(opts.maxPages > 0 && numPages > opts.maxPages) numPages = opts.maxPages;

	for(int p = 0; p < numPages; p++) {
		PopplerPage* page = poppler_document_get_page(doc, p);

		if(!page) continue;

		double w = 0.0f;
		double h = 0.0f;

		unsigned int width  = opts.width;
		unsigned int height = opts.height;

		poppler_page_get_size(page, &w, &h);

		Image* image = 0;
	
		if(opts.keepPageSize) image = new Image(w, h, CAIRO_FORMAT_ARGB32);
		
		else image = new Image(width, height, CAIRO_FORMAT_ARGB32);

		if(image->createContext()) {
			if(!opts.keepPageSize) {
				if(opts.keepAspectRatio) {
					double sf = 1.0f;

					if(width == height) {
						if(w > h) sf = width / w;

						else sf = height / h;
					}
	
					else if(width > height) sf = height / h;

					else sf = width / w;

					image->scale(sf, sf);
				}

				else image->scale(width / w, height / h);
			}
			
			image->save();
			image->setSourceRGBA(opts.backgroundColor);
			image->rectangle(0.0f, 0.0f, w, h);
			image->fill();
			image->restore();

			poppler_page_render(page, image->getContext());

			pages.push_back(image);
		}
	}

	g_object_unref(doc);

	return true;

#else
	osg::notify(osg::WARN)
		<< "Poppler support not compiled into osgCairo; cannot load PDF "
		<< "files. Please see:" << std::endl << std::endl
		<< "	http://poppler.freedesktop.org" << std::endl << std::endl
		<< "...for more information."
		<< std::endl
	;

	return false;
#endif
}

} // namespace osgCairo
