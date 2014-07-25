PROJECT(osgcairoutil)

LINK_LIBRARIES(osgCairo
	debug     osgViewer${CMAKE_DEBUG_POSTFIX}
	optimized osgViewer
	debug     osgGA${CMAKE_DEBUG_POSTFIX}
	optimized osgGA
)

ADD_EXECUTABLE(osgcairoutil osgcairoutil.cpp)

INSTALL(TARGETS osgcairoutil DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
SET_TARGET_PROPERTIES(osgcairoutil PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
