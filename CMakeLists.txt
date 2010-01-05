project(BoincGui C CXX)

cmake_minimum_required(VERSION 2.4)
cmake_policy(SET CMP0003 NEW)

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG   "-O2 -g3 -ggdb -Wall -Wextra -DDEBUG")

if(CMAKE_BUILD_TYPE MATCHES Debug)
	message(STATUS "Configured to compile including full debug information in the executables")
	add_definitions(-DQT_DEBUG)
else(CMAKE_BUILD_TYPE MATCHES Debug)
	message(STATUS "Configured to compile including debug information in the executables")
	set(CMAKE_BUILD_TYPE Release)
	add_definitions(-DQT_NO_DEBUG)
endif(CMAKE_BUILD_TYPE MATCHES Debug)

option(DEBUG_BOINC_COMMUNICATION "Build the boincmgr - boinc communication debugging code" OFF)
option(DEBUG_MSG_COLORING "Colored debug messages" OFF)
option(DEBUG_MSG_COLORING_256 "256 colors in debug messages" OFF)

find_package(Qt4 4.5.0 COMPONENTS QtCore QtGui QtNetwork QtXml REQUIRED)
include(${QT_USE_FILE})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories(
	"${PROJECT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/InfoBoinc"
	"${CMAKE_CURRENT_BINARY_DIR}/InfoBoinc"
	${QT_INCLUDES}
)

link_directories(${CMAKE_CURRENT_BINARY_DIR}/InfoBoinc ${CMAKE_CURRENT_BINARY_DIR}/src)

add_subdirectory(InfoBoinc)
add_subdirectory(src)

add_executable(qboincgui main.cpp)
target_link_libraries(qboincgui ${QT_LIBRARIES} AppLib InfoBoinc)

install(TARGETS qboincgui
	RUNTIME
	DESTINATION bin
	COMPONENT app)

# --- CPack ---
set(CPACK_PACKAGE_NAME "QBoincGui")
set(CPACK_PACKAGE_VENDOR "LinuxOS.sk")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Qt Boinc GUI")
set(CPACK_PACKAGE_VERSION "0.1.1")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING")
set(CPACK_COMPONENT_INCLUDES_DISPLAY_NAME "Includes")
set(CPACK_COMPONENT_INCLUDES_DESCRIPTION  "Header files")
set(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} app includes)
set(CPACK_COMPONENT_APP_HIDDEN ON)
set(CPACK_COMPONENT_APP_REQUIRED ON)
include(CPack)

