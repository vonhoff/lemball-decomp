#include "../game.h"
#include "../startup.h"

#include "vsgdi.h"
#include "vsmem.h"
#include "vsinit.h"
#include "vswin.h"

#include <ctype.h>
#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif
#include <setjmp.h>
#include <stdio.h>
#include <string.h>

void AppendStringToDebugOutFile(const char *pszText);
extern void TriggerReleaseAssertFailure(const char *pszExpression, const char *pszFile, int nLine);
extern void ReleaseTypedResourceObjectReference(void *pResourceObject);
extern void InitializeRenderQueueNodeBase(void *pRenderQueueNode);
extern void RegisterOrderedRenderDispatchClient(void *pDispatchQueue, void *pClient, int nOrder);
extern void UnregisterOrderedRenderDispatchClient(void *pDispatchQueue, void *pClient, int nOrder);
extern void *g_pStatusEntryRegistry;
extern void *g_pSharedRenderDispatchQueue;
extern void *g_pSharedGeometryHelper;

struct VSINIT_FixedBufferStream {
    void *m_pVtable;
    char *m_pszBuffer;
    unsigned int m_cbBuffer;
    char *m_pszCursor;
    unsigned int m_cchWritten;
    int m_nTabWidth;
    void (*m_pfnFlush)(char *);
};

struct VSINIT_StreamFormatTargetState {
    void *m_pVtable;
    int m_nReserved04;
    unsigned int m_dwFlags;
    int m_nReserved0c;
    char m_chFill;
    char m_abReserved11[3];
    int m_nWidth;
    int m_nRadix;
    VSINIT_FixedBufferStream *m_pDownstream;
};

struct VSINIT_FormattedOutputStream {
    void *m_pVtable;
    char m_szFormatBuffer[0x124];
    char *m_pszFormattedText;
    VSINIT_StreamFormatTargetState m_TargetState;
};

#define VSINIT_FORMAT_TARGET(pStream) \
    ((VSINIT_StreamFormatTargetState *)((char *)(pStream) + (unsigned long)((void **)((pStream)->m_pVtable))[1]))

void FlushStatusFixedBufferStream(char *pszText);
void FlushStartupFixedBufferStream(char *pszText);
void FlushErrorFixedBufferStream(char *pszText);
static void DestroyFormattedOutputStream(VSINIT_FormattedOutputStream *pStream, int fFreeMemory);
static void DeleteFixedBufferStream(VSINIT_FixedBufferStream *pStream, int fFreeMemory);
void FormatSignedIntToRadixString(int nValue, char *pszBuffer, unsigned int nRadix);
char *FormatUnsignedIntToRadixString(unsigned int uValue, char *pszBuffer, unsigned int nRadix);
void ApplyStreamIntegerWidthPadding(VSINIT_FormattedOutputStream *pStream);

static void *g_StreamBaseVtable[1] = { 0 };
static void *g_FormattedOutputStreamVtable[2] = { 0, (void *)0x12c };
static void *g_StreamFormatTargetStateVtable[1] = { 0 };
static void *g_StreamFormatSubobjectVtable[1] = { 0 };
static void *g_FixedBufferStreamVtable[4] = {
    (void *)DeleteFixedBufferStreamReturnThis,
    (void *)ResetFixedBufferStream,
    (void *)AppendCharToFixedBufferStream,
    (void *)AppendCStringToFixedBufferStream,
};
static const char g_VSINIT_RadixDigits[] = "0123456789abcdef";
static const char g_VSINIT_NullPointerText[] = "(null)";
static const char g_VSINIT_HexPointerPrefix[] = "0x";
static int g_fRadixPowerTableInitialized = 0;
static unsigned int g_adwRadixPowerTable[17];

// FUNCTION: LEMBALL 0x004584C0
void ApplyStreamIntegerWidthPadding(VSINIT_FormattedOutputStream *pStream) {
    int cchText;
    unsigned int cchWidth;
    unsigned int fNegative;
    unsigned int cchSourceSkip;
    unsigned int cchTargetSkip;
    char *pszFill;
    char *pszSource;
    char *pszTarget;
    VSINIT_StreamFormatTargetState *pTargetState;
    unsigned int i;

    pTargetState = VSINIT_FORMAT_TARGET(pStream);
    cchWidth = (unsigned int)pTargetState->m_nWidth;
    if (cchWidth == 0) {
        pStream->m_pszFormattedText = pStream->m_szFormatBuffer;
        return;
    }

    fNegative = pStream->m_szFormatBuffer[0] == '-';
    cchText = (int)strlen(pStream->m_szFormatBuffer);

    pszFill = pStream->m_szFormatBuffer + 0x21;
    for (i = cchWidth >> 2; i != 0; --i) {
        *(u32 *)pszFill = (unsigned char)pTargetState->m_chFill |
                          ((u32)(unsigned char)pTargetState->m_chFill << 8) |
                          ((u32)(unsigned char)pTargetState->m_chFill << 16) |
                          ((u32)(unsigned char)pTargetState->m_chFill << 24);
        pszFill += 4;
    }
    for (i = cchWidth & 3; i != 0; --i) {
        *pszFill = pTargetState->m_chFill;
        ++pszFill;
    }
    pStream->m_szFormatBuffer[0x21 + cchWidth] = '\0';
    if (fNegative) {
        pStream->m_szFormatBuffer[0x21] = '-';
    }

    cchSourceSkip = fNegative;
    cchTargetSkip = fNegative;
    if ((pTargetState->m_dwFlags & 2) == 0) {
        if (cchText < (int)cchWidth) {
            cchTargetSkip = fNegative - (unsigned int)cchText + cchWidth;
        } else {
            cchSourceSkip = (unsigned int)cchText - cchWidth;
        }
    }

    pszSource = pStream->m_szFormatBuffer + cchSourceSkip;
    pszTarget = pStream->m_szFormatBuffer + 0x21 + cchTargetSkip;
    for (i = (cchWidth - fNegative) >> 2; i != 0; --i) {
        *(u32 *)pszTarget = *(u32 *)pszSource;
        pszSource += 4;
        pszTarget += 4;
    }
    for (i = (cchWidth - fNegative) & 3; i != 0; --i) {
        *pszTarget = *pszSource;
        ++pszSource;
        ++pszTarget;
    }
    pStream->m_pszFormattedText = pStream->m_szFormatBuffer + 0x21;
}

// FUNCTION: LEMBALL 0x00458D80
void FormatSignedIntToRadixString(int nValue, char *pszBuffer, unsigned int nRadix) {
    unsigned int uValue;

    uValue = (unsigned int)nValue;
    if (nValue < 0) {
        *pszBuffer = '-';
        ++pszBuffer;
        uValue = (unsigned int)-nValue;
    }
    FormatUnsignedIntToRadixString(uValue, pszBuffer, nRadix);
}

// FUNCTION: LEMBALL 0x00458DB0
char *FormatUnsignedIntToRadixString(unsigned int uValue, char *pszBuffer, unsigned int nRadix) {
    unsigned int nPower;
    unsigned int nMaxPower;
    unsigned int nLimit;
    unsigned int nDigit;
    int fEmitting;
    int cchOutput;
    int nLoopCount;

    if (g_fRadixPowerTableInitialized == 0) {
        nPower = 2;
        do {
            nLimit = 0xffffffffu / nPower;
            nMaxPower = nPower;
            if (nPower <= nLimit) {
                do {
                    nMaxPower *= nPower;
                } while (nMaxPower <= nLimit);
            }
            g_adwRadixPowerTable[nPower] = nMaxPower;
            ++nPower;
        } while ((int)nPower < 0x11);
        g_fRadixPowerTableInitialized = 1;
    }

    fEmitting = 0;
    cchOutput = 0;
    nLoopCount = 0;
    nPower = g_adwRadixPowerTable[nRadix];
    do {
        nDigit = uValue / nPower;
        if (nDigit != 0 || fEmitting) {
            pszBuffer[cchOutput] = g_VSINIT_RadixDigits[nDigit];
            ++cchOutput;
            fEmitting = 1;
        }
        uValue -= nPower * nDigit;
        nPower /= nRadix;
    } while (nPower != 1 && (++nLoopCount < 0x21));

    pszBuffer[cchOutput] = g_VSINIT_RadixDigits[uValue];
    pszBuffer[cchOutput + 1] = '\0';
    return pszBuffer;
}

