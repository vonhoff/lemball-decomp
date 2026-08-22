cmake_minimum_required(VERSION 3.15)
project(lemball_decomp CXX)

file(GLOB_RECURSE LEMBALL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(LEMBALL_RC "${CMAKE_CURRENT_SOURCE_DIR}/src/LEMBALL.RC")
set(LEMBALL_RESOURCE "${CMAKE_CURRENT_BINARY_DIR}/LEMBALL.RES")
add_custom_command(
    OUTPUT "${LEMBALL_RESOURCE}"
    COMMAND "${LEMBALL_RC_COMPILER}" /fo "${LEMBALL_RESOURCE}" "${LEMBALL_RC}"
    DEPENDS "${LEMBALL_RC}"
    VERBATIM
)
set_source_files_properties("${LEMBALL_RESOURCE}" PROPERTIES
    EXTERNAL_OBJECT TRUE
    GENERATED TRUE
)

add_executable(LEMBALL WIN32 ${LEMBALL_SOURCES} ${LEMBALL_RESOURCE})
target_include_directories(LEMBALL PRIVATE src)
target_compile_definitions(LEMBALL PRIVATE LEMBALL_1996_CXX=1)
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
