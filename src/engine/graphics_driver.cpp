#include "graphics_driver.h"
#include "../network/safe_vtable.h"
#include "../main.h"
#include "../platform/startup_options.h"
#include "../engine/memory_arena.h"
#include "../engine/runtime_init.h"
#include "../game/game_app.h"

#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif
#include <string.h>

extern LRESULT CALLBACK WindowOwnerWindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
extern "C" DWORD WINAPI timeGetTime(void);
extern void *LEMBALL_FASTCALL InitializePackagedRectQueueEntry(void *pObject);

int g_fStartupFullscreen = 0;
int g_fStartupGraphicsDriverWing = 0;
int g_fStartupGraphicsDriverCds = 1;
int g_fStartupGraphicsDriverGdk = 0;
// GLOBAL: LEMBALL 0x004a0768
void *g_pSelectedGraphicsDriverRuntime = 0;
static void *g_pResourceGeometryHelperSlotManager = 0;
static void *g_pResourceGeometryHelperTarget = 0;
// GLOBAL: LEMBALL 0x004a9bf4
int *g_pArrowCursorStatusIndicatorRenderClient = 0;
static VSGDI_DisplayState *g_pDisplayState = 0;

static const char g_VSGDI_DdrawDll[] = "DDRAW.DLL";
static const char g_VSGDI_Dspdib32Dll[] = "DSPDIB32.DLL";
static const char *g_VSGDI_BaseWindowClass = "VS_Base_Window_Class";
static const char g_VSGDI_DirectDrawCreate[] = "DirectDrawCreate";
static const char g_VSGDI_DirectDrawWindowClass[] = "DirectDrawClass";
static const char g_VSGDI_DirectDrawWindowTitle[] = "DirectDraw";
static const char g_VSGDI_UnknownDirectDrawError[] = "UNKNOWN DIRECT DRAW ERROR: ";
static const char *g_pVSGDI_UnknownDirectDrawError = g_VSGDI_UnknownDirectDrawError;
static char g_VSGDI_DirectDrawErrorText[64];
static const char g_VSGDI_DisplayDibStart32[] = "DisplayDibStart32";
static const char g_VSGDI_DisplayDibEnd32[] = "DisplayDibEnd32";
static const char g_VSGDI_DisplayDibGetAddress32[] = "DisplayDibGetAddress32";
static const char g_VSGDI_DisplayDib32[] = "DisplayDib32";
static const char g_VSGDI_ErrorNoValidDriver[] = "No valid driver selected to initialise\n";
static const char g_VSGDI_ErrorNoValidDriverAvailable[] = "No valid driver available\n";
static const char g_VSGDI_ErrorRegisterClass[] = "Unable to register DD base window class";
static const char g_VSGDI_ErrorSetCooperativeLevel[] =
    "Direct Draw Set Coorperative Level (DD object) failed : ";
static const char g_VSGDI_ErrorSetDisplayMode[] = "Direct Draw Set Display Mode failed : ";
static const char g_VSGDI_ErrorCreatePrimarySurface[] =
    "Direct Draw Create Primary Surface failed : ";
static const char g_VSGDI_ErrorLineEnd[] = "\n";
static const char g_VSGDI_InitDriverPrefix[] = "Initialising graphics device driver: ";
static const char g_VSGDI_AutoSelectedPrefix[] = "[ Auto selected: ";
static const char g_VSGDI_AutoSelectedSuffix[] = " ]\n";
static const char g_VSGDI_DefaultWindowModeSuffix[] =
    ". Defaulting to normal window mode (using CreateDIBSection)";
static const char g_VSGDI_WarningCaption[] = "WARNING";
static FARPROC g_pApplyFullscreenDisplayModeThunk = 0;

// FUNCTION: LEMBALL 0x00456720
char *LookupDirectDrawErrorString(unsigned int nError) {
    const char *pszPrefix = g_pVSGDI_UnknownDirectDrawError;

    strcpy(g_VSGDI_DirectDrawErrorText, pszPrefix);
    FormatSignedIntToRadixString(
        (int)nError,
        g_VSGDI_DirectDrawErrorText + strlen(pszPrefix),
        10);
    return g_VSGDI_DirectDrawErrorText;
}

// FUNCTION: LEMBALL 0x00456780
void NoopDirectDrawCallback(unsigned int nResult, unsigned int nUnused) {
    (void)nResult;
    (void)nUnused;
}
#define VSGDI_SAFE_TABLE(name) static void *name[32]
VSGDI_SAFE_TABLE(g_VSGDI_CompactResourceGeometryHelperVtable);
VSGDI_SAFE_TABLE(g_VSGDI_ResourceGeometryRowBufferVtable);
VSGDI_SAFE_TABLE(g_VSGDI_ResourceGeometryHelperGroup0Vtable);
VSGDI_SAFE_TABLE(g_VSGDI_ResourceGeometryHelperGroup0CompactSubobjectVtable);
VSGDI_SAFE_TABLE(g_VSGDI_ResourceGeometryHelperGroup1Vtable);
VSGDI_SAFE_TABLE(g_VSGDI_ResourceGeometryHelperGroup1CompactSubobjectVtable);
VSGDI_SAFE_TABLE(g_VSGDI_ResourceGeometryHelperTargetConstructionVtable);
VSGDI_SAFE_TABLE(g_VSGDI_CompactResourceGeometryHelperConstructionVtable);
VSGDI_SAFE_TABLE(g_VSGDI_ResourceGeometryHelperTargetVtable);
VSGDI_SAFE_TABLE(g_VSGDI_ResourceGeometryHelperGroup1RowBufferVtable);
VSGDI_SAFE_TABLE(g_VSGDI_ResourceGeometryHelperGroup0RowBufferVtable);
// GLOBAL: LEMBALL 0x00499508
static void *g_VSGDI_CompactResourceGeometryHelperSubobjectVtable[32];
VSGDI_SAFE_TABLE(g_VSGDI_ResourceGeometryHelperTargetParamWrapperVtable);
VSGDI_SAFE_TABLE(g_VSGDI_ResourceGeometryHelperSlotManagerVtable);
VSGDI_SAFE_TABLE(g_VSGDI_StatusIndicatorBaseVtable);
VSGDI_SAFE_TABLE(g_VSGDI_ArrowCursorRuntimeVtable);
VSGDI_SAFE_TABLE(g_VSGDI_StatusIndicatorRenderClientVtable);
VSGDI_SAFE_TABLE(g_VSGDI_StatusIndicatorPointSinkVtable);

static void *LEMBALL_FASTCALL VSGDI_DeleteDisplayState(
    void *pDisplayState, int nUnused, int fDelete);

// FUNCTION: LEMBALL 0x0046CBD0
int LEMBALL_FASTCALL GetResourceGeometryHelperTargetActiveUploadState(void *pTarget) {
    return *(int *)((char *)pTarget + 0x550);
}

#undef VSGDI_SAFE_TABLE

struct VSGDI_SafeTableInitializer {
    VSGDI_SafeTableInitializer(void) {
        void **tables[] = {
            g_VSGDI_CompactResourceGeometryHelperVtable,
            g_VSGDI_ResourceGeometryRowBufferVtable,
            g_VSGDI_ResourceGeometryHelperGroup0Vtable,
            g_VSGDI_ResourceGeometryHelperGroup0CompactSubobjectVtable,
            g_VSGDI_ResourceGeometryHelperGroup1Vtable,
            g_VSGDI_ResourceGeometryHelperGroup1CompactSubobjectVtable,
            g_VSGDI_ResourceGeometryHelperTargetConstructionVtable,
            g_VSGDI_CompactResourceGeometryHelperConstructionVtable,
            g_VSGDI_ResourceGeometryHelperTargetVtable,
            g_VSGDI_ResourceGeometryHelperGroup1RowBufferVtable,
            g_VSGDI_ResourceGeometryHelperGroup0RowBufferVtable,
            g_VSGDI_CompactResourceGeometryHelperSubobjectVtable,
            g_VSGDI_ResourceGeometryHelperTargetParamWrapperVtable,
            g_VSGDI_ResourceGeometryHelperSlotManagerVtable,
            g_VSGDI_StatusIndicatorBaseVtable,
            g_VSGDI_ArrowCursorRuntimeVtable,
            g_VSGDI_StatusIndicatorRenderClientVtable,
            g_VSGDI_StatusIndicatorPointSinkVtable
        };
        int i;
        int j;
        for (i = 0; i < 18; ++i)
            for (j = 0; j < 32; ++j)
                tables[i][j] = (void *)NetworkSafeVtableNoop;

        g_VSGDI_ResourceGeometryHelperTargetVtable[2] =
            (void *)GetResourceGeometryHelperTargetActiveUploadState;
    }
};
static VSGDI_SafeTableInitializer g_VSGDI_SafeTableInitializer;

struct VSGDI_CompactHelperAdjustorTable {
    int m_nThisDelta;
    int m_nSubobjectOffset;
};

/* Ghidra 00466440/00466670: both temporary row-group adjustors are {-4, 0x54}. */
static VSGDI_CompactHelperAdjustorTable g_VSGDI_ResourceGeometryHelperGroup0ConstructionAdjustorVtable = {
    -4, 0x54
};
static VSGDI_CompactHelperAdjustorTable g_VSGDI_ResourceGeometryHelperGroup1ConstructionAdjustorVtable = {
    -4, 0x54
};

static VSGDI_CompactHelperAdjustorTable g_VSGDI_CompactHelperTargetAdjustor = { -0x40, 0x51c };
static VSGDI_CompactHelperAdjustorTable g_VSGDI_HelperGroup1Adjustor = { -4, 0x514 };
static VSGDI_CompactHelperAdjustorTable g_VSGDI_HelperGroup0Adjustor = { -4, 0x4c0 };

static const char g_VSGDI_UnallocatedHelperSlotMessage[] = "Unallocated helper slot found\n";
static int g_nResourceGeometryHelperSlotCount = 0;
static void *g_pResourceGeometryListHead = 0;

struct VSGDI_HelperTargetParamWrapper {
    void *m_pVtable;
    int m_nWrappedParam;
    int m_nReserved;
};

struct VSGDI_ResourceGeometryHelperSlot {
    int m_nOwnerOrState0;
    int m_nOwnerOrState4;
    int m_nOwnerOrState8;
    int m_nOwnerOrStateC;
    int m_nOwnerOrState10;
    int m_fAvailable;
    int m_nOwnerOrState18;
};

struct VSGDI_ResourceGeometryHelperSlotManager {
    void *m_pVtable;
    VSGDI_ResourceGeometryHelperSlot *m_pSlots;
    int m_nReserved08;
    int m_nReserved0C;
    int m_cSlots;

    VSGDI_ResourceGeometryHelperSlotManager *InitializeResourceGeometryHelperSlotManager(int cSlots);
    void *AllocateResourceGeometryHelperSlot(short *pRect, void *pWrappedTarget);
};

struct VSGDI_ResourceGeometryLinkNode {
    void *m_pTarget;
    VSGDI_ResourceGeometryLinkNode *m_pPrev;
    VSGDI_ResourceGeometryLinkNode *m_pNext;
};

struct VSGDI_ResourceGeometryLinkList {
    VSGDI_ResourceGeometryLinkNode *m_pTail;
    VSGDI_ResourceGeometryLinkNode *m_pHead;
    int m_cNodes;
};

struct VSGDI_CompactResourceGeometryHelperFields {
    unsigned char m_abReserved00[0x3c];
    int m_nThisAdjust;
    void *m_pVtable;
    unsigned char m_abReserved44[0x1c];
    void *m_pLinkedTarget;
    unsigned char m_abReserved64[0xc];
    int m_nQueuedState;
};

struct VSGDI_ResourceGeometryHelperTargetView {
    unsigned char m_abRowBuffer[0x44];
    unsigned char m_abHelperGroup1[0x54];
    unsigned char m_abHelperGroup0[0x54];
    short m_nReservedEC;
    short m_nReservedEE;
    void *m_pBackingBitmap;
    int m_nReservedF4;
    void *m_pDisplayBinding;
    unsigned char m_abReservedFC[0x42c];
    VSGDI_ResourceGeometryLinkList m_ChildTargetLinks;
    unsigned char m_abCriticalSection[0x18];
    int m_fCriticalSectionReady;
    int m_nActiveUploadState;
    unsigned char m_abReserved554[8];

    VSGDI_ResourceGeometryHelperTargetView *ConstructResourceGeometryHelperTarget(
        int nWrappedParam, int fConstructCompactHelper);
    VSGDI_ResourceGeometryHelperTargetView *InitializeResourceGeometryHelper(
        const VSGDI_Rect *pRect, void *pWrappedTarget, int fConstructCompactHelper);
    void MarkOrForwardHelperTargetDirtyRect(short *pRect);
};

struct VSGDI_GeometryHelperPointerArray {
    void *m_pEntries;
    int m_nReserved;
    int m_nEntryCount;
    VSGDI_ResourceGeometryHelperTargetView *m_pTarget;

    VSGDI_GeometryHelperPointerArray *InitializeGeometryHelperPointerArray(
        short *pRect, int nEntryParameter, void *pWrappedTarget);
};

struct VSGDI_HelperSurfaceVariableBlock {
    unsigned char m_abReserved00[0x4c];
    short m_aRequestedRect[4];
    short m_aWorkingRect[4];
    int m_nReserved5C;
    void *m_pLinkedTarget;
    short m_aBackingRect[2];
    unsigned char m_abReserved68[0x18];
    int m_nBackingWidthHint;
};

struct VSGDI_HelperSurfaceBindingSurface {
    void **m_pVtable;
    unsigned char m_abReserved04[0x10];
    short m_nOriginY;
    short m_nOriginX;
};

struct VSGDI_HelperDispatchQueue {
    void **m_ppEntries;
    int m_cEntries;
    int m_nReserved08;
    VSGDI_HelperSurface *m_pHelperTarget;
};

struct VSGDI_QuantizedHelperUploadRectTracker {
    int InitializeQuantizedHelperUploadRectTracker(
        int cRectEntries, int nTrackerExtent, int nCellExtent);
    void MarkHelperUploadCellsForRect(short *pRect);
};

void LEMBALL_FASTCALL InitializeHelperUploadStatePending(int nUploadState);

// FUNCTION: LEMBALL 0x00466AC0
void LEMBALL_FASTCALL EnsureHelperUploadStateMapCapacity(void *pTracker) {
    int nMapEntries;

    if (*(int *)((char *)pTracker + 0x10) == 0) {
        nMapEntries = (int)*(short *)((char *)pTracker + 0x1c) *
            (int)*(short *)((char *)pTracker + 0x1e);
        *(int *)((char *)pTracker + 0x20) = nMapEntries;
        *(void **)((char *)pTracker + 0x10) = AllocateVSMemBlock((unsigned int)nMapEntries);
    }
}

