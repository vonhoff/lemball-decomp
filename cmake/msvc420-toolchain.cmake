# MSVC 4.20 toolchain for byte-matching LEMBALL builds.
# This bypasses most modern CMake compiler machinery and drives the original
# compiler and linker with explicit rules, similar to the legoland setup.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR x86)

get_filename_component(_tc_dir "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
get_filename_component(_root_dir "${_tc_dir}/.." ABSOLUTE)

set(CMAKE_CXX_COMPILER "${_root_dir}/msvc420/bin/CL.EXE")
set(CMAKE_CXX_COMPILER_ID_RUN TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

set(CMAKE_CXX_OUTPUT_EXTENSION ".obj")
set(CMAKE_EXECUTABLE_SUFFIX ".exe")
set(CMAKE_INCLUDE_FLAG_CXX "/I")
set(CMAKE_CXX_DEFINE_FLAG "/D")
set(CMAKE_DEPFILE_FLAGS_CXX "")
set(CMAKE_LINK_LIBRARY_FLAG "")
set(CMAKE_LINK_LIBRARY_SUFFIX "")
set(CMAKE_C_USE_RESPONSE_FILE_FOR_OBJECTS 1)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1)
set(CMAKE_C_RESPONSE_FILE_LINK_FLAG "@")
set(CMAKE_CXX_RESPONSE_FILE_LINK_FLAG "@")
set(CMAKE_NMAKE_FORCE_RESPONSE_FILE 1)

set(CMAKE_CXX_FLAGS_INIT "/nologo /W3 /GX- /GR- /MT")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "")

set(CMAKE_CXX_COMPILE_OBJECT
    "<CMAKE_CXX_COMPILER> <DEFINES> <INCLUDES> <FLAGS> /TP /O2 /Ob1 /Oy /G4 /Z7 /c /Fo<OBJECT> <SOURCE>")

set(CMAKE_CXX_LINK_EXECUTABLE
    "python ${_root_dir}/tools/check_link_output.py ${_root_dir}/msvc420/bin/LINK.EXE /nologo /DEBUG /INCREMENTAL:NO /SUBSYSTEM:WINDOWS <LINK_FLAGS> /OUT:<TARGET> <OBJECTS> <LINK_LIBRARIES>")
