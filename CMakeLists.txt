cmake_minimum_required(VERSION 3.15)
project(lemball_decomp CXX)

option(LEMBALL_ENFORCE_STARTUP_CHECKS "Enforce the reconstructed CD and installation checks" ON)

# MSVC 4.00 register allocation depends on include spelling even with identical
# preprocessed tokens. Keep compiler identities and link order stable on moves.
find_package(Python3 REQUIRED COMPONENTS Interpreter)
execute_process(
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/stage_sources.py"
        --root "${CMAKE_CURRENT_SOURCE_DIR}" --build "${CMAKE_BINARY_DIR}"
    RESULT_VARIABLE _lemball_stage_result
)
if(NOT _lemball_stage_result EQUAL 0)
    message(FATAL_ERROR "Failed to prepare MSVC source inputs")
endif()
include("${CMAKE_BINARY_DIR}/compiler-sources.cmake")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    ${LEMBALL_SOURCE_INPUTS}
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/source-layout.json"
    "${CMAKE_CURRENT_SOURCE_DIR}/tools/stage_sources.py"
)

set(LEMBALL_RC "${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/Windows/Resources/LEMBALL.RC")
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
target_include_directories(LEMBALL PRIVATE "${CMAKE_BINARY_DIR}/compiler-src")
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
