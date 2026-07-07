#include "startup.h"

#include "main.h"
#include "visos/vsgdi.h"

#include <ctype.h>
#include <string.h>

static const char g_STARTUP_DefaultOverrideLevelPath[] = "level\\testlvl.lvl";
static const char g_STARTUP_SwitchNoMusic[] = "/NOMUSIC";
static const char g_STARTUP_SwitchNoEffects[] = "/NOEFFECTS";
static const char g_STARTUP_SwitchSoundDebug[] = "/SNDDEBUG";
static const char g_STARTUP_SwitchStatusDebug[] = "/STATDEBUG";
static const char g_STARTUP_SwitchMemoryDebug[] = "/MEMDEBUG";
static const char g_STARTUP_SwitchNoAnim[] = "/NOANIM";
static const char g_STARTUP_SwitchNoZoom[] = "/NOZOOM";
static const char g_STARTUP_SwitchCompact[] = "/320";
static const char g_STARTUP_SwitchTestAllLevels[] = "/TESTALLLEVELS";
static const char g_STARTUP_SwitchHelp0[] = "/?";
static const char g_STARTUP_SwitchHelp1[] = "?";
static const char g_STARTUP_SwitchGraphics[] = "/GRAPHICS";

int g_fStartupAnimationsEnabled = 0;
int g_fSkipStartupSequence = 0;
int g_fMusicOptionAvailable = 0;
int g_fMusicEnabled = 0;
int g_fEffectsOptionAvailable = 0;
int g_fEffectsEnabled = 0;
int g_fZoomOptionAvailable = 0;
int g_fZoomedViewportEnabled = 0;
int g_fCompactPrimaryContextLayout = 0;
int g_fStartupEditLevelOverride = 0;
int g_fStartupPlayLevelOverride = 0;
int g_fStartupTestAllLevels = 0;
int g_fStartupGraphicsDialogRequested = 0;
int g_fLevelDemoModeEnabled = 0;
unsigned char g_abOverrideLevelFilePathBuffer[260];

static int g_fSoundDebugRequested = 0;
static int g_fStatusDebugRequested = 0;
static int g_fMemoryDebugRequested = 0;
static int g_fUnknownStartupFlag0 = 0;
static int g_fUnknownStartupFlag1 = 0;
static int g_fUnknownStartupFlag2 = 0;

static STARTUP_GraphicsWindowConfig g_StartupGraphicsWindowConfig;
static u32 g_aStartupGraphicsWindowItems[7];

// FUNCTION: LEMBALL 0x00406300
static void NoopHelpSwitchCallback(void) {
}

static void CopyCString(char *pszTarget, unsigned int cchTarget, const char *pszSource) {
    unsigned int i;

    if (pszTarget == 0 || cchTarget == 0) {
        return;
    }

    if (pszSource == 0) {
        pszTarget[0] = '\0';
        return;
    }

    i = 0;
    while (pszSource[i] != '\0' && i + 1 < cchTarget) {
        pszTarget[i] = pszSource[i];
        ++i;
    }
    pszTarget[i] = '\0';
}

// FUNCTION: LEMBALL 0x00406160
STARTUP_GraphicsWindowConfig *BuildStartupGraphicsWindowConfig(const STARTUP_GraphicsWindowConfig *pSeedConfig) {
    int i;
    const u32 *pSeedWords;
    u32 *pConfigWords;

    pSeedWords = (const u32 *)pSeedConfig;
    pConfigWords = (u32 *)&g_StartupGraphicsWindowConfig;
    for (i = 0; i < 7; ++i) {
        pConfigWords[i] = pSeedWords[i];
    }

    g_StartupGraphicsWindowConfig.m_cbSize = 0x50;
    g_StartupGraphicsWindowConfig.m_dwStyle = 0x300000;
    g_StartupGraphicsWindowConfig.m_hIcon = LoadIconA(g_hApplicationInstance, (LPCSTR)0x75);
    g_StartupGraphicsWindowConfig.m_cItems = LEMBALL_ARRAY_COUNT(g_aStartupGraphicsWindowItems);
    g_StartupGraphicsWindowConfig.m_pItemDataEnd =
        g_aStartupGraphicsWindowItems + LEMBALL_ARRAY_COUNT(g_aStartupGraphicsWindowItems);
    g_aStartupGraphicsWindowItems[0] = 100;
    g_aStartupGraphicsWindowItems[1] = 0xc0;
    g_aStartupGraphicsWindowItems[2] = 0x140;
    g_aStartupGraphicsWindowItems[3] = 0x140;
    g_aStartupGraphicsWindowItems[4] = 0x400;
    g_aStartupGraphicsWindowItems[5] = 0x200;
    g_aStartupGraphicsWindowItems[6] = 0x80;
    return &g_StartupGraphicsWindowConfig;
}

