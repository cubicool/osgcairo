// -*-c++-*- osgCairo - Copyright (C) 2009 Jeremy Moles
// $Id$

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

}
