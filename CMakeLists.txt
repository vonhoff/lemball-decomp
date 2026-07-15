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
    src/VISOS/GDI/VSGDI.CPP
    src/VISOS/MEMORY/VSMEM.CPP
    src/VISOS/GDI/VSWING.CPP
    src/VISOS/CORE/VSINIT.CPP
    src/VISOS/DEBUG/RAWOUT.CPP
    src/VISOS/GDI/VSWINDOW.CPP
    src/VISOS/STREAM/EFFSTRM.CPP
    src/VISOS/GDI/WINDOWVT.CPP
    src/VISOS/GDI/GDIVT.CPP
    src/VISOS/SOUND/SOUNDVT.CPP
    src/VISOS/STREAM/STREAMVT.CPP
    src/VISOS/DEBUG/DEBUGVT.CPP
    src/VISOS/STAT/VSSTAT.CPP
    src/VISOS/NET/VSNETFIL.CPP
    src/VISOS/NET/VSNETTCP.CPP
    src/VISOS/NET/VSNET.CPP
    src/VISOS/NET/VSNETSTR.CPP
    src/VISOS/SOUND/VSSOUND.CPP
    src/VISOS/CORE/VSMATH.CPP
    src/VISOS/GDI/INVISWIN.CPP
    src/VISOS/DEBUG/TEXTWIN.CPP
    src/VISOS/RESOURCE/MOGLOAD.CPP
    src/VISOS/CORE/LINKSCF.CPP
)

set(LEMBALL_SOURCES
    src/LEMBALL/GAME.CPP
    src/LEMBALL/FRONTEND/MAINMENU.CPP
    src/LEMBALL/SHELL/PRIMARY.CPP
    src/LEMBALL/LEVEL/TYPE18.CPP
    src/LEMBALL/LEVEL/LEVELRUN.CPP
    src/LEMBALL/LEVEL/LEVELVT.CPP
    src/LEMBALL/FRONTEND/SCREENVT.CPP
    src/LEMBALL/FRONTEND/LOBBYVT.CPP
    src/LEMBALL/LEVEL/LEVELSTAT.CPP
    src/LEMBALL/FRONTEND/MENUSEL.CPP
    src/LEMBALL/RESOURCE/VARLOAD.CPP
    src/LEMBALL/RESOURCE/VARMGR.CPP
    src/LEMBALL/FRONTEND/LOBBY.CPP
    src/LEMBALL/SHELL/ENTRY.CPP
    src/LEMBALL/SHELL/SHELLUI.CPP
    src/LEMBALL/SHELL/OPTIONS.CPP
    src/LEMBALL/LEVEL/DEMO.CPP
)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "OpenWatcom" AND (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "11.0" OR NOT MSVC))
    set(CMAKE_CXX_STANDARD 98)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

add_library(VISOS STATIC ${VISOS_SOURCES})
target_include_directories(VISOS
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
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
                    "${CMAKE_CURRENT_SOURCE_DIR}/src/LEMBALL/LEMBALL.RC"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/LEMBALL/LEMBALL.RC"
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
        # Keep COMDAT functions until their binary-backed callers or vtables
        # are reconstructed.
        lemball_force_include(LEMBALL
            "?_RAWOUT_DebugString@@YAHPBD@Z"
            "?initialize_debug_text_line_record@@YIPAXPAX@Z"
            "?noop_embedded_mci_callback_0046e050@@YGXPAX@Z"
            "?two_array_list_is_first_entry_loaded_false@@YAHXZ"
            "?SetStateIdWithVariantLocalThunk@LevelManagedEntityStateFields@@QAEXHH@Z"
            "?CopyThunk@LevelThreeDwordPoint@@QAEPAU1@PBU1@@Z"
            "?CopyThunk@LevelTwoDwordPair@@QAEPAU1@PBU1@@Z"
            "?CopyThunk@LevelThreeDwordPointVariant@@QAEPAU1@PBU1@@Z"
            "?GetRequestCodeThunk@LevelQueuedProjectileRequestTable@@QAEHH@Z"
            "?CopyThunk@LevelThreeDwordPointVariantB@@QAEPAU1@PBU1@@Z"
            "?CopyWorldPositionThunk@LevelManagedEntityPositionView@@QAEPAULevelThreeDwordPoint@@PAU2@@Z"
            "?GetPositionThunk@LevelManagedEntityQueueCursor@@QAEXPAH0@Z"
            "?HasPendingCommandsThunk@LevelManagedEntityCommandQueueView@@QAEHXZ"
            "?get_object_field_0x160@@YIHPAX@Z"
            "?get_object_field_0x124@@YIHPAX@Z"
            "?return_object_field_0x0c_00435880@@YIHPAX@Z"
            "?set_managed_entity_state_id_local@@YIXPAX0H@Z"
            "?set_managed_entity_owner_group@@YIXPAX00@Z"
            "?get_managed_entity_owner_group@@YIPAXPAX@Z"
            "?set_managed_entity_primary_active_flag@@YIXPAX0H@Z"
            "?set_managed_entity_secondary_active_flag@@YIXPAX0H@Z"
            "?add_level_time_bonus_counter@@YIXPAX0H@Z"
            "?set_level_mode_field_0xf8@@YIXPAX0H@Z"
            "?set_managed_entity_group_command_state@@YIXPAX0H@Z"
            "?set_object_field_0x160@@YIXPAX0H@Z"
            "?IsManagedEntityState18@@YGHH@Z"
            "?IsManagedEntityState18Thunk@@YGHH@Z"
            "?ForwardManagedEntityVslot9C@@YAXPAX0@Z"
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
