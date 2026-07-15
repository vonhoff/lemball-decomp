cmake_minimum_required(VERSION 3.15)
project(lemball_decomp CXX)

function(lemball_force_include target)
    foreach(symbol ${ARGN})
        set_property(TARGET ${target} APPEND_STRING PROPERTY
            LINK_FLAGS " /INCLUDE:${symbol}")
    endforeach()
endfunction()

option(LEMBALL_ENABLE_WIN32 "Build the reconstructed Win32 executable" ON)
option(LEMBALL_SKIP_CD_TEST "Skip CD probing and use the current directory" OFF)
option(LEMBALL_SKIP_REGISTRY_RUNNING_STATE "Skip updating the registry running state" OFF)

set(VISOS_SOURCES
    src/ENGINE/GDI/VSGDI.CPP
    src/ENGINE/CORE/VSMEM.CPP
    src/ENGINE/GDI/VSWING.CPP
    src/ENGINE/CORE/VSINIT.CPP
    src/ENGINE/DEBUG/RAWOUT.CPP
    src/ENGINE/GDI/VSWINDOW.CPP
    src/ENGINE/MEDIA/EFFSTRM.CPP
    src/ENGINE/GDI/WINDOWVT.CPP
    src/ENGINE/GDI/GDIVT.CPP
    src/ENGINE/MEDIA/SOUNDVT.CPP
    src/ENGINE/MEDIA/STREAMVT.CPP
    src/ENGINE/DEBUG/DEBUGVT.CPP
    src/ENGINE/CORE/VSSTAT.CPP
    src/ENGINE/NET/VSNETFIL.CPP
    src/ENGINE/NET/VSNETTCP.CPP
    src/ENGINE/NET/VSNET.CPP
    src/ENGINE/NET/VSNETSTR.CPP
    src/ENGINE/MEDIA/VSSOUND.CPP
    src/ENGINE/CORE/VSMATH.CPP
    src/ENGINE/GDI/INVISWIN.CPP
    src/ENGINE/DEBUG/TEXTWIN.CPP
    src/RESOURCE/MOGLOAD.CPP
    src/ENGINE/CORE/LINKSCF.CPP
)

set(LEMBALL_SOURCES
    src/GAME.CPP
    src/FRONTEND/MAINMENU.CPP
    src/SHELL/PRIMARY.CPP
    src/LEVEL/TYPE18.CPP
    src/LEVEL/LEVELRUN.CPP
    src/LEVEL/LEVELVT.CPP
    src/FRONTEND/SCREENVT.CPP
    src/FRONTEND/LOBBYVT.CPP
    src/LEVEL/LEVELSTAT.CPP
    src/FRONTEND/MENUSEL.CPP
    src/RESOURCE/VARLOAD.CPP
    src/RESOURCE/VARMGR.CPP
    src/FRONTEND/LOBBY.CPP
    src/SHELL/ENTRY.CPP
    src/SHELL/SHELLUI.CPP
    src/SHELL/OPTIONS.CPP
    src/LEVEL/DEMO.CPP
)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "OpenWatcom" AND (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "11.0" OR NOT MSVC))
    set(CMAKE_CXX_STANDARD 98)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

add_library(VISOS STATIC ${VISOS_SOURCES})
target_include_directories(VISOS
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_compile_definitions(VISOS PRIVATE LEMBALL_1996_CXX=1)
set_property(TARGET VISOS PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
if(LEMBALL_SKIP_CD_TEST)
    target_compile_definitions(VISOS PRIVATE LEMBALL_SKIP_CD_TEST=1)
endif()

if(LEMBALL_SKIP_REGISTRY_RUNNING_STATE)
    target_compile_definitions(VISOS PRIVATE LEMBALL_SKIP_REGISTRY_RUNNING_STATE=1)
endif()
if(APPLE)
    target_compile_definitions(VISOS PRIVATE LEMBALL_HOST_MACOS=1)
endif()

if(LEMBALL_ENABLE_WIN32)
    set(LEMBALL_RESOURCE_OBJECTS)
    if(LEMBALL_RC_COMPILER)
        set(LEMBALL_RESOURCE_OBJECT "${CMAKE_CURRENT_BINARY_DIR}/LEMBALL.res")
        add_custom_command(
            OUTPUT "${LEMBALL_RESOURCE_OBJECT}"
            COMMAND "${LEMBALL_RC_COMPILER}" /r /fo "${LEMBALL_RESOURCE_OBJECT}"
                    "${CMAKE_CURRENT_SOURCE_DIR}/src/LEMBALL.RC"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/LEMBALL.RC"
            VERBATIM
        )
        set_source_files_properties("${LEMBALL_RESOURCE_OBJECT}" PROPERTIES
            GENERATED TRUE
            EXTERNAL_OBJECT TRUE
        )
        list(APPEND LEMBALL_RESOURCE_OBJECTS "${LEMBALL_RESOURCE_OBJECT}")
    endif()

    add_executable(LEMBALL WIN32 ${LEMBALL_SOURCES} ${LEMBALL_RESOURCE_OBJECTS})
    target_include_directories(LEMBALL PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
    target_link_libraries(LEMBALL PRIVATE VISOS)
    target_compile_definitions(LEMBALL PRIVATE LEMBALL_1996_CXX=1)
    if(LEMBALL_SKIP_CD_TEST)
        target_compile_definitions(LEMBALL PRIVATE LEMBALL_SKIP_CD_TEST=1)
    endif()
    if(LEMBALL_SKIP_REGISTRY_RUNNING_STATE)
        target_compile_definitions(LEMBALL PRIVATE LEMBALL_SKIP_REGISTRY_RUNNING_STATE=1)
    endif()
    if(APPLE)
        target_compile_definitions(LEMBALL PRIVATE LEMBALL_HOST_MACOS=1)
    endif()
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
        # Pull the four otherwise-unreferenced VISOS archive members that
        # contain annotated binary-owned callbacks and debug helpers.
        set_property(TARGET LEMBALL APPEND_STRING PROPERTY LINK_FLAGS
            " @${CMAKE_CURRENT_SOURCE_DIR}/cmake/msvc420-force-symbols.rsp")
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
