#include "SHELL/OPTIONS.H"

#include "SHELL/ENTRY.H"
#include "GDI/VSGDI.H"

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
static const char g_STARTUP_SwitchEditPrefix[] = "/EDIT@";
static const char g_STARTUP_SwitchPlayPrefix[] = "/PLAY@";
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
static short g_fUnknownStartupFlag0 = 0;
static short g_fUnknownStartupFlag1 = 0;
static int g_fUnknownStartupFlag2 = 0;

static StartupGraphicsWindowConfig g_StartupGraphicsWindowConfig;
int g_fSmallMemoryBucketTableEnabled = 1;
int g_adwStartupGraphicsBucketSizeTable[7];
// GLOBAL: LEMBALL 0x004a27a8
StartupGraphicsWindowConfig g_StartupGraphicsDriverConfig = {
    0x40,
    8,
    0,
    7,
    g_adwStartupGraphicsBucketSizeTable,
    3,
    0,
};

// FUNCTION: LEMBALL 0x00406300
void NoopHelpSwitchCallback(void) {
}

// FUNCTION: LEMBALL 0x004034C7
StartupGraphicsWindowConfig *BuildStartupGraphicsWindowConfigThunk(const StartupGraphicsWindowConfig *pSeedConfig) {
    return BuildStartupGraphicsWindowConfig(pSeedConfig);
}

// FUNCTION: LEMBALL 0x00406160
StartupGraphicsWindowConfig *BuildStartupGraphicsWindowConfig(const StartupGraphicsWindowConfig *pSeedConfig) {
    memcpy(&g_StartupGraphicsWindowConfig, pSeedConfig,
        sizeof(g_StartupGraphicsWindowConfig));

    g_StartupGraphicsWindowConfig.m_cbSize = 0x50;
    g_StartupGraphicsWindowConfig.m_dwStyle = 0x300000;
    g_StartupGraphicsWindowConfig.m_hIcon = LoadIconA(g_hApplicationInstance, (LPCSTR)0x75);
    g_StartupGraphicsWindowConfig.m_pItemDataEnd[
        g_StartupGraphicsWindowConfig.m_cItems - 1] = 0x80;
    g_StartupGraphicsWindowConfig.m_pItemDataEnd[
        g_StartupGraphicsWindowConfig.m_cItems - 2] = 0x200;
    g_StartupGraphicsWindowConfig.m_pItemDataEnd[
        g_StartupGraphicsWindowConfig.m_cItems - 3] = 0x400;
    g_StartupGraphicsWindowConfig.m_pItemDataEnd[
        g_StartupGraphicsWindowConfig.m_cItems - 4] = 0x140;
    g_StartupGraphicsWindowConfig.m_pItemDataEnd[
        g_StartupGraphicsWindowConfig.m_cItems - 5] = 0x140;
    g_StartupGraphicsWindowConfig.m_pItemDataEnd[
        g_StartupGraphicsWindowConfig.m_cItems - 6] = 0xc0;
    g_StartupGraphicsWindowConfig.m_pItemDataEnd[
        g_StartupGraphicsWindowConfig.m_cItems - 7] = 100;
    return &g_StartupGraphicsWindowConfig;
}

// FUNCTION: LEMBALL 0x00406230
void InitializeStartupSwitchDefaults(void) {
    int nSelectedDriver;

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
    nSelectedDriver = *(int *)g_pSelectedGraphicsDriverRuntime;
    if (nSelectedDriver < VsGdiDRIVER_DIB_320_200 || nSelectedDriver > VsGdiDRIVER_DIB_640_480) {
        g_fCompactPrimaryContextLayout = 0;
    } else {
        g_fCompactPrimaryContextLayout = 1;
    }
    g_fUnknownStartupFlag1 = 0;
    g_fUnknownStartupFlag0 = 0;
    g_fStartupTestAllLevels = 0;
    g_fLevelDemoModeEnabled = 0;

    strcpy((char *)g_abOverrideLevelFilePathBuffer,
        g_STARTUP_DefaultOverrideLevelPath);
}

// FUNCTION: LEMBALL 0x00406790
int CompareSwitchNameCaseInsensitive(const char *pszLeft, const char *pszRight, int cchMax) {
    int cchLeft;
    int cchRight;
    int nDifference;
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
    if (cchLeft != cchRight) {
        return -1;
    }

    if (cchLeft < cchMax) {
        cchMax = cchLeft;
    }
    if (cchRight < cchMax) {
        cchMax = cchRight;
    }

    nDifference = 0;
    while (cchMax != 0) {
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

        nDifference = (int)chLeft - (int)chRight;
        if (nDifference != 0) {
            break;
        }

        ++pszLeft;
        ++pszRight;
        --cchMax;
    }

    return nDifference;
}