// FUNCTION: LEMBALL 0x00406230
void InitializeStartupSwitchDefaults(void) {
    g_fStartupAnimationsEnabled = 1;
    g_fSkipStartupSequence = 0;
    g_fMusicOptionAvailable = 1;
    g_fMusicEnabled = 1;
    g_fEffectsOptionAvailable = 1;
    g_fEffectsEnabled = 1;
    g_fStatusDebugRequested = 0;
    g_fMemoryDebugRequested = 0;
    g_fStartupGraphicsDialogRequested = 0;
    g_fSoundDebugRequested = 0;
    g_fZoomOptionAvailable = 1;
    g_fZoomedViewportEnabled = 0;
    g_fUnknownStartupFlag2 = 0;
    g_fStartupEditLevelOverride = 0;
    g_fStartupPlayLevelOverride = 0;
    if (GetSelectedGraphicsDriverId() < VSGDI_DRIVER_DIB_320_200 ||
        GetSelectedGraphicsDriverId() > VSGDI_DRIVER_DIB_640_480) {
        g_fCompactPrimaryContextLayout = 0;
    } else {
        g_fCompactPrimaryContextLayout = 1;
    }
    g_fUnknownStartupFlag1 = 0;
    g_fUnknownStartupFlag0 = 0;
    g_fLevelDemoModeEnabled = 0;
    g_fStartupTestAllLevels = 0;
    CopyCString((char *)g_abOverrideLevelFilePathBuffer,
                sizeof(g_abOverrideLevelFilePathBuffer),
                g_STARTUP_DefaultOverrideLevelPath);
}

// FUNCTION: LEMBALL 0x00406790
int CompareSwitchNameCaseInsensitive(const char *pszLeft, const char *pszRight, int cchMax) {
    int cchLeft;
    int cchRight;
    char chLeft;
    char chRight;

    cchLeft = (int)strlen(pszLeft);
    cchRight = (int)strlen(pszRight);

    if (cchMax < cchLeft) {
        cchLeft = cchMax;
    }
    if (cchMax < cchRight) {
        cchRight = cchMax;
    }
    if (cchRight != cchLeft) {
        return -1;
    }

    if (cchLeft < cchMax) {
        cchMax = cchLeft;
    }
    if (cchRight < cchMax) {
        cchMax = cchRight;
    }

    while (1) {
        if (cchMax == 0) {
            return 0;
        }
        if (isalpha((int)*pszLeft)) {
            chLeft = (char)toupper((int)*pszLeft);
        } else {
            chLeft = *pszLeft;
        }

        if (isalpha((int)*pszRight)) {
            chRight = (char)toupper((int)*pszRight);
        } else {
            chRight = *pszRight;
        }

        if ((int)chLeft - (int)chRight != 0) {
            break;
        }

        ++pszLeft;
        ++pszRight;
        --cchMax;
    }

    return (int)chLeft - (int)chRight;
}

