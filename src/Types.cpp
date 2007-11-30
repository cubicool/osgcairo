// -*-c++-*- osgCairo - Copyright (C) 2006 Jeremy Moles

#include <osgCairo/Types>

namespace osgCairo {

CairoPatternType stringToPatternType(const std::string& str) {
	if(str == "SOLID")        return CAIRO_PATTERN_TYPE_SOLID;
	else if(str == "SURFACE") return CAIRO_PATTERN_TYPE_SURFACE;
	else if(str == "LINEAR")  return CAIRO_PATTERN_TYPE_LINEAR;
	else if(str == "RADIAL")  return CAIRO_PATTERN_TYPE_RADIAL;
	else                      return CAIRO_PATTERN_TYPE_SOLID;
}
	
CairoContent stringToContent(const std::string& str) {
	if(str == "COLOR")            return CAIRO_CONTENT_COLOR;
	else if(str == "ALPHA")       return CAIRO_CONTENT_ALPHA;
	else if(str == "COLOR_ALPHA") return CAIRO_CONTENT_COLOR_ALPHA;
	else                          return CAIRO_CONTENT_COLOR;
}

CairoAntialias stringToAntialias(const std::string& str) {
	if(str == "DEFAULT")       return CAIRO_ANTIALIAS_DEFAULT;
	else if(str == "NONE")     return CAIRO_ANTIALIAS_NONE;
	else if(str == "GRAY")     return CAIRO_ANTIALIAS_GRAY;
	else if(str == "SUBPIXEL") return CAIRO_ANTIALIAS_SUBPIXEL;
	else                       return CAIRO_ANTIALIAS_DEFAULT;
}

CairoFillRule stringToFillRule(const std::string& str) {
	if(str == "WINDING")       return CAIRO_FILL_RULE_WINDING;
	else if(str == "EVEN_ODD") return CAIRO_FILL_RULE_EVEN_ODD;
	else                       return CAIRO_FILL_RULE_WINDING;
}

CairoLineCap stringToLineCap(const std::string& str) {
	if(str == "BUTT")        return CAIRO_LINE_CAP_BUTT;
	else if(str == "ROUND")  return CAIRO_LINE_CAP_ROUND;
	else if(str == "SQUARE") return CAIRO_LINE_CAP_SQUARE;
	else                     return CAIRO_LINE_CAP_BUTT;
}

CairoLineJoin stringToLineJoin(const std::string& str) {
	if(str == "MITER")      return CAIRO_LINE_JOIN_MITER;
	else if(str == "ROUND") return CAIRO_LINE_JOIN_ROUND;
	else if(str == "BEVEL") return CAIRO_LINE_JOIN_BEVEL;
	else                    return CAIRO_LINE_JOIN_MITER;
}

CairoOperator stringToOperator(const std::string& str) {
	if(str == "CLEAR")          return CAIRO_OPERATOR_CLEAR;
	else if(str == "SOURCE")    return CAIRO_OPERATOR_SOURCE;
	else if(str == "OVER")      return CAIRO_OPERATOR_OVER;
	else if(str == "IN")        return CAIRO_OPERATOR_IN;
	else if(str == "OUT")       return CAIRO_OPERATOR_OUT;
	else if(str == "ATOP")      return CAIRO_OPERATOR_ATOP;
	else if(str == "DEST")      return CAIRO_OPERATOR_DEST;
	else if(str == "DEST_OVER") return CAIRO_OPERATOR_DEST_OVER;
	else if(str == "DEST_IN")   return CAIRO_OPERATOR_DEST_IN;
	else if(str == "DEST_OUT")  return CAIRO_OPERATOR_DEST_OUT;
	else if(str == "DEST_ATOP") return CAIRO_OPERATOR_DEST_ATOP;
	else if(str == "XOR")       return CAIRO_OPERATOR_XOR;
	else if(str == "ADD")       return CAIRO_OPERATOR_ADD;
	else if(str == "SATURATE")  return CAIRO_OPERATOR_SATURATE;
	else                        return CAIRO_OPERATOR_CLEAR;
}

} // namespace osgCairo
