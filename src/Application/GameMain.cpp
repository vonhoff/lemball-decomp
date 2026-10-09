#include "GameMain.h"

#include "CDemo.h"
#include "CGame.h"
#include "Engine/Math/VSTrig.h"
#include "Engine/Memory/SmallMemoryConstants.h"
#include "Engine/Startup/tagPRE_INIT.h"
#include "Engine/Streams/CVSOStream.h"
#include "Engine/Strings/VsString.h"
#include "Frontend/CBaseFrontendDrawer.h"
#include "Frontend/FrontendLayoutMode.h"
#include "Level/CLevelLoader.h"
#include "Platform/Windows/Entry.h"
#include "Platform/Windows/Graphics/CGraphicsDriver.h"
#include "Platform/Windows/Graphics/CGraphicsState.h"

#include <string.h>

#pragma intrinsic(memcpy, strcpy)

enum {
	GAME_GDI_SURFACE_SLOT_CAPACITY = 80,
	GAME_MEMORY_BUDGET_BYTES = 3 * 1024 * 1024,
	GAME_RANDOM_INITIAL_SEED = 0xad28,
	GAME_WINDOW_ICON_RESOURCE_ID = 117,
	GAME_DEMO_QUEUE_SOURCE_ID = 0x19000,
	COMMAND_LINE_SWITCH_MAX_COMPARE_LENGTH = 99,
	COMMAND_LINE_LEVEL_PATH_MIN_LENGTH = 1
};

extern "C" __declspec(dllimport) void* __stdcall LoadIconA(void* p_instance, const char* p_name);

// GLOBAL: LEMBALL 0x0049ca30
int g_nTestAllLevels = 0;

// GLOBAL: LEMBALL 0x004a6284
int g_nAnimationsDisabled = 0;

// GLOBAL: LEMBALL 0x004a6288
int g_nZoomEnabled = 0;

// GLOBAL: LEMBALL 0x004a628c
int g_nMusicAvailable = 0;

// GLOBAL: LEMBALL 0x004a6290
int g_nEffectsAvailable = 0;

// GLOBAL: LEMBALL 0x004a6294
int g_nAnimationsAvailable = 0;

// GLOBAL: LEMBALL 0x004a6298
int g_nZoomAvailable = 0;

// GLOBAL: LEMBALL 0x004a6300
int g_nDisplayMode = 0;

// GLOBAL: LEMBALL 0x004a1bcc
int* g_pRandomSeed = NULL;

// FUNCTION: LEMBALL 0x00406160
tagPRE_INIT* VSPreInit(tagPRE_INIT* p_preInit)
{
	memcpy(&g_preInit, p_preInit, sizeof(g_preInit));
	g_preInit.m_flags = GAME_GDI_SURFACE_SLOT_CAPACITY;
	g_preInit.m_memoryBudget = GAME_MEMORY_BUDGET_BYTES;
	g_preInit.m_icon = LoadIconA(g_pApplicationInstance, (char*) GAME_WINDOW_ICON_RESOURCE_ID);
	g_preInit.m_capabilities[g_preInit.m_capabilityCount - 1] = SMALL_MEMORY_256_BYTE_BUCKET_BLOCKS_REQUESTED;
	g_preInit.m_capabilities[g_preInit.m_capabilityCount - 2] = SMALL_MEMORY_128_BYTE_BUCKET_BLOCKS_REQUESTED;
	g_preInit.m_capabilities[g_preInit.m_capabilityCount - 3] = SMALL_MEMORY_64_BYTE_BUCKET_BLOCKS_REQUESTED;
	g_preInit.m_capabilities[g_preInit.m_capabilityCount - 4] = SMALL_MEMORY_32_BYTE_BUCKET_BLOCKS_REQUESTED;
	g_preInit.m_capabilities[g_preInit.m_capabilityCount - 5] = SMALL_MEMORY_16_BYTE_BUCKET_BLOCKS_REQUESTED;
	g_preInit.m_capabilities[g_preInit.m_capabilityCount - 6] = SMALL_MEMORY_8_BYTE_BUCKET_BLOCKS_REQUESTED;
	g_preInit.m_capabilities[g_preInit.m_capabilityCount - SMALL_MEMORY_BUCKET_COUNT] =
		SMALL_MEMORY_4_BYTE_BUCKET_BLOCKS_REQUESTED;
	return &g_preInit;
}

// FUNCTION: LEMBALL 0x00406230
void SetGameDefaults()
{
	CGraphicsState* graphicsSystem;
#if LEMBALL_ENFORCE_STARTUP_CHECKS
	g_nAnimationsAvailable = 1;
	g_nAnimationsDisabled = 0;
	g_nMusicAvailable = 1;
	g_nMusicVolume = 1;
#else
	g_nAnimationsAvailable = 0;
	g_nAnimationsDisabled = 1;
	g_nMusicAvailable = 0;
	g_nMusicVolume = 0;
#endif
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
	switch (graphicsSystem->m_driverMode) {
	case GFX_MODE_VGA_320X200:
	case GFX_MODE_VGA_320X240:
		g_nCompactPrimaryContextLayout = FRONTEND_LAYOUT_COMPACT;
		break;
	default:
		g_nCompactPrimaryContextLayout = FRONTEND_LAYOUT_STANDARD;
	}
	g_nLevelViewportHorizontalRemainder = 0;
	g_nLevelViewportVerticalRemainder = 0;
	g_nStoredLevelDemoModeEnabled = 0;
	g_nDemoMode = 0;
	strcpy(g_szCommandLineLevelFile, g_szDefaultOverrideLevelPath);
}

