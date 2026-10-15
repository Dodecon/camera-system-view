cmake_minimum_required(VERSION 3.16)

project(Cameras VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------
# Qt
# ---------------------------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

# ---------------------------------------------------------------
# spdlog
# ---------------------------------------------------------------
find_package(spdlog REQUIRED)

# ---------------------------------------------------------------
# Sources — all .cpp/.h in the project tree, any subfolder.
# CONFIGURE_DEPENDS re-checks on every build, so new files are
# picked up without touching this file.
# ---------------------------------------------------------------
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)
list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/build/.*")

qt_add_executable(Cameras
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    resources/external.qrc
)

# ---------------------------------------------------------------
# FFmpeg (libav): avformat avcodec avutil swscale swresample
#   Windows: FFMPEG_DIR = install prefix (C:/msys64/ucrt64).
#            Already set via Qt Creator, with a default fallback below.
#   Linux:   found automatically in standard system paths.
# ---------------------------------------------------------------
if(WIN32 AND NOT FFMPEG_DIR)
    set(FFMPEG_DIR "C:/msys64/ucrt64")
endif()

find_path(FFMPEG_INCLUDE_DIR libavformat/avformat.h HINTS ${FFMPEG_DIR}/include)
foreach(_lib avformat avcodec avutil swscale swresample)
    find_library(FFMPEG_${_lib}_LIBRARY NAMES ${_lib} HINTS ${FFMPEG_DIR}/lib)
endforeach()

if(NOT FFMPEG_INCLUDE_DIR
   OR NOT FFMPEG_avformat_LIBRARY OR NOT FFMPEG_avcodec_LIBRARY
   OR NOT FFMPEG_avutil_LIBRARY OR NOT FFMPEG_swscale_LIBRARY
   OR NOT FFMPEG_swresample_LIBRARY)
    message(FATAL_ERROR
        "FFmpeg not found. Set FFMPEG_DIR to its install prefix "
        "(e.g. C:/msys64/ucrt64) and re-run CMake.")
endif()

message(STATUS "FFmpeg include : ${FFMPEG_INCLUDE_DIR}")
message(STATUS "FFmpeg avformat: ${FFMPEG_avformat_LIBRARY}")

add_library(ffmpeg INTERFACE)
target_include_directories(ffmpeg INTERFACE ${FFMPEG_INCLUDE_DIR})
target_link_libraries(ffmpeg INTERFACE
    ${FFMPEG_avformat_LIBRARY}
    ${FFMPEG_avcodec_LIBRARY}
    ${FFMPEG_avutil_LIBRARY}
    ${FFMPEG_swscale_LIBRARY}
    ${FFMPEG_swresample_LIBRARY}
)

# ---------------------------------------------------------------
# Link
# ---------------------------------------------------------------
target_link_libraries(Cameras PRIVATE
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    spdlog::spdlog
    ffmpeg
    $<$<BOOL:${MINGW}>:ws2_32>
)

target_include_directories(Cameras PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(Cameras PRIVATE PROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# ---------------------------------------------------------------
# Windows: copy FFmpeg runtime DLLs next to the exe after build
# ---------------------------------------------------------------
if(WIN32)
    get_filename_component(FFMPEG_ROOT_DIR "${FFMPEG_avformat_LIBRARY}" DIRECTORY) # .../lib
    get_filename_component(FFMPEG_ROOT_DIR "${FFMPEG_ROOT_DIR}" DIRECTORY)         # .../ucrt64
    file(GLOB FFMPEG_RUNTIME_DLLS
        "${FFMPEG_ROOT_DIR}/bin/avcodec-*.dll"
        "${FFMPEG_ROOT_DIR}/bin/avformat-*.dll"
        "${FFMPEG_ROOT_DIR}/bin/avutil-*.dll"
        "${FFMPEG_ROOT_DIR}/bin/swscale-*.dll"
        "${FFMPEG_ROOT_DIR}/bin/swresample-*.dll"
    )
    foreach(_dll ${FFMPEG_RUNTIME_DLLS})
        add_custom_command(TARGET Cameras POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_dll}" $<TARGET_FILE_DIR:Cameras>
        )
    endforeach()
endif()

set_target_properties(Cameras PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Cameras
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(Cameras)