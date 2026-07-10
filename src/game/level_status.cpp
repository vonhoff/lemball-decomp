#include "../game/game_app.h"
#include "../platform/startup_options.h"
#include "../game/demo_playback.h"
#include "../resource/resource_archive.h"
#include "../engine/graphics_driver.h"
#include "../engine/memory_arena.h"
#include "../network/safe_vtable.h"

extern "C" DWORD timeGetTime(void);

static void *g_LEVEL_QueuedRenderPointSinkFinalizeVtable = NetworkGetSafeVtable();
static void *g_LEVEL_CompositePointRectSinkEntryInitVtable = NetworkGetSafeVtable();
static void *g_LEVEL_FramedScreenRenderChildEntryVtable = NetworkGetSafeVtable();
static void *g_LEVEL_PackagedSpriteRenderEntryVtable = NetworkGetSafeVtable();
static void *g_LEVEL_StatusIndicatorPointSinkEntryVtable = NetworkGetSafeVtable();
static void *g_LEVEL_RenderPointRectSinkEntryVtable = NetworkGetSafeVtable();
static void *g_LEVEL_PackagedRectQueueEntryDeleteVtable = NetworkGetSafeVtable();
static void *g_LEVEL_ActionButtonPointSinkDeleteVtable = NetworkGetSafeVtable();
static void *g_LEVEL_VariantRenderManagerVtable = NetworkGetSafeVtable();
static void *g_LEVEL_StatusIndicatorManagerVtable = NetworkGetSafeVtable();
static void *g_LEVEL_StatusIndicatorManagerSlot70Vtable = NetworkGetSafeVtable();
static void *g_LEVEL_StatusIndicatorManagerSlot74Vtable = NetworkGetSafeVtable();
static void *g_LEVEL_TimedVariantFrameIndexSelectorCtorVtable = NetworkGetSafeVtable();
static void *g_LEVEL_TimedVariantFrameIndexSelectorVtable = NetworkGetSafeVtable();
static void *g_LEVEL_TimedVariantFrameIndexSelectorCallbackVtable = NetworkGetSafeVtable();
static int *g_LEVEL_StatusIndicatorModeResourceIds;
static unsigned int *g_LEVEL_CompactStatusIndicatorPointTable;
static unsigned int *g_LEVEL_StandardStatusIndicatorPointTable;
int g_fLevelScreenStatusIndicatorConfigured = 0;

int g_nLevelScreenCompositeSequencePrimaryResourceId = 0;
int g_nLevelScreenCompositeSequenceSecondaryResourceId = 0;
int g_nLevelScreenContextSelectedVariantResourceId = 0;
int g_nLevelScreenMappedVariantResourceId0x202 = 0;
int g_nLevelScreenMappedVariantResourceId0x206 = 0;
int g_nLevelScreenMappedVariantResourceId0x207 = 0;
int g_nLevelScreenMappedVariantResourceId0x208 = 0;
int g_nLevelScreenMappedVariantResourceId0x209 = 0;
int g_nLevelScreenMappedVariantResourceId0x20A = 0;
int g_nLevelScreenMappedVariantResourceId0x20B = 0;
int g_nLevelScreenMappedVariantResourceId0x20C = 0;
int g_nLevelScreenMappedVariantResourceId0x20D = 0;
int g_nLevelScreenMappedVariantResourceId0x20E = 0;
int g_nLevelScreenMappedVariantResourceId0x20F = 0;
int g_nLevelScreenMultiStageSequencePrimaryResourceId = 0;
int g_nLevelScreenMultiStageSequenceSecondaryResourceId = 0;
int g_nLevelScreenTimedVariantResourceId = 0;
int g_nLevelScreenMultiPhaseSequenceResourceId = 0;

extern void *g_pQueuedRenderPointSinkFinalizeThunk;
extern void *g_pMainResourceArchive;
extern int g_fRootHelperGeometryDispatchSuppressed;
extern void InitializeHelperUploadStatePending(int nUploadState);
extern void *LoadBitmapResource(int nResourceId);
extern void *LoadPalResource(int nResourceId);
extern void *LoadZrleOnlyListResource(int nResourceId);
extern void *LoadZrleResource(int nResourceId);
extern void ReleaseTypedResourceObjectReference(void *pResourceObject);
extern void PruneUnreferencedCachedResourceObjects(void *pArchive);
extern void SampleRootHelperGeometryAndDispatchRenderGroups(void *pPrimaryContext, int nToken);

void SetStatusIndicatorActive(void *pObject, int fActive);

