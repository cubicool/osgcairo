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
	"${FREETYPE_SRC}/include"
)

SET(SRC_FILES
	${FREETYPE_SRC}/src/autofit/autofit.c
	${FREETYPE_SRC}/src/base/ftbase.c
	${FREETYPE_SRC}/src/base/ftbbox.c
	${FREETYPE_SRC}/src/base/ftbitmap.c
	${FREETYPE_SRC}/src/base/ftdebug.c
	${FREETYPE_SRC}/src/base/ftfstype.c
	${FREETYPE_SRC}/src/base/ftgasp.c
	${FREETYPE_SRC}/src/base/ftglyph.c
	${FREETYPE_SRC}/src/base/ftgxval.c
	${FREETYPE_SRC}/src/base/ftinit.c
	${FREETYPE_SRC}/src/base/ftlcdfil.c
	${FREETYPE_SRC}/src/base/ftmm.c
	${FREETYPE_SRC}/src/base/ftotval.c
	${FREETYPE_SRC}/src/base/ftpatent.c
	${FREETYPE_SRC}/src/base/ftpfr.c
	${FREETYPE_SRC}/src/base/ftstroke.c
	${FREETYPE_SRC}/src/base/ftsynth.c
	${FREETYPE_SRC}/src/base/ftsystem.c
	${FREETYPE_SRC}/src/base/fttype1.c
	${FREETYPE_SRC}/src/base/ftwinfnt.c
	${FREETYPE_SRC}/src/base/ftxf86.c
	${FREETYPE_SRC}/src/bdf/bdf.c
	${FREETYPE_SRC}/src/cache/ftcache.c
	${FREETYPE_SRC}/src/cff/cff.c
	${FREETYPE_SRC}/src/cid/type1cid.c
	${FREETYPE_SRC}/src/gzip/ftgzip.c
	${FREETYPE_SRC}/src/lzw/ftlzw.c
	${FREETYPE_SRC}/src/pcf/pcf.c
	${FREETYPE_SRC}/src/pfr/pfr.c
	${FREETYPE_SRC}/src/psaux/psaux.c
	${FREETYPE_SRC}/src/pshinter/pshinter.c
	${FREETYPE_SRC}/src/psnames/psmodule.c
	${FREETYPE_SRC}/src/raster/raster.c
	${FREETYPE_SRC}/src/sfnt/sfnt.c
	${FREETYPE_SRC}/src/smooth/smooth.c
	${FREETYPE_SRC}/src/truetype/truetype.c
	${FREETYPE_SRC}/src/type1/type1.c
	${FREETYPE_SRC}/src/type42/type42.c
	${FREETYPE_SRC}/src/winfonts/winfnt.c
)

ADD_LIBRARY(osgCairo-deps-freetype STATIC ${SRC_FILES})