// FUNCTION: LEMBALL 0x004669A0
int VSGDI_QuantizedHelperUploadRectTracker::InitializeQuantizedHelperUploadRectTracker(int cRectEntries, int nTrackerExtent, int nCellExtent) {
    short *pTracker;
    short *pRect;
    int i;
    short cxTracker;
    short cyTracker;
    short cxCell;
    short cyCell;

    pTracker = (short *)this;
    pTracker[0xb] = 0;
    pTracker[0xa] = 0;
    pTracker[0xd] = 0;
    pTracker[0xc] = 0;
    pTracker[0xf] = 0;
    pTracker[0xe] = 0;
    pTracker[0x19] = 0;
    pTracker[0x18] = 0;
    pTracker[0x1b] = 0;
    pTracker[0x1a] = 0;

    pRect = 0;
    if (cRectEntries != 0) {
        pRect = (short *)AllocateVSMemBlock((unsigned int)(cRectEntries * 0xc));
    }
    if (pRect == 0) {
        *(void **)((char *)this + 0xc) = 0;
    } else {
        for (i = 0; i < cRectEntries; ++i) {
            pRect[0] = 0;
            pRect[1] = 0;
            pRect[2] = 0;
            pRect[3] = 0;
            pRect[4] = 0;
            pRect[5] = 0;
            pRect += 6;
        }
        *(short **)((char *)this + 0xc) = pRect - cRectEntries * 6;
    }

    *(int *)this = cRectEntries;
    *(int *)((char *)this + 0x18) = nCellExtent;
    *(int *)((char *)this + 0x14) = nTrackerExtent;
    cxTracker = (short)nTrackerExtent;
    cyTracker = (short)((unsigned int)nTrackerExtent >> 16);
    cxCell = (short)nCellExtent;
    cyCell = (short)((unsigned int)nCellExtent >> 16);
    *(short *)((char *)this + 0x1c) =
        (short)((cxTracker + cxCell - 1) / cxCell);
    *(short *)((char *)this + 0x1e) =
        (short)((cyTracker + cyCell - 1) / cyCell);
    *(int *)((char *)this + 0x44) = 0;
    *(int *)((char *)this + 0x10) = 0;
    EnsureHelperUploadStateMapCapacity(this);
    InitializeHelperUploadStatePending((int)(unsigned long)this);
    return (int)(unsigned long)this;
}

// FUNCTION: LEMBALL 0x00466BE0
void VSGDI_QuantizedHelperUploadRectTracker::MarkHelperUploadCellsForRect(short *pRect) {
    char *pTracker;
    short cxCell;
    short cyCell;
    int nFirstColumn;
    int nFirstRow;
    unsigned int cColumns;
    int cRows;
    int nGridWidth;
    int nRow;
    unsigned char *pRow;
    unsigned int i;

    pTracker = (char *)this;
    if (*(int *)(pTracker + 0x10) == 0)
        return;

    cxCell = *(short *)(pTracker + 0x18);
    cyCell = *(short *)(pTracker + 0x1a);
    nFirstColumn = pRect[2] / cxCell;
    nFirstRow = pRect[3] / cyCell;
    cColumns = (unsigned int)((pRect[0] + pRect[2] - 1 + cxCell) / cxCell - nFirstColumn);
    cRows = (pRect[1] + pRect[3] - 1 + cyCell) / cyCell - nFirstRow;
    if (*(short *)(pTracker + 0x1e) < nFirstRow + cRows)
        cRows = *(short *)(pTracker + 0x1e) - nFirstRow;
    nGridWidth = *(short *)(pTracker + 0x1c);
    if (nGridWidth < nFirstColumn + (int)cColumns)
        cColumns = (unsigned int)(nGridWidth - nFirstColumn);

    if ((int)cColumns <= 0 || cRows <= 0)
        return;

    pRow = (unsigned char *)(*(int *)(pTracker + 0x10) +
        nGridWidth * nFirstRow + nFirstColumn);
    nRow = cRows;
    do {
        for (i = 0; i < cColumns; ++i)
            pRow[i] = *(unsigned char *)(pTracker + 0x48);
        pRow += nGridWidth;
        --nRow;
    } while (nRow != 0);
    *(int *)(pTracker + 0x2c) += (int)pRect[0] * (int)pRect[1];
}

extern void TriggerReleaseAssertFailure(const char *pszExpression, const char *pszFile, int nLine);
extern int g_fRootHelperGeometryDispatchSuppressed;
extern void *g_pQueuedRenderPointSinkFinalizeThunk;
static int GetHelperSurfaceVariableBlockOffset(const VSGDI_HelperSurface *pSurface);

// FUNCTION: LEMBALL 0x00472310
void LEMBALL_FASTCALL ClearResourceGeometryRowBufferFields(void *pvRowBuffer) {
    char *pRowBuffer;

    pRowBuffer = (char *)pvRowBuffer;
    *(int *)(pRowBuffer + 0x10) = 0;
    *(int *)(pRowBuffer + 0x08) = 0;
    *(int *)(pRowBuffer + 0x0c) = 0;
    *(int *)(pRowBuffer + 0x18) = 0;
    *(int *)(pRowBuffer + 0x14) = 0;
    *(int *)(pRowBuffer + 0x04) = 0;
    *(int *)(pRowBuffer + 0x1c) = 0;
    *(int *)(pRowBuffer + 0x24) = 0;
    *(int *)(pRowBuffer + 0x20) = 0;
    *(int *)(pRowBuffer + 0x28) = 0;
}

static void ReleaseResourceGeometryRowBufferStorage(int nRowBuffer) {
    if (*(int *)(unsigned long)(nRowBuffer + 4) != 0) {
        FreeVSMemBlock((void *)(unsigned long)*(int *)(unsigned long)(nRowBuffer + 4));
    }
    ClearResourceGeometryRowBufferFields((void *)(unsigned long)nRowBuffer);
}

static void ResetHelperUploadStateMap(int) {
}

struct VSGDI_CompactResourceGeometryHelper {
    void *InitializeCompactResourceGeometryHelper(void);
};

// FUNCTION: LEMBALL 0x004663D0
void *VSGDI_CompactResourceGeometryHelper::InitializeCompactResourceGeometryHelper(void) {
    char *pbHelper;
    unsigned short wZero;

    pbHelper = (char *)this;
    wZero = 0;
    *(unsigned short *)(pbHelper + 6) = wZero;
    *(unsigned short *)(pbHelper + 4) = wZero;
    *(unsigned short *)(pbHelper + 10) = wZero;
    *(unsigned short *)(pbHelper + 8) = wZero;
    *(unsigned short *)(pbHelper + 0xe) = wZero;
    *(unsigned short *)(pbHelper + 0xc) = wZero;
    *(unsigned short *)(pbHelper + 0x12) = wZero;
    *(unsigned short *)(pbHelper + 0x10) = wZero;
    *(unsigned short *)(pbHelper + 0x16) = wZero;
    *(unsigned short *)(pbHelper + 0x14) = wZero;
    *(unsigned short *)(pbHelper + 0x1a) = wZero;
    *(unsigned short *)(pbHelper + 0x18) = wZero;
    *(unsigned short *)(pbHelper + 0x1e) = wZero;
    *(unsigned short *)(pbHelper + 0x1c) = wZero;
    *(void **)pbHelper = &g_VSGDI_CompactResourceGeometryHelperVtable;
    *(int *)(pbHelper + 0x3c) = 1;
    *(unsigned short *)(pbHelper + 0x26) = wZero;
    *(int *)(pbHelper + 0x40) = 0;
    *(unsigned short *)(pbHelper + 0x24) = wZero;
    *(unsigned short *)(pbHelper + 0x2a) = wZero;
    *(unsigned short *)(pbHelper + 0x28) = wZero;
    return this;
}

// FUNCTION: LEMBALL 0x00472290
void *InitializeResourceGeometryRowBuffer(void *pvRowBuffer) {
    int *pRowBuffer;

    pRowBuffer = (int *)pvRowBuffer;
    *(short *)((char *)pRowBuffer + 0x2e) = 0;
    *(short *)(pRowBuffer + 0xb) = 0;
    *(short *)((char *)pRowBuffer + 0x32) = 0;
    *(short *)(pRowBuffer + 0xc) = 0;
    *(short *)((char *)pRowBuffer + 0x36) = 0;
    *(short *)(pRowBuffer + 0xd) = 0;
    *pRowBuffer = (int)(unsigned long)&g_VSGDI_ResourceGeometryRowBufferVtable;
    *(short *)((char *)pRowBuffer + 0x3a) = 0;
    *(short *)(pRowBuffer + 0xe) = 0;
    *(short *)((char *)pRowBuffer + 0x3e) = 0;
    *(short *)(pRowBuffer + 0xf) = 0;
    ClearResourceGeometryRowBufferFields(pRowBuffer);
    return pRowBuffer;
}

// FUNCTION: LEMBALL 0x00466670
void *ConstructHelperGroup1RowBuffer(void *pvHelperGroup, int fConstructCompactHelper) {
    int iCompactDelta;
    int *pHelperGroup;

    pHelperGroup = (int *)pvHelperGroup;
    if (fConstructCompactHelper != 0) {
        *(void **)((char *)pHelperGroup + 4) = &g_VSGDI_ResourceGeometryHelperGroup1ConstructionAdjustorVtable;
        ((VSGDI_CompactResourceGeometryHelper *)((char *)pHelperGroup + 0x58))
            ->InitializeCompactResourceGeometryHelper();
    }
    InitializeResourceGeometryRowBuffer((char *)pHelperGroup + 8);
    *(void **)pHelperGroup = &g_VSGDI_ResourceGeometryHelperGroup1Vtable;
    iCompactDelta = *(int *)((char *)(unsigned long)pHelperGroup[1] + 4);
    *(void **)((char *)pHelperGroup + 4 + iCompactDelta) =
        (void *)&g_VSGDI_ResourceGeometryHelperGroup1CompactSubobjectVtable;
    *(int *)((char *)pHelperGroup + iCompactDelta) = iCompactDelta - 0x54;
    *(short *)((char *)pHelperGroup + 0x50) = 0;
    *(int *)((char *)pHelperGroup + 0x48) = 0;
    *(short *)((char *)pHelperGroup + 0x52) = 0;
    *(int *)((char *)pHelperGroup + 0x4c) = 0;
    return pHelperGroup;
}

// FUNCTION: LEMBALL 0x00466440
void *ConstructHelperGroup0RowBuffer(void *pvHelperGroup, int fConstructCompactHelper) {
    int iCompactDelta;
    int *pHelperGroup;

    pHelperGroup = (int *)pvHelperGroup;
    if (fConstructCompactHelper != 0) {
        *(void **)((char *)pHelperGroup + 4) = &g_VSGDI_ResourceGeometryHelperGroup0ConstructionAdjustorVtable;
        ((VSGDI_CompactResourceGeometryHelper *)((char *)pHelperGroup + 0x58))
            ->InitializeCompactResourceGeometryHelper();
    }
    InitializeResourceGeometryRowBuffer((char *)pHelperGroup + 8);
    *(void **)pHelperGroup = &g_VSGDI_ResourceGeometryHelperGroup0Vtable;
    iCompactDelta = *(int *)((char *)(unsigned long)pHelperGroup[1] + 4);
    *(void **)((char *)pHelperGroup + 4 + iCompactDelta) =
        (void *)&g_VSGDI_ResourceGeometryHelperGroup0CompactSubobjectVtable;
    *(int *)((char *)pHelperGroup + iCompactDelta) = iCompactDelta - 0x54;
    *(short *)((char *)pHelperGroup + 0x50) = 0;
    *(int *)((char *)pHelperGroup + 0x4c) = 0;
    *(short *)((char *)pHelperGroup + 0x52) = 0;
    *(int *)((char *)pHelperGroup + 0x48) = 0;
    return pHelperGroup;
}

// FUNCTION: LEMBALL 0x00466510
void LEMBALL_FASTCALL ReleaseHelperGroup0BackingBuffer(int nHelperGroup) {
    if (*(int *)(unsigned long)(nHelperGroup + 0x4c) != 0) {
        FreeVSMemBlock((void *)(unsigned long)*(int *)(unsigned long)(nHelperGroup + 0x4c));
        *(int *)(unsigned long)(nHelperGroup + 0x4c) = 0;
        *(short *)(unsigned long)(nHelperGroup + 0x52) = 0;
        *(short *)(unsigned long)(nHelperGroup + 0x50) = 0;
    }
    ReleaseResourceGeometryRowBufferStorage(nHelperGroup + 8);
}

// FUNCTION: LEMBALL 0x00466710
void ReleaseHelperGroup1BackingBuffer(int nHelperGroup) {
    if (*(int *)(unsigned long)(nHelperGroup + 0x4c) != 0) {
        FreeVSMemBlock((void *)(unsigned long)*(int *)(unsigned long)(nHelperGroup + 0x4c));
        *(int *)(unsigned long)(nHelperGroup + 0x4c) = 0;
        *(short *)(unsigned long)(nHelperGroup + 0x52) = 0;
        *(short *)(unsigned long)(nHelperGroup + 0x50) = 0;
    }
    ReleaseResourceGeometryRowBufferStorage(nHelperGroup + 8);
}

// FUNCTION: LEMBALL 0x00466B90
void ResetHelperUploadRectListAndStateMap(int nUploadState) {
    if (*(int *)(unsigned long)(nUploadState + 0xc) != 0) {
        FreeVSMemBlock((void *)(unsigned long)*(int *)(unsigned long)(nUploadState + 0xc));
        *(int *)(unsigned long)(nUploadState + 0xc) = 0;
    }
    ResetHelperUploadStateMap(nUploadState);
}

// FUNCTION: LEMBALL 0x0046BC00
VSGDI_ResourceGeometryHelperSlotManager *VSGDI_ResourceGeometryHelperSlotManager::InitializeResourceGeometryHelperSlotManager(int cSlots) {
    VSGDI_ResourceGeometryHelperSlotManager *pSlotManager;
    VSGDI_ResourceGeometryHelperSlot *pSlot;
    int cRemaining;

    pSlotManager = this;
    pSlotManager->m_pVtable = &g_VSGDI_ResourceGeometryHelperSlotManagerVtable;
    pSlotManager->m_pSlots =
        (VSGDI_ResourceGeometryHelperSlot *)AllocateVSMemBlock((unsigned int)(cSlots * sizeof(*pSlotManager->m_pSlots)));
    pSlotManager->m_cSlots = cSlots;
    pSlotManager->m_nReserved08 = 0;
    pSlotManager->m_nReserved0C = 0;
    if (0 < cSlots) {
        pSlot = pSlotManager->m_pSlots;
        cRemaining = cSlots;
        do {
            pSlot->m_nOwnerOrState0 = 0;
            pSlot->m_nOwnerOrState8 = 0;
            pSlot->m_nOwnerOrState4 = 0;
            pSlot->m_nOwnerOrState10 = 0;
            pSlot->m_nOwnerOrStateC = 0;
            pSlot->m_fAvailable = 1;
            pSlot->m_nOwnerOrState18 = 0;
            ++pSlot;
            --cRemaining;
        } while (cRemaining != 0);
    }
    g_pResourceGeometryHelperTarget = 0;
    return pSlotManager;
}

void *InitializeResourceGeometryHelperSlotManager(void *pvSlotManager, int cSlots) {
    return ((VSGDI_ResourceGeometryHelperSlotManager *)pvSlotManager)
        ->InitializeResourceGeometryHelperSlotManager(cSlots);
}

// FUNCTION: LEMBALL 0x0046BC90
void DestroyResourceGeometryHelperSlotManager(void *pvSlotManager) {
    VSGDI_ResourceGeometryHelperSlotManager *pSlotManager;
    VSGDI_ResourceGeometryHelperSlot *pSlot;
    int i;

    pSlotManager = (VSGDI_ResourceGeometryHelperSlotManager *)pvSlotManager;
    pSlotManager->m_pVtable = &g_VSGDI_ResourceGeometryHelperSlotManagerVtable;
    i = 0;
    if (0 < pSlotManager->m_cSlots) {
        pSlot = pSlotManager->m_pSlots;
        do {
            if (pSlot->m_fAvailable == 0) {
                AppendErrorCString(g_VSGDI_UnallocatedHelperSlotMessage);
            }
            ++pSlot;
            ++i;
        } while (i < pSlotManager->m_cSlots);
    }
    FreeVSMemBlock(pSlotManager->m_pSlots);
}