// FUNCTION: LEMBALL 0x0046B310
void SetStatusIndicatorZrleResource(void *pObject, int nResourceId) {
    int *pClient;
    int nOffset;

    pClient = (int *)pObject;
    if (nResourceId != pClient[0xa]) {
        if (pClient[0xa] != 0) {
            ReleaseTypedResourceObjectReference((void *)(unsigned long)pClient[8]);
        }
        pClient[0xa] = nResourceId;
        if (nResourceId != 0) {
            pClient[8] = (int)(unsigned long)LoadZrleResource(nResourceId);
            nOffset = 0;
            do {
                nOffset += 0x18;
                *(int *)(pClient[7] - 0x10 + nOffset) = pClient[8];
            } while (nOffset < 0x18);
            return;
        }
        pClient[8] = 0;
    }
}

// FUNCTION: LEMBALL 0x0046B3B0
void SetStatusIndicatorListResourceEntry(void *pObject, int nResourceId, int nEntryIndex) {
    int *pClient;
    int nOffset;

    pClient = (int *)pObject;
    if (nResourceId != pClient[0xa]) {
        if (pClient[0xa] != 0) {
            ReleaseTypedResourceObjectReference((void *)(unsigned long)pClient[8]);
        }
        pClient[0xa] = nResourceId;
        pClient[9] = -1;
        if (nResourceId == 0) {
            pClient[8] = 0;
        } else {
            pClient[8] = (int)(unsigned long)LoadZrleOnlyListResource(nResourceId);
        }
    }

    if (pClient[9] != nEntryIndex && pClient[8] != 0) {
        pClient[9] = nEntryIndex;
        nOffset = 0;
        do {
            nOffset += 0x18;
            *(int *)(pClient[7] - 0x10 + nOffset) = *(int *)(pClient[8] + 0x78) + pClient[9] * 0x54;
        } while (nOffset < 0x18);
    }
}

// FUNCTION: LEMBALL 0x0043A720
void SetLevelScreenStatusIndicatorMode(int nMode, int nValue) {
    int nResourceId;

    switch (nMode) {
    case 0:
        SetStatusIndicatorActive(g_pArrowCursorStatusIndicatorRenderClient, 0);
        SetStatusIndicatorZrleResource(g_pArrowCursorStatusIndicatorRenderClient, g_LEVEL_StatusIndicatorModeResourceIds[0]);
        return;

    case 1:
        if (g_pLevelDemoPlaybackController != 0 && *(int *)((char *)g_pLevelDemoPlaybackController + 0x4c) != 0) {
            nValue = 0;
        }
        SetStatusIndicatorListResourceEntry(g_pArrowCursorStatusIndicatorRenderClient,
                                            g_LEVEL_StatusIndicatorModeResourceIds[1],
                                            nValue);
        if (g_fLevelScreenStatusIndicatorConfigured == 0) {
            *(int *)((char *)g_pArrowCursorStatusIndicatorRenderClient + 0x2c) = 1;
            g_fLevelScreenStatusIndicatorConfigured = 1;
        }
        SetStatusIndicatorActive(g_pArrowCursorStatusIndicatorRenderClient, 1);
        *(unsigned short *)((char *)g_pArrowCursorStatusIndicatorRenderClient + 0x14) = 3;
        *(unsigned short *)((char *)g_pArrowCursorStatusIndicatorRenderClient + 0x16) = 3;
        return;

    case 2:
    case 3:
        nResourceId = g_LEVEL_StatusIndicatorModeResourceIds[nMode];
        SetStatusIndicatorZrleResource(g_pArrowCursorStatusIndicatorRenderClient, nResourceId);
        if (g_fLevelScreenStatusIndicatorConfigured == 0) {
            *(int *)((char *)g_pArrowCursorStatusIndicatorRenderClient + 0x2c) = 1;
            *(unsigned short *)((char *)g_pArrowCursorStatusIndicatorRenderClient + 0x14) = 3;
            *(unsigned short *)((char *)g_pArrowCursorStatusIndicatorRenderClient + 0x16) = 3;
            g_fLevelScreenStatusIndicatorConfigured = 1;
        }
        SetStatusIndicatorActive(g_pArrowCursorStatusIndicatorRenderClient, 1);
        break;
    }
}

