cmake_minimum_required(VERSION 3.15)
project(lemball_decomp CXX)

option(LEMBALL_ENABLE_WIN32 "Build the reconstructed Win32 executable" ON)
option(LEMBALL_SKIP_CD_TEST "Skip CD probing and use the current directory" OFF)
option(LEMBALL_SKIP_REGISTRY_RUNNING_STATE "Skip updating the registry running state" OFF)

set(LEMBALL_SOURCES
    src/engine/graphics_driver.cpp
    src/engine/memory_arena.cpp
    src/engine/render_dispatch.cpp
    src/engine/runtime_init.cpp
    src/game/game_app.cpp
    src/game/main_menu_screen.cpp
    src/game/primary_context_window.cpp
    src/game/window_owner.cpp
    src/game/eff_streams.cpp
    src/game/type18_chunk_stream.cpp
    src/game/level_runtime.cpp
    src/game/vtable_callbacks.cpp
    src/game/status_entry.cpp
    src/game/level_status.cpp
    src/game/menu_selector.cpp
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
set_property(TARGET LEMBALL_objects PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
if(LEMBALL_SKIP_CD_TEST)
    target_compile_definitions(LEMBALL_objects PRIVATE LEMBALL_SKIP_CD_TEST=1)
endif()
if(LEMBALL_SKIP_REGISTRY_RUNNING_STATE)
    target_compile_definitions(LEMBALL_objects PRIVATE LEMBALL_SKIP_REGISTRY_RUNNING_STATE=1)
endif()
if(APPLE)
    target_compile_definitions(LEMBALL_objects PRIVATE LEMBALL_HOST_MACOS=1)
endif()

if(LEMBALL_ENABLE_WIN32)
    set(LEMBALL_RESOURCE_OBJECTS)
    if(LEMBALL_RC_COMPILER)
        set(LEMBALL_RESOURCE_OBJECT "${CMAKE_CURRENT_BINARY_DIR}/LEMBALL.res")
        add_custom_command(
            OUTPUT "${LEMBALL_RESOURCE_OBJECT}"
            COMMAND "${LEMBALL_RC_COMPILER}" /r /fo "${LEMBALL_RESOURCE_OBJECT}"
                    "${CMAKE_CURRENT_SOURCE_DIR}/src/platform/lemball.rc"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/platform/lemball.rc"
            VERBATIM
        )
        set_source_files_properties("${LEMBALL_RESOURCE_OBJECT}" PROPERTIES
            GENERATED TRUE
            EXTERNAL_OBJECT TRUE
        )
        list(APPEND LEMBALL_RESOURCE_OBJECTS "${LEMBALL_RESOURCE_OBJECT}")
    endif()

    add_executable(LEMBALL WIN32 $<TARGET_OBJECTS:LEMBALL_objects> ${LEMBALL_RESOURCE_OBJECTS})
    set_property(TARGET LEMBALL PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
    set_target_properties(LEMBALL PROPERTIES
        OUTPUT_NAME "LEMBALL"
        SUFFIX ".EXE"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
    if(CMAKE_CXX_COMPILER_ID STREQUAL "OpenWatcom")
        set_target_properties(LEMBALL PROPERTIES
            LINK_FLAGS "system nt_win library kernel32.lib,user32.lib,gdi32.lib,advapi32.lib,winmm.lib,wsock32.lib,ws2_32.lib"
        )
    else()
        set_target_properties(LEMBALL PROPERTIES
            LINK_FLAGS "user32.lib gdi32.lib advapi32.lib winmm.lib wsock32.lib ws2_32.lib /MAP:LEMBALL.map"
        )
    endif()
    set(LEMBALL_DEPLOY_COMMANDS)
    foreach(LEMBALL_DEPENDENCY DSPDIB16.DLL DSPDIB32.DLL PBAIMOG.VSR)
        if(EXISTS "${CMAKE_SOURCE_DIR}/data/${LEMBALL_DEPENDENCY}")
            list(APPEND LEMBALL_DEPLOY_COMMANDS
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_SOURCE_DIR}/data/${LEMBALL_DEPENDENCY}"
                    "$<TARGET_FILE_DIR:LEMBALL>/${LEMBALL_DEPENDENCY}")
        endif()
    endforeach()
    if(LEMBALL_DEPLOY_COMMANDS)
        add_custom_command(TARGET LEMBALL POST_BUILD ${LEMBALL_DEPLOY_COMMANDS})
    endif()
endif()
