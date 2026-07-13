#include "../game/game_app.h"
#include "../platform/startup_options.h"

#include "../engine/graphics_driver.h"
#include "../engine/memory_arena.h"
#include "runtime_init.h"
#include "../platform/message_window.h"
#include "../network/safe_vtable.h"

#include <ctype.h>
#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif
#include <setjmp.h>
#include <stdio.h>
#include <string.h>

extern "C" int __cdecl _isctype(int nCharacter, int nType);
#undef _tolower
extern "C" int __cdecl _tolower(int nCharacter);
extern char *FindCharInCString0047FE00(char *pszText, char ch);

#define LEMBALL_CRT_ISCTYPE(nCharacter, nType) \
    (MB_CUR_MAX > 1 ? _isctype((nCharacter), (nType)) : (_pctype[(nCharacter)] & (nType)))

void AppendStringToDebugOutFile(const char *pszText);
extern void TriggerReleaseAssertFailure(const char *pszExpression, const char *pszFile, int nLine);
extern void LEMBALL_FASTCALL ReleaseTypedResourceObjectReference(void *pResourceObject);
extern void *LEMBALL_FASTCALL InitializeRenderQueueNodeBase(void *pRenderQueueNode);
extern void *g_pStatusEntryRegistry;
extern void *g_pSharedRenderDispatchQueue;
extern void *g_pSharedGeometryHelper;

#define VSINIT_FORMAT_TARGET(pStream) \
    ((VSINIT_StreamFormatTargetState *)((char *)(pStream) + (unsigned long)((void **)((pStream)->m_pVtable))[1]))

void FlushStatusFixedBufferStream(char *pszText);
void FlushStartupFixedBufferStream(char *pszText);
void FlushErrorFixedBufferStream(char *pszText);
static void DestroyFormattedOutputStream(VSINIT_FormattedOutputStream *pStream, int fFreeMemory);
static void DeleteFixedBufferStream(VSINIT_FixedBufferStream *pStream, int fFreeMemory);
void FormatSignedIntToRadixString(int nValue, char *pszBuffer, unsigned int nRadix);
char *FormatUnsignedIntToRadixString(unsigned int uValue, char *pszBuffer, unsigned int nRadix);
void LEMBALL_FASTCALL ApplyStreamIntegerWidthPadding(VSINIT_FormattedOutputStream *pStream);

typedef void *(LEMBALL_FASTCALL *VSINIT_DeleteProc)(
    void *pStream, int nUnused, int fFreeMemory);
void *LEMBALL_FASTCALL DeleteStreamBase(
    void *pStream, int nUnused, int fFreeMemory);
void LEMBALL_FASTCALL DestroyStreamBaseVtable(void *pStream);
void *g_StreamBaseVtable[4] = {
    (void *)DeleteStreamBase,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
};
unsigned long g_FormattedOutputStreamVtable[2] = { 0, 0x12c };
unsigned long g_EffFormattedOutputStreamVtable[3] = { 0, 0x20, 0x40 };
unsigned long g_EffStreamSubobjectVtable[2] = { 0, (unsigned long)-0x20 };
// GLOBAL: LEMBALL 0x00493034
void *g_StreamFormatTargetStateVtable[1] = { (void *)NetworkSafeVtableNoop };
void *g_StreamFormatSubobjectVtable[1] = { (void *)NetworkSafeVtableNoop };
// GLOBAL: LEMBALL 0x00498968
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
void LEMBALL_FASTCALL ApplyStreamIntegerWidthPadding(VSINIT_FormattedOutputStream *pStream) {
    unsigned int dwFlags;
    int cchWidth;
    int cchText;
    int cchSourceSkip;
    int cchTargetSkip;
    int cchCopy;
    unsigned int fNegative;
    char *pszSource;
    char *pszTarget;

    if (VSINIT_FORMAT_TARGET(pStream)->m_nWidth == 0) {
        pStream->m_pszFormattedText = pStream->m_szFormatBuffer;
        return;
    }

    fNegative = pStream->m_szFormatBuffer[0] == '-';
    cchSourceSkip = pStream->m_szFormatBuffer[0] == '-';
    cchText = strlen(pStream->m_szFormatBuffer);
    cchWidth = VSINIT_FORMAT_TARGET(pStream)->m_nWidth;

    pszTarget = pStream->m_szFormatBuffer + 0x21;
    memset(pszTarget, VSINIT_FORMAT_TARGET(pStream)->m_chFill, cchWidth);

    pStream->m_szFormatBuffer[0x21 + cchWidth] = '\0';
    if (fNegative) {
        pStream->m_szFormatBuffer[0x21] = '-';
    }

    cchTargetSkip = fNegative;
    dwFlags = VSINIT_FORMAT_TARGET(pStream)->m_dwFlags;
    if ((dwFlags & 2) == 0) {
        if (cchText < cchWidth) {
            cchTargetSkip = cchWidth + fNegative - cchText;
        } else {
            cchSourceSkip = cchText - cchWidth;
        }
    }

    pszSource = pStream->m_szFormatBuffer + cchSourceSkip;
    pszTarget = pStream->m_szFormatBuffer + 0x21 + cchTargetSkip;
    cchCopy = cchWidth - fNegative;
    memcpy(pszTarget, pszSource, cchCopy);

    pStream->m_pszFormattedText = pStream->m_szFormatBuffer + 0x21;
}

// FUNCTION: LEMBALL 0x00458D80
void FormatSignedIntToRadixString(int nValue, char *pszBuffer, unsigned int nRadix) {
    if (nValue < 0) {
        *pszBuffer = '-';
        ++pszBuffer;
        nValue = -nValue;
    }
    FormatUnsignedIntToRadixString((unsigned int)nValue, pszBuffer, nRadix);
}

// FUNCTION: LEMBALL 0x00458DB0
char *FormatUnsignedIntToRadixString(unsigned int uValue, char *pszBuffer, unsigned int nRadix) {
    int fEmitting;
    int cchOutput;
    int cLoops;
    unsigned int nLimit;
    unsigned int nPower;
    unsigned int nDigit;
    unsigned int nMaxPower;

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
    cLoops = 0;
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
        if (nPower == 1) {
            break;
        }
        ++cLoops;
    } while (cLoops < 0x21);

    pszBuffer[cchOutput] = g_VSINIT_RadixDigits[uValue];
    pszBuffer[cchOutput + 1] = '\0';
    return pszBuffer;
}

// FUNCTION: LEMBALL 0x004585B0
VSINIT_FormattedOutputStream *VSINIT_FormattedOutputStream::AppendCStringToStream(const char *pszText) {
    AppendCStringToFixedBufferStream(VSINIT_FORMAT_TARGET(this)->m_pDownstream, pszText);
    return this;
}

VSINIT_FormattedOutputStream *AppendCStringToStream(VSINIT_FormattedOutputStream *pStream,
                                                     const char *pszText) {
    return pStream->AppendCStringToStream(pszText);
}

// FUNCTION: LEMBALL 0x00458630
VSINIT_FormattedOutputStream *VSINIT_FormattedOutputStream::AppendIntToStream(unsigned int uValue) {
    FormatSignedIntToRadixString((int)uValue, m_szFormatBuffer,
                                 (unsigned int)VSINIT_FORMAT_TARGET(this)->m_nRadix);
    ApplyStreamIntegerWidthPadding(this);
    return AppendCStringToStream(m_pszFormattedText);
}

