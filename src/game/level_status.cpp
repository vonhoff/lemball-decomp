#include "../game/game_app.h"
#include "../platform/startup_options.h"
#include "../game/demo_playback.h"
#include "../resource/resource_archive.h"
#include "../engine/graphics_driver.h"
#include "../engine/memory_arena.h"
#include "../network/safe_vtable.h"

extern "C" DWORD timeGetTime(void);
extern void CrtFatalRuntimeError0x19(void);
extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);
extern void NoopVtableCallbackThunk(void);
extern void VariantResourceBundleNoopThunk(void);
void SetTimedVariantFrameDuration(void *pObject, int nFrameDuration);

void *DestroyPackagedRectQueueEntryArray(void *pObject, unsigned int fDelete);
void ResetBaseModeActionButtonPointSink(void *pObject);
void *DestroyBaseModeActionButtonPointSinkArray(void *pObject, unsigned int fDelete);
extern void AppendPointerQueueEntry(void *pQueue, void *pEntry);
static void *DeleteQueuedRenderPointSinkWrapper00432350(void *pObject, BYTE fDelete);
static void *DeleteQueuedRenderPointSinkThunk00403580(void *pObject, BYTE fDelete);
static void *DeleteStatusIndicatorPointSinkEntry00432A90(void *pObject, BYTE fDelete);
static void *DeleteStatusIndicatorPointSinkThunk00401834(void *pObject, BYTE fDelete);
void RestoreStatusIndicatorPointSinkEntryVtable(void *pObject);
void RestorePackagedSpriteRenderEntryVtableA(void *pObject);
static void *DestroyStatusIndicatorPointSinkArray00467BB0(void *pObject, unsigned int fDelete);
static void QueueStatusIndicatorPointSink00432AD0(void *pObject, void *pQueue);
static void QueueStatusIndicatorPointSinkThunk004018AC(void *pObject, void *pQueue);
static void DispatchStatusIndicatorPointSink00432AE0(void *pObject, void *pContext);
static void DispatchStatusIndicatorPointSinkThunk004025AE(void *pObject, void *pContext);
static void UpdateStatusIndicatorLoadProgress00434D10(void *pObject);
static void UpdateStatusIndicatorLoadProgressThunk00401DB1(void *pObject);
static int GetStatusIndicatorPointSinkField00435870(void *pObject);
static int GetStatusIndicatorPointSinkFieldThunk004011B8(void *pObject);
static void QueueHelperLocalRectUpdateEntry00432B10(void *pObject, void *pQueue);
static void QueueHelperLocalRectUpdateThunk00401E6F(void *pObject, void *pQueue);
static void DispatchHelperLocalRectUpdateEntry00432B20(void *pObject, void *pContext);
static void DispatchHelperLocalRectUpdateThunk004037A1(void *pObject, void *pContext);
static void *g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage[8] = {
    (void *)DeleteQueuedRenderPointSinkThunk00403580,
    (void *)CrtFatalRuntimeError0x19,
    (void *)CrtFatalRuntimeError0x19,
    0,
    (void *)DeleteStatusIndicatorPointSinkThunk00401834,
    (void *)QueueHelperLocalRectUpdateThunk00401E6F,
    (void *)DispatchHelperLocalRectUpdateThunk004037A1,
    0,
};
static void *g_LEVEL_QueuedRenderPointSinkFinalizeVtable =
    g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