// FUNCTION: LEMBALL 0x00406460
int ApplyStartupCommandLineSwitches(int cArgs, const char *const *ppszArgs) {
    int fContinue;

    fContinue = 1;
    if (0 < cArgs) {
        do {
            const char *pszArg;

            pszArg = *ppszArgs;
            if (CompareSwitchNameCaseInsensitive(pszArg, g_STARTUP_SwitchNoMusic, 99) == 0) {
                g_fMusicOptionAvailable = 0;
            }
            if (CompareSwitchNameCaseInsensitive(pszArg, g_STARTUP_SwitchNoEffects, 99) == 0) {
                g_fEffectsOptionAvailable = 0;
            }
            if (CompareSwitchNameCaseInsensitive(pszArg, g_STARTUP_SwitchSoundDebug, 99) == 0) {
                g_fSoundDebugRequested = 1;
            }
            if (CompareSwitchNameCaseInsensitive(pszArg, g_STARTUP_SwitchStatusDebug, 99) == 0) {
                g_fStatusDebugRequested = 1;
            }
            if (CompareSwitchNameCaseInsensitive(pszArg, g_STARTUP_SwitchMemoryDebug, 99) == 0) {
                g_fMemoryDebugRequested = 1;
            }
            if (CompareSwitchNameCaseInsensitive(pszArg, g_STARTUP_SwitchNoAnim, 99) == 0) {
                g_fStartupAnimationsEnabled = 0;
            }
            if (CompareSwitchNameCaseInsensitive(pszArg, g_STARTUP_SwitchNoZoom, 99) == 0) {
                g_fZoomOptionAvailable = 0;
            }
            if (CompareSwitchNameCaseInsensitive(pszArg, g_STARTUP_SwitchCompact, 99) == 0) {
                g_fCompactPrimaryContextLayout = 1;
            }
            if (CompareSwitchNameCaseInsensitive(pszArg, g_STARTUP_SwitchTestAllLevels, 99) == 0) {
                g_fStartupTestAllLevels = 1;
            }
            if (CompareSwitchNameCaseInsensitive(pszArg, g_STARTUP_SwitchHelp0, 99) == 0 ||
                CompareSwitchNameCaseInsensitive(pszArg, g_STARTUP_SwitchHelp1, 99) == 0) {
                NoopHelpSwitchCallback();
                fContinue = 0;
            }
            if (CompareSwitchNameCaseInsensitive(pszArg, "/EDIT@", (int)strlen("/EDIT@")) == 0) {
                g_fStartupEditLevelOverride = strlen(pszArg + strlen("/EDIT@")) != 0;
                strcpy((char *)g_abOverrideLevelFilePathBuffer, pszArg + strlen("/EDIT@"));
            }
            if (CompareSwitchNameCaseInsensitive(pszArg, "/PLAY@", (int)strlen("/PLAY@")) == 0) {
                g_fStartupPlayLevelOverride = strlen(pszArg + strlen("/PLAY@")) != 0;
                strcpy((char *)g_abOverrideLevelFilePathBuffer, pszArg + strlen("/PLAY@"));
            }
            if (CompareSwitchNameCaseInsensitive(pszArg, g_STARTUP_SwitchGraphics, 99) == 0) {
                g_fStartupGraphicsDialogRequested = 1;
            }

            ++ppszArgs;
            --cArgs;
        } while (cArgs != 0);
    }

    if (!g_fMusicOptionAvailable) {
        g_fMusicEnabled = 0;
    }
    if (!g_fEffectsOptionAvailable) {
        g_fEffectsEnabled = 0;
    }
    if (!g_fZoomOptionAvailable) {
        g_fZoomedViewportEnabled = 0;
    }
    if (!g_fStartupAnimationsEnabled) {
        g_fSkipStartupSequence = 1;
    }

    return fContinue;
}

// FUNCTION: LEMBALL 0x004727B0
void FinalizeStartupGraphicsDriverConfig(void) {
    STARTUP_GraphicsWindowConfig Config;

    memset(&Config, 0, sizeof(Config));
    BuildStartupGraphicsWindowConfig(&Config);

    if (g_fStartupGraphicsDialogRequested) {
        InitializeSelectedGraphicsDriver(VSGDI_DRIVER_METRICS);
    } else {
        InitializeSelectedGraphicsDriver(VSGDI_DRIVER_AUTO);
    }
}
