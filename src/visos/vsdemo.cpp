#include "vsdemo.h"

#include "../game.h"
#include "vsmem.h"
#include "vsinit.h"
#include "win32.h"

extern "C" DWORD timeGetTime(void);

typedef void *(*VSDEMO_DeleteProc)(void *pObject, unsigned char fDelete);

int DispatchLevelDemoEventsForFrameThunk(void *pPlaybackController, char nFrame);

static void *g_LevelDemoPlaybackControllerVtable[2] = {
    (void *)RestoreLevelDemoPlaybackBaseVtable,
    (void *)DeleteLevelDemoPlaybackController,
};
static void *g_NonZrleVariantRenderEntryInitializeVtable[2] = { 0, 0 };

void *g_pLevelDemoPlaybackController = 0;
void *g_pLevelDemoPlaybackDescriptor = 0;
int g_nStoredLevelDemoModeEnabled = 0;

// FUNCTION: LEMBALL 0x00409150
void CreateFrameTimerController(unsigned int uFrameInterval) {
    void *pPlaybackController;

    pPlaybackController = AllocateVSMemBlock(0x58);
    if (pPlaybackController != 0) {
        g_pLevelDemoPlaybackController = ConstructLevelDemoPlaybackController(pPlaybackController, uFrameInterval);
        return;
    }
    g_pLevelDemoPlaybackController = 0;
}

// FUNCTION: LEMBALL 0x00409180
void DestroyFrameTimerController(void) {
    void *pPlaybackController;

    pPlaybackController = g_pLevelDemoPlaybackController;
    if (g_pLevelDemoPlaybackController != 0) {
        ((VSDEMO_DeleteProc)(*(void ***)pPlaybackController)[1])(pPlaybackController, 1);
        g_pLevelDemoPlaybackController = 0;
    }
}

// FUNCTION: LEMBALL 0x004091B0
void *ConstructLevelDemoPlaybackController(void *pPlaybackController, unsigned int uFrameInterval) {
    InitializeRenderQueueNodeBase(pPlaybackController);
    *(short *)((char *)pPlaybackController + 0x42) = 0;
    *(void ***)pPlaybackController = g_LevelDemoPlaybackControllerVtable;
    *(short *)((char *)pPlaybackController + 0x40) = 0;
    *(unsigned int *)((char *)pPlaybackController + 0x14) = uFrameInterval;
    *(void **)((char *)pPlaybackController + 0x10) = 0;
    *(void **)((char *)pPlaybackController + 0x34) = 0;
    *(int *)((char *)pPlaybackController + 0x28) = 0;
    *(int *)((char *)pPlaybackController + 0x2c) = 0;
    *(int *)((char *)pPlaybackController + 0x30) = 0;
    *(void **)((char *)pPlaybackController + 0x24) = 0;
    *(void **)((char *)pPlaybackController + 0x20) = 0;
    *(int *)((char *)pPlaybackController + 0x4c) = 0;
    *(int *)((char *)pPlaybackController + 0x48) = 0;
    *(int *)((char *)pPlaybackController + 0x50) = 0;
    *(int *)((char *)pPlaybackController + 0x18) = -1;
    *(int *)((char *)pPlaybackController + 0x54) = 0;
    RegisterOrderedRenderDispatchClient(g_pSharedRenderDispatchQueue, pPlaybackController, -100);
    ResetLevelDemoPlaybackCursor(pPlaybackController);
    return pPlaybackController;
}

// FUNCTION: LEMBALL 0x00409220
void DestroyLevelDemoPlaybackController(void *pPlaybackController) {
    *(void ***)pPlaybackController = g_LevelDemoPlaybackControllerVtable;
    ReleaseLevelDemoRecordBuffer(pPlaybackController);
    UnregisterOrderedRenderDispatchClient(g_pSharedRenderDispatchQueue, pPlaybackController, -100);
    *(void ***)pPlaybackController = g_NonZrleVariantRenderEntryInitializeVtable;
}

// FUNCTION: LEMBALL 0x004095E0
void ResetLevelDemoPlaybackCursor(void *pPlaybackController) {
    *(int *)((char *)pPlaybackController + 0x18) = -1;
    *(int *)((char *)pPlaybackController + 0x44) = 0;
    *(int *)((char *)pPlaybackController + 0x50) = 0;
    *(int *)((char *)pPlaybackController + 0x3c) = 0;
}