// FUNCTION: LEMBALL 0x004585D0
VSINIT_FormattedOutputStream *VSINIT_FormattedOutputStream::AppendCharToStreamVariant(char ch) {
    AppendCharToFixedBufferStream(VSINIT_FORMAT_TARGET(this)->m_pDownstream, ch);
    return this;
}

// FUNCTION: LEMBALL 0x004585F0
VSINIT_FormattedOutputStream *VSINIT_FormattedOutputStream::AppendSignedIntToStreamVariant(int nValue) {
    FormatSignedIntToRadixString(nValue, m_szFormatBuffer,
                                 (unsigned int)VSINIT_FORMAT_TARGET(this)->m_nRadix);
    ApplyStreamIntegerWidthPadding(this);
    return AppendCStringToStream(m_pszFormattedText);
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
    pStream->AppendCharToStreamVariant(ch);
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
    unsigned int nRadix;

    dwFlags = VSINIT_FORMAT_TARGET(pStream)->m_dwFlags;
    VSINIT_FORMAT_TARGET(pStream)->m_dwFlags = (dwFlags & 0xffff7fcf) | 0x40;
    nRadix = (unsigned int)VSINIT_FORMAT_TARGET(pStream)->m_nRadix;
    VSINIT_FORMAT_TARGET(pStream)->m_nRadix = 0x10;
    AppendUIntToStream(pStream, uValue);
    VSINIT_FORMAT_TARGET(pStream)->m_nRadix = (int)nRadix;
    VSINIT_FORMAT_TARGET(pStream)->m_dwFlags = dwFlags;
    return pStream;
}

// FUNCTION: LEMBALL 0x004589C0
VSINIT_FormattedOutputStream *AppendHexUIntToStream(VSINIT_FormattedOutputStream *pStream, unsigned int uValue) {
    char chFill;
    unsigned int nWidth;
    unsigned int dwFlags;
    unsigned int nRadix;

    chFill = VSINIT_FORMAT_TARGET(pStream)->m_chFill;
    VSINIT_FORMAT_TARGET(pStream)->m_chFill = '0';
    nWidth = (unsigned int)VSINIT_FORMAT_TARGET(pStream)->m_nWidth;
    VSINIT_FORMAT_TARGET(pStream)->m_nWidth = 8;
    dwFlags = VSINIT_FORMAT_TARGET(pStream)->m_dwFlags;
    VSINIT_FORMAT_TARGET(pStream)->m_dwFlags = (dwFlags & 0xffff7fcf) | 0x40;
    nRadix = (unsigned int)VSINIT_FORMAT_TARGET(pStream)->m_nRadix;
    VSINIT_FORMAT_TARGET(pStream)->m_nRadix = 0x10;
    AppendUIntToStream(pStream, uValue);
    VSINIT_FORMAT_TARGET(pStream)->m_nWidth = (int)nWidth;
    VSINIT_FORMAT_TARGET(pStream)->m_nRadix = (int)nRadix;
    VSINIT_FORMAT_TARGET(pStream)->m_chFill = chFill;
    VSINIT_FORMAT_TARGET(pStream)->m_dwFlags = dwFlags;
    return pStream;
}

static const char g_VSINIT_SyncDebugName[] = "Sync_Debug";
static const char g_VSINIT_DebugOutPath[] = "debug.out";
static const char g_VSINIT_ErrorTitle[] = "ERROR";
static const char g_VSINIT_DebugStartError[] = "Unable to start 'Debug Message loop' thread";
static const char g_VSINIT_BaseWindowClass[] = "VS_Base_Window_Class";
static const char g_VSINIT_VersionPrefix[] = "ViSOS v";
static const char g_VSINIT_VersionMinorSeparator[] = ".";
static const char g_VSINIT_VersionBuildPrefix[] = "(";
static const char g_VSINIT_VersionSuffix[] = ")\n";
static const char g_VSINIT_CopyrightPrefix[] = "(c)";
static const char g_VSINIT_CopyrightYears[] = "1994,1995";
static const char g_VSINIT_CopyrightOwner[] = " Visual Sciences Ltd\n\n";
static const char g_VSINIT_SourceFileName[] = "VSINIT.CPP";
static const char g_VSINIT_EnoughMemoryAssert[] = "EnoughMemory";
static const char g_VSINIT_Success[] = "OK";
static const char g_VSINIT_Failed[] = "Failed";
static const char g_VSINIT_MemInitPrefix[] = "_MEM_Init   : ";
static const char g_VSINIT_StreamInitPrefix[] = "_STRM_Init  : ";
static const char g_VSINIT_DebugInitPrefix[] = "_DBG_Init   : ";
static const char g_VSINIT_InputInitPrefix[] = "_INP_Init   : ";
static const char g_VSINIT_TimeInitPrefix[] = "_TIME_Init  : ";
static const char g_VSINIT_GdiInitPrefix[] = "_GDI_Init   : ";
static const char g_VSINIT_StatusInitPrefix[] = "_STAT_Init  : ";
static const char g_VSINIT_ResInitPrefix[] = "_RES_Init   : ";
static const char g_VSINIT_BytesSuffix[] = ")\n";
static const char g_VSINIT_LineBreak[] = "\n";
static const char g_VSINIT_CommandLineOptionsHeader[] = "Command line options:\n";
static const char g_VSINIT_CommandLineOptionPrefix[] = "  Option ";
static const char g_VSINIT_CommandLineOptionIs[] = " is ";
static const char g_VSINIT_NotSelected[] = "not ";
static const char g_VSINIT_Selected[] = "selected\n";
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

struct VSINIT_StatusEntryPointerArray;

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

    VSINIT_VSMemPointerTable(int cItems);
    VSINIT_VSMemPointerTable *ConstructVSMemPointerTable(int cItems);
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
    VSINIT_SharedGeometryHelper *ConstructSharedGeometryHelper(void *pRenderDispatchQueue);
};
extern void *g_pSharedGeometryHelperVtable;
void LEMBALL_FASTCALL DestroySharedGeometryHelper(VSINIT_SharedGeometryHelper *pHelper);

struct VSINIT_SharedGeometryHelperVtableModel {
    // FUNCTION: LEMBALL 0x00472210
    virtual void *ReturnArgument(void *pArgument) {
        return pArgument;
    }

    // FUNCTION: LEMBALL 0x00472270
    virtual void *Delete(BYTE fDelete) {
        VSINIT_SharedGeometryHelper *pHelper = (VSINIT_SharedGeometryHelper *)this;

        DestroySharedGeometryHelper(pHelper);
        if ((fDelete & 1) != 0) {
            FreeVSMemBlock(pHelper);
        }
        return pHelper;
    }
};
static VSINIT_SharedGeometryHelperVtableModel
    g_SharedGeometryHelperVtableModel;
void *g_pSharedGeometryHelperVtable =
    *(void ***)&g_SharedGeometryHelperVtableModel;

