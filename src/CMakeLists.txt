set(AppLib_SRCS
	Engine.cpp
)

set(AppLib_MOC_HDRS
	Engine.h
)

qt4_wrap_cpp(AppLib_MOC_SRCS ${AppLib_MOC_HDRS})

add_library(AppLib ${AppLib_MOC_SRCS} ${AppLib_SRCS})

