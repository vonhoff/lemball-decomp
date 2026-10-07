cmake_minimum_required(VERSION 3.15)
project(lemball_decomp CXX)

option(LEMBALL_ENFORCE_STARTUP_CHECKS "Enforce the reconstructed CD and installation checks" ON)

file(GLOB_RECURSE LEMBALL_DOMAIN_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Application/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Frontend/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/GameView/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Gameplay/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Level/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Map/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Multiplayer/*.cpp"
)
file(GLOB_RECURSE LEMBALL_WINDOWS_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/Windows/*.cpp"
)
file(GLOB_RECURSE LEMBALL_MSVC_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/MSVC/*.cpp"
)
set(LEMBALL_SOURCES ${LEMBALL_DOMAIN_SOURCES} ${LEMBALL_WINDOWS_SOURCES} ${LEMBALL_MSVC_SOURCES})

set(LEMBALL_RC "${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/Windows/LEMBALL.RC")
set(LEMBALL_RESOURCE "${CMAKE_BINARY_DIR}/LEMBALL.RES")
get_filename_component(_lemball_cl_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
get_filename_component(_lemball_msvc_root "${_lemball_cl_dir}" DIRECTORY)
add_custom_command(
    OUTPUT "${LEMBALL_RESOURCE}"
    COMMAND "${LEMBALL_RC_COMPILER}" /i "${_lemball_msvc_root}/include" /fo "${LEMBALL_RESOURCE}" "${LEMBALL_RC}"
    DEPENDS "${LEMBALL_RC}"
    VERBATIM
)
set_source_files_properties("${LEMBALL_RESOURCE}" PROPERTIES
    EXTERNAL_OBJECT TRUE
    GENERATED TRUE
)

add_executable(LEMBALL WIN32 ${LEMBALL_SOURCES} ${LEMBALL_RESOURCE})
set_property(TARGET LEMBALL APPEND PROPERTY LINK_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tools/link_binary.py"
    "${CMAKE_CURRENT_SOURCE_DIR}/tools/data/linker-thunks.json"
)
target_include_directories(LEMBALL PRIVATE src)
target_compile_definitions(LEMBALL PRIVATE
    LEMBALL_1996_CXX=1
    LEMBALL_ENFORCE_STARTUP_CHECKS=$<BOOL:${LEMBALL_ENFORCE_STARTUP_CHECKS}>
)
set_property(TARGET LEMBALL PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
set_target_properties(LEMBALL PROPERTIES
    OUTPUT_NAME "LEMBALL"
    SUFFIX ".EXE"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LINK_FLAGS "user32.lib gdi32.lib advapi32.lib winmm.lib vfw32.lib version.lib wsock32.lib"
)

file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/reccmp-build.yml" CONTENT
"project: '${CMAKE_SOURCE_DIR}'
targets:
  LEMBALL:
    path: '$<TARGET_FILE:LEMBALL>'
    pdb: '${CMAKE_BINARY_DIR}/LEMBALL.pdb'
")