// FUNCTION: LEMBALL 0x00406300
void DisplayHelp()
{
}

// FUNCTION: LEMBALL 0x00406310
int VSmain(int p_argc, char** p_argv)
{
	int* seed;
	CGame* game;

	g_pVSTrig = new VSTrig();

	seed = new int;
	if (seed != NULL) {
		*seed = GAME_RANDOM_INITIAL_SEED;
		g_pRandomSeed = seed;
	}
	else {
		g_pRandomSeed = NULL;
	}

	_DEMO_Init(GAME_DEMO_QUEUE_SOURCE_ID);
	SetGameDefaults();
	if (DoCommandLine(p_argc, p_argv) == 1) {
		game = NULL;
		game = new CGame(NULL);
		if (g_nEditLevelMode != 0) {
			strcpy(game->m_runtimeName, g_szCommandLineLevelFile);
		}
		if (g_nPlayLevelMode != 0) {
			strcpy(game->m_runtimeName, g_szCommandLineLevelFile);
		}
		game->Run();
		if (game != NULL) {
			delete game;
		}
	}

	_DEMO_Quit();
	delete g_pRandomSeed;
	operator delete(g_pVSTrig);
	*g_pDebugOutput << g_szGameClosedDown;
	return 0;
}

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
			if (StrCmpI(*argv, g_szSwitchNoMusic, COMMAND_LINE_SWITCH_MAX_COMPARE_LENGTH) == 0) {
				g_nMusicAvailable = 0;
			}
			if (StrCmpI(*argv, g_szSwitchNoEffects, COMMAND_LINE_SWITCH_MAX_COMPARE_LENGTH) == 0) {
				g_nEffectsAvailable = 0;
			}
			if (StrCmpI(*argv, g_szSwitchSoundDebug, COMMAND_LINE_SWITCH_MAX_COMPARE_LENGTH) == 0) {
				g_nSoundDebugRequested = 1;
			}
			if (StrCmpI(*argv, g_szSwitchStatusDebug, COMMAND_LINE_SWITCH_MAX_COMPARE_LENGTH) == 0) {
				g_nStatusDebugRequested = 1;
			}
			if (StrCmpI(*argv, g_szSwitchMemoryDebug, COMMAND_LINE_SWITCH_MAX_COMPARE_LENGTH) == 0) {
				g_nMemoryDebugRequested = 1;
			}
			if (StrCmpI(*argv, g_szSwitchNoAnim, COMMAND_LINE_SWITCH_MAX_COMPARE_LENGTH) == 0) {
				g_nAnimationsAvailable = 0;
			}
			if (StrCmpI(*argv, g_szSwitchNoZoom, COMMAND_LINE_SWITCH_MAX_COMPARE_LENGTH) == 0) {
				g_nZoomAvailable = 0;
			}
			if (StrCmpI(*argv, g_szSwitchCompact, COMMAND_LINE_SWITCH_MAX_COMPARE_LENGTH) == 0) {
				g_nCompactPrimaryContextLayout = FRONTEND_LAYOUT_COMPACT;
			}
			if (StrCmpI(*argv, g_szSwitchTestAllLevels, COMMAND_LINE_SWITCH_MAX_COMPARE_LENGTH) == 0) {
				g_nTestAllLevels = 1;
			}
			if (StrCmpI(*argv, g_szSwitchHelp0, COMMAND_LINE_SWITCH_MAX_COMPARE_LENGTH) == 0 ||
				StrCmpI(*argv, g_szSwitchHelp1, COMMAND_LINE_SWITCH_MAX_COMPARE_LENGTH) == 0) {
				DisplayHelp();
				keepGoing = 0;
			}
			prefixLength = strlen(g_szSwitchEditPrefix0);
			if (StrCmpI(*argv, g_szSwitchEditPrefix1, prefixLength) == 0) {
				unsigned int editLength;
				editLength = strlen(*argv + strlen(g_szSwitchEditPrefix2));
				g_nEditLevelMode = editLength >= COMMAND_LINE_LEVEL_PATH_MIN_LENGTH;
				strcpy(g_szCommandLineLevelFile, *argv + strlen(g_szSwitchEditPrefix3));
			}
			prefixLength = strlen(g_szSwitchPlayPrefix0);
			if (StrCmpI(*argv, g_szSwitchPlayPrefix1, prefixLength) == 0) {
				unsigned int playLength;
				playLength = strlen(*argv + strlen(g_szSwitchPlayPrefix2));
				g_nPlayLevelMode = playLength >= COMMAND_LINE_LEVEL_PATH_MIN_LENGTH;
				strcpy(g_szCommandLineLevelFile, *argv + strlen(g_szSwitchPlayPrefix3));
			}
			if (StrCmpI(*argv, g_szSwitchGraphics, COMMAND_LINE_SWITCH_MAX_COMPARE_LENGTH) == 0) {
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
int g_nCompactPrimaryContextLayout = FRONTEND_LAYOUT_STANDARD;

// GLOBAL: LEMBALL 0x004a6310
short g_nLevelViewportHorizontalRemainder = 0;

// GLOBAL: LEMBALL 0x004a6312
short g_nLevelViewportVerticalRemainder = 0;