// FUNCTION: LEMBALL 0x00409600
void SetLevelDemoPlaybackEnabled(void *pPlaybackController, int fEnabled) {
    *(int *)((char *)pPlaybackController + 0x4c) = fEnabled;
    if (fEnabled != 0) {
        ResetLevelDemoPlaybackCursor(pPlaybackController);
        return;
    }
    ReleaseLevelDemoRecordBuffer(pPlaybackController);
}

// FUNCTION: LEMBALL 0x00409620
void ServiceLevelDemoPlayback(void *pPlaybackController) {
    DWORD dwNow;
    int fDispatched;

    if (*(int *)((char *)pPlaybackController + 0x50) == 0 &&
        *(int *)((char *)pPlaybackController + 0x4c) != 0) {
        dwNow = timeGetTime();
        if (*(unsigned int *)((char *)pPlaybackController + 0x3c) != 0 &&
            *(unsigned int *)((char *)pPlaybackController + 0x3c) <=
                dwNow - *(unsigned int *)((char *)pPlaybackController + 0x38)) {
            StopLevelDemoPlayback(pPlaybackController);
            return;
        }
        ++*(int *)((char *)pPlaybackController + 0x44);
        do {
            fDispatched = DispatchLevelDemoEventsForFrameThunk(
                pPlaybackController, (char)*(int *)((char *)pPlaybackController + 0x44));
        } while (fDispatched != 0);
    }
}

void ServiceLevelDemoPlaybackThunk(void *pPlaybackController) {
    ServiceLevelDemoPlayback(pPlaybackController);
}

// FUNCTION: LEMBALL 0x00409660
void ReleaseLevelDemoRecordBuffer(void *pPlaybackController) {
    int *pResourceObject;
    int *pReferenceCount;

    pResourceObject = *(int **)((char *)pPlaybackController + 0x24);
    if (pResourceObject != 0) {
        pReferenceCount = pResourceObject + 2;
        --*pReferenceCount;
        ReleaseTypedResourceObjectReference(pResourceObject);
        *(void **)((char *)pPlaybackController + 0x24) = 0;
        *(void **)((char *)pPlaybackController + 0x10) = 0;
        *(void **)((char *)pPlaybackController + 0x1c) = 0;
        return;
    }
    if (*(void **)((char *)pPlaybackController + 0x10) != 0) {
        FreeVSMemBlock(*(void **)((char *)pPlaybackController + 0x10));
    }
    *(void **)((char *)pPlaybackController + 0x10) = 0;
    *(void **)((char *)pPlaybackController + 0x1c) = 0;
}

// FUNCTION: LEMBALL 0x004096A0
void StopLevelDemoPlayback(void *pPlaybackController) {
    *(int *)((char *)pPlaybackController + 0x50) = 1;
    ReleaseLevelDemoRecordBuffer(pPlaybackController);
}

// FUNCTION: LEMBALL 0x004096B0
int FilterLevelDemoPlaybackInputEvent(void *pPlaybackController, unsigned short *pEventId) {
    unsigned short wEventId;

    if (*(int *)((char *)pPlaybackController + 0x4c) != 0) {
        wEventId = *pEventId;
        if ((wEventId & 0x8000) == 0) {
            switch (wEventId) {
            case 1:
            case 3:
            case 5:
                StopLevelDemoPlayback(pPlaybackController);
                return 1;
            default:
                return 1;
            case 7:
            case 0xf:
                break;
            }
        } else {
            *pEventId = wEventId & 0x7fff;
        }
    }
    return 0;
}

// FUNCTION: LEMBALL 0x004098B0
void *DeleteLevelDemoPlaybackController(void *pPlaybackController, unsigned char fDelete) {
    DestroyLevelDemoPlaybackController(pPlaybackController);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pPlaybackController);
    }
    return pPlaybackController;
}

// FUNCTION: LEMBALL 0x004098E0
void *RestoreLevelDemoPlaybackBaseVtable(void *pPlaybackController, unsigned char fDelete) {
    *(void ***)pPlaybackController = g_NonZrleVariantRenderEntryInitializeVtable;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pPlaybackController);
    }
    return pPlaybackController;
}

int DispatchLevelDemoEventsForFrameThunk(void *pPlaybackController, char nFrame) {
    (void)pPlaybackController;
    (void)nFrame;
    return 0;
}