static void *DeleteFramedScreenRenderChildEntryB(void *pObject, BYTE fDelete);
static void *DeleteFramedScreenRenderChildEntryThunk(void *pObject, BYTE fDelete);
static void QueueFramedScreenRenderEntry(void *pObject, void *pQueue);
static void QueueFramedScreenRenderEntryThunk(void *pObject, void *pQueue);
static void FinalizeFramedScreenRenderEntry(void *pObject, void *pManager);
static void FinalizeFramedScreenRenderEntryThunk(void *pObject, void *pManager);
static void *DeleteBaseModeObjectAdjusted(void *pObject, BYTE fDelete);
static void *DeleteBaseModeObjectAdjustedThunk(void *pObject, BYTE fDelete);
static int HandleBaseModeActionButtonEvent(void *pObject, void *pEvent);
static int HandleBaseModeActionButtonEventThunk(void *pObject, void *pEvent);
static void *g_LEVEL_CompositePointRectSinkEntryInitVtableStorage[8] = {
    (void *)DeleteFramedScreenRenderChildEntryThunk,
    (void *)QueueFramedScreenRenderEntryThunk,
    (void *)FinalizeFramedScreenRenderEntryThunk,
    0,
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)DeleteBaseModeObjectAdjustedThunk,
    (void *)HandleBaseModeActionButtonEventThunk,
    0,
};
static void *g_LEVEL_CompositePointRectSinkEntryInitVtable =
    g_LEVEL_CompositePointRectSinkEntryInitVtableStorage;
static void *DeleteFramedScreenRenderChildEntryA(void *pObject, BYTE fDelete);
static void *DeleteFramedScreenRenderChildEntryAThunk(void *pObject, BYTE fDelete);
static void QueueBitmapRenderEntry(void *pObject, void *pQueue);
static void QueueBitmapRenderEntryThunk(void *pObject, void *pQueue);
static void RenderBitmapEntryViaContext(void *pObject, void *pManager);
static void RenderBitmapEntryViaContextThunk(void *pObject, void *pManager);
static void *g_LEVEL_FramedScreenRenderChildEntryVtableStorage[8] = {
    (void *)DeleteFramedScreenRenderChildEntryAThunk,
    (void *)QueueBitmapRenderEntryThunk,
    (void *)RenderBitmapEntryViaContextThunk,
    0,
    (void *)DeleteFramedScreenRenderChildEntryThunk,
    (void *)QueueFramedScreenRenderEntryThunk,
    (void *)FinalizeFramedScreenRenderEntryThunk,
    0,
};
static void *g_LEVEL_FramedScreenRenderChildEntryVtable =
    g_LEVEL_FramedScreenRenderChildEntryVtableStorage;
static void *DestroyPackagedSpriteRenderEntryArray00469930(void *pObject, unsigned int fDelete);
static void QueuePackagedSpriteRenderEntry00439800(void *pObject, void *pQueue);
static void QueuePackagedSpriteRenderEntryThunk00403846(void *pObject, void *pQueue);
static void DispatchPackagedSpriteRenderEntry00439810(void *pObject, void *pContext);
static void DispatchPackagedSpriteRenderEntryThunk00402A8B(void *pObject, void *pContext);
static void *DeletePackagedSpriteRenderEntry00439400(void *pObject, BYTE fDelete);
static void *DeletePackagedSpriteRenderEntryThunk004016F9(void *pObject, BYTE fDelete);
static void NoopPackagedSpriteRenderEntry00439370(void);
static void NoopPackagedSpriteRenderEntryThunk00402A22(void);
static void NoopPackagedSpriteRenderEntry00439380(void);
static void NoopPackagedSpriteRenderEntryThunk00401B77(void);
static void *g_LEVEL_PackagedSpriteRenderEntryVtableStorage[8] = {
    (void *)DestroyPackagedSpriteRenderEntryArray00469930,
    (void *)QueuePackagedSpriteRenderEntryThunk00403846,
    (void *)DispatchPackagedSpriteRenderEntryThunk00402A8B,
    0,
    (void *)DeletePackagedSpriteRenderEntryThunk004016F9,
    (void *)NoopPackagedSpriteRenderEntryThunk00402A22,
    (void *)CrtFatalRuntimeError0x19,
    (void *)NoopPackagedSpriteRenderEntryThunk00401B77,
};
static void *g_LEVEL_PackagedSpriteRenderEntryVtable =
    g_LEVEL_PackagedSpriteRenderEntryVtableStorage;
