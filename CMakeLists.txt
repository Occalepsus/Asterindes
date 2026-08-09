cmake_minimum_required(VERSION 3.16)

project(Asterindes VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOGEN_BUILD_DIR "${CMAKE_BINARY_DIR}/autogen")

# Set Asterindes as the default startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

set(ASTERINDES_DEVELOPMENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/development)

set(ASTERINDES_HEADERS
	${ASTERINDES_DEVELOPMENT_DIR}/inc/AsterindesCore.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/AsterindesProject.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/BroadcastServer.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/ProjectManagerService.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/ProjectViewModel.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/ProjectWindow.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/ResourceBroadcastManager.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/ResourceListModel.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/ResourceRegistry.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/ResourceSortFilterProxyModel.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/ResourcesViewModel.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/SingleInstanceGuard.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/StartupWindow.h
)

set(ASTERINDES_SOURCES
	${ASTERINDES_DEVELOPMENT_DIR}/src/AsterindesApp.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/AsterindesCore.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/AsterindesProject.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/BroadcastServer.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/ProjectManagerService.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/ProjectViewModel.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/ProjectWindow.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/ResourceBroadcastManager.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/ResourceListModel.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/ResourceRegistry.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/ResourceSortFilterProxyModel.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/ResourcesViewModel.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/SingleInstanceGuard.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/StartupWindow.cpp
)

set(ASTERINDES_QML_FILES
	"development/res/AsterindesMenuBar.qml"
	"development/res/Main.qml"
	"development/res/ResourceBroadcastPanel.qml"
	"development/res/ResourcePreviewPanel.qml"
	"development/res/ResourcesGridPanel.qml"
	"development/res/StartupWindow.qml"
	"development/res/TagSelectionList.qml"
)

set(ASTERINDES_HTML_RESOURCES
	"development/res/ClientPage.html"
)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick HttpServer WebSockets)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
	${ASTERINDES_SOURCES}
	${ASTERINDES_HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${ASTERINDES_DEVELOPMENT_DIR}/inc)

qt_add_qml_module(${PROJECT_NAME}
	URI ${PROJECT_NAME}
	VERSION 0.1
	RESOURCE_PREFIX /qt/qml
	QML_FILES
		${ASTERINDES_QML_FILES}
)

foreach(resource IN LISTS ASTERINDES_HTML_RESOURCES)
	get_filename_component(resourceName "${resource}" NAME)
	set_source_files_properties("${resource}" PROPERTIES QT_RESOURCE_ALIAS "${resourceName}")
endforeach()


qt_add_resources(${PROJECT_NAME} "HTML"
	PREFIX "/html"
	FILES
		${ASTERINDES_HTML_RESOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Qml Qt6::Quick Qt6::HttpServer Qt6::WebSockets)

if(WIN32)
	find_program(WINDEPLOYQT windeployqt HINTS ${Qt6_DIR}/../../../bin)
	if(WINDEPLOYQT)
		set(ASTERINDES_QML_DIR "${ASTERINDES_DEVELOPMENT_DIR}/res")
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${WINDEPLOYQT} --no-translations --qmldir "${ASTERINDES_QML_DIR}" $<TARGET_FILE:${PROJECT_NAME}>
			COMMENT "Deploying Qt DLLs with windeployqt"
		)
	endif()
endif()

# Organize the project files in Visual Studio in folders
source_group("Source Files" FILES ${ASTERINDES_SOURCES})
source_group("Header Files" FILES ${ASTERINDES_HEADERS})
source_group("Resources/qml" FILES ${ASTERINDES_QML_FILES})
source_group("Resources/html" FILES ${ASTERINDES_HTML_RESOURCES})

# Set the application to be a Windows application only in Release mode
set_target_properties(${PROJECT_NAME} PROPERTIES
	WIN32_EXECUTABLE $<$<CONFIG:Release>:ON>
)