// FUNCTION: LEMBALL 0x00472090
void LEMBALL_FASTCALL DestroySharedGeometryHelper(VSINIT_SharedGeometryHelper *pHelper) {
    unsigned int i;
    void *pNode;
    void *pNext;

    pHelper->m_pVtable = g_pSharedGeometryHelperVtable;
    pNode = *(void **)((char *)pHelper + 0x10);
    i = 0;
    while (i < (unsigned int)pHelper->m_nReserved08) {
        pNext = *(void **)((char *)pNode + 4);
        ++i;
        FreeVSMemBlock(pNode);
        pNode = pNext;
    }
}

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
// GLOBAL: LEMBALL 0x004a97b8
static VSINIT_FormattedOutputStream *g_pStartupOutputStream = 0;
// GLOBAL: LEMBALL 0x004a97bc
VSINIT_FormattedOutputStream *g_pStatusOutputStream = 0;
// GLOBAL: LEMBALL 0x004a93a8
VSINIT_FormattedOutputStream *g_pErrorOutputStream = 0;
static char *g_apszParsedArgs[16];
static unsigned int g_cParsedArgs = 0;
// GLOBAL: LEMBALL 0x004a0e68
static int g_fSubsystemsReady = 0;
static const char *g_pszDebugOutPath = g_VSINIT_DebugOutPath;
static FILE *g_pDebugOutFile = 0;
static void *g_pProcessCurrentDirectoryState = 0;
char g_abProcessCurrentDirectoryBuffer[0x100];
HANDLE g_hDebugSyncEvent = 0;
static HANDLE g_hDebugMessageThread = 0;
static DWORD g_dwDebugMessageThreadId = 0;
static PLATFORM_InvisibleMessageWindow g_InvisibleMessageWindow;
// GLOBAL: LEMBALL 0x004a0e88
static long g_cbMainArenaAvailableAfterInit = 0;
// GLOBAL: LEMBALL 0x004a0e94
static unsigned int g_uViSOSMajorVersion = 1;
// GLOBAL: LEMBALL 0x004a0e98
static unsigned int g_uViSOSMinorVersion = 0;
// GLOBAL: LEMBALL 0x00498958
static void *g_StatusEntryPointerArrayVtable[1] = { (void *)WriteStatusEntryPointerArray };
// GLOBAL: LEMBALL 0x004a1d64
static VSINIT_ResourceTypeTable *g_pPrimaryResourceTypeTable = 0;
// GLOBAL: LEMBALL 0x004a1d68
static VSINIT_ResourceTypeTable *g_pSecondaryResourceTypeTable = 0;
// GLOBAL: LEMBALL 0x004a1d6c
static VSINIT_ResourceTypeTable *g_pTertiaryResourceTypeTable = 0;
// GLOBAL: LEMBALL 0x004a2000
static VSINIT_VSMemPointerTable *g_pPaletteRemapPointerTable = 0;
static void *g_pSharedRenderQueueNode = 0;
static void *g_RenderDispatchQueueDeleteThunkVtable[1] = { (void *)NetworkSafeVtableNoop };
static void LEMBALL_FASTCALL EnterRenderDispatchQueueCriticalSection(
    void *pObject);
static void LEMBALL_FASTCALL LeaveRenderDispatchQueueCriticalSection(
    void *pObject);
static void *g_RenderDispatchQueueCriticalSectionHelperVtable[2] = {
    (void *)EnterRenderDispatchQueueCriticalSection,
    (void *)LeaveRenderDispatchQueueCriticalSection,
};
static void *g_RenderDispatchQueueVtable[4];
static void *g_DeleteRenderDispatchQueueCriticalSectionHelperThunk[2] = {
    (void *)EnterRenderDispatchQueueCriticalSection,
    (void *)LeaveRenderDispatchQueueCriticalSection,
};
struct VSINIT_KeyCodeTranslation {
    unsigned int m_dwVirtualKey;
    unsigned int m_dwInputCode;
};

static const VSINIT_KeyCodeTranslation g_aKeyCodeTranslations[61] = {
    { 0x20, 0x1f }, { 0xbe, 0x20 }, { 0xbc, 0x21 }, { 0x73, 0x25 },
    { 0x1b, 0x23 }, { 0x41, 0x05 }, { 0x42, 0x06 }, { 0x43, 0x07 },
    { 0x44, 0x08 }, { 0x45, 0x09 }, { 0x46, 0x0a }, { 0x47, 0x0b },
    { 0x48, 0x0c }, { 0x49, 0x0d }, { 0x4a, 0x0e }, { 0x4b, 0x0f },
    { 0x4c, 0x10 }, { 0x4d, 0x11 }, { 0x4e, 0x12 }, { 0x4f, 0x13 },
    { 0x50, 0x14 }, { 0x51, 0x15 }, { 0x52, 0x16 }, { 0x53, 0x17 },
    { 0x54, 0x18 }, { 0x55, 0x19 }, { 0x56, 0x1a }, { 0x57, 0x1b },
    { 0x58, 0x1c }, { 0x59, 0x1d }, { 0x5a, 0x1e }, { 0x30, 0x39 },
    { 0x31, 0x3a }, { 0x32, 0x3b }, { 0x33, 0x3c }, { 0x34, 0x3d },
    { 0x35, 0x3e }, { 0x36, 0x3f }, { 0x37, 0x40 }, { 0x38, 0x41 },
    { 0x39, 0x42 }, { 0x60, 0x39 }, { 0x61, 0x3a }, { 0x62, 0x3b },
    { 0x63, 0x3c }, { 0x64, 0x3d }, { 0x65, 0x3e }, { 0x66, 0x3f },
    { 0x67, 0x40 }, { 0x68, 0x41 }, { 0x69, 0x42 }, { 0x26, 0x01 },
    { 0x28, 0x02 }, { 0x25, 0x03 }, { 0x27, 0x04 }, { 0x0d, 0x4c },
    { 0x2e, 0x4d }, { 0x2e, 0x4d }, { 0x08, 0x4e }, { 0x10, 0x49 },
    { 0xa0, 0x4a },
};

static int LEMBALL_FASTCALL TranslateKeyboardRenderQueueEntry(
    void *pRenderQueueNode, int nUnused, RDISPATCH_QueueEntry *pEntry);
static void *g_SharedRenderQueueNodeVtable[4] = {
    (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop,
    (void *)TranslateKeyboardRenderQueueEntry,
    0,
};
// GLOBAL: LEMBALL 0x004988c8
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

// FUNCTION: LEMBALL 0x00472A60
static int LEMBALL_FASTCALL TranslateKeyboardRenderQueueEntry(void *pRenderQueueNode, int, RDISPATCH_QueueEntry *pEntry) {
    RDISPATCH_QueueEntry TranslatedEntry;
    unsigned short wEntryType;
    unsigned int i;

    wEntryType = *(unsigned short *)&pEntry->m_awords[0];
    TranslatedEntry.m_awords[1] = pEntry->m_awords[1];
    if (wEntryType < 1 || 2 < wEntryType) {
        ++((unsigned int *)pRenderQueueNode)[3];
        return 0;
    }

    for (i = 0; i < sizeof(g_aKeyCodeTranslations) / sizeof(g_aKeyCodeTranslations[0]); ++i) {
        if (g_aKeyCodeTranslations[i].m_dwVirtualKey == pEntry->m_awords[2]) {
            break;
        }
    }
    if (i == sizeof(g_aKeyCodeTranslations) / sizeof(g_aKeyCodeTranslations[0])) {
        ++((unsigned int *)pRenderQueueNode)[3];
        return 0;
    }

    *(unsigned short *)&TranslatedEntry.m_awords[0] = wEntryType == 1 ? 3 : 4;
    TranslatedEntry.m_awords[2] = g_aKeyCodeTranslations[i].m_dwInputCode;
    if (TranslatedEntry.m_awords[2] == 0x49 && GetKeyState(VK_LSHIFT) < 0) {
        TranslatedEntry.m_awords[2] = 0x4a;
    }

    ((int (LEMBALL_FASTCALL *)(void *, int, RDISPATCH_QueueEntry *))
         (*(void ***)g_pSharedRenderDispatchQueue)[2])(
        g_pSharedRenderDispatchQueue, 0, &TranslatedEntry);
    return 1;
}

void EnterObjectCriticalSection(void *pObject) {
    EnterCriticalSection((char *)pObject + 4);
}

void LeaveObjectCriticalSection(void *pObject) {
    LeaveCriticalSection((char *)pObject + 4);
}

extern int __stdcall DebugMessageThreadMain(void);

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
void *VSINIT_FixedBufferStream::ConstructStreamBaseVtable(char *, unsigned int) {
    m_pVtable = g_StreamBaseVtable;
    return this;
}

// FUNCTION: LEMBALL 0x00458E60
void *LEMBALL_FASTCALL DeleteStreamBase(
    void *pStream, int nUnused, int fFreeMemory) {
    (void)nUnused;
    DestroyStreamBaseVtable(pStream);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pStream);
    }
    return pStream;
}

