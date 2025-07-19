cmake_minimum_required(VERSION 3.28...3.30)
project(replayer_project)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/libuiohook)

if (WIN32)
    set(PLATFORM_SRC platform_win.h platform_win.cpp)
    add_definitions(-DPLATFORM_WINDOWS)
elseif (UNIX)
    set(PLATFORM_SRC platform_x11.h platform_x11.cpp)
    add_definitions(-DPLATFORM_LINUX)
endif()
add_executable(replayer platform.h ${PLATFORM_SRC} replay.h replay.cpp main.cpp)

set(OBS_DIR "${CMAKE_SOURCE_DIR}/external/obs-studio")

target_include_directories(replayer PRIVATE
    external/obs-studio/libobs
    external/libuiohook/include
)

if(WIN32)
    set(LIBOBS_LIB "${OBS_DIR}/build-windows/libobs/Debug/obs.lib")
else()
    set(LIBOBS_LIB "${OBS_DIR}/build-linux/rundir/RelWithDebInfo/lib/libobs.so.30")
    set_target_properties(replayer PROPERTIES
        BUILD_RPATH "${OBS_DIR}/build-linux/rundir/RelWithDebInfo/lib"
        INSTALL_RPATH "$ORIGIN/../lib/replayer/"
    )
endif()

target_link_libraries(replayer PRIVATE ${LIBOBS_LIB} uiohook)

install(TARGETS replayer RUNTIME)