PROJECT(osgCairo-deps-pixman)

SET(
	PIXMAN_SRC
	"${OSGCAIRO_DEPS_BASE}/build/pixman-0.20.0"
	CACHE PATH
	"The extracted pixman source"
)

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

INCLUDE_DIRECTORIES(${PIXMAN_SRC}/pixman)

SET(SRC_FILES
	${PIXMAN_SRC}/pixman/pixman-image.c
	${PIXMAN_SRC}/pixman/pixman-access.c
	${PIXMAN_SRC}/pixman/pixman-access-accessors.c
	${PIXMAN_SRC}/pixman/pixman-region16.c
	${PIXMAN_SRC}/pixman/pixman-region32.c
	${PIXMAN_SRC}/pixman/pixman-combine32.c
	${PIXMAN_SRC}/pixman/pixman-combine64.c
	${PIXMAN_SRC}/pixman/pixman-utils.c
	${PIXMAN_SRC}/pixman/pixman-edge.c
	${PIXMAN_SRC}/pixman/pixman-edge-accessors.c
	${PIXMAN_SRC}/pixman/pixman-trap.c
	${PIXMAN_SRC}/pixman/pixman-timer.c
	${PIXMAN_SRC}/pixman/pixman-matrix.c
	${PIXMAN_SRC}/pixman/pixman-gradient-walker.c
	${PIXMAN_SRC}/pixman/pixman-linear-gradient.c
	${PIXMAN_SRC}/pixman/pixman-radial-gradient.c
	${PIXMAN_SRC}/pixman/pixman-bits-image.c
	${PIXMAN_SRC}/pixman/pixman.c
	${PIXMAN_SRC}/pixman/pixman-cpu.c
	${PIXMAN_SRC}/pixman/pixman-fast-path.c
	${PIXMAN_SRC}/pixman/pixman-implementation.c
	${PIXMAN_SRC}/pixman/pixman-solid-fill.c
	${PIXMAN_SRC}/pixman/pixman-general.c
	${PIXMAN_SRC}/pixman/pixman-mmx.c
	${PIXMAN_SRC}/pixman/pixman-sse2.c
)

ADD_LIBRARY(osgCairo-deps-pixman STATIC ${SRC_FILES})

