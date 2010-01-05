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