// FUNCTION: LEMBALL 0x004585B0
VSINIT_FormattedOutputStream *AppendCStringToStream(VSINIT_FormattedOutputStream *pStream, const char *pszText) {
    AppendCStringToFixedBufferStream(VSINIT_FORMAT_TARGET(pStream)->m_pDownstream, pszText);
    return pStream;
}

// FUNCTION: LEMBALL 0x00458630
void AppendIntToStream(VSINIT_FormattedOutputStream *pStream, unsigned int uValue) {
    FormatSignedIntToRadixString((int)uValue, pStream->m_szFormatBuffer,
                                 (unsigned int)VSINIT_FORMAT_TARGET(pStream)->m_nRadix);
    ApplyStreamIntegerWidthPadding(pStream);
    AppendCStringToStream(pStream, pStream->m_pszFormattedText);
}

// FUNCTION: LEMBALL 0x004585D0
VSINIT_FormattedOutputStream *AppendCharToStreamVariant(VSINIT_FormattedOutputStream *pStream, char ch) {
    AppendCharToFixedBufferStream(VSINIT_FORMAT_TARGET(pStream)->m_pDownstream, ch);
    return pStream;
}

// FUNCTION: LEMBALL 0x004585F0
void AppendSignedIntToStreamVariant(VSINIT_FormattedOutputStream *pStream, int nValue) {
    FormatSignedIntToRadixString(nValue, pStream->m_szFormatBuffer,
                                 (unsigned int)VSINIT_FORMAT_TARGET(pStream)->m_nRadix);
    ApplyStreamIntegerWidthPadding(pStream);
    AppendCStringToStream(pStream, pStream->m_pszFormattedText);
}

// FUNCTION: LEMBALL 0x00458670
void AppendPointerToStreamVariant(VSINIT_FormattedOutputStream *pStream, unsigned int uValue) {
    if (uValue == 0) {
        AppendCStringToStream(pStream, g_VSINIT_NullPointerText);
        return;
    }
    FormatUnsignedIntToRadixString(uValue, pStream->m_szFormatBuffer, 0x10);
    ApplyStreamIntegerWidthPadding(pStream);
    AppendCStringToStream(AppendCStringToStream(pStream, g_VSINIT_HexPointerPrefix), pStream->m_pszFormattedText);
}

// FUNCTION: LEMBALL 0x004586D0
void AppendCharToFormattedStream(VSINIT_FormattedOutputStream *pStream, char ch) {
    AppendCharToStreamVariant(pStream, ch);
}

// FUNCTION: LEMBALL 0x004586E0
void AppendUIntToStream(VSINIT_FormattedOutputStream *pStream, unsigned int uValue) {
    FormatUnsignedIntToRadixString(uValue, pStream->m_szFormatBuffer,
                                   (unsigned int)VSINIT_FORMAT_TARGET(pStream)->m_nRadix);
    ApplyStreamIntegerWidthPadding(pStream);
    AppendCStringToStream(pStream, pStream->m_pszFormattedText);
}

// FUNCTION: LEMBALL 0x00458720
void AppendUnsignedIntToStreamVariant(VSINIT_FormattedOutputStream *pStream, unsigned int uValue) {
    FormatUnsignedIntToRadixString(uValue, pStream->m_szFormatBuffer,
                                   (unsigned int)VSINIT_FORMAT_TARGET(pStream)->m_nRadix);
    ApplyStreamIntegerWidthPadding(pStream);
    AppendCStringToStream(pStream, pStream->m_pszFormattedText);
}

// FUNCTION: LEMBALL 0x00458780
VSINIT_FormattedOutputStream *AppendUIntHexToStream(VSINIT_FormattedOutputStream *pStream, unsigned int uValue) {
    unsigned int dwFlags;
    int nRadix;
    VSINIT_StreamFormatTargetState *pTargetState;

    pTargetState = VSINIT_FORMAT_TARGET(pStream);
    dwFlags = pTargetState->m_dwFlags;
    pTargetState->m_dwFlags = (dwFlags & 0xffff7fcf) | 0x40;
    nRadix = pTargetState->m_nRadix;
    pTargetState->m_nRadix = 0x10;
    AppendUIntToStream(pStream, uValue);
    pTargetState->m_nRadix = nRadix;
    pTargetState->m_dwFlags = dwFlags;
    return pStream;
}

// FUNCTION: LEMBALL 0x004589C0
VSINIT_FormattedOutputStream *AppendHexUIntToStream(VSINIT_FormattedOutputStream *pStream, unsigned int uValue) {
    char chFill;
    int nWidth;
    unsigned int dwFlags;
    int nRadix;
    VSINIT_StreamFormatTargetState *pTargetState;

    pTargetState = VSINIT_FORMAT_TARGET(pStream);
    chFill = pTargetState->m_chFill;
    pTargetState->m_chFill = '0';
    nWidth = pTargetState->m_nWidth;
    pTargetState->m_nWidth = 8;
    dwFlags = pTargetState->m_dwFlags;
    pTargetState->m_dwFlags = (dwFlags & 0xffff7fcf) | 0x40;
    nRadix = pTargetState->m_nRadix;
    pTargetState->m_nRadix = 0x10;
    AppendUIntToStream(pStream, uValue);
    pTargetState->m_nWidth = nWidth;
    pTargetState->m_nRadix = nRadix;
    pTargetState->m_chFill = chFill;
    pTargetState->m_dwFlags = dwFlags;
    return pStream;
}

static const char g_VSINIT_SyncDebugName[] = "Sync_Debug";
static const char g_VSINIT_DebugOutPath[] = "debug.out";
static const char g_VSINIT_ErrorTitle[] = "ERROR";
static const char g_VSINIT_DebugStartError[] = "Unable to start 'Debug Message loop' thread";
static const char g_VSINIT_BaseWindowClass[] = "VS_Base_Window_Class";
static const char g_VSINIT_VersionPrefix[] = "ViSOS v";
static const char g_VSINIT_VersionMinorSeparator[] = ".";
static const char g_VSINIT_VersionBuildPrefix[] = " (Build: ";
static const char g_VSINIT_VersionSuffix[] = ")\n";
static const char g_VSINIT_CopyrightPrefix[] = "(c)  ";
static const char g_VSINIT_CopyrightYears[] = "1994,1995";
static const char g_VSINIT_CopyrightOwner[] = " Visual Sciences Ltd\n";
static const char g_VSINIT_SourceFileName[] = "VSINIT.CPP";
static const char g_VSINIT_EnoughMemoryAssert[] = "EnoughMemory";
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
static const char g_VSINIT_StatusOutputHeader[] = "Stats Output\n";
static const char g_VSINIT_StatusOutputRule[] = "---------------------------------------------------\n";
static const char g_VSINIT_StatusOutputColumns[] = "Average  Total    Max      Min      nSamples Desc\n";

static int g_afCommandLineOptionSpecified[14];
static int g_fViSOSParanoidMode = 0;
int g_fStartupNoWait = 0;
static int g_fShowResourceUnloading = 0;
static int g_fShowResourceLoading = 0;
static int g_nGarbageCollectorMode = 0;
static int g_fDebugMessageThreadRunning = 0;
static int g_fDebugFileEnabled = 0;

struct VSINIT_StatusEntryPointerArray {
    void *m_pVtable;
    int m_nReserved;
    void **m_ppEntries;
    int m_cEntriesMax;
    int m_cEntries;
};

struct VSINIT_ResourceTypeTable {
    int m_nCursor;
    int m_cTagsMax;
    u32 *m_pTags;
    int m_cTags;
};

struct VSINIT_VSMemPointerTable {
    void **m_ppItems;
    int m_cItems;
    int m_nCursor;
};

struct VSINIT_PaletteRemapVariant {
    void *m_pRemapTable;
    void *m_pResourceObject;
};

