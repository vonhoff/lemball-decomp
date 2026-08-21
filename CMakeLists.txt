cmake_minimum_required(VERSION 3.15)
project(lemball_decomp CXX)

# Order is significant for the reconstructed executable.
set(LEMBALL_SOURCES
        src_old/Control/CGame.cpp
        src_old/Frontend/GAMEEVT.CPP
        src_old/views/2d/C2D.cpp
        src_old/views/2d/C2DRender.cpp
        src_old/Control/CGameHelp.cpp
        src_old/Control/CGameVT.cpp
        src_old/Frontend/MAINMENU.CPP
        src_old/Frontend/MMRENDCH.CPP
        src_old/Frontend/MMCONTEXT.CPP
        src_old/Platform/Windows/Primary.cpp
        src_old/AI/TYPE18.CPP
        src_old/AI/TYPE18RESET.CPP
        src_old/AI/LV35BOUND.CPP
        src_old/Control/CGameStatusPasswordBits.cpp
        src_old/Platform/Windows/Mixed/Level/ENTITY/LVMODESLOT.CPP
        src_old/AI/LVTRIPLET.CPP
        src_old/AI/LVOWNOBJ.CPP
        src_old/AI/LVDELMOVE.CPP
        src_old/AI/LVSTREAM.CPP
        src_old/Platform/Windows/Mixed/Level/RUN/LEVELRUN.CPP
        src_old/AI/CMaze.cpp
        src_old/AI/CMineManager.cpp
        src_old/AI/CMove3d.cpp
        src_old/AI/CMover.cpp
        src_old/AI/LVNODE.CPP
        src_old/Platform/Windows/Mixed/Level/ENTITY/LVTILE.CPP
        src_old/Platform/Windows/Mixed/Level/IO/LVSTRVT.CPP
        src_old/Platform/Windows/Mixed/Level/IO/LVSTRWRT.CPP
        src_old/Platform/Windows/Mixed/Level/IO/LVSTRREAD.CPP
        src_old/Platform/Windows/Mixed/Level/STATE/LVQUIT.CPP
        src_old/Platform/Windows/Mixed/Level/RENDER/LVRENDISP.CPP
        src_old/AI/LVSTATEVAR.CPP
        src_old/Platform/Windows/Mixed/Level/ENTITY/LVCHFLAGS.CPP
        src_old/AI/LVPOSCOPY.CPP
        src_old/Visos/Generic/LVSTRFMT.CPP
        src_old/Platform/Windows/Mixed/Level/VTABLE/LEVELVT.CPP
        src_old/AI/AICoord.cpp
        src_old/AI/LVVTDEF.CPP
        src_old/AI/LVPOSVT.CPP
        src_old/AI/LVACTDSP.CPP
        src_old/AI/CGameObject.cpp
        src_old/AI/CGameObjectMove.cpp
        src_old/AI/CGameObjectLifecycle.cpp
        src_old/AI/CGameObjectLift.cpp
        src_old/AI/CGlobalGameObject.cpp
        src_old/AI/CFormationManager.cpp
        src_old/AI/CGenericGroup.cpp
        src_old/AI/CPlayerLemmingGroup.cpp
        src_old/AI/CPlayerLemmingGroupManager.cpp
        src_old/AI/CPlayerLemming.cpp
        src_old/AI/CAI.cpp
        src_old/AI/CAICursor.cpp
        src_old/AI/CAmmo.cpp
        src_old/AI/CBall.cpp
        src_old/AI/CBallManager.cpp
        src_old/AI/CBalloonPost.cpp
        src_old/AI/CBaseRemap.cpp
        src_old/AI/CBullet.cpp
        src_old/AI/CCollectableManager.cpp
        src_old/AI/CCrate.cpp
        src_old/AI/CDoorManager.cpp
        src_old/AI/CDuplicator.cpp
        src_old/AI/CGenericGroupManager.cpp
        src_old/AI/CMoverManager.cpp
        src_old/AI/CResFONT.cpp
        src_old/AI/CRocket.cpp
        src_old/AI/CRocketManager.cpp
        src_old/AI/CSheep.cpp
        src_old/AI/CSheepGroup.cpp
        src_old/AI/CSlinky.cpp
        src_old/AI/CSlinkyManager.cpp
        src_old/AI/CTower.cpp
        src_old/AI/LVMOVEVT.CPP
        src_old/Platform/Windows/Mixed/Level/IO/LVNETVT.CPP
        src_old/AI/LVPROJVT.CPP
        src_old/Frontend/SCREENVT.CPP
        src_old/Frontend/SCRVTDEF.CPP
        src_old/Platform/Windows/Mixed/Level/STATE/LEVELSTAT.CPP
        src_old/views/2d/LVSTATMODE.CPP
        src_old/views/2d/LVSTATUS.CPP
        src_old/views/2d/LVSTATRG.CPP
        src_old/Frontend/MENUSEL.CPP
        src_old/Frontend/MENUSELRECT.CPP
        src_old/Frontend/MSSPREVT.CPP
        src_old/Frontend/MSRESCHD.CPP
        src_old/Frontend/MENUSELSINK.CPP
        src_old/Frontend/MENUSELPAUSE.CPP
        src_old/Platform/Windows/Mixed/Resource/VARLOAD.CPP
        src_old/Platform/Windows/Mixed/Resource/VARMGR.CPP
        src_old/Frontend/LOBBY.CPP
        src_old/Frontend/CEditString.cpp
        src_old/Platform/Windows/Entry.cpp
        src_old/Platform/Windows/ShellUI.cpp
        src_old/Control/Options.cpp
        src_old/Platform/Windows/Mixed/Level/RUN/DEMO.CPP
        src_old/Visos/Windows/CGDI.cpp
        src_old/Visos/Windows/CGDISwitch.cpp
        src_old/Visos/Windows/CGDI240.cpp
        src_old/Visos/Generic/Memory.cpp
        src_old/Platform/Windows/Mixed/Engine/GDI/WINDOW/VSWING.CPP
        src_old/Platform/Windows/Mixed/Engine/GDI/WINDOW/CBaseQueue.cpp
        src_old/Visos/Windows/VSWINGRECT.CPP
        src_old/Visos/Windows/VGAIO.CPP
        src_old/Platform/Windows/Mixed/Engine/CORE/VSINIT.CPP
        src_old/Platform/Windows/Mixed/Engine/CORE/VSFORMAT.CPP
        src_old/Platform/Windows/Mixed/Engine/CORE/VSRETURN.CPP
        src_old/Visos/Windows/RAWOUT.CPP
        src_old/Platform/Windows/Mixed/Engine/GDI/WINDOW/VSWINDOW.CPP
        src_old/Platform/Windows/Mixed/Engine/GDI/WINDOW/VSWINDOWMODE.CPP
        src_old/Platform/Windows/Mixed/Engine/GDI/RENDER/GEOMETRYVT.CPP
        src_old/Platform/Windows/Mixed/Engine/GDI/RENDER/GDISTATUSVT.CPP
        src_old/Visos/Generic/CPVSurface.cpp
        src_old/Platform/Windows/Mixed/Engine/GDI/RENDER/GDIRENDERVT.CPP
        src_old/Visos/Generic/CSurface.cpp
        src_old/Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.CPP
        src_old/Platform/Windows/Mixed/Engine/MEDIA/EFFVT.CPP
        src_old/Platform/Windows/Mixed/Engine/GDI/WINDOW/WINDOWSLOT38.CPP
        src_old/Platform/Windows/Mixed/Engine/GDI/RENDER/GDIVT.CPP
        src_old/Platform/Windows/Mixed/Engine/GDI/WINDOW/VSSPROWN.CPP
        src_old/Visos/Windows/Debug/DEBUGVT.CPP
        src_old/Visos/Generic/VSSTAT.CPP
        src_old/Platform/Windows/Mixed/Engine/NET/VSNETFIL.CPP
        src_old/Platform/Windows/Mixed/Engine/NET/VSNETTCP.CPP
        src_old/Platform/Windows/Mixed/Engine/NET/VSNET.CPP
        src_old/Platform/Windows/Mixed/Engine/NET/VSNETSTR.CPP
        src_old/Platform/Windows/Mixed/Engine/NET/NSTRBSTH.CPP
        src_old/Platform/Windows/Mixed/Engine/MEDIA/VSSOUND.CPP
        src_old/Visos/Generic/VsMathFixed12Vector2D.cpp
        src_old/Visos/Generic/SignedTrigTable.cpp
        src_old/Visos/Generic/CVSMath.cpp
        src_old/Visos/Generic/CVSPoint.cpp
        src_old/Visos/Windows/INVISWIN.CPP
        src_old/Visos/Windows/Debug/TEXTWIN.CPP
        src_old/Visos/Windows/Debug/TXWININV.CPP
        src_old/Visos/Windows/Debug/TEXTLINE.CPP
        src_old/Visos/Generic/MogLoad.cpp
        src_old/Visos/Generic/MogPatch.cpp
        src_old/Visos/Generic/MogConst.cpp
        src_old/Visos/Generic/MogBst.cpp
        src_old/Platform/Windows/Mixed/Engine/CORE/LINKSCF.CPP
        src_old/Platform/Windows/Mixed/Engine/CORE/ManagedEntityGroupQuery.cpp
        src_old/Platform/Windows/Mixed/Engine/CORE/LINKSCFBYTES.CPP
        src_old/AI/Projectile.cpp
        src_old/AI/GroupState.cpp
        src_old/AI/CInvisibleSwitch.cpp
        src_old/Control/ClusterScanOps.cpp
        src_old/Control/ClusterScanOps2.cpp
        src_old/Control/LargePoolRehome.cpp
        src_old/Control/LargePoolZrle.cpp
        src_old/AI/CInvisibleSwitchManager.cpp
        src_old/AI/CLaser.cpp
        src_old/AI/CLaserManager.cpp
        src_old/AI/CLift.cpp
        src_old/AI/CLiftManager.cpp
        src_old/AI/CTrapDoor.cpp
        src_old/AI/CDoorAct.cpp
        src_old/AI/CDoorSlot.cpp
        src_old/AI/CHand.cpp
        src_old/AI/CHandManager.cpp
        src_old/AI/CIce.cpp
        src_old/AI/CIceManager.cpp
        src_old/AI/CTrampoline.cpp
        src_old/AI/CTrampolineManager.cpp
        src_old/AI/CPaintGun.cpp
        src_old/AI/CPaintGunManager.cpp
        src_old/AI/CLiftAct.cpp
        src_old/AI/ChildState.cpp
        src_old/AI/LVITER.CPP
        src_old/AI/CGameObjectCollision.cpp
        src_old/AI/LVPKT.CPP
        src_old/AI/LVPKTTH.CPP
        src_old/AI/LVRSTTH.CPP
        src_old/AI/LVSAVEPOS.CPP
        src_old/Platform/Windows/Mixed/Level/STATE/LVPROFILE.CPP
        src_old/Control/CGameStatusLevel.cpp
        src_old/AI/CGameObjectId.cpp
        src_old/AI/LVBSTH.CPP
        src_old/AI/CDoorCtor.cpp
        src_old/AI/PlasChild.cpp
        src_old/AI/CCatapult.cpp
        src_old/AI/CEnemyGrp.cpp
        src_old/AI/CMine.cpp
        src_old/AI/CObjectManager.cpp
        src_old/AI/GroupPlas.cpp
        src_old/AI/CHUNKOBJVT.CPP
        src_old/AI/CLiftRun.cpp
        src_old/AI/CHOBJDEL.CPP
        src_old/AI/CHOBJRST.CPP
        src_old/AI/COLLSCORE.CPP
        src_old/Platform/Windows/Mixed/Level/CHUNK/ANIMREC.CPP
        src_old/AI/PROJRST.CPP
        src_old/Platform/Windows/Mixed/Level/IO/DRAWTEXT.CPP
        src_old/Visos/Generic/TXTCNT.CPP
        src_old/Visos/Generic/TXTCONST.CPP
        src_old/Frontend/MMPASSTH.CPP
        src_old/Frontend/MMPASS.CPP
        src_old/AI/LVRANDVT.CPP
        src_old/AI/LVPREDVT.CPP
        src_old/AI/LVPRJGEOM.CPP
        src_old/AI/LVGMOBPT.CPP
        src_old/views/2d/LVRUNEXT.CPP
        src_old/Platform/Windows/Mixed/Level/STATE/LevelStatusPanelRender.cpp
        src_old/Visos/Windows/CGDICircleOutline.cpp
        src_old/Visos/Windows/CGDILocalRect.cpp
)

set(LEMBALL_RC "${CMAKE_CURRENT_SOURCE_DIR}/src_old/LEMBALL.RC")
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
target_include_directories(LEMBALL PRIVATE src_old)
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
