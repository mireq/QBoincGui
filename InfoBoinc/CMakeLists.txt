option(DEBUG_BOINC_COMMUNICATION "Build the boincmgr - boinc communication debugging code" OFF)
option(DEBUG_MSG_COLORING "Colored debug messages" OFF)
option(DEBUG_MSG_COLORING_256 "256 colors in debug messages" OFF)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set(InfoBoinc_SRCS
	debug.cpp
	Session.cpp
	Socket.cpp
	XMLAttributeMap.cpp
	model/ProjectInfo.cpp
)

set(InfoBoinc_HDRS
	XMLAttributeMap.h
	model/ProjectInfo.h
)

set(InfoBoinc_MOC_HDRS
	Session.h
	Socket.h
)

qt4_wrap_cpp(InfoBoinc_MOC_SRCS ${InfoBoinc_MOC_HDRS})

add_library(InfoBoinc ${InfoBoinc_SRCS} ${InfoBoinc_HDRS} ${InfoBoinc_MOC_HDRS} ${InfoBoinc_MOC_SRCS})

