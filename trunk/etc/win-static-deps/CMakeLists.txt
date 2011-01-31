PROJECT(osgCairo-deps)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(OSGCAIRO_DEPS_BASE "${PROJECT_SOURCE_DIR}")

SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Add a postfix, usually d on windows")

SET(ZLIB_SRC
	"${OSGCAIRO_DEPS_BASE}/build/zlib-1.2.5"
	CACHE PATH "The extracted zlib source"
)

SET(LIBPNG_SRC
	"${OSGCAIRO_DEPS_BASE}/build/libpng-1.4.3"
	CACHE PATH "The extracted libpng source"
)

SET(PIXMAN_SRC
	"${OSGCAIRO_DEPS_BASE}/build/pixman-0.20.0"
	CACHE PATH "The extracted pixman source"
)

SET(CAIRO_SRC
	"${OSGCAIRO_DEPS_BASE}/build/cairo-1.10.2"
	CACHE PATH "The extracted cairo source"
)

SET(FREETYPE_SRC
	"${OSGCAIRO_DEPS_BASE}/build/freetype-2.4.4"
	CACHE PATH "The extracted freetpye source"
)

SET(EXPAT_SRC
	"${OSGCAIRO_DEPS_BASE}/build/expat-2.0.1"
	CACHE PATH "The extracted expat source"
)

SET(FONTCONFIG_SRC
	"${OSGCAIRO_DEPS_BASE}/build/fontconfig-2.8.0"
	CACHE PATH "The extracted fontconfig source"
)

ADD_SUBDIRECTORY(zlib)
ADD_SUBDIRECTORY(libpng)
ADD_SUBDIRECTORY(pixman)
ADD_SUBDIRECTORY(cairo)
ADD_SUBDIRECTORY(freetype)
ADD_SUBDIRECTORY(expat)
ADD_SUBDIRECTORY(fontconfig)

