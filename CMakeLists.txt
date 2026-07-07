cmake_minimum_required(VERSION 3.15)
project(lemball_decomp CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(LEMBALL_ENABLE_WIN32 "Build the reconstructed Win32 executable" OFF)

add_library(lemball_tus OBJECT
    LEMBALL/GAME.CPP
    LEMBALL/MAIN.CPP
    LEMBALL/SHELLUI.CPP
    LEMBALL/VISOS/MOGLOAD.CPP
    LEMBALL/VISOS/VSGDI.CPP
    LEMBALL/VISOS/VSINIT.CPP
    LEMBALL/VISOS/VSMEM.CPP
    LEMBALL/VISOS/VSWIN.CPP
)

target_include_directories(lemball_tus PRIVATE LEMBALL LEMBALL/VISOS)
target_compile_definitions(lemball_tus PRIVATE LEMBALL_1996_CXX=1)

if(APPLE)
    target_compile_definitions(lemball_tus PRIVATE LEMBALL_HOST_MACOS=1)
endif()

if(MSVC)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "11.0")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Od /Ob0 /Oy- /Z7 /FdLEMBALL.PDB")
        set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "/DEBUG /PDB:LEMBALL.PDB /INCREMENTAL:NO")
        target_compile_options(lemball_tus PRIVATE /GX- /GR- /Gy- /ML)
    endif()
endif()

if(LEMBALL_ENABLE_WIN32 AND WIN32)
    add_executable(LEMBALL WIN32 $<TARGET_OBJECTS:lemball_tus>)
    set_target_properties(LEMBALL PROPERTIES OUTPUT_NAME "LEMBALL")
endif()
