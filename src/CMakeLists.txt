
include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_BINARY_DIR}"
)

set(QBoincGui_SRCS
	main.cpp
	ElidedTextLabel.cpp
	Engine.cpp
	GuiIcon.cpp
	ui_Master.cpp
	ui_Advanced/AddClientWizard.cpp
	ui_Advanced/BoincTree.cpp
	ui_Advanced/CoreBoincPlugin.cpp
	ui_Advanced/InfoWidget.cpp
	ui_Advanced/MainWindow.cpp
	ui_Advanced/ui_Advanced.cpp
	ui_Advanced/InfoWidgets/SessionInfoWidget.cpp
)

set(QBoincGui_HDRS
	GuiIcon.h
)

set(QBoincGui_MOC_HDRS
	ElidedTextLabel.h
	Engine.h
	ui_Master.h
	ui_Advanced/AddClientWizard.h
	ui_Advanced/BoincTree.h
	ui_Advanced/CoreBoincPlugin.h
	ui_Advanced/InfoWidget.h
	ui_Advanced/BoincPlugin.h
	ui_Advanced/MainWindow.h
	ui_Advanced/ui_Advanced.h
	ui_Advanced/InfoWidgets/SessionInfoWidget.h
)

set(QBoincGui_UIS
	ui_Advanced/MainWindow.ui
	ui_Advanced/InfoWidgets/SessionInfoWidget.ui
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