struct VSINIT_RenderDispatchQueue {
    char m_abReserved[0x58];
};

struct VSINIT_SharedGeometryHelper {
    void *m_pVtable;
    int m_nReserved04;
    int m_nReserved08;
    void *m_pRenderDispatchQueue;
    int m_nReserved10;
    int m_dwFlags;
};

static VSINIT_CommandLineOption g_aCommandLineOptions[] = {
    { "paranoid", &g_fViSOSParanoidMode },
    { "nowait", &g_fStartupNoWait },
    { "nosmallmemory", &g_fSmallMemoryBucketTableEnabled },
    { "wing", &g_fStartupGraphicsDriverWing },
    { "cds", &g_fStartupGraphicsDriverCds },
    { "gdk", &g_fStartupGraphicsDriverGdk },
    { "full", &g_fStartupFullscreen },
    { "showunloading", &g_fShowResourceUnloading },
    { "showloading", &g_fShowResourceLoading },
    { "surfaces:", (int *)&g_StartupGraphicsDriverConfig.m_cbSize },
    { "memorysize:", (int *)&g_StartupGraphicsDriverConfig.m_dwStyle },
    { "nodebug", &g_fDebugMessageThreadRunning },
    { "debugfile", &g_fDebugFileEnabled },
    { "gc:", &g_nGarbageCollectorMode },
};

static char g_abStreamFixedBuffer[0x400];
static VSINIT_FixedBufferStream *g_pStartupFixedBufferStream = 0;
static VSINIT_FixedBufferStream *g_pStatusFixedBufferStream = 0;
static VSINIT_FixedBufferStream *g_pErrorFixedBufferStream = 0;
static VSINIT_FormattedOutputStream *g_pStartupOutputStream = 0;
static VSINIT_FormattedOutputStream *g_pStatusOutputStream = 0;
VSINIT_FormattedOutputStream *g_pErrorOutputStream = 0;
static char *g_apszParsedArgs[16];
static unsigned int g_cParsedArgs = 0;
static int g_fSubsystemsReady = 0;
static const char *g_pszDebugOutPath = g_VSINIT_DebugOutPath;
static FILE *g_pDebugOutFile = 0;
static void *g_pProcessCurrentDirectoryState = 0;
static char g_abProcessCurrentDirectoryBuffer[0x100];
static HANDLE g_hDebugSyncEvent = 0;
static HANDLE g_hDebugMessageThread = 0;
static DWORD g_dwDebugMessageThreadId = 0;
static VSWIN_InvisibleMessageWindow g_InvisibleMessageWindow;
static long g_cbMainArenaAvailableAfterInit = 0;
static unsigned int g_uViSOSMajorVersion = 1;
static unsigned int g_uViSOSMinorVersion = 0;
static unsigned int g_uViSOSBuildNumber = 201;
static void *g_StatusEntryPointerArrayVtable[1] = { (void *)WriteStatusEntryPointerArray };
static VSINIT_ResourceTypeTable *g_pPrimaryResourceTypeTable = 0;
static VSINIT_ResourceTypeTable *g_pSecondaryResourceTypeTable = 0;
static VSINIT_ResourceTypeTable *g_pTertiaryResourceTypeTable = 0;
static VSINIT_VSMemPointerTable *g_pPaletteRemapPointerTable = 0;
static void *g_pSharedRenderQueueNode = 0;
static void *g_SharedRenderDispatchQueueVtable[1] = { 0 };
static void *g_SharedRenderDispatchQueueVariantVtable[1] = { 0 };
static void *g_SharedRenderQueueNodeVtable[2] = { 0 };
static void *g_SharedGeometryHelperVtable[1] = { 0 };
static void *g_MainMemoryArenaStatusEntryVtable[8] = {
    (void *)WriteNamedStatusEntry,
    (void *)UpdateNamedStatusEntry,
    (void *)EnterObjectCriticalSection,
    (void *)LeaveObjectCriticalSection,
    (void *)EnterObjectCriticalSection,
    (void *)LeaveObjectCriticalSection,
    (void *)WriteMemoryArenaReport,
    (void *)DestroyMemoryArenaBaseStateReturnThis,
};
static jmp_buf g_GameStartupJumpBuffer;
static jmp_buf g_DebugThreadJumpBuffer;

static HANDLE HostCreateEventA(LPSECURITY_ATTRIBUTES pEventAttributes,
                               BOOL fManualReset,
                               BOOL fInitialState,
                               LPCSTR pszName) {
    (void)pEventAttributes;
    (void)fManualReset;
    (void)fInitialState;
    (void)pszName;
    return (HANDLE)1;
}

static HANDLE HostCreateThread(LPSECURITY_ATTRIBUTES pThreadAttributes,
                               DWORD cbStackSize,
                               LPTHREAD_START_ROUTINE pfnThreadStart,
                               LPVOID pvThreadParam,
                               DWORD dwCreationFlags,
                               LPDWORD pdwThreadId) {
    (void)pThreadAttributes;
    (void)cbStackSize;
    (void)dwCreationFlags;

    if (pdwThreadId != 0) {
        *pdwThreadId = 1;
    }

    if (pfnThreadStart != 0) {
        (void)pfnThreadStart(pvThreadParam);
    }

    return (HANDLE)1;
}

static BOOL HostSetThreadPriority(HANDLE hThread, int nPriority) {
    (void)hThread;
    (void)nPriority;
    return 1;
}

static DWORD HostWaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds) {
    (void)hHandle;
    (void)dwMilliseconds;
    return WAIT_OBJECT_0;
}

static BOOL HostTerminateThread(HANDLE hThread, DWORD dwExitCode) {
    (void)hThread;
    (void)dwExitCode;
    return 1;
}

static void HostExitProcess(UINT uExitCode) {
    (void)uExitCode;
}

static DWORD CALLBACK HostDebugMessageThreadMain(LPVOID pvThreadParam) {
    (void)pvThreadParam;
    return (DWORD)DebugMessageThreadMain();
}

void EnterObjectCriticalSection(void *pObject) {
    EnterCriticalSection((char *)pObject + 4);
}

void LeaveObjectCriticalSection(void *pObject) {
    LeaveCriticalSection((char *)pObject + 4);
}

void FlushStatusFixedBufferStream(char *pszText) {
    if (g_fDebugFileEnabled != 0) {
        AppendStringToDebugOutFile(pszText);
    }
}

void FlushStartupFixedBufferStream(char *pszText) {
    if (g_fDebugFileEnabled != 0) {
        AppendStringToDebugOutFile(pszText);
    }
}

void FlushErrorFixedBufferStream(char *pszText) {
    if (g_fDebugFileEnabled != 0) {
        AppendStringToDebugOutFile(pszText);
    }
}

// FUNCTION: LEMBALL 0x004583E0
void *ConstructStreamBaseVtable(void *pStream) {
    *(void **)pStream = g_StreamBaseVtable;
    return pStream;
}

// FUNCTION: LEMBALL 0x00458400
void DestroyStreamBaseVtable(void *pStream) {
    *(void **)pStream = g_StreamBaseVtable;
}

// FUNCTION: LEMBALL 0x00458410
VSINIT_StreamFormatTargetState *ConstructStreamFormatTargetState(VSINIT_StreamFormatTargetState *pState,
                                                                        VSINIT_FixedBufferStream *pDownstream) {
    pState->m_pVtable = g_StreamFormatTargetStateVtable;
    pState->m_chFill = ' ';
    pState->m_dwFlags = 0x14;
    pState->m_nWidth = 0;
    pState->m_nRadix = 10;
    pState->m_pDownstream = pDownstream;
    return pState;
}

// FUNCTION: LEMBALL 0x00458440
void ConstructStreamFormatState(VSINIT_StreamFormatTargetState *pState) {
    pState->m_pVtable = g_StreamFormatTargetStateVtable;
}