// FUNCTION: LEMBALL 0x00458400
void LEMBALL_FASTCALL DestroyStreamBaseVtable(void *pStream) {
    *(void **)pStream = g_StreamBaseVtable;
}

// FUNCTION: LEMBALL 0x00458410
VSINIT_StreamFormatTargetState *VSINIT_StreamFormatTargetState::ConstructStreamFormatTargetState(VSINIT_FixedBufferStream *pDownstream) {
    m_pVtable = g_StreamFormatTargetStateVtable;
    m_chFill = ' ';
    m_dwFlags = 0x14;
    m_nWidth = 0;
    m_nRadix = 10;
    m_pDownstream = pDownstream;
    return this;
}

static void LEMBALL_FASTCALL EnterRenderDispatchQueueCriticalSection(
    void *pObject) {
    EnterObjectCriticalSection(pObject);
}

static void LEMBALL_FASTCALL LeaveRenderDispatchQueueCriticalSection(
    void *pObject) {
    LeaveObjectCriticalSection(pObject);
}

VSINIT_StreamFormatTargetState *ConstructStreamFormatTargetState(VSINIT_StreamFormatTargetState *pState,
                                                                 VSINIT_FixedBufferStream *pDownstream) {
    return pState->ConstructStreamFormatTargetState(pDownstream);
}

// FUNCTION: LEMBALL 0x00458440
void LEMBALL_FASTCALL ConstructStreamFormatState(VSINIT_StreamFormatTargetState *pState) {
    pState->m_pVtable = g_StreamFormatTargetStateVtable;
}

// FUNCTION: LEMBALL 0x00458450
VSINIT_FormattedOutputStream *VSINIT_FormattedOutputStream::ConstructFormattedOutputStream(VSINIT_FixedBufferStream *pDownstream, int fConstructTargetState) {
    if (fConstructTargetState != 0) {
        m_pVtable = (void **)g_FormattedOutputStreamVtable;
        m_TargetState.ConstructStreamFormatTargetState(pDownstream);
    }
    *(void **)((char *)this + (unsigned long)((void **)m_pVtable)[1]) = g_StreamFormatSubobjectVtable;
    return this;
}

VSINIT_FormattedOutputStream *ConstructFormattedOutputStream(VSINIT_FormattedOutputStream *pStream,
                                                             VSINIT_FixedBufferStream *pDownstream,
                                                             int fConstructTargetState) {
    return pStream->ConstructFormattedOutputStream(pDownstream, fConstructTargetState);
}

// FUNCTION: LEMBALL 0x004584A0
void LEMBALL_FASTCALL RestoreStreamFormatSubobjectVtable(VSINIT_StreamFormatTargetState *pState) {
    VSINIT_FormattedOutputStream *pStream;
    void **pVtable;

    pStream = (VSINIT_FormattedOutputStream *)((char *)pState - 0x12c);
    pVtable = *(void ***)pStream;
    *(void **)((char *)pStream + (unsigned long)pVtable[1]) = g_StreamFormatSubobjectVtable;
}

// FUNCTION: LEMBALL 0x0045AD70
VSINIT_FixedBufferStream *VSINIT_FixedBufferStream::ConstructFixedBufferStream(char *pszBuffer, unsigned int cbBuffer, void (*pfnFlush)(char *)) {
    ConstructStreamBaseVtable(pszBuffer, cbBuffer);
    m_pVtable = g_FixedBufferStreamVtable;
    m_pfnFlush = pfnFlush;
    m_pszBuffer = pszBuffer;
    m_pszCursor = pszBuffer;
    m_cchWritten = 0;
    m_cbBuffer = cbBuffer;
    memset(pszBuffer, 0, cbBuffer);
    return this;
}

VSINIT_FixedBufferStream *ConstructFixedBufferStream(VSINIT_FixedBufferStream *pStream,
                                                     char *pszBuffer,
                                                     unsigned int cbBuffer,
                                                     void (*pfnFlush)(char *)) {
    return pStream->ConstructFixedBufferStream(pszBuffer, cbBuffer, pfnFlush);
}

// FUNCTION: LEMBALL 0x0045ADD0
void __fastcall ResetFixedBufferStream(VSINIT_FixedBufferStream *pStream) {
    if (pStream->m_pfnFlush != 0) {
        pStream->m_pfnFlush(pStream->m_pszBuffer);
    }
    pStream->m_pszCursor = pStream->m_pszBuffer;
    pStream->m_cchWritten = 0;
    pStream->m_nTabWidth = 8;
    memset(pStream->m_pszBuffer, 0, pStream->m_cbBuffer);
}

