#ifndef LEMBALL_CONTROL_GAME_GAMEMAIN_H
#define LEMBALL_CONTROL_GAME_GAMEMAIN_H

#include "../../Common.h"

PreInit* VsPreInit(PreInit* p_preInit);
void SetGameDefaults();
void DisplayHelp();
int VsMain(int p_argc, char** p_argv);
int DoCommandLine(int p_argc, char** p_argv);

extern int g_nSoundDebugRequested;
extern int g_nStartupGraphicsDialogRequested;
extern int g_nStoredLevelDemoModeEnabled;
extern int g_nStatusDebugRequested;
extern int g_nMemoryDebugRequested;
extern int g_nCompactPrimaryContextLayout;
extern short g_nLevelViewportHorizontalRemainder;
extern short g_nLevelViewportVerticalRemainder;
extern char g_szDefaultOverrideLevelPath[20];
extern char g_szGameClosedDown[52];
extern char g_szSwitchNoMusic[12];
extern char g_szSwitchNoEffects[12];
extern char g_szSwitchSoundDebug[12];
extern char g_szSwitchStatusDebug[12];
extern char g_szSwitchMemoryDebug[12];
extern char g_szSwitchNoAnim[8];
extern char g_szSwitchNoZoom[8];
extern char g_szSwitchCompact[8];
extern char g_szSwitchTestAllLevels[16];
extern char g_szSwitchHelp0[4];
extern char g_szSwitchHelp1[4];
extern char g_szSwitchEditPrefix0[8];
extern char g_szSwitchEditPrefix1[8];
extern char g_szSwitchEditPrefix2[8];
extern char g_szSwitchEditPrefix3[8];
extern char g_szSwitchPlayPrefix0[8];
extern char g_szSwitchPlayPrefix1[8];
extern char g_szSwitchPlayPrefix2[8];
extern char g_szSwitchPlayPrefix3[8];
extern char g_szSwitchGraphics[16];
#endif
