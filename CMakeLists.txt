cmake_minimum_required(VERSION 3.16)

project(Asterindes VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOGEN_BUILD_DIR "${CMAKE_BINARY_DIR}/autogen")

set(ASTERINDES_DEVELOPMENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/development)
set(ASTERINDES_SOURCES
	${ASTERINDES_DEVELOPMENT_DIR}/src/asterindes.cpp
)


find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick)

qt_standard_project_setup()

qt_add_executable(Asterindes
	${ASTERINDES_SOURCES}
)

target_include_directories(Asterindes PRIVATE ${ASTERINDES_DEVELOPMENT_DIR}/inc)

qt_add_qml_module(Asterindes
	URI Asterindes
	VERSION 0.1
	RESOURCE_PREFIX /qt/qml
	QML_FILES
		"development/res/Main.qml"
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
