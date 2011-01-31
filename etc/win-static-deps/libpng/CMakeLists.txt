PROJECT(osgCairo-deps-libpng)

# -D_DEBUG
# -DDEBUG
# -DPNG_DEBUG=1

ADD_DEFINITIONS(
	-DWIN32
	-DNDEBUG
	-D_CRT_SECURE_NO_WARNINGS
)

INCLUDE_DIRECTORIES(${SRC_ZLIB} ${SRC_LIBPNG})

SET(SRC_FILES
	${SRC_LIBPNG}/png.c
	${SRC_LIBPNG}/pngerror.c
	${SRC_LIBPNG}/pngget.c
	${SRC_LIBPNG}/pngmem.c
	${SRC_LIBPNG}/pngpread.c
	${SRC_LIBPNG}/pngread.c
	${SRC_LIBPNG}/pngrio.c
	${SRC_LIBPNG}/pngrtran.c
	${SRC_LIBPNG}/pngrutil.c
	${SRC_LIBPNG}/pngset.c
	${SRC_LIBPNG}/pngtrans.c
	${SRC_LIBPNG}/pngwio.c
	${SRC_LIBPNG}/pngwrite.c
	${SRC_LIBPNG}/pngwtran.c
	${SRC_LIBPNG}/pngwutil.c
)

ADD_LIBRARY(osgCairo-deps-libpng STATIC ${SRC_FILES})

