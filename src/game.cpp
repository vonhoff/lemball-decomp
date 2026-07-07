#include "game.h"

#include "shellui.h"
#include "startup.h"
#include "vsmem.h"
#include "vsinit.h"

#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif
#include <stdio.h>

static const char g_GAME_RegistryRoot[] = "SOFTWARE\\Visual Sciences\\";
static const char g_GAME_RegistryKey[] = "SOFTWARE\\Visual Sciences\\Lemmings Paintball";
static const char g_GAME_RunningValueName[] = "Running";
static const char g_GAME_RunningState[] = "running";
static const char g_GAME_NotRunningState[] = "";
static const char g_GAME_WindowTitle[] = "Lemmings Paintball";
static const char g_GAME_MainArchiveName[] = "pbaimog.vsr";
static const char g_GAME_StartupMusicName[] = "lemball";
static const char g_GAME_ProcessingName[] = "Processing";
static const char g_GAME_RefreshingName[] = "Refreshing";
static const char g_GAME_SrcDiskValueName[] = "SrcDisk";
static const char g_GAME_VsmemDllName[] = "vsmem.dll";
static const char g_GAME_NotInstalledTitle[] = "Paintball Not Installed";
static const char g_GAME_InstallPrompt[] =
    "To play Lemmings Paintball, you must first install it.  Run the SETUP.EXE program on the CD";
static const char g_GAME_CdromErrorTitle[] = "Unable to find CD";
static const char g_GAME_CdromErrorText[] = "Please insert the Paintball CD in a local CD Drive";

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

static void AppendCString(char *pszTarget, unsigned int cchTarget, const char *pszSuffix) {
    unsigned int cchCurrent;

    if (pszTarget == 0 || cchTarget == 0) {
        return;
    }

    cchCurrent = 0;
    while (pszTarget[cchCurrent] != '\0' && cchCurrent < cchTarget) {
        ++cchCurrent;
    }

    if (cchCurrent < cchTarget) {
        CopyCString(pszTarget + cchCurrent, cchTarget - cchCurrent, pszSuffix);
    }
}

// FUNCTION: LEMBALL 0x0045ECB0
static int SetVisualSciencesRegistryRunningState(const char *pszProductName, int fRunning) {
    char szKeyPath[256];
    HKEY hRegistryKey;
    const char *pszState;
    unsigned int cbState;

    CopyCString(szKeyPath, sizeof(szKeyPath), g_GAME_RegistryRoot);
    AppendCString(szKeyPath, sizeof(szKeyPath), pszProductName);

    if (RegOpenKeyExA((HKEY)0x80000002, szKeyPath, 0, KEY_ALL_ACCESS, &hRegistryKey) != 0) {
        return 0;
    }

    pszState = g_GAME_RunningState;
    if (!fRunning) {
        pszState = g_GAME_NotRunningState;
    }

    cbState = 0;
    while (pszState[cbState] != '\0') {
        ++cbState;
    }
    ++cbState;

    if (RegSetValueExA(hRegistryKey, g_GAME_RunningValueName, 0, 1, (const BYTE *)pszState, cbState) != 0) {
        RegCloseKey(hRegistryKey);
        return 0;
    }

    RegCloseKey(hRegistryKey);
    return 1;
}

static char *GetSrcDiskRegistryValueBuffer(char *pszBuffer, unsigned int cchBuffer) {
    HKEY hRegistryKey;
    DWORD cbValue;
    DWORD dwType;

    if (pszBuffer == 0 || cchBuffer == 0) {
        return 0;
    }

    pszBuffer[0] = '\0';
    if (RegOpenKeyExA((HKEY)0x80000002, g_GAME_RegistryKey, 0, KEY_ALL_ACCESS, &hRegistryKey) != 0) {
        return pszBuffer;
    }

    cbValue = cchBuffer;
    dwType = 0xffffffffu;
    if (RegQueryValueExA(hRegistryKey, g_GAME_SrcDiskValueName, 0, &dwType, (BYTE *)pszBuffer, &cbValue) != 0) {
        pszBuffer[0] = '\0';
    }

    RegCloseKey(hRegistryKey);
    return pszBuffer;
}

// FUNCTION: LEMBALL 0x0045EDA0
char *FindCdromFilePathBySuffix(const char *pszSuffix) {
    static char szCandidatePath[256] = "A:\\";
    DWORD dwDrives;
    char chDrive;
    int i;

    if (pszSuffix == 0) {
        return 0;
    }

    CopyCString(szCandidatePath + 3, sizeof(szCandidatePath) - 3, pszSuffix);
    dwDrives = GetLogicalDrives();
    chDrive = 'A';
    i = 0;

    while (i <= 31) {
        if ((dwDrives & 1u) != 0) {
            szCandidatePath[0] = chDrive;
            if (GetDriveTypeA(szCandidatePath) == DRIVE_CDROM) {
                FILE *pFile;

                pFile = fopen(szCandidatePath, "rb");
                if (pFile != 0) {
                    fclose(pFile);
                    return szCandidatePath;
                }
            }
        }

        dwDrives >>= 1;
        ++chDrive;
        ++i;
    }

    return 0;
}

static void ShowStartupMessage(const char *pszTitle, const char *pszText) {
    lemball_platform_show_error(pszTitle, pszText);
}

GAME_StatusEntry::GAME_StatusEntry(const char *pszName) {
    m_pszName = pszName;
    m_nReserved0 = 0;
    m_nReserved1 = 0;
    m_fActive = 0;
}

