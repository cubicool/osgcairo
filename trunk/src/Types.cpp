// -*-c++-*- osgCairo - Copyright (C) 2009 Jeremy Moles

#include <osgCairo/Types>

namespace osgCairo {

std::string cairoFormatAsString(CairoFormat format) {
	if(format == CAIRO_FORMAT_ARGB32) return "CAIRO_FORMAT_ARGB32";

	else if(format == CAIRO_FORMAT_RGB24) return "CAIRO_FORMAT_RGB24";

	else if(format == CAIRO_FORMAT_A8) return "CAIRO_FORMAT_A8";
	
	else return "CAIRO_FORMAT_A1";
}

}
