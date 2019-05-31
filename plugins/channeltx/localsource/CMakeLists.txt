project(localsource)

set(localsource_SOURCES
	localsource.cpp
	localsourcethread.cpp
	localsourceplugin.cpp
	localsourcesettings.cpp
)

set(localsource_HEADERS
	localsource.h
	localsourcethread.h
	localsourceplugin.h
	localsourcesettings.h
)

include_directories(
    ${CMAKE_SOURCE_DIR}/swagger/sdrangel/code/qt5/client
    ${Boost_INCLUDE_DIRS}
)

if(NOT SERVER_MODE)
    set(localsource_SOURCES
        ${localsource_SOURCES}
        localsourcegui.cpp

        localsourcegui.ui
    )
    set(localsource_HEADERS
        ${localsource_HEADERS}
        localsourcegui.h
    )

    set(TARGET_NAME localsource)
    set(TARGET_LIB "Qt5::Widgets")
    set(TARGET_LIB_GUI "sdrgui")
    set(INSTALL_FOLDER ${INSTALL_PLUGINS_DIR})
else()
    set(TARGET_NAME localsourcesrv)
    set(TARGET_LIB "")
    set(TARGET_LIB_GUI "")
    set(INSTALL_FOLDER ${INSTALL_PLUGINSSRV_DIR})
endif()

add_library(${TARGET_NAME} SHARED
	${localsource_SOURCES}
)

target_link_libraries(${TARGET_NAME}
        Qt5::Core
        ${TARGET_LIB}
	sdrbase
	${TARGET_LIB_GUI}
        swagger
)

install(TARGETS ${TARGET_NAME} DESTINATION ${INSTALL_FOLDER})