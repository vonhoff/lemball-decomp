#include "graphics_driver.h"
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
static int g_VSGDI_CompactResourceGeometryHelperVtable = 0;
static int g_VSGDI_ResourceGeometryRowBufferVtable = 0;
static int g_VSGDI_ResourceGeometryHelperGroup0ConstructionAdjustorVtable = 0;
static int g_VSGDI_ResourceGeometryHelperGroup0Vtable = 0;
static int g_VSGDI_ResourceGeometryHelperGroup0CompactSubobjectVtable = 0;
static int g_VSGDI_ResourceGeometryHelperGroup1ConstructionAdjustorVtable = 0;
static int g_VSGDI_ResourceGeometryHelperGroup1Vtable = 0;
static int g_VSGDI_ResourceGeometryHelperGroup1CompactSubobjectVtable = 0;
static int g_VSGDI_ResourceGeometryHelperTargetConstructionVtable = 0;
static int g_VSGDI_CompactResourceGeometryHelperConstructionVtable = 0;
static int g_VSGDI_ResourceGeometryHelperTargetVtable = 0;
static int g_VSGDI_ResourceGeometryHelperGroup1RowBufferVtable = 0;
static int g_VSGDI_ResourceGeometryHelperGroup0RowBufferVtable = 0;
static int g_VSGDI_CompactResourceGeometryHelperSubobjectVtable = 0;
static int g_VSGDI_ResourceGeometryHelperTargetParamWrapperVtable = 0;
static int g_VSGDI_ResourceGeometryHelperSlotManagerVtable = 0;
static int g_VSGDI_StatusIndicatorBaseVtable = 0;
static int g_VSGDI_ArrowCursorRuntimeVtable = 0;
static int g_VSGDI_StatusIndicatorRenderClientVtable = 0;
static int g_VSGDI_StatusIndicatorPointSinkVtable = 0;

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

extern void TriggerReleaseAssertFailure(const char *pszExpression, const char *pszFile, int nLine);
extern int g_fRootHelperGeometryDispatchSuppressed;
extern void *g_pQueuedRenderPointSinkFinalizeThunk;

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

    pHelper = (int *)pvHelper;
    *(short *)((char *)pHelper + 6) = 0;
    *(short *)(pHelper + 1) = 0;
    *(short *)((char *)pHelper + 10) = 0;
    *(short *)(pHelper + 2) = 0;
    *(short *)((char *)pHelper + 0xe) = 0;
    *(short *)(pHelper + 3) = 0;
    *(short *)((char *)pHelper + 0x12) = 0;
    *(short *)(pHelper + 4) = 0;
    *(short *)((char *)pHelper + 0x16) = 0;
    *(short *)(pHelper + 5) = 0;
    *(short *)((char *)pHelper + 0x1a) = 0;
    *(short *)(pHelper + 6) = 0;
    *(short *)((char *)pHelper + 0x1e) = 0;
    *(short *)(pHelper + 7) = 0;
    *pHelper = (int)(unsigned long)&g_VSGDI_CompactResourceGeometryHelperVtable;
    pHelper[0xf] = 1;
    *(short *)((char *)pHelper + 0x26) = 0;
    pHelper[0x10] = 0;
    *(short *)(pHelper + 9) = 0;
    *(short *)((char *)pHelper + 0x2a) = 0;
    *(short *)(pHelper + 10) = 0;
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
    *(void ***)(unsigned long)(*(int *)((char *)(unsigned long)pHelperGroup[1] + 4) + 4 + (int)(unsigned long)pHelperGroup) =
        (void **)&g_VSGDI_ResourceGeometryHelperGroup1CompactSubobjectVtable;
    iCompactDelta = *(int *)((char *)(unsigned long)pHelperGroup[1] + 4);
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
    *(void ***)(unsigned long)(*(int *)((char *)(unsigned long)pHelperGroup[1] + 4) + 4 + (int)(unsigned long)pHelperGroup) =
        (void **)&g_VSGDI_ResourceGeometryHelperGroup0CompactSubobjectVtable;
    iCompactDelta = *(int *)((char *)(unsigned long)pHelperGroup[1] + 4);
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
    int cbSlots;
    int iSlotOffset;
    int *pSlotManager;
    int fEnabled;
    int i;

    pSlotManager = (int *)pvSlotManager;
    *(void **)pSlotManager = &g_VSGDI_ResourceGeometryHelperSlotManagerVtable;
    cbSlots = ((cSlots * 8) - cSlots) * 4;
    pSlotManager[1] = (int)(unsigned long)AllocateVSMemBlock((unsigned int)cbSlots);
    pSlotManager[4] = cSlots;
    pSlotManager[2] = 0;
    pSlotManager[3] = 0;
    if (0 < cSlots) {
        iSlotOffset = 0;
        fEnabled = 1;
        i = cSlots;
        do {
            iSlotOffset = iSlotOffset + 0x1c;
            --i;
            *(int *)((char *)(unsigned long)pSlotManager[1] + iSlotOffset - 0x1c) = 0;
            *(int *)((char *)(unsigned long)pSlotManager[1] + iSlotOffset - 0x14) = 0;
            *(int *)((char *)(unsigned long)pSlotManager[1] + iSlotOffset - 0x18) = 0;
            *(int *)((char *)(unsigned long)pSlotManager[1] + iSlotOffset - 0xc) = 0;
            *(int *)((char *)(unsigned long)pSlotManager[1] + iSlotOffset - 0x10) = 0;
            *(int *)((char *)(unsigned long)pSlotManager[1] + iSlotOffset - 8) = fEnabled;
            *(int *)((char *)(unsigned long)pSlotManager[1] + iSlotOffset - 4) = 0;
        } while (i != 0);
    }
    g_pResourceGeometryHelperTarget = 0;
    return pSlotManager;
}

