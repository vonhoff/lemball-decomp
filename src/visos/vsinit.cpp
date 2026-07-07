#include "vsgdi.h"
#include "vsmem.h"
#include "vsinit.h"
#include "vswin.h"

#include <stdio.h>
#include <string.h>

static void AppendStringToDebugOutFile(const char *pszText);

class VSINIT_TextStream {
public:
    VSINIT_TextStream(void) : m_cchBuffer(0) {
        m_szBuffer[0] = '\0';
    }

    void Append(const char *pszText) {
        size_t cchText;
        size_t cchCopy;

        if (pszText == 0) {
            return;
        }

        cchText = strlen(pszText);
        cchCopy = cchText;
        if (m_cchBuffer + cchCopy >= sizeof(m_szBuffer)) {
            cchCopy = sizeof(m_szBuffer) - m_cchBuffer - 1;
        }

        if (cchCopy != 0) {
            memcpy(m_szBuffer + m_cchBuffer, pszText, cchCopy);
            m_cchBuffer += cchCopy;
            m_szBuffer[m_cchBuffer] = '\0';
        }

        AppendStringToDebugOutFile(pszText);
    }

    void AppendInt(unsigned int uValue) {
        char szValue[16];
        char szReversed[16];
        unsigned int cchValue;

        cchValue = 0;
        if (uValue == 0) {
            szValue[0] = '0';
            szValue[1] = '\0';
            Append(szValue);
            return;
        }

        while (uValue != 0 && cchValue < sizeof(szReversed)) {
            szReversed[cchValue] = (char)('0' + (uValue % 10));
            uValue /= 10;
            ++cchValue;
        }

        for (unsigned int i = 0; i < cchValue; ++i) {
            szValue[i] = szReversed[cchValue - i - 1];
        }
        szValue[cchValue] = '\0';
        Append(szValue);
    }

private:
    char m_szBuffer[8192];
    size_t m_cchBuffer;
};

static VSINIT_TextStream *AppendCStringToStream(VSINIT_TextStream *pStream, const char *pszText) {
    if (pStream != 0) {
        pStream->Append(pszText);
    }
    return pStream;
}

static VSINIT_TextStream *AppendIntToStream(VSINIT_TextStream *pStream, unsigned int uValue) {
    if (pStream != 0) {
        pStream->AppendInt(uValue);
    }
    return pStream;
}

struct VSINIT_SubsystemPhase {
    const char *m_pszLogPrefix;
    int (*m_pfnInitialise)(void);
    int m_fAppendByteCount;
};

static const char g_VSINIT_SyncDebugName[] = "Sync_Debug";
static const char g_VSINIT_DebugOutPath[] = "debug.out";
static const char g_VSINIT_DebugStartError[] = "Unable to start 'Debug Message loop' thread";
static const char g_VSINIT_BaseWindowClass[] = "VS_Base_Window_Class";
static const char g_VSINIT_VersionPrefix[] = "ViSOS v";
static const char g_VSINIT_VersionMinorSeparator[] = ".";
static const char g_VSINIT_VersionBuildPrefix[] = " (Build: ";
static const char g_VSINIT_VersionSuffix[] = ")\n";
static const char g_VSINIT_CopyrightPrefix[] = "(c)  ";
static const char g_VSINIT_CopyrightYears[] = "1994,1995";
static const char g_VSINIT_CopyrightOwner[] = " Visual Sciences Ltd\n";
static const char g_VSINIT_Success[] = "Success";
static const char g_VSINIT_Failed[] = "Failed";
static const char g_VSINIT_MemInitPrefix[] = "_MEM_Init   : ";
static const char g_VSINIT_StreamInitPrefix[] = "_STRM_Init  : ";
static const char g_VSINIT_DebugInitPrefix[] = "_DBG_Init   : ";
static const char g_VSINIT_InputInitPrefix[] = "_INP_Init   : ";
static const char g_VSINIT_TimeInitPrefix[] = "_TIME_Init  : ";
static const char g_VSINIT_GdiInitPrefix[] = "_GDI_Init   : ";
static const char g_VSINIT_StatusInitPrefix[] = "_STAT_Init  : ";
static const char g_VSINIT_ResInitPrefix[] = "_RES_Init   : ";
static const char g_VSINIT_BytesSuffix[] = " bytes\n";
static const char g_VSINIT_LineBreak[] = "\n";
static const char g_VSINIT_CommandLineOptionsHeader[] = "Command line options:\n";
static const char g_VSINIT_CommandLineOptionPrefix[] = "  Option ";
static const char g_VSINIT_CommandLineOptionIs[] = " is ";
static const char g_VSINIT_NotSelected[] = "not ";
static const char g_VSINIT_Selected[] = "selected\n";
static const char g_VSINIT_MainMemoryArenaName[] = "Main memory arena";
static const char g_VSINIT_MemoryLeakMessage[] = "**** MEMORY LEAK, dumping memory contents ****\n";

