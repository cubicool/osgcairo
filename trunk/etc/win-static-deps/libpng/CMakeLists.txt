PROJECT(osgCairo-deps-libpng)


# -D_DEBUG
# -DDEBUG
# -DPNG_DEBUG=1

ADD_DEFINITIONS(
	-DWIN32
	-DNDEBUG
	-D_CRT_SECURE_NO_WARNINGS
)

INCLUDE_DIRECTORIES(${ZLIB_SRC} ${LIBPNG_SRC})

SET(SRC_FILES
	${LIBPNG_SRC}/png.c
	${LIBPNG_SRC}/pngerror.c
	${LIBPNG_SRC}/pngget.c
	${LIBPNG_SRC}/pngmem.c
	${LIBPNG_SRC}/pngpread.c
	${LIBPNG_SRC}/pngread.c
	${LIBPNG_SRC}/pngrio.c
	${LIBPNG_SRC}/pngrtran.c
	${LIBPNG_SRC}/pngrutil.c
	${LIBPNG_SRC}/pngset.c
	${LIBPNG_SRC}/pngtrans.c
	${LIBPNG_SRC}/pngwio.c
	${LIBPNG_SRC}/pngwrite.c
	${LIBPNG_SRC}/pngwtran.c
	${LIBPNG_SRC}/pngwutil.c
)

ADD_LIBRARY(osgCairo-deps-libpng STATIC ${SRC_FILES})

