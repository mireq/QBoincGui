
include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_BINARY_DIR}"
)

set(QBoincGui_SRCS
	main.cpp
	ElidedTextLabel.cpp
	Engine.cpp
	GuiIcon.cpp
	PresentationSingleton.cpp
	ui_Master.cpp
	ui_Advanced/AddClientWizard.cpp
	ui_Advanced/BoincTree.cpp
	ui_Advanced/BoincTreeItems.cpp
	ui_Advanced/CoreBoincPlugin.cpp
	ui_Advanced/InfoWidget.cpp
	ui_Advanced/MainWindow.cpp
	ui_Advanced/ProgressItemDelegate.cpp
	ui_Advanced/ui_Advanced.cpp
	ui_Advanced/InfoWidgets/ProjectInfoWidget.cpp
	ui_Advanced/InfoWidgets/SessionInfoWidget.cpp
	ui_Advanced/InfoWidgets/SystemInfoWidget.cpp
	ui_Advanced/InfoWidgets/TransfersInfoWidget.cpp
)

set(QBoincGui_HDRS
	GuiIcon.h
	PresentationSingleton.h
	ui_Advanced/BoincTreeItems.h
)

set(QBoincGui_MOC_HDRS
	ElidedTextLabel.h
	Engine.h
	ui_Master.h
	ui_Advanced/AddClientWizard.h
	ui_Advanced/BoincPlugin.h
	ui_Advanced/BoincTree.h
	ui_Advanced/CoreBoincPlugin.h
	ui_Advanced/InfoWidget.h
	ui_Advanced/MainWindow.h
	ui_Advanced/ProgressItemDelegate.h
	ui_Advanced/ui_Advanced.h
	ui_Advanced/InfoWidgets/ProjectInfoWidget.h
	ui_Advanced/InfoWidgets/SessionInfoWidget.h
	ui_Advanced/InfoWidgets/SystemInfoWidget.h
	ui_Advanced/InfoWidgets/TransfersInfoWidget.h
)

set(QBoincGui_UIS
	ui_Advanced/MainWindow.ui
	ui_Advanced/InfoWidgets/ProjectInfoWidget.ui
	ui_Advanced/InfoWidgets/SessionInfoWidget.ui
	ui_Advanced/InfoWidgets/SystemInfoWidget.ui
	ui_Advanced/InfoWidgets/TransfersInfoWidget.ui
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