// FUNCTION: LEMBALL 0x00458450
VSINIT_FormattedOutputStream *ConstructFormattedOutputStream(VSINIT_FormattedOutputStream *pStream,
                                                                    VSINIT_FixedBufferStream *pDownstream,
                                                                    int fConstructTargetState) {
    if (fConstructTargetState != 0) {
        pStream->m_pVtable = g_FormattedOutputStreamVtable;
        ConstructStreamFormatTargetState(&pStream->m_TargetState, pDownstream);
    }
    pStream->m_TargetState.m_pVtable = g_StreamFormatSubobjectVtable;
    return pStream;
}

// FUNCTION: LEMBALL 0x004584A0
void RestoreStreamFormatSubobjectVtable(VSINIT_StreamFormatTargetState *pState) {
    VSINIT_FormattedOutputStream *pStream;

    pStream = (VSINIT_FormattedOutputStream *)((char *)pState - 0x12c);
    pStream->m_TargetState.m_pVtable = g_StreamFormatSubobjectVtable;
}

// FUNCTION: LEMBALL 0x0045AD70
VSINIT_FixedBufferStream *ConstructFixedBufferStream(VSINIT_FixedBufferStream *pStream,
                                                           char *pszBuffer,
                                                           unsigned int cbBuffer,
                                                           void (*pfnFlush)(char *)) {
    unsigned int i;

    ConstructStreamBaseVtable(pStream);
    pStream->m_pVtable = g_FixedBufferStreamVtable;
    pStream->m_pfnFlush = pfnFlush;
    pStream->m_pszBuffer = pszBuffer;
    pStream->m_pszCursor = pszBuffer;
    pStream->m_cchWritten = 0;
    pStream->m_cbBuffer = cbBuffer;
    for (i = cbBuffer >> 2; i != 0; --i) {
        *(u32 *)pszBuffer = 0;
        pszBuffer += 4;
    }
    for (i = cbBuffer & 3; i != 0; --i) {
        *pszBuffer = '\0';
        ++pszBuffer;
    }
    return pStream;
}

void ResetFixedBufferStream(VSINIT_FixedBufferStream *pStream) {
    unsigned int i;
    char *pszBuffer;

    if (pStream->m_pfnFlush != 0) {
        pStream->m_pfnFlush(pStream->m_pszBuffer);
    }

    pszBuffer = pStream->m_pszBuffer;
    pStream->m_pszCursor = pszBuffer;
    pStream->m_cchWritten = 0;
    pStream->m_nTabWidth = 8;
    for (i = pStream->m_cbBuffer >> 2; i != 0; --i) {
        *(u32 *)pszBuffer = 0;
        pszBuffer += 4;
    }
    for (i = pStream->m_cbBuffer & 3; i != 0; --i) {
        *pszBuffer = '\0';
        ++pszBuffer;
    }
}

// FUNCTION: LEMBALL 0x0045ADC0
void DestroyFixedBufferStream(VSINIT_FixedBufferStream *pStream) {
    pStream->m_pVtable = g_FixedBufferStreamVtable;
    DestroyStreamBaseVtable(pStream);
}

static void DeleteFixedBufferStream(VSINIT_FixedBufferStream *pStream, int fFreeMemory) {
    ResetFixedBufferStream(pStream);
    DestroyFixedBufferStream(pStream);
    if (fFreeMemory != 0) {
        FreeVSMemBlock(pStream);
    }
}

void *DeleteFixedBufferStreamReturnThis(VSINIT_FixedBufferStream *pStream, unsigned char fFreeMemory) {
    DestroyFixedBufferStream(pStream);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pStream);
    }
    return pStream;
}

static void DestroyFormattedOutputStream(VSINIT_FormattedOutputStream *pStream, int fFreeMemory) {
    RestoreStreamFormatSubobjectVtable(&pStream->m_TargetState);
    if (fFreeMemory != 0) {
        FreeVSMemBlock(pStream);
    }
}

void AppendCharToFixedBufferStream(VSINIT_FixedBufferStream *pStream, char ch) {
    char *pszCursor;
    int nRemainder;
    int fTab;

    fTab = 0;
    if (ch == '\t') {
        ch = ' ';
        fTab = 1;
    } else if (ch == '\n') {
        pszCursor = pStream->m_pszCursor;
        *pszCursor = ch;
        ++pszCursor;
        pStream->m_pszCursor = pszCursor;
        *pszCursor = '\0';
        ++pStream->m_cchWritten;
        if (pStream->m_cbBuffer - pStream->m_cchWritten == 1) {
            ResetFixedBufferStream(pStream);
        }
        if (pStream->m_pfnFlush != 0) {
            ResetFixedBufferStream(pStream);
        }
        return;
    }

    pszCursor = pStream->m_pszCursor;
    *pszCursor = ch;
    ++pszCursor;
    pStream->m_pszCursor = pszCursor;
    *pszCursor = '\0';
    ++pStream->m_cchWritten;

    if (pStream->m_cbBuffer - pStream->m_cchWritten == 1) {
        ResetFixedBufferStream(pStream);
    }

    if (fTab == 0) {
        return;
    }

    nRemainder = (int)pStream->m_cchWritten % pStream->m_nTabWidth;
    while (nRemainder != 0) {
        pszCursor = pStream->m_pszCursor;
        *pszCursor = ' ';
        ++pszCursor;
        pStream->m_pszCursor = pszCursor;
        *pszCursor = '\0';
        ++pStream->m_cchWritten;
        if (pStream->m_cbBuffer - pStream->m_cchWritten == 1) {
            ResetFixedBufferStream(pStream);
        }
        nRemainder = (int)pStream->m_cchWritten % pStream->m_nTabWidth;
    }
}

void AppendCStringToFixedBufferStream(VSINIT_FixedBufferStream *pStream, const char *pszText) {
    while (*pszText != '\0') {
        AppendCharToFixedBufferStream(pStream, *pszText);
        ++pszText;
    }
}

void *ReturnStreamArgument(void *pStream, void *pArgument) {
    (void)pStream;
    return pArgument;
}

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
void AppendStringToDebugOutFile(const char *pszText) {
    size_t cchText;

    if (g_pszDebugOutPath == 0 || pszText == 0 || *pszText == '\0') {
        return;
    }

    cchText = strlen(pszText);
    if (cchText == 0) {
        return;
    }

    g_pDebugOutFile = fopen(g_pszDebugOutPath, "a");
    if (g_pDebugOutFile == 0) {
        return;
    }

    fwrite(pszText, 1, cchText, g_pDebugOutFile);
    fflush(g_pDebugOutFile);
    fclose(g_pDebugOutFile);
    g_pDebugOutFile = 0;
}

// FUNCTION: LEMBALL 0x00458F10
const char *SelectSuccessOrFailedString(int fSuccess) {
    const char *pszResult;

    pszResult = g_VSINIT_Success;
    if (fSuccess == 0) {
        pszResult = g_VSINIT_Failed;
    }
    return pszResult;
}

// FUNCTION: LEMBALL 0x00458F30
int ParseDecimalIntAndAdvance(char *pszText, char **ppszEnd, int nRadix) {
    char chCurrent;
    char chDigit;
    int nValue;

    if (nRadix != 10) {
        *ppszEnd = pszText;
        return 0;
    }

    nValue = 0;
    chCurrent = *pszText;
    while ('/' < chCurrent && (chDigit = *pszText, chDigit < ':')) {
        ++pszText;
        chCurrent = *pszText;
        nValue = chDigit + -0x30 + nValue * 10;
    }

    *ppszEnd = pszText;
    return nValue;
}

// FUNCTION: LEMBALL 0x00472BE0
int InitializeDebugMessageThread(void) {
    (void)g_VSINIT_BaseWindowClass;

    if (g_fDebugMessageThreadRunning == 1) {
        g_hDebugSyncEvent = HostCreateEventA(0, 0, 0, g_VSINIT_SyncDebugName);
        g_hDebugMessageThread =
            HostCreateThread(0, 0, HostDebugMessageThreadMain, 0, 0, &g_dwDebugMessageThreadId);
        if (g_hDebugMessageThread == 0) {
            lemball_platform_show_error(g_VSINIT_ErrorTitle, g_VSINIT_DebugStartError);
            HostExitProcess(0xbbbb);
            return 0;
        }

        HostSetThreadPriority(g_hDebugMessageThread, 1);
        HostWaitForSingleObject(g_hDebugSyncEvent, INFINITE);
    }

    return 1;
}