// FUNCTION: LEMBALL 0x0046BCE0
int FindFirstFreeResourceGeometryHelperSlotIndex(void *pvSlotManager) {
    VSGDI_ResourceGeometryHelperSlotManager *pSlotManager;
    VSGDI_ResourceGeometryHelperSlot *pSlot;
    int i;

    pSlotManager = (VSGDI_ResourceGeometryHelperSlotManager *)pvSlotManager;
    i = 0;
    if (0 < pSlotManager->m_cSlots) {
        pSlot = pSlotManager->m_pSlots;
        do {
            if (pSlot->m_fAvailable != 0) {
                return i;
            }
            ++pSlot;
            ++i;
        } while (i < pSlotManager->m_cSlots);
    }
    return -1;
}

// FUNCTION: LEMBALL 0x0046C5D0
VSGDI_ResourceGeometryHelperTargetView *VSGDI_ResourceGeometryHelperTargetView::ConstructResourceGeometryHelperTarget(int nWrappedParam, int fConstructCompactHelper) {
    int iCompactDelta;
    VSGDI_CompactResourceGeometryHelperFields *pCompactHelper;
    VSGDI_HelperTargetParamWrapper *pParamWrapper;
    VSGDI_ResourceGeometryHelperTargetView *pTarget;

    pTarget = this;
    if (fConstructCompactHelper != 0) {
        *(void **)((char *)pTarget + 0x40) = &g_VSGDI_CompactHelperTargetAdjustor;
        *(void **)((char *)pTarget + 0x48) = &g_VSGDI_HelperGroup1Adjustor;
        *(void **)((char *)pTarget + 0x9c) = &g_VSGDI_HelperGroup0Adjustor;
        ((VSGDI_CompactResourceGeometryHelper *)((char *)pTarget + 0x55c))
            ->InitializeCompactResourceGeometryHelper();
    }
    InitializeResourceGeometryRowBuffer(pTarget);
    *(void **)pTarget = &g_VSGDI_ResourceGeometryHelperTargetConstructionVtable;
    iCompactDelta = *(int *)((char *)(unsigned long)*(int *)((char *)pTarget + 0x40) + 4);
    pCompactHelper = (VSGDI_CompactResourceGeometryHelperFields *)((char *)pTarget + iCompactDelta);
    pCompactHelper->m_pVtable = &g_VSGDI_CompactResourceGeometryHelperConstructionVtable;
    ConstructHelperGroup1RowBuffer((char *)pTarget + 0x44, 0);
    ConstructHelperGroup0RowBuffer((char *)pTarget + 0x98, 0);
    pTarget->m_nReservedEE = 0;
    pTarget->m_ChildTargetLinks.m_pTail = 0;
    pTarget->m_nReservedEC = 0;
    pTarget->m_ChildTargetLinks.m_pHead = 0;
    pTarget->m_ChildTargetLinks.m_cNodes = 0;
    *(void **)pTarget = &g_VSGDI_ResourceGeometryHelperTargetVtable;
    *(void **)((char *)pTarget + 0x44) = &g_VSGDI_ResourceGeometryHelperGroup1RowBufferVtable;
    *(void **)((char *)pTarget + 0x98) = &g_VSGDI_ResourceGeometryHelperGroup0RowBufferVtable;
    pCompactHelper->m_pVtable = &g_VSGDI_CompactResourceGeometryHelperSubobjectVtable;
    pCompactHelper->m_nThisAdjust = iCompactDelta - 0x51c;

    pParamWrapper = (VSGDI_HelperTargetParamWrapper *)AllocateVSMemBlock(0xc);
    if (pParamWrapper == 0) {
        pTarget->m_pDisplayBinding = 0;
    } else {
        pParamWrapper->m_pVtable = &g_VSGDI_ResourceGeometryHelperTargetParamWrapperVtable;
        pParamWrapper->m_nWrappedParam = nWrappedParam;
        pParamWrapper->m_nReserved = 0;
        pTarget->m_pDisplayBinding = pParamWrapper;
    }
    pTarget->m_pBackingBitmap = 0;
    pTarget->m_nActiveUploadState = 0;
    pCompactHelper->m_pLinkedTarget = 0;
    InitializeCriticalSection(pTarget->m_abCriticalSection);
    pTarget->m_fCriticalSectionReady = 1;
    pCompactHelper->m_nQueuedState = 0;
    return pTarget;
}

// FUNCTION: LEMBALL 0x0046C050
VSGDI_ResourceGeometryHelperTargetView *VSGDI_ResourceGeometryHelperTargetView::InitializeResourceGeometryHelper(const VSGDI_Rect *pRect, void *pWrappedTarget, int fConstructCompactHelper) {
    int iCompactDelta;
    VSGDI_CompactResourceGeometryHelperFields *pCompactHelper;
    VSGDI_ResourceGeometryLinkList *pGlobalList;
    VSGDI_ResourceGeometryLinkNode *pLink;
    VSGDI_ResourceGeometryLinkList *pChildList;
    VSGDI_QuantizedHelperUploadRectTracker *pTracker;
    VSGDI_Rect TrackerExtent;
    VSGDI_Rect CellExtent;

    if (fConstructCompactHelper != 0) {
        *(void **)((char *)this + 0x40) = &g_VSGDI_CompactHelperTargetAdjustor;
        *(void **)((char *)this + 0x48) = &g_VSGDI_HelperGroup1Adjustor;
        *(void **)((char *)this + 0x9c) = &g_VSGDI_HelperGroup0Adjustor;
        ((VSGDI_CompactResourceGeometryHelper *)((char *)this + 0x55c))
            ->InitializeCompactResourceGeometryHelper();
    }

    InitializeResourceGeometryRowBuffer(this);
    *(void **)this = &g_VSGDI_ResourceGeometryHelperTargetConstructionVtable;
    iCompactDelta = *(int *)((char *)(unsigned long)*(int *)((char *)this + 0x40) + 4);
    pCompactHelper = (VSGDI_CompactResourceGeometryHelperFields *)((char *)this + iCompactDelta);
    pCompactHelper->m_pVtable = &g_VSGDI_CompactResourceGeometryHelperConstructionVtable;
    ConstructHelperGroup1RowBuffer((char *)this + 0x44, 0);
    ConstructHelperGroup0RowBuffer((char *)this + 0x98, 0);
    m_nReservedEE = 0;
    m_ChildTargetLinks.m_pTail = 0;
    m_nReservedEC = 0;
    m_ChildTargetLinks.m_pHead = 0;
    m_ChildTargetLinks.m_cNodes = 0;
    *(void **)this = &g_VSGDI_ResourceGeometryHelperTargetVtable;
    *(void **)((char *)this + 0x44) = &g_VSGDI_ResourceGeometryHelperGroup1RowBufferVtable;
    *(void **)((char *)this + 0x98) = &g_VSGDI_ResourceGeometryHelperGroup0RowBufferVtable;
    pCompactHelper->m_pVtable = &g_VSGDI_CompactResourceGeometryHelperSubobjectVtable;
    pCompactHelper->m_nThisAdjust = iCompactDelta - 0x51c;
    pCompactHelper->m_pLinkedTarget = pWrappedTarget;
    pCompactHelper->m_nQueuedState = 0;

    pChildList = (VSGDI_ResourceGeometryLinkList *)((char *)pWrappedTarget + 0x528);
    pLink = (VSGDI_ResourceGeometryLinkNode *)AllocateVSMemBlock(0xc);
    if (pLink != 0) {
        pLink->m_pTarget = this;
        pLink->m_pPrev = 0;
        pLink->m_pNext = pChildList->m_pHead;
    }
    if (pChildList->m_pHead != 0)
        pChildList->m_pHead->m_pPrev = pLink;
    pChildList->m_pHead = pLink;
    if (pChildList->m_pTail == 0)
        pChildList->m_pTail = pLink;
    ++pChildList->m_cNodes;

    if (g_pResourceGeometryListHead == 0) {
        g_pResourceGeometryListHead = AllocateVSMemBlock(0xc);
        if (g_pResourceGeometryListHead != 0) {
            pGlobalList = (VSGDI_ResourceGeometryLinkList *)g_pResourceGeometryListHead;
            pGlobalList->m_pTail = 0;
            pGlobalList->m_pHead = 0;
            pGlobalList->m_cNodes = 0;
        }
    }
    pGlobalList = (VSGDI_ResourceGeometryLinkList *)g_pResourceGeometryListHead;
    pLink = (VSGDI_ResourceGeometryLinkNode *)AllocateVSMemBlock(0xc);
    if (pLink != 0) {
        pLink->m_pTarget = this;
        pLink->m_pPrev = 0;
        pLink->m_pNext = pGlobalList->m_pHead;
    }
    if (pGlobalList->m_pHead != 0)
        pGlobalList->m_pHead->m_pPrev = pLink;
    pGlobalList->m_pHead = pLink;
    if (pGlobalList->m_pTail == 0)
        pGlobalList->m_pTail = pLink;
    ++pGlobalList->m_cNodes;

    *(short *)((char *)this + iCompactDelta + 0x6c) = 1;
    *(int *)((char *)this + 0xf0) = 0;
    m_pBackingBitmap = 0;
    *(int *)((char *)this + iCompactDelta + 0x80) = 0;
    InitializeCriticalSection(m_abCriticalSection);
    m_fCriticalSectionReady = 1;

    TrackerExtent.m_x = 8;
    TrackerExtent.m_y = 8;
    TrackerExtent.m_cx = 0;
    TrackerExtent.m_cy = 0;
    CellExtent.m_x = 8;
    CellExtent.m_y = 8;
    CellExtent.m_cx = 0;
    CellExtent.m_cy = 0;
    pTracker = (VSGDI_QuantizedHelperUploadRectTracker *)AllocateVSMemBlock(0x4c);
    if (pTracker != 0) {
        pTracker->InitializeQuantizedHelperUploadRectTracker(
            pWrappedTarget == g_pResourceGeometryHelperTarget ? 0x1000 : 0,
            *(int *)&TrackerExtent,
            *(int *)&CellExtent);
    }
    *(void **)((char *)this + 0x550) = pTracker;

    if (pWrappedTarget == g_pResourceGeometryHelperTarget) {
        VSGDI_DisplayStateDispatch *pDisplayDispatch =
            (VSGDI_DisplayStateDispatch *)g_pDisplayState;
        if (pDisplayDispatch != 0) {
            (void)pDisplayDispatch->IsReady();
            m_pDisplayBinding = pDisplayDispatch->CreateDisplayBinding();
        }
    }
    ((VSGDI_HelperSurface *)this)->UpdateWorkingRectAndBacking(pRect);
    return this;
}

void *ConstructResourceGeometryHelperTarget(void *pvTarget, int nWrappedParam, int fConstructCompactHelper) {
    return ((VSGDI_ResourceGeometryHelperTargetView *)pvTarget)
        ->ConstructResourceGeometryHelperTarget(nWrappedParam, fConstructCompactHelper);
}

// FUNCTION: LEMBALL 0x0046BD10
void *VSGDI_ResourceGeometryHelperSlotManager::AllocateResourceGeometryHelperSlot(
    short *pRect, void *pWrappedTarget) {
    VSGDI_ResourceGeometryHelperSlotManager *pSlotManager;
    VSGDI_ResourceGeometryHelperSlot *pSlot;
    VSGDI_ResourceGeometryHelperTargetView *pTarget;
    int i;

    pSlotManager = this;
    i = FindFirstFreeResourceGeometryHelperSlotIndex(pSlotManager);
    if (i < 0)
        return 0;

    pSlot = pSlotManager->m_pSlots + i;
    pTarget = (VSGDI_ResourceGeometryHelperTargetView *)AllocateVSMemBlock(0x5a0);
    if (pTarget != 0) {
        pTarget->InitializeResourceGeometryHelper((const VSGDI_Rect *)pRect, pWrappedTarget, 1);
    }
    pSlot->m_nOwnerOrState0 = (int)(unsigned long)pTarget;
    pSlot->m_nOwnerOrState4 = (int)(unsigned long)pWrappedTarget;
    pSlot->m_nOwnerOrState8 = 0;
    pSlot->m_nOwnerOrStateC = 0;
    pSlot->m_nOwnerOrState10 = pWrappedTarget == g_pResourceGeometryHelperTarget;
    pSlot->m_fAvailable = 0;
    pSlot->m_nOwnerOrState18 = 0;
    if (pSlot->m_nOwnerOrState10 != 0)
        ++g_nResourceGeometryHelperSlotCount;
    return pTarget;
}

// FUNCTION: LEMBALL 0x00467060
VSGDI_GeometryHelperPointerArray *
VSGDI_GeometryHelperPointerArray::InitializeGeometryHelperPointerArray(
    short *pRect, int nEntryParameter, void *pWrappedTarget) {
    int i;
    int nEntryCount;
    void *pSlotManager;
    if ((int)pRect[0] * (int)pRect[1] > 1) {
        nEntryCount = nEntryParameter + 3;
        m_nEntryCount = nEntryCount;
        m_pEntries = AllocateVSMemBlock((unsigned int)nEntryCount * 4);

        pSlotManager = g_pResourceGeometryHelperSlotManager;
        m_pTarget = (VSGDI_ResourceGeometryHelperTargetView *)
            ((VSGDI_ResourceGeometryHelperSlotManager *)pSlotManager)
                ->AllocateResourceGeometryHelperSlot(pRect, pWrappedTarget);
        m_nReserved = 0;
        for (i = 0; i < m_nEntryCount; ++i) {
            ((void **)m_pEntries)[i] = 0;
        }
    }
    return this;
}

extern void LEMBALL_FASTCALL RetainRootZrleGeometryOwner(void *pOwner);

