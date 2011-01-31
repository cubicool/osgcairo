PROJECT(osgCairo-deps-expat)

# -D_DEBUG

ADD_DEFINITIONS(
	-DWIN32
	-D_WINDOWS
	-DCOMPILED_FROM_DSP
	-D_LIB
)

INCLUDE_DIRECTORIES(
	${EXPAT_SRC}/lib
)

SET(SRC_FILES
	${EXPAT_SRC}/lib/xmlparse.c
	${EXPAT_SRC}/lib/xmlrole.c
	${EXPAT_SRC}/lib/xmltok.c
	${EXPAT_SRC}/lib/xmltok_impl.c
	${EXPAT_SRC}/lib/xmltolk_ns.c
)

ADD_LIBRARY(osgCairo-deps-expat STATIC ${SRC_FILES})

