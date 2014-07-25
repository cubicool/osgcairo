PROJECT(osgCairo-deps-zlib)

ADD_DEFINITIONS(
	-DWIN32
	-DZLIB_WINAPI
	-D_CRT_NONSTDC_NO_DEPRECATE
	-D_CRT_SECURE_NO_DEPRECATE
	-D_CRT_NONSTDC_NO_WARNINGS
)

INCLUDE_DIRECTORIES(${SRC_ZLIB})

SET(SRC_FILES
	${SRC_ZLIB}/adler32.c
	${SRC_ZLIB}/compress.c
	${SRC_ZLIB}/crc32.c
	${SRC_ZLIB}/deflate.c
	${SRC_ZLIB}/gzclose.c
	${SRC_ZLIB}/gzlib.c
	${SRC_ZLIB}/gzread.c
	${SRC_ZLIB}/gzwrite.c
	${SRC_ZLIB}/infback.c
	${SRC_ZLIB}/inffast.c
	${SRC_ZLIB}/inflate.c
	${SRC_ZLIB}/inftrees.c
	${SRC_ZLIB}/trees.c
	${SRC_ZLIB}/uncompr.c
	${SRC_ZLIB}/zutil.c
)

ADD_LIBRARY(osgCairo-deps-zlib STATIC ${SRC_FILES})

