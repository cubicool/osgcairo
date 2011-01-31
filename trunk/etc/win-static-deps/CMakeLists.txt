PROJECT(osgCairo-deps)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(OSGCAIRO_DEPS_BASE "${PROJECT_SOURCE_DIR}")

SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Add a postfix, usually d on windows")

SET(SRC_ZLIB
	${OSGCAIRO_DEPS_BASE}/build/zlib-1.2.5
	CACHE PATH "The extracted zlib source"
)

SET(SRC_LIBPNG
	${OSGCAIRO_DEPS_BASE}/build/libpng-1.4.3
	CACHE PATH "The extracted libpng source"
)

SET(SRC_PIXMAN
	${OSGCAIRO_DEPS_BASE}/build/pixman-0.20.0
	CACHE PATH "The extracted pixman source"
)

SET(SRC_CAIRO
	${OSGCAIRO_DEPS_BASE}/build/cairo-1.10.2
	CACHE PATH "The extracted cairo source"
)

SET(SRC_FREETYPE
	${OSGCAIRO_DEPS_BASE}/build/freetype-2.4.4
	CACHE PATH "The extracted freetpye source"
)

SET(SRC_EXPAT
	${OSGCAIRO_DEPS_BASE}/build/expat-2.0.1
	CACHE PATH "The extracted expat source"
)

SET(SRC_FONTCONFIG
	${OSGCAIRO_DEPS_BASE}/build/fontconfig-2.8.0
	CACHE PATH "The extracted fontconfig source"
)

ADD_SUBDIRECTORY(zlib)
ADD_SUBDIRECTORY(libpng)
ADD_SUBDIRECTORY(pixman)
ADD_SUBDIRECTORY(cairo)
ADD_SUBDIRECTORY(freetype)
ADD_SUBDIRECTORY(expat)
ADD_SUBDIRECTORY(fontconfig)