// FUNCTION: LEMBALL 0x0045ADC0
void LEMBALL_FASTCALL DestroyFixedBufferStream(VSINIT_FixedBufferStream *pStream) {
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

// FUNCTION: LEMBALL 0x0045AF60
void *LEMBALL_FASTCALL DeleteFixedBufferStreamReturnThis(
    VSINIT_FixedBufferStream *pStream, int nUnused, int fFreeMemory) {
    (void)nUnused;
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

// FUNCTION: LEMBALL 0x0045AE10
void AppendCharToFixedBufferStream(VSINIT_FixedBufferStream *pStream, char ch) {
    int cSpaces;
    int fTab;

    fTab = ch == '\t';
    if (fTab) {
        ch = ' ';
    }
    if (ch == '\n' && pStream->m_pfnFlush != 0) {
        *pStream->m_pszCursor++ = ch;
        *pStream->m_pszCursor = '\0';
        ++pStream->m_cchWritten;
        if (pStream->m_cbBuffer - pStream->m_cchWritten == 1) {
            ResetFixedBufferStream(pStream);
        }
        ResetFixedBufferStream(pStream);
        return;
    }

    *pStream->m_pszCursor++ = ch;
    *pStream->m_pszCursor = '\0';
    ++pStream->m_cchWritten;
    if (pStream->m_cbBuffer - pStream->m_cchWritten == 1) {
        ResetFixedBufferStream(pStream);
    }

    if (fTab && pStream->m_nTabWidth > 0) {
        cSpaces = pStream->m_cchWritten % pStream->m_nTabWidth;
        while (cSpaces != 0) {
            *pStream->m_pszCursor++ = ' ';
            *pStream->m_pszCursor = '\0';
            ++pStream->m_cchWritten;
            if (pStream->m_cbBuffer - pStream->m_cchWritten == 1) {
                ResetFixedBufferStream(pStream);
            }
            cSpaces = pStream->m_cchWritten % pStream->m_nTabWidth;
        }
    }
}

// FUNCTION: LEMBALL 0x0045AF20
void AppendCStringToFixedBufferStream(VSINIT_FixedBufferStream *pStream,
                                      const char *pszText) {
    while (*pszText != '\0') {
        AppendCharToFixedBufferStream(pStream, *pszText++);
    }
}

// FUNCTION: LEMBALL 0x00463280
void *LEMBALL_FASTCALL ReturnStreamArgument(
    void *pStream, int, void *pArgument) {
    (void)pStream;
    return pArgument;
}

void AppendStartupCString(const char *pszText) {
    AppendCStringToStream(g_pStartupOutputStream, pszText);
}

void AppendStartupUInt(unsigned int uValue) {
    g_pStartupOutputStream->AppendIntToStream(uValue);
}

void AppendStatusCString(const char *pszText) {
    AppendCStringToStream(g_pStatusOutputStream, pszText);
}

void AppendErrorCString(const char *pszText) {
    AppendCStringToStream(g_pErrorOutputStream, pszText);
}

void AppendErrorUInt(unsigned int uValue) {
    g_pErrorOutputStream->AppendIntToStream(uValue);
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

    pszResult = "OK";
    if (fSuccess == 0) {
        pszResult = "Failed";
    }
    return pszResult;
}

// FUNCTION: LEMBALL 0x00458F30
int ParseDecimalIntAndAdvance(char *pszText, char **ppszEnd, int nRadix) {
    char chDigit;
    int nValue;

    if (nRadix == 10) {
        nValue = 0;
        while (*pszText >= '0') {
            chDigit = *pszText;
            if (chDigit > '9') {
                break;
            }
            nValue = chDigit - '0' + nValue * 10;
            ++pszText;
        }
        *ppszEnd = pszText;
        return nValue;
    }

    *ppszEnd = pszText;
    return 0;
}

// FUNCTION: LEMBALL 0x00472BE0
int InitializeDebugMessageThread(void) {
    (void)g_VSINIT_BaseWindowClass;

    if (g_fDebugMessageThreadRunning == 1) {
        g_hDebugSyncEvent = CreateEventA(0, 0, 0, g_VSINIT_SyncDebugName);
        g_hDebugMessageThread =
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)DebugMessageThreadMain, 0, 0, &g_dwDebugMessageThreadId);
        if (g_hDebugMessageThread == 0) {
            MessageBoxA(0, g_VSINIT_DebugStartError, g_VSINIT_ErrorTitle, 0);
            ExitProcess(0xbbbb);
        }

        SetThreadPriority(g_hDebugMessageThread, 1);
        WaitForSingleObject(g_hDebugSyncEvent, INFINITE);
    }

    return 1;
}

// FUNCTION: LEMBALL 0x00472C70
int ShutdownDebugMessageThread(int fForceTerminate) {
    if (g_fDebugMessageThreadRunning == 1) {
        if (fForceTerminate == 0) {
            WaitForSingleObject(g_hDebugSyncEvent, INFINITE);
        } else {
            TerminateThread(g_hDebugMessageThread, 0xaaaa);
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
void *LEMBALL_FASTCALL ConstructProcessCurrentDirectoryMarker(void *pMarker) {
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
            ((VSINIT_FixedBufferStream *)pStream)->ConstructFixedBufferStream(
                g_abStreamFixedBuffer, 0x400, FlushStatusFixedBufferStream);
    }

    pStream = AllocateVSMemBlock(0x1c);
    if (pStream == 0) {
        g_pStartupFixedBufferStream = 0;
    } else {
        g_pStartupFixedBufferStream =
            ((VSINIT_FixedBufferStream *)pStream)->ConstructFixedBufferStream(
                g_abStreamFixedBuffer, 0x400, FlushStartupFixedBufferStream);
    }

    pStream = AllocateVSMemBlock(0x1c);
    if (pStream == 0) {
        g_pErrorFixedBufferStream = 0;
    } else {
        g_pErrorFixedBufferStream =
            ((VSINIT_FixedBufferStream *)pStream)->ConstructFixedBufferStream(
                g_abStreamFixedBuffer, 0x400, FlushErrorFixedBufferStream);
    }

    pStream = AllocateVSMemBlock(0x14c);
    if (pStream == 0) {
        g_pStatusOutputStream = 0;
    } else {
        g_pStatusOutputStream =
            ((VSINIT_FormattedOutputStream *)pStream)->ConstructFormattedOutputStream(g_pStatusFixedBufferStream, 1);
    }

    pStream = AllocateVSMemBlock(0x14c);
    if (pStream == 0) {
        g_pStartupOutputStream = 0;
    } else {
        g_pStartupOutputStream =
            ((VSINIT_FormattedOutputStream *)pStream)->ConstructFormattedOutputStream(g_pStartupFixedBufferStream, 1);
    }

    pStream = AllocateVSMemBlock(0x14c);
    if (pStream == 0) {
        g_pErrorOutputStream = 0;
    } else {
        g_pErrorOutputStream =
            ((VSINIT_FormattedOutputStream *)pStream)->ConstructFormattedOutputStream(g_pErrorFixedBufferStream, 1);
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
    cchDirectory = strlen(g_abProcessCurrentDirectoryBuffer) + 1;
    if (g_abProcessCurrentDirectoryBuffer[cchDirectory - 2] == '\\') {
        g_abProcessCurrentDirectoryBuffer[cchDirectory - 2] = '\0';
    }

    return 1;
}

extern int DrainRenderDispatchQueueEntries(void *pDispatchQueue, unsigned int cEntries);

// FUNCTION: LEMBALL 0x00463020
void *ConstructRenderDispatchQueue(void *pQueue, int cEntries) {
    RDISPATCH_Queue *pDispatchQueue;
    RDISPATCH_QueueEntry *pEntryBuffer;
    int nEntryBuffer;

    pDispatchQueue = (RDISPATCH_Queue *)pQueue;
    pDispatchQueue->m_pVtable = g_RenderDispatchQueueDeleteThunkVtable;
    pDispatchQueue->m_pLockVtable = g_RenderDispatchQueueCriticalSectionHelperVtable;
    InitializeCriticalSection((char *)pQueue + 0xc);
    pDispatchQueue->m_pVtable = g_RenderDispatchQueueVtable;
    pDispatchQueue->m_pLockVtable = g_DeleteRenderDispatchQueueCriticalSectionHelperThunk;

    nEntryBuffer = (int)(unsigned long)AllocateVSMemBlock((unsigned int)(cEntries * 0x14));
    pEntryBuffer = (RDISPATCH_QueueEntry *)(unsigned long)nEntryBuffer;
    pDispatchQueue->m_pEntryBuffer = pEntryBuffer;
    pDispatchQueue->m_cEntryCapacity = cEntries;
    pDispatchQueue->m_pEntryBufferEnd = (RDISPATCH_QueueEntry *)(unsigned long)(cEntries * 0x14 + nEntryBuffer);
    pDispatchQueue->m_pTail = pEntryBuffer;
    pDispatchQueue->m_pHead = pEntryBuffer;
    pDispatchQueue->m_cQueuedEntries = 0;
    pDispatchQueue->m_cClients = 0;
    pDispatchQueue->m_nReserved34 = 0;
    pDispatchQueue->m_nReserved38 = 0;
    pDispatchQueue->m_nReserved3C = 0;
    pDispatchQueue->m_cEntriesDropped = 0;
    pDispatchQueue->m_nReserved30 = 0;
    pDispatchQueue->m_pClientList = 0;
    return pQueue;
}

// FUNCTION: LEMBALL 0x004630A0
RDISPATCH_Queue *RDISPATCH_Queue::ConstructRenderDispatchQueueVariant(
    int cEntries, const char *pszName) {
    RDISPATCH_QueueEntry *pEntryBuffer;
    int nEntryBuffer;

    (void)pszName;
    m_pVtable = g_RenderDispatchQueueDeleteThunkVtable;
    m_pLockVtable = g_RenderDispatchQueueCriticalSectionHelperVtable;
    InitializeCriticalSection((char *)this + 0xc);
    m_pVtable = g_RenderDispatchQueueVtable;
    m_pLockVtable = g_DeleteRenderDispatchQueueCriticalSectionHelperThunk;

    nEntryBuffer = (int)(unsigned long)AllocateVSMemBlock((unsigned int)(cEntries * 0x14));
    pEntryBuffer = (RDISPATCH_QueueEntry *)(unsigned long)nEntryBuffer;
    m_pEntryBuffer = pEntryBuffer;
    m_cEntryCapacity = cEntries;
    m_pEntryBufferEnd = (RDISPATCH_QueueEntry *)(unsigned long)(cEntries * 0x14 + nEntryBuffer);
    m_pTail = pEntryBuffer;
    m_pHead = pEntryBuffer;
    m_cQueuedEntries = 0;
    m_cClients = 0;
    m_nReserved34 = 0;
    m_nReserved38 = 0;
    m_nReserved3C = 0;
    m_cEntriesDropped = 0;
    m_nReserved30 = 0;
    m_pClientList = 0;
    return this;
}

// FUNCTION: LEMBALL 0x00463120
void DestroyRenderDispatchQueue(void *pQueue) {
    RDISPATCH_Queue *pDispatchQueue;
    unsigned int cClientsReleased;
    unsigned int cClients;
    RDISPATCH_ClientNode *pClientNode;
    RDISPATCH_ClientNode *pNextClientNode;
    void **ppLockThunk;

    pDispatchQueue = (RDISPATCH_Queue *)pQueue;
    pDispatchQueue->m_pVtable = g_RenderDispatchQueueVtable;
    pDispatchQueue->m_pLockVtable = g_DeleteRenderDispatchQueueCriticalSectionHelperThunk;

    pClientNode = pDispatchQueue->m_pClientList;
    if (DrainRenderDispatchQueueEntries(pQueue, pDispatchQueue->m_cQueuedEntries) == 1) {
        FreeVSMemBlock(pDispatchQueue->m_pEntryBuffer);
    }

    cClientsReleased = 0;
    cClients = (unsigned int)pDispatchQueue->m_cClients;
    if (cClients != 0) {
        do {
            pNextClientNode = pClientNode->m_pNext;
            FreeVSMemBlock(pClientNode);
            ++cClientsReleased;
            pClientNode = pNextClientNode;
        } while (cClientsReleased < cClients);
    }

    ppLockThunk = &pDispatchQueue->m_pLockVtable;
    *ppLockThunk = g_RenderDispatchQueueCriticalSectionHelperVtable;
    DeleteCriticalSection((char *)ppLockThunk + 4);
}

// FUNCTION: LEMBALL 0x004599B0
void *DeleteRenderDispatchQueue(void *pQueue, unsigned char fFreeMemory) {
    DestroyRenderDispatchQueue(pQueue);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pQueue);
    }
    return pQueue;
}

// FUNCTION: LEMBALL 0x00463940
static void *LEMBALL_FASTCALL DeleteRenderDispatchQueueThunk00463940(
    void *pQueue, int, int fFreeMemory) {
    DestroyRenderDispatchQueue(pQueue);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pQueue);
    }
    return pQueue;
}

