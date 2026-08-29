#include "GameMain.h"

#include "../Level/LevelLoader.h"
#include "../Support/PreInit.h"
#include "../../Frontend/Base/BaseFrontendDrawer.h"
#include "../../Frontend/Base/BaseFrontendProcess.h"
#include "../../Platform/Windows/Entry.h"
#include "../../Visos/Foundation/VsOStream.h"
#include "../../Visos/Foundation/VsString.h"
#include "../../Visos/Foundation/VsTrig.h"
#include "../../Visos/Target/TargetGraphicsSystemState.h"
#include "../../Visos/Target/TargetGraphicsDriver.h"
#include "Demo.h"
#include "Game.h"
#include "GameTime.h"

#include <new.h>
#include <string.h>

#pragma intrinsic(memcpy, strcpy, strlen)

extern "C" __declspec(dllimport) void* __stdcall LoadIconA(void* p_instance, const char* p_name);

// 68K 0x1070000c VSPreInit__FP11tagPRE_INIT
// FUNCTION: LEMBALL 0x00406160
PreInit* VsPreInit(PreInit* p_preInit)
{
	memcpy(&g_preInit, p_preInit, sizeof(g_preInit));
	g_preInit.m_flags = 0x50;
	g_preInit.m_memoryBudget = 0x300000;
	g_preInit.m_icon = LoadIconA(g_pApplicationInstance, (char*) 0x75);
	g_preInit.m_capabilities[g_preInit.m_capabilityCount - 1] = 0x80;
	g_preInit.m_capabilities[g_preInit.m_capabilityCount - 2] = 0x200;
	g_preInit.m_capabilities[g_preInit.m_capabilityCount - 3] = 0x400;
	g_preInit.m_capabilities[g_preInit.m_capabilityCount - 4] = 0x140;
	g_preInit.m_capabilities[g_preInit.m_capabilityCount - 5] = 0x140;
	g_preInit.m_capabilities[g_preInit.m_capabilityCount - 6] = 0xc0;
	g_preInit.m_capabilities[g_preInit.m_capabilityCount - 7] = 100;
	return &g_preInit;
}

// 68K 0x107000f6 SetGameDefaults__Fv
// FUNCTION: LEMBALL 0x00406230
void SetGameDefaults()
{
	TargetGraphicsSystemState* graphicsSystem;

	g_nAnimationsAvailable = 1;
	g_nAnimationsDisabled = 0;
	g_nMusicAvailable = 1;
	g_nMusicVolume = 1;
	g_nEffectsAvailable = 1;
	g_nEffectsVolume = 1;
	g_nStatusDebugRequested = 0;
	g_nMemoryDebugRequested = 0;
	g_nStartupGraphicsDialogRequested = 0;
	g_nSoundDebugRequested = 0;
	g_nZoomAvailable = 1;
	g_nZoomEnabled = 0;
	graphicsSystem = g_pTargetGraphicsSystem;
	g_nDisplayMode = 0;
	g_nEditLevelMode = 0;
	g_nPlayLevelMode = 0;
	if (graphicsSystem->m_driverMode < 2 || 3 < graphicsSystem->m_driverMode) {
		g_nCompactPrimaryContextLayout = 0;
	}
	else {
		g_nCompactPrimaryContextLayout = 1;
	}
	g_nLevelViewportHorizontalRemainder = 0;
	g_nLevelViewportVerticalRemainder = 0;
	g_nStoredLevelDemoModeEnabled = 0;
	g_nDemoMode = 0;
	strcpy(g_szCommandLineLevelFile, g_szDefaultOverrideLevelPath);
}

// 68K 0x107001d0 DisplayHelp__Fv
// FUNCTION: LEMBALL 0x00406300
void DisplayHelp()
{
}

