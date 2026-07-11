#include "graphics_driver.h"
#include "../network/safe_vtable.h"
#include "../main.h"
#include "../platform/startup_options.h"
#include "../engine/memory_arena.h"
#include "../engine/runtime_init.h"

#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif
#include <string.h>

int g_fStartupFullscreen = 0;
int g_fStartupGraphicsDriverWing = 0;
int g_fStartupGraphicsDriverCds = 1;
int g_fStartupGraphicsDriverGdk = 0;
static void *g_pSelectedGraphicsDriverRuntime = 0;
static void *g_pResourceGeometryHelperSlotManager = 0;
static void *g_pResourceGeometryHelperTarget = 0;
int *g_pArrowCursorStatusIndicatorRenderClient = 0;
static VSGDI_DisplayState *g_pDisplayState = 0;

static const char g_VSGDI_DdrawDll[] = "DDRAW.DLL";
static const char g_VSGDI_Dspdib32Dll[] = "DSPDIB32.DLL";
static const char *g_VSGDI_BaseWindowClass = "VS_Base_Window_Class";
static const char g_VSGDI_DirectDrawCreate[] = "DirectDrawCreate";
static const char g_VSGDI_DirectDrawWindowClass[] = "DirectDrawClass";
static const char g_VSGDI_DirectDrawWindowTitle[] = "DirectDraw";
static const char g_VSGDI_DisplayDibStart32[] = "DisplayDibStart32";
static const char g_VSGDI_DisplayDibEnd32[] = "DisplayDibEnd32";
static const char g_VSGDI_DisplayDibGetAddress32[] = "DisplayDibGetAddress32";
static const char g_VSGDI_DisplayDib32[] = "DisplayDib32";
static const char g_VSGDI_ErrorNoValidDriver[] = "No valid driver selected to initialise\n";
static const char g_VSGDI_ErrorNoValidDriverAvailable[] = "No valid driver available\n";
static const char g_VSGDI_ErrorRegisterClass[] = "Unable to register DD base window class";
static const char g_VSGDI_InitDriverPrefix[] = "Initialising graphics device driver: ";
static const char g_VSGDI_AutoSelectedPrefix[] = "[ Auto selected: ";
static const char g_VSGDI_AutoSelectedSuffix[] = " ]\n";
static const char g_VSGDI_DefaultWindowModeSuffix[] =
    ". Defaulting to normal window mode (using CreateDIBSection)";
static const char g_VSGDI_WarningCaption[] = "WARNING";
static FARPROC g_pApplyFullscreenDisplayModeThunk = 0;
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
VSGDI_SAFE_TABLE(g_VSGDI_CompactResourceGeometryHelperSubobjectVtable);
VSGDI_SAFE_TABLE(g_VSGDI_ResourceGeometryHelperTargetParamWrapperVtable);
VSGDI_SAFE_TABLE(g_VSGDI_ResourceGeometryHelperSlotManagerVtable);
VSGDI_SAFE_TABLE(g_VSGDI_StatusIndicatorBaseVtable);
VSGDI_SAFE_TABLE(g_VSGDI_ArrowCursorRuntimeVtable);
VSGDI_SAFE_TABLE(g_VSGDI_StatusIndicatorRenderClientVtable);
VSGDI_SAFE_TABLE(g_VSGDI_StatusIndicatorPointSinkVtable);
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

struct VSGDI_SelectedGraphicsDriverRuntime {
    int m_nSelectedDriver;
    int m_nReserved0;
    int m_fFallbackWarningShown;
};

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

extern void TriggerReleaseAssertFailure(const char *pszExpression, const char *pszFile, int nLine);
extern int g_fRootHelperGeometryDispatchSuppressed;
extern void *g_pQueuedRenderPointSinkFinalizeThunk;
static int GetHelperSurfaceVariableBlockOffset(const VSGDI_HelperSurface *pSurface);

static void ReleaseResourceGeometryRowBufferStorage(int nRowBuffer) {
    if (*(int *)(unsigned long)(nRowBuffer + 4) != 0) {
        FreeVSMemBlock((void *)(unsigned long)*(int *)(unsigned long)(nRowBuffer + 4));
        *(int *)(unsigned long)(nRowBuffer + 4) = 0;
    }
    *(int *)(unsigned long)(nRowBuffer + 0x28) = 0;
}