// FUNCTION: LEMBALL 0x00472C70
int ShutdownDebugMessageThread(int fForceTerminate) {
    if (g_fDebugMessageThreadRunning == 1) {
        if (fForceTerminate == 0) {
            HostWaitForSingleObject(g_hDebugSyncEvent, INFINITE);
        } else {
            HostTerminateThread(g_hDebugMessageThread, 0xaaaa);
        }
        g_fDebugMessageThreadRunning = 0;
        return 1;
    }

    if (g_pDebugOutFile != 0) {
        fclose(g_pDebugOutFile);
        g_pDebugOutFile = 0;
    }

    return 1;
}

int ShutdownDebugMessageThreadFromStartup(int fForceTerminate) {
    return ShutdownDebugMessageThread(fForceTerminate);
}

// FUNCTION: LEMBALL 0x0045EC90
void *ConstructProcessCurrentDirectoryMarker(void *pMarker) {
    return pMarker;
}

// FUNCTION: LEMBALL 0x0045ECA0
void DestroyProcessCurrentDirectoryMarker(void) {
}

// FUNCTION: LEMBALL 0x00458F70
int InitializeStreamChannels(void) {
    void *pStream;

    pStream = AllocateVSMemBlock(0x1c);
    if (pStream == 0) {
        g_pStatusFixedBufferStream = 0;
    } else {
        g_pStatusFixedBufferStream =
            ConstructFixedBufferStream((VSINIT_FixedBufferStream *)pStream, g_abStreamFixedBuffer, 0x400,
                                       FlushStatusFixedBufferStream);
    }

    pStream = AllocateVSMemBlock(0x1c);
    if (pStream == 0) {
        g_pStartupFixedBufferStream = 0;
    } else {
        g_pStartupFixedBufferStream =
            ConstructFixedBufferStream((VSINIT_FixedBufferStream *)pStream, g_abStreamFixedBuffer, 0x400,
                                       FlushStartupFixedBufferStream);
    }

    pStream = AllocateVSMemBlock(0x1c);
    if (pStream == 0) {
        g_pErrorFixedBufferStream = 0;
    } else {
        g_pErrorFixedBufferStream =
            ConstructFixedBufferStream((VSINIT_FixedBufferStream *)pStream, g_abStreamFixedBuffer, 0x400,
                                       FlushErrorFixedBufferStream);
    }

    pStream = AllocateVSMemBlock(0x14c);
    if (pStream == 0) {
        g_pStatusOutputStream = 0;
    } else {
        g_pStatusOutputStream =
            ConstructFormattedOutputStream((VSINIT_FormattedOutputStream *)pStream, g_pStatusFixedBufferStream, 1);
    }

    pStream = AllocateVSMemBlock(0x14c);
    if (pStream == 0) {
        g_pStartupOutputStream = 0;
    } else {
        g_pStartupOutputStream =
            ConstructFormattedOutputStream((VSINIT_FormattedOutputStream *)pStream, g_pStartupFixedBufferStream, 1);
    }

    pStream = AllocateVSMemBlock(0x14c);
    if (pStream == 0) {
        g_pErrorOutputStream = 0;
    } else {
        g_pErrorOutputStream =
            ConstructFormattedOutputStream((VSINIT_FormattedOutputStream *)pStream, g_pErrorFixedBufferStream, 1);
    }

    return 1;
}

// FUNCTION: LEMBALL 0x00456680
int InitializeProcessCurrentDirectoryState(void) {
    void *pMarker;
    size_t cchDirectory;

    pMarker = AllocateVSMemBlock(1);
    if (pMarker == 0) {
        g_pProcessCurrentDirectoryState = 0;
    } else {
        g_pProcessCurrentDirectoryState = ConstructProcessCurrentDirectoryMarker(pMarker);
    }

    GetCurrentDirectoryA(sizeof(g_abProcessCurrentDirectoryBuffer), g_abProcessCurrentDirectoryBuffer);
    cchDirectory = strlen(g_abProcessCurrentDirectoryBuffer);
    if (g_abProcessCurrentDirectoryBuffer[cchDirectory - 1] == '\\') {
        g_abProcessCurrentDirectoryBuffer[cchDirectory - 1] = '\0';
    }

    return 1;
}

// FUNCTION: LEMBALL 0x004630A0
void *ConstructRenderDispatchQueueVariant(void *pQueue, int cEntries) {
    int *pQueueWords;
    void *pEntryBuffer;

    pQueueWords = (int *)pQueue;
    memset(pQueue, 0, sizeof(VSINIT_RenderDispatchQueue));
    pQueueWords[0] = (int)(unsigned long)g_SharedRenderDispatchQueueVtable;
    pQueueWords[2] = (int)(unsigned long)g_SharedRenderDispatchQueueVariantVtable;
    pEntryBuffer = AllocateVSMemBlock((unsigned int)(cEntries * 0x14));
    *(void **)((char *)pQueue + 0x44) = pEntryBuffer;
    *(int *)((char *)pQueue + 0x24) = cEntries;
    *(char **)((char *)pQueue + 0x48) = (char *)pEntryBuffer + cEntries * 0x14;
    *(void **)((char *)pQueue + 0x50) = pEntryBuffer;
    *(void **)((char *)pQueue + 0x4c) = pEntryBuffer;
    return pQueue;
}

// FUNCTION: LEMBALL 0x00463120
void DestroyRenderDispatchQueue(void *pQueue) {
    int cClients;
    void *pClientNode;
    void *pNextClientNode;

    if (*(void **)((char *)pQueue + 0x44) != 0) {
        FreeVSMemBlock(*(void **)((char *)pQueue + 0x44));
    }
    cClients = *(int *)((char *)pQueue + 0x2c);
    pClientNode = *(void **)((char *)pQueue + 0x54);
    while (cClients != 0) {
        pNextClientNode = *(void **)((char *)pClientNode + 8);
        FreeVSMemBlock(pClientNode);
        pClientNode = pNextClientNode;
        --cClients;
    }
}

// FUNCTION: LEMBALL 0x00472070
void *ConstructSharedGeometryHelper(void *pHelper, void *pRenderDispatchQueue) {
    VSINIT_SharedGeometryHelper *pSharedHelper;

    pSharedHelper = (VSINIT_SharedGeometryHelper *)pHelper;
    pSharedHelper->m_pVtable = g_SharedGeometryHelperVtable;
    pSharedHelper->m_nReserved08 = 0;
    pSharedHelper->m_nReserved10 = 0;
    pSharedHelper->m_pRenderDispatchQueue = pRenderDispatchQueue;
    pSharedHelper->m_dwFlags = 0;
    return pSharedHelper;
}

// FUNCTION: LEMBALL 0x00456660
int SetSharedGeometryHelperFlags03(void) {
    ((VSINIT_SharedGeometryHelper *)g_pSharedGeometryHelper)->m_dwFlags |= 3;
    return 1;
}

// FUNCTION: LEMBALL 0x00456670
int ClearSharedGeometryHelperFlags03(void) {
    ((VSINIT_SharedGeometryHelper *)g_pSharedGeometryHelper)->m_dwFlags &= ~3;
    return 1;
}

// FUNCTION: LEMBALL 0x00459130
int InitializeSharedEventQueueRuntime(void) {
    void *pQueue;
    void *pHelper;
    void *pRenderQueueNode;

    pQueue = AllocateVSMemBlock(0x58);
    if (pQueue == 0) {
        g_pSharedRenderDispatchQueue = 0;
    } else {
        g_pSharedRenderDispatchQueue = ConstructRenderDispatchQueueVariant(pQueue, 10);
        *(void **)g_pSharedRenderDispatchQueue = g_SharedRenderDispatchQueueVtable;
        *(void **)((char *)g_pSharedRenderDispatchQueue + 8) = g_SharedRenderDispatchQueueVariantVtable;
    }

    pHelper = AllocateVSMemBlock(0x18);
    if (pHelper == 0) {
        g_pSharedGeometryHelper = 0;
    } else {
        g_pSharedGeometryHelper = ConstructSharedGeometryHelper(pHelper, g_pSharedRenderDispatchQueue);
    }

    pRenderQueueNode = AllocateVSMemBlock(0x10);
    if (pRenderQueueNode == 0) {
        g_pSharedRenderQueueNode = 0;
    } else {
        InitializeRenderQueueNodeBase(pRenderQueueNode);
        *(void **)pRenderQueueNode = g_SharedRenderQueueNodeVtable;
        g_pSharedRenderQueueNode = pRenderQueueNode;
    }

    RegisterOrderedRenderDispatchClient(g_pSharedRenderDispatchQueue, g_pSharedRenderQueueNode, -0x32);
    SetSharedGeometryHelperFlags03();
    return 1;
}

