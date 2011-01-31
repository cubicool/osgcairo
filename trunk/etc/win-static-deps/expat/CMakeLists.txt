PROJECT(osgCairo-deps-expat)

# -D_DEBUG

ADD_DEFINITIONS(
	-DWIN32
	-D_WINDOWS
	-DCOMPILED_FROM_DSP
	-D_LIB
)

INCLUDE_DIRECTORIES(
)

SET(SRC_FILES
	xmlparse.c
	xmlrole.c
	xmltok.c
	xmltok_impl.c
	xmltolk_ns.c
)

ADD_LIBRARY(osgCairo-deps-expat STATIC ${SRC_FILES})