// FUNCTION: LEMBALL 0x00463C30
void LEMBALL_FASTCALL BuildGeometryHelperFromRenderRect(void *pOwner) {
    unsigned char *pbOwner;
    void *pParentOwner;
    void *pWrappedTarget;
    short aRect[4];
    short aWindowRect[4];
    short sOriginX;
    short sOriginY;
    unsigned int uOwnerFlags;
    unsigned int uWindowStyle;
    VSGDI_ResourceGeometryHelperTargetView *pTarget;
    VSGDI_GeometryHelperPointerArray *pHelper;
    typedef unsigned int (LEMBALL_FASTCALL *GetOwnerFlagsProc)(void *);
    typedef void (LEMBALL_FASTCALL *SetWindowRectProc)(void *, int, short *);

    pbOwner = (unsigned char *)pOwner;
    RetainRootZrleGeometryOwner(pOwner);

    pParentOwner = *(void **)(pbOwner + 0x20);
    if (pParentOwner == 0) {
        pWrappedTarget = g_pResourceGeometryHelperTarget;
    } else {
        pWrappedTarget = *(void **)((char *)*(void **)(pbOwner + 0x20) + 0x4c);
        pWrappedTarget = *(void **)((char *)pWrappedTarget + 0x0c);
    }

    aRect[0] = *(short *)(pbOwner + 0x08);
    aRect[1] = *(short *)(pbOwner + 0x0a);
    aRect[2] = *(short *)(pbOwner + 0x18);
    aRect[3] = *(short *)(pbOwner + 0x1a);

    if ((short)*(short *)(pbOwner + 0x10) *
            (short)*(short *)(pbOwner + 0x12) != 0) {
        aRect[0] = *(short *)(pbOwner + 0x10);
        aRect[1] = *(short *)(pbOwner + 0x12);
        aRect[2] = *(short *)(pbOwner + 0x14) + *(short *)(pbOwner + 0x18);
        aRect[3] = *(short *)(pbOwner + 0x16) + *(short *)(pbOwner + 0x1a);
    }
    if (pParentOwner == 0) {
        aRect[0] = (short)((int)aRect[0] / *(int *)(pbOwner + 0x38));
        aRect[1] = (short)((int)aRect[1] / *(int *)(pbOwner + 0x38));
    }

    pHelper = (VSGDI_GeometryHelperPointerArray *)AllocateVSMemBlock(sizeof(*pHelper));
    if (pHelper == 0) {
        *(void **)(pbOwner + 0x4c) = 0;
        return;
    }
    pHelper = pHelper->InitializeGeometryHelperPointerArray(
        aRect, *(int *)(pbOwner + 0x50), pWrappedTarget);
    *(VSGDI_GeometryHelperPointerArray **)(pbOwner + 0x4c) = pHelper;

    pTarget = pHelper->m_pTarget;
    if (pParentOwner == 0) {
        *(short *)((char *)pTarget +
                   *(int *)(*(int *)((char *)pTarget + 0x40) + 4) + 0x6c) =
            (short)*(int *)(pbOwner + 0x38);
    }

    sOriginX = *(short *)(pbOwner + 0x14);
    sOriginY = *(short *)(pbOwner + 0x16);
    if (pParentOwner != 0) {
        sOriginX = (short)(sOriginX - *(short *)((char *)pParentOwner + 0x18));
        sOriginY = (short)(sOriginY - *(short *)((char *)pParentOwner + 0x1a));
    }
    *(short *)((char *)pTarget +
               *(int *)(*(int *)((char *)pTarget + 0x40) + 4) + 0x5c) = sOriginX;
    *(short *)((char *)pTarget +
               *(int *)(*(int *)((char *)pTarget + 0x40) + 4) + 0x5e) = sOriginY;

    uOwnerFlags = ((GetOwnerFlagsProc)(*(void ***)pOwner)[0x64 / sizeof(void *)])(
        pOwner);
    if ((uOwnerFlags & 0x40000000u) != 0 && *(HWND *)(pbOwner + 0x44) != 0) {
        uWindowStyle = GetWindowLongA(*(HWND *)(pbOwner + 0x44), -16);
        if ((uWindowStyle & 0x40000000u) != 0) {
            aWindowRect[0] = *(short *)(pbOwner + 0x14) -
                             *(short *)((char *)*(void **)(pbOwner + 0x48) + 0x18);
            aWindowRect[1] = *(short *)(pbOwner + 0x16) -
                             *(short *)((char *)*(void **)(pbOwner + 0x48) + 0x1a);
            aWindowRect[2] = aRect[2];
            aWindowRect[3] = aRect[3];
            *(short *)((char *)pTarget + 0xec) = aWindowRect[0];
            *(short *)((char *)pTarget + 0xee) = aWindowRect[1];
            ((SetWindowRectProc)(*(void ***)*(void **)(pbOwner + 0x48))[0x84 / sizeof(void *)])(
                *(void **)(pbOwner + 0x48), 0, aWindowRect);
        }
    }
    *(int *)((char *)pTarget + 0x10) =
        *(int *)g_pSelectedGraphicsDriverRuntime != 3;
}

// FUNCTION: LEMBALL 0x004722D0
void LEMBALL_FASTCALL DestroyResourceGeometryRowBuffer(void *pvRowBuffer) {
    *(void **)pvRowBuffer = &g_VSGDI_ResourceGeometryRowBufferVtable;
    ReleaseResourceGeometryRowBufferStorage((int)(unsigned long)pvRowBuffer);
}

// FUNCTION: LEMBALL 0x004664B0
void LEMBALL_FASTCALL DestroyHelperGroup0RowBuffer(void *pvHelperGroup) {
    int iCompactDelta;
    int *pHelperGroup;
    int *pRowBuffer;

    pHelperGroup = (int *)pvHelperGroup;
    *(void **)pHelperGroup = &g_VSGDI_ResourceGeometryHelperGroup0Vtable;
    pRowBuffer = pHelperGroup + 2;
    iCompactDelta = *(int *)((char *)(unsigned long)pHelperGroup[1] + 4);
    *(void **)((char *)pRowBuffer + iCompactDelta - 4) = (void *)&g_VSGDI_ResourceGeometryHelperGroup0CompactSubobjectVtable;
    *(int *)((char *)pRowBuffer + iCompactDelta - 8) = iCompactDelta - 0x54;
    DestroyResourceGeometryRowBuffer(pRowBuffer);
}

// FUNCTION: LEMBALL 0x004666E0
void LEMBALL_FASTCALL DestroyHelperGroup1RowBuffer(void *pvHelperGroup) {
    int *pHelperGroup;

    pHelperGroup = (int *)pvHelperGroup;
    *(void **)pHelperGroup = &g_VSGDI_ResourceGeometryHelperGroup1Vtable;
    pHelperGroup += 2;
    *(void **)((char *)pHelperGroup +
               *(int *)((char *)(unsigned long)pHelperGroup[-1] + 4) - 4) =
        (void *)&g_VSGDI_ResourceGeometryHelperGroup1CompactSubobjectVtable;
    *(int *)((char *)pHelperGroup +
             *(int *)((char *)(unsigned long)pHelperGroup[-1] + 4) - 8) =
        *(int *)((char *)(unsigned long)pHelperGroup[-1] + 4) - 0x54;
    DestroyResourceGeometryRowBuffer(pHelperGroup);
}

// FUNCTION: LEMBALL 0x0046C710
void DestroyResourceGeometryHelperTarget(void *pvTarget) {
    int iCompactDelta;
    int iCriticalSectionReady;
    unsigned int pFollowingNode;
    VSGDI_CompactResourceGeometryHelperFields *pCompactHelper;
    VSGDI_ResourceGeometryLinkList *pGlobalList;
    VSGDI_ResourceGeometryLinkList *pLinkedTargetList;
    VSGDI_ResourceGeometryLinkNode *pLink;
    VSGDI_ResourceGeometryLinkNode *pNextNode;
    VSGDI_ResourceGeometryLinkNode *pPreviousLink;
    VSGDI_ResourceGeometryHelperTargetView *pLinkedTarget;
    VSGDI_ResourceGeometryHelperTargetView *pTarget;

    pTarget = (VSGDI_ResourceGeometryHelperTargetView *)pvTarget;
    *(void **)pTarget = &g_VSGDI_ResourceGeometryHelperTargetVtable;
    *(void **)((char *)pTarget + 0x44) = &g_VSGDI_ResourceGeometryHelperGroup1RowBufferVtable;
    *(void **)((char *)pTarget + 0x98) = &g_VSGDI_ResourceGeometryHelperGroup0RowBufferVtable;
    iCompactDelta = *(int *)((char *)(unsigned long)*(int *)((char *)pTarget + 0x40) + 4);
    pCompactHelper = (VSGDI_CompactResourceGeometryHelperFields *)((char *)pTarget + iCompactDelta);
    pCompactHelper->m_pVtable = &g_VSGDI_CompactResourceGeometryHelperSubobjectVtable;
    pCompactHelper->m_nThisAdjust = iCompactDelta - 0x51c;

    iCriticalSectionReady = pTarget->m_fCriticalSectionReady;
    if (iCriticalSectionReady != 0) {
        EnterCriticalSection(pTarget->m_abCriticalSection);
    }
    if (pTarget->m_pBackingBitmap != 0 && g_pDisplayState != 0) {
        ((void (*)(void *, int, int))(*(void ***)g_pDisplayState)[0x28 / sizeof(void *)])(
            g_pDisplayState, (int)(unsigned long)pTarget->m_pDisplayBinding, (int)(unsigned long)pTarget->m_pBackingBitmap);
        ((void (*)(void *, int))(*(void ***)g_pDisplayState)[0x14 / sizeof(void *)])(
            g_pDisplayState, (int)(unsigned long)pTarget->m_pBackingBitmap);
        pTarget->m_pBackingBitmap = 0;
    }
    if (pCompactHelper->m_pLinkedTarget == g_pResourceGeometryHelperTarget) {
        ReleaseHelperGroup0BackingBuffer((int)(unsigned long)((char *)pTarget + 0x98));
        ReleaseHelperGroup1BackingBuffer((int)(unsigned long)((char *)pTarget + 0x44));
    }
    if (pTarget->m_pDisplayBinding != 0 && g_pDisplayState != 0 &&
        (*(void ***)g_pDisplayState)[2] != 0) {
        ((void (*)(void *, int))(*(void ***)g_pDisplayState)[8 / sizeof(void *)])(
            g_pDisplayState, (int)(unsigned long)pTarget->m_pDisplayBinding);
        pTarget->m_pDisplayBinding = 0;
    }
    if (pTarget->m_nActiveUploadState != 0) {
        ResetHelperUploadRectListAndStateMap(pTarget->m_nActiveUploadState);
        FreeVSMemBlock((void *)(unsigned long)pTarget->m_nActiveUploadState);
        pTarget->m_nActiveUploadState = 0;
    }
    pLinkedTarget = (VSGDI_ResourceGeometryHelperTargetView *)pCompactHelper->m_pLinkedTarget;
    if (pLinkedTarget != 0) {
        pLinkedTargetList = &pLinkedTarget->m_ChildTargetLinks;
        pLink = pLinkedTargetList->m_pTail;
        while (pLink != 0) {
            if (pLink->m_pTarget == pvTarget) {
                break;
            }
            pLink = pLink->m_pPrev;
        }
        if (pLink != 0) {
            pPreviousLink = pLink->m_pPrev;
            pNextNode = pLink->m_pNext;
            FreeVSMemBlock(pLink);
            if (pPreviousLink == 0) {
                pLinkedTargetList->m_pHead = pNextNode;
            } else {
                pPreviousLink->m_pNext = pNextNode;
            }
            if (pNextNode == 0) {
                pLinkedTargetList->m_pTail = pPreviousLink;
            } else {
                pNextNode->m_pPrev = pPreviousLink;
            }
            pLinkedTargetList->m_cNodes = pLinkedTargetList->m_cNodes - 1;
        }
        pCompactHelper->m_pLinkedTarget = 0;
    }
    if (iCriticalSectionReady != 0) {
        LeaveCriticalSection(pTarget->m_abCriticalSection);
        DeleteCriticalSection(pTarget->m_abCriticalSection);
        pTarget->m_fCriticalSectionReady = 0;
        pGlobalList = (VSGDI_ResourceGeometryLinkList *)g_pResourceGeometryListHead;
        if (pGlobalList != 0) {
            pLink = pGlobalList->m_pTail;
            while (pLink != 0) {
                if (pLink->m_pTarget == pvTarget) {
                    break;
                }
                pLink = pLink->m_pPrev;
            }
            if (pLink != 0) {
                pPreviousLink = pLink->m_pPrev;
                pNextNode = pLink->m_pNext;
                FreeVSMemBlock(pLink);
                if (pPreviousLink == 0) {
                    pGlobalList->m_pHead = pNextNode;
                } else {
                    pPreviousLink->m_pNext = pNextNode;
                }
                if (pNextNode == 0) {
                    pGlobalList->m_pTail = pPreviousLink;
                } else {
                    pNextNode->m_pPrev = pPreviousLink;
                }
                pGlobalList->m_cNodes = pGlobalList->m_cNodes - 1;
            }
            if (pGlobalList->m_cNodes == 0) {
                pNextNode = pGlobalList->m_pTail;
                while (pNextNode != 0) {
                    pFollowingNode = (unsigned int)(unsigned long)pNextNode->m_pPrev;
                    FreeVSMemBlock((void *)(unsigned long)pNextNode);
                    pNextNode = (VSGDI_ResourceGeometryLinkNode *)(unsigned long)pFollowingNode;
                }
                FreeVSMemBlock(pGlobalList);
                g_pResourceGeometryListHead = 0;
            }
        }
    }
    pNextNode = pTarget->m_ChildTargetLinks.m_pTail;
    if (pNextNode != 0) {
        do {
            pFollowingNode = (unsigned int)(unsigned long)pNextNode->m_pPrev;
            FreeVSMemBlock((void *)(unsigned long)pNextNode);
            pNextNode = (VSGDI_ResourceGeometryLinkNode *)(unsigned long)pFollowingNode;
        } while (pNextNode != 0);
    }
    DestroyHelperGroup0RowBuffer((char *)pTarget + 0x98);
    DestroyHelperGroup1RowBuffer((char *)pTarget + 0x44);
    DestroyResourceGeometryRowBuffer(pTarget);
}

// FUNCTION: LEMBALL 0x00466430
static void LEMBALL_FASTCALL RestoreCompactResourceGeometryHelperVtable(void *pvHelper) {
    *(void **)pvHelper = g_VSGDI_CompactResourceGeometryHelperSubobjectVtable;
}

// FUNCTION: LEMBALL 0x0046AFD0
void LEMBALL_FASTCALL InitializeStatusIndicatorRenderClientDefaults(int *pRenderClient) {
    int *pEntryBlock;
    int *pEntry;
    int i;
    int nZero;
    DWORD dwNow;

    nZero = 0;
    pRenderClient[0xa] = nZero;
    pEntryBlock = (int *)AllocateVSMemBlock(0x1c);
    if (pEntryBlock == 0) {
        pRenderClient[7] = 0;
    } else {
        pEntryBlock[0] = 1;
        pEntry = pEntryBlock + 1;
        for (i = 0; i <= 1; ++i) {
            InitializePackagedRectQueueEntry(pEntry);
            pEntry += 6;
        }
        pRenderClient[7] = (int)(unsigned long)(pEntryBlock + 1);
    }
    pEntry = (int *)(unsigned long)pRenderClient[7];
    *(short *)((char *)pEntry + 4) = 0;
    *(short *)((char *)pEntry + 6) = 0;
    pEntry[2] = 0;
    pEntry[3] = 0;
    pEntry[4] = 0;
    ((GAME_RenderDispatchQueue *)g_pSharedRenderDispatchQueue)
        ->RegisterOrderedRenderDispatchClient(pRenderClient, -0x19);
    pRenderClient[0xd] = 0;
    pRenderClient[0xc] = 0;
    pRenderClient[0xb] = 0;
    pRenderClient[6] = 0;
    pRenderClient[0xe] = 0;
    pRenderClient[0xf] = 1;
    pRenderClient[8] = 0;
    pRenderClient[0x12] = 1;
    pRenderClient[0x1f] = 3;
    pRenderClient[0x20] = 4;
    pRenderClient[0x21] = 1;
    pRenderClient[0x24] = 0;
    pRenderClient[0x22] = 2;
    pRenderClient[0x23] = 0x1f;
    pRenderClient[0x25] = 0x49;
    pRenderClient[0x13] = 0x199;
    pRenderClient[0x14] = 0x8000;
    pRenderClient[0x17] = 0;
    pRenderClient[0x15] = (int)*(short *)((char *)pRenderClient + 0x10) << 12;
    pRenderClient[0x18] = 0;
    pRenderClient[0x19] = 0;
    pRenderClient[0x1a] = 0;
    pRenderClient[0x16] = (int)*(short *)((char *)pRenderClient + 0x12) << 12;
    dwNow = timeGetTime();
    pRenderClient[0x1b] = (int)dwNow;
    pRenderClient[0x1c] = (int)dwNow;
}

