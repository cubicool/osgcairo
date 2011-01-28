PROJECT(osgCairo-deps-zlib)

SET(
	ZLIB_SRC
	"${OSGCAIRO_DEPS_BASE}/zlib/zlib-1.2.5"
	CACHE PATH
	"The extracted zlib source"
)

ADD_DEFINITIONS(
	-DWIN32
	-DZLIB_WINAPI
	-D_CRT_NONSTDC_NO_DEPRECATE
	-D_CRT_SECURE_NO_DEPRECATE
	-D_CRT_NONSTDC_NO_WARNINGS
)

SET(SRC_FILES
	${ZLIB_SRC}/adler32.c
	${ZLIB_SRC}/compress.c
	${ZLIB_SRC}/crc32.c
	${ZLIB_SRC}/deflate.c
	${ZLIB_SRC}/gzclose.c
	${ZLIB_SRC}/gzguts.h
	${ZLIB_SRC}/gzlib.c
	${ZLIB_SRC}/gzread.c
	${ZLIB_SRC}/gzwrite.c
	${ZLIB_SRC}/infback.c
	${ZLIB_SRC}/inffast.c
	${ZLIB_SRC}/inflate.c
	${ZLIB_SRC}/inftrees.c
	${ZLIB_SRC}/minizip\ioapi.c
	${ZLIB_SRC}/minizip\unzip.c
	${ZLIB_SRC}/minizip\zip.c
	${ZLIB_SRC}/trees.c
	${ZLIB_SRC}/uncompr.c
	${ZLIB_SRC}/zutil.c
)

SET(HEADER_FILES
	${ZLIB_SRC}/deflate.h
	${ZLIB_SRC}/infblock.h
	${ZLIB_SRC}/infcodes.h
	${ZLIB_SRC}/inffast.h
	${ZLIB_SRC}/inftrees.h
	${ZLIB_SRC}/infutil.h
	${ZLIB_SRC}/zconf.h
	${ZLIB_SRC}/zlib.h
	${ZLIB_SRC}/zutil.h
)

SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})
SOURCE_GROUP("Source Files" FILES ${SRC_FILES})

ADD_LIBRARY(osgCairo-deps-zlib STATIC ${SRC_FILES} ${HEADER_FILES})