// 68K 0x1070067e VSmain__FiPPc
// FUNCTION: LEMBALL 0x00406310
int Vsmain(int p_argc, char** p_argv)
{
	int* sentinel;
	Game* game;

	g_pVSTrig = new VsTrig();

	sentinel = (int*) operator new(4);
	if (sentinel != 0) {
		*sentinel = 0xad28;
		g_pSentinel = sentinel;
	}
	else {
		g_pSentinel = 0;
	}

	DemoInit(0x19000);
	SetGameDefaults();
	if (DoCommandLine(p_argc, p_argv) == 1) {
		game = 0;
		game = new Game(0);
		if (g_nEditLevelMode != 0) {
			strcpy(game->m_runtimeName, g_szCommandLineLevelFile);
		}
		if (g_nPlayLevelMode != 0) {
			strcpy(game->m_runtimeName, g_szCommandLineLevelFile);
		}
		game->Run();
		if (game != 0) {
			delete game;
		}
	}

	DemoQuit();
	operator delete(g_pSentinel);
	operator delete(g_pVSTrig);
	*g_pDebugOutput << g_szGameClosedDown;
	return 0;
}

// 68K 0x107002da DoCommandLine__FiPPc
// FUNCTION: LEMBALL 0x00406460
int DoCommandLine(int p_argc, char** p_argv)
{
	int keepGoing;
	int prefixLength;
	int argc;
	char** argv;

	keepGoing = 1;
	argc = p_argc;
	if (0 < argc) {
		argv = p_argv;
		do {
			if (StrCmpI(*argv, g_szSwitchNoMusic, 99) == 0) {
				g_nMusicAvailable = 0;
			}
			if (StrCmpI(*argv, g_szSwitchNoEffects, 99) == 0) {
				g_nEffectsAvailable = 0;
			}
			if (StrCmpI(*argv, g_szSwitchSoundDebug, 99) == 0) {
				g_nSoundDebugRequested = 1;
			}
			if (StrCmpI(*argv, g_szSwitchStatusDebug, 99) == 0) {
				g_nStatusDebugRequested = 1;
			}
			if (StrCmpI(*argv, g_szSwitchMemoryDebug, 99) == 0) {
				g_nMemoryDebugRequested = 1;
			}
			if (StrCmpI(*argv, g_szSwitchNoAnim, 99) == 0) {
				g_nAnimationsAvailable = 0;
			}
			if (StrCmpI(*argv, g_szSwitchNoZoom, 99) == 0) {
				g_nZoomAvailable = 0;
			}
			if (StrCmpI(*argv, g_szSwitchCompact, 99) == 0) {
				g_nCompactPrimaryContextLayout = 1;
			}
			if (StrCmpI(*argv, g_szSwitchTestAllLevels, 99) == 0) {
				g_nTestAllLevels = 1;
			}
			if (StrCmpI(*argv, g_szSwitchHelp0, 99) == 0 || StrCmpI(*argv, g_szSwitchHelp1, 99) == 0) {
				DisplayHelp();
				keepGoing = 0;
			}
			prefixLength = strlen(g_szSwitchEditPrefix0);
			if (StrCmpI(*argv, g_szSwitchEditPrefix1, prefixLength) == 0) {
				unsigned int editLength;
				editLength = strlen(*argv + strlen(g_szSwitchEditPrefix2));
				g_nEditLevelMode = editLength >= 1;
				strcpy(g_szCommandLineLevelFile, *argv + strlen(g_szSwitchEditPrefix3));
			}
			prefixLength = strlen(g_szSwitchPlayPrefix0);
			if (StrCmpI(*argv, g_szSwitchPlayPrefix1, prefixLength) == 0) {
				unsigned int playLength;
				playLength = strlen(*argv + strlen(g_szSwitchPlayPrefix2));
				g_nPlayLevelMode = playLength >= 1;
				strcpy(g_szCommandLineLevelFile, *argv + strlen(g_szSwitchPlayPrefix3));
			}
			if (StrCmpI(*argv, g_szSwitchGraphics, 99) == 0) {
				g_nStartupGraphicsDialogRequested = 1;
			}
			argv = argv + 1;
			argc = argc - 1;
		} while (argc != 0);
	}
	if (g_nMusicAvailable == 0) {
		g_nMusicVolume = 0;
	}
	if (g_nEffectsAvailable == 0) {
		g_nEffectsVolume = 0;
	}
	if (g_nZoomAvailable == 0) {
		g_nZoomEnabled = 0;
	}
	if (g_nAnimationsAvailable == 0) {
		g_nAnimationsDisabled = 1;
	}
	return keepGoing;
}

