PROJECT(osgCairo-deps-zlib)

ADD_DEFINITIONS(
	-DWIN32
	-DZLIB_WINAPI
	-D_CRT_NONSTDC_NO_DEPRECATE
	-D_CRT_SECURE_NO_DEPRECATE
	-D_CRT_NONSTDC_NO_WARNINGS
)

INCLUDE_DIRECTORIES(${ZLIB_SRC})

SET(SRC_FILES
	${ZLIB_SRC}/adler32.c
	${ZLIB_SRC}/compress.c
	${ZLIB_SRC}/crc32.c
	${ZLIB_SRC}/deflate.c
	${ZLIB_SRC}/gzclose.c
	${ZLIB_SRC}/gzlib.c
	${ZLIB_SRC}/gzread.c
	${ZLIB_SRC}/gzwrite.c
	${ZLIB_SRC}/infback.c
	${ZLIB_SRC}/inffast.c
	${ZLIB_SRC}/inflate.c
	${ZLIB_SRC}/inftrees.c
	${ZLIB_SRC}/trees.c
	${ZLIB_SRC}/uncompr.c
	${ZLIB_SRC}/zutil.c
)

ADD_LIBRARY(osgCairo-deps-zlib STATIC ${SRC_FILES})