// FUNCTION: LEMBALL 0x00462E60
int InitializeTimingSubsystemStub(void) {
    return 1;
}

// FUNCTION: LEMBALL 0x0045AAF0
VSINIT_StatusEntryPointerArray *ConstructStatusEntryPointerArray(void *pRegistry, int cEntriesMax) {
    VSINIT_StatusEntryPointerArray *pPointerArray;

    pPointerArray = (VSINIT_StatusEntryPointerArray *)pRegistry;
    pPointerArray->m_pVtable = g_StatusEntryPointerArrayVtable;
    pPointerArray->m_ppEntries = (void **)AllocateVSMemBlock(cEntriesMax * sizeof(void *));
    pPointerArray->m_cEntriesMax = cEntriesMax;
    pPointerArray->m_cEntries = 0;
    return pPointerArray;
}

VSINIT_FormattedOutputStream *WriteStatusEntryPointerArray(void *pRegistry, VSINIT_FormattedOutputStream *pStream) {
    VSINIT_StatusEntryPointerArray *pPointerArray;
    VSINIT_FormattedOutputStream *(*pfnWriteEntry)(void *, VSINIT_FormattedOutputStream *);
    int i;

    pPointerArray = (VSINIT_StatusEntryPointerArray *)pRegistry;
    AppendCStringToStream(pStream, g_VSINIT_StatusOutputHeader);
    AppendCStringToStream(pStream, g_VSINIT_StatusOutputRule);
    AppendCStringToStream(pStream, g_VSINIT_StatusOutputColumns);
    for (i = 0; i < pPointerArray->m_cEntries; ++i) {
        pfnWriteEntry = (VSINIT_FormattedOutputStream * (*)(void *, VSINIT_FormattedOutputStream *))
            (*(void ***)pPointerArray->m_ppEntries[i])[0];
        pfnWriteEntry(pPointerArray->m_ppEntries[i], pStream);
    }
    AppendCStringToStream(pStream, g_VSINIT_StatusOutputRule);
    return pStream;
}

// FUNCTION: LEMBALL 0x0045AB30
void DestroyStatusEntryPointerArray(void *pRegistry) {
    VSINIT_StatusEntryPointerArray *pPointerArray;
    int i;

    pPointerArray = (VSINIT_StatusEntryPointerArray *)pRegistry;
    pPointerArray->m_pVtable = g_StatusEntryPointerArrayVtable;
    if (pPointerArray->m_ppEntries != 0) {
        for (i = 0; i < pPointerArray->m_cEntries; ++i) {
            if (pPointerArray->m_ppEntries[i] != 0) {
                DestroyNamedStatusEntry(pPointerArray->m_ppEntries[i]);
                FreeVSMemBlock(pPointerArray->m_ppEntries[i]);
            }
        }
        FreeVSMemBlock(pPointerArray->m_ppEntries);
        pPointerArray->m_ppEntries = 0;
    }
}

// FUNCTION: LEMBALL 0x0045AB90
void AppendStatusEntryToRegistry(void *pRegistry, void *pEntry) {
    VSINIT_StatusEntryPointerArray *pPointerArray;

    pPointerArray = (VSINIT_StatusEntryPointerArray *)pRegistry;
    if (pPointerArray->m_cEntries < pPointerArray->m_cEntriesMax) {
        pPointerArray->m_ppEntries[pPointerArray->m_cEntries] = pEntry;
        ++pPointerArray->m_cEntries;
    }
}

// FUNCTION: LEMBALL 0x0045AA80
int InitializeStatusEntryRegistry(void) {
    void *pRegistry;

    pRegistry = AllocateVSMemBlock(0x14);
    if (pRegistry == 0) {
        g_pStatusEntryRegistry = 0;
    } else {
        g_pStatusEntryRegistry = ConstructStatusEntryPointerArray(pRegistry, 0x20);
    }

    return g_pStatusEntryRegistry != 0;
}

static VSINIT_ResourceTypeTable *ConstructResourceTypeTable(void *pTable, int cTagsMax) {
    VSINIT_ResourceTypeTable *pResourceTypeTable;

    pResourceTypeTable = (VSINIT_ResourceTypeTable *)pTable;
    pResourceTypeTable->m_cTagsMax = cTagsMax;
    pResourceTypeTable->m_nCursor = -1;
    pResourceTypeTable->m_cTags = 0;
    pResourceTypeTable->m_pTags = (u32 *)AllocateVSMemBlock((unsigned int)(pResourceTypeTable->m_cTagsMax << 2));
    return pResourceTypeTable;
}

static void AppendResourceTypeTag(VSINIT_ResourceTypeTable *pTable, u32 uTag) {
    pTable->m_pTags[pTable->m_cTags] = uTag;
    ++pTable->m_cTags;
}

// FUNCTION: LEMBALL 0x0046ACD0
VSINIT_VSMemPointerTable *ConstructVSMemPointerTable(void *pTable, int cItems) {
    VSINIT_VSMemPointerTable *pPointerTable;
    int i;

    pPointerTable = (VSINIT_VSMemPointerTable *)pTable;
    pPointerTable->m_cItems = cItems;
    pPointerTable->m_nCursor = 0;
    pPointerTable->m_ppItems = (void **)AllocateVSMemBlock((unsigned int)(cItems << 2));
    for (i = 0; i < pPointerTable->m_cItems; ++i) {
        pPointerTable->m_ppItems[i] = 0;
    }
    return pPointerTable;
}

// FUNCTION: LEMBALL 0x00473630
void *ConstructVSMemPointerTableWrapper(void *pTable, int cItems) {
    ConstructVSMemPointerTable(pTable, cItems);
    return pTable;
}

// FUNCTION: LEMBALL 0x0046AAD0
void ReleasePaletteRemapVariantFields(VSINIT_PaletteRemapVariant *pVariant) {
    if (pVariant->m_pRemapTable != 0) {
        FreeVSMemBlock(pVariant->m_pRemapTable);
    }
    if (pVariant->m_pResourceObject != 0) {
        ReleaseTypedResourceObjectReference(pVariant->m_pResourceObject);
    }
}

// FUNCTION: LEMBALL 0x0046AD10
void DestroyPaletteRemapPointerTable(VSINIT_VSMemPointerTable *pPointerTable) {
    int i;

    if (pPointerTable->m_ppItems != 0 && 0 < pPointerTable->m_nCursor) {
        for (i = 0; i < pPointerTable->m_nCursor; ++i) {
            if (pPointerTable->m_ppItems[i] != 0) {
                ReleasePaletteRemapVariantFields((VSINIT_PaletteRemapVariant *)pPointerTable->m_ppItems[i]);
                FreeVSMemBlock(pPointerTable->m_ppItems[i]);
            }
            pPointerTable->m_ppItems[i] = 0;
        }
    }
    if (pPointerTable->m_ppItems != 0) {
        FreeVSMemBlock(pPointerTable->m_ppItems);
    }
}