// FUNCTION: LEMBALL 0x00406460
int ApplyStartupCommandLineSwitches(int cArgs, const char *const *ppszArgs) {
    unsigned int cchPrefix;
    int fContinue = 1;
    if (0 < cArgs) {
        do {
            const char *pszPath;

            if (CompareSwitchNameCaseInsensitive(*ppszArgs, g_STARTUP_SwitchNoMusic, 99) == 0) {
                g_fMusicOptionAvailable = 0;
            }
            if (CompareSwitchNameCaseInsensitive(*ppszArgs, g_STARTUP_SwitchNoEffects, 99) == 0) {
                g_fEffectsOptionAvailable = 0;
            }
            if (CompareSwitchNameCaseInsensitive(*ppszArgs, g_STARTUP_SwitchSoundDebug, 99) == 0) {
                g_fSoundDebugRequested = 1;
            }
            if (CompareSwitchNameCaseInsensitive(*ppszArgs, g_STARTUP_SwitchStatusDebug, 99) == 0) {
                g_fStatusDebugRequested = 1;
            }
            if (CompareSwitchNameCaseInsensitive(*ppszArgs, g_STARTUP_SwitchMemoryDebug, 99) == 0) {
                g_fMemoryDebugRequested = 1;
            }
            if (CompareSwitchNameCaseInsensitive(*ppszArgs, g_STARTUP_SwitchNoAnim, 99) == 0) {
                g_fStartupAnimationsEnabled = 0;
            }
            if (CompareSwitchNameCaseInsensitive(*ppszArgs, g_STARTUP_SwitchNoZoom, 99) == 0) {
                g_fZoomOptionAvailable = 0;
            }
            if (CompareSwitchNameCaseInsensitive(*ppszArgs, g_STARTUP_SwitchCompact, 99) == 0) {
                g_fCompactPrimaryContextLayout = 1;
            }
            if (CompareSwitchNameCaseInsensitive(*ppszArgs, g_STARTUP_SwitchTestAllLevels, 99) == 0) {
                g_fStartupTestAllLevels = 1;
            }
            if (CompareSwitchNameCaseInsensitive(*ppszArgs, g_STARTUP_SwitchHelp0, 99) == 0 ||
                CompareSwitchNameCaseInsensitive(*ppszArgs, g_STARTUP_SwitchHelp1, 99) == 0) {
                NoopHelpSwitchCallback();
                fContinue = 0;
            }
            cchPrefix = (unsigned int)strlen(g_STARTUP_SwitchEditPrefix);
            if (CompareSwitchNameCaseInsensitive(*ppszArgs, g_STARTUP_SwitchEditPrefix, (int)cchPrefix) == 0) {
                pszPath = *ppszArgs + cchPrefix;
                g_fStartupEditLevelOverride = *pszPath != '\0';
                strcpy((char *)g_abOverrideLevelFilePathBuffer, pszPath);
            }
            cchPrefix = (unsigned int)strlen(g_STARTUP_SwitchPlayPrefix);
            if (CompareSwitchNameCaseInsensitive(*ppszArgs, g_STARTUP_SwitchPlayPrefix, (int)cchPrefix) == 0) {
                pszPath = *ppszArgs + cchPrefix;
                g_fStartupPlayLevelOverride = *pszPath != '\0';
                strcpy((char *)g_abOverrideLevelFilePathBuffer, pszPath);
            }
            if (CompareSwitchNameCaseInsensitive(*ppszArgs, g_STARTUP_SwitchGraphics, 99) == 0) {
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
    int i;
    StartupGraphicsWindowConfig *pBuiltConfig;
    int *pBucketSize;
    const u32 *pBuiltDword;
    u32 *pConfigDword;

    for (i = 0; i < 7; ++i) {
        g_adwStartupGraphicsBucketSizeTable[i] = 0x100;
    }
    g_StartupGraphicsDriverConfig.m_dwStyle <<= 0x13;
    pBuiltConfig = BuildStartupGraphicsWindowConfig(&g_StartupGraphicsDriverConfig);
    if (pBuiltConfig != 0) {
        pBuiltDword = (const u32 *)pBuiltConfig;
        pConfigDword = (u32 *)&g_StartupGraphicsDriverConfig;
        i = 7;
        do {
            *pConfigDword = *pBuiltDword;
            ++pBuiltDword;
            ++pConfigDword;
            --i;
        } while (i != 0);
    }
    if (7 < (int)g_StartupGraphicsDriverConfig.m_cItems) {
        g_StartupGraphicsDriverConfig.m_cItems = 7;
    }
    if ((int)g_StartupGraphicsDriverConfig.m_cItems <= 0) {
        g_fSmallMemoryBucketTableEnabled = 0;
    }
    pBucketSize = g_adwStartupGraphicsBucketSizeTable;
    do {
        if (*pBucketSize % 0x20 != 0) {
            *pBucketSize = ((*pBucketSize + 0x1f) / 0x20) * 0x20;
        }
        ++pBucketSize;
    } while (pBucketSize < g_adwStartupGraphicsBucketSizeTable + LEMBALL_ARRAY_COUNT(g_adwStartupGraphicsBucketSizeTable));

    switch (g_StartupGraphicsDriverConfig.m_dwReserved1) {
    case 0:
        g_fStartupGraphicsDriverCds = 0;
        g_fStartupGraphicsDriverWing = 1;
        g_fStartupGraphicsDriverGdk = 0;
        return;
    case 1:
        g_fStartupGraphicsDriverWing = 0;
        g_fStartupGraphicsDriverCds = 1;
        g_fStartupGraphicsDriverGdk = 0;
        return;
    case 2:
        g_fStartupGraphicsDriverWing = 0;
        g_fStartupGraphicsDriverGdk = 1;
        g_fStartupGraphicsDriverCds = 0;
        return;
    }
}