// FUNCTION: LEMBALL 0x0046AEC0
void *LEMBALL_FASTCALL ConstructStatusIndicatorRenderClientBase(int *pRenderClient) {
    InitializeRenderQueueNodeBase(pRenderClient);
    *(void **)((char *)pRenderClient + 0x40) = &g_VSGDI_StatusIndicatorRenderClientVtable;
    *(void **)((char *)pRenderClient + 0x44) = &g_VSGDI_StatusIndicatorPointSinkVtable;
    *(short *)((char *)pRenderClient + 0x12) = 0;
    pRenderClient[0x12] = 0;
    *(short *)((char *)pRenderClient + 0x10) = 0;
    pRenderClient[0x13] = 0xaa55aa55;
    *(short *)((char *)pRenderClient + 0x16) = 0;
    pRenderClient[0x14] = 0xaa55aa55;
    *(short *)((char *)pRenderClient + 0x14) = 0;
    pRenderClient[0x15] = 0xaa55aa55;
    pRenderClient[0x16] = 0xaa55aa55;
    pRenderClient[0x17] = 0xaa55aa55;
    *(short *)((char *)pRenderClient + 0x76) = 0;
    pRenderClient[0x18] = 0xaa55aa55;
    *(short *)((char *)pRenderClient + 0x74) = 0;
    pRenderClient[0x19] = 0xaa55aa55;
    *(short *)((char *)pRenderClient + 0x7a) = 0;
    pRenderClient[0x1a] = 0xaa55aa55;
    *(short *)((char *)pRenderClient + 0x78) = 0;
    *(void **)pRenderClient = &g_VSGDI_StatusIndicatorBaseVtable;
    InitializeStatusIndicatorRenderClientDefaults(pRenderClient);
    return pRenderClient;
}

// FUNCTION: LEMBALL 0x00474B50
void LEMBALL_FASTCALL InitializeArrowCursorState(int *pState) {
    struct CursorState {
        virtual void Reserved00();
        virtual void Reserved01();
        virtual void Reserved02();
        virtual void Reserved03();
        virtual void Reserved04();
        virtual void Reserved05();
        virtual void InitializeCursor();
    };

    pState[0x27] = 0;
    pState[0x27] = (int)(unsigned long)LoadCursorA((HINSTANCE)0, (LPCSTR)0x7f00);
    ((CursorState *)pState)->InitializeCursor();
}

// FUNCTION: LEMBALL 0x00472400
void LEMBALL_FASTCALL RebuildResourceGeometryRowPointerTable(VSGDI_HelperSurface *pSurface) {
    int nRowByteOffset;
    int iRowIndex;
    int iLogicalRow;

    nRowByteOffset = *(int *)((char *)pSurface + 8);
    iLogicalRow = *(int *)((char *)pSurface + 0x14);
    iRowIndex = 0;
    if (0 < *(short *)((char *)pSurface + 0x2e)) {
        do {
            *(int *)(*(int *)((char *)pSurface + 4) + iLogicalRow * 4) =
                *(int *)((char *)pSurface + 0x18) + nRowByteOffset;
            ++iLogicalRow;
            nRowByteOffset = nRowByteOffset + *(int *)((char *)pSurface + 0x1c);
            if ((int)*(short *)((char *)pSurface + 0x2e) <= iLogicalRow) {
                iLogicalRow = iLogicalRow - (int)*(short *)((char *)pSurface + 0x2e);
            }
            ++iRowIndex;
        } while (iRowIndex < (int)*(short *)((char *)pSurface + 0x2e));
    }
}

// FUNCTION: LEMBALL 0x004723A0
void VSGDI_HelperSurface::ClearBackingBorderRows(void) {
    unsigned int cbBorderRow;
    unsigned int cDwords;
    unsigned int cTailBytes;
    char *pbBackingBase;
    char *pbTarget;
    int nAbsoluteStride;
    int nStrideSign;

    *(int *)((char *)this + 0x18) = 0;
    *(int *)((char *)this + 0x14) = 0;
    RebuildResourceGeometryRowPointerTable(this);

    cbBorderRow = (unsigned int)*(int *)((char *)this + 0x20);
    if (0 < (int)cbBorderRow) {
        pbBackingBase = (char *)(unsigned long)*(int *)((char *)this + 0xc);
        pbTarget = pbBackingBase;
        cDwords = cbBorderRow >> 2;
        while (cDwords != 0) {
            *(unsigned int *)pbTarget = 0;
            pbTarget = pbTarget + 4;
            --cDwords;
        }
        cTailBytes = cbBorderRow & 3;
        while (cTailBytes != 0) {
            *pbTarget = 0;
            ++pbTarget;
            --cTailBytes;
        }

        nAbsoluteStride = *(int *)((char *)this + 0x1c);
        nStrideSign = nAbsoluteStride >> 0x1f;
        nAbsoluteStride = (nAbsoluteStride ^ nStrideSign) - nStrideSign;
        pbTarget = pbBackingBase + nAbsoluteStride * (int)*(short *)((char *)this + 0x2e) + cbBorderRow;
        cDwords = cbBorderRow >> 2;
        while (cDwords != 0) {
            *(unsigned int *)pbTarget = 0;
            pbTarget = pbTarget + 4;
            --cDwords;
        }
        cTailBytes = cbBorderRow & 3;
        while (cTailBytes != 0) {
            *pbTarget = 0;
            ++pbTarget;
            --cTailBytes;
        }
    }
}

// FUNCTION: LEMBALL 0x00472760
void VSGDI_HelperSurface::ClearBackingBorderRowsThunk(void) {
    ClearBackingBorderRows();
}

// FUNCTION: LEMBALL 0x00472340
void LEMBALL_FASTCALL EnsureHelperBackingRowIndexCapacity(VSGDI_HelperSurface *pSurface) {
    int cRows;
    int cbRowIndexBytes;

    cRows = (int)*(short *)((char *)pSurface + 0x2e);
    if (*(int *)((char *)pSurface + 0x28) < cRows) {
        if (*(int *)((char *)pSurface + 4) != 0) {
            FreeVSMemBlock((void *)(unsigned long)*(int *)((char *)pSurface + 4));
            *(int *)((char *)pSurface + 4) = 0;
        }
        if (0 < *(short *)((char *)pSurface + 0x2e)) {
            cbRowIndexBytes = cRows << 2;
            *(int *)((char *)pSurface + 4) = (int)(unsigned long)AllocateVSMemBlock((unsigned int)cbRowIndexBytes);
            pSurface->ClearBackingBorderRows();
        }
        *(int *)((char *)pSurface + 0x28) = cRows;
        return;
    }
    pSurface->ClearBackingBorderRows();
}

// FUNCTION: LEMBALL 0x004725F0
void VSGDI_HelperSurface::ComputeBackingDimensions(short *paOut, short *paRect, int nWidth) {
    VSGDI_HelperSurface *pSurface = this;
    short cxRect;
    short cyRect;
    int cBorderRows;

    cxRect = paRect[0];
    *(short *)((char *)pSurface + 0x2c) = cxRect;
    cyRect = paRect[1];
    *(short *)((char *)pSurface + 0x2e) = cyRect;
    if (nWidth == 0 || *(int *)((char *)pSurface + 0x10) != 0) {
        *(int *)((char *)pSurface + 0x20) = 0;
    } else {
        *(int *)((char *)pSurface + 0x20) = nWidth - cxRect;
    }
    if (cxRect == 0) {
        *(int *)((char *)pSurface + 0x24) = 0;
    } else {
        *(int *)((char *)pSurface + 0x24) =
            (cxRect - 1 + *(int *)((char *)pSurface + 0x20) * 2) / (int)cxRect;
    }
    cBorderRows = *(int *)((char *)pSurface + 0x24);
    paOut[0] = cxRect;
    paOut[1] = (short)cBorderRows + cyRect;
}

// FUNCTION: LEMBALL 0x00472670
void VSGDI_HelperSurface::ConfigureBackingStrideAndOrigin(int nStride, int nOrigin) {
    VSGDI_HelperSurface *pSurface = this;
    int nRowBase;

    nRowBase = *(int *)((char *)pSurface + 0x20) + nStride;
    *(int *)((char *)pSurface + 0xc) = nStride;
    *(int *)((char *)pSurface + 8) = nRowBase;
    *(int *)((char *)pSurface + 0x1c) = nOrigin;
    if (nOrigin < 0) {
        *(int *)((char *)pSurface + 8) =
            (1 - (int)*(short *)((char *)pSurface + 0x2e)) * nOrigin + nRowBase;
    }
    EnsureHelperBackingRowIndexCapacity(pSurface);
}

// FUNCTION: LEMBALL 0x00466D10
void LEMBALL_FASTCALL PromoteHelperUploadStateToActive(int nUploadState) {
    if (*(char *)(unsigned long)(nUploadState + 0x48) == 'P') {
        *(char *)(unsigned long)(nUploadState + 0x48) = 'A';
        return;
    }
    if (*(char *)(unsigned long)(nUploadState + 0x48) != 'p') {
        return;
    }
    *(char *)(unsigned long)(nUploadState + 0x48) = 'a';
}

// FUNCTION: LEMBALL 0x00466B60
void LEMBALL_FASTCALL InitializeHelperUploadStatePending(int nUploadState) {
    *(unsigned short *)(unsigned long)(nUploadState + 0x30) = 0;
    *(int *)(unsigned long)(nUploadState + 0x2c) = 0;
    *(char *)(unsigned long)(nUploadState + 0x48) = 'P';
    *(unsigned short *)(unsigned long)(nUploadState + 0x32) = 0;
    *(unsigned short *)(unsigned long)(nUploadState + 0x34) = 0;
    *(unsigned short *)(unsigned long)(nUploadState + 0x36) = 0;
    *(int *)(unsigned long)(nUploadState + 4) = -1;
    *(int *)(unsigned long)(nUploadState + 8) = -1;
}

// FUNCTION: LEMBALL 0x004670F0
void AppendPointerQueueEntry(void *pQueue, void *pEntry) {
    VSGDI_HelperDispatchQueue *pDispatchQueue;

    pDispatchQueue = (VSGDI_HelperDispatchQueue *)pQueue;
    pDispatchQueue->m_ppEntries[pDispatchQueue->m_cEntries] = pEntry;
    pDispatchQueue->m_cEntries = pDispatchQueue->m_cEntries + 1;
}

// FUNCTION: LEMBALL 0x00432380
void QueueQueuedRenderPointSink(void *pPointSink, void *pQueue) {
    AppendPointerQueueEntry(pQueue, pPointSink);
}

// FUNCTION: LEMBALL 0x00432390
void __stdcall PromoteQueuedRenderPointSinkTargetState(void *pPointSink) {
    int nUploadState;
    struct TargetState {
        virtual void Reserved0();
        virtual void Reserved1();
        virtual int GetUploadState();
    };

    nUploadState = ((TargetState *)*(void ***)((char *)pPointSink + 0x0c))->GetUploadState();
    PromoteHelperUploadStateToActive(nUploadState);
}

// FUNCTION: LEMBALL 0x0040381E
void QueueQueuedRenderPointSinkThunk(void *pPointSink, void *pQueue) {
    QueueQueuedRenderPointSink(pPointSink, pQueue);
}

// FUNCTION: LEMBALL 0x00402FEA
void PromoteQueuedRenderPointSinkTargetStateThunk(void *pPointSink) {
    PromoteQueuedRenderPointSinkTargetState(pPointSink);
}

// FUNCTION: LEMBALL 0x0046D9F0
int RefreshHelperTargetBindingIfChanged(void *pTarget) {
    int *pnCachedBindingValue;
    VSGDI_HelperSurfaceVariableBlock *pVariableBlock;
    void *pBindingSource;
    VSGDI_HelperSurface *pLinkedTarget;
    int *pBackingRow;
    int nBindingValue;

    if (*(int *)((char *)pTarget + 4) == 0) {
        return 0;
    }

    pVariableBlock =
        (VSGDI_HelperSurfaceVariableBlock *)((char *)pTarget + GetHelperSurfaceVariableBlockOffset((const VSGDI_HelperSurface *)pTarget));
    pnCachedBindingValue = (int *)((char *)pTarget + 0xc);
    pLinkedTarget = (VSGDI_HelperSurface *)pVariableBlock->m_pLinkedTarget;
    if ((void *)pLinkedTarget == g_pResourceGeometryHelperTarget) {
        pBindingSource = *(void **)((char *)pTarget + 0xf0);
        if (pBindingSource == 0) {
            return 0;
        }
        if (((int (*)())(*(void ***)pBindingSource)[1])() == 0) {
            return 0;
        }
        nBindingValue = ((int (*)(void))(*(void ***)pBindingSource)[3])();
        if (nBindingValue != 0 && *pnCachedBindingValue != nBindingValue) {
            *pnCachedBindingValue = nBindingValue;
            EnsureHelperBackingRowIndexCapacity((VSGDI_HelperSurface *)pTarget);
            return 1;
        }
    } else {
        if (pLinkedTarget == 0) {
            return 0;
        }
        if (RefreshHelperTargetBindingIfChanged(pLinkedTarget) == 0) {
            return 0;
        }
        pBackingRow = *(int **)(unsigned long)(*(int *)((char *)pLinkedTarget + 4) + pVariableBlock->m_aWorkingRect[3] * 4);
        nBindingValue = *(int *)((char *)pBackingRow + pVariableBlock->m_aWorkingRect[2]);
        if (nBindingValue != *pnCachedBindingValue) {
            EnsureHelperBackingRowIndexCapacity((VSGDI_HelperSurface *)pTarget);
            return 1;
        }
    }
    return 1;
}

// FUNCTION: LEMBALL 0x0046DAA0
void InvokeRootHelperTargetPostQueueCallback(int nTarget) {
    void *pBindingSource;
    VSGDI_HelperSurfaceVariableBlock *pVariableBlock;
    VSGDI_HelperSurface *pRootTarget;
    VSGDI_HelperSurface *pTarget;

    pTarget = (VSGDI_HelperSurface *)(unsigned long)nTarget;
    do {
        pRootTarget = pTarget;
        pVariableBlock =
            (VSGDI_HelperSurfaceVariableBlock *)((char *)pRootTarget + GetHelperSurfaceVariableBlockOffset(pRootTarget));
        pTarget = (VSGDI_HelperSurface *)pVariableBlock->m_pLinkedTarget;
    } while (g_pResourceGeometryHelperTarget != (void *)pTarget);

    pBindingSource = *(void **)((char *)pRootTarget + 0xf0);
    ((void (*)())(*(void ***)pBindingSource)[2])();
}

// FUNCTION: LEMBALL 0x00467110
void DispatchAndClearPointerQueue(void *pQueue) {
    void *pQueuedEntry;
    VSGDI_HelperDispatchQueue *pDispatchQueue;
    int nByteOffset;
    int i;

    pDispatchQueue = (VSGDI_HelperDispatchQueue *)pQueue;
    if (RefreshHelperTargetBindingIfChanged(pDispatchQueue->m_pHelperTarget) != 0) {
        nByteOffset = 0;
        i = 0;
        if (0 < pDispatchQueue->m_cEntries) {
            do {
                pQueuedEntry = *(void **)((char *)pDispatchQueue->m_ppEntries + nByteOffset);
                if (IsPointerInsideManagedMemoryRegions(pQueuedEntry) != 0) {
                    ((void (*)(void *))(*(void ***)pQueuedEntry)[2])(pDispatchQueue);
                }
                nByteOffset += 4;
                ++i;
            } while (i < pDispatchQueue->m_cEntries);
        }
        InvokeRootHelperTargetPostQueueCallback((int)(unsigned long)pDispatchQueue->m_pHelperTarget);
    }
}

