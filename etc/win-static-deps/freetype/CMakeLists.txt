PROJECT(osgCairo-deps-freetype)

ADD_DEFINITIONS(
	-DWIN32
	-D_LIB
	-D_CRT_SECURE_NO_WARINGS
	-DFT2_BUILD_LIBRARY
	-DFT_DEBUG_LEVEL_ERROR
	-DFG_DEBUG_LEVEL_TRACE
)

INCLUDE_DIRECTORIES(
	${SRC_EXPAT}/lib
	${SRC_FREETYPE}/include
)

SET(SRC_FILES
	${SRC_FREETYPE}/src/autofit/autofit.c
	${SRC_FREETYPE}/src/base/ftbase.c
	${SRC_FREETYPE}/src/base/ftbbox.c
	${SRC_FREETYPE}/src/base/ftbitmap.c
	${SRC_FREETYPE}/src/base/ftdebug.c
	${SRC_FREETYPE}/src/base/ftfstype.c
	${SRC_FREETYPE}/src/base/ftgasp.c
	${SRC_FREETYPE}/src/base/ftglyph.c
	${SRC_FREETYPE}/src/base/ftgxval.c
	${SRC_FREETYPE}/src/base/ftinit.c
	${SRC_FREETYPE}/src/base/ftlcdfil.c
	${SRC_FREETYPE}/src/base/ftmm.c
	${SRC_FREETYPE}/src/base/ftotval.c
	${SRC_FREETYPE}/src/base/ftpatent.c
	${SRC_FREETYPE}/src/base/ftpfr.c
	${SRC_FREETYPE}/src/base/ftstroke.c
	${SRC_FREETYPE}/src/base/ftsynth.c
	${SRC_FREETYPE}/src/base/ftsystem.c
	${SRC_FREETYPE}/src/base/fttype1.c
	${SRC_FREETYPE}/src/base/ftwinfnt.c
	${SRC_FREETYPE}/src/base/ftxf86.c
	${SRC_FREETYPE}/src/bdf/bdf.c
	${SRC_FREETYPE}/src/cache/ftcache.c
	${SRC_FREETYPE}/src/cff/cff.c
	${SRC_FREETYPE}/src/cid/type1cid.c
	${SRC_FREETYPE}/src/gzip/ftgzip.c
	${SRC_FREETYPE}/src/lzw/ftlzw.c
	${SRC_FREETYPE}/src/pcf/pcf.c
	${SRC_FREETYPE}/src/pfr/pfr.c
	${SRC_FREETYPE}/src/psaux/psaux.c
	${SRC_FREETYPE}/src/pshinter/pshinter.c
	${SRC_FREETYPE}/src/psnames/psmodule.c
	${SRC_FREETYPE}/src/raster/raster.c
	${SRC_FREETYPE}/src/sfnt/sfnt.c
	${SRC_FREETYPE}/src/smooth/smooth.c
	${SRC_FREETYPE}/src/truetype/truetype.c
	${SRC_FREETYPE}/src/type1/type1.c
	${SRC_FREETYPE}/src/type42/type42.c
	${SRC_FREETYPE}/src/winfonts/winfnt.c
)

ADD_LIBRARY(osgCairo-deps-freetype STATIC ${SRC_FILES})