static VSINIT_CommandLineOption g_aCommandLineOptions[] = {
    { "paranoid", 0, 0 },
    { "nowait", 0, 0 },
    { "nosmallmemory", 0, 0 },
    { "wing", 0, 0 },
    { "full", 0, 0 },
    { "showunloading", 0, 0 },
    { "showloading", 0, 0 },
    { "surfaces:", 0, 0 },
    { "memorysize:", 0, 0 },
    { "nodebug", 0, 0 },
    { "debugfile", 0, 0 },
    { "SNDDEBUG", 0, 0 },
    { "STATDEBUG", 0, 0 },
    { "MEMDEBUG", 0, 0 },
};

static VSINIT_TextStream g_StartupOutputStream;
static VSINIT_TextStream g_StatusOutputStream;
static VSINIT_TextStream g_ErrorOutputStream;
static VSINIT_TextStream *g_pStartupOutputStream = &g_StartupOutputStream;
static VSINIT_TextStream *g_pStatusOutputStream = &g_StatusOutputStream;
static VSINIT_TextStream *g_pErrorOutputStream = &g_ErrorOutputStream;
static char g_aszParsedArgs[16][64];
static const char *g_apszParsedArgs[16];
static unsigned int g_cParsedArgs = 0;
static int g_fSubsystemsReady = 0;
static int g_fDebugMessageThreadRunning = 0;
static int g_fDebugFileEnabled = 1;
static VSWIN_InvisibleMessageWindow g_InvisibleMessageWindow;
static long g_cbMainArenaAvailableAfterInit = 0;
static unsigned int g_cbResourceGeometryHelperState = 0;
static unsigned int g_uViSOSMajorVersion = 1;
static unsigned int g_uViSOSMinorVersion = 0;
static unsigned int g_uViSOSBuildNumber = 201;

void AppendStartupCString(const char *pszText) {
    AppendCStringToStream(g_pStartupOutputStream, pszText);
}

void AppendStartupUInt(unsigned int uValue) {
    AppendIntToStream(g_pStartupOutputStream, uValue);
}

void AppendStatusCString(const char *pszText) {
    AppendCStringToStream(g_pStatusOutputStream, pszText);
}

void AppendErrorCString(const char *pszText) {
    AppendCStringToStream(g_pErrorOutputStream, pszText);
}

void AppendErrorUInt(unsigned int uValue) {
    AppendIntToStream(g_pErrorOutputStream, uValue);
}

// FUNCTION: LEMBALL 0x004728B0
static void AppendStringToDebugOutFile(const char *pszText) {
    FILE *pDebugFile;

    if (!g_fDebugFileEnabled || pszText == 0 || *pszText == '\0') {
        return;
    }

    pDebugFile = fopen(g_VSINIT_DebugOutPath, "a");
    if (pDebugFile == 0) {
        return;
    }

    fputs(pszText, pDebugFile);
    fflush(pDebugFile);
    fclose(pDebugFile);
}

