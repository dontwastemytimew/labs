cmake_minimum_required(VERSION 3.29)
project(lab2)
set(PROJECT_SOURCES
        main.cpp
        mainwindow.h mainwindow.cpp mainwindow.ui
        datamanager.h datamanager.cpp
        schema.h schema.cpp
        note.h note.cpp
        schemamanager.h schemamanager.cpp schemamanager.ui
        schemaeditor.h schemaeditor.cpp schemaeditor.ui
        noteeditor.h noteeditor.cpp noteeditor.ui
        tageditor.h tageditor.cpp tageditor.ui
        notewidget.h notewidget.cpp notewidget.ui
        resources.qrc
        logger.cpp
        logger.h
)
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
        LinguistTools
        REQUIRED)

add_executable(lab2 ${PROJECT_SOURCES})

add_executable(lab2_tests
        tests/tests_main.cpp
        tests/test_models.h
        tests/test_datamanager.h
        tests/test_io.h
        datamanager.cpp
        note.cpp
        schema.cpp
)

target_link_libraries(lab2
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Svg
)

target_link_libraries(lab2_tests PRIVATE
        Qt6::Core
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


set(TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/i18n/lab2_en_US.ts)


qt_add_translations(lab2
        QM_FILES_OUTPUT_DIR "${CMAKE_BINARY_DIR}/i18n"
        TS_FILES ${TS_FILES})


target_sources(lab2 PRIVATE ${lab2_TRANSLATION_FILES})
