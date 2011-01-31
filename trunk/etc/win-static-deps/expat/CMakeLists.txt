PROJECT(osgCairo-deps-expat)

# -D_DEBUG

ADD_DEFINITIONS(
	-DWIN32
	-D_WINDOWS
	-DCOMPILED_FROM_DSP
	-D_LIB
)

INCLUDE_DIRECTORIES(
	${SRC_EXPAT}/lib
)

SET(SRC_FILES
	${SRC_EXPAT}/lib/xmlparse.c
	${SRC_EXPAT}/lib/xmlrole.c
	${SRC_EXPAT}/lib/xmltok.c
	${SRC_EXPAT}/lib/xmltok_impl.c
	${SRC_EXPAT}/lib/xmltok_ns.c
)

ADD_LIBRARY(osgCairo-deps-expat STATIC ${SRC_FILES})

