// -*-c++-*- osgCairo - Copyright (C) 2006 Jeremy Moles

#include <osgCairo/Path>

namespace osgCairo {

Path::Path(CairoPath* path):
_path(path) {
}

Path::~Path() {
	cairo_path_destroy(_path);
}

CairoPath* Path::getPath() {
	return _path;
}

} // namespace osgCairo
