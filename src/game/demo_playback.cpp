#include "game/demo_playback.h"

#include "../game/game_app.h"
#include "../resource/resource_archive.h"
#include "../engine/memory_arena.h"
#include "../engine/runtime_init.h"
#include "../platform/win32.h"

#include <stdio.h>

extern "C" DWORD WINAPI timeGetTime(void);

typedef void *(*DEMO_DeleteProc)(void *pObject, unsigned char fDelete);
extern void ReturnVoidVtableCallback(void);

int DispatchLevelDemoEventsForFrameThunk(void *pPlaybackController, char nFrame);

static const char g_DEMO_OpenReadMode[] = "rb";
static const unsigned int g_DEMO_BinResourceTypeTag = 0x42494e20;

extern void CopyBufferIntoTypedResourceObjectAndParse(void *pObject, unsigned int *pSource,
                                                       unsigned int nUnused, unsigned int cbBuffer);
extern void ParseIntResourceDescriptor(void);
extern int GetField14NeedsAgeIncrement(void *pObject);
extern int GetField1CVfunc04(void *pObject);
extern int GetField14Vfunc05(void *pObject);
extern int ReturnZeroVfunc06(void);
extern void EnsureTypedResourceObjectLoaded(void *pObject);
extern void UnloadTypedResourceObject(void *pObject, int fReleaseMode);
extern void NoopVfunc09(void);
extern void *GetEffResourceDataPointer(void *pObject);
extern void NoopOnLoadVfunc11(void);
extern void NoopVfunc12(void);
extern int GetField28GetMemorySize(void *pObject);
extern void InitializeTypedResourceObjectBaseVtable(void *pObject);

// FUNCTION: LEMBALL 0x0045EC60
static void SetBinResourceTypeTag(void *pObject) {
    *(unsigned int *)((char *)pObject + 0x40) = g_DEMO_BinResourceTypeTag;
}