// FUNCTION: LEMBALL 0x0046BC90
void DestroyResourceGeometryHelperSlotManager(void *pvSlotManager) {
    int *pSlotManager;
    int i;
    int iSlotOffset;

    pSlotManager = (int *)pvSlotManager;
    *(void **)pSlotManager = &g_VSGDI_ResourceGeometryHelperSlotManagerVtable;
    i = 0;
    if (0 < pSlotManager[4]) {
        iSlotOffset = 0;
        do {
            if (*(int *)((char *)(unsigned long)pSlotManager[1] + iSlotOffset + 0x14) == 0) {
                AppendErrorCString(g_VSGDI_UnallocatedHelperSlotMessage);
            }
            iSlotOffset = iSlotOffset + 0x1c;
            ++i;
        } while (i < pSlotManager[4]);
    }
    FreeVSMemBlock((void *)(unsigned long)pSlotManager[1]);
}

// FUNCTION: LEMBALL 0x0046BCE0
int FindFirstFreeResourceGeometryHelperSlotIndex(void *pvSlotManager) {
    int *pSlotManager;
    int *piSlotState;
    int i;

    pSlotManager = (int *)pvSlotManager;
    i = 0;
    if (0 < pSlotManager[4]) {
        piSlotState = (int *)((char *)(unsigned long)pSlotManager[1] + 0x14);
        do {
            if (*piSlotState != 0) {
                return i;
            }
            piSlotState = piSlotState + 7;
            i = i + 1;
        } while (i < pSlotManager[4]);
    }
    return -1;
}

// FUNCTION: LEMBALL 0x0046C5D0
void *ConstructResourceGeometryHelperTarget(void *pvTarget, int nWrappedParam, int fConstructCompactHelper) {
    int iCompactDelta;
    VSGDI_HelperTargetParamWrapper *pParamWrapper;
    int *pTarget;

    pTarget = (int *)pvTarget;
    if (fConstructCompactHelper != 0) {
        *(void **)((char *)pTarget + 0x40) = (void *)0x499de8;
        *(void **)((char *)pTarget + 0x48) = (void *)0x499de0;
        *(void **)((char *)pTarget + 0x9c) = (void *)0x499dd8;
        InitializeCompactResourceGeometryHelper((char *)pTarget + 0x55c);
    }
    InitializeResourceGeometryRowBuffer(pTarget);
    *(void **)pTarget = &g_VSGDI_ResourceGeometryHelperTargetConstructionVtable;
    *(void ***)(unsigned long)(*(int *)((char *)(unsigned long)pTarget[0x10] + 4) + 0x40 + (int)(unsigned long)pTarget) =
        (void **)&g_VSGDI_CompactResourceGeometryHelperConstructionVtable;
    ConstructHelperGroup1RowBuffer((char *)pTarget + 0x44, 0);
    ConstructHelperGroup0RowBuffer((char *)pTarget + 0x98, 0);
    *(short *)((char *)pTarget + 0xee) = 0;
    *(int *)((char *)pTarget + 0x528) = 0;
    *(short *)((char *)pTarget + 0xec) = 0;
    *(int *)((char *)pTarget + 0x52c) = 0;
    *(int *)((char *)pTarget + 0x530) = 0;
    *(void **)pTarget = &g_VSGDI_ResourceGeometryHelperTargetVtable;
    *(void **)((char *)pTarget + 0x44) = &g_VSGDI_ResourceGeometryHelperGroup1RowBufferVtable;
    *(void **)((char *)pTarget + 0x98) = &g_VSGDI_ResourceGeometryHelperGroup0RowBufferVtable;
    *(void ***)(unsigned long)(*(int *)((char *)(unsigned long)pTarget[0x10] + 4) + 0x40 + (int)(unsigned long)pTarget) =
        (void **)&g_VSGDI_CompactResourceGeometryHelperSubobjectVtable;
    iCompactDelta = *(int *)((char *)(unsigned long)pTarget[0x10] + 4);
    *(int *)((char *)pTarget + iCompactDelta + 0x3c) = iCompactDelta - 0x51c;

    pParamWrapper = (VSGDI_HelperTargetParamWrapper *)AllocateVSMemBlock(0xc);
    if (pParamWrapper == 0) {
        *(int *)((char *)pTarget + 0xf8) = 0;
    } else {
        pParamWrapper->m_pVtable = &g_VSGDI_ResourceGeometryHelperTargetParamWrapperVtable;
        pParamWrapper->m_nWrappedParam = nWrappedParam;
        pParamWrapper->m_nReserved = 0;
        *(VSGDI_HelperTargetParamWrapper **)((char *)pTarget + 0xf8) = pParamWrapper;
    }
    *(int *)((char *)pTarget + 0xf0) = 0;
    *(int *)((char *)pTarget + 0x550) = 0;
    *(int *)(unsigned long)(*(int *)((char *)(unsigned long)pTarget[0x10] + 4) + 0x60 + (int)(unsigned long)pTarget) = 0;
    InitializeCriticalSection((char *)pTarget + 0x534);
    *(int *)((char *)pTarget + 0x54c) = 1;
    *(int *)(unsigned long)(*(int *)((char *)(unsigned long)pTarget[0x10] + 4) + 0x70 + (int)(unsigned long)pTarget) = 0;
    return pTarget;
}

