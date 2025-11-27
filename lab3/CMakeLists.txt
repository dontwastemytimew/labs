cmake_minimum_required(VERSION 3.29)
project(lab3)

set(CMAKE_PREFIX_PATH "D:/Qt/6.5.3/mingw_64")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Svg LinguistTools PrintSupport REQUIRED)

add_library(QCustomPlotLib STATIC libs/qcustomplot.cpp libs/qcustomplot.h)
target_link_libraries(QCustomPlotLib PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets Qt6::PrintSupport)
target_include_directories(QCustomPlotLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs)

set(PROJECT_SOURCES
        main.cpp
        resources.qrc
        src/core/datamanager.h src/core/datamanager.cpp
        src/core/schema.h      src/core/schema.cpp
        src/core/note.h        src/core/note.cpp
        src/utils/logger.h     src/utils/logger.cpp
        src/gui/mainwindow.h     src/gui/mainwindow.cpp     src/gui/mainwindow.ui
        src/gui/settingsdialog.h src/gui/settingsdialog.cpp src/gui/settingsdialog.ui
        src/gui/schemamanager.h  src/gui/schemamanager.cpp  src/gui/schemamanager.ui
        src/gui/schemaeditor.h   src/gui/schemaeditor.cpp   src/gui/schemaeditor.ui
        src/gui/noteeditor.h     src/gui/noteeditor.cpp     src/gui/noteeditor.ui
        src/gui/tageditor.h      src/gui/tageditor.cpp      src/gui/tageditor.ui
        src/gui/notewidget.h     src/gui/notewidget.cpp     src/gui/notewidget.ui
        src/gui/statisticsdialog.h src/gui/statisticsdialog.cpp src/gui/statisticsdialog.ui
        libs/nanobench.h
)

add_executable(lab3 ${PROJECT_SOURCES})

target_include_directories(lab3 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gui
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/libs
)

target_link_libraries(lab3
        PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Svg
        Qt6::PrintSupport
        QCustomPlotLib
)

add_executable(lab3_tests
        tests/tests_main.cpp
        tests/test_models.h
        tests/test_datamanager.h
        tests/test_io.h

        src/core/datamanager.cpp
        src/core/note.cpp
        src/core/schema.cpp
)

target_include_directories(lab3_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core
        ${CMAKE_CURRENT_SOURCE_DIR}/libs
)

target_link_libraries(lab3_tests PRIVATE Qt6::Core)

if (WIN32)
    set(QT_BIN_DIR "${CMAKE_PREFIX_PATH}/bin")
    set(QT_PLUGIN_DIR "${CMAKE_PREFIX_PATH}/plugins")
    set(QT_LIBS Core Gui Widgets Svg PrintSupport)

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

set(TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/i18n/lab3_en_US.ts)
qt_add_translations(lab3
        QM_FILES_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/i18n"
        TS_FILES ${TS_FILES}
        SOURCES ${PROJECT_SOURCES}
)
target_sources(lab3 PRIVATE ${lab3_TRANSLATION_FILES})

target_precompile_headers(lab3 PRIVATE
        <QApplication> <QMainWindow> <QDialog> <QPushButton>
        <QLabel> <QList> <QVector> <QString> <QFile> <QDebug>
        <vector> <string> <fstream>
        "libs/json.hpp"
        "libs/qcustomplot.h"
)