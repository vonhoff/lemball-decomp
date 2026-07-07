#include "GAME.H"

#include "SHELLUI.H"
#include "VSMEM.H"
#include "VSINIT.H"

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

static int HostRegOpenKeyExA(HKEY hRootKey, const char *pszSubKey, HKEY *phKey) {
    (void)hRootKey;
    (void)pszSubKey;
    if (phKey != 0) {
        *phKey = 0;
    }
    return 1;
}

static int HostRegSetValueExA(HKEY hKey,
                              const char *pszValueName,
                              DWORD dwReserved,
                              DWORD dwType,
                              const BYTE *pbData,
                              DWORD cbData) {
    (void)hKey;
    (void)pszValueName;
    (void)dwReserved;
    (void)dwType;
    (void)pbData;
    (void)cbData;
    return 1;
}

static int HostRegQueryValueExA(HKEY hKey,
                                const char *pszValueName,
                                DWORD *pdwReserved,
                                DWORD *pdwType,
                                BYTE *pbData,
                                DWORD *pcbData) {
    (void)hKey;
    (void)pszValueName;
    (void)pdwReserved;
    (void)pdwType;

    if (pbData != 0 && pcbData != 0 && *pcbData != 0) {
        pbData[0] = '\0';
    }

    return 1;
}

static void HostRegCloseKey(HKEY hKey) {
    (void)hKey;
}

static int SetVisualSciencesRegistryRunningState(const char *pszProductName, int fRunning) {
    char szKeyPath[256];
    HKEY hRegistryKey;
    const char *pszState;
    unsigned int cbState;

    CopyCString(szKeyPath, sizeof(szKeyPath), g_GAME_RegistryRoot);
    AppendCString(szKeyPath, sizeof(szKeyPath), pszProductName);

    if (HostRegOpenKeyExA((HKEY)0x80000002, szKeyPath, &hRegistryKey) != 0) {
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

    if (HostRegSetValueExA(hRegistryKey, g_GAME_RunningValueName, 0, 1, (const BYTE *)pszState, cbState) != 0) {
        HostRegCloseKey(hRegistryKey);
        return 0;
    }

    HostRegCloseKey(hRegistryKey);
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
    if (HostRegOpenKeyExA((HKEY)0x80000002, g_GAME_RegistryKey, &hRegistryKey) != 0) {
        return pszBuffer;
    }

    cbValue = cchBuffer;
    dwType = 0xffffffffu;
    if (HostRegQueryValueExA(
            hRegistryKey,
            g_GAME_SrcDiskValueName,
            0,
            &dwType,
            (BYTE *)pszBuffer,
            &cbValue
        ) != 0) {
        pszBuffer[0] = '\0';
    }

    HostRegCloseKey(hRegistryKey);
    return pszBuffer;
}

static int FindCdromFilePathBySuffix(const char *pszSuffix) {
    (void)pszSuffix;
    return 0;
}

static void ShowStartupMessage(const char *pszTitle, const char *pszText) {
    lemball_platform_show_error(pszTitle, pszText);
}

static GAME_StatusEntry *AllocateNamedStatusEntry(const char *pszName) {
    GAME_StatusEntry *pEntry;

    pEntry = (GAME_StatusEntry *)AllocateVSMemBlock(sizeof(GAME_StatusEntry));
    if (pEntry == 0) {
        return 0;
    }

    pEntry->m_pszName = pszName;
    pEntry->m_nReserved0 = 0;
    pEntry->m_nReserved1 = 0;
    pEntry->m_fActive = 0;
    return pEntry;
}

GAME_MainContext *InitializeMainGameContext(GAME_MainContext *pMainContext, const char *pszCmdLine) {
    (void)pszCmdLine;

    if (pMainContext == 0) {
        return 0;
    }

    pMainContext->m_fRegistryRunning = 0;
    pMainContext->m_fMusicEnabled = 1;
    pMainContext->m_fEffectsEnabled = 1;
    pMainContext->m_fSessionReady = 0;
    pMainContext->m_fInstalled = 0;
    pMainContext->m_pszRegistryKey = g_GAME_RegistryKey;
    pMainContext->m_pszWindowTitle = g_GAME_WindowTitle;
    pMainContext->m_pszResourceArchiveName = g_GAME_MainArchiveName;
    pMainContext->m_pszStartupMusicName = g_GAME_StartupMusicName;
    pMainContext->m_pProcessingStatus = 0;
    pMainContext->m_pRefreshingStatus = 0;
    pMainContext->m_szSrcDisk[0] = '\0';
    pMainContext->m_szDisplayCaption[0] = '\0';

    pMainContext->m_fRegistryRunning = SetVisualSciencesRegistryRunningState(g_GAME_WindowTitle, 1);
    if (FindCdromFilePathBySuffix(g_GAME_VsmemDllName) == 2) {
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

int RunMainGameSession(GAME_MainContext *pMainContext) {
    SHELLUI_PrimaryContextShell Shell;

    if (pMainContext == 0) {
        return 1;
    }

    if (!pMainContext->m_fSessionReady) {
        ShowStartupMessage(g_GAME_NotInstalledTitle, g_GAME_InstallPrompt);
        AppendErrorCString(g_GAME_InstallPrompt);
        AppendErrorCString("\n");
        return 1;
    }

    InitializePrimaryContextShell(&Shell);
    BuildSystemInformationReportString();
    return 0;
}

int RunGameStartupSequence(char *pszCmdLine) {
    GAME_MainContext MainContext;
    int nResult;

    TokenizeAndFilterCommandLineArgs(pszCmdLine);

    if (!InitializeCoreSubsystems()) {
        ShutdownCoreSubsystems();
        return 1;
    }

    if (InitializeMainGameContext(&MainContext, pszCmdLine) == 0) {
        ShutdownCoreSubsystems();
        return 1;
    }

    LogParsedCommandLineOptions();
    nResult = RunMainGameSession(&MainContext);
    ShutdownMainGameContext(&MainContext);
    ShutdownCoreSubsystems();
    return nResult;
}
