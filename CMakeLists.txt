cmake_minimum_required(VERSION 3.16)

project(Asterindes VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOGEN_BUILD_DIR "${CMAKE_BINARY_DIR}/autogen")

# Set Asterindes as the default startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Asterindes)

set(ASTERINDES_DEVELOPMENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/development)

set(ASTERINDES_HEADERS
	${ASTERINDES_DEVELOPMENT_DIR}/inc/AsterindesCore.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/ProjectManager.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/Resource.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/ResourcesManager.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/ResourceListModel.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/ResourcesViewModel.h
	${ASTERINDES_DEVELOPMENT_DIR}/inc/GuiClient.h
)

set(ASTERINDES_SOURCES
	${ASTERINDES_DEVELOPMENT_DIR}/src/AsterindesApp.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/AsterindesCore.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/ProjectManager.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/Resource.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/ResourcesManager.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/ResourceListModel.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/ResourcesViewModel.cpp
	${ASTERINDES_DEVELOPMENT_DIR}/src/GuiClient.cpp
)

set(ASTERINDES_QML_FILES
	"development/res/Main.qml"
	"development/res/ResourcePreview.qml"
	"development/res/ResourcesGridPanel.qml"
)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick)

qt_standard_project_setup()

qt_add_executable(Asterindes
	${ASTERINDES_SOURCES}
	${ASTERINDES_HEADERS}
)

target_include_directories(Asterindes PRIVATE ${ASTERINDES_DEVELOPMENT_DIR}/inc)

qt_add_qml_module(Asterindes
	URI Asterindes
	VERSION 0.1
	RESOURCE_PREFIX /qt/qml
	QML_FILES
		${ASTERINDES_QML_FILES}
)

target_link_libraries(Asterindes PRIVATE Qt6::Core Qt6::Qml Qt6::Quick)

if(WIN32)
	find_program(WINDEPLOYQT windeployqt HINTS ${Qt6_DIR}/../../../bin)
	if(WINDEPLOYQT)
		set(ASTERINDES_QML_DIR "${ASTERINDES_DEVELOPMENT_DIR}/res")
		add_custom_command(TARGET Asterindes POST_BUILD
			COMMAND ${WINDEPLOYQT} --no-translations --qmldir "${ASTERINDES_QML_DIR}" $<TARGET_FILE:Asterindes>
			COMMENT "Déploiement des DLLs Qt avec windeployqt"
		)
	endif()
endif()

# Organize the project files in Visual Studio in folders
source_group("Source Files" FILES ${ASTERINDES_SOURCES})
source_group("Header Files" FILES ${ASTERINDES_HEADERS})
source_group("Resources" FILES ${ASTERINDES_QML_FILES})