static const char *SelectSuccessOrFailedString(int fSuccess) {
    if (fSuccess == 0) {
        return g_VSINIT_Failed;
    }

    return g_VSINIT_Success;
}

static int ParseDecimalInt(const char *pszText) {
    int nValue;

    nValue = 0;
    if (pszText == 0) {
        return 0;
    }

    while (*pszText >= '0' && *pszText <= '9') {
        nValue = nValue * 10 + (*pszText - '0');
        ++pszText;
    }

    return nValue;
}

// FUNCTION: LEMBALL 0x00472BE0
static int InitializeDebugMessageThread(void) {
    AppendStringToDebugOutFile(g_VSINIT_SyncDebugName);
    AppendStringToDebugOutFile(g_VSINIT_LineBreak);

    if (!ConstructInvisibleMessageWindow(&g_InvisibleMessageWindow, g_VSINIT_BaseWindowClass, &g_fDebugMessageThreadRunning)) {
        return 0;
    }

    if (!DebugMessageThreadMain()) {
        AppendErrorCString(g_VSINIT_DebugStartError);
        AppendErrorCString(g_VSINIT_LineBreak);
        return 0;
    }

    g_fDebugMessageThreadRunning = 1;
    return 1;
}

// FUNCTION: LEMBALL 0x00472C70
static void ShutdownDebugMessageThread(int fForceTerminate) {
    (void)fForceTerminate;
    g_fDebugMessageThreadRunning = 0;
}

static int InitialiseStreams(void) {
    return 1;
}

static int InitialiseDebug(void) {
    return InitializeDebugMessageThread();
}

static int InitialiseInput(void) {
    return 1;
}

static int InitialiseTime(void) {
    return 1;
}

static int InitialiseStatus(void) {
    return 1;
}

static int InitialiseResources(void) {
    return 1;
}

static int MatchOptionName(const char *pszToken, const char *pszOptionName) {
    size_t cchToken;
    size_t cchOption;
    const char *pszColon;

    if (pszToken == 0 || pszOptionName == 0) {
        return 0;
    }

    if (*pszToken == '-' || *pszToken == '/') {
        ++pszToken;
    }

    cchToken = 0;
    while (pszToken[cchToken] != '\0') {
        ++cchToken;
    }

    cchOption = 0;
    pszColon = strchr(pszOptionName, ':');
    while (pszOptionName[cchOption] != '\0' && pszOptionName + cchOption != pszColon) {
        ++cchOption;
    }

    if (cchToken < cchOption) {
        return 0;
    }

    return strncmp(pszToken, pszOptionName, cchOption) == 0;
}

static void CopyToken(char *pszTarget, size_t cchTarget, const char *pszTokenStart, size_t cchToken) {
    size_t cchCopy;

    if (cchTarget == 0) {
        return;
    }

    cchCopy = cchToken;
    if (cchCopy >= cchTarget) {
        cchCopy = cchTarget - 1;
    }

    if (cchCopy != 0) {
        memcpy(pszTarget, pszTokenStart, cchCopy);
    }

    pszTarget[cchCopy] = '\0';
}

void LogParsedCommandLineOptions(void) {
    size_t i;

    AppendStatusCString(g_VSINIT_CommandLineOptionsHeader);

    for (i = 0; i < LEMBALL_ARRAY_COUNT(g_aCommandLineOptions); ++i) {
        AppendStatusCString(g_VSINIT_CommandLineOptionPrefix);
        AppendStatusCString(g_aCommandLineOptions[i].m_pszName);
        AppendStatusCString(g_VSINIT_CommandLineOptionIs);
        if (!g_aCommandLineOptions[i].m_fSpecified) {
            AppendStatusCString(g_VSINIT_NotSelected);
        }
        AppendStatusCString(g_VSINIT_Selected);
    }
}