// FUNCTION: LEMBALL 0x004722D0
void DestroyResourceGeometryRowBuffer(void *pvRowBuffer) {
    *(void **)pvRowBuffer = &g_VSGDI_ResourceGeometryRowBufferVtable;
    ReleaseResourceGeometryRowBufferStorage((int)(unsigned long)pvRowBuffer);
}

// FUNCTION: LEMBALL 0x004664B0
void DestroyHelperGroup0RowBuffer(void *pvHelperGroup) {
    int *pHelperGroup;
    int *pRowBuffer;

    pHelperGroup = (int *)pvHelperGroup;
    *(void **)pHelperGroup = &g_VSGDI_ResourceGeometryHelperGroup0Vtable;
    pRowBuffer = pHelperGroup + 2;
    *(void ***)(unsigned long)(*(int *)((char *)(unsigned long)pHelperGroup[1] + 4) - 4 + (int)(unsigned long)pRowBuffer) =
        (void **)&g_VSGDI_ResourceGeometryHelperGroup0CompactSubobjectVtable;
    *(int *)(unsigned long)(*(int *)((char *)(unsigned long)pHelperGroup[1] + 4) - 8 + (int)(unsigned long)pRowBuffer) =
        *(int *)((char *)(unsigned long)pHelperGroup[1] + 4) - 0x54;
    DestroyResourceGeometryRowBuffer(pRowBuffer);
}

// FUNCTION: LEMBALL 0x004666E0
void DestroyHelperGroup1RowBuffer(void *pvHelperGroup) {
    int *pHelperGroup;
    int *pRowBuffer;

    pHelperGroup = (int *)pvHelperGroup;
    *(void **)pHelperGroup = &g_VSGDI_ResourceGeometryHelperGroup1Vtable;
    pRowBuffer = pHelperGroup + 2;
    *(void ***)(unsigned long)(*(int *)((char *)(unsigned long)pHelperGroup[1] + 4) - 4 + (int)(unsigned long)pRowBuffer) =
        (void **)&g_VSGDI_ResourceGeometryHelperGroup1CompactSubobjectVtable;
    *(int *)(unsigned long)(*(int *)((char *)(unsigned long)pHelperGroup[1] + 4) - 8 + (int)(unsigned long)pRowBuffer) =
        *(int *)((char *)(unsigned long)pHelperGroup[1] + 4) - 0x54;
    DestroyResourceGeometryRowBuffer(pRowBuffer);
}