// FUNCTION: LEMBALL 0x0045B900
int InitializeResourceTypeTables(void) {
    VSINIT_ResourceTypeTable *pResourceTypeTable;
    void *pPointerTable;

    pResourceTypeTable = ConstructResourceTypeTable(AllocateVSMemBlock(0x10), 2);
    AppendResourceTypeTag(pResourceTypeTable, 0x494e5420);
    AppendResourceTypeTag(pResourceTypeTable, 0x5a524c45);
    g_pSecondaryResourceTypeTable = pResourceTypeTable;

    pResourceTypeTable = ConstructResourceTypeTable(AllocateVSMemBlock(0x10), 1);
    AppendResourceTypeTag(pResourceTypeTable, 0x5a524c45);
    g_pPrimaryResourceTypeTable = pResourceTypeTable;

    pResourceTypeTable = ConstructResourceTypeTable(AllocateVSMemBlock(0x10), 2);
    AppendResourceTypeTag(pResourceTypeTable, 0x53545247);
    AppendResourceTypeTag(pResourceTypeTable, 0x494e5420);
    g_pTertiaryResourceTypeTable = pResourceTypeTable;

    pPointerTable = AllocateVSMemBlock(0xc);
    if (pPointerTable != 0) {
        g_pPaletteRemapPointerTable = (VSINIT_VSMemPointerTable *)ConstructVSMemPointerTableWrapper(pPointerTable, 0x20);
        return 1;
    }
    g_pPaletteRemapPointerTable = 0;
    return 1;
}