struct RDISPATCH_FinalVtableInitializer {
    RDISPATCH_FinalVtableInitializer(void) {
        g_RenderDispatchQueueVtable[0] = (void *)ReturnStreamArgument;
        g_RenderDispatchQueueVtable[1] =
            (void *)DeleteRenderDispatchQueueThunk00463940;
        g_RenderDispatchQueueVtable[2] =
            (void *)AppendRenderDispatchQueueEntry;
        g_RenderDispatchQueueVtable[3] =
            (void *)DispatchRenderQueueEntryImmediately;
    }
};
static RDISPATCH_FinalVtableInitializer g_RDISPATCH_FinalVtableInitializer;

// FUNCTION: LEMBALL 0x00472070
VSINIT_SharedGeometryHelper *VSINIT_SharedGeometryHelper::ConstructSharedGeometryHelper(void *pRenderDispatchQueue) {
    m_pVtable = g_pSharedGeometryHelperVtable;
    m_nReserved08 = 0;
    m_nReserved10 = 0;
    m_pRenderDispatchQueue = pRenderDispatchQueue;
    m_dwFlags = 0;
    return this;
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
        g_pSharedRenderDispatchQueue =
            ((RDISPATCH_Queue *)pQueue)->ConstructRenderDispatchQueueVariant(
                10, "Master Input Queue.");
    }

    pHelper = AllocateVSMemBlock(0x18);
    if (pHelper == 0) {
        g_pSharedGeometryHelper = 0;
    } else {
        g_pSharedGeometryHelper =
            ((VSINIT_SharedGeometryHelper *)pHelper)
                ->ConstructSharedGeometryHelper(g_pSharedRenderDispatchQueue);
    }

    pRenderQueueNode = AllocateVSMemBlock(0x10);
    if (pRenderQueueNode == 0) {
        g_pSharedRenderQueueNode = 0;
    } else {
        InitializeRenderQueueNodeBase(pRenderQueueNode);
        *(void **)pRenderQueueNode = g_SharedRenderQueueNodeVtable;
        g_pSharedRenderQueueNode = pRenderQueueNode;
    }

    ((GAME_RenderDispatchQueue *)g_pSharedRenderDispatchQueue)
        ->RegisterOrderedRenderDispatchClient(g_pSharedRenderQueueNode, -0x32);
    return SetSharedGeometryHelperFlags03();
}

// FUNCTION: LEMBALL 0x00462E60
int InitializeTimingSubsystemStub(void) {
    return 1;
}

// FUNCTION: LEMBALL 0x0045AAF0
VSINIT_StatusEntryPointerArray *VSINIT_StatusEntryPointerArray::ConstructStatusEntryPointerArray(int cEntriesMax) {
    VSINIT_StatusEntryPointerArray *pPointerArray;

    pPointerArray = this;
    pPointerArray->m_pVtable = g_StatusEntryPointerArrayVtable;
    pPointerArray->m_ppEntries = (void **)AllocateVSMemBlock(cEntriesMax * sizeof(void *));
    pPointerArray->m_cEntriesMax = cEntriesMax;
    pPointerArray->m_cEntries = 0;
    return pPointerArray;
}

