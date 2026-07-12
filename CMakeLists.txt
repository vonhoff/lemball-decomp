cmake_minimum_required(VERSION 3.15)
project(lemball_decomp CXX)

option(LEMBALL_ENABLE_WIN32 "Build the reconstructed Win32 executable" ON)

set(LEMBALL_SOURCES
    src/engine/graphics_driver.cpp
    src/engine/memory_arena.cpp
    src/engine/render_dispatch.cpp
    src/engine/runtime_init.cpp
    src/game/game_app.cpp
    src/game/window_owner.cpp
    src/game/eff_streams.cpp
    src/game/level_runtime.cpp
    src/game/vtable_callbacks.cpp
    src/game/status_entry.cpp
    src/game/level_status.cpp
    src/game/variant_resource_loader.cpp
    src/game/variant_resource_manager.cpp
    src/network/lobby.cpp
    src/network/file.cpp
    src/network/socket.cpp
    src/network/runtime.cpp
    src/network/stream.cpp
    src/main.cpp
    src/audio/audio_manager.cpp
    src/engine/fixed_math.cpp
    src/platform/message_window.cpp
    src/platform/shell_ui.cpp
    src/platform/startup_options.cpp
    src/resource/resource_archive.cpp
    src/game/demo_playback.cpp
    src/stubs/link_scaffold.cpp
)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "OpenWatcom" AND (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "11.0" OR NOT MSVC))
    set(CMAKE_CXX_STANDARD 98)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

add_library(LEMBALL_objects OBJECT ${LEMBALL_SOURCES})
target_include_directories(LEMBALL_objects PRIVATE src)
target_compile_definitions(LEMBALL_objects PRIVATE LEMBALL_1996_CXX=1)
if(APPLE)
    target_compile_definitions(LEMBALL_objects PRIVATE LEMBALL_HOST_MACOS=1)
endif()

if(LEMBALL_ENABLE_WIN32)
    add_executable(LEMBALL WIN32 $<TARGET_OBJECTS:LEMBALL_objects>)
    set_target_properties(LEMBALL PROPERTIES
        OUTPUT_NAME "LEMBALL"
        SUFFIX ".EXE"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
    if(CMAKE_CXX_COMPILER_ID STREQUAL "OpenWatcom")
        set_target_properties(LEMBALL PROPERTIES
            LINK_FLAGS "system nt_win library kernel32.lib,user32.lib,gdi32.lib,advapi32.lib,winmm.lib,ws2_32.lib,wsock32.lib"
        )
    else()
        set_target_properties(LEMBALL PROPERTIES
            LINK_FLAGS "user32.lib gdi32.lib advapi32.lib winmm.lib ws2_32.lib wsock32.lib /MAP:LEMBALL.map"
        )
    endif()
endif()