static void *DeleteRenderPointRectSinkEntry004396E0(void *pObject, BYTE fDelete);
static void DeleteRenderPointRectSinkThunk00401898(void *pObject, BYTE fDelete);
static void QueueRenderPointRectSinkEntry004398A0(void *pObject, void *pQueue);
static void QueueRenderPointRectSinkThunk00402DD3(void *pObject, void *pQueue);
static void DispatchRenderPointRectSinkEntry004398B0(void *pObject, void *pContext);
static void DispatchRenderPointRectSinkThunk00401C71(void *pObject, void *pContext);
static void *DeleteRenderPointRectContext00439A40(void *pObject, BYTE fDelete);
static void *DeleteRenderPointRectContextThunk00401488(void *pObject, BYTE fDelete);
static void ResetRenderPointRectEntry00439960(void *pObject, void *pContext);
static void ResetRenderPointRectEntryThunk00401703(void *pObject, void *pContext);
static void ResetRenderPointRectEntry00439970(void *pObject, void *pContext);
static void ResetRenderPointRectEntryThunk0040355D(void *pObject, void *pContext);
static void ClearRenderPointRectEntryFields00439980(void *pObject, int nIndex);
static void ClearRenderPointRectEntryFieldsThunk00403724(void *pObject, int nIndex);
static void *g_LEVEL_RenderPointRectSinkEntryVtableStorage[8] = {
    (void *)DeleteRenderPointRectSinkThunk00401898,
    (void *)QueueRenderPointRectSinkThunk00402DD3,
    (void *)DispatchRenderPointRectSinkThunk00401C71,
    0,
    (void *)DeleteRenderPointRectContextThunk00401488,
    (void *)ResetRenderPointRectEntryThunk00401703,
    (void *)ResetRenderPointRectEntryThunk0040355D,
    (void *)ClearRenderPointRectEntryFieldsThunk00403724,
};
static void *g_LEVEL_RenderPointRectSinkEntryVtable =
    g_LEVEL_RenderPointRectSinkEntryVtableStorage;
static void *g_LEVEL_StatusIndicatorPointSinkEntryVtableStorage[8] = {
    (void *)DestroyStatusIndicatorPointSinkArray00467BB0,
    (void *)QueueStatusIndicatorPointSinkThunk004018AC,
    (void *)DispatchStatusIndicatorPointSinkThunk004025AE,
    0,
    (void *)CrtFatalRuntimeError0x19,
    (void *)UpdateStatusIndicatorLoadProgressThunk00401DB1,
    (void *)VariantResourceBundleNoopThunk,
    (void *)GetStatusIndicatorPointSinkFieldThunk004011B8,
};
static void *g_LEVEL_StatusIndicatorPointSinkEntryVtable =
    g_LEVEL_StatusIndicatorPointSinkEntryVtableStorage;
static void *g_LEVEL_PackagedRectQueueEntryDeleteVtable[1] = {
    (void *)DestroyPackagedRectQueueEntryArray,
};
static void *g_LEVEL_ActionButtonPointSinkDeleteVtable[1] = {
    (void *)DestroyBaseModeActionButtonPointSinkArray,
};
static void QueueVariantRenderEntryToContext00467AE0(void *pObject, void *pQueue);
static void DispatchVariantRenderEntryToContext00467B10(void *pObject, void *pContext);
/* 00499620. Slots 6 and 7 targets remain outside recovered source ownership;
 * use callable placeholders until their owner hierarchy is reconstructed. */
static void *g_LEVEL_VariantRenderManagerVtableStorage[8] = {
    (void *)VariantResourceBundleNoopThunk,
    0,
    (void *)DestroyPackagedRectQueueEntryArray,
    (void *)QueueVariantRenderEntryToContext00467AE0,
    (void *)DispatchVariantRenderEntryToContext00467B10,
    0,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
};
static void *g_LEVEL_VariantRenderManagerVtable =
    g_LEVEL_VariantRenderManagerVtableStorage;