// FUNCTION: LEMBALL 0x00465AA0
void GAME_PrimaryContext::SampleRootHelperGeometryAndDispatchRenderGroups(int nToken) {
    int *pContext;
    VSGDI_HelperDispatchQueue *pDispatchQueue;
    VSGDI_HelperSurfaceBindingSurface *pBindingSurface;
    VSGDI_HelperSurface *pHelperTarget;
    void *pQueuedPointSink;
    void *pUploadCallbackTarget;
    int nUploadState;
    int nResult;
    short x1;
    short y1;
    short y2;

    pContext = (int *)this;
    if (g_fRootHelperGeometryDispatchSuppressed != 0 || pContext[1] != 1 || pContext[0x13] == 0) {
        return;
    }

    if (nToken == -1) {
        nToken = 0;
    }

    pDispatchQueue = (VSGDI_HelperDispatchQueue *)(unsigned long)pContext[0x13];
    pHelperTarget = pDispatchQueue->m_pHelperTarget;
    pBindingSurface =
        (VSGDI_HelperSurfaceBindingSurface *)((char *)pHelperTarget + GetHelperSurfaceVariableBlockOffset(pHelperTarget) + 0x40);
    ((void (*)())pBindingSurface->m_pVtable[0xe])();

    x1 = pBindingSurface->m_nOriginX;
    y1 = pBindingSurface->m_nOriginY;

    pQueuedPointSink = AllocateVSMemBlock(4);
    if (pQueuedPointSink != 0) {
        *(void **)pQueuedPointSink = g_pQueuedRenderPointSinkFinalizeThunk;
    }

    nUploadState = ((int (*)(void))(*(void ***)pHelperTarget)[2])();
    InitializeHelperUploadStatePending(nUploadState);
    ((void (*)(void *, void *))(*(void ***)pQueuedPointSink)[1])(
        pQueuedPointSink, pDispatchQueue);

    *(short *)((char *)pContext + 0x5c) = *(short *)&nUploadState;
    *(short *)((char *)pContext + 0x5e) = *((short *)&nUploadState + 1);
    *(short *)((char *)pContext + 0x60) = y1;
    *(short *)((char *)pContext + 0x62) = x1;
    pContext[0x19] = nToken;
    pUploadCallbackTarget = (void *)(unsigned long)pContext[0x16];
    ((void (*)(void *))(*(void ***)pUploadCallbackTarget)[1])(pDispatchQueue);

    nResult = ((int (*)())pBindingSurface->m_pVtable[0xf])();
    y2 = *((short *)&nResult + 1);
    if (nResult != 0) {
        *(short *)((char *)pContext + 0x6c) = 0;
        *(short *)((char *)pContext + 0x6e) = 0;
        *(short *)((char *)pContext + 0x70) = (short)nResult;
        *(short *)((char *)pContext + 0x72) = y2;
        *(short *)((char *)pContext + 0x74) = *(short *)&nUploadState;
        *(short *)((char *)pContext + 0x76) = *((short *)&nUploadState + 1);
        pUploadCallbackTarget = (void *)(unsigned long)pContext[0x1a];
        ((void (*)(void *))(*(void ***)pUploadCallbackTarget)[1])(pDispatchQueue);
    }

    nResult = ((int (*)())pBindingSurface->m_pVtable[0x10])();
    if (nResult != 0) {
        *(short *)((char *)pContext + 0x80) = (short)nResult;
        *(short *)((char *)pContext + 0x82) = *((short *)&nResult + 1);
        *(short *)((char *)pContext + 0x84) = *(short *)&nUploadState;
        *(short *)((char *)pContext + 0x86) = *((short *)&nUploadState + 1);
        pContext[0x1f] = 0;
        pUploadCallbackTarget = (void *)(unsigned long)pContext[0x1e];
        ((void (*)(void *))(*(void ***)pUploadCallbackTarget)[1])(pDispatchQueue);
    }

    DispatchAndClearPointerQueue(pDispatchQueue);
    pDispatchQueue->m_cEntries = 0;
    ((void (*)())(*(void ***)pContext)[0x2d])();

    if (pQueuedPointSink != 0) {
        ((void (*)(void *, int))(*(void ***)pQueuedPointSink)[0])(pQueuedPointSink, 1);
    }
}

static int GetHelperSurfaceVariableBlockOffset(const VSGDI_HelperSurface *pSurface) {
    return *(const int *)((const char *)(unsigned long)*(const int *)((const char *)pSurface + 0x40) + 4);
}

// FUNCTION: LEMBALL 0x0046CB20
void VSGDI_ResourceGeometryHelperTargetView::MarkOrForwardHelperTargetDirtyRect(short *pRect) {
    char *pTarget;
    char *pCompactHelper;
    short *pOrigin;
    short *pTail;
    short aForwardedRect[4];
    VSGDI_ResourceGeometryHelperTargetView *pLinkedTarget;

    pCompactHelper = (char *)this + GetHelperSurfaceVariableBlockOffset((VSGDI_HelperSurface *)this);
    pLinkedTarget = *(VSGDI_ResourceGeometryHelperTargetView **)(pCompactHelper + 0x60);
    if (pLinkedTarget != g_pResourceGeometryHelperTarget &&
        *(int *)(pCompactHelper + 0x74) != 0 &&
        *(int *)(pCompactHelper + 0x70) != 0) {
        pTarget = (char *)this + 0x50 + GetHelperSurfaceVariableBlockOffset((VSGDI_HelperSurface *)this);
        pOrigin = (short *)pTarget;
        aForwardedRect[0] = pRect[0];
        aForwardedRect[1] = pRect[1];
        pTail = pRect + 2;
        aForwardedRect[2] = pTail[0] + pOrigin[0];
        aForwardedRect[3] = pTail[1] + pOrigin[1];
        ((VSGDI_ResourceGeometryHelperTargetView *)pLinkedTarget)
            ->MarkOrForwardHelperTargetDirtyRect(aForwardedRect);
        return;
    }

    ((VSGDI_QuantizedHelperUploadRectTracker *)((char *)this + 0x550))
        ->MarkHelperUploadCellsForRect(pRect);
}

struct VSGDI_ResourceGeometryVtableFixup {
    VSGDI_ResourceGeometryVtableFixup(void) {
        union {
            void (VSGDI_ResourceGeometryHelperTargetView::*p)(short *);
            void *v;
        } markDirtyMethod;
        markDirtyMethod.p = &VSGDI_ResourceGeometryHelperTargetView::MarkOrForwardHelperTargetDirtyRect;
        g_VSGDI_ResourceGeometryHelperTargetVtable[1] = markDirtyMethod.v;
    }
};
static VSGDI_ResourceGeometryVtableFixup g_VSGDI_ResourceGeometryVtableFixup;

static const char *g_apszGraphicsDriverNames[] = {
    "NO",
    "CDS",
    "VGA (Full Screen 320*200)",
    "VGA (Full Screen 320*240)",
    "Direct Draw (Full Screen 640*480)",
    "Direct Draw (Full Screen 320*200)",
    "Direct Draw (Windowed 640*480)",
    "Direct Draw (Windowed 320*200)",
    "Auto Select",
};

static const char *g_apszGraphicsDriverFallbackMessages[] = {
    "None",
    "Defaulting to normal 640*480 mode (using CreateDIBSection)",
    "Unable to find DispDib32 Libraries for full screen 320*200 mode (dspdib16.dll & dspdib32.dll) - please reinstall",
    "Unable to find DispDib32 Libraries for full screen 320*240 mode (dspdib16.dll & dspdib32.dll) - please reinstall",
    "Unable to find Direct Draw libraries for full screen 640*480 mode (ddraw.dll) - please reinstall the DirectX libraries",
    "Unable to find Direct Draw libraries for full screen 320*200 mode (ddraw.dll) - please reinstall the DirectX libraries",
};

static void CallDeleteVirtual(void *pObject, unsigned int nByteOffset, int nDeleteFlag) {
    void **pVtable;

    pVtable = *(void ***)pObject;
    ((void *(LEMBALL_FASTCALL *)(void *, int, int))
         pVtable[nByteOffset / sizeof(void *)])(pObject, 0, nDeleteFlag);
}

static int IsDisplayStateReady(const VSGDI_DisplayState *pDisplayState) {
    return *(const int *)((const char *)pDisplayState + 0xc) != 0;
}

struct VSGDI_DisplayBitmap {
    void **m_pVtable;
    void *m_pBits;
    void *m_pOrigin;
    void *m_pBackendBitmap;
    unsigned int m_cbBits;

    int Bits(void);
    int Origin(void);
};

struct VSGDI_DisplayBitmapDispatch {
    virtual void Reserved0(void) = 0;
    virtual void Reserved1(void) = 0;
    virtual void Reserved2(void) = 0;
    virtual int Bits(void) = 0;
    virtual int Origin(void) = 0;
};

static void *g_VSGDI_DisplayBitmapVtable[16];
static void *g_VSGDI_MetricsDisplayVtable[14];
static void *g_VSGDI_DibDisplayVtable[14];
static void *g_VSGDI_DirectDrawDisplayVtable[14];

int VSGDI_DisplayBitmap::Bits(void) {
    return (int)(unsigned long)m_pBits;
}

int VSGDI_DisplayBitmap::Origin(void) {
    return 0;
}

static void *LEMBALL_FASTCALL VSGDI_DeleteDisplayState(
    void *pDisplayState, int nUnused, int fDelete) {
    (void)nUnused;
    if (fDelete != 0)
        FreeVSMemBlock(pDisplayState);
    return pDisplayState;
}

void *VSGDI_DisplayState::CreateDisplayBinding(void) {
    VSGDI_HelperTargetParamWrapper *pBinding;

    pBinding = (VSGDI_HelperTargetParamWrapper *)AllocateVSMemBlock(0xc);
    if (pBinding == 0)
        return 0;
    pBinding->m_pVtable = g_VSGDI_ResourceGeometryHelperTargetParamWrapperVtable;
    pBinding->m_nWrappedParam = 0;
    pBinding->m_nReserved = 0;
    return pBinding;
}

void VSGDI_DisplayState::ReleaseDisplayBinding(void *pBinding) {
    if (pBinding != 0)
        FreeVSMemBlock(pBinding);
}

int VSGDI_DisplayState::InitializeDisplayBitmapInfo(void *pBitmapInfo) {
    unsigned char *pbInfo = (unsigned char *)pBitmapInfo;
    *(unsigned short *)(pbInfo + 0xc) = 1;
    *(unsigned int *)pbInfo = 0x28;
    *(unsigned int *)(pbInfo + 0x10) = 0;
    *(unsigned int *)(pbInfo + 0x14) = 0;
    *(unsigned int *)(pbInfo + 0x18) = 0;
    *(unsigned int *)(pbInfo + 0x1c) = 0;
    *(unsigned int *)(pbInfo + 0x20) = 0xffffffff;
    *(unsigned short *)(pbInfo + 0x0e) = 8;
    *(unsigned int *)(pbInfo + 0x24) = 0;
    return 1;
}

void *VSGDI_DisplayState::CreateDisplayBitmap(int nDriver, void *pBitmapInfo) {
    VSGDI_DisplayBitmap *pBitmap;
    unsigned int cx;
    unsigned int cy;
    unsigned int cbBits;

    (void)nDriver;
    cx = *(unsigned int *)((char *)pBitmapInfo + 4);
    cy = *(unsigned int *)((char *)pBitmapInfo + 8);
    if (cx == 0)
        cx = 1;
    if (cy == 0)
        cy = 1;
    if (cx > 2048)
        cx = 2048;
    if (cy > 2048)
        cy = 2048;
    cbBits = cx * cy * 4;
    pBitmap = (VSGDI_DisplayBitmap *)AllocateVSMemBlock(0x14);
    if (pBitmap == 0)
        return 0;
    pBitmap->m_pVtable = g_VSGDI_DisplayBitmapVtable;
    pBitmap->m_pBits = AllocateVSMemBlock(cbBits);
    pBitmap->m_pOrigin = 0;
    pBitmap->m_pBackendBitmap = 0;
    pBitmap->m_cbBits = cbBits;
    if (pBitmap->m_pBits == 0) {
        FreeVSMemBlock(pBitmap);
        return 0;
    }
    return pBitmap;
}

int VSGDI_DisplayState::BindDisplayBitmap(int nDriver, void *pBitmap) {
    (void)nDriver;
    return pBitmap != 0;
}

int VSGDI_DisplayState::ReleaseDisplayBitmap(void *pBitmap) {
    VSGDI_DisplayBitmap *pDisplayBitmap;

    pDisplayBitmap = (VSGDI_DisplayBitmap *)pBitmap;
    if (pDisplayBitmap == 0)
        return 1;
    if (pDisplayBitmap->m_pBackendBitmap != 0) {
        FreeVSMemBlock(pDisplayBitmap->m_pBackendBitmap);
        pDisplayBitmap->m_pBackendBitmap = 0;
    }
    FreeVSMemBlock(pDisplayBitmap->m_pBits);
    pDisplayBitmap->m_pBits = 0;
    return 1;
}

int VSGDI_DisplayState::DeleteDisplayBitmap(int nDriver, void *pBitmap) {
    VSGDI_DisplayBitmap *pDisplayBitmap;

    (void)nDriver;
    pDisplayBitmap = (VSGDI_DisplayBitmap *)pBitmap;
    if (pDisplayBitmap == 0)
        return 0;
    pDisplayBitmap->m_pBackendBitmap = 0;
    return 1;
}

static int VSGDI_DisplayStateReady(void *pDisplayState) {
    return IsDisplayStateReady((const VSGDI_DisplayState *)pDisplayState);
}

