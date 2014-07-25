PROJECT(osgCairo-deps-fontconfig)

ADD_DEFINITIONS(
	-DWIN32
	-D_WIN32
	_D_LIB
)

INCLUDE_DIRECTORIES(
	${SRC_FREETYPE}/include
	${SRC_EXPAT}/lib
	${SRC_FONTCONFIG}/src
)

SET(SRC_FILES
	${SRC_FONTCONFIG}/src/fcdefault.c
	${SRC_FONTCONFIG}/src/fcblanks.c
	${SRC_FONTCONFIG}/src/fcftint.h
	${SRC_FONTCONFIG}/src/fcinit.c
	${SRC_FONTCONFIG}/src/fcint.h
	${SRC_FONTCONFIG}/src/fcfs.c
	${SRC_FONTCONFIG}/src/fccharset.c
	${SRC_FONTCONFIG}/src/fclist.c
	${SRC_FONTCONFIG}/src/fcstr.c
	${SRC_FONTCONFIG}/src/fcdeprecate.h
	${SRC_FONTCONFIG}/src/fcformat.c
	${SRC_FONTCONFIG}/src/fcmatrix.c
	${SRC_FONTCONFIG}/src/fcserialize.c
	${SRC_FONTCONFIG}/src/fccfg.c
	${SRC_FONTCONFIG}/src/fcpat.c
	${SRC_FONTCONFIG}/src/fcxml.c
	${SRC_FONTCONFIG}/src/fclang.c
	${SRC_FONTCONFIG}/src/fcdir.c
	${SRC_FONTCONFIG}/src/fcname.c
	${SRC_FONTCONFIG}/src/fcfreetype.c
	${SRC_FONTCONFIG}/src/fcmatch.c
	${SRC_FONTCONFIG}/src/fcatomic.c
	${SRC_FONTCONFIG}/src/fccache.c
	${SRC_FONTCONFIG}/src/ftglue.c
	${SRC_FONTCONFIG}/src/fcdbg.c
)

ADD_LIBRARY(osgCairo-deps-fontconfig STATIC ${SRC_FILES})
