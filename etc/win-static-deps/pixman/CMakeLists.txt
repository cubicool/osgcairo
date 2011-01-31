PROJECT(osgCairo-deps-pixman)

ADD_DEFINITIONS(
	-MD
	-D_CRT_SECURE_NO_DEPRECATE
	-D_CRT_NONSTDC_NO_DEPRECATE
	-DPACKAGE=pixman-1
	-DPACKAGE_VERSION=""
	-DPACKAGE_BUGREPORT=""
	-DUSE_MMX
	-w14710
	-w14714
	-DUSE_SSE2
)

INCLUDE_DIRECTORIES(${SRC_PIXMAN}/pixman)

SET(SRC_FILES
	${SRC_PIXMAN}/pixman/pixman-image.c
	${SRC_PIXMAN}/pixman/pixman-access.c
	${SRC_PIXMAN}/pixman/pixman-access-accessors.c
	${SRC_PIXMAN}/pixman/pixman-region16.c
	${SRC_PIXMAN}/pixman/pixman-region32.c
	${SRC_PIXMAN}/pixman/pixman-combine32.c
	${SRC_PIXMAN}/pixman/pixman-combine64.c
	${SRC_PIXMAN}/pixman/pixman-utils.c
	${SRC_PIXMAN}/pixman/pixman-edge.c
	${SRC_PIXMAN}/pixman/pixman-edge-accessors.c
	${SRC_PIXMAN}/pixman/pixman-trap.c
	${SRC_PIXMAN}/pixman/pixman-timer.c
	${SRC_PIXMAN}/pixman/pixman-matrix.c
	${SRC_PIXMAN}/pixman/pixman-gradient-walker.c
	${SRC_PIXMAN}/pixman/pixman-linear-gradient.c
	${SRC_PIXMAN}/pixman/pixman-radial-gradient.c
	${SRC_PIXMAN}/pixman/pixman-bits-image.c
	${SRC_PIXMAN}/pixman/pixman.c
	${SRC_PIXMAN}/pixman/pixman-cpu.c
	${SRC_PIXMAN}/pixman/pixman-fast-path.c
	${SRC_PIXMAN}/pixman/pixman-implementation.c
	${SRC_PIXMAN}/pixman/pixman-solid-fill.c
	${SRC_PIXMAN}/pixman/pixman-general.c
	${SRC_PIXMAN}/pixman/pixman-mmx.c
	${SRC_PIXMAN}/pixman/pixman-sse2.c
)

ADD_LIBRARY(osgCairo-deps-pixman STATIC ${SRC_FILES})