/* 00497C90: manager base table installed after construction. */
static void *g_LEVEL_StatusIndicatorManagerVtable[4] = {
    (void *)NoopVtableCallbackThunk,
    (void *)CrtFatalRuntimeError0x19,
    (void *)CrtFatalRuntimeError0x19,
    0,
};
static void *g_LEVEL_StatusIndicatorManagerSlot70Vtable[8] = {
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)CrtFatalRuntimeError0x19,
    (void *)CrtFatalRuntimeError0x19,
    (void *)NoopVtableCallbackThunk,
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
};
static void *g_LEVEL_StatusIndicatorManagerSlot74Vtable[8] = {
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)CrtFatalRuntimeError0x19,
    (void *)CrtFatalRuntimeError0x19,
    (void *)NoopVtableCallbackThunk,
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)NoopVtableCallbackThunk,
};
static void *g_LEVEL_TimedVariantFrameIndexSelectorCtorVtable[8] = {
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)CrtFatalRuntimeError0x19,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)0,
};
static void *g_LEVEL_TimedVariantFrameIndexSelectorVtable[6] = {
    (void *)NoopVtableCallbackThunk,
    (void *)CrtFatalRuntimeError0x19,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)0,
};
static void *g_LEVEL_TimedVariantFrameIndexSelectorCallbackVtable[6] = {
    (void *)NoopVtableCallbackThunk,
    (void *)SetTimedVariantFrameDuration,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)0,
};
static int *g_LEVEL_StatusIndicatorModeResourceIds;
static unsigned int *g_LEVEL_CompactStatusIndicatorPointTable;
static unsigned int *g_LEVEL_StandardStatusIndicatorPointTable;
int g_fLevelScreenStatusIndicatorConfigured = 0;