GAME_MainContext::GAME_MainContext(void) {
    m_fRegistryRunning = 0;
    m_fMusicEnabled = 1;
    m_fEffectsEnabled = 1;
    m_fSessionReady = 0;
    m_fInstalled = 0;
    m_pszRegistryKey = g_GAME_RegistryKey;
    m_pszWindowTitle = g_GAME_WindowTitle;
    m_pszResourceArchiveName = g_GAME_MainArchiveName;
    m_pszStartupMusicName = g_GAME_StartupMusicName;
    m_szSrcDisk[0] = '\0';
    m_szDisplayCaption[0] = '\0';
    m_pProcessingStatus = 0;
    m_pRefreshingStatus = 0;
}

static GAME_StatusEntry *AllocateNamedStatusEntry(const char *pszName) {
    GAME_StatusEntry *pEntry;

    pEntry = new (AllocateVSMemBlock(sizeof(GAME_StatusEntry))) GAME_StatusEntry(pszName);
    if (pEntry == 0) {
        return 0;
    }
    return pEntry;
}

// FUNCTION: LEMBALL 0x00406DF0
GAME_MainContext *InitializeMainGameContext(GAME_MainContext *pMainContext, const char *pszCmdLine) {
    (void)pszCmdLine;

    if (pMainContext == 0) {
        return 0;
    }

    *pMainContext = GAME_MainContext();

    pMainContext->m_fRegistryRunning = SetVisualSciencesRegistryRunningState(g_GAME_WindowTitle, 1);
    if (FindCdromFilePathBySuffix(g_GAME_VsmemDllName) == 0) {
        ShowStartupMessage(g_GAME_CdromErrorTitle, g_GAME_CdromErrorText);
        return pMainContext;
    }
    GetSrcDiskRegistryValueBuffer(pMainContext->m_szSrcDisk, sizeof(pMainContext->m_szSrcDisk));
    pMainContext->m_fInstalled = pMainContext->m_szSrcDisk[0] != '\0';

    pMainContext->m_pProcessingStatus = AllocateNamedStatusEntry(g_GAME_ProcessingName);
    pMainContext->m_pRefreshingStatus = AllocateNamedStatusEntry(g_GAME_RefreshingName);

    if (pMainContext->m_pProcessingStatus != 0) {
        pMainContext->m_pProcessingStatus->m_fActive = 1;
    }
    if (pMainContext->m_pRefreshingStatus != 0) {
        pMainContext->m_pRefreshingStatus->m_fActive = 1;
    }

    CopyCString(pMainContext->m_szDisplayCaption, sizeof(pMainContext->m_szDisplayCaption), g_GAME_WindowTitle);
    if (pMainContext->m_fInstalled) {
        pMainContext->m_fSessionReady = 1;
    } else {
        CopyCString(pMainContext->m_szDisplayCaption, sizeof(pMainContext->m_szDisplayCaption), g_GAME_InstallPrompt);
    }

    return pMainContext;
}

// FUNCTION: LEMBALL 0x004071D0
void ShutdownMainGameContext(GAME_MainContext *pMainContext) {
    if (pMainContext == 0) {
        return;
    }

    if (pMainContext->m_pRefreshingStatus != 0) {
        FreeVSMemBlock(pMainContext->m_pRefreshingStatus);
        pMainContext->m_pRefreshingStatus = 0;
    }

    if (pMainContext->m_pProcessingStatus != 0) {
        FreeVSMemBlock(pMainContext->m_pProcessingStatus);
        pMainContext->m_pProcessingStatus = 0;
    }

    if (pMainContext->m_fRegistryRunning) {
        SetVisualSciencesRegistryRunningState(g_GAME_WindowTitle, 0);
        pMainContext->m_fRegistryRunning = 0;
    }
}

// FUNCTION: LEMBALL 0x00406310
int RunMainGameSession(int cArgs, const char *const *ppszArgs) {
    GAME_MainContext *pMainContext;

    InitializeStartupSwitchDefaults();
    if (!ApplyStartupCommandLineSwitches(cArgs, ppszArgs)) {
        return 0;
    }

    pMainContext = (GAME_MainContext *)AllocateVSMemBlock(sizeof(GAME_MainContext));
    if (pMainContext == 0) {
        return 1;
    }

    pMainContext = InitializeMainGameContext(pMainContext, 0);
    if (pMainContext == 0) {
        FreeVSMemBlock(pMainContext);
        return 1;
    }

    if (!pMainContext->m_fSessionReady) {
        ShowStartupMessage(g_GAME_NotInstalledTitle, g_GAME_InstallPrompt);
        AppendErrorCString(g_GAME_InstallPrompt);
        AppendErrorCString("\n");
        ShutdownMainGameContext(pMainContext);
        FreeVSMemBlock(pMainContext);
        return 1;
    }

    BuildSystemInformationReportString();
    ShutdownMainGameContext(pMainContext);
    FreeVSMemBlock(pMainContext);
    return 0;
}

// FUNCTION: LEMBALL 0x00459860
int RunGameStartupSequence(char *pszCmdLine) {
    int nResult;

    TokenizeAndFilterCommandLineArgs(pszCmdLine);
    FinalizeStartupGraphicsDriverConfig();

    if (!InitializeCoreSubsystems()) {
        ShutdownCoreSubsystems();
        return 1;
    }

    LogParsedCommandLineOptions();
    nResult = RunMainGameSession((int)GetParsedCommandLineArgumentCount(), GetParsedCommandLineArgs());
    ShutdownCoreSubsystems();
    return nResult;
}
