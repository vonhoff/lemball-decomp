#ifndef LEMBALL_STARTUP_OPTIONS_H
#define LEMBALL_STARTUP_OPTIONS_H

#include "engine/common.h"

struct STARTUP_GraphicsWindowConfig {
    u32 m_cbSize;
    u32 m_dwStyle;
    u32 m_dwReserved0;
    u32 m_cItems;
    u32 *m_pItemDataEnd;
    u32 m_dwReserved1;
    void *m_hIcon;
};

extern int g_fStartupAnimationsEnabled;
extern int g_fSkipStartupSequence;
extern int g_fMusicOptionAvailable;
extern int g_fMusicEnabled;
extern int g_fEffectsOptionAvailable;
extern int g_fEffectsEnabled;
extern int g_fZoomOptionAvailable;
extern int g_fZoomedViewportEnabled;
extern int g_fCompactPrimaryContextLayout;
extern int g_fStartupEditLevelOverride;
extern int g_fStartupPlayLevelOverride;
extern int g_fStartupTestAllLevels;
extern int g_fStartupGraphicsDialogRequested;
extern int g_fLevelDemoModeEnabled;
extern unsigned char g_abOverrideLevelFilePathBuffer[260];
extern STARTUP_GraphicsWindowConfig g_StartupGraphicsDriverConfig;
extern int g_fSmallMemoryBucketTableEnabled;

STARTUP_GraphicsWindowConfig *BuildStartupGraphicsWindowConfig(const STARTUP_GraphicsWindowConfig *pSeedConfig);
void InitializeStartupSwitchDefaults(void);
int ApplyStartupCommandLineSwitches(int cArgs, const char *const *ppszArgs);
int CompareSwitchNameCaseInsensitive(const char *pszLeft, const char *pszRight, int cchMax);
void FinalizeStartupGraphicsDriverConfig(void);

#endif
