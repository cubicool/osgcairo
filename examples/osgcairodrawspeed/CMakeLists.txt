PROJECT(osgcairodrawspeed)

LINK_LIBRARIES(osgCairo
	debug     osgViewer${CMAKE_DEBUG_POSTFIX}
	optimized osgViewer
	debug     osgGA${CMAKE_DEBUG_POSTFIX}
	optimized osgGA
)

ADD_EXECUTABLE(osgcairodrawspeed osgcairodrawspeed.cpp)

INSTALL(TARGETS osgcairodrawspeed DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
SET_TARGET_PROPERTIES(osgcairodrawspeed PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