static void ResetHelperUploadStateMap(int) {
}

// FUNCTION: LEMBALL 0x004663D0
void *InitializeCompactResourceGeometryHelper(void *pvHelper) {
    int *pHelper;
    unsigned short wZero;

    pHelper = (int *)pvHelper;
    wZero = 0;
    *(unsigned short *)((char *)pHelper + 6) = wZero;
    *(unsigned short *)(pHelper + 1) = wZero;
    *(unsigned short *)((char *)pHelper + 10) = wZero;
    *(unsigned short *)(pHelper + 2) = wZero;
    *(unsigned short *)((char *)pHelper + 0xe) = wZero;
    *(unsigned short *)(pHelper + 3) = wZero;
    *(unsigned short *)((char *)pHelper + 0x12) = wZero;
    *(unsigned short *)(pHelper + 4) = wZero;
    *(unsigned short *)((char *)pHelper + 0x16) = wZero;
    *(unsigned short *)(pHelper + 5) = wZero;
    *(unsigned short *)((char *)pHelper + 0x1a) = wZero;
    *(unsigned short *)(pHelper + 6) = wZero;
    *(unsigned short *)((char *)pHelper + 0x1e) = wZero;
    *(unsigned short *)(pHelper + 7) = wZero;
    *pHelper = (int)(unsigned long)&g_VSGDI_CompactResourceGeometryHelperVtable;
    pHelper[0xf] = 1;
    *(unsigned short *)((char *)pHelper + 0x26) = wZero;
    pHelper[0x10] = 0;
    *(unsigned short *)(pHelper + 9) = wZero;
    *(unsigned short *)((char *)pHelper + 0x2a) = wZero;
    *(unsigned short *)(pHelper + 10) = wZero;
    return pHelper;
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
    ReleaseResourceGeometryRowBufferStorage((int)(unsigned long)pRowBuffer);
    return pRowBuffer;
}