void DestroyResourceGeometryHelperTarget(void *pvTarget) {
    int iPreviousLink;
    int iNextLink;
    int iGlobalListHead;
    int iLinkedTarget;
    int iCriticalSectionReady;
    int *pLink;
    unsigned int pNextNode;
    int *pTarget;

    pTarget = (int *)pvTarget;
    *(void **)pTarget = &g_VSGDI_ResourceGeometryHelperTargetVtable;
    pTarget[0x11] = (int)(unsigned long)&g_VSGDI_ResourceGeometryHelperGroup1RowBufferVtable;
    pTarget[0x26] = (int)(unsigned long)&g_VSGDI_ResourceGeometryHelperGroup0RowBufferVtable;
    *(void ***)(unsigned long)(*(int *)((char *)(unsigned long)pTarget[0x10] + 4) + 0x40 + (int)(unsigned long)pTarget) =
        (void **)&g_VSGDI_CompactResourceGeometryHelperSubobjectVtable;
    *(int *)(unsigned long)(*(int *)((char *)(unsigned long)pTarget[0x10] + 4) + 0x3c + (int)(unsigned long)pTarget) =
        *(int *)((char *)(unsigned long)pTarget[0x10] + 4) - 0x51c;

    iCriticalSectionReady = pTarget[0x153];
    if (iCriticalSectionReady != 0) {
        EnterCriticalSection(pTarget + 0x14d);
    }
    if (pTarget[0x3c] != 0) {
        ((void (*)(void *, int, int))(*(void ***)g_pDisplayState)[0x28 / sizeof(void *)])(
            g_pDisplayState, pTarget[0x3e], pTarget[0x3c]);
        ((void (*)(void *, int))(*(void ***)g_pDisplayState)[0x14 / sizeof(void *)])(g_pDisplayState, pTarget[0x3c]);
        pTarget[0x3c] = 0;
    }
    if (*(int *)(unsigned long)(*(int *)((char *)(unsigned long)pTarget[0x10] + 4) + 0x60 + (int)(unsigned long)pTarget) ==
        (int)(unsigned long)g_pResourceGeometryHelperTarget) {
        ReleaseHelperGroup0BackingBuffer((int)(unsigned long)(pTarget + 0x26));
        ReleaseHelperGroup1BackingBuffer((int)(unsigned long)(pTarget + 0x11));
    }
    if (pTarget[0x3e] != 0) {
        ((void (*)(void *, int))(*(void ***)g_pDisplayState)[8 / sizeof(void *)])(g_pDisplayState, pTarget[0x3e]);
        pTarget[0x3e] = 0;
    }
    pNextNode = (unsigned int)pTarget[0x154];
    if (pNextNode != 0) {
        ResetHelperUploadRectListAndStateMap((int)pNextNode);
        FreeVSMemBlock((void *)(unsigned long)pNextNode);
        pTarget[0x154] = 0;
    }
    iLinkedTarget = *(int *)(unsigned long)(*(int *)((char *)(unsigned long)pTarget[0x10] + 4) + 0x60 + (int)(unsigned long)pTarget);
    if (iLinkedTarget != 0) {
        pLink = *(int **)(unsigned long)(iLinkedTarget + 0x528);
        if (pLink != 0) {
            do {
                if ((void *)(unsigned long)*pLink == pvTarget) {
                    break;
                }
                pLink = (int *)(unsigned long)pLink[1];
            } while (pLink != 0);
            if (pLink != 0) {
                iPreviousLink = pLink[1];
                iNextLink = pLink[2];
                FreeVSMemBlock(pLink);
                if (iPreviousLink == 0) {
                    *(int *)(unsigned long)(iLinkedTarget + 0x52c) = iNextLink;
                } else {
                    *(int *)(unsigned long)(iPreviousLink + 8) = iNextLink;
                }
                if (iNextLink == 0) {
                    *(int *)(unsigned long)(iLinkedTarget + 0x528) = iPreviousLink;
                } else {
                    *(int *)(unsigned long)(iNextLink + 4) = iPreviousLink;
                }
                *(int *)(unsigned long)(iLinkedTarget + 0x530) = *(int *)(unsigned long)(iLinkedTarget + 0x530) - 1;
            }
        }
        *(int *)(unsigned long)(*(int *)((char *)(unsigned long)pTarget[0x10] + 4) + 0x60 + (int)(unsigned long)pTarget) = 0;
    }
    if (iCriticalSectionReady != 0) {
        LeaveCriticalSection(pTarget + 0x14d);
        DeleteCriticalSection(pTarget + 0x14d);
        pTarget[0x153] = 0;
        iGlobalListHead = (int)(unsigned long)g_pResourceGeometryListHead;
        if (g_pResourceGeometryListHead != 0) {
            pLink = *(int **)g_pResourceGeometryListHead;
            if (pLink != 0) {
                do {
                    if ((void *)(unsigned long)*pLink == pvTarget) {
                        break;
                    }
                    pLink = (int *)(unsigned long)pLink[1];
                } while (pLink != 0);
                if (pLink != 0) {
                    iCriticalSectionReady = pLink[1];
                    iLinkedTarget = pLink[2];
                    FreeVSMemBlock(pLink);
                    if (iCriticalSectionReady == 0) {
                        *(int *)(unsigned long)(iGlobalListHead + 4) = iLinkedTarget;
                    } else {
                        *(int *)(unsigned long)(iCriticalSectionReady + 8) = iLinkedTarget;
                    }
                    if (iLinkedTarget == 0) {
                        *(int *)(unsigned long)iGlobalListHead = iCriticalSectionReady;
                    } else {
                        *(int *)(unsigned long)(iLinkedTarget + 4) = iCriticalSectionReady;
                    }
                    *(int *)(unsigned long)(iGlobalListHead + 8) = *(int *)(unsigned long)(iGlobalListHead + 8) - 1;
                }
            }
            iGlobalListHead = (int)(unsigned long)g_pResourceGeometryListHead;
            if (*(int *)(unsigned long)((int)(unsigned long)g_pResourceGeometryListHead + 8) == 0) {
                if (g_pResourceGeometryListHead != 0) {
                    pNextNode = *(unsigned int *)g_pResourceGeometryListHead;
                    while (pNextNode != 0) {
                        unsigned int pFollowingNode;

                        pFollowingNode = *(unsigned int *)(unsigned long)(pNextNode + 4);
                        FreeVSMemBlock((void *)(unsigned long)pNextNode);
                        pNextNode = pFollowingNode;
                    }
                    FreeVSMemBlock((void *)(unsigned long)iGlobalListHead);
                }
                g_pResourceGeometryListHead = 0;
            }
        }
    }
    pNextNode = (unsigned int)pTarget[0x14a];
    while (pNextNode != 0) {
        unsigned int pFollowingNode;

        pFollowingNode = *(unsigned int *)(unsigned long)(pNextNode + 4);
        FreeVSMemBlock((void *)(unsigned long)pNextNode);
        pNextNode = pFollowingNode;
    }
    DestroyHelperGroup0RowBuffer(pvTarget == 0 ? 0 : (void *)(pTarget + 0x26));
    DestroyHelperGroup1RowBuffer(pvTarget == 0 ? 0 : (void *)(pTarget + 0x11));
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
    int nRowByteOffset;
    int iRowIndex;
    int iLogicalRow;
    int cRows;

    nRowByteOffset = *(int *)((char *)pSurface + 8);
    iLogicalRow = *(int *)((char *)pSurface + 0x14);
    iRowIndex = 0;
    cRows = (int)*(short *)((char *)pSurface + 0x2e);
    if (0 < cRows) {
        do {
            *(int *)((char *)(unsigned long)*(int *)((char *)pSurface + 4) + iLogicalRow * 4) =
                *(int *)((char *)pSurface + 0x18) + nRowByteOffset;
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
void ClearHelperBackingBorderRows(VSGDI_HelperSurface *pSurface) {
    unsigned int cbBorderRow;
    unsigned int cDwords;
    unsigned int cTailBytes;
    char *pbTarget;
    int nAbsoluteStride;
    int nStrideSign;

    *(int *)((char *)pSurface + 0x18) = 0;
    *(int *)((char *)pSurface + 0x14) = 0;
    RebuildResourceGeometryRowPointerTable(pSurface);

    cbBorderRow = (unsigned int)*(int *)((char *)pSurface + 0x20);
    if (0 < (int)cbBorderRow) {
        pbTarget = (char *)(unsigned long)*(int *)((char *)pSurface + 0xc);
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

        nAbsoluteStride = *(int *)((char *)pSurface + 0x1c);
        nStrideSign = nAbsoluteStride >> 0x1f;
        nAbsoluteStride = (nAbsoluteStride ^ nStrideSign) - nStrideSign;
        pbTarget = (char *)(unsigned long)(nAbsoluteStride * (int)*(short *)((char *)pSurface + 0x2e) +
                                           *(int *)((char *)pSurface + 0xc) + cbBorderRow);
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
            ClearHelperBackingBorderRows(pSurface);
        }
        *(int *)((char *)pSurface + 0x28) = cRows;
        return;
    }
    ClearHelperBackingBorderRows(pSurface);
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
    *(void **)(*(int *)pQueue + *(int *)((char *)pQueue + 4) * 4) = pEntry;
    *(int *)((char *)pQueue + 4) = *(int *)((char *)pQueue + 4) + 1;
}

// FUNCTION: LEMBALL 0x0040381E
void QueueQueuedRenderPointSink(void *pPointSink, void *pQueue) {
    AppendPointerQueueEntry(pQueue, pPointSink);
}

// FUNCTION: LEMBALL 0x0046D9F0
int RefreshHelperTargetBindingIfChanged(void *pTarget) {
    int *pTargetWords;
    int *pLinkedTarget;
    int nVariableBlockOffset;
    int nBindingValue;

    pTargetWords = (int *)pTarget;
    if (pTargetWords[1] == 0) {
        return 0;
    }

    pLinkedTarget =
        *(int **)(*(int *)(unsigned long)(pTargetWords[0x10] + 4) + 0x60 + (int)(unsigned long)pTargetWords);
    if ((void *)pLinkedTarget == g_pResourceGeometryHelperTarget) {
        if ((int *)pTargetWords[0x3c] == 0) {
            return 0;
        }
        if (((int (*)())**(void ***)pTargetWords[0x3c])() == 0) {
            return 0;
        }
        nBindingValue = ((int (*)(void))(*(void ***)pTargetWords[0x3c])[3])();
        if (nBindingValue != 0 && pTargetWords[3] != nBindingValue) {
            pTargetWords[3] = nBindingValue;
            EnsureHelperBackingRowIndexCapacity((VSGDI_HelperSurface *)pTargetWords);
            return 1;
        }
    } else {
        if (pLinkedTarget == 0) {
            return 0;
        }
        if (RefreshHelperTargetBindingIfChanged(pLinkedTarget) == 0) {
            return 0;
        }
        nVariableBlockOffset = *(int *)(unsigned long)(pTargetWords[0x10] + 4);
        if (*(int *)(*(int *)(*(int *)(nVariableBlockOffset + (int)(unsigned long)pTargetWords + 0x60) + 4) +
                    *(short *)(nVariableBlockOffset + (int)(unsigned long)pTargetWords + 0x5a) * 4) +
                (int)*(short *)(nVariableBlockOffset + (int)(unsigned long)pTargetWords + 0x58) !=
            pTargetWords[3]) {
            EnsureHelperBackingRowIndexCapacity((VSGDI_HelperSurface *)pTargetWords);
            return 1;
        }
    }
    return 1;
}

// FUNCTION: LEMBALL 0x0046DAA0
void InvokeRootHelperTargetPostQueueCallback(int nTarget) {
    int nRootTarget;

    do {
        nRootTarget = nTarget;
        nTarget = *(int *)(*(int *)(*(int *)(unsigned long)(nRootTarget + 0x40) + 4) + 0x60 + nRootTarget);
    } while (g_pResourceGeometryHelperTarget != (void *)(unsigned long)nTarget);

    ((void (*)())(*(void ***)*(int **)(unsigned long)(nRootTarget + 0xf0))[2])();
}

// FUNCTION: LEMBALL 0x00467110
void DispatchAndClearPointerQueue(void *pQueue) {
    int *pQueueWords;
    int nByteOffset;
    int i;

    pQueueWords = (int *)pQueue;
    if (RefreshHelperTargetBindingIfChanged((void *)(unsigned long)pQueueWords[3]) != 0) {
        nByteOffset = 0;
        i = 0;
        if (0 < pQueueWords[1]) {
            do {
                if (IsPointerInsideManagedMemoryRegions(*(void **)(unsigned long)(pQueueWords[0] + nByteOffset)) != 0) {
                    ((void (*)(void *))(*(void ***)*(int **)(unsigned long)(pQueueWords[0] + nByteOffset))[2])(pQueue);
                }
                nByteOffset += 4;
                ++i;
            } while (i < pQueueWords[1]);
        }
        InvokeRootHelperTargetPostQueueCallback(pQueueWords[3]);
    }
}

// FUNCTION: LEMBALL 0x00465AA0
void SampleRootHelperGeometryAndDispatchRenderGroups(void *pPrimaryContext, int nToken) {
    int *pContext;
    int nHelperTarget;
    int *pQueuedPointSink;
    int nUploadState;
    int nResult;
    short x0;
    short y0;
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

    nHelperTarget = *(int *)(pContext[0x13] + 0xc);
    ((void (*)())(*(void ***)*(int **)(*(int *)(*(int *)(unsigned long)(nHelperTarget + 0x40) + 4) + 0x40 + nHelperTarget))[0xe])();

    nHelperTarget = *(int *)(pContext[0x13] + 0xc);
    nHelperTarget = *(int *)(*(int *)(unsigned long)(nHelperTarget + 0x40) + 4) + 0x40 + nHelperTarget;
    x1 = *(short *)(unsigned long)(nHelperTarget + 0x16);
    y1 = *(short *)(unsigned long)(nHelperTarget + 0x14);

    pQueuedPointSink = (int *)AllocateVSMemBlock(4);
    if (pQueuedPointSink != 0) {
        *pQueuedPointSink = (int)(unsigned long)&g_pQueuedRenderPointSinkFinalizeThunk;
    }

    nUploadState = ((int (*)(void))(*(void ***)*(int **)(unsigned long)(pContext[0x13] + 0xc))[2])();
    InitializeHelperUploadStatePending(nUploadState);
    ((void (*)(void *))(*(void ***)*(int **)pQueuedPointSink)[1])((void *)(unsigned long)pContext[0x13]);

    x0 = *(short *)((char *)&nUploadState); /* intentional dead local shaping placeholder */
    (void)x0;
    x2 = (short)nUploadState; /* keep locals materialized in old-style layout */
    (void)x2;

    *(short *)((char *)pContext + 0x5c) = *(short *)&nUploadState;
    *(short *)((char *)pContext + 0x5e) = *((short *)&nUploadState + 1);
    *(short *)((char *)pContext + 0x60) = y1;
    *(short *)((char *)pContext + 0x62) = x1;
    pContext[0x19] = nToken;
    ((void (*)(void *))(*(void ***)pContext[0x16])[1])((void *)(unsigned long)pContext[0x13]);

    nHelperTarget = *(int *)(pContext[0x13] + 0xc);
    nResult =
        ((int (*)())(*(void ***)*(int **)(*(int *)(*(int *)(unsigned long)(nHelperTarget + 0x40) + 4) + 0x40 +
                                          nHelperTarget))[0xf])();
    y2 = *((short *)&nResult + 1);
    if (nResult != 0) {
        *(short *)((char *)pContext + 0x6c) = 0;
        *(short *)((char *)pContext + 0x6e) = 0;
        *(short *)((char *)pContext + 0x70) = (short)nResult;
        *(short *)((char *)pContext + 0x72) = y2;
        *(short *)((char *)pContext + 0x74) = *(short *)&nUploadState;
        *(short *)((char *)pContext + 0x76) = *((short *)&nUploadState + 1);
        ((void (*)(void *))(*(void ***)pContext[0x1a])[1])((void *)(unsigned long)pContext[0x13]);
    }

    nHelperTarget = *(int *)(pContext[0x13] + 0xc);
    nResult =
        ((int (*)())(*(void ***)*(int **)(*(int *)(*(int *)(unsigned long)(nHelperTarget + 0x40) + 4) + 0x40 +
                                          nHelperTarget))[0x10])();
    if (nResult != 0) {
        *(short *)((char *)pContext + 0x80) = (short)nResult;
        *(short *)((char *)pContext + 0x82) = *((short *)&nResult + 1);
        *(short *)((char *)pContext + 0x84) = *(short *)&nUploadState;
        *(short *)((char *)pContext + 0x86) = *((short *)&nUploadState + 1);
        pContext[0x1f] = 0;
        ((void (*)(void *))(*(void ***)pContext[0x1e])[1])((void *)(unsigned long)pContext[0x13]);
    }

    DispatchAndClearPointerQueue((void *)(unsigned long)pContext[0x13]);
    *(int *)(pContext[0x13] + 4) = 0;
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
    short *paRequestedRect;
    short *paWorkingRect;
    short *paTargetWorkingRect;
    short *paBackingObject;
    char *pLinkedTargetBytes;
    short sWidth;
    short sHeight;
    int nVariableBlockOffset;
    int nLinkedTargetVariableBlockOffset;
    int pLinkedTarget;
    int nBitmapHandle;
    int nBitmapStride;
    int nBitmapOrigin;
    void **pDisplayVtable;
    void **pBackingVtable;
    const short *paRectWords;
    const short *paTailRect;

    EnterCriticalSection((char *)this + 0x534);

    nVariableBlockOffset = *(int *)((char *)this + 0x40);
    nVariableBlockOffset = *(int *)((char *)(unsigned long)nVariableBlockOffset + 4);
    paRectWords = (const short *)pRect;

    paRequestedRect = (short *)((char *)this + nVariableBlockOffset + 0x4c);
    paRequestedRect[0] = paRectWords[0];
    paRequestedRect[1] = paRectWords[1];
    paTailRect = paRectWords + 2;
    if (paRectWords == 0) {
        paTailRect = 0;
    }
    paRequestedRect[2] = paTailRect[0];
    paRequestedRect[3] = paTailRect[1];

    paWorkingRect = (short *)((char *)this + nVariableBlockOffset + 0x54);
    paWorkingRect[0] = paRectWords[0];
    paWorkingRect[1] = paRectWords[1];
    if (paRectWords != 0) {
        paTailRect = paRectWords + 2;
    } else {
        paTailRect = 0;
    }
    paWorkingRect[2] = paTailRect[0];
    paWorkingRect[3] = paTailRect[1];

    pLinkedTarget = *(int *)((char *)this + nVariableBlockOffset + 0x60);
    if (g_pResourceGeometryHelperTarget != (void *)(unsigned long)pLinkedTarget) {
        pLinkedTargetBytes = (char *)(unsigned long)pLinkedTarget;
        nLinkedTargetVariableBlockOffset =
            *(int *)((char *)(unsigned long)*(int *)(pLinkedTargetBytes + 0x40) + 4);
        paTargetWorkingRect = (short *)(pLinkedTargetBytes + nLinkedTargetVariableBlockOffset + 0x54);
        sWidth = paTargetWorkingRect[0];
        sHeight = paTargetWorkingRect[1];

        if (*(short *)((char *)this + nVariableBlockOffset + 0x58) < 0) {
            *(short *)((char *)this + nVariableBlockOffset + 0x54) =
                (short)(*(short *)((char *)this + nVariableBlockOffset + 0x54) +
                        *(short *)((char *)this + nVariableBlockOffset + 0x58));
            *(short *)((char *)this + nVariableBlockOffset + 0x58) = 0;
        }
        if (sWidth < (short)(*(short *)((char *)this + nVariableBlockOffset + 0x54) +
                             *(short *)((char *)this + nVariableBlockOffset + 0x58))) {
            *(short *)((char *)this + nVariableBlockOffset + 0x54) =
                (short)(sWidth - *(short *)((char *)this + nVariableBlockOffset + 0x58));
        }
        if (*(short *)((char *)this + nVariableBlockOffset + 0x5a) < 0) {
            *(short *)((char *)this + nVariableBlockOffset + 0x56) =
                (short)(*(short *)((char *)this + nVariableBlockOffset + 0x56) +
                        *(short *)((char *)this + nVariableBlockOffset + 0x5a));
            *(short *)((char *)this + nVariableBlockOffset + 0x5a) = 0;
        }
        if (sHeight < (short)(*(short *)((char *)this + nVariableBlockOffset + 0x56) +
                              *(short *)((char *)this + nVariableBlockOffset + 0x5a))) {
            *(short *)((char *)this + nVariableBlockOffset + 0x56) =
                (short)(sHeight - *(short *)((char *)this + nVariableBlockOffset + 0x5a));
        }
        if (*(short *)((char *)this + nVariableBlockOffset + 0x54) <= 0 ||
            *(short *)((char *)this + nVariableBlockOffset + 0x56) <= 0) {
            *(short *)((char *)this + nVariableBlockOffset + 0x56) = 0;
            *(short *)((char *)this + nVariableBlockOffset + 0x54) = 0;
            *(short *)((char *)this + nVariableBlockOffset + 0x5a) = 0;
            *(short *)((char *)this + nVariableBlockOffset + 0x58) = 0;
        }

        paWorkingRect = (short *)((char *)this + nVariableBlockOffset + 0x54);
        *(short *)((char *)this + nVariableBlockOffset + 0x64) = paWorkingRect[0];
        *(short *)((char *)this + nVariableBlockOffset + 0x66) = paWorkingRect[1];
        ComputeHelperBackingDimensions(this, aRectBuffer, paWorkingRect, (int)paWorkingRect[0]);
        *(int *)((char *)this + 0x524) = 0;
        EnsureHelperBackingRowIndexCapacity(this);
        return;
    }

    *(unsigned short *)((char *)this + nVariableBlockOffset + 0x54) =
        (unsigned short)(*(short *)((char *)this + nVariableBlockOffset + 0x54) + 3U) & 0xfffc;
    *(short *)((char *)this + nVariableBlockOffset + 0x64) = *(short *)((char *)this + nVariableBlockOffset + 0x54);
    *(short *)((char *)this + nVariableBlockOffset + 0x66) = *(short *)((char *)this + nVariableBlockOffset + 0x56);

    paWorkingRect = (short *)((char *)this + nVariableBlockOffset + 0x54);
    ComputeHelperBackingDimensions(this, aRectBuffer, paWorkingRect, *(int *)((char *)this + nVariableBlockOffset + 0x80));
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

    if (*(short *)((char *)this + nVariableBlockOffset + 0x54) == 0 ||
        *(short *)((char *)this + nVariableBlockOffset + 0x56) == 0) {
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
            *(int *)((char *)this + 0xfc) = 0x28;
            *(short *)((char *)this + 0x10a) = 8;
            nBitmapHandle = ((int (*)(void *, int, void *))pDisplayVtable[0x10 / sizeof(void *)])(
                g_pDisplayState, *(int *)((char *)this + 0xf8), (char *)this + 0xfc);
            *(int *)((char *)this + 0xf0) = nBitmapHandle;
            if (nBitmapHandle != 0) {
                ((void (*)(void *, int, int))pDisplayVtable[0x24 / sizeof(void *)])(
                    g_pDisplayState, *(int *)((char *)this + 0xf8), nBitmapHandle);
                *(int *)((char *)this + 0x524) =
                    (int)*(short *)((char *)this + nVariableBlockOffset + 0x54) *
                    (int)*(short *)((char *)this + nVariableBlockOffset + 0x56);
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
        aRectBuffer[0] = *(short *)((char *)this + nVariableBlockOffset + 0x4c);
        aRectBuffer[1] = *(short *)((char *)this + nVariableBlockOffset + 0x4e);
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
    void *pvSlotManager;
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

    pvSlotManager = AllocateVSMemBlock(0x14);
    if (pvSlotManager == 0) {
        g_pResourceGeometryHelperSlotManager = 0;
    } else {
        g_pResourceGeometryHelperSlotManager =
            InitializeResourceGeometryHelperSlotManager(pvSlotManager, (int)g_StartupGraphicsDriverConfig.m_cbSize);
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
