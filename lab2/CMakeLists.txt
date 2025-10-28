cmake_minimum_required(VERSION 3.29)
project(lab2)
set(CMAKE_PREFIX_PATH "D:/Qt/6.5.3/mingw_64")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Svg
        REQUIRED)

add_executable(lab2
        main.cpp
        mainwindow.h
        mainwindow.cpp
        mainwindow.ui
        schemaeditor.h
        schemaeditor.cpp
        schema.cpp
        datamanager.h
        datamanager.cpp
        note.h
        note.cpp
        noteeditor.h
        noteeditor.cpp
        noteeditor.ui
        schemamanager.cpp
        schemamanager.h
        schemamanager.ui
        resources.qrc
        notewidget.h
        notewidget.cpp
        notewidget.ui
        tageditor.h
        tageditor.cpp
        tageditor.ui
)
target_link_libraries(lab2
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Svg
)

if (WIN32)
    set(QT_BIN_DIR "${CMAKE_PREFIX_PATH}/bin")
    set(QT_PLUGIN_DIR "${CMAKE_PREFIX_PATH}/plugins")

    set(QT_LIBS Core Gui Widgets Svg)

    foreach(QT_LIB ${QT_LIBS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${QT_BIN_DIR}/Qt6${QT_LIB}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
        )
    endforeach()

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_PLUGIN_DIR}/platforms/qwindows.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/"
    )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/imageformats"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_PLUGIN_DIR}/imageformats/qsvg.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/imageformats/"
    )
endif()