// FUNCTION: LEMBALL 0x0045BA50
int ShutdownResourceTypeTables(void) {
    if (g_pPaletteRemapPointerTable != 0) {
        DestroyPaletteRemapPointerTable(g_pPaletteRemapPointerTable);
        FreeVSMemBlock(g_pPaletteRemapPointerTable);
        g_pPaletteRemapPointerTable = 0;
    }
    if (g_pTertiaryResourceTypeTable != 0) {
        FreeVSMemBlock(g_pTertiaryResourceTypeTable->m_pTags);
        FreeVSMemBlock(g_pTertiaryResourceTypeTable);
        g_pTertiaryResourceTypeTable = 0;
    }
    if (g_pSecondaryResourceTypeTable != 0) {
        FreeVSMemBlock(g_pSecondaryResourceTypeTable->m_pTags);
        FreeVSMemBlock(g_pSecondaryResourceTypeTable);
        g_pSecondaryResourceTypeTable = 0;
    }
    if (g_pPrimaryResourceTypeTable != 0) {
        FreeVSMemBlock(g_pPrimaryResourceTypeTable->m_pTags);
        FreeVSMemBlock(g_pPrimaryResourceTypeTable);
        g_pPrimaryResourceTypeTable = 0;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x0045AAB0
int ShutdownStatusEntryRegistry(void) {
    void *pRegistry;
    VSINIT_FormattedOutputStream *(*pfnWriteRegistry)(void *, VSINIT_FormattedOutputStream *);

    pRegistry = g_pStatusEntryRegistry;
    if (g_pStatusEntryRegistry != 0) {
        pfnWriteRegistry =
            (VSINIT_FormattedOutputStream * (*)(void *, VSINIT_FormattedOutputStream *))(*(void ***)pRegistry)[0];
        pfnWriteRegistry(pRegistry, g_pStartupOutputStream);
        DestroyStatusEntryPointerArray(g_pStatusEntryRegistry);
        FreeVSMemBlock(pRegistry);
        g_pStatusEntryRegistry = 0;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00462E70
int ShutdownTimingSubsystemStub(void) {
    return 1;
}

// FUNCTION: LEMBALL 0x004591F0
int ShutdownSharedEventQueueRuntime(void) {
    int fResult;

    fResult = ClearSharedGeometryHelperFlags03();
    UnregisterOrderedRenderDispatchClient(g_pSharedRenderDispatchQueue, g_pSharedRenderQueueNode, -0x32);
    if (g_pSharedRenderQueueNode != 0) {
        FreeVSMemBlock(g_pSharedRenderQueueNode);
        g_pSharedRenderQueueNode = 0;
    }
    if (g_pSharedGeometryHelper != 0) {
        FreeVSMemBlock(g_pSharedGeometryHelper);
        g_pSharedGeometryHelper = 0;
    }
    if (g_pSharedRenderDispatchQueue != 0) {
        DestroyRenderDispatchQueue(g_pSharedRenderDispatchQueue);
        FreeVSMemBlock(g_pSharedRenderDispatchQueue);
        g_pSharedRenderDispatchQueue = 0;
    }
    return fResult;
}

// FUNCTION: LEMBALL 0x004566F0
int ShutdownProcessCurrentDirectoryState(void) {
    if (g_pProcessCurrentDirectoryState != 0) {
        DestroyProcessCurrentDirectoryMarker();
        FreeVSMemBlock(g_pProcessCurrentDirectoryState);
        g_pProcessCurrentDirectoryState = 0;
    }

    return 1;
}

// FUNCTION: LEMBALL 0x004590B0
int ShutdownStreamChannels(void) {
    if (g_pErrorOutputStream != 0) {
        DestroyFormattedOutputStream(g_pErrorOutputStream, 1);
    }
    if (g_pStartupOutputStream != 0) {
        DestroyFormattedOutputStream(g_pStartupOutputStream, 1);
    }
    if (g_pStatusOutputStream != 0) {
        DestroyFormattedOutputStream(g_pStatusOutputStream, 1);
    }
    if (g_pErrorFixedBufferStream != 0) {
        DeleteFixedBufferStream(g_pErrorFixedBufferStream, 1);
    }
    if (g_pStartupFixedBufferStream != 0) {
        DeleteFixedBufferStream(g_pStartupFixedBufferStream, 1);
    }
    if (g_pStatusFixedBufferStream != 0) {
        DeleteFixedBufferStream(g_pStatusFixedBufferStream, 1);
    }

    return 1;
}

void LogParsedCommandLineOptions(void) {
    size_t i;

    AppendStatusCString(g_VSINIT_CommandLineOptionsHeader);

    for (i = 0; i < LEMBALL_ARRAY_COUNT(g_aCommandLineOptions); ++i) {
        AppendStatusCString(g_VSINIT_CommandLineOptionPrefix);
        AppendStatusCString(g_aCommandLineOptions[i].m_pszName);
        AppendStatusCString(g_VSINIT_CommandLineOptionIs);
        if (!g_afCommandLineOptionSpecified[i]) {
            AppendStatusCString(g_VSINIT_NotSelected);
        }
        AppendStatusCString(g_VSINIT_Selected);
    }
}

// FUNCTION: LEMBALL 0x00459250
void InitializeCoreSubsystems(void) {
    int fMemoryInitialized;
    int fStreamsInitialized;
    int fDebugInitialized;
    int fInputInitialized;
    int fTimeInitialized;
    int fGdiInitialized;
    int fStatusInitialized;
    int fResourcesInitialized;
    GAME_StatusEntry *pMainMemoryArenaStatusEntry;

    fMemoryInitialized = InitializeMasterMainRamArena();
    if (fMemoryInitialized == 0) {
        TriggerReleaseAssertFailure(g_VSINIT_EnoughMemoryAssert, g_VSINIT_SourceFileName, 0x19e);
    }

    fStreamsInitialized = InitializeStreamChannels();
    fDebugInitialized = InitializeDebugMessageThread();
    g_fSubsystemsReady = fDebugInitialized;
    InitializeProcessCurrentDirectoryState();

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

    AppendStartupCString(g_VSINIT_MemInitPrefix);
    AppendStartupCString(SelectSuccessOrFailedString(fMemoryInitialized));
    AppendStartupCString(" ");
    AppendStartupUInt(g_StartupGraphicsDriverConfig.m_dwStyle);
    AppendStartupCString(g_VSINIT_BytesSuffix);

    AppendStartupCString(g_VSINIT_StreamInitPrefix);
    AppendStartupCString(SelectSuccessOrFailedString(fStreamsInitialized));
    AppendStartupCString(g_VSINIT_LineBreak);

    AppendStartupCString(g_VSINIT_DebugInitPrefix);
    AppendStartupCString(SelectSuccessOrFailedString(fDebugInitialized));
    AppendStartupCString(g_VSINIT_LineBreak);

    g_cbMainArenaAvailableAfterInit = CalculateMemoryArenaAvailableBytes(g_pMainMemoryArena);

    fInputInitialized = InitializeSharedEventQueueRuntime();
    AppendStartupCString(g_VSINIT_InputInitPrefix);
    AppendStartupCString(SelectSuccessOrFailedString(fInputInitialized));
    AppendStartupCString(g_VSINIT_LineBreak);

    fTimeInitialized = InitializeTimingSubsystemStub();
    AppendStartupCString(g_VSINIT_TimeInitPrefix);
    AppendStartupCString(SelectSuccessOrFailedString(fTimeInitialized));
    AppendStartupCString(g_VSINIT_LineBreak);

    fGdiInitialized = InitializeResourceGeometryHelperRuntime();
    AppendStartupCString(g_VSINIT_GdiInitPrefix);
    AppendStartupCString(SelectSuccessOrFailedString(fGdiInitialized));
    AppendStartupCString(" ");
    AppendStartupUInt(g_StartupGraphicsDriverConfig.m_cbSize);
    AppendStartupCString(g_VSINIT_BytesSuffix);

    fStatusInitialized = InitializeStatusEntryRegistry();
    AppendStartupCString(g_VSINIT_StatusInitPrefix);
    AppendStartupCString(SelectSuccessOrFailedString(fStatusInitialized));
    AppendStartupCString(g_VSINIT_LineBreak);

    fResourcesInitialized = InitializeResourceTypeTables();
    AppendStartupCString(g_VSINIT_ResInitPrefix);
    AppendStartupCString(SelectSuccessOrFailedString(fResourcesInitialized));
    AppendStartupCString(g_VSINIT_LineBreak);

    pMainMemoryArenaStatusEntry =
        new (AllocateVSMemBlock(sizeof(GAME_StatusEntry))) GAME_StatusEntry(g_VSINIT_MainMemoryArenaName);
    if (pMainMemoryArenaStatusEntry != 0) {
        pMainMemoryArenaStatusEntry->m_pVtable = g_MainMemoryArenaStatusEntryVtable;
    }
    AppendStatusEntryToRegistry(g_pStatusEntryRegistry, pMainMemoryArenaStatusEntry);
    if (g_pMainMemoryArena != 0) {
        *(GAME_StatusEntry **)((char *)g_pMainMemoryArena + 0x30) = pMainMemoryArenaStatusEntry;
    }
}

// FUNCTION: LEMBALL 0x00459520
void ShutdownCoreSubsystems(void) {
    long cbAvailableNow;

    AppendStartupCString(g_VSINIT_LineBreak);
    AppendStatusCString(g_VSINIT_LineBreak);
    AppendErrorCString(g_VSINIT_LineBreak);

    ShutdownResourceTypeTables();
    ShutdownStatusEntryRegistry();
    ShutdownTimingSubsystemStub();
    ShutdownResourceGeometryHelperRuntime();
    ShutdownSharedEventQueueRuntime();

    cbAvailableNow = CalculateMemoryArenaAvailableBytes(g_pMainMemoryArena);
    if (cbAvailableNow != g_cbMainArenaAvailableAfterInit) {
        AppendErrorCString(g_VSINIT_MemoryLeakMessage);
        WriteMemoryArenaReport(g_pMainMemoryArena, g_pErrorOutputStream);
        AppendErrorCString(g_VSINIT_LineBreak);
    }

    ShutdownProcessCurrentDirectoryState();
    ShutdownDebugMessageThread(g_fStartupNoWait);
    ShutdownStreamChannels();
    ShutdownMasterMainRamArena();
}

// FUNCTION: LEMBALL 0x004595D0
int ParseCommandLineOptionToken(char *pszToken) {
    int i;

    if (*pszToken == '-' || *pszToken == '/') {
        ++pszToken;
        for (i = 0; i < (int)LEMBALL_ARRAY_COUNT(g_aCommandLineOptions); ++i) {
            const char *pszColon;
            size_t cchOption;

            pszColon = strchr(g_aCommandLineOptions[i].m_pszName, ':');
            if (pszColon == 0) {
                cchOption = strlen(g_aCommandLineOptions[i].m_pszName);
            } else {
                cchOption = (size_t)(pszColon - g_aCommandLineOptions[i].m_pszName);
            }

            if (strncmp(pszToken, g_aCommandLineOptions[i].m_pszName, cchOption) == 0) {
                if (strlen(g_aCommandLineOptions[i].m_pszName) == cchOption) {
                    *g_aCommandLineOptions[i].m_pnValue ^= 1;
                } else {
                    char *pszEnd;

                    *g_aCommandLineOptions[i].m_pnValue =
                        ParseDecimalIntAndAdvance(pszToken + cchOption + 1, &pszEnd, 10);
                }
                g_afCommandLineOptionSpecified[i] = 1;
                return 1;
            }
        }
    }

    return 0;
}

// FUNCTION: LEMBALL 0x004596B0
void TokenizeAndFilterCommandLineArgs(char *pszCmdLine) {
    char *pszCursor;
    char *pszNext;
    unsigned int i;

    g_cParsedArgs = 0;
    g_apszParsedArgs[0] = pszCmdLine;
    for (i = 0; i < LEMBALL_ARRAY_COUNT(g_afCommandLineOptionSpecified); ++i) {
        g_afCommandLineOptionSpecified[i] = 0;
    }

    if (pszCmdLine == 0) {
        return;
    }

    while (isspace((int)*pszCmdLine)) {
        ++pszCmdLine;
    }
    if (*pszCmdLine == '\0') {
        return;
    }

    g_cParsedArgs = 0;
    pszCursor = pszCmdLine;
    while (*pszCursor != '\0') {
        if (isupper((int)*pszCursor)) {
            *pszCursor = (char)tolower((int)*pszCursor);
        }

        if (isspace((int)*pszCursor)) {
            pszNext = pszCursor;
            do {
                pszCursor = pszNext;
                *pszCursor = '\0';
                pszNext = pszCursor + 1;
            } while (isspace((int)*pszNext));

            if (*pszNext != '\0') {
                ++g_cParsedArgs;
                g_apszParsedArgs[g_cParsedArgs] = pszNext;
            }
        }

        ++pszCursor;
    }

    ++g_cParsedArgs;
    if (g_cParsedArgs != 0) {
        unsigned int nRemaining;

        i = 0;
        nRemaining = g_cParsedArgs;
        while (nRemaining != 0) {
            if (!ParseCommandLineOptionToken(g_apszParsedArgs[i])) {
                ++i;
                --nRemaining;
                continue;
            }

            --g_cParsedArgs;
            --nRemaining;
            if (i < g_cParsedArgs) {
                unsigned int j;

                for (j = i; j < g_cParsedArgs; ++j) {
                    g_apszParsedArgs[j] = g_apszParsedArgs[j + 1];
                }
            }
        }
    }
}

unsigned int GetParsedCommandLineArgumentCount(void) {
    return g_cParsedArgs;
}

const char *const *GetParsedCommandLineArgs(void) {
    return (const char *const *)g_apszParsedArgs;
}

const VSINIT_CommandLineOption *GetCommandLineOptions(size_t *pcOptions) {
    if (pcOptions != 0) {
        *pcOptions = LEMBALL_ARRAY_COUNT(g_aCommandLineOptions);
    }
    return g_aCommandLineOptions;
}

// FUNCTION: LEMBALL 0x00459860
int RunGameStartupSequence(char *pszCmdLine) {
    int nResult;
    int nJumpResult;

    TokenizeAndFilterCommandLineArgs(pszCmdLine);
    FinalizeStartupGraphicsDriverConfig();
    InitializeCoreSubsystems();
    LogParsedCommandLineOptions();

    nJumpResult = setjmp(g_GameStartupJumpBuffer);
    if (nJumpResult != 0) {
        ShutdownCoreSubsystems();
        return nJumpResult;
    }

    nJumpResult = setjmp(g_DebugThreadJumpBuffer);
    if (nJumpResult != 0) {
        ShutdownDebugMessageThreadFromStartup(g_fStartupNoWait);
        return nJumpResult;
    }

    nResult = RunMainGameSession((int)GetParsedCommandLineArgumentCount(), GetParsedCommandLineArgs());
    ShutdownCoreSubsystems();
    return nResult;
}
