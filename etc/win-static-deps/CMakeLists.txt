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

ADD_SUBDIRECTORY(zlib)
ADD_SUBDIRECTORY(libpng)
ADD_SUBDIRECTORY(pixman)

