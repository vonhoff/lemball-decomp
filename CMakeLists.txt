cmake_minimum_required(VERSION 3.15)
project(lemball_decomp CXX)

# Order is significant for the reconstructed executable.
set(LEMBALL_SOURCES
    src/Control/CGame.cpp
    src/Frontend/GAMEEVT.CPP
    src/views/2d/C2D.cpp
    src/Control/CGameHelp.cpp
    src/Control/CGameVT.cpp
    src/Frontend/MAINMENU.CPP
    src/Frontend/MMRENDCH.CPP
    src/Frontend/MMCONTEXT.CPP
    src/Platform/Windows/Primary.cpp
    src/AI/TYPE18.CPP
    src/AI/TYPE18RESET.CPP
    src/AI/LV35BOUND.CPP
    src/Control/CGameStatusPasswordBits.cpp
    src/Platform/Windows/Mixed/Level/ENTITY/LVMODESLOT.CPP
    src/AI/LVTRIPLET.CPP
    src/AI/LVOWNOBJ.CPP
    src/AI/LVDELMOVE.CPP
    src/AI/LVSTREAM.CPP
    src/Platform/Windows/Mixed/Level/RUN/LEVELRUN.CPP
    src/AI/CMaze.cpp
    src/AI/CMineManager.cpp
    src/AI/CMove3d.cpp
    src/AI/CMover.cpp
    src/AI/LVNODE.CPP
    src/Platform/Windows/Mixed/Level/ENTITY/LVTILE.CPP
    src/Platform/Windows/Mixed/Level/IO/LVSTRVT.CPP
    src/Platform/Windows/Mixed/Level/IO/LVSTRWRT.CPP
    src/Platform/Windows/Mixed/Level/IO/LVSTRREAD.CPP
    src/Platform/Windows/Mixed/Level/STATE/LVQUIT.CPP
    src/Platform/Windows/Mixed/Level/RENDER/LVRENDISP.CPP
    src/AI/LVSTATEVAR.CPP
    src/Platform/Windows/Mixed/Level/ENTITY/LVCHFLAGS.CPP
    src/AI/LVPOSCOPY.CPP
    src/Visos/Generic/LVSTRFMT.CPP
    src/Platform/Windows/Mixed/Level/VTABLE/LEVELVT.CPP
    src/AI/LVVTDEF.CPP
    src/AI/LVPOSVT.CPP
    src/AI/LVACTDSP.CPP
    src/AI/CGameObjectMove.cpp
    src/AI/CGameObjectLifecycle.cpp
    src/AI/CGameObjectLift.cpp
    src/AI/CGlobalGameObject.cpp
    src/AI/CFormationManager.cpp
    src/AI/CGenericGroup.cpp
    src/AI/CPlayerLemmingGroup.cpp
    src/AI/CPlayerLemmingGroupManager.cpp
    src/AI/CPlayerLemming.cpp
    src/AI/LVMOVEVT.CPP
    src/Platform/Windows/Mixed/Level/IO/LVNETVT.CPP
    src/AI/LVPROJVT.CPP
    src/Frontend/SCREENVT.CPP
    src/Frontend/SCRVTDEF.CPP
    src/Platform/Windows/Mixed/Level/STATE/LEVELSTAT.CPP
    src/views/2d/LVSTATMODE.CPP
    src/views/2d/LVSTATUS.CPP
    src/views/2d/LVSTATRG.CPP
    src/Frontend/MENUSEL.CPP
    src/Frontend/MENUSELRECT.CPP
    src/Frontend/MSSPREVT.CPP
    src/Frontend/MSRESCHD.CPP
    src/Frontend/MENUSELSINK.CPP
    src/Frontend/MENUSELPAUSE.CPP
    src/Platform/Windows/Mixed/Resource/VARLOAD.CPP
    src/Platform/Windows/Mixed/Resource/VARMGR.CPP
    src/Frontend/LOBBY.CPP
    src/Frontend/CEditString.cpp
    src/Platform/Windows/Entry.cpp
    src/Platform/Windows/ShellUI.cpp
    src/Control/Options.cpp
    src/Platform/Windows/Mixed/Level/RUN/DEMO.CPP
    src/Visos/Windows/CGDI.cpp
    src/Visos/Windows/CGDISwitch.cpp
    src/Visos/Windows/CGDI240.cpp
    src/Visos/Generic/Memory.cpp
    src/Platform/Windows/Mixed/Engine/GDI/WINDOW/VSWING.CPP
    src/Visos/Windows/VSWINGRECT.CPP
    src/Visos/Windows/VGAIO.CPP
    src/Platform/Windows/Mixed/Engine/CORE/VSINIT.CPP
    src/Platform/Windows/Mixed/Engine/CORE/VSFORMAT.CPP
    src/Platform/Windows/Mixed/Engine/CORE/VSRETURN.CPP
    src/Visos/Windows/RAWOUT.CPP
    src/Platform/Windows/Mixed/Engine/GDI/WINDOW/VSWINDOW.CPP
    src/Platform/Windows/Mixed/Engine/GDI/WINDOW/VSWINDOWMODE.CPP
    src/Platform/Windows/Mixed/Engine/GDI/RENDER/GEOMETRYVT.CPP
    src/Platform/Windows/Mixed/Engine/GDI/RENDER/GDISTATUSVT.CPP
    src/Visos/Generic/CPVSurface.cpp
    src/Platform/Windows/Mixed/Engine/GDI/RENDER/GDIRENDERVT.CPP
    src/Visos/Generic/CSurface.cpp
    src/Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.CPP
    src/Platform/Windows/Mixed/Engine/MEDIA/EFFVT.CPP
    src/Platform/Windows/Mixed/Engine/GDI/WINDOW/WINDOWSLOT38.CPP
    src/Platform/Windows/Mixed/Engine/GDI/RENDER/GDIVT.CPP
    src/Platform/Windows/Mixed/Engine/GDI/WINDOW/VSSPROWN.CPP
    src/Visos/Windows/Debug/DEBUGVT.CPP
    src/Visos/Generic/VSSTAT.CPP
    src/Platform/Windows/Mixed/Engine/NET/VSNETFIL.CPP
    src/Platform/Windows/Mixed/Engine/NET/VSNETTCP.CPP
    src/Platform/Windows/Mixed/Engine/NET/VSNET.CPP
    src/Platform/Windows/Mixed/Engine/NET/VSNETSTR.CPP
    src/Platform/Windows/Mixed/Engine/NET/NSTRBSTH.CPP
    src/Platform/Windows/Mixed/Engine/MEDIA/VSSOUND.CPP
    src/Visos/Generic/VsMathFixed12Vector2D.cpp
    src/Visos/Generic/SignedTrigTable.cpp
    src/Visos/Generic/CVSMath.cpp
    src/Visos/Generic/CVSPoint.cpp
    src/Visos/Windows/INVISWIN.CPP
    src/Visos/Windows/Debug/TEXTWIN.CPP
    src/Visos/Windows/Debug/TXWININV.CPP
    src/Visos/Windows/Debug/TEXTLINE.CPP
    src/Visos/Generic/MogLoad.cpp
    src/Visos/Generic/MogPatch.cpp
    src/Visos/Generic/MogConst.cpp
    src/Visos/Generic/MogBst.cpp
    src/Platform/Windows/Mixed/Engine/CORE/LINKSCF.CPP
    src/Platform/Windows/Mixed/Engine/CORE/LINKSCFBYTES.CPP
    src/AI/Projectile.cpp
    src/AI/GroupState.cpp
    src/AI/CInvisibleSwitch.cpp
    src/AI/CInvisibleSwitchManager.cpp
    src/AI/CLaser.cpp
    src/AI/CLaserManager.cpp
    src/AI/CLift.cpp
    src/AI/CLiftManager.cpp
    src/AI/CDoorAct.cpp
    src/AI/CDoorSlot.cpp
    src/AI/CIceAct.cpp
    src/AI/CLiftAct.cpp
    src/AI/ChildState.cpp
    src/AI/LVITER.CPP
    src/AI/CGameObjectCollision.cpp
    src/AI/LVPKT.CPP
    src/AI/LVPKTTH.CPP
    src/AI/LVRSTTH.CPP
    src/AI/LVSAVEPOS.CPP
    src/Platform/Windows/Mixed/Level/STATE/LVPROFILE.CPP
    src/Control/CGameStatusLevel.cpp
    src/AI/CGameObjectId.cpp
    src/AI/LVBSTH.CPP
    src/AI/CDoorCtor.cpp
    src/AI/PlasChild.cpp
    src/AI/CEnemyGrp.cpp
    src/AI/CMine.cpp
    src/AI/GroupPlas.cpp
    src/AI/CHUNKOBJVT.CPP
    src/AI/CLiftRun.cpp
    src/AI/CHOBJDEL.CPP
    src/AI/CHOBJRST.CPP
    src/AI/COLLSCORE.CPP
    src/Platform/Windows/Mixed/Level/CHUNK/ANIMREC.CPP
    src/AI/PROJRST.CPP
    src/Platform/Windows/Mixed/Level/IO/DRAWTEXT.CPP
    src/Visos/Generic/TXTCNT.CPP
    src/Visos/Generic/TXTCONST.CPP
    src/Frontend/MMPASSTH.CPP
    src/Frontend/MMPASS.CPP
    src/AI/LVRANDVT.CPP
    src/AI/LVPREDVT.CPP
    src/AI/LVPRJGEOM.CPP
    src/AI/LVGMOBPT.CPP
    src/views/2d/LVRUNEXT.CPP
)

set(LEMBALL_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/LEMBALL.RES")
set_source_files_properties("${LEMBALL_RESOURCE}" PROPERTIES EXTERNAL_OBJECT TRUE)

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