// FUNCTION: LEMBALL 0x00466670
void *ConstructHelperGroup1RowBuffer(void *pvHelperGroup, int fConstructCompactHelper) {
    int iCompactDelta;
    int *pHelperGroup;

    pHelperGroup = (int *)pvHelperGroup;
    if (fConstructCompactHelper != 0) {
        *(void **)((char *)pHelperGroup + 4) = &g_VSGDI_ResourceGeometryHelperGroup1ConstructionAdjustorVtable;
        InitializeCompactResourceGeometryHelper((char *)pHelperGroup + 0x58);
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
        InitializeCompactResourceGeometryHelper((char *)pHelperGroup + 0x58);
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
void ReleaseHelperGroup0BackingBuffer(int nHelperGroup) {
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
void *InitializeResourceGeometryHelperSlotManager(void *pvSlotManager, int cSlots) {
    VSGDI_ResourceGeometryHelperSlotManager *pSlotManager;
    VSGDI_ResourceGeometryHelperSlot *pSlot;
    int cRemaining;

    pSlotManager = (VSGDI_ResourceGeometryHelperSlotManager *)pvSlotManager;
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
void *ConstructResourceGeometryHelperTarget(void *pvTarget, int nWrappedParam, int fConstructCompactHelper) {
    int iCompactDelta;
    VSGDI_CompactResourceGeometryHelperFields *pCompactHelper;
    VSGDI_HelperTargetParamWrapper *pParamWrapper;
    VSGDI_ResourceGeometryHelperTargetView *pTarget;

    pTarget = (VSGDI_ResourceGeometryHelperTargetView *)pvTarget;
    if (fConstructCompactHelper != 0) {
        *(void **)((char *)pTarget + 0x40) = &g_VSGDI_CompactHelperTargetAdjustor;
        *(void **)((char *)pTarget + 0x48) = &g_VSGDI_HelperGroup1Adjustor;
        *(void **)((char *)pTarget + 0x9c) = &g_VSGDI_HelperGroup0Adjustor;
        InitializeCompactResourceGeometryHelper((char *)pTarget + 0x55c);
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

// FUNCTION: LEMBALL 0x004722D0
void DestroyResourceGeometryRowBuffer(void *pvRowBuffer) {
    *(void **)pvRowBuffer = &g_VSGDI_ResourceGeometryRowBufferVtable;
    ReleaseResourceGeometryRowBufferStorage((int)(unsigned long)pvRowBuffer);
}

// FUNCTION: LEMBALL 0x004664B0
void DestroyHelperGroup0RowBuffer(void *pvHelperGroup) {
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
void DestroyHelperGroup1RowBuffer(void *pvHelperGroup) {
    int iCompactDelta;
    int *pHelperGroup;
    int *pRowBuffer;

    pHelperGroup = (int *)pvHelperGroup;
    *(void **)pHelperGroup = &g_VSGDI_ResourceGeometryHelperGroup1Vtable;
    pRowBuffer = pHelperGroup + 2;
    iCompactDelta = *(int *)((char *)(unsigned long)pHelperGroup[1] + 4);
    *(void **)((char *)pRowBuffer + iCompactDelta - 4) = (void *)&g_VSGDI_ResourceGeometryHelperGroup1CompactSubobjectVtable;
    *(int *)((char *)pRowBuffer + iCompactDelta - 8) = iCompactDelta - 0x54;
    DestroyResourceGeometryRowBuffer(pRowBuffer);
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
    if (pTarget->m_pBackingBitmap != 0) {
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
    if (pTarget->m_pDisplayBinding != 0) {
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

static void RestoreCompactResourceGeometryHelperVtable(void *pvHelper) {
    *(void **)pvHelper = &g_VSGDI_CompactResourceGeometryHelperSubobjectVtable;
}

// FUNCTION: LEMBALL 0x0046AEC0
void ConstructStatusIndicatorRenderClientBase(int *pRenderClient) {
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
}

// FUNCTION: LEMBALL 0x00474B50
void InitializeArrowCursorState(int *pRenderClient) {
    void **pVtable;

    pRenderClient[0x27] = 0;
    pRenderClient[0x27] = (int)(unsigned long)LoadCursorA(0, (LPCSTR)0x7f00);
    pVtable = *(void ***)pRenderClient;
    ((void (*)(void *))pVtable[0x18 / sizeof(void *)])(pRenderClient);
}

// FUNCTION: LEMBALL 0x00472400
void RebuildResourceGeometryRowPointerTable(VSGDI_HelperSurface *pSurface) {
    int *pnRowTable;
    int nRowBase;
    int nRowByteOffset;
    int iRowIndex;
    int iLogicalRow;
    int cRows;

    pnRowTable = (int *)(unsigned long)*(int *)((char *)pSurface + 4);
    nRowBase = *(int *)((char *)pSurface + 0x18);
    nRowByteOffset = *(int *)((char *)pSurface + 8);
    iLogicalRow = *(int *)((char *)pSurface + 0x14);
    iRowIndex = 0;
    cRows = (int)*(short *)((char *)pSurface + 0x2e);
    if (0 < cRows) {
        do {
            pnRowTable[iLogicalRow] = nRowBase + nRowByteOffset;
            ++iLogicalRow;
            nRowByteOffset = nRowByteOffset + *(int *)((char *)pSurface + 0x1c);
            if (cRows <= iLogicalRow) {
                iLogicalRow = iLogicalRow - cRows;
            }
            ++iRowIndex;
        } while (iRowIndex < cRows);
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
void EnsureHelperBackingRowIndexCapacity(VSGDI_HelperSurface *pSurface) {
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
void ComputeHelperBackingDimensions(VSGDI_HelperSurface *pSurface, short *paOut, short *paRect, int nWidth) {
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
void ConfigureHelperBackingStrideAndOrigin(VSGDI_HelperSurface *pSurface, int nStride, int nOrigin) {
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
void PromoteHelperUploadStateToActive(int nUploadState) {
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
void InitializeHelperUploadStatePending(int nUploadState) {
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

// FUNCTION: LEMBALL 0x0040381E
void QueueQueuedRenderPointSink(void *pPointSink, void *pQueue) {
    AppendPointerQueueEntry(pQueue, pPointSink);
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
void SampleRootHelperGeometryAndDispatchRenderGroups(void *pPrimaryContext, int nToken) {
    int *pContext;
    VSGDI_HelperDispatchQueue *pDispatchQueue;
    VSGDI_HelperSurfaceBindingSurface *pBindingSurface;
    VSGDI_HelperSurface *pHelperTarget;
    int *pQueuedPointSink;
    void *pUploadCallbackTarget;
    int nUploadState;
    int nResult;
    short x0;
    short x1;
    short y1;
    short x2;
    short y2;

    pContext = (int *)pPrimaryContext;
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

    pQueuedPointSink = (int *)AllocateVSMemBlock(4);
    if (pQueuedPointSink != 0) {
        *pQueuedPointSink = (int)(unsigned long)g_pQueuedRenderPointSinkFinalizeThunk;
    }

    nUploadState = ((int (*)(void))(*(void ***)pHelperTarget)[2])();
    InitializeHelperUploadStatePending(nUploadState);
    ((void (*)(void *))(*(void ***)*(int **)pQueuedPointSink)[1])(pDispatchQueue);

    x0 = *(short *)((char *)&nUploadState); /* intentional dead local shaping placeholder */
    (void)x0;
    x2 = (short)nUploadState; /* keep locals materialized in old-style layout */
    (void)x2;

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
        ((void (*)(int))(*(void ***)*(int **)pQueuedPointSink)[0])(1);
    }
}

static int GetHelperSurfaceVariableBlockOffset(const VSGDI_HelperSurface *pSurface) {
    return *(const int *)((const char *)(unsigned long)*(const int *)((const char *)pSurface + 0x40) + 4);
}

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
    ((void (*)(void *, int))pVtable[nByteOffset / sizeof(void *)])(pObject, nDeleteFlag);
}

static int IsDisplayStateReady(const VSGDI_DisplayState *pDisplayState) {
    return *(const int *)((const char *)pDisplayState + 0xc) != 0;
}

VSGDI_DisplayState::VSGDI_DisplayState(void) {
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
    void **pDisplayVtable;
    void **pBackingVtable;
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
        ComputeHelperBackingDimensions(this, aRectBuffer, paWorkingRect, (int)paWorkingRect[0]);
        *(int *)((char *)this + 0x524) = 0;
        EnsureHelperBackingRowIndexCapacity(this);
        return;
    }

    *(unsigned short *)&pVariableBlock->m_aWorkingRect[0] = (unsigned short)(pVariableBlock->m_aWorkingRect[0] + 3U) & 0xfffc;
    pVariableBlock->m_aBackingRect[0] = pVariableBlock->m_aWorkingRect[0];
    pVariableBlock->m_aBackingRect[1] = pVariableBlock->m_aWorkingRect[1];

    paWorkingRect = pVariableBlock->m_aWorkingRect;
    ComputeHelperBackingDimensions(this, aRectBuffer, paWorkingRect, pVariableBlock->m_nBackingWidthHint);
    sWidth = aRectBuffer[0];
    sHeight = aRectBuffer[1];

    if (*(int *)((char *)this + 0xf0) != 0) {
        pDisplayVtable = *(void ***)g_pDisplayState;
        ((void (*)(void *, int, int))pDisplayVtable[0x28 / sizeof(void *)])(
            g_pDisplayState, *(int *)((char *)this + 0xf8), *(int *)((char *)this + 0xf0));
        ((void (*)(void *, int))pDisplayVtable[0x14 / sizeof(void *)])(
            g_pDisplayState, *(int *)((char *)this + 0xf0));
        *(int *)((char *)this + 0xf0) = 0;
    }

    if (pVariableBlock->m_aWorkingRect[0] == 0 || pVariableBlock->m_aWorkingRect[1] == 0) {
        *(int *)((char *)this + 0x524) = 0;
    } else {
        if (*(int *)((char *)this + 0xf0) == 0) {
            pDisplayVtable = *(void ***)g_pDisplayState;
            ((void (*)(void *, void *))pDisplayVtable[0x0c / sizeof(void *)])(g_pDisplayState, (char *)this + 0xfc);
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
            pBackingBitmap = (void *)(unsigned long)((int (*)(void *, int, void *))pDisplayVtable[0x10 / sizeof(void *)])(
                g_pDisplayState, *(int *)((char *)this + 0xf8), pDisplayBitmapInfo);
            *(void **)((char *)this + 0xf0) = pBackingBitmap;
            if (pBackingBitmap != 0) {
                ((void (*)(void *, int, int))pDisplayVtable[0x24 / sizeof(void *)])(
                    g_pDisplayState, *(int *)((char *)this + 0xf8), (int)(unsigned long)pBackingBitmap);
                *(int *)((char *)this + 0x524) =
                    (int)pVariableBlock->m_aWorkingRect[0] * (int)pVariableBlock->m_aWorkingRect[1];
            }
        }

        if (*(int *)((char *)this + 0xf0) == 0) {
            TriggerReleaseAssertFailure("AllocatedBitmap", "VSGDI.CPP", 0x2e0);
        }

        paBackingObject = (short *)(unsigned long)*(int *)((char *)this + 0xf0);
        pBackingVtable = *(void ***)paBackingObject;
        nBitmapOrigin = ((int (*)(void *))pBackingVtable[0x10 / sizeof(void *)])(paBackingObject);
        nBitmapStride = ((int (*)(void *))pBackingVtable[0x0c / sizeof(void *)])(paBackingObject);
        ConfigureHelperBackingStrideAndOrigin(this, nBitmapStride, nBitmapOrigin);
        PromoteHelperUploadStateToActive(*(int *)((char *)this + 0x550));

        aRectBuffer[0] = 0;
        aRectBuffer[1] = 0;
        aRectBuffer[2] = 0;
        aRectBuffer[3] = 0;
        aRectBuffer[0] = pVariableBlock->m_aRequestedRect[0];
        aRectBuffer[1] = pVariableBlock->m_aRequestedRect[1];
        ((void (*)(void *, short *))(*(void ***)this)[4 / sizeof(void *)])(this, aRectBuffer);
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
    m_fReady = 1;
    m_cxDisplay = (short)GetSystemMetrics(0);
    m_cyDisplay = (short)GetSystemMetrics(1);
}

int VSGDI_MetricsDisplayState::Create(HWND hWnd) {
    m_hDisplayWindow = hWnd;
    return m_fReady;
}

// FUNCTION: LEMBALL 0x00456D70
VSGDI_DibDisplayState::VSGDI_DibDisplayState(short cxDisplay, short cyDisplay) {
    FARPROC pfnThunk;

    m_cxDisplay = cxDisplay;
    m_cyDisplay = cyDisplay;
    m_fReady = 0;
    m_pDisplayAddress = 0;
    m_dwReserved20 = 0;
    m_pfnDisplayDibStart32 = 0;
    m_pfnDisplayDibEnd32 = 0;
    m_pfnDisplayDibGetAddress32 = 0;
    g_pApplyFullscreenDisplayModeThunk = 0;
    m_hGraphicsModule = LoadLibraryA(g_VSGDI_Dspdib32Dll);
    if (m_hGraphicsModule != 0) {
        m_pfnDisplayDibStart32 = GetProcAddress(m_hGraphicsModule, g_VSGDI_DisplayDibStart32);
        m_pfnDisplayDibEnd32 = GetProcAddress(m_hGraphicsModule, g_VSGDI_DisplayDibEnd32);
        m_pfnDisplayDibGetAddress32 = GetProcAddress(m_hGraphicsModule, g_VSGDI_DisplayDibGetAddress32);
        pfnThunk = GetProcAddress(m_hGraphicsModule, g_VSGDI_DisplayDib32);
        g_pApplyFullscreenDisplayModeThunk = pfnThunk;
        if (m_pfnDisplayDibGetAddress32 != 0) {
            m_pDisplayAddress = ((void *(*)())m_pfnDisplayDibGetAddress32)();
        }
        if (m_pfnDisplayDibStart32 != 0) {
            ((void (*)())m_pfnDisplayDibStart32)();
            m_fReady = 1;
        }
    }
}

int VSGDI_DibDisplayState::Create(HWND hWnd) {
    m_hDisplayWindow = hWnd;
    return m_fReady;
}

// FUNCTION: LEMBALL 0x00457410
VSGDI_DirectDrawDisplayState::VSGDI_DirectDrawDisplayState(short cxDisplay, short cyDisplay) {
    m_cxDisplay = cxDisplay;
    m_cyDisplay = cyDisplay;
    m_fReady = 0;
    m_pDirectDrawObject = 0;
    m_pPrimarySurface = 0;
    m_pOffscreenSurface = 0;
    m_pPalette = 0;
    m_fDirectDrawCursorVisible = 1;
    memset(m_abReserved34, 0, sizeof(m_abReserved34));
    m_pfnDirectDrawCreate = 0;
    memset(m_abReservedA4, 0, sizeof(m_abReservedA4));
    m_fWindowOwnershipInitialized = 1;
    m_hGraphicsModule = LoadLibraryA(g_VSGDI_DdrawDll);
    if (m_hGraphicsModule != 0) {
        m_pfnDirectDrawCreate = GetProcAddress(m_hGraphicsModule, g_VSGDI_DirectDrawCreate);
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
    VSGDI_ResourceGeometryHelperSlotManager *pSlotManager;
    void *pvTarget;
    int *piArrowCursorClient;

    (void)g_VSGDI_BaseWindowClass;

    g_pSelectedGraphicsDriverRuntime = AllocateVSMemBlock(0xc);
    if (g_pSelectedGraphicsDriverRuntime == 0) {
        g_pSelectedGraphicsDriverRuntime = 0;
    } else {
        *(int *)((char *)g_pSelectedGraphicsDriverRuntime + 4) = 0;
        *(int *)((char *)g_pSelectedGraphicsDriverRuntime + 8) = 0;
    }
    InitializeSelectedGraphicsDriver(VSGDI_DRIVER_AUTO);

    pSlotManager = (VSGDI_ResourceGeometryHelperSlotManager *)AllocateVSMemBlock(sizeof(VSGDI_ResourceGeometryHelperSlotManager));
    if (pSlotManager == 0) {
        g_pResourceGeometryHelperSlotManager = 0;
    } else {
        g_pResourceGeometryHelperSlotManager =
            InitializeResourceGeometryHelperSlotManager(pSlotManager, (int)g_StartupGraphicsDriverConfig.m_cbSize);
    }

    pvTarget = AllocateVSMemBlock(0x5a0);
    if (pvTarget == 0) {
        g_pResourceGeometryHelperTarget = 0;
    } else {
        g_pResourceGeometryHelperTarget = ConstructResourceGeometryHelperTarget(pvTarget, 0, 1);
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
int InitializeSelectedGraphicsDriver(int nRequestedDriver) {
    int nSelectedDriver;
    char szWarningMessage[256];
    VSGDI_SelectedGraphicsDriverRuntime *pRuntime;

    pRuntime = (VSGDI_SelectedGraphicsDriverRuntime *)g_pSelectedGraphicsDriverRuntime;

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
                g_pDisplayState = new (pvDisplayState) VSGDI_DibDisplayState(0x140, 200);
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
                g_pDisplayState = new (pvDisplayState) VSGDI_DibDisplayState(0x140, 0xf0);
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
                g_pDisplayState = new (pvDisplayState) VSGDI_DirectDrawDisplayState(0x280, 0x1e0);
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
                g_pDisplayState = new (pvDisplayState) VSGDI_DirectDrawDisplayState(0x280, 0x1e0);
            }
        }
        break;
    default:
        AppendErrorCString(g_VSGDI_ErrorNoValidDriver);
        return 0;
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

        if (pRuntime != 0 && pRuntime->m_fFallbackWarningShown == 0) {
            strcpy(szWarningMessage, g_apszGraphicsDriverFallbackMessages[nSelectedDriver]);
            strcat(szWarningMessage, g_VSGDI_DefaultWindowModeSuffix);
            MessageBoxA((HWND)0, szWarningMessage, g_VSGDI_WarningCaption, 0x12000);
            pRuntime->m_fFallbackWarningShown = 1;
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

int GetSelectedGraphicsDriverId(void) {
    if (g_pSelectedGraphicsDriverRuntime == 0) {
        return VSGDI_DRIVER_AUTO;
    }

    return ((VSGDI_SelectedGraphicsDriverRuntime *)g_pSelectedGraphicsDriverRuntime)->m_nSelectedDriver;
}

VSGDI_DisplayState *GetDisplayState(void) {
    return g_pDisplayState;
}