// GLOBAL: LEMBALL 0x0049ca34
char g_szDefaultOverrideLevelPath[20] = "level\\testlvl.lvl";

// GLOBAL: LEMBALL 0x0049ca48
char g_szGameClosedDown[52] = "\n*************\nGAME CLOSED DOWN\n****************\n";

// GLOBAL: LEMBALL 0x0049ca7c
char g_szSwitchNoMusic[12] = "/NOMUSIC";

// GLOBAL: LEMBALL 0x0049ca88
char g_szSwitchNoEffects[12] = "/NOEFFECTS";

// GLOBAL: LEMBALL 0x0049ca94
char g_szSwitchSoundDebug[12] = "/SNDDEBUG";

// GLOBAL: LEMBALL 0x0049caa0
char g_szSwitchStatusDebug[12] = "/STATDEBUG";

// GLOBAL: LEMBALL 0x0049caac
char g_szSwitchMemoryDebug[12] = "/MEMDEBUG";

// GLOBAL: LEMBALL 0x0049cab8
char g_szSwitchNoAnim[8] = "/NOANIM";

// GLOBAL: LEMBALL 0x0049cac0
char g_szSwitchNoZoom[8] = "/NOZOOM";

// GLOBAL: LEMBALL 0x0049cac8
char g_szSwitchCompact[8] = "/320";

// GLOBAL: LEMBALL 0x0049cad0
char g_szSwitchTestAllLevels[16] = "/TESTALLLEVELS";

// GLOBAL: LEMBALL 0x0049cae0
char g_szSwitchHelp0[4] = "/?";

// GLOBAL: LEMBALL 0x0049cae4
char g_szSwitchHelp1[4] = "?";

// GLOBAL: LEMBALL 0x0049cae8
char g_szSwitchEditPrefix0[8] = "/EDIT@";

// GLOBAL: LEMBALL 0x0049caf0
char g_szSwitchEditPrefix1[8] = "/EDIT@";

// GLOBAL: LEMBALL 0x0049caf8
char g_szSwitchEditPrefix2[8] = "/EDIT@";

// GLOBAL: LEMBALL 0x0049cb00
char g_szSwitchEditPrefix3[8] = "/EDIT@";

// GLOBAL: LEMBALL 0x0049cb08
char g_szSwitchPlayPrefix0[8] = "/PLAY@";

// GLOBAL: LEMBALL 0x0049cb10
char g_szSwitchPlayPrefix1[8] = "/PLAY@";

// GLOBAL: LEMBALL 0x0049cb18
char g_szSwitchPlayPrefix2[8] = "/PLAY@";

// GLOBAL: LEMBALL 0x0049cb20
char g_szSwitchPlayPrefix3[8] = "/PLAY@";

// GLOBAL: LEMBALL 0x0049cb28
char g_szSwitchGraphics[16] = "/GRAPHICS";

// GLOBAL: LEMBALL 0x004a6280
int g_nSoundDebugRequested = 0;

// GLOBAL: LEMBALL 0x004a629c
int g_nStartupGraphicsDialogRequested = 0;

// GLOBAL: LEMBALL 0x004a62a0
int g_nStoredLevelDemoModeEnabled = 0;

// GLOBAL: LEMBALL 0x004a62f8
int g_nStatusDebugRequested = 0;

// GLOBAL: LEMBALL 0x004a62fc
int g_nMemoryDebugRequested = 0;

// GLOBAL: LEMBALL 0x004a630c
int g_nCompactPrimaryContextLayout = 0;

// GLOBAL: LEMBALL 0x004a6310
short g_nLevelViewportHorizontalRemainder = 0;

// GLOBAL: LEMBALL 0x004a6312
short g_nLevelViewportVerticalRemainder = 0;