// FUNCTION: LEMBALL 0x0045EC70
static void *DestroyBinResource(void *pObject, int fDelete) {
    InitializeTypedResourceObjectBaseVtable(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

static void *g_DEMO_BinResourceVtableStorage[15] = {
    (void *)DestroyBinResource,
    (void *)CopyBufferIntoTypedResourceObjectAndParse,
    (void *)ParseIntResourceDescriptor,
    (void *)GetField14NeedsAgeIncrement,
    (void *)GetField1CVfunc04,
    (void *)GetField14Vfunc05,
    (void *)ReturnZeroVfunc06,
    (void *)EnsureTypedResourceObjectLoaded,
    (void *)UnloadTypedResourceObject,
    (void *)NoopVfunc09,
    (void *)GetEffResourceDataPointer,
    (void *)NoopOnLoadVfunc11,
    (void *)NoopVfunc12,
    (void *)SetBinResourceTypeTag,
    (void *)GetField28GetMemorySize,
};
static void *g_DEMO_BinResourceVtable = g_DEMO_BinResourceVtableStorage;

extern void *g_pMainResourceArchive;
extern void *g_pCachedResourceObjectBaseDeleteVtable;
extern void InitializeResourceObjectFromId(void *pObject, int nResourceId);
extern void *FinalizeLoadedResourceObjectResult(void *pObject);

static void *g_LevelDemoPlaybackControllerVtable[2] = {
    (void *)RestoreLevelDemoPlaybackBaseVtable,
    (void *)DeleteLevelDemoPlaybackController,
};
void *g_pNonZrleVariantRenderEntryInitializeVtable[2] = {
    (void *)ReturnVoidVtableCallback, (void *)ReturnVoidVtableCallback
};

void *g_pLevelDemoPlaybackController = 0;
void *g_pLevelDemoPlaybackDescriptor = 0;
int g_nStoredLevelDemoModeEnabled = 0;

// FUNCTION: LEMBALL 0x00409150
void CreateFrameTimerController(unsigned int uFrameInterval) {
    DEMO_LevelDemoPlaybackController *pPlaybackController;

    pPlaybackController = (DEMO_LevelDemoPlaybackController *)AllocateVSMemBlock(0x58);
    if (pPlaybackController != 0) {
        g_pLevelDemoPlaybackController =
            pPlaybackController->ConstructLevelDemoPlaybackControllerThunk(uFrameInterval);
        return;
    }
    g_pLevelDemoPlaybackController = 0;
}

// FUNCTION: LEMBALL 0x00409180
void DestroyFrameTimerController(void) {
    void *pPlaybackController;

    pPlaybackController = g_pLevelDemoPlaybackController;
    if (g_pLevelDemoPlaybackController != 0) {
        ((DEMO_DeleteProc)(*(void ***)pPlaybackController)[1])(pPlaybackController, 1);
        g_pLevelDemoPlaybackController = 0;
    }
}

union DEMO_LevelDemoPlaybackConstructorAddress {
    DEMO_LevelDemoPlaybackController *(DEMO_LevelDemoPlaybackController::*m_pMethod)(unsigned int);
    void *m_pAddress;
};

static DEMO_LevelDemoPlaybackConstructorAddress g_DEMO_LevelDemoPlaybackConstructorAddress = {
    &DEMO_LevelDemoPlaybackController::ConstructLevelDemoPlaybackController
};

// FUNCTION: LEMBALL 0x004017A3
__declspec(naked) DEMO_LevelDemoPlaybackController *
DEMO_LevelDemoPlaybackController::ConstructLevelDemoPlaybackControllerThunk(unsigned int uFrameInterval) {
    __asm {
        jmp dword ptr [g_DEMO_LevelDemoPlaybackConstructorAddress.m_pAddress]
    }
}

// FUNCTION: LEMBALL 0x004091B0
DEMO_LevelDemoPlaybackController *DEMO_LevelDemoPlaybackController::ConstructLevelDemoPlaybackController(unsigned int uFrameInterval) {
    InitializeRenderQueueNodeBase(this);
    *(short *)((char *)this + 0x42) = 0;
    *(void ***)this = g_LevelDemoPlaybackControllerVtable;
    *(short *)((char *)this + 0x40) = 0;
    *(unsigned int *)((char *)this + 0x14) = uFrameInterval;
    *(void **)((char *)this + 0x10) = 0;
    *(void **)((char *)this + 0x34) = 0;
    *(int *)((char *)this + 0x28) = 0;
    *(int *)((char *)this + 0x2c) = 0;
    *(int *)((char *)this + 0x30) = 0;
    *(void **)((char *)this + 0x24) = 0;
    *(void **)((char *)this + 0x20) = 0;
    *(int *)((char *)this + 0x4c) = 0;
    *(int *)((char *)this + 0x48) = 0;
    *(int *)((char *)this + 0x50) = 0;
    *(int *)((char *)this + 0x18) = -1;
    *(int *)((char *)this + 0x54) = 0;
    ((GAME_RenderDispatchQueue *)g_pSharedRenderDispatchQueue)
        ->RegisterOrderedRenderDispatchClient(this, -100);
    ResetLevelDemoPlaybackCursorThunk();
    return this;
}

// FUNCTION: LEMBALL 0x00409220
void DestroyLevelDemoPlaybackController(void *pPlaybackController) {
    *(void ***)pPlaybackController = g_LevelDemoPlaybackControllerVtable;
    ReleaseLevelDemoRecordBuffer(pPlaybackController);
    ((GAME_RenderDispatchQueue *)g_pSharedRenderDispatchQueue)
        ->UnregisterOrderedRenderDispatchClient(pPlaybackController, -100);
    *(void ***)pPlaybackController = g_pNonZrleVariantRenderEntryInitializeVtable;
}

// FUNCTION: LEMBALL 0x0040179E
void DEMO_LevelDemoPlaybackController::ResetLevelDemoPlaybackCursorThunk(void) {
    ResetLevelDemoPlaybackCursor();
}

// FUNCTION: LEMBALL 0x004095E0
void DEMO_LevelDemoPlaybackController::ResetLevelDemoPlaybackCursor(void) {
    *(int *)((char *)this + 0x18) = -1;
    *(int *)((char *)this + 0x44) = 0;
    *(int *)((char *)this + 0x50) = 0;
    *(int *)((char *)this + 0x3c) = 0;
}

// FUNCTION: LEMBALL 0x00409600
void SetLevelDemoPlaybackEnabled(void *pPlaybackController, int fEnabled) {
    *(int *)((char *)pPlaybackController + 0x4c) = fEnabled;
    if (fEnabled != 0) {
        ((DEMO_LevelDemoPlaybackController *)pPlaybackController)->ResetLevelDemoPlaybackCursorThunk();
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

void LEMBALL_FASTCALL ServiceLevelDemoPlaybackThunk(void *pPlaybackController) {
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
    *(void ***)pPlaybackController = g_pNonZrleVariantRenderEntryInitializeVtable;
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

// FUNCTION: LEMBALL 0x00462EE0
FILE *OpenFileWithMode(const char *pszPath, const char *pszMode) {
    return fopen(pszPath, pszMode);
}

// FUNCTION: LEMBALL 0x00462F20
void CloseCrtFilePointer(FILE *pFile) {
    fclose(pFile);
}

// FUNCTION: LEMBALL 0x00462F30
unsigned int ReadFileBytes(FILE *pFile, void *pBuffer, size_t cbBuffer) {
    return (unsigned int)fread(pBuffer, 1, cbBuffer, pFile);
}

// FUNCTION: LEMBALL 0x00462FB0
unsigned int TellFile(FILE *pFile) {
    return (unsigned int)ftell(pFile);
}

// FUNCTION: LEMBALL 0x00462F80
unsigned int SeekFile(FILE *pFile, long lOffset, int nOrigin) {
    fseek(pFile, lOffset, nOrigin);
    return (unsigned int)ftell(pFile);
}

// FUNCTION: LEMBALL 0x00462FC0
unsigned int GetFileLengthPreservingPosition(FILE *pFile) {
    unsigned int nSavedOffset;
    unsigned int cbFile;

    nSavedOffset = TellFile(pFile);
    cbFile = SeekFile(pFile, 0, SEEK_END);
    SeekFile(pFile, (long)nSavedOffset, SEEK_SET);
    return cbFile;
}

// FUNCTION: LEMBALL 0x0045E540
int *LoadBinResource(int nResourceId) {
    int *pResourceObject;
    int *pObjectWords;

    pResourceObject = (int *)FindCachedResourceObjectById(g_pMainResourceArchive, nResourceId);
    if (pResourceObject != 0) {
        if ((unsigned int)pResourceObject[0x10] != g_DEMO_BinResourceTypeTag) {
            ReleaseTypedResourceObjectReference(pResourceObject);
            pResourceObject = 0;
        }
        return pResourceObject;
    }

    pObjectWords = (int *)AllocateVSMemBlock(0x48);
    if (pObjectWords != 0) {
        *(void **)pObjectWords = g_pCachedResourceObjectBaseDeleteVtable;
        pObjectWords[6] = 0;
        *(void **)pObjectWords = g_DEMO_BinResourceVtable;
        InitializeResourceObjectFromId(pObjectWords, nResourceId);
        return (int *)FinalizeLoadedResourceObjectResult(pObjectWords);
    }
    return (int *)FinalizeLoadedResourceObjectResult(0);
}

// FUNCTION: LEMBALL 0x00409460
int LoadNextLevelDemoRecordBuffer(void *pPlaybackController) {
    unsigned char bValue;
    unsigned char *pbCursor;
    unsigned short wValue;
    unsigned int cbLength;
    FILE *pFile;
    unsigned int cbFile;
    void *pBuffer;
    unsigned int cbRead;
    int *pResourceObject;
    unsigned int *pcbRemaining;
    int nNextResourceId;

    if (*(char **)((char *)pPlaybackController + 0x20) == 0) {
        pResourceObject = LoadBinResource(*(int *)((char *)pPlaybackController + 0x28));
        *(int **)((char *)pPlaybackController + 0x24) = pResourceObject;
        if (pResourceObject[4] == 0) {
            ((void (*)())*(void **)(*pResourceObject + 0x1c))();
        } else {
            pResourceObject[9] = 0;
        }
        ++pResourceObject[2];
        *(void **)((char *)pPlaybackController + 0x10) =
            ((void *(*)(void))*(void **)(**(int **)((char *)pPlaybackController + 0x24) + 0x28))();
        nNextResourceId = *(int *)((char *)pPlaybackController + 0x28) + 1;
        *(int *)((char *)pPlaybackController + 0x18) = *(int *)(*(int *)((char *)pPlaybackController + 0x24) + 0x28);
        *(int *)((char *)pPlaybackController + 0x28) = nNextResourceId;
        if (*(int *)((char *)pPlaybackController + 0x30) + *(int *)((char *)pPlaybackController + 0x2c) <=
            nNextResourceId) {
            *(int *)((char *)pPlaybackController + 0x28) = *(int *)((char *)pPlaybackController + 0x2c);
        }
    } else {
        pFile = OpenFileWithMode(*(char **)((char *)pPlaybackController + 0x20), g_DEMO_OpenReadMode);
        if (pFile == 0) {
            return 0;
        }
        cbFile = GetFileLengthPreservingPosition(pFile);
        pBuffer = AllocateVSMemBlock(cbFile);
        *(void **)((char *)pPlaybackController + 0x10) = pBuffer;
        cbRead = ReadFileBytes(pFile, pBuffer, cbFile);
        CloseCrtFilePointer(pFile);
        *(unsigned int *)((char *)pPlaybackController + 0x18) = cbRead;
    }

    pcbRemaining = (unsigned int *)((char *)pPlaybackController + 0x18);
    pbCursor = *(unsigned char **)((char *)pPlaybackController + 0x10);
    *(unsigned char **)((char *)pPlaybackController + 0x1c) = pbCursor;
    bValue = *pbCursor;
    *pcbRemaining = (unsigned int)bValue;
    wValue = (unsigned short)(pbCursor[1] << 8) | bValue;
    *pcbRemaining = (unsigned int)wValue;
    cbLength = ((unsigned int)pbCursor[2] << 0x10) | wValue;
    *pcbRemaining = cbLength;
    bValue = pbCursor[3];
    *(unsigned char **)((char *)pPlaybackController + 0x1c) = pbCursor + 4;
    *pcbRemaining = ((unsigned int)bValue << 0x18) | cbLength;
    return 1;
}

// FUNCTION: LEMBALL 0x00409560
void ReadLevelDemoLengthPrefixedRecordThunk(void *pPlaybackController, void *pRecordBuffer, unsigned int *pcbRecord) {
    unsigned char bValue;
    unsigned short wValue;
    unsigned int cbCopy;
    unsigned int cbRemaining;
    unsigned int *pdwCursor;

    if (*(int *)((char *)pPlaybackController + 0x18) == -1) {
        LoadNextLevelDemoRecordBuffer(pPlaybackController);
    }

    bValue = **(unsigned char **)((char *)pPlaybackController + 0x1c);
    *pcbRecord = (unsigned int)bValue;
    pdwCursor = (unsigned int *)*(int *)((char *)pPlaybackController + 0x1c);
    wValue = (unsigned short)(*((unsigned char *)pdwCursor + 1) << 8) | bValue;
    *pcbRecord = (unsigned int)wValue;
    *pcbRecord = ((unsigned int)*((unsigned char *)pdwCursor + 2) << 0x10) | wValue;
    *pcbRecord = ((unsigned int)*((unsigned char *)pdwCursor + 3) << 0x18) | *pcbRecord;

    pdwCursor = (unsigned int *)(*(int *)((char *)pPlaybackController + 0x1c) + 4);
    *(unsigned int **)((char *)pPlaybackController + 0x1c) = pdwCursor;
    *(int *)((char *)pPlaybackController + 0x18) -= (*pcbRecord + 4);

    cbRemaining = *pcbRecord;
    CopyMemoryBytes(pRecordBuffer, pdwCursor, cbRemaining);
    *(int *)((char *)pPlaybackController + 0x1c) += *pcbRecord;
    cbCopy = cbRemaining;
    (void)cbCopy;
}
#include "../network/safe_vtable.h"
