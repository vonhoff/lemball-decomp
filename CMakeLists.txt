cmake_minimum_required(VERSION 3.15)
project(lemball_decomp CXX)

option(LEMBALL_ENABLE_WIN32 "Build the reconstructed Win32 executable" OFF)

set(LEMBALL_SOURCES
    src/game/geff.cpp
    src/game/gmcb.cpp
    src/game/gvariant.cpp
    src/game.cpp
    src/main.cpp
    src/shellui.cpp
    src/startup.cpp
    src/visos/mogload.cpp
    src/visos/vsaudio.cpp
    src/visos/vsdemo.cpp
    src/visos/vsgdi.cpp
    src/visos/vsinit.cpp
    src/visos/vsmath.cpp
    src/visos/vsmem.cpp
    src/visos/vswin.cpp
)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "OpenWatcom" AND (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "11.0" OR NOT MSVC))
    set(CMAKE_CXX_STANDARD 98)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if(LEMBALL_ENABLE_WIN32)
    add_executable(LEMBALL WIN32 ${LEMBALL_SOURCES})
    target_include_directories(LEMBALL PRIVATE src src/visos)
    target_compile_definitions(LEMBALL PRIVATE LEMBALL_1996_CXX=1)
    if(APPLE)
        target_compile_definitions(LEMBALL PRIVATE LEMBALL_HOST_MACOS=1)
    endif()
    set_target_properties(LEMBALL PROPERTIES
        OUTPUT_NAME "LEMBALL"
        SUFFIX ".EXE"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
    if(CMAKE_CXX_COMPILER_ID STREQUAL "OpenWatcom")
        set_target_properties(LEMBALL PROPERTIES
            LINK_FLAGS "system nt_win library kernel32.lib,user32.lib,advapi32.lib"
        )
    else()
        set_target_properties(LEMBALL PROPERTIES
            LINK_FLAGS "user32.lib advapi32.lib"
        )
    endif()
endif()