// FUNCTION: LEMBALL 0x00432A90
static void *DeleteStatusIndicatorPointSinkEntry00432A90(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00401834
static void *DeleteStatusIndicatorPointSinkThunk00401834(void *pObject, BYTE fDelete) {
    return DeleteStatusIndicatorPointSinkEntry00432A90(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x00432B10
static void QueueHelperLocalRectUpdateEntry00432B10(void *pObject, void *pQueue) {
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x00401E6F
static void QueueHelperLocalRectUpdateThunk00401E6F(void *pObject, void *pQueue) {
    QueueHelperLocalRectUpdateEntry00432B10(pObject, pQueue);
}

// FUNCTION: LEMBALL 0x00432B20
static void DispatchHelperLocalRectUpdateEntry00432B20(void *pObject, void *pContext) {
    int pOwner;
    int pOwnerOffsets;
    int pDispatchBase;
    void (*pDispatch)(void *);

    pOwner = *(int *)((char *)pContext + 0x0c);
    pOwnerOffsets = *(int *)(pOwner + 0x40);
    pDispatchBase = *(int *)(pOwnerOffsets + 4);
    pDispatch = *(void (**)(void *))(pDispatchBase + 0x40 + pOwner);
    pDispatch = *(void (**)(void *))((char *)pDispatch + 4);
    pDispatch(pObject);
}

// FUNCTION: LEMBALL 0x004037A1
static void DispatchHelperLocalRectUpdateThunk004037A1(void *pObject, void *pContext) {
    DispatchHelperLocalRectUpdateEntry00432B20(pObject, pContext);
}

// FUNCTION: LEMBALL 0x004471A0
static void *DeleteFramedScreenRenderChildEntryB(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00403166
static void *DeleteFramedScreenRenderChildEntryThunk(void *pObject, BYTE fDelete) {
    return DeleteFramedScreenRenderChildEntryB(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x00447300
static void QueueFramedScreenRenderEntry(void *pObject, void *pQueue) {
    int *pOwner;

    pOwner = *(int **)((char *)pObject + 0x10);
    if (pOwner[4] == 0) {
        ((void (*)(void))(*(void ***)pOwner)[7])();
    } else {
        pOwner[9] = 0;
    }
    ++pOwner[2];
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x004029CD
static void QueueFramedScreenRenderEntryThunk(void *pObject, void *pQueue) {
    QueueFramedScreenRenderEntry(pObject, pQueue);
}

// FUNCTION: LEMBALL 0x00447340
static void FinalizeFramedScreenRenderEntry(void *pObject, void *pManager) {
    int *pOwner;
    int pOwnerOffsets;
    int pDispatchBase;
    void (*pDispatch)(void *, void *);

    pOwner = *(int **)((char *)pObject + 0x10);
    pOwnerOffsets = *(int *)(*(int *)((char *)pManager + 0x0c) + 0x40);
    pDispatchBase = *(int *)(pOwnerOffsets + 4);
    pDispatch = *(void (**)(void *, void *))(pDispatchBase + 0x40 +
                                             *(int *)((char *)pManager + 0x0c) + 0x20);
    pDispatch(pObject, pOwner);
    --pOwner[2];
}

// FUNCTION: LEMBALL 0x0040213A
static void FinalizeFramedScreenRenderEntryThunk(void *pObject, void *pManager) {
    FinalizeFramedScreenRenderEntry(pObject, pManager);
}

// FUNCTION: LEMBALL 0x004472E0
static void *DeleteBaseModeObjectAdjusted(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock((char *)pObject - 0x0c);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00402C20
static void *DeleteBaseModeObjectAdjustedThunk(void *pObject, BYTE fDelete) {
    return DeleteBaseModeObjectAdjusted(pObject, fDelete);
}

/* Assumption: callback body is not yet recovered; keep callable return-zero behavior
 * while preserving raw slot/thunk ownership. */
// FUNCTION: LEMBALL 0x004468D0
static int HandleBaseModeActionButtonEvent(void *pObject, void *pEvent) {
    (void)pObject;
    (void)pEvent;
    return 0;
}

// FUNCTION: LEMBALL 0x0040134D
static int HandleBaseModeActionButtonEventThunk(void *pObject, void *pEvent) {
    return HandleBaseModeActionButtonEvent(pObject, pEvent);
}

// FUNCTION: LEMBALL 0x00447170
static void *DeleteFramedScreenRenderChildEntryA(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x004033FA
static void *DeleteFramedScreenRenderChildEntryAThunk(void *pObject, BYTE fDelete) {
    return DeleteFramedScreenRenderChildEntryA(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x00447380
static void QueueBitmapRenderEntry(void *pObject, void *pQueue) {
    int *pOwner;

    pOwner = *(int **)((char *)pObject + 0x10);
    if (pOwner[4] == 0) {
        ((void (*)(void))(*(void ***)pOwner)[7])();
    } else {
        pOwner[9] = 0;
    }
    ++pOwner[2];
    *(int *)((char *)pOwner + 0x50) = 1;
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x0040362F
static void QueueBitmapRenderEntryThunk(void *pObject, void *pQueue) {
    QueueBitmapRenderEntry(pObject, pQueue);
}

// FUNCTION: LEMBALL 0x004473D0
static void RenderBitmapEntryViaContext(void *pObject, void *pManager) {
    int *pOwner;
    int pOwnerOffsets;
    int pDispatchBase;
    void (*pDispatch)(void *, void *);

    pOwner = *(int **)((char *)pObject + 0x10);
    pOwnerOffsets = *(int *)(*(int *)((char *)pManager + 0x0c) + 0x40);
    pDispatchBase = *(int *)(pOwnerOffsets + 4);
    pDispatch = *(void (**)(void *, void *))(pDispatchBase + 0x40 +
                                             *(int *)((char *)pManager + 0x0c) + 0x1c);
    pDispatch(pObject, pOwner);
    *(int *)((char *)pOwner + 0x50) = 0;
    --pOwner[2];
}

// FUNCTION: LEMBALL 0x004031AC
static void RenderBitmapEntryViaContextThunk(void *pObject, void *pManager) {
    RenderBitmapEntryViaContext(pObject, pManager);
}

// FUNCTION: LEMBALL 0x00432350
static void *DeleteQueuedRenderPointSinkWrapper00432350(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00403580
static void *DeleteQueuedRenderPointSinkThunk00403580(void *pObject, BYTE fDelete) {
    return DeleteQueuedRenderPointSinkWrapper00432350(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x00467BB0
static void *DestroyStatusIndicatorPointSinkArray00467BB0(void *pObject, unsigned int fDelete) {
    int cEntries;
    char *pEntry;

    if ((fDelete & 2) == 0) {
        *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
        if ((fDelete & 1) != 0) {
            FreeVSMemBlock(pObject);
        }
        return pObject;
    }

    cEntries = *((int *)pObject - 1);
    pEntry = (char *)pObject + cEntries * 0x10;
    while (cEntries-- > 0) {
        pEntry -= 0x10;
        RestoreStatusIndicatorPointSinkEntryVtable(pEntry);
    }
    FreeVSMemBlock((char *)pObject - 4);
    return pObject;
}

// FUNCTION: LEMBALL 0x00469930
static void *DestroyPackagedSpriteRenderEntryArray00469930(void *pObject, unsigned int fDelete) {
    int cEntries;
    char *pEntry;

    if ((fDelete & 2) == 0) {
        *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
        if ((fDelete & 1) != 0) {
            FreeVSMemBlock(pObject);
        }
        return pObject;
    }

    cEntries = *((int *)pObject - 1);
    pEntry = (char *)pObject + cEntries * 0x10;
    while (cEntries-- > 0) {
        pEntry -= 0x10;
        RestorePackagedSpriteRenderEntryVtableA(pEntry);
    }
    FreeVSMemBlock((char *)pObject - 4);
    return pObject;
}

// FUNCTION: LEMBALL 0x00439800
static void QueuePackagedSpriteRenderEntry00439800(void *pObject, void *pQueue) {
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x00403846
static void QueuePackagedSpriteRenderEntryThunk00403846(void *pObject, void *pQueue) {
    QueuePackagedSpriteRenderEntry00439800(pObject, pQueue);
}

// FUNCTION: LEMBALL 0x00439810
static void DispatchPackagedSpriteRenderEntry00439810(void *pObject, void *pContext) {
    int pOwner;
    int pOwnerOffsets;
    int pDispatchBase;
    void (*pDispatch)(void *);

    pOwner = *(int *)((char *)pContext + 0x0c);
    pOwnerOffsets = *(int *)(pOwner + 0x40);
    pDispatchBase = *(int *)(pOwnerOffsets + 4);
    pDispatch = *(void (**)(void *))(pDispatchBase + 0x40 + pOwner + 0x18);
    pDispatch(pObject);
}

// FUNCTION: LEMBALL 0x00402A8B
static void DispatchPackagedSpriteRenderEntryThunk00402A8B(void *pObject, void *pContext) {
    DispatchPackagedSpriteRenderEntry00439810(pObject, pContext);
}

// FUNCTION: LEMBALL 0x00439400
static void *DeletePackagedSpriteRenderEntry00439400(void *pObject, BYTE fDelete) {
    *(void **)pObject = &g_LEVEL_PackagedSpriteRenderEntryVtableStorage[4];
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x004016F9
static void *DeletePackagedSpriteRenderEntryThunk004016F9(void *pObject, BYTE fDelete) {
    return DeletePackagedSpriteRenderEntry00439400(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x00439370
static void NoopPackagedSpriteRenderEntry00439370(void) {
}

// FUNCTION: LEMBALL 0x00402A22
static void NoopPackagedSpriteRenderEntryThunk00402A22(void) {
    NoopPackagedSpriteRenderEntry00439370();
}

// FUNCTION: LEMBALL 0x00439380
static void NoopPackagedSpriteRenderEntry00439380(void) {
}

// FUNCTION: LEMBALL 0x00401B77
static void NoopPackagedSpriteRenderEntryThunk00401B77(void) {
    NoopPackagedSpriteRenderEntry00439380();
}

// FUNCTION: LEMBALL 0x004396E0
static void *DeleteRenderPointRectSinkEntry004396E0(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00401898
static void DeleteRenderPointRectSinkThunk00401898(void *pObject, BYTE fDelete) {
    DeleteRenderPointRectSinkEntry004396E0(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x004398A0
static void QueueRenderPointRectSinkEntry004398A0(void *pObject, void *pQueue) {
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x00402DD3
static void QueueRenderPointRectSinkThunk00402DD3(void *pObject, void *pQueue) {
    QueueRenderPointRectSinkEntry004398A0(pObject, pQueue);
}

// FUNCTION: LEMBALL 0x004398B0
static void DispatchRenderPointRectSinkEntry004398B0(void *pObject, void *pContext) {
    int pOwner;
    void (*pDispatch)(void *);

    pOwner = *(int *)((char *)pContext + 0x0c);
    pDispatch = *(void (**)(void *))(*(int *)(pOwner + 0x98) + 4);
    pDispatch(pObject);
}

// FUNCTION: LEMBALL 0x00401C71
static void DispatchRenderPointRectSinkThunk00401C71(void *pObject, void *pContext) {
    DispatchRenderPointRectSinkEntry004398B0(pObject, pContext);
}

// FUNCTION: LEMBALL 0x00439A40
static void *DeleteRenderPointRectContext00439A40(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00401488
static void *DeleteRenderPointRectContextThunk00401488(void *pObject, BYTE fDelete) {
    return DeleteRenderPointRectContext00439A40(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x00439960
static void ResetRenderPointRectEntry00439960(void *pObject, void *pContext) {
    (void)pObject;
    (void)pContext;
}

// FUNCTION: LEMBALL 0x00401703
static void ResetRenderPointRectEntryThunk00401703(void *pObject, void *pContext) {
    ResetRenderPointRectEntry00439960(pObject, pContext);
}

// FUNCTION: LEMBALL 0x00439970
static void ResetRenderPointRectEntry00439970(void *pObject, void *pContext) {
    (void)pObject;
    (void)pContext;
}

// FUNCTION: LEMBALL 0x0040355D
static void ResetRenderPointRectEntryThunk0040355D(void *pObject, void *pContext) {
    ResetRenderPointRectEntry00439970(pObject, pContext);
}

// FUNCTION: LEMBALL 0x00439980
static void ClearRenderPointRectEntryFields00439980(void *pObject, int nIndex) {
    char *pEntry;

    pEntry = (char *)pObject + nIndex * 4;
    *(int *)(pEntry + 0x1c) = 0;
    *(int *)(pEntry + 0x10) = 0;
}

// FUNCTION: LEMBALL 0x00403724
static void ClearRenderPointRectEntryFieldsThunk00403724(void *pObject, int nIndex) {
    ClearRenderPointRectEntryFields00439980(pObject, nIndex);
}

// FUNCTION: LEMBALL 0x00432AD0
static void QueueStatusIndicatorPointSink00432AD0(void *pObject, void *pQueue) {
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x004018AC
static void QueueStatusIndicatorPointSinkThunk004018AC(void *pObject, void *pQueue) {
    QueueStatusIndicatorPointSink00432AD0(pObject, pQueue);
}

// FUNCTION: LEMBALL 0x00432AE0
static void DispatchStatusIndicatorPointSink00432AE0(void *pObject, void *pContext) {
    int pOwner;
    int pOwnerOffsets;
    char *pDispatchObject;
    void (*pDispatch)(void *);

    pOwner = *(int *)((char *)pContext + 0x0c);
    pOwnerOffsets = *(int *)(pOwner + 0x40);
    pDispatchObject = (char *)(*(int *)(pOwnerOffsets + 4) + pOwner + 0x40);
    pDispatch = *(void (**)(void *))((char *)*(void **)pDispatchObject + 0x10);
    pDispatch(pObject);
}

// FUNCTION: LEMBALL 0x004025AE
static void DispatchStatusIndicatorPointSinkThunk004025AE(void *pObject, void *pContext) {
    DispatchStatusIndicatorPointSink00432AE0(pObject, pContext);
}

// FUNCTION: LEMBALL 0x00434D10
static void UpdateStatusIndicatorLoadProgress00434D10(void *pObject) {
    int nLoaded;
    void *pOwner;

    nLoaded = ++*(int *)((char *)pObject + 0x1c);
    pOwner = *(void **)((char *)pObject + 0x08);
    if (pOwner != 0) {
        ((void (*)(int))**(void ***)((char *)pOwner + 0x70))(
            (nLoaded * 100) / *(int *)((char *)pObject + 0x20));
    }
}

// FUNCTION: LEMBALL 0x00401DB1
static void UpdateStatusIndicatorLoadProgressThunk00401DB1(void *pObject) {
    UpdateStatusIndicatorLoadProgress00434D10(pObject);
}

// FUNCTION: LEMBALL 0x00435870
static int GetStatusIndicatorPointSinkField00435870(void *pObject) {
    return *(int *)((char *)pObject + 0x0c);
}

// FUNCTION: LEMBALL 0x004011B8
static int GetStatusIndicatorPointSinkFieldThunk004011B8(void *pObject) {
    return GetStatusIndicatorPointSinkField00435870(pObject);
}

// FUNCTION: LEMBALL 0x00467B40
void *DestroyPackagedRectQueueEntryArray(void *pObject, unsigned int fDelete) {
    int cEntries;
    char *pEntry;

    if ((fDelete & 2) == 0) {
        *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
        if ((fDelete & 1) != 0) {
            FreeVSMemBlock(pObject);
        }
        return pObject;
    }

    cEntries = *((int *)pObject - 1);
    pEntry = (char *)pObject + cEntries * 0x18;
    while (cEntries-- > 0) {
        pEntry -= 0x18;
        *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    }
    FreeVSMemBlock((char *)pObject - 4);
    return pObject;
}

// FUNCTION: LEMBALL 0x00467AE0
static void QueueVariantRenderEntryToContext00467AE0(void *pObject, void *pQueue) {
    int *pOwner;

    pOwner = *(int **)((char *)pObject + 0x08);
    if (pOwner[4] == 0) {
        ((void (*)(void))(*(void ***)pOwner)[7])();
    } else {
        pOwner[9] = 0;
    }
    ++pOwner[2];
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x00467B10
static void DispatchVariantRenderEntryToContext00467B10(void *pObject, void *pContext) {
    int *pOwner;
    int pOwnerOffsets;
    int pDispatchBase;
    void (*pDispatch)(void *, void *);

    pOwner = *(int **)((char *)pObject + 0x08);
    pOwnerOffsets = *(int *)(*(int *)((char *)pContext + 0x0c) + 0x40);
    pDispatchBase = *(int *)(pOwnerOffsets + 4);
    pDispatch = *(void (**)(void *, void *))(pDispatchBase + 0x40 +
                                             *(int *)((char *)pContext + 0x0c) + 0x24);
    pDispatch(pObject, pOwner);
    --pOwner[2];
}

// FUNCTION: LEMBALL 0x004439E0
void ResetBaseModeActionButtonPointSink(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
}

// FUNCTION: LEMBALL 0x004452E0
void *DestroyBaseModeActionButtonPointSinkArray(void *pObject, unsigned int fDelete) {
    int cEntries;
    char *pEntry;

    if ((fDelete & 2) == 0) {
        ResetBaseModeActionButtonPointSink(pObject);
        if ((fDelete & 1) != 0) {
            FreeVSMemBlock(pObject);
        }
        return pObject;
    }

    cEntries = *((int *)pObject - 1);
    pEntry = (char *)pObject + cEntries * 0x20;
    while (cEntries-- > 0) {
        pEntry -= 0x20;
        ResetBaseModeActionButtonPointSink(pEntry);
    }
    FreeVSMemBlock((char *)pObject - 4);
    return pObject;
}

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

    *(void **)((char *)pManager + 0x120) = g_pQueuedRenderPointSinkFinalizeThunk;
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
