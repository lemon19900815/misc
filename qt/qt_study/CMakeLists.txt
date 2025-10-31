cmake_minimum_required(VERSION 3.5)

project(qt_study VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        sortview.h
        sortview.cpp
        currencymodel.h
        currencymodel.cpp
        filesystemwidget.h
        filesystemwidget.cpp
        mylistview.h
        mylistview.cpp
        mylistviewdelegate.h
        mylistviewdelegate.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

add_executable(qt_study
    ${PROJECT_SOURCES}
)
target_link_libraries(qt_study PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(qt_study PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS qt_study
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qt_study)
endif()