// FUNCTION: LEMBALL 0x00459250
int InitializeCoreSubsystems(void) {
    static const VSINIT_SubsystemPhase aPhases[] = {
        { g_VSINIT_MemInitPrefix, InitializeMasterMainRamArena, 1 },
        { g_VSINIT_StreamInitPrefix, InitialiseStreams, 0 },
        { g_VSINIT_DebugInitPrefix, InitialiseDebug, 0 },
        { g_VSINIT_InputInitPrefix, InitialiseInput, 0 },
        { g_VSINIT_TimeInitPrefix, InitialiseTime, 0 },
        { g_VSINIT_GdiInitPrefix, InitializeResourceGeometryHelperRuntime, 1 },
        { g_VSINIT_StatusInitPrefix, InitialiseStatus, 0 },
        { g_VSINIT_ResInitPrefix, InitialiseResources, 0 },
    };
    size_t i;

    AppendStartupCString(g_VSINIT_VersionPrefix);
    AppendStartupUInt(g_uViSOSMajorVersion);
    AppendStartupCString(g_VSINIT_VersionMinorSeparator);
    AppendStartupUInt(g_uViSOSMinorVersion);
    AppendStartupCString(g_VSINIT_VersionBuildPrefix);
    AppendStartupUInt(g_uViSOSBuildNumber);
    AppendStartupCString(g_VSINIT_VersionSuffix);
    AppendStartupCString(g_VSINIT_CopyrightPrefix);
    AppendStartupCString(g_VSINIT_CopyrightYears);
    AppendStartupCString(g_VSINIT_CopyrightOwner);

    for (i = 0; i < LEMBALL_ARRAY_COUNT(aPhases); ++i) {
        int fPhaseSuccess;

        fPhaseSuccess = aPhases[i].m_pfnInitialise();
        AppendStartupCString(aPhases[i].m_pszLogPrefix);
        AppendStartupCString(SelectSuccessOrFailedString(fPhaseSuccess));

        if (aPhases[i].m_fAppendByteCount) {
            AppendStartupCString(" ");
            if (aPhases[i].m_pfnInitialise == InitializeMasterMainRamArena) {
                AppendStartupUInt((unsigned int)CalculateMemoryArenaAvailableBytes());
            } else {
                AppendStartupUInt(g_cbResourceGeometryHelperState);
            }
            AppendStartupCString(g_VSINIT_BytesSuffix);
        } else {
            AppendStartupCString(g_VSINIT_LineBreak);
        }

        if (!fPhaseSuccess) {
            g_fSubsystemsReady = 0;
            return 0;
        }
    }

    g_cbMainArenaAvailableAfterInit = CalculateMemoryArenaAvailableBytes();
    AppendStatusCString(g_VSINIT_MainMemoryArenaName);
    AppendStatusCString(g_VSINIT_LineBreak);

    g_fSubsystemsReady = 1;
    return 1;
}

// FUNCTION: LEMBALL 0x00459520
void ShutdownCoreSubsystems(void) {
    long cbAvailableNow;

    if (!g_fSubsystemsReady) {
        return;
    }

    AppendStartupCString(g_VSINIT_LineBreak);
    AppendStatusCString(g_VSINIT_LineBreak);
    AppendErrorCString(g_VSINIT_LineBreak);

    cbAvailableNow = CalculateMemoryArenaAvailableBytes();
    if (cbAvailableNow != g_cbMainArenaAvailableAfterInit) {
        AppendErrorCString(g_VSINIT_MemoryLeakMessage);
    }

    ShutdownResourceGeometryHelperRuntime();
    g_fSubsystemsReady = 0;
    ShutdownDebugMessageThread(0);
    ShutdownMasterMainRamArena();
}

