cmake_minimum_required(VERSION 3.15)
project(lemball_decomp CXX)

# Order is significant for the reconstructed executable.
set(LEMBALL_SOURCES
    src/Control/CGame.cpp
    src/Frontend/GAMEEVT.cpp
    src/views/2d/C2D.cpp
    src/Control/CGameHelp.cpp
    src/Control/CGameVT.cpp
    src/Frontend/MAINMENU.cpp
    src/Frontend/MMRENDCH.cpp
    src/Frontend/MMCONTEXT.cpp
    src/Platform/Windows/Primary.cpp
    src/AI/TYPE18.cpp
    src/AI/TYPE18RESET.cpp
    src/AI/LV35BOUND.cpp
    src/Control/CGameStatusPasswordBits.cpp
    src/Platform/Windows/Mixed/Level/ENTITY/LVMODESLOT.cpp
    src/AI/lvtriplet.cpp
    src/AI/LVOWNOBJ.cpp
    src/AI/LVDELMOVE.cpp
    src/AI/LVSTREAM.cpp
    src/Platform/Windows/Mixed/Level/RUN/LEVELRUN.cpp
    src/AI/CMaze.cpp
    src/AI/CMineManager.cpp
    src/AI/CMove3d.cpp
    src/AI/CMover.cpp
    src/AI/LVNODE.cpp
    src/Platform/Windows/Mixed/Level/ENTITY/LVTILE.cpp
    src/Platform/Windows/Mixed/Level/IO/LVSTRVT.cpp
    src/Platform/Windows/Mixed/Level/IO/LVSTRWRT.cpp
    src/Platform/Windows/Mixed/Level/IO/LVSTRREAD.cpp
    src/Platform/Windows/Mixed/Level/STATE/LVQUIT.cpp
    src/Platform/Windows/Mixed/Level/RENDER/LVRENDISP.cpp
    src/AI/LVSTATEVAR.cpp
    src/Platform/Windows/Mixed/Level/ENTITY/LVCHFLAGS.cpp
    src/AI/LVPOSCOPY.cpp
    src/Visos/Generic/LVSTRFMT.cpp
    src/Platform/Windows/Mixed/Level/VTABLE/LEVELVT.cpp
    src/AI/AICoord.cpp
    src/AI/LVVTDEF.cpp
    src/AI/LVPOSVT.cpp
    src/AI/LVACTDSP.cpp
    src/AI/CGameObject.cpp
    src/AI/CGameObjectMove.cpp
    src/AI/CGameObjectLifecycle.cpp
    src/AI/CGameObjectLift.cpp
    src/AI/CGlobalGameObject.cpp
    src/AI/CFormationManager.cpp
    src/AI/CGenericGroup.cpp
    src/AI/CPlayerLemmingGroup.cpp
    src/AI/CPlayerLemmingGroupManager.cpp
    src/AI/CPlayerLemming.cpp
    src/AI/LVMOVEVT.cpp
    src/Platform/Windows/Mixed/Level/IO/LVNETVT.cpp
    src/AI/LVPROJVT.cpp
    src/Frontend/SCREENVT.cpp
    src/Frontend/SCRVTDEF.cpp
    src/Platform/Windows/Mixed/Level/STATE/LEVELSTAT.cpp
    src/views/2d/LVSTATMODE.cpp
    src/views/2d/LVSTATUS.cpp
    src/views/2d/LVSTATRG.cpp
    src/Frontend/MENUSEL.cpp
    src/Frontend/MENUSELRECT.cpp
    src/Frontend/MSSPREVT.cpp
    src/Frontend/MSRESCHD.cpp
    src/Frontend/MENUSELSINK.cpp
    src/Frontend/MENUSELPAUSE.cpp
    src/Platform/Windows/Mixed/Resource/VARLOAD.cpp
    src/Platform/Windows/Mixed/Resource/VARMGR.cpp
    src/Frontend/LOBBY.cpp
    src/Frontend/CEditString.cpp
    src/Platform/Windows/Entry.cpp
    src/Platform/Windows/ShellUI.cpp
    src/Control/Options.cpp
    src/Platform/Windows/Mixed/Level/RUN/DEMO.cpp
    src/Visos/Windows/CGDI.cpp
    src/Visos/Windows/CGDISwitch.cpp
    src/Visos/Windows/CGDI240.cpp
    src/Visos/Generic/Memory.cpp
    src/Platform/Windows/Mixed/Engine/GDI/WINDOW/VSWING.cpp
    src/Visos/Windows/VSWINGRECT.cpp
    src/Visos/Windows/VGAIO.cpp
    src/Platform/Windows/Mixed/Engine/CORE/VSINIT.cpp
    src/Platform/Windows/Mixed/Engine/CORE/VSFORMAT.cpp
    src/Platform/Windows/Mixed/Engine/CORE/VSRETURN.cpp
    src/Visos/Windows/RAWOUT.cpp
    src/Platform/Windows/Mixed/Engine/GDI/WINDOW/VSWINDOW.cpp
    src/Platform/Windows/Mixed/Engine/GDI/WINDOW/VSWINDOWMODE.cpp
    src/Platform/Windows/Mixed/Engine/GDI/RENDER/GEOMETRYVT.cpp
    src/Platform/Windows/Mixed/Engine/GDI/RENDER/GDISTATUSVT.cpp
    src/Visos/Generic/CPVSurface.cpp
    src/Platform/Windows/Mixed/Engine/GDI/RENDER/GDIRENDERVT.cpp
    src/Visos/Generic/CSurface.cpp
    src/Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.cpp
    src/Platform/Windows/Mixed/Engine/MEDIA/EFFVT.cpp
    src/Platform/Windows/Mixed/Engine/GDI/WINDOW/WINDOWSLOT38.cpp
    src/Platform/Windows/Mixed/Engine/GDI/RENDER/GDIVT.cpp
    src/Platform/Windows/Mixed/Engine/GDI/WINDOW/VSSPROWN.cpp
    src/Visos/Windows/Debug/DEBUGVT.cpp
    src/Visos/Generic/VSSTAT.cpp
    src/Platform/Windows/Mixed/Engine/NET/VSNETFIL.cpp
    src/Platform/Windows/Mixed/Engine/NET/VSNETTCP.cpp
    src/Platform/Windows/Mixed/Engine/NET/VSNET.cpp
    src/Platform/Windows/Mixed/Engine/NET/VSNETSTR.cpp
    src/Platform/Windows/Mixed/Engine/NET/NSTRBSTH.cpp
    src/Platform/Windows/Mixed/Engine/MEDIA/VSSOUND.cpp
    src/Visos/Generic/VsMathFixed12Vector2D.cpp
    src/Visos/Generic/SignedTrigTable.cpp
    src/Visos/Generic/CVSMath.cpp
    src/Visos/Generic/CVSPoint.cpp
    src/Visos/Windows/INVISWIN.cpp
    src/Visos/Windows/Debug/TEXTWIN.cpp
    src/Visos/Windows/Debug/TXWININV.cpp
    src/Visos/Windows/Debug/TEXTLINE.cpp
    src/Visos/Generic/MogLoad.cpp
    src/Visos/Generic/MogPatch.cpp
    src/Visos/Generic/MogConst.cpp
    src/Visos/Generic/MogBst.cpp
    src/Platform/Windows/Mixed/Engine/CORE/LINKSCF.cpp
    src/Platform/Windows/Mixed/Engine/CORE/LINKSCFBYTES.cpp
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
    src/AI/CHand.cpp
    src/AI/CHandManager.cpp
    src/AI/CIce.cpp
    src/AI/CIceManager.cpp
    src/AI/CLiftAct.cpp
    src/AI/ChildState.cpp
    src/AI/LVITER.cpp
    src/AI/CGameObjectCollision.cpp
    src/AI/LVPKT.cpp
    src/AI/LVPKTTH.cpp
    src/AI/LVRSTTH.cpp
    src/AI/LVSAVEPOS.cpp
    src/Platform/Windows/Mixed/Level/STATE/LVPROFILE.cpp
    src/Control/CGameStatusLevel.cpp
    src/AI/CGameObjectId.cpp
    src/AI/LVBSTH.cpp
    src/AI/CDoorCtor.cpp
    src/AI/PlasChild.cpp
    src/AI/CEnemyGrp.cpp
    src/AI/CMine.cpp
    src/AI/GroupPlas.cpp
    src/AI/CHUNKOBJVT.cpp
    src/AI/CLiftRun.cpp
    src/AI/CHOBJDEL.cpp
    src/AI/CHOBJRST.cpp
    src/AI/COLLSCORE.cpp
    src/Platform/Windows/Mixed/Level/CHUNK/ANIMREC.cpp
    src/AI/PROJRST.cpp
    src/Platform/Windows/Mixed/Level/IO/DRAWTEXT.cpp
    src/Visos/Generic/TXTCNT.cpp
    src/Visos/Generic/TXTCONST.cpp
    src/Frontend/MMPASSTH.cpp
    src/Frontend/MMPASS.cpp
    src/AI/LVRANDVT.cpp
    src/AI/LVPREDVT.cpp
    src/AI/LVPRJGEOM.cpp
    src/AI/lvgmobpt.cpp
    src/views/2d/LVRUNEXT.cpp
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