include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_BINARY_DIR}"
)

set(AppLib_SRCS
	Engine.cpp
	ui_Master.cpp
	ui_Advanced/MainWindow.cpp
	ui_Advanced/ui_Advanced.cpp
)

set(AppLib_HDRS
	ui_Master.h
	ui_Advanced/ui_Advanced.h
)

set(AppLib_MOC_HDRS
	Engine.h
	ui_Advanced/MainWindow.h
)

set(AppLib_UIS
	ui_Advanced/MainWindow.ui
)

qt4_wrap_cpp(AppLib_MOC_SRCS ${AppLib_MOC_HDRS})
qt4_wrap_ui(AppLib_UIS_SRCS ${AppLib_UIS})

add_library(AppLib
	${AppLib_SRCS}
	${AppLib_HDRS}
	${AppLib_MOC_HDRS}
	${AppLib_UIS}
	${AppLib_QRC}
	${AppLib_MOC_SRCS}
	${AppLib_UIS_SRCS})