// FUNCTION: LEMBALL 0x00467AC0
void *InitializePackagedRectQueueEntry(void *pObject) {
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(void **)pEntry = g_LEVEL_PackagedRectQueueEntryDeleteVtable;
    *(unsigned short *)(pEntry + 1) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00403265
void *InitializeBaseModeActionButtonPointSink(void *pObject) {
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(void **)pEntry = g_LEVEL_ActionButtonPointSinkDeleteVtable;
    *(unsigned short *)(pEntry + 1) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x004678C0
void ResetOrFlipVariantRenderBuffers(int nManager) {
    int nTarget;
    unsigned char *pbToken;

    *(int *)(unsigned long)(nManager + 0xc) = 0;
    if (*(int *)(unsigned long)(nManager + 0x38) == 0) {
        *(int *)(unsigned long)(nManager + 0x5c) = 0;
        *(int *)(unsigned long)(nManager + 0x40) = *(int *)(unsigned long)(nManager + 0x5c);
        *(int *)(unsigned long)(nManager + 0x44) = *(int *)(unsigned long)(nManager + 0x60);
        nTarget = *(int *)(*(int *)(unsigned long)(nManager + 0x1c) + 0xc);
        pbToken =
            (unsigned char *)(**(int (***)(void))(*(int *)(*(int *)(*(int *)(unsigned long)(nTarget + 0x40) + 4) + 0x40 +
                                                     nTarget) +
                                                    0x38))();
        *(unsigned char *)(unsigned long)(nManager + 0x44) = *pbToken;
        *(unsigned char *)(unsigned long)(nManager + 0x60) = *pbToken;
        return;
    }

    *(int *)(unsigned long)(*(int *)(unsigned long)(nManager + 0x1c) + 4) = 0;
    *(int *)(unsigned long)(nManager + 0x4c) = 0;
    *(int *)(unsigned long)(nManager + 0x48) = 0;
    *(unsigned int *)(unsigned long)(nManager + 0x50) ^= 1;
}

// FUNCTION: LEMBALL 0x00467260
void *ConstructVariantRenderManager(void *pObject,
                                    int nOwner,
                                    int cResourceIds,
                                    int cPackagedRectEntries,
                                    int cActionButtonEntries,
                                    int nUnknown58,
                                    int fDoubleBuffered) {
    int *pManager;
    int i;
    int cEntries;
    int *pEntry;

    pManager = (int *)pObject;
    *(void **)pManager = g_LEVEL_VariantRenderManagerVtable;
    pManager[0x10] = 0;
    pManager[0x17] = 0;
    pManager[7] = nOwner;
    pManager[0xb] = cPackagedRectEntries;
    pManager[0xc] = cResourceIds;
    pManager[0x15] = 0;
    pManager[0x19] = 0;
    pManager[8] = 0;
    pManager[0xe] = fDoubleBuffered;
    pManager[0xd] = 0;
    pManager[0x1b] = -1;
    pManager[9] = (int)(unsigned long)AllocateVSMemBlock((unsigned int)cPackagedRectEntries * 4U);
    pManager[10] = (int)(unsigned long)AllocateVSMemBlock((unsigned int)cResourceIds * 2U);

    for (i = 0; i < pManager[0xb]; ++i) {
        *(int *)(unsigned long)(pManager[9] + i * 4) = 0;
    }

    for (i = 0; i < pManager[0xc]; ++i) {
        *(short *)(unsigned long)(pManager[10] + i * 2) = (short)pManager[0xb];
    }

    if (pManager[0xe] == 0) {
        pManager[0x16] = nUnknown58;
        pManager[0xf] = cActionButtonEntries;
    } else {
        pManager[0x14] = 0;
        pManager[0x16] = nUnknown58 * 2;
        pManager[0x13] = 0;
        pManager[0x12] = 0;
        pManager[0xf] = cActionButtonEntries * 2;
    }

    cEntries = pManager[0x16];
    if (cEntries != 0) {
        pEntry = (int *)AllocateVSMemBlock((unsigned int)(cEntries * 0x18 + 4));
        if (pEntry == 0) {
            pManager[0x19] = 0;
        } else {
            *pEntry = cEntries;
            ++pEntry;
            while (--cEntries >= 0) {
                InitializePackagedRectQueueEntry(pEntry);
                pEntry += 6;
            }
            pManager[0x19] = (int)(unsigned long)pEntry - 0x18 * pManager[0x16];
        }
    }

    cEntries = pManager[0xf];
    if (cEntries != 0) {
        pEntry = (int *)AllocateVSMemBlock((unsigned int)(cEntries * 0x20 + 4));
        if (pEntry == 0) {
            pManager[0x15] = 0;
        } else {
            *pEntry = cEntries;
            ++pEntry;
            while (--cEntries >= 0) {
                InitializeBaseModeActionButtonPointSink(pEntry);
                pEntry += 8;
            }
            pManager[0x15] = (int)(unsigned long)pEntry - 0x20 * pManager[0xf];
        }
    }

    pManager[4] = 0;
    pManager[1] = 0;
    ResetOrFlipVariantRenderBuffers((int)(unsigned long)pManager);
    return pObject;
}

// FUNCTION: LEMBALL 0x00467490
void LoadVariantRenderResource(void *pObject, int nResourceId) {
    int *pManager;
    short *pnResourceIndex;
    int iSlot;
    int *pnLoadedResource;

    pManager = (int *)pObject;
    iSlot = (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2);
    if (pManager[0xb] == iSlot) {
        pnLoadedResource = (int *)(unsigned long)pManager[9];
        iSlot = 0;
        while (*pnLoadedResource != 0) {
            ++pnLoadedResource;
            ++iSlot;
        }
    }

    *(void **)(unsigned long)(pManager[9] + iSlot * 4) = LoadZrleOnlyListResource(nResourceId);
    if (*(int *)(unsigned long)(pManager[9] + iSlot * 4) == 0) {
        *(void **)(unsigned long)(pManager[9] + iSlot * 4) = LoadZrleResource(nResourceId);
    }

    pnResourceIndex = (short *)(unsigned long)(pManager[10] + nResourceId * 2);
    if ((int)*pnResourceIndex == pManager[0xb]) {
        *pnResourceIndex = (short)iSlot;
        ++pManager[0xd];
    }
}

// FUNCTION: LEMBALL 0x00467500
void ReleaseVariantRenderResource(void *pObject, int nResourceId) {
    int *pManager;
    int iSlot;

    pManager = (int *)pObject;
    iSlot = (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2);
    ReleaseTypedResourceObjectReference(*(void **)(unsigned long)(pManager[9] + iSlot * 4));
    *(int *)(unsigned long)(pManager[9] + iSlot * 4) = 0;
    *(short *)(unsigned long)(pManager[10] + nResourceId * 2) = (short)pManager[0xb];
    --pManager[0xd];
}

// FUNCTION: LEMBALL 0x00467540
int GetVariantRenderFrameCount(void *pObject, int nResourceId) {
    int *pManager;
    int *pResourceObject;

    pManager = (int *)pObject;
    pResourceObject =
        *(int **)(unsigned long)(pManager[9] + (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2) * 4);
    if (pResourceObject[0x10] == 0x5a524c45) {
        return 1;
    }
    return pResourceObject[0x1b];
}

// FUNCTION: LEMBALL 0x004671B0
void SetTimedVariantFrameDuration(void *pObject, int nFrameDuration) {
    int *pSelector;

    pSelector = (int *)pObject;
    pSelector[4] = nFrameDuration;
    if (pSelector[5] == -1) {
        pSelector[3] = (int)timeGetTime();
    } else {
        pSelector[3] = pSelector[5];
    }
}

// FUNCTION: LEMBALL 0x0046B370
void SetStatusIndicatorActive(void *pObject, int fActive) {
    int *pClient;
    DWORD dwNow;

    pClient = (int *)pObject;
    if (pClient[6] != fActive) {
        pClient[6] = fActive;
        if (fActive == 0) {
            ((void (*)())(*(void ***)pClient)[5])();
            return;
        }
        pClient[0x17] = 0;
        pClient[0x18] = 0;
        pClient[0x19] = 0;
        pClient[0x1a] = 0;
        dwNow = timeGetTime();
        pClient[0x1b] = (int)dwNow;
        pClient[0x1c] = (int)dwNow;
    }
}

// FUNCTION: LEMBALL 0x004030BC
void *InitializeFramedScreenRenderChildEntry(void *pObject) {
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(unsigned short *)(pEntry + 1) = 0;
    *(void **)pEntry = g_LEVEL_CompositePointRectSinkEntryInitVtable;
    *(unsigned short *)((char *)pEntry + 10) = 0;
    *(unsigned short *)(pEntry + 2) = 0;
    *(void **)pEntry = g_LEVEL_FramedScreenRenderChildEntryVtable;
    *(unsigned short *)((char *)pEntry + 0xe) = 0;
    *(unsigned short *)(pEntry + 3) = 0;
    *(unsigned short *)((char *)pEntry + 0x22) = 0;
    *(unsigned short *)(pEntry + 8) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x0040272F
void *InitializePackagedSpriteRenderEntry(void *pObject) {
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(void **)pEntry = g_LEVEL_PackagedSpriteRenderEntryVtable;
    *(unsigned short *)(pEntry + 1) = 0;
    *(unsigned short *)((char *)pEntry + 10) = 0;
    *(unsigned short *)(pEntry + 2) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x0040135C
void *InitializeRenderPointRectSinkEntry(void *pObject) {
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(unsigned short *)(pEntry + 1) = 0;
    *(void **)pEntry = g_LEVEL_RenderPointRectSinkEntryVtable;
    *(unsigned short *)((char *)pEntry + 10) = 0;
    *(unsigned short *)(pEntry + 2) = 0;
    *(unsigned short *)((char *)pEntry + 0xe) = 0;
    *(unsigned short *)(pEntry + 3) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00401C7B
void *InitializeStatusIndicatorPointSinkEntry(void *pObject) {
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(void **)pEntry = g_LEVEL_StatusIndicatorPointSinkEntryVtable;
    *(unsigned short *)(pEntry + 1) = 0;
    *(unsigned short *)((char *)pEntry + 10) = 0;
    *(unsigned short *)(pEntry + 2) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00402784
void *InitializeLevelSelectionPointRectSinkEntry(void *pObject) {
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(unsigned short *)(pEntry + 1) = 0;
    *(void **)pEntry = g_LEVEL_CompositePointRectSinkEntryInitVtable;
    *(unsigned short *)((char *)pEntry + 10) = 0;
    *(unsigned short *)(pEntry + 2) = 0;
    *(unsigned short *)((char *)pEntry + 0xe) = 0;
    *(unsigned short *)(pEntry + 3) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00401974
void RestoreFramedScreenRenderChildEntryVtable(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
}

// FUNCTION: LEMBALL 0x00401DDE
void RestorePackagedSpriteRenderEntryVtableA(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
}

// FUNCTION: LEMBALL 0x00401B8B
void RestorePackagedSpriteRenderEntryVtableE(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
}

// FUNCTION: LEMBALL 0x00401307
void RestoreStatusIndicatorPointSinkEntryVtable(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
}

// FUNCTION: LEMBALL 0x004022A2
void RestoreLevelSelectionPointSinkFinalizeVtable(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
}

// FUNCTION: LEMBALL 0x004673D0
void DestroyVariantRenderManager(void *pObject) {
    int *pManager;
    int iLoaded;
    int iSlot;
    int *pnLoadedResource;

    pManager = (int *)pObject;
    *(void **)pManager = g_LEVEL_VariantRenderManagerVtable;
    iSlot = 0;
    if (pManager[0xd] != 0) {
        iLoaded = 0;
        while (iLoaded < pManager[0xd]) {
            pnLoadedResource = (int *)(unsigned long)(pManager[9] + iSlot * 4);
            while (*pnLoadedResource == 0) {
                ++pnLoadedResource;
                ++iSlot;
            }
            ReleaseTypedResourceObjectReference(*(void **)(unsigned long)(pManager[9] + iSlot * 4));
            ++iLoaded;
            ++iSlot;
        }
    }

    if (pManager[9] != 0) {
        FreeVSMemBlock((void *)(unsigned long)pManager[9]);
        pManager[9] = 0;
    }
    if (pManager[10] != 0) {
        FreeVSMemBlock((void *)(unsigned long)pManager[10]);
        pManager[10] = 0;
    }
    if (pManager[0x19] != 0) {
        ((void (*)(int))**(void ***)(unsigned long)pManager[0x19])(3);
        pManager[0x19] = 0;
    }
    if (pManager[0x15] != 0) {
        ((void (*)(int))**(void ***)(unsigned long)pManager[0x15])(3);
        pManager[0x15] = 0;
    }
    if (pManager[4] != 0 && pManager[1] != 0) {
        ((void (*)(int))**(void ***)(unsigned long)pManager[1])(3);
        pManager[1] = 0;
    }
}

// FUNCTION: LEMBALL 0x00401CD5
void ReleaseLevelScreenVariantStateWrapper(void *pObject, int nResourceId) {
    int *pManager;
    unsigned int uStatePointer;

    pManager = (int *)pObject;
    uStatePointer = *(unsigned int *)(unsigned long)(*(int *)(unsigned long)(pManager[0x1d] + 4) +
                                                     (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2) * 4);
    if (uStatePointer != 0) {
        FreeVSMemBlock((void *)(unsigned long)uStatePointer);
        *(int *)(unsigned long)(*(int *)(unsigned long)(pManager[0x1d] + 4) +
                                (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2) * 4) = 0;
    }
    ReleaseVariantRenderResource(pObject, nResourceId);
}

// FUNCTION: LEMBALL 0x00402F1D
void ReleaseLevelScreenVariantStateWrapperRange(void *pObject, int nFirstResourceId, int nLastResourceId) {
    int nResourceId;
    int *pManager;
    unsigned int uStatePointer;

    pManager = (int *)pObject;
    for (nResourceId = nFirstResourceId; nResourceId <= nLastResourceId; ++nResourceId) {
        uStatePointer = *(unsigned int *)(unsigned long)(*(int *)(unsigned long)(pManager[0x1d] + 4) +
                                                         (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2) *
                                                             4);
        if (uStatePointer != 0) {
            FreeVSMemBlock((void *)(unsigned long)uStatePointer);
            *(int *)(unsigned long)(*(int *)(unsigned long)(pManager[0x1d] + 4) +
                                    (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2) * 4) = 0;
        }
        ReleaseVariantRenderResource(pObject, nResourceId);
    }
}

// FUNCTION: LEMBALL 0x00402FEF
int ReturnTrueVirtualHook(void) {
    return 1;
}

// FUNCTION: LEMBALL 0x0044AA80
void *ConstructLevelScreenStatusIndicatorManager(void *pObject, int nStatusMode, void *pPrimaryContext) {
    int *pManager;
    unsigned int *puPointTable;
    int i;
    int nFrameCount;
    int *pSelector;
    short *paPoint;
    int *pnPrimaryContext;

    pManager = (int *)pObject;
    pnPrimaryContext = (int *)pPrimaryContext;

    ConstructVariantRenderManager(pObject, (int)(unsigned long)pPrimaryContext, 0x2b6, 1, 1, 0, 0);
    *(void **)(pManager + 0x1c) = g_LEVEL_StatusIndicatorManagerSlot70Vtable;
    *(void **)(pManager + 0x1d) = g_LEVEL_StatusIndicatorManagerSlot74Vtable;
    *(unsigned short *)((char *)pManager + 0x8a) = 0;
    *(unsigned short *)((char *)pManager + 0x88) = 0;

    for (i = 0; i < 1; ++i) {
        InitializeFramedScreenRenderChildEntry((char *)pManager + 0x9c + i * 0x24);
    }
    for (i = 0; i < 1; ++i) {
        InitializeLevelSelectionPointRectSinkEntry((char *)pManager + 0xc0 + i * 0x1c);
    }
    for (i = 0; i < 1; ++i) {
        InitializePackagedSpriteRenderEntry((char *)pManager + 0xe0 + i * 0x10);
    }
    for (i = 0; i < 1; ++i) {
        InitializeStatusIndicatorPointSinkEntry((char *)pManager + 0xf0 + i * 0x10);
        InitializeStatusIndicatorPointSinkEntry((char *)pManager + 0x100 + i * 0x10);
        InitializeStatusIndicatorPointSinkEntry((char *)pManager + 0x110 + i * 0x10);
    }

    *(void **)((char *)pManager + 0x120) = &g_pQueuedRenderPointSinkFinalizeThunk;
    for (i = 0; i < 1; ++i) {
        InitializeRenderPointRectSinkEntry((char *)pManager + 0x124 + i * 0x10);
    }

    *(void **)pManager = g_LEVEL_StatusIndicatorManagerVtable;
    *(void **)(pManager + 0x1f) = pPrimaryContext;
    *(void **)(pManager + 0x1c) = g_LEVEL_StatusIndicatorManagerSlot70Vtable;
    *(void **)(pManager + 0x1d) = g_LEVEL_StatusIndicatorManagerSlot74Vtable;
    pManager[0x20] = nStatusMode;

    paPoint = (short *)AllocateVSMemBlock(0x14);
    if (paPoint == 0) {
        pManager[0x1e] = 0;
    } else {
        for (i = 0; i < 5; ++i) {
            paPoint[i * 2] = 0;
            paPoint[i * 2 + 1] = 0;
        }
        pManager[0x1e] = (int)(unsigned long)paPoint;
    }

    SetStatusIndicatorActive(g_pArrowCursorStatusIndicatorRenderClient, 0);
    if (g_fCompactPrimaryContextLayout == 0) {
        pManager[0x25] = (int)(unsigned long)LoadBitmapResource(0xf1);
        puPointTable = g_LEVEL_StandardStatusIndicatorPointTable;
        pManager[0x26] = (int)(unsigned long)LoadBitmapResource(0xf2);
        pManager[0x21] = 0xf3;
    } else {
        pManager[0x25] = (int)(unsigned long)LoadBitmapResource(0xee);
        puPointTable = g_LEVEL_CompactStatusIndicatorPointTable;
        pManager[0x26] = (int)(unsigned long)LoadBitmapResource(0xef);
        pManager[0x21] = 0xf0;
    }

    LoadVariantRenderResource(pManager, pManager[0x21]);
    {
        void *pPaletteObject;

        pPaletteObject = LoadPalResource(0xed);
        if (*(int *)(pnPrimaryContext[0x1f] + 4) == 1) {
            SampleRootHelperGeometryAndDispatchRenderGroups(pPrimaryContext, -1);
        }
        ((void (*)(int))(*(void ***)pPrimaryContext)[0x2b])(0xed);
        ReleaseTypedResourceObjectReference(pPaletteObject);
    }

    *(unsigned short *)((char *)pManager + 0x8a) =
        (short)((*(short *)(pnPrimaryContext[0x1f] + 10) - *(short *)(unsigned long)(pManager[0x25] + 0x4a)) / 2);
    *(unsigned short *)((char *)pManager + 0x88) =
        (short)((*(short *)(pnPrimaryContext[0x1f] + 8) - *(short *)(unsigned long)(pManager[0x25] + 0x48)) / 2);

    for (i = 0; i < 5; ++i) {
        paPoint = (short *)(unsigned long)pManager[0x1e];
        paPoint[i * 2] = (short)puPointTable[i];
        paPoint[i * 2 + 1] = (short)(puPointTable[i] >> 16);
    }

    *(unsigned short *)((char *)pManager + 0x90) = 0;
    pManager[0x23] = 1;
    nFrameCount = GetVariantRenderFrameCount(pManager, pManager[0x21]);

    pSelector = (int *)AllocateVSMemBlock(0x1c);
    if (pSelector == 0) {
        pManager[0x37] = 0;
    } else {
        *(void **)pSelector = g_LEVEL_TimedVariantFrameIndexSelectorCtorVtable;
        pSelector[1] = nFrameCount;
        *(void **)pSelector = g_LEVEL_TimedVariantFrameIndexSelectorVtable;
        pSelector[6] = 1;
        *(void **)pSelector = g_LEVEL_TimedVariantFrameIndexSelectorCallbackVtable;
        pManager[0x37] = (int)(unsigned long)pSelector;
    }

    *(int *)(unsigned long)(pManager[0x37] + 0x14) = -1;
    SetTimedVariantFrameDuration((void *)(unsigned long)pManager[0x37], nFrameCount * 0x42);
    return pObject;
}

// FUNCTION: LEMBALL 0x004338B0
void ReleaseLevelSelectionModeRenderStateResource(void *pObject) {
    int *pManager;

    pManager = (int *)pObject;
    if (pManager[0x21] == 0) {
        return;
    }

    ReleaseVariantRenderResource(pObject, 0xf6);
    ReleaseVariantRenderResource(pObject, 0xf7);
    ReleaseVariantRenderResource(pObject, 0xf9);
    ReleaseVariantRenderResource(pObject, 0xfa);

    if (pManager[0x27] != 0) {
        ReleaseTypedResourceObjectReference((void *)(unsigned long)pManager[0x27]);
    }
    if (pManager[0x28] != 0) {
        ReleaseTypedResourceObjectReference((void *)(unsigned long)pManager[0x28]);
    }
    if (pManager[0x29] != 0) {
        ReleaseTypedResourceObjectReference((void *)(unsigned long)pManager[0x29]);
    }

    ReleaseLevelScreenVariantStateWrapper(pObject, 0xfd);
    ReleaseLevelScreenVariantStateWrapperRange(pObject, 0x35, 0x3c);
    ReleaseLevelScreenVariantStateWrapperRange(pObject, 0x47, 0x4e);
    ReleaseLevelScreenVariantStateWrapperRange(pObject, 0x4f, 0x56);
    ReleaseLevelScreenVariantStateWrapperRange(pObject, 0x5f, 0x66);

    if (ReturnTrueVirtualHook() != 0 || ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x90);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x91);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x92);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x93);
    }

    ReleaseLevelScreenVariantStateWrapper(pObject, 0x6f);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x70);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x71);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x3d);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x3e);

    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapperRange(pObject, 0xa1, 0xa8);
        ReleaseLevelScreenVariantStateWrapperRange(pObject, 0xa9, 0xac);
    }

    ReleaseLevelScreenVariantStateWrapper(pObject, 0x40);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x41);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x42);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0xaf);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0xad);

    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenCompositeSequencePrimaryResourceId);
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenCompositeSequenceSecondaryResourceId);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x74);
    }

    ReleaseLevelScreenVariantStateWrapper(pObject, 0x7c);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x82);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0xae);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x75);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x76);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x43);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x44);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x46);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x45);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x3f);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x67);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x68);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x69);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x6a);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x6b);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x6c);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x6d);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x6e);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x80);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x81);

    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x28);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x29);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x7d);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x7e);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x85);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x87);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x86);
    }
    if (pManager[0x2a] != 3 && ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenContextSelectedVariantResourceId);
    }

    ReleaseLevelScreenVariantStateWrapper(pObject, 0xdf);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0xe0);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0xe1);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0xdd);

    if (g_nLevelScreenMappedVariantResourceId0x206 != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x206);
    }
    if (g_nLevelScreenMappedVariantResourceId0x207 != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x207);
    }
    if (g_nLevelScreenMappedVariantResourceId0x208 != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x208);
    }
    if (g_nLevelScreenMappedVariantResourceId0x209 != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x209);
    }
    if (g_nLevelScreenMappedVariantResourceId0x20A != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x20A);
    }
    if (g_nLevelScreenMappedVariantResourceId0x20B != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x20B);
    }
    if (g_nLevelScreenMappedVariantResourceId0x20C != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x20C);
    }
    if (g_nLevelScreenMappedVariantResourceId0x20D != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x20D);
    }
    if (g_nLevelScreenMappedVariantResourceId0x20E != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x20E);
    }
    if (g_nLevelScreenMappedVariantResourceId0x20F != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x20F);
    }
    if (g_nLevelScreenMappedVariantResourceId0x202 != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x202);
    }
    if (g_nLevelScreenMultiStageSequencePrimaryResourceId != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMultiStageSequencePrimaryResourceId);
    }
    if (g_nLevelScreenMultiStageSequenceSecondaryResourceId != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMultiStageSequenceSecondaryResourceId);
    }
    if (g_nLevelScreenTimedVariantResourceId != 0 && ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenTimedVariantResourceId);
    }

    ReleaseLevelScreenVariantStateWrapper(pObject, 0x1f);
    if (ReturnTrueVirtualHook() != 0 || ReturnTrueVirtualHook() != 0 || ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0xb2);
    }

    ReleaseLevelScreenVariantStateWrapper(pObject, 0xde);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x83);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x84);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0xa0);

    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x88);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x89);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x8c);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x8d);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x8e);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x8f);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x97);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMultiPhaseSequenceResourceId);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x98);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x9d);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x9f);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x9c);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x9e);
    }
    if (ReturnTrueVirtualHook() != 0 || ReturnTrueVirtualHook() != 0 || ReturnTrueVirtualHook() != 0 ||
        ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x94);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x95);
    }

    ReleaseLevelScreenVariantStateWrapper(pObject, 0x96);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x2a);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x2c);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x2b);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x2d);

    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0xb1);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0xb0);
    }

    PruneUnreferencedCachedResourceObjects(g_pMainResourceArchive);
}

