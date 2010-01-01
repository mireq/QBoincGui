set(InfoBoinc_SRCS
	debug.cpp
	Socket.cpp
)

set(InfoBoinc_MOC_HDRS
	Socket.h
)

qt4_wrap_cpp(InfoBoinc_MOC_SRCS ${InfoBoinc_MOC_HDRS})

add_library(InfoBoinc ${InfoBoinc_MOC_SRCS} ${InfoBoinc_SRCS})

