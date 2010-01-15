
include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_BINARY_DIR}"
)

set(QBoincGui_SRCS
	main.cpp
	Engine.cpp
	ui_Master.cpp
	ui_Advanced/AddClientWizard.cpp
	ui_Advanced/BoincTree.cpp
	ui_Advanced/MainWindow.cpp
	ui_Advanced/ui_Advanced.cpp
)

set(QBoincGui_HDRS
)

set(QBoincGui_MOC_HDRS
	Engine.h
	ui_Master.h
	ui_Advanced/AddClientWizard.h
	ui_Advanced/BoincTree.h
	ui_Advanced/MainWindow.h
	ui_Advanced/ui_Advanced.h
)

set(QBoincGui_UIS
	ui_Advanced/MainWindow.ui
)

set(QBoincGui_QRC
	resources.qrc
)

qt4_wrap_cpp(QBoincGui_MOC_SRCS ${QBoincGui_MOC_HDRS})
qt4_wrap_ui(QBoincGui_UIS_SRCS ${QBoincGui_UIS})
qt4_add_resources(QBoincGui_QRC_SRCS ${QBoincGui_QRC})

add_executable(qboincgui
	${QBoincGui_SRCS}
	${QBoincGui_HDRS}
	${QBoincGui_MOC_HDRS}
	${QBoincGui_UIS}
	${QBoincGui_QRC}
	${QBoincGui_MOC_SRCS}
	${QBoincGui_UIS_SRCS}
	${QBoincGui_QRC_SRCS})

target_link_libraries(qboincgui ${QT_LIBRARIES} InfoBoinc)

install(TARGETS qboincgui
	RUNTIME
	DESTINATION bin
	COMPONENT app)