// FUNCTION: LEMBALL 0x004595D0
int ParseCommandLineOptionToken(const char *pszToken) {
    size_t i;

    if (pszToken == 0) {
        return 0;
    }

    for (i = 0; i < LEMBALL_ARRAY_COUNT(g_aCommandLineOptions); ++i) {
        if (MatchOptionName(pszToken, g_aCommandLineOptions[i].m_pszName)) {
            const char *pszColon;
            size_t cchOption;
            size_t cchToken;

            if (*pszToken == '-' || *pszToken == '/') {
                ++pszToken;
            }

            cchToken = strlen(pszToken);
            pszColon = strchr(g_aCommandLineOptions[i].m_pszName, ':');
            cchOption = pszColon == 0
                ? strlen(g_aCommandLineOptions[i].m_pszName)
                : (size_t)(pszColon - g_aCommandLineOptions[i].m_pszName);

            if (cchToken == cchOption) {
                g_aCommandLineOptions[i].m_nValue ^= 1;
            } else if (pszColon != 0 && pszToken[cchOption] == ':') {
                g_aCommandLineOptions[i].m_nValue = ParseDecimalInt(pszToken + cchOption + 1);
            }

            g_aCommandLineOptions[i].m_fSpecified = 1;
            if (strcmp(g_aCommandLineOptions[i].m_pszName, "nodebug") == 0) {
                g_fDebugFileEnabled = 0;
            } else if (strcmp(g_aCommandLineOptions[i].m_pszName, "debugfile") == 0) {
                g_fDebugFileEnabled = 1;
            }
            return 1;
        }
    }

    return 0;
}

// FUNCTION: LEMBALL 0x004596B0
void TokenizeAndFilterCommandLineArgs(const char *pszCmdLine) {
    const char *pszCursor;
    const char *pszTokenStart;
    size_t cchToken;

    g_cParsedArgs = 0;
    for (cchToken = 0; cchToken < LEMBALL_ARRAY_COUNT(g_aCommandLineOptions); ++cchToken) {
        g_aCommandLineOptions[cchToken].m_nValue = 0;
        g_aCommandLineOptions[cchToken].m_fSpecified = 0;
    }

    if (pszCmdLine == 0) {
        return;
    }

    pszCursor = pszCmdLine;
    while (*pszCursor != '\0') {
        while (*pszCursor == ' ' || *pszCursor == '\t') {
            ++pszCursor;
        }

        if (*pszCursor == '\0') {
            break;
        }

        pszTokenStart = pszCursor;
        cchToken = 0;

        if (*pszCursor == '"') {
            pszTokenStart = pszCursor + 1;
            ++pszCursor;
            while (pszCursor[cchToken] != '\0' && pszCursor[cchToken] != '"') {
                ++cchToken;
            }
            pszCursor += cchToken;
            if (*pszCursor == '"') {
                ++pszCursor;
            }
        } else {
            while (pszCursor[cchToken] != '\0' && pszCursor[cchToken] != ' ' && pszCursor[cchToken] != '\t') {
                ++cchToken;
            }
            pszCursor += cchToken;
        }

        if (g_cParsedArgs >= LEMBALL_ARRAY_COUNT(g_apszParsedArgs)) {
            continue;
        }

        CopyToken(g_aszParsedArgs[g_cParsedArgs], sizeof(g_aszParsedArgs[g_cParsedArgs]), pszTokenStart, cchToken);

        if (!ParseCommandLineOptionToken(g_aszParsedArgs[g_cParsedArgs])) {
            g_apszParsedArgs[g_cParsedArgs] = g_aszParsedArgs[g_cParsedArgs];
            ++g_cParsedArgs;
        }
    }
}

unsigned int GetParsedCommandLineArgumentCount(void) {
    return g_cParsedArgs;
}

const VSINIT_CommandLineOption *GetCommandLineOptions(size_t *pcOptions) {
    if (pcOptions != 0) {
        *pcOptions = LEMBALL_ARRAY_COUNT(g_aCommandLineOptions);
    }
    return g_aCommandLineOptions;
}
