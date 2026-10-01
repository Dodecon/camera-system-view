cmake_minimum_required(VERSION 3.5)

project(Cameras VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Added Multimedia and MultimediaWidgets
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

find_package(spdlog REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Cameras
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        securitycamera.h securitycamera.cpp
        resources/no-connection.jpeg
        camcontainer.h camcontainer.cpp


        cameramenu.h cameramenu.cpp
        resources/external.qrc
        logListner.h
        cameraview.h cameraview.cpp
        camerabuilder.h camerabuilder.cpp
    )
else()
    if(ANDROID)
        add_library(Cameras SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Cameras
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Added the new modules to the link line
target_link_libraries(Cameras PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
    spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
)

target_compile_definitions(Cameras PRIVATE PROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Cameras)
endif()
set_target_properties(Cameras PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Cameras
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBRARY}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Cameras)

endif()