// FUNCTION: LEMBALL 0x0044AD60
void DestroyLevelScreenStatusIndicatorManager(void *pObject) {
    int *pManager;
    int i;

    pManager = (int *)pObject;
    *(void **)pManager = g_LEVEL_StatusIndicatorManagerVtable;
    *(void **)(pManager + 0x1c) = g_LEVEL_StatusIndicatorManagerSlot70Vtable;
    *(void **)(pManager + 0x1d) = g_LEVEL_StatusIndicatorManagerSlot74Vtable;
    FreeVSMemBlock((void *)(unsigned long)pManager[0x1e]);
    FreeVSMemBlock((void *)(unsigned long)pManager[0x37]);
    ReleaseVariantRenderResource(pManager, pManager[0x21]);
    if (pManager[0x25] != 0) {
        ReleaseTypedResourceObjectReference((void *)(unsigned long)pManager[0x25]);
    }
    ReleaseTypedResourceObjectReference((void *)(unsigned long)pManager[0x26]);
    if (*(int *)(pManager[0x1f] + 4) == 1) {
        SampleRootHelperGeometryAndDispatchRenderGroups((void *)(unsigned long)pManager[0x1f], -1);
    }

    for (i = 0; i < 1; ++i) {
        RestorePackagedSpriteRenderEntryVtableE((char *)pManager + 0xe0 - i * 0x10);
    }
    *(void **)(pManager + 0x48) = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    for (i = 0; i < 1; ++i) {
        RestoreStatusIndicatorPointSinkEntryVtable((char *)pManager + 0x120 - (i + 1) * 0x10);
        RestoreStatusIndicatorPointSinkEntryVtable((char *)pManager + 0x110 - i * 0x10);
        RestoreStatusIndicatorPointSinkEntryVtable((char *)pManager + 0x100 - i * 0x10);
    }
    for (i = 0; i < 1; ++i) {
        RestorePackagedSpriteRenderEntryVtableA((char *)pManager + 0xf0 - i * 0x10);
    }
    for (i = 0; i < 1; ++i) {
        RestoreLevelSelectionPointSinkFinalizeVtable((char *)pManager + 0xc0 - i * 0x1c);
    }
    for (i = 0; i < 1; ++i) {
        RestoreFramedScreenRenderChildEntryVtable((char *)pManager + 0x9c - i * 0x24);
    }
    DestroyVariantRenderManager(pObject);
}
#include "../network/safe_vtable.h"