static void InitializeDisplayStateVtables(void) {
    int i;
    union { int (VSGDI_DisplayBitmap::*p)(void); void *v; } bitmapMethod;
    union { void *(VSGDI_DisplayState::*p)(void); void *v; } createBindingMethod;
    union { void (VSGDI_DisplayState::*p)(void *); void *v; } releaseBindingMethod;
    union { int (VSGDI_DisplayState::*p)(void *); void *v; } bitmapInfoMethod;
    union { void *(VSGDI_DisplayState::*p)(int, void *); void *v; } createBitmapMethod;
    union { int (VSGDI_DisplayState::*p)(void *); void *v; } bitmapObjectMethod;
    union { int (VSGDI_DisplayState::*p)(int, void *); void *v; } bitmapReadyMethod;
    union { int (VSGDI_DisplayState::*p)(int, void *); void *v; } bitmapDeleteMethod;
    union { int (VSGDI_DisplayState::*p)(void) const; void *v; } readyMethod;

    for (i = 0; i < 16; ++i)
        g_VSGDI_DisplayBitmapVtable[i] = (void *)NetworkSafeVtableNoop;
    bitmapMethod.p = &VSGDI_DisplayBitmap::Bits;
    g_VSGDI_DisplayBitmapVtable[3] = bitmapMethod.v;
    bitmapMethod.p = &VSGDI_DisplayBitmap::Origin;
    g_VSGDI_DisplayBitmapVtable[4] = bitmapMethod.v;

    for (i = 0; i < 14; ++i) {
        g_VSGDI_MetricsDisplayVtable[i] = (void *)NetworkSafeVtableNoop;
        g_VSGDI_DibDisplayVtable[i] = (void *)NetworkSafeVtableNoop;
        g_VSGDI_DirectDrawDisplayVtable[i] = (void *)NetworkSafeVtableNoop;
    }
    g_VSGDI_MetricsDisplayVtable[0] = (void *)VSGDI_DeleteDisplayState;
    g_VSGDI_DibDisplayVtable[0] = (void *)VSGDI_DeleteDisplayState;
    g_VSGDI_DirectDrawDisplayVtable[0] = (void *)VSGDI_DeleteDisplayState;
    createBindingMethod.p = &VSGDI_DisplayState::CreateDisplayBinding;
    releaseBindingMethod.p = &VSGDI_DisplayState::ReleaseDisplayBinding;
    bitmapInfoMethod.p = &VSGDI_DisplayState::InitializeDisplayBitmapInfo;
    createBitmapMethod.p = &VSGDI_DisplayState::CreateDisplayBitmap;
    bitmapObjectMethod.p = &VSGDI_DisplayState::ReleaseDisplayBitmap;
    bitmapReadyMethod.p = &VSGDI_DisplayState::BindDisplayBitmap;
    bitmapDeleteMethod.p = &VSGDI_DisplayState::DeleteDisplayBitmap;
    readyMethod.p = &VSGDI_DisplayState::IsReady;
    g_VSGDI_MetricsDisplayVtable[1] = createBindingMethod.v;
    g_VSGDI_DibDisplayVtable[1] = createBindingMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[1] = createBindingMethod.v;
    g_VSGDI_MetricsDisplayVtable[2] = releaseBindingMethod.v;
    g_VSGDI_DibDisplayVtable[2] = releaseBindingMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[2] = releaseBindingMethod.v;
    g_VSGDI_MetricsDisplayVtable[3] = bitmapInfoMethod.v;
    g_VSGDI_DibDisplayVtable[3] = bitmapInfoMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[3] = bitmapInfoMethod.v;
    g_VSGDI_MetricsDisplayVtable[4] = createBitmapMethod.v;
    g_VSGDI_DibDisplayVtable[4] = createBitmapMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[4] = createBitmapMethod.v;
    g_VSGDI_MetricsDisplayVtable[5] = bitmapObjectMethod.v;
    g_VSGDI_DibDisplayVtable[5] = bitmapObjectMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[5] = bitmapObjectMethod.v;
    g_VSGDI_MetricsDisplayVtable[9] = bitmapReadyMethod.v;
    g_VSGDI_DibDisplayVtable[9] = bitmapReadyMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[9] = bitmapReadyMethod.v;
    g_VSGDI_MetricsDisplayVtable[10] = bitmapDeleteMethod.v;
    g_VSGDI_DibDisplayVtable[10] = bitmapDeleteMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[10] = bitmapDeleteMethod.v;
    g_VSGDI_MetricsDisplayVtable[13] = readyMethod.v;
    g_VSGDI_DibDisplayVtable[13] = readyMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[13] = readyMethod.v;
}

struct VSGDI_DisplayStateVtableInitializer {
    VSGDI_DisplayStateVtableInitializer(void) {
        InitializeDisplayStateVtables();
    }
};
static VSGDI_DisplayStateVtableInitializer g_VSGDI_DisplayStateVtableInitializer;

inline VSGDI_DisplayState::VSGDI_DisplayState(void) {
    m_pVtable = 0;
    m_hGraphicsModule = 0;
    m_pReserved08 = 0;
    m_fReady = 0;
    m_hDisplayWindow = 0;
    m_cxDisplay = 0;
    m_cyDisplay = 0;
    m_dwReserved18 = 0;
}

int VSGDI_DisplayState::IsReady(void) const {
    return m_fReady;
}

short VSGDI_DisplayState::Width(void) const {
    return m_cxDisplay;
}

short VSGDI_DisplayState::Height(void) const {
    return m_cyDisplay;
}

VSGDI_HelperSurface::VSGDI_HelperSurface(void) {
    memset(m_abState, 0, sizeof(m_abState));
}

// FUNCTION: LEMBALL 0x0046D090
void VSGDI_HelperSurface::UpdateWorkingRectAndBacking(const VSGDI_Rect *pRect) {
    short aRectBuffer[4];
    VSGDI_HelperSurfaceVariableBlock *pLinkedVariableBlock;
    VSGDI_HelperSurfaceVariableBlock *pVariableBlock;
    short *paRequestedRect;
    short *paWorkingRect;
    short *paTargetWorkingRect;
    short *paBackingObject;
    void *pBackingBitmap;
    void *pDisplayBitmapInfo;
    VSGDI_HelperSurface *pLinkedTarget;
    short sWidth;
    short sHeight;
    int nVariableBlockOffset;
    int nBitmapStride;
    int nBitmapOrigin;
    void **pBackingVtable;
    VSGDI_DisplayStateDispatch *pDisplayDispatch;
    const short *paRectWords;
    const short *paTailRect;

    EnterCriticalSection((char *)this + 0x534);

    nVariableBlockOffset = GetHelperSurfaceVariableBlockOffset(this);
    pVariableBlock = (VSGDI_HelperSurfaceVariableBlock *)((char *)this + nVariableBlockOffset);
    paRectWords = (const short *)pRect;

    paRequestedRect = pVariableBlock->m_aRequestedRect;
    paRequestedRect[0] = paRectWords[0];
    paRequestedRect[1] = paRectWords[1];
    paTailRect = paRectWords + 2;
    if (paRectWords == 0) {
        paTailRect = 0;
    }
    paRequestedRect[2] = paTailRect[0];
    paRequestedRect[3] = paTailRect[1];

    paWorkingRect = pVariableBlock->m_aWorkingRect;
    paWorkingRect[0] = paRectWords[0];
    paWorkingRect[1] = paRectWords[1];
    if (paRectWords != 0) {
        paTailRect = paRectWords + 2;
    } else {
        paTailRect = 0;
    }
    paWorkingRect[2] = paTailRect[0];
    paWorkingRect[3] = paTailRect[1];

    pLinkedTarget = (VSGDI_HelperSurface *)pVariableBlock->m_pLinkedTarget;
    if (g_pResourceGeometryHelperTarget != (void *)pLinkedTarget) {
        pLinkedVariableBlock = (VSGDI_HelperSurfaceVariableBlock *)((char *)pLinkedTarget + GetHelperSurfaceVariableBlockOffset(pLinkedTarget));
        paTargetWorkingRect = pLinkedVariableBlock->m_aWorkingRect;
        sWidth = paTargetWorkingRect[0];
        sHeight = paTargetWorkingRect[1];

        if (pVariableBlock->m_aWorkingRect[2] < 0) {
            pVariableBlock->m_aWorkingRect[0] = (short)(pVariableBlock->m_aWorkingRect[0] + pVariableBlock->m_aWorkingRect[2]);
            pVariableBlock->m_aWorkingRect[2] = 0;
        }
        if (sWidth < (short)(pVariableBlock->m_aWorkingRect[0] + pVariableBlock->m_aWorkingRect[2])) {
            pVariableBlock->m_aWorkingRect[0] = (short)(sWidth - pVariableBlock->m_aWorkingRect[2]);
        }
        if (pVariableBlock->m_aWorkingRect[3] < 0) {
            pVariableBlock->m_aWorkingRect[1] = (short)(pVariableBlock->m_aWorkingRect[1] + pVariableBlock->m_aWorkingRect[3]);
            pVariableBlock->m_aWorkingRect[3] = 0;
        }
        if (sHeight < (short)(pVariableBlock->m_aWorkingRect[1] + pVariableBlock->m_aWorkingRect[3])) {
            pVariableBlock->m_aWorkingRect[1] = (short)(sHeight - pVariableBlock->m_aWorkingRect[3]);
        }
        if (pVariableBlock->m_aWorkingRect[0] <= 0 || pVariableBlock->m_aWorkingRect[1] <= 0) {
            pVariableBlock->m_aWorkingRect[1] = 0;
            pVariableBlock->m_aWorkingRect[0] = 0;
            pVariableBlock->m_aWorkingRect[3] = 0;
            pVariableBlock->m_aWorkingRect[2] = 0;
        }

        paWorkingRect = pVariableBlock->m_aWorkingRect;
        pVariableBlock->m_aBackingRect[0] = paWorkingRect[0];
        pVariableBlock->m_aBackingRect[1] = paWorkingRect[1];
        ComputeBackingDimensions(aRectBuffer, paWorkingRect, (int)paWorkingRect[0]);
        *(int *)((char *)this + 0x524) = 0;
        EnsureHelperBackingRowIndexCapacity(this);
        return;
    }

    *(unsigned short *)&pVariableBlock->m_aWorkingRect[0] = (unsigned short)(pVariableBlock->m_aWorkingRect[0] + 3U) & 0xfffc;
    pVariableBlock->m_aBackingRect[0] = pVariableBlock->m_aWorkingRect[0];
    pVariableBlock->m_aBackingRect[1] = pVariableBlock->m_aWorkingRect[1];

    paWorkingRect = pVariableBlock->m_aWorkingRect;
        ComputeBackingDimensions(aRectBuffer, paWorkingRect, pVariableBlock->m_nBackingWidthHint);
    sWidth = aRectBuffer[0];
    sHeight = aRectBuffer[1];

    if (*(int *)((char *)this + 0xf0) != 0) {
        pDisplayDispatch = (VSGDI_DisplayStateDispatch *)g_pDisplayState;
        pDisplayDispatch->DeleteDisplayBitmap(*(int *)((char *)this + 0xf8),
                                               (void *)(unsigned long)*(int *)((char *)this + 0xf0));
        pDisplayDispatch->ReleaseDisplayBitmap(
            (void *)(unsigned long)*(int *)((char *)this + 0xf0));
        *(int *)((char *)this + 0xf0) = 0;
    }

    if (pVariableBlock->m_aWorkingRect[0] == 0 || pVariableBlock->m_aWorkingRect[1] == 0) {
        *(int *)((char *)this + 0x524) = 0;
    } else {
        if (*(int *)((char *)this + 0xf0) == 0) {
            pDisplayDispatch = (VSGDI_DisplayStateDispatch *)g_pDisplayState;
            pDisplayDispatch->InitializeDisplayBitmapInfo((char *)this + 0xfc);
            *(short *)((char *)this + 0x108) = 1;
            *(int *)((char *)this + 0x100) = (int)sWidth;
            *(int *)((char *)this + 0x104) = (int)sHeight * *(int *)((char *)this + 0x104);
            *(int *)((char *)this + 0x10c) = 0;
            *(int *)((char *)this + 0x110) = 0;
            *(int *)((char *)this + 0x114) = 0;
            *(int *)((char *)this + 0x118) = 0;
            *(int *)((char *)this + 0x11c) = 0;
            *(int *)((char *)this + 0x120) = 0;
            pDisplayBitmapInfo = (char *)this + 0xfc;
            *(int *)pDisplayBitmapInfo = 0x28;
            *(short *)((char *)this + 0x10a) = 8;
            pDisplayDispatch = (VSGDI_DisplayStateDispatch *)g_pDisplayState;
            pBackingBitmap = pDisplayDispatch->CreateDisplayBitmap(
                *(int *)((char *)this + 0xf8), pDisplayBitmapInfo);
            *(void **)((char *)this + 0xf0) = pBackingBitmap;
            if (pBackingBitmap != 0) {
                pDisplayDispatch->BindDisplayBitmap(*(int *)((char *)this + 0xf8),
                                                     (void *)pBackingBitmap);
                *(int *)((char *)this + 0x524) =
                    (int)pVariableBlock->m_aWorkingRect[0] * (int)pVariableBlock->m_aWorkingRect[1];
            }
        }

        if (*(int *)((char *)this + 0xf0) == 0) {
            TriggerReleaseAssertFailure("AllocatedBitmap", "VSGDI.CPP", 0x2e0);
        }

        paBackingObject = (short *)(unsigned long)*(int *)((char *)this + 0xf0);
        pBackingVtable = *(void ***)paBackingObject;
        nBitmapOrigin = ((VSGDI_DisplayBitmapDispatch *)paBackingObject)->Origin();
        nBitmapStride = ((VSGDI_DisplayBitmapDispatch *)paBackingObject)->Bits();
        ConfigureBackingStrideAndOrigin(nBitmapStride, nBitmapOrigin);
        PromoteHelperUploadStateToActive(*(int *)((char *)this + 0x550));

        aRectBuffer[0] = 0;
        aRectBuffer[1] = 0;
        aRectBuffer[2] = 0;
        aRectBuffer[3] = 0;
        aRectBuffer[0] = pVariableBlock->m_aRequestedRect[0];
        aRectBuffer[1] = pVariableBlock->m_aRequestedRect[1];
        ((VSGDI_ResourceGeometryHelperTargetView *)this)
            ->MarkOrForwardHelperTargetDirtyRect(aRectBuffer);
    }

    LeaveCriticalSection((char *)this + 0x534);
}

int VSGDI_HelperSurface::IsReady(void) const {
    return *(const int *)((const char *)this + 0x524) != 0;
}

short VSGDI_HelperSurface::BackingWidth(void) const {
    return *(const short *)((const char *)this + GetHelperSurfaceVariableBlockOffset(this) + 0x64);
}

short VSGDI_HelperSurface::BackingHeight(void) const {
    return *(const short *)((const char *)this + GetHelperSurfaceVariableBlockOffset(this) + 0x66);
}

// FUNCTION: LEMBALL 0x00456A90
VSGDI_MetricsDisplayState::VSGDI_MetricsDisplayState(void) {
    m_pVtable = g_VSGDI_MetricsDisplayVtable;
    m_fReady = 1;
    m_cxDisplay = (short)GetSystemMetrics(0);
    m_cyDisplay = (short)GetSystemMetrics(1);
}

int VSGDI_MetricsDisplayState::Create(HWND hWnd) {
    m_hDisplayWindow = hWnd;
    return m_fReady;
}

// FUNCTION: LEMBALL 0x00456D70
VSGDI_DibDisplayState::VSGDI_DibDisplayState(const unsigned short *pDisplaySize) {
    FARPROC pfnThunk;

    m_cxDisplay = (short)pDisplaySize[0];
    m_cyDisplay = (short)pDisplaySize[1];
    m_fReady = 0;
    m_pDisplayAddress = 0;
    m_dwReserved20 = 0;
    m_pfnDisplayDibStart32 = 0;
    m_pfnDisplayDibEnd32 = 0;
    m_pfnDisplayDibGetAddress32 = 0;
    m_pVtable = g_VSGDI_DibDisplayVtable;
    g_pApplyFullscreenDisplayModeThunk = 0;
    m_hGraphicsModule = LoadLibraryA(g_VSGDI_Dspdib32Dll);
    if (m_hGraphicsModule != 0) {
        m_pfnDisplayDibStart32 = GetProcAddress(m_hGraphicsModule, g_VSGDI_DisplayDibStart32);
        m_pfnDisplayDibEnd32 = GetProcAddress(m_hGraphicsModule, g_VSGDI_DisplayDibEnd32);
        m_pfnDisplayDibGetAddress32 = GetProcAddress(m_hGraphicsModule, g_VSGDI_DisplayDibGetAddress32);
        pfnThunk = GetProcAddress(m_hGraphicsModule, g_VSGDI_DisplayDib32);
        g_pApplyFullscreenDisplayModeThunk = pfnThunk;
        m_pDisplayAddress = ((void *(*)())m_pfnDisplayDibGetAddress32)();
        ((void (*)())m_pfnDisplayDibStart32)();
        m_fReady = 1;
    }
}

int VSGDI_DibDisplayState::Create(HWND hWnd) {
    m_hDisplayWindow = hWnd;
    return m_fReady;
}