VSINIT_StatusEntryPointerArray *ConstructStatusEntryPointerArray(void *pRegistry, int cEntriesMax) {
    return ((VSINIT_StatusEntryPointerArray *)pRegistry)->ConstructStatusEntryPointerArray(cEntriesMax);
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
void VSINIT_StatusEntryPointerArray::AppendStatusEntry(void *pEntry) {
    if (m_cEntries < m_cEntriesMax) {
        m_ppEntries[m_cEntries] = pEntry;
        ++m_cEntries;
    }
}

// FUNCTION: LEMBALL 0x0045AA80
int InitializeStatusEntryRegistry(void) {
    void *pRegistry;

    pRegistry = AllocateVSMemBlock(0x14);
    if (pRegistry != 0) {
        pRegistry =
            ((VSINIT_StatusEntryPointerArray *)pRegistry)->ConstructStatusEntryPointerArray(0x20);
    } else {
        pRegistry = 0;
    }
    g_pStatusEntryRegistry = pRegistry;

    return g_pStatusEntryRegistry != 0;
}

// FUNCTION: LEMBALL 0x0046ACD0
VSINIT_VSMemPointerTable *VSINIT_VSMemPointerTable::ConstructVSMemPointerTable(int cItems) {
    VSINIT_VSMemPointerTable *pPointerTable;
    int i;

    pPointerTable = this;
    pPointerTable->m_cItems = cItems;
    pPointerTable->m_nCursor = 0;
    pPointerTable->m_ppItems = (void **)AllocateVSMemBlock((unsigned int)(cItems << 2));
    for (i = 0; i < pPointerTable->m_cItems; ++i) {
        pPointerTable->m_ppItems[i] = 0;
    }
    return pPointerTable;
}

// FUNCTION: LEMBALL 0x00473630
VSINIT_VSMemPointerTable::VSINIT_VSMemPointerTable(int cItems) {
    ConstructVSMemPointerTable(cItems);
}

// FUNCTION: LEMBALL 0x0046AAD0
void LEMBALL_FASTCALL ReleasePaletteRemapVariantFields(VSINIT_PaletteRemapVariant *pVariant) {
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

    pResourceTypeTable = (VSINIT_ResourceTypeTable *)AllocateVSMemBlock(0x10);
    if (pResourceTypeTable != 0) {
        pResourceTypeTable->m_cTagsMax = 2;
        pResourceTypeTable->m_nCursor = -1;
        pResourceTypeTable->m_cTags = 0;
        pResourceTypeTable->m_pTags = (u32 *)AllocateVSMemBlock((unsigned int)(pResourceTypeTable->m_cTagsMax << 2));
    } else {
        pResourceTypeTable = 0;
    }
    pResourceTypeTable->m_pTags[pResourceTypeTable->m_cTags] = 0x494e5420;
    ++pResourceTypeTable->m_cTags;
    pResourceTypeTable->m_pTags[pResourceTypeTable->m_cTags] = 0x5a524c45;
    ++pResourceTypeTable->m_cTags;
    g_pSecondaryResourceTypeTable = pResourceTypeTable;

    pResourceTypeTable = (VSINIT_ResourceTypeTable *)AllocateVSMemBlock(0x10);
    if (pResourceTypeTable != 0) {
        pResourceTypeTable->m_cTagsMax = 1;
        pResourceTypeTable->m_nCursor = -1;
        pResourceTypeTable->m_cTags = 0;
        pResourceTypeTable->m_pTags = (u32 *)AllocateVSMemBlock((unsigned int)(pResourceTypeTable->m_cTagsMax << 2));
    } else {
        pResourceTypeTable = 0;
    }
    pResourceTypeTable->m_pTags[pResourceTypeTable->m_cTags] = 0x5a524c45;
    ++pResourceTypeTable->m_cTags;
    g_pPrimaryResourceTypeTable = pResourceTypeTable;

    pResourceTypeTable = (VSINIT_ResourceTypeTable *)AllocateVSMemBlock(0x10);
    if (pResourceTypeTable != 0) {
        pResourceTypeTable->m_cTagsMax = 2;
        pResourceTypeTable->m_nCursor = -1;
        pResourceTypeTable->m_cTags = 0;
        pResourceTypeTable->m_pTags = (u32 *)AllocateVSMemBlock((unsigned int)(pResourceTypeTable->m_cTagsMax << 2));
    } else {
        pResourceTypeTable = 0;
    }
    pResourceTypeTable->m_pTags[pResourceTypeTable->m_cTags] = 0x53545247;
    ++pResourceTypeTable->m_cTags;
    pResourceTypeTable->m_pTags[pResourceTypeTable->m_cTags] = 0x494e5420;
    ++pResourceTypeTable->m_cTags;
    g_pTertiaryResourceTypeTable = pResourceTypeTable;

    g_pPaletteRemapPointerTable =
        new (AllocateVSMemBlock(sizeof(VSINIT_VSMemPointerTable)))
            VSINIT_VSMemPointerTable(0x20);
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
    ((GAME_RenderDispatchQueue *)g_pSharedRenderDispatchQueue)
        ->UnregisterOrderedRenderDispatchClient(g_pSharedRenderQueueNode, -0x32);
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
        ((VSINIT_DeleteProc)**(void ***)g_pErrorFixedBufferStream)(
            g_pErrorFixedBufferStream, 0, 1);
    }
    if (g_pStartupFixedBufferStream != 0) {
        ((VSINIT_DeleteProc)**(void ***)g_pStartupFixedBufferStream)(
            g_pStartupFixedBufferStream, 0, 1);
    }
    if (g_pStatusFixedBufferStream != 0) {
        ((VSINIT_DeleteProc)**(void ***)g_pStatusFixedBufferStream)(
            g_pStatusFixedBufferStream, 0, 1);
    }

    return 1;
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
    void *pMainMemoryArenaStatusStorage;

    fMemoryInitialized = InitializeMasterMainRamArena();
    if (fMemoryInitialized == 0) {
        TriggerReleaseAssertFailure("EnoughMemory", "VSINIT.CPP", 0x19e);
    }

    fStreamsInitialized = InitializeStreamChannels();
    fDebugInitialized = InitializeDebugMessageThread();
    g_fSubsystemsReady = fDebugInitialized;
    InitializeProcessCurrentDirectoryState();

    g_pStartupOutputStream->AppendCStringToStream("ViSOS v")
        ->AppendIntToStream(g_uViSOSMajorVersion)
        ->AppendCStringToStream(".")
        ->AppendIntToStream(g_uViSOSMinorVersion)
        ->AppendCStringToStream("(")
        ->AppendIntToStream(201)
        ->AppendCStringToStream(")")
        ->AppendCStringToStream("\n")
        ;

    g_pStartupOutputStream->AppendCStringToStream("(c)")
        ->AppendCStringToStream("1994,1995")
        ->AppendCStringToStream(" Visual Sciences Ltd\n\n");

    g_pStartupOutputStream->AppendCStringToStream("_MEM_Init   : ")
        ->AppendCStringToStream(SelectSuccessOrFailedString(fMemoryInitialized))
        ->AppendCStringToStream("...\t(")
        ->AppendIntToStream(g_StartupGraphicsDriverConfig.m_dwStyle)
        ->AppendCStringToStream(")\n");

    g_pStartupOutputStream->AppendCStringToStream("_STRM_Init  : ")
        ->AppendCStringToStream(SelectSuccessOrFailedString(fStreamsInitialized))
        ->AppendCStringToStream("...\n");

    g_pStartupOutputStream->AppendCStringToStream("_DBG_Init   : ")
        ->AppendCStringToStream(SelectSuccessOrFailedString(fDebugInitialized))
        ->AppendCStringToStream("...\n");

    g_cbMainArenaAvailableAfterInit = CalculateMemoryArenaAvailableBytes(g_pMainMemoryArena);

    fInputInitialized = InitializeSharedEventQueueRuntime();
    g_pStartupOutputStream->AppendCStringToStream("_INP_Init   : ")
        ->AppendCStringToStream(SelectSuccessOrFailedString(fInputInitialized))
        ->AppendCStringToStream("...\n");

    fTimeInitialized = InitializeTimingSubsystemStub();
    g_pStartupOutputStream->AppendCStringToStream("_TIME_Init  : ")
        ->AppendCStringToStream(SelectSuccessOrFailedString(fTimeInitialized))
        ->AppendCStringToStream("...\n");

    fGdiInitialized = InitializeResourceGeometryHelperRuntime();
    g_pStartupOutputStream->AppendCStringToStream("_GDI_Init   : ")
        ->AppendCStringToStream(SelectSuccessOrFailedString(fGdiInitialized))
        ->AppendCStringToStream("...\t(")
        ->AppendIntToStream(g_StartupGraphicsDriverConfig.m_cbSize)
        ->AppendCStringToStream(")\n");

    fStatusInitialized = InitializeStatusEntryRegistry();
    g_pStartupOutputStream->AppendCStringToStream("_STAT_Init  : ")
        ->AppendCStringToStream(SelectSuccessOrFailedString(fStatusInitialized))
        ->AppendCStringToStream("...\n");

    fResourcesInitialized = InitializeResourceTypeTables();
    g_pStartupOutputStream->AppendCStringToStream("_RES_Init   : ")
        ->AppendCStringToStream(SelectSuccessOrFailedString(fResourcesInitialized))
        ->AppendCStringToStream("...\n");

    pMainMemoryArenaStatusStorage = AllocateVSMemBlock(sizeof(GAME_StatusEntry));
    if (pMainMemoryArenaStatusStorage != 0) {
        pMainMemoryArenaStatusEntry =
            (GAME_StatusEntry *)pMainMemoryArenaStatusStorage;
        new (pMainMemoryArenaStatusStorage)
            GAME_StatusEntry("Main memory arena");
        pMainMemoryArenaStatusEntry->m_pVtable = g_MainMemoryArenaStatusEntryVtable;
    } else {
        pMainMemoryArenaStatusEntry = 0;
    }
    ((VSINIT_StatusEntryPointerArray *)g_pStatusEntryRegistry)
        ->AppendStatusEntry(pMainMemoryArenaStatusEntry);
    ((CArena *)g_pMainMemoryArena)->m_pStatusEntry = pMainMemoryArenaStatusEntry;
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

// FUNCTION: LEMBALL 0x00480010
int CompareCommandLineOptionPrefix(const char *pszLeft, const char *pszRight, size_t cchMax) {
    while (cchMax != 0 && *pszLeft == *pszRight) {
        ++pszLeft;
        ++pszRight;
        --cchMax;
    }
    if (cchMax == 0) {
        return 0;
    }
    return (unsigned char)*pszLeft < (unsigned char)*pszRight ? -1 : 1;
}

// FUNCTION: LEMBALL 0x004595D0
int ParseCommandLineOptionToken(char *pszToken) {
    int i;

    if (*pszToken == '-' || *pszToken == '/') {
        ++pszToken;
        for (i = 0; i < (int)LEMBALL_ARRAY_COUNT(g_aCommandLineOptions); ++i) {
            const char *pszColon;
            size_t cchOption;

            pszColon = FindCharInCString0047FE00((char *)g_aCommandLineOptions[i].m_pszName, ':');
            if (pszColon == 0) {
                cchOption = strlen(g_aCommandLineOptions[i].m_pszName);
            } else {
                cchOption = (size_t)(pszColon - g_aCommandLineOptions[i].m_pszName);
            }

            if (CompareCommandLineOptionPrefix(pszToken, g_aCommandLineOptions[i].m_pszName, cchOption) == 0) {
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
    unsigned int i;

    g_cParsedArgs = 0;
    for (i = 0; i < LEMBALL_ARRAY_COUNT(g_afCommandLineOptionSpecified); ++i) {
        g_afCommandLineOptionSpecified[i] = 0;
    }
    g_apszParsedArgs[0] = pszCmdLine;

    if (pszCmdLine == 0) {
        return;
    }

    while (LEMBALL_CRT_ISCTYPE((int)*pszCmdLine, _SPACE)) {
        ++pszCmdLine;
    }
    if (*pszCmdLine == '\0') {
        return;
    }

    g_cParsedArgs = 0;
    pszCursor = pszCmdLine;
    while (*pszCursor != '\0') {
        if (LEMBALL_CRT_ISCTYPE((int)*pszCursor, _UPPER)) {
            *pszCursor = (char)_tolower((int)*pszCursor);
        }

        if (LEMBALL_CRT_ISCTYPE((int)*pszCursor, _SPACE)) {
            do {
                *pszCursor = '\0';
                ++pszCursor;
            } while (LEMBALL_CRT_ISCTYPE((int)*pszCursor, _SPACE));

            if (*pszCursor != '\0') {
                ++g_cParsedArgs;
                g_apszParsedArgs[g_cParsedArgs] = pszCursor;
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

// FUNCTION: LEMBALL 0x00459970
void LongjmpStartupExceptionFrame(int nJumpResult) {
    longjmp(g_GameStartupJumpBuffer, nJumpResult);
}

// FUNCTION: LEMBALL 0x00459860
int RunGameStartupSequence(char *pszCmdLine) {
    size_t i;
    int nResult;
    int nJumpResult;

    TokenizeAndFilterCommandLineArgs(pszCmdLine);
    FinalizeStartupGraphicsDriverConfig();
    InitializeCoreSubsystems();

    g_pStatusOutputStream->AppendCStringToStream("Command line options:\n");
    i = 0;
    do {
        g_pStatusOutputStream->AppendCStringToStream("  Option ")
            ->AppendCStringToStream(g_aCommandLineOptions[i].m_pszName)
            ->AppendCStringToStream(" is ");
        if (!g_afCommandLineOptionSpecified[i]) {
            g_pStatusOutputStream->AppendCStringToStream("not ");
        }
        g_pStatusOutputStream->AppendCStringToStream("selected\n");
        ++i;
    } while (i < LEMBALL_ARRAY_COUNT(g_aCommandLineOptions));

    nJumpResult = setjmp(g_GameStartupJumpBuffer);
    if (nJumpResult != 0) {
        ShutdownCoreSubsystems();
        return nJumpResult;
    }

    nJumpResult = setjmp(g_DebugThreadJumpBuffer);
    if (nJumpResult != 0) {
        ShutdownDebugMessageThread(g_fStartupNoWait);
        return nJumpResult;
    }

    nResult = RunMainGameSessionThunk(
        (int)g_cParsedArgs, (const char *const *)g_apszParsedArgs);
    ShutdownCoreSubsystems();
    return nResult;
}