// FUNCTION: LEMBALL 0x00457410
VSGDI_DirectDrawDisplayState::VSGDI_DirectDrawDisplayState(const unsigned short *pDisplaySize, int fCreateWindow) {
    typedef unsigned int (__stdcall *DIRECTDRAW_CREATE)(void *, void **, void *);
    typedef unsigned int (__stdcall *DIRECTDRAW_SET_COOPERATIVE_LEVEL)(void *, HWND, unsigned int);
    typedef unsigned int (__stdcall *DIRECTDRAW_SET_DISPLAY_MODE)(void *, int, int, int);
    typedef unsigned int (__stdcall *DIRECTDRAW_CREATE_SURFACE)(void *, void *, void **, void *);
    unsigned char aSurfaceDescription[0x6c];
    unsigned int uCooperativeFlags;
    unsigned int uResult;
    WNDCLASSA WindowClass;
    void **ppDirectDrawObject = &m_pDirectDrawObject;

    (void)fCreateWindow;
    m_pVtable = g_VSGDI_DirectDrawDisplayVtable;
    m_cxDisplay = (short)pDisplaySize[0];
    m_cyDisplay = (short)pDisplaySize[1];
    m_fReady = 0;
    *ppDirectDrawObject = 0;
    m_pPrimarySurface = 0;
    m_pOffscreenSurface = 0;
    m_pPalette = 0;
    m_fDirectDrawCursorVisible = 0;
    m_fWindowOwnershipInitialized = 1;
    *(int *)((char *)this + 0x30) = 0;
    m_hGraphicsModule = LoadLibraryA(g_VSGDI_DdrawDll);
    if (m_hGraphicsModule != 0) {
        m_pfnDirectDrawCreate = GetProcAddress(m_hGraphicsModule, g_VSGDI_DirectDrawCreate);
    }
    if (m_pfnDirectDrawCreate == 0) {
        return;
    }

    uResult = ((DIRECTDRAW_CREATE)m_pfnDirectDrawCreate)(0, ppDirectDrawObject, 0);
    if (uResult != 0) {
        NoopDirectDrawCallback(uResult, 0);
        return;
    }
    if (*ppDirectDrawObject == 0) {
        return;
    }

    uCooperativeFlags = 8;
    if (fCreateWindow != 0) {
        WindowClass.style = 0xb;
        WindowClass.lpfnWndProc = WindowOwnerWindowProc;
        WindowClass.cbClsExtra = 0;
        WindowClass.cbWndExtra = 0;
        WindowClass.hInstance = g_hApplicationInstance;
        WindowClass.hIcon = LoadIconA((HINSTANCE)0, (LPCSTR)0x7f00);
        WindowClass.hCursor = LoadCursorA((HINSTANCE)0, (LPCSTR)0x7f00);
        WindowClass.hbrBackground = GetStockObject(4);
        WindowClass.lpszMenuName = 0;
        WindowClass.lpszClassName = g_VSGDI_DirectDrawWindowClass;
        if (RegisterClassA(&WindowClass) == 0) {
            return;
        }
        ShowCursor(0);
        m_hDisplayWindow = CreateWindowExA(
            8, g_VSGDI_DirectDrawWindowClass, g_VSGDI_DirectDrawWindowTitle,
            0x80000000, 0, 0, m_cxDisplay, m_cyDisplay,
            (HWND)0, (HMENU)0, g_hApplicationInstance, (LPVOID)0);
        if (m_hDisplayWindow == 0) {
            return;
        }
        ShowWindow(m_hDisplayWindow, 5);
        UpdateWindow(m_hDisplayWindow);
        SetForegroundWindow(m_hDisplayWindow);
        uCooperativeFlags = 0x11;
    }

    {
        void **pDirectDrawVtable = *(void ***)*ppDirectDrawObject;
        uResult = ((DIRECTDRAW_SET_COOPERATIVE_LEVEL)pDirectDrawVtable[0x50 / 4])(
            *ppDirectDrawObject, m_hDisplayWindow, uCooperativeFlags);
        if (uResult != 0) {
            return;
        }
        if ((uCooperativeFlags & 1) != 0) {
            uResult = ((DIRECTDRAW_SET_DISPLAY_MODE)pDirectDrawVtable[0x54 / 4])(
                *ppDirectDrawObject, m_cxDisplay, m_cyDisplay, 8);
            if (uResult != 0) {
                return;
            }
        }
        *(unsigned int *)aSurfaceDescription = 0x6c;
        *(unsigned int *)(aSurfaceDescription + 4) = 0;
        *(unsigned int *)(aSurfaceDescription + 0x68) = 0x200;
        uResult = ((DIRECTDRAW_CREATE_SURFACE)pDirectDrawVtable[0x18 / 4])(
            *ppDirectDrawObject, aSurfaceDescription, &m_pPrimarySurface, 0);
        if (uResult == 0) {
            m_fReady = 1;
        }
    }
}

int VSGDI_DirectDrawDisplayState::Create(HWND hWnd) {
    m_hDisplayWindow = hWnd;
    (void)g_hApplicationInstance;
    (void)g_VSGDI_BaseWindowClass;
    (void)g_VSGDI_DirectDrawWindowClass;
    (void)g_VSGDI_DirectDrawWindowTitle;
    (void)g_VSGDI_ErrorRegisterClass;
    return m_fReady;
}

// FUNCTION: LEMBALL 0x0046BA80
int InitializeResourceGeometryHelperRuntime(void) {
    VSGDI_SelectedGraphicsDriverRuntime *pSelectedGraphicsDriverRuntime;
    VSGDI_ResourceGeometryHelperSlotManager *pSlotManager;
    void *pvTarget;
    int *piArrowCursorClient;

    (void)g_VSGDI_BaseWindowClass;

    pSelectedGraphicsDriverRuntime =
        (VSGDI_SelectedGraphicsDriverRuntime *)AllocateVSMemBlock(0xc);
    if (pSelectedGraphicsDriverRuntime != 0) {
        pSelectedGraphicsDriverRuntime->m_pWindowOwner = 0;
        pSelectedGraphicsDriverRuntime->m_fFallbackWarningShown = 0;
        g_pSelectedGraphicsDriverRuntime = pSelectedGraphicsDriverRuntime;
    } else {
        g_pSelectedGraphicsDriverRuntime = 0;
    }
    ((VSGDI_SelectedGraphicsDriverRuntime *)g_pSelectedGraphicsDriverRuntime)
        ->InitializeSelectedGraphicsDriver(VSGDI_DRIVER_AUTO);

    pSlotManager = (VSGDI_ResourceGeometryHelperSlotManager *)AllocateVSMemBlock(sizeof(VSGDI_ResourceGeometryHelperSlotManager));
    if (pSlotManager == 0) {
        g_pResourceGeometryHelperSlotManager = 0;
    } else {
        g_pResourceGeometryHelperSlotManager =
            pSlotManager->InitializeResourceGeometryHelperSlotManager(
                (int)g_StartupGraphicsDriverConfig.m_cbSize);
    }

    g_VSGDI_ArrowCursorRuntimeVtable[1] = (void *)VSGDI_DeleteDisplayState;

    pvTarget = AllocateVSMemBlock(0x5a0);
    if (pvTarget == 0) {
        g_pResourceGeometryHelperTarget = 0;
    } else {
        g_pResourceGeometryHelperTarget =
            ((VSGDI_ResourceGeometryHelperTargetView *)pvTarget)
                ->ConstructResourceGeometryHelperTarget(0, 1);
    }

    piArrowCursorClient = (int *)AllocateVSMemBlock(0xa4);
    if (piArrowCursorClient == 0) {
        g_pArrowCursorStatusIndicatorRenderClient = 0;
    } else {
        ConstructStatusIndicatorRenderClientBase(piArrowCursorClient);
        *(void **)piArrowCursorClient = &g_VSGDI_ArrowCursorRuntimeVtable;
        InitializeArrowCursorState(piArrowCursorClient);
        g_pArrowCursorStatusIndicatorRenderClient = piArrowCursorClient;
    }

    if (g_pResourceGeometryHelperSlotManager != 0 && g_pResourceGeometryHelperTarget != 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0046BB70
int ShutdownResourceGeometryHelperRuntime(void) {
    if (g_pArrowCursorStatusIndicatorRenderClient != 0) {
        CallDeleteVirtual(g_pArrowCursorStatusIndicatorRenderClient, 4, 1);
    }
    if (g_pResourceGeometryHelperTarget != 0) {
        DestroyResourceGeometryHelperTarget(g_pResourceGeometryHelperTarget);
        RestoreCompactResourceGeometryHelperVtable((char *)g_pResourceGeometryHelperTarget + 0x55c);
        FreeVSMemBlock(g_pResourceGeometryHelperTarget);
    }
    if (g_pResourceGeometryHelperSlotManager != 0) {
        DestroyResourceGeometryHelperSlotManager(g_pResourceGeometryHelperSlotManager);
        FreeVSMemBlock(g_pResourceGeometryHelperSlotManager);
    }
    g_pResourceGeometryHelperTarget = 0;
    g_pResourceGeometryHelperSlotManager = 0;
    if (g_pDisplayState != 0) {
        CallDeleteVirtual(g_pDisplayState, 0, 1);
    }
    if (g_pSelectedGraphicsDriverRuntime != 0) {
        FreeVSMemBlock(g_pSelectedGraphicsDriverRuntime);
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00457E10
int VSGDI_SelectedGraphicsDriverRuntime::InitializeSelectedGraphicsDriver(int nRequestedDriver) {
    int nSelectedDriver;
    VSGDI_SelectedGraphicsDriverRuntime *pRuntime;

    pRuntime = this;

    if (nRequestedDriver < 9) {
        AppendStatusCString(g_VSGDI_InitDriverPrefix);
        AppendStatusCString(g_apszGraphicsDriverNames[nRequestedDriver]);
        AppendStatusCString("\n");
    }

    nSelectedDriver = nRequestedDriver;
    if (nRequestedDriver == VSGDI_DRIVER_AUTO) {
        if (g_fStartupGraphicsDriverGdk == 0) {
            nSelectedDriver = (-(unsigned int)(g_fStartupFullscreen == 0) & 0xfffffffeU) + 3;
        } else {
            nSelectedDriver = (-(unsigned int)(g_fStartupFullscreen == 0) & 2U) + 4;
        }
    }

    {
        unsigned short aDisplaySize[2];

    switch (nSelectedDriver) {
    case VSGDI_DRIVER_METRICS:
        {
            void *pvDisplayState;

            pvDisplayState = AllocateVSMemBlock(0x1c);
            if (pvDisplayState == 0) {
                g_pDisplayState = 0;
            } else {
                g_pDisplayState = new (pvDisplayState) VSGDI_MetricsDisplayState;
            }
        }
        break;
    case VSGDI_DRIVER_DIB_320_200:
        {
            void *pvDisplayState;

            pvDisplayState = AllocateVSMemBlock(0x30);
            if (pvDisplayState == 0) {
                g_pDisplayState = 0;
            } else {
                aDisplaySize[0] = 0x140;
                aDisplaySize[1] = 200;
                g_pDisplayState = new (pvDisplayState) VSGDI_DibDisplayState(aDisplaySize);
            }
        }
        break;
    case VSGDI_DRIVER_DIB_640_480:
        {
            void *pvDisplayState;

            pvDisplayState = AllocateVSMemBlock(0x30);
            if (pvDisplayState == 0) {
                g_pDisplayState = 0;
            } else {
                aDisplaySize[0] = 0x140;
                aDisplaySize[1] = 0xf0;
                g_pDisplayState = new (pvDisplayState) VSGDI_DibDisplayState(aDisplaySize);
            }
        }
        break;
    case VSGDI_DRIVER_DDRAW_320_200:
        {
            void *pvDisplayState;

            pvDisplayState = AllocateVSMemBlock(0x4a8);
            if (pvDisplayState == 0) {
                g_pDisplayState = 0;
            } else {
                aDisplaySize[0] = 0x280;
                aDisplaySize[1] = 0x1e0;
                g_pDisplayState = new (pvDisplayState) VSGDI_DirectDrawDisplayState(aDisplaySize, 1);
            }
        }
        break;
    case VSGDI_DRIVER_DDRAW_640_480:
        nSelectedDriver = VSGDI_DRIVER_DDRAW_320_200;
        {
            void *pvDisplayState;

            pvDisplayState = AllocateVSMemBlock(0x4a8);
            if (pvDisplayState == 0) {
                g_pDisplayState = 0;
            } else {
                aDisplaySize[0] = 0x280;
                aDisplaySize[1] = 0x1e0;
                g_pDisplayState = new (pvDisplayState) VSGDI_DirectDrawDisplayState(aDisplaySize, 1);
            }
        }
        break;
    default:
        AppendErrorCString(g_VSGDI_ErrorNoValidDriver);
        return 0;
    }
    }

    if (g_pDisplayState == 0 || !IsDisplayStateReady(g_pDisplayState)) {
        if (g_pDisplayState != 0) {
            CallDeleteVirtual(g_pDisplayState, 0, 1);
        }
        {
            void *pvDisplayState;

            pvDisplayState = AllocateVSMemBlock(0x1c);
            if (pvDisplayState == 0) {
                g_pDisplayState = 0;
            } else {
                g_pDisplayState = new (pvDisplayState) VSGDI_MetricsDisplayState;
            }
        }
        if (g_pDisplayState == 0 || !IsDisplayStateReady(g_pDisplayState)) {
            AppendErrorCString(g_VSGDI_ErrorNoValidDriverAvailable);
            return 0;
        }

        if (pRuntime->m_fFallbackWarningShown == 0) {
            GAME_DynamicCString WarningMessage;
            GAME_DynamicCString WarningResult;

            WarningMessage.ConstructDynamicCStringFromCString(
                g_apszGraphicsDriverFallbackMessages[nSelectedDriver]);
            WarningMessage.AppendDynamicCStringAndCopyResult(
                &WarningResult, g_VSGDI_DefaultWindowModeSuffix);
            DestroyDynamicCString(&WarningResult);
            MessageBoxA((HWND)0, WarningMessage.m_pszText, g_VSGDI_WarningCaption, 0x12000);
            pRuntime->m_fFallbackWarningShown = 1;
            DestroyDynamicCString(&WarningMessage);
        }
        nSelectedDriver = VSGDI_DRIVER_METRICS;
    }

    if (nRequestedDriver != nSelectedDriver) {
        AppendStatusCString(g_VSGDI_AutoSelectedPrefix);
        AppendStatusCString(g_apszGraphicsDriverNames[nSelectedDriver]);
        AppendStatusCString(g_VSGDI_AutoSelectedSuffix);
    }

    if (pRuntime != 0) {
        pRuntime->m_nSelectedDriver = nSelectedDriver;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x004581D0
void VSGDI_SelectedGraphicsDriverRuntime::ResizeActiveDisplayState(unsigned short *pSize) {
    VSGDI_DisplayState *pDisplayState;

    if (g_pDisplayState != 0) {
        pDisplayState = g_pDisplayState;
        *(unsigned short *)((char *)pDisplayState + 0x14) = pSize[0];
        *(unsigned short *)((char *)pDisplayState + 0x16) = pSize[1];
        NotifyWindowOwnersDisplayChanged(&m_nSelectedDriver);
    }
}

int InitializeSelectedGraphicsDriver(int nRequestedDriver) {
    return ((VSGDI_SelectedGraphicsDriverRuntime *)g_pSelectedGraphicsDriverRuntime)
        ->InitializeSelectedGraphicsDriver(nRequestedDriver);
}

int GetSelectedGraphicsDriverId(void) {
    if (g_pSelectedGraphicsDriverRuntime == 0) {
        return VSGDI_DRIVER_AUTO;
    }

    return ((VSGDI_SelectedGraphicsDriverRuntime *)g_pSelectedGraphicsDriverRuntime)->m_nSelectedDriver;
}

VSGDI_DisplayState *GetDisplayState(void) {
    return g_pDisplayState;
}
