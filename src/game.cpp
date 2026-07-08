#include "game.h"

#include "shellui.h"
#include "startup.h"
#include "visos/vsaudio.h"
#include "visos/vsdemo.h"
#include "visos/vsgdi.h"
#include "visos/vsmath.h"
#include "vsmem.h"
#include "vsinit.h"

#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif
#include <stdio.h>
#include <string.h>

static void *g_pSignedTrigTable = 0;
static void *g_pSessionRandomState = 0;
static char g_GAME_SrcDiskBuffer[0x100];

static const char g_GAME_RegistryRoot[] = "SOFTWARE\\Visual Sciences\\";
static const char g_GAME_RegistryKey[] = "SOFTWARE\\Visual Sciences\\Lemmings Paintball";
static const char g_GAME_RunningValueName[] = "Running";
static const char g_GAME_RunningState[] = "running";
static const char g_GAME_NotRunningState[] = "";
static const char g_GAME_WindowTitle[] = "Lemmings Paintball";
static const char g_GAME_MainArchiveName[] = "pbaimog.vsr";
static const char g_GAME_StartupMusicName[] = "lemball";
static const char g_GAME_ProcessingName[] = "Processing";
static const char g_GAME_RefreshingName[] = "Refreshing";
static const char g_GAME_SrcDiskValueName[] = "SrcDisk";
static const char g_GAME_VsmemDllName[] = "vsmem.dll";
static const char g_GAME_NotInstalledTitle[] = "Paintball Not Installed";
static const char g_GAME_InstallPrompt[] =
    "To play Lemmings Paintball, you must first install it.  Run the SETUP.EXE program on the CD";
static const char g_GAME_CdromErrorTitle[] = "Unable to find CD";
static const char g_GAME_CdromErrorText[] = "Please insert the Paintball CD in a local CD Drive";
static const char g_GAME_ClosedDownText[] = "GAME CLOSED DOWN\n";
static const char g_GAME_IsValidResourceFileAssert[] = "IsValidResourceFile()";
static const char g_GAME_SourceFileName[] = "GAME.CPP";
static void *g_GAME_PrimaryContextMenuDefinitionTable = (void *)0x0049e718;

static void *g_GAME_StatusEntryVtable[] = {
    (void *)WriteNamedStatusEntry,
    (void *)UpdateNamedStatusEntry,
    (void *)DeleteFixedBufferStreamReturnThis,
    (void *)ResetFixedBufferStream,
    (void *)AppendCharToFixedBufferStream,
    (void *)AppendCStringToFixedBufferStream,
    (void *)ReturnStreamArgument,
    0,
};

extern void ServiceNetworkLobbySelectedPeerUpdates(void *pVsnetRuntime);
extern int PumpMessagesAndRunFrame(void);
extern void *ClearLevelProgressPasswordState(void *pLevelProgressState);
extern void *AllocateResourceArchiveMemory(unsigned int cbBytes);
extern void *ConstructResourceArchive(void *pArchive, const char *pszArchiveName, unsigned int cbArenaSize);
extern int ValidateResourceFileSignature(void);
extern void TriggerReleaseAssertFailure(const char *pszExpression, const char *pszFile, int nLine);
extern void *ConstructVariantResourceEntryManager(void *pManager);
extern void ReleaseMainGameVariantResourceMode(GAME_MainContext *pMainContext);
extern void EnsureMainGameVariantResourceMode(GAME_MainContext *pMainContext, int nVariantMode);
extern void InitializeBaseModeObject(void *pModeObject, GAME_MainContext *pMainContext);
extern void *ConstructLevelSelectionModeController(void *pModeObject, GAME_MainContext *pMainContext);
extern void *ConstructNetworkLobbyTransportController(void *pModeObject, GAME_MainContext *pMainContext);
extern void *ConstructPaintballSequenceModeController(void *pModeObject,
                                                      GAME_MainContext *pMainContext,
                                                      int fSequenceVariant);
extern void *ConstructLevelGameMode(void *pModeObject, GAME_MainContext *pMainContext);
extern void DestroyMainGameVariantResourceMode(void *pVariantMode);
extern void UnregisterNetworkLobbyVsnetRuntimeFromTransport(void *pVsnetRuntime);
extern void ShutdownVariantResourceEntryManager(void *pManager);
extern void DestroyResourceArchive(void *pArchive);
extern void FreeResourceArchiveMemory(void *pArchiveMemoryBlock);
extern int g_fMusicEnabled;
extern int g_fEffectsEnabled;
extern void *g_pStatusEntryRegistry;
extern void *g_pLevelProgressState;
extern void *g_pMainResourceArchive;
extern void *g_pVariantResourceEntryManager;
extern void *g_pNetworkLobbyVsnetRuntime;
extern void *g_pActiveNetworkRuntimeWindow;
extern void *g_pStartupModeVtable;
extern void *g_pStartupModeRenderThunk;
extern void *g_pMainMenuModeVtable;
extern void *g_pMainMenuModeRenderThunk;
extern void *g_pMode3Vtable;
extern void *g_pMode3RenderThunk;
extern void *g_pSimpleModeDeleteVtable;
extern void *g_pSimpleModeVtable;
extern void *g_pMode16Vtable;
extern void *g_pMode16RenderThunk;
extern void ConstructWindowOwnerRenderContext(void *pPrimaryContext);
extern void *LoadZrleResource(int nResourceId);
extern void *LoadPalResource(int nResourceId);
extern void SetLevelScreenStatusIndicatorMode(int nMode, int nValue);
extern void SetWindowOwnerScaleFactor(void *pPrimaryContext, int nScaleFactor);
extern void InitializeHelperUploadStatePending(int nUploadState);
extern void *ConstructIntroSequenceScreen(void *pScreenObject,
                                          void *pPrimaryContext,
                                          void *pWindowOwnerContext,
                                          short *paRect,
                                          int fSkipVariant);
extern void *ConstructMainMenuScreen(void *pScreenObject,
                                     void *pPrimaryContext,
                                     void *pWindowOwnerContext,
                                     short *paRect);
extern void *ConstructOptionsScreen(void *pScreenObject,
                                    void *pPrimaryContext,
                                    void *pWindowOwnerContext,
                                    short *paRect);
extern void *ConstructLevelSelectionScreen(void *pScreenObject,
                                           void *pPrimaryContext,
                                           void *pWindowOwnerContext,
                                           short *paRect);
extern void *ConstructLevelScreen(void *pScreenObject,
                                  void *pPrimaryContext,
                                  void *pLevelModeObject,
                                  void *pWindowOwnerContext,
                                  int nStatusIndicatorMode,
                                  short *paRect);
extern void *ConstructRegistrationInfoScreen(void *pScreenObject,
                                             void *pPrimaryContext,
                                             void *pWindowOwnerContext,
                                             short *paRect);
extern void *ConstructNetworkLobbyScreen(void *pScreenObject,
                                         void *pPrimaryContext,
                                         void *pWindowOwnerContext,
                                         short *paRect);
extern void *ConstructPaintballSequenceScreen(void *pScreenObject,
                                              void *pPrimaryContext,
                                              void *pWindowOwnerContext,
                                              short *paRect,
                                              int fFirstSequenceVariant);
extern void *ConstructPasswordEntryScreen(void *pScreenObject,
                                          void *pPrimaryContext,
                                          void *pWindowOwnerContext,
                                          short *paRect);
extern void *g_pPrimaryContextVtable;
extern void *g_pPrimaryContextRenderQueueNodeVtable;
extern void *g_pQueuedRenderPointSinkFinalizeThunk;
extern void *g_pSharedRenderDispatchQueue;
extern void *g_pSharedGeometryHelper;
extern "C" DWORD timeGetTime(void);

struct GAME_PrimaryContext {
    void *m_pVtable;
    char m_Reserved4[0x48];
    void *m_pWindowOwnerContext;
    int m_nFrameInterval;
    char m_Reserved54[0x3c];
    void *m_pRenderQueueNodeVtable;
    char m_Reserved94[0xc];
    int m_fA0;
    int m_nA4;
    void *m_pBackgroundZrle;
    void *m_pPalette2E;
    void *m_pPalette2F;
    int m_nLevelScreenStatusIndicatorMode;
    int m_nB8;
    int m_fScreenLayoutDirty;
    GAME_MainContext *m_pMainContext;
    void *m_pActiveLevelMode;
    char m_ReservedC8[4];
    int m_nActiveScreenMode;
    void *m_pFinalizeThunk;
    void *m_pActiveScreen;
    int m_fCompactLayout;
    short m_cxCompactLayout;
    short m_cyCompactLayout;
    short m_cxWideLayout;
    short m_cyWideLayout;
};

static void StubNoOpVoid(void) {
}

static void StubDeleteInt(int) {
}

static int StubReturnZero(void) {
    return 0;
}

static void StubNoOpPtr(void *) {
}

static void *g_GAME_PrimaryContextVtableSlots[30] = {0};
static void *g_GAME_RenderQueueNodeVtableSlots[2] = {0};
static void *g_GAME_GenericModeVtableSlots[2] = {0};
static void *g_GAME_GenericScreenVtableSlots[14] = {0};

void *g_pStatusEntryRegistry = 0;
void *g_pLevelProgressState = 0;
void *g_pMainResourceArchive = 0;
void *g_pVariantResourceEntryManager = 0;
void *g_pNetworkLobbyVsnetRuntime = 0;
void *g_pActiveNetworkRuntimeWindow = 0;
void *g_pStartupModeVtable = g_GAME_GenericModeVtableSlots;
void *g_pStartupModeRenderThunk = 0;
void *g_pMainMenuModeVtable = g_GAME_GenericModeVtableSlots;
void *g_pMainMenuModeRenderThunk = 0;
void *g_pMode3Vtable = g_GAME_GenericModeVtableSlots;
void *g_pMode3RenderThunk = 0;
void *g_pSimpleModeDeleteVtable = g_GAME_GenericModeVtableSlots;
void *g_pSimpleModeVtable = g_GAME_GenericModeVtableSlots;
void *g_pMode16Vtable = g_GAME_GenericModeVtableSlots;
void *g_pMode16RenderThunk = 0;
void *g_pPrimaryContextVtable = g_GAME_PrimaryContextVtableSlots;
void *g_pPrimaryContextRenderQueueNodeVtable = g_GAME_RenderQueueNodeVtableSlots;
void *g_pQueuedRenderPointSinkFinalizeThunk = 0;
void *g_pSharedRenderDispatchQueue = 0;
void *g_pSharedGeometryHelper = 0;

static void InitializeGameStubVtables(void) {
    static int g_fInitialized = 0;

    if (g_fInitialized != 0) {
        return;
    }

    g_GAME_PrimaryContextVtableSlots[0x74 / sizeof(void *)] = (void *)StubNoOpVoid;
    g_GAME_RenderQueueNodeVtableSlots[1] = (void *)StubDeleteInt;

    g_GAME_GenericModeVtableSlots[0] = (void *)StubDeleteInt;
    g_GAME_GenericModeVtableSlots[1] = (void *)StubNoOpVoid;

    g_GAME_GenericScreenVtableSlots[0] = (void *)StubDeleteInt;
    g_GAME_GenericScreenVtableSlots[0x14 / sizeof(void *)] = (void *)StubNoOpPtr;
    g_GAME_GenericScreenVtableSlots[0x18 / sizeof(void *)] = (void *)StubNoOpVoid;
    g_GAME_GenericScreenVtableSlots[0x1c / sizeof(void *)] = (void *)StubNoOpVoid;
    g_GAME_GenericScreenVtableSlots[0x20 / sizeof(void *)] = (void *)StubNoOpVoid;
    g_GAME_GenericScreenVtableSlots[0x24 / sizeof(void *)] = (void *)StubNoOpVoid;
    g_GAME_GenericScreenVtableSlots[0x28 / sizeof(void *)] = (void *)StubReturnZero;
    g_GAME_GenericScreenVtableSlots[0x2c / sizeof(void *)] = (void *)StubReturnZero;
    g_GAME_GenericScreenVtableSlots[0x34 / sizeof(void *)] = (void *)StubNoOpVoid;

    g_fInitialized = 1;
}

void ServiceNetworkLobbySelectedPeerUpdates(void *pVsnetRuntime) {
    (void)pVsnetRuntime;
}

int PumpMessagesAndRunFrame(void) {
    return 1;
}

void *ClearLevelProgressPasswordState(void *pLevelProgressState) {
    if (pLevelProgressState != 0) {
        memset(pLevelProgressState, 0, 0x50);
    }
    return pLevelProgressState;
}

void *AllocateResourceArchiveMemory(unsigned int cbBytes) {
    return AllocateVSMemBlock(cbBytes);
}

void *ConstructResourceArchive(void *pArchive, const char *pszArchiveName, unsigned int cbArenaSize) {
    (void)pszArchiveName;
    (void)cbArenaSize;
    return pArchive;
}

// FUNCTION: LEMBALL 0x0045D180
void ReleaseTypedResourceObjectReference(void *pResourceObject) {
    int *pObject;

    pObject = (int *)pResourceObject;
    --pObject[3];
}

int ValidateResourceFileSignature(void) {
    return 1;
}

void TriggerReleaseAssertFailure(const char *pszExpression, const char *pszFile, int nLine) {
    (void)pszExpression;
    (void)pszFile;
    (void)nLine;
}

void *ConstructVariantResourceEntryManager(void *pManager) {
    return pManager;
}

void ReleaseMainGameVariantResourceMode(GAME_MainContext *pMainContext) {
    (void)pMainContext;
}

void EnsureMainGameVariantResourceMode(GAME_MainContext *pMainContext, int nVariantMode) {
    (void)pMainContext;
    (void)nVariantMode;
}

void InitializeBaseModeObject(void *pModeObject, GAME_MainContext *pMainContext) {
    memset(pModeObject, 0, 0x10);
    ((void **)pModeObject)[0] = g_GAME_GenericModeVtableSlots;
    ((void **)pModeObject)[3] = pMainContext;
}

void *ConstructLevelSelectionModeController(void *pModeObject, GAME_MainContext *pMainContext) {
    memset(pModeObject, 0, 0x28);
    ((void **)pModeObject)[0] = g_GAME_GenericModeVtableSlots;
    ((void **)pModeObject)[3] = pMainContext;
    return pModeObject;
}

void *ConstructNetworkLobbyTransportController(void *pModeObject, GAME_MainContext *pMainContext) {
    memset(pModeObject, 0, 0x38);
    ((void **)pModeObject)[0] = g_GAME_GenericModeVtableSlots;
    ((void **)pModeObject)[3] = pMainContext;
    return pModeObject;
}

void *ConstructPaintballSequenceModeController(void *pModeObject,
                                               GAME_MainContext *pMainContext,
                                               int fSequenceVariant) {
    (void)fSequenceVariant;
    memset(pModeObject, 0, 0x2c);
    ((void **)pModeObject)[0] = g_GAME_GenericModeVtableSlots;
    ((void **)pModeObject)[3] = pMainContext;
    return pModeObject;
}

void *ConstructLevelGameMode(void *pModeObject, GAME_MainContext *pMainContext) {
    memset(pModeObject, 0, 0x1f0);
    *(void **)((char *)pModeObject + 0x10) = g_GAME_GenericModeVtableSlots;
    *(GAME_MainContext **)((char *)pModeObject + 0x1c) = pMainContext;
    *(int *)((char *)pModeObject + 0x110) = 0;
    return pModeObject;
}

void DestroyMainGameVariantResourceMode(void *pVariantMode) {
    (void)pVariantMode;
}

void UnregisterNetworkLobbyVsnetRuntimeFromTransport(void *pVsnetRuntime) {
    (void)pVsnetRuntime;
}

void ShutdownVariantResourceEntryManager(void *pManager) {
    (void)pManager;
}

void DestroyResourceArchive(void *pArchive) {
    (void)pArchive;
}

void FreeResourceArchiveMemory(void *pArchiveMemoryBlock) {
    FreeVSMemBlock(pArchiveMemoryBlock);
}

void ConstructWindowOwnerRenderContext(void *pPrimaryContext) {
    InitializeGameStubVtables();
    memset(pPrimaryContext, 0, 0xe4);
}

// FUNCTION: LEMBALL 0x00462EA0
void InitializeRenderQueueNodeBase(void *pRenderQueueNode) {
    int *pNode;

    pNode = (int *)pRenderQueueNode;
    pNode[0] = (int)(unsigned long)g_GAME_RenderQueueNodeVtableSlots;
    pNode[2] = 0;
    pNode[3] = 0;
    pNode[1] = 0x51484452;
}

void *LoadZrleResource(int nResourceId) {
    (void)nResourceId;
    return 0;
}

void *LoadPalResource(int nResourceId) {
    (void)nResourceId;
    return 0;
}

void SetLevelScreenStatusIndicatorMode(int nMode, int nValue) {
    (void)nMode;
    (void)nValue;
}

// FUNCTION: LEMBALL 0x004632A0
void RegisterOrderedRenderDispatchClient(void *pDispatchQueue, void *pClient, int nOrder) {
    int *pQueue;
    int *pNode;
    int *pCurrent;
    int *pPrevious;
    unsigned int i;

    pQueue = (int *)pDispatchQueue;
    pNode = (int *)AllocateVSMemBlock(0xc);
    pNode[0] = (int)(unsigned long)pClient;
    pNode[1] = nOrder;
    pNode[2] = 0;

    pCurrent = *(int **)((char *)pQueue + 0x54);
    if (pCurrent == 0 || pQueue[0x2c / 4] == 0) {
        *(int **)((char *)pQueue + 0x54) = pNode;
        pQueue[0x2c / 4] = 1;
        return;
    }

    pPrevious = pCurrent;
    i = 0;
    while (i < (unsigned int)pQueue[0x2c / 4]) {
        if (nOrder < pCurrent[1]) {
            if (i == 0) {
                pNode[2] = *(int *)((char *)pQueue + 0x54);
                *(int **)((char *)pQueue + 0x54) = pNode;
            } else {
                pNode[2] = pPrevious[2];
                pPrevious[2] = (int)(unsigned long)pNode;
            }
            ++pQueue[0x2c / 4];
            return;
        }
        if (pCurrent[2] == 0) {
            pCurrent[2] = (int)(unsigned long)pNode;
            ++pQueue[0x2c / 4];
            return;
        }
        ++i;
        pPrevious = pCurrent;
        pCurrent = (int *)(unsigned long)pCurrent[2];
    }
}

// FUNCTION: LEMBALL 0x004633B0
void UnregisterOrderedRenderDispatchClient(void *pDispatchQueue, void *pClient, int nOrder) {
    int *pQueue;
    int *pCurrent;
    int *pPrevious;
    unsigned int i;

    pQueue = (int *)pDispatchQueue;
    pCurrent = *(int **)((char *)pQueue + 0x54);
    pPrevious = pCurrent;
    i = 0;
    while (i < (unsigned int)pQueue[0x2c / 4]) {
        if (pCurrent[1] == nOrder && pCurrent[0] == (int)(unsigned long)pClient) {
            if (i != 0) {
                pPrevious[2] = pCurrent[2];
            } else {
                *(int *)((char *)pQueue + 0x54) = pCurrent[2];
            }
            FreeVSMemBlock(pCurrent);
            --pQueue[0x2c / 4];
            return;
        }
        ++i;
        pPrevious = pCurrent;
        pCurrent = (int *)(unsigned long)pCurrent[2];
    }
}

void SetWindowOwnerScaleFactor(void *pPrimaryContext, int nScaleFactor) {
    (void)pPrimaryContext;
    (void)nScaleFactor;
}

void InitializeHelperUploadStatePending(int nUploadState) {
    (void)nUploadState;
}

static void *ConstructStubScreenObject(void *pScreenObject, unsigned int cbScreenObject) {
    InitializeGameStubVtables();
    memset(pScreenObject, 0, cbScreenObject);
    *(void **)pScreenObject = g_GAME_GenericScreenVtableSlots;
    return pScreenObject;
}

void *ConstructIntroSequenceScreen(void *pScreenObject,
                                   void *pPrimaryContext,
                                   void *pWindowOwnerContext,
                                   short *paRect,
                                   int fSkipVariant) {
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    (void)fSkipVariant;
    return ConstructStubScreenObject(pScreenObject, 0x484);
}

void *ConstructMainMenuScreen(void *pScreenObject, void *pPrimaryContext, void *pWindowOwnerContext, short *paRect) {
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    return ConstructStubScreenObject(pScreenObject, 0x3bc);
}

void *ConstructOptionsScreen(void *pScreenObject, void *pPrimaryContext, void *pWindowOwnerContext, short *paRect) {
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    return ConstructStubScreenObject(pScreenObject, 0x3ac);
}

void *ConstructLevelSelectionScreen(void *pScreenObject,
                                    void *pPrimaryContext,
                                    void *pWindowOwnerContext,
                                    short *paRect) {
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    return ConstructStubScreenObject(pScreenObject, 0x488);
}

void *ConstructLevelScreen(void *pScreenObject,
                           void *pPrimaryContext,
                           void *pLevelModeObject,
                           void *pWindowOwnerContext,
                           int nStatusIndicatorMode,
                           short *paRect) {
    (void)pPrimaryContext;
    (void)pLevelModeObject;
    (void)pWindowOwnerContext;
    (void)nStatusIndicatorMode;
    (void)paRect;
    return ConstructStubScreenObject(pScreenObject, 0x2428);
}

void *ConstructRegistrationInfoScreen(void *pScreenObject,
                                      void *pPrimaryContext,
                                      void *pWindowOwnerContext,
                                      short *paRect) {
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    return ConstructStubScreenObject(pScreenObject, 0x9c);
}

void *ConstructNetworkLobbyScreen(void *pScreenObject,
                                  void *pPrimaryContext,
                                  void *pWindowOwnerContext,
                                  short *paRect) {
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    return ConstructStubScreenObject(pScreenObject, 0x438);
}

void *ConstructPaintballSequenceScreen(void *pScreenObject,
                                       void *pPrimaryContext,
                                       void *pWindowOwnerContext,
                                       short *paRect,
                                       int fFirstSequenceVariant) {
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    (void)fFirstSequenceVariant;
    return ConstructStubScreenObject(pScreenObject, 0x60c);
}

void *ConstructPasswordEntryScreen(void *pScreenObject,
                                   void *pPrimaryContext,
                                   void *pWindowOwnerContext,
                                   short *paRect) {
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    return ConstructStubScreenObject(pScreenObject, 0x494);
}

extern "C" DWORD timeGetTime(void) {
    return 0;
}

static void CopyCStringUnchecked(char *pszTarget, const char *pszSource) {
    size_t cchCopy;

    if (pszTarget == 0 || pszSource == 0) {
        return;
    }

    cchCopy = strlen(pszSource) + 1;
    memcpy(pszTarget, pszSource, cchCopy);
}

static void CallNoArgVirtual(void *pObject, unsigned int nByteOffset) {
    void **pVtable;

    pVtable = *(void ***)pObject;
    ((void (*)())pVtable[nByteOffset / sizeof(void *)])();
}

static void CallDeleteVirtual(void *pObject, unsigned int nByteOffset, int nDeleteFlag) {
    void **pVtable;

    pVtable = *(void ***)pObject;
    ((void (*)(int))pVtable[nByteOffset / sizeof(void *)])(nDeleteFlag);
}

static int CallNestedVirtualInt(void *pObject, unsigned int nObjectOffset, unsigned int nVtableOffset) {
    void *pSubobject;
    void **pVtable;

    pSubobject = *(void **)((char *)pObject + nObjectOffset);
    pVtable = *(void ***)pSubobject;
    return ((int (*)())pVtable[nVtableOffset / sizeof(void *)])();
}

static int CallIntVirtual(void *pObject, unsigned int nByteOffset) {
    void **pVtable;

    pVtable = *(void ***)pObject;
    return ((int (*)())pVtable[nByteOffset / sizeof(void *)])();
}

static void CallIntArgVirtual(void *pObject, unsigned int nByteOffset, int nArg) {
    void **pVtable;

    pVtable = *(void ***)pObject;
    ((void (*)(int))pVtable[nByteOffset / sizeof(void *)])(nArg);
}

static void *CreateRawBaseModeObject(unsigned int cbModeObject,
                                     GAME_MainContext *pMainContext,
                                     void *pModeVtable,
                                     void *pRenderThunk) {
    void **pModeObject;

    pModeObject = (void **)AllocateVSMemBlock(cbModeObject);
    if (pModeObject == 0) {
        return 0;
    }

    InitializeBaseModeObject(pModeObject, pMainContext);
    pModeObject[0] = pModeVtable;
    pModeObject[3] = pRenderThunk;
    return pModeObject;
}

static void *CreateSimpleModeObject(GAME_MainContext *pMainContext) {
    void **pModeObject;

    pModeObject = (void **)AllocateVSMemBlock(0x10);
    if (pModeObject == 0) {
        return 0;
    }

    pModeObject[0] = g_pSimpleModeDeleteVtable;
    pModeObject[1] = 0;
    pModeObject[2] = 0;
    pModeObject[0] = g_pSimpleModeVtable;
    pModeObject[3] = pMainContext;
    return pModeObject;
}

GAME_PrimaryContext *ConstructPrimaryContext(GAME_PrimaryContext *pPrimaryContext, GAME_MainContext *pMainContext);
void ComputePrimaryContextCenteredScreenRect(GAME_PrimaryContext *pPrimaryContext,
                                             short *paRect,
                                             int nLeftOverride,
                                             int nTopOverride);
void DestroyPrimaryContextActiveScreen(void *pPrimaryContext);
void SwitchPrimaryContextScreen(void *pPrimaryContext, int nMode);
void ForwardPrimaryContextActiveScreenSlot0x14(void *pPrimaryContext);
int IsPrimaryContextWindowReadyForPresent(int *pPrimaryContext);
void PresentPrimaryContextScreen(void *pPrimaryContext);
void UpdatePrimaryContextActiveScreen(void *pPrimaryContext);
int GetPrimaryContextMenuDefinition(int *pnMenuId, void **ppMenuDefinition);
int PollPrimaryContextScreenStatus(void *pPrimaryContext);
int GetPrimaryContextRequestedScreen(void *pPrimaryContext);
void UpdateMainGameActiveMode(void *pMainContext);
void PresentMainGameFrame(void *pMainContext);
void RunMainGameLoop(GAME_MainContext *pMainContext);
void SwitchMainGameMode(GAME_MainContext *pMainContext, int nMode);

static void CopyCString(char *pszTarget, unsigned int cchTarget, const char *pszSource) {
    unsigned int i;

    if (pszTarget == 0 || cchTarget == 0) {
        return;
    }

    if (pszSource == 0) {
        pszTarget[0] = '\0';
        return;
    }

    i = 0;
    while (pszSource[i] != '\0' && i + 1 < cchTarget) {
        pszTarget[i] = pszSource[i];
        ++i;
    }
    pszTarget[i] = '\0';
}

static void AppendCString(char *pszTarget, unsigned int cchTarget, const char *pszSuffix) {
    unsigned int cchCurrent;

    if (pszTarget == 0 || cchTarget == 0) {
        return;
    }

    cchCurrent = 0;
    while (pszTarget[cchCurrent] != '\0' && cchCurrent < cchTarget) {
        ++cchCurrent;
    }

    if (cchCurrent < cchTarget) {
        CopyCString(pszTarget + cchCurrent, cchTarget - cchCurrent, pszSuffix);
    }
}

// FUNCTION: LEMBALL 0x0045ECB0
int SetVisualSciencesRegistryRunningState(const char *pszProductName, int fRunning) {
    char szKeyPath[256];
    HKEY hRegistryKey;
    const char *pszState;
    unsigned int cbState;

    CopyCString(szKeyPath, sizeof(szKeyPath), g_GAME_RegistryRoot);
    AppendCString(szKeyPath, sizeof(szKeyPath), pszProductName);

    if (RegOpenKeyExA((HKEY)0x80000002, szKeyPath, 0, KEY_ALL_ACCESS, &hRegistryKey) != 0) {
        return 0;
    }

    pszState = g_GAME_RunningState;
    if (!fRunning) {
        pszState = g_GAME_NotRunningState;
    }

    cbState = 0;
    while (pszState[cbState] != '\0') {
        ++cbState;
    }
    ++cbState;

    if (RegSetValueExA(hRegistryKey, g_GAME_RunningValueName, 0, 1, (const BYTE *)pszState, cbState) != 0) {
        RegCloseKey(hRegistryKey);
        return 0;
    }

    RegCloseKey(hRegistryKey);
    return 1;
}

// FUNCTION: LEMBALL 0x0046DCD0
char *GetSrcDiskRegistryValueBuffer(void) {
    HKEY hRegistryKey;
    DWORD cbValue;
    DWORD dwType;

    g_GAME_SrcDiskBuffer[0] = '\0';
    if (RegOpenKeyExA((HKEY)0x80000002, g_GAME_RegistryKey, 0, KEY_ALL_ACCESS, &hRegistryKey) != 0) {
        return g_GAME_SrcDiskBuffer;
    }

    cbValue = sizeof(g_GAME_SrcDiskBuffer);
    dwType = 0xffffffffu;
    if (RegQueryValueExA(hRegistryKey, g_GAME_SrcDiskValueName, 0, &dwType, (BYTE *)g_GAME_SrcDiskBuffer, &cbValue) !=
        0) {
        g_GAME_SrcDiskBuffer[0] = '\0';
        RegCloseKey(hRegistryKey);
        return g_GAME_SrcDiskBuffer;
    }

    RegCloseKey(hRegistryKey);
    return g_GAME_SrcDiskBuffer;
}

// FUNCTION: LEMBALL 0x0045EDA0
char *FindCdromFilePathBySuffix(const char *pszSuffix) {
    static char szCandidatePath[256] = "A:\\";
    DWORD dwDrives;
    char chDrive;
    int i;

    if (pszSuffix == 0) {
        return 0;
    }

    CopyCString(szCandidatePath + 3, sizeof(szCandidatePath) - 3, pszSuffix);
    dwDrives = GetLogicalDrives();
    chDrive = 'A';
    i = 0;

    while (i <= 31) {
        if ((dwDrives & 1u) != 0) {
            szCandidatePath[0] = chDrive;
            if (GetDriveTypeA(szCandidatePath) == DRIVE_CDROM) {
                FILE *pFile;

                pFile = fopen(szCandidatePath, "rb");
                if (pFile != 0) {
                    fclose(pFile);
                    return szCandidatePath;
                }
            }
        }

        dwDrives >>= 1;
        ++chDrive;
        ++i;
    }

    return 0;
}

static void ShowStartupMessage(const char *pszTitle, const char *pszText) {
    lemball_platform_show_error(pszTitle, pszText);
}

// FUNCTION: LEMBALL 0x0046E410
GAME_DynamicCString *ConstructDynamicCString(GAME_DynamicCString *pString) {
    pString->m_cchCapacity = 1;
    pString->m_pszText = (char *)AllocateVSMemBlock(1);
    pString->m_pszText[0] = '\0';
    return pString;
}

// FUNCTION: LEMBALL 0x0046E500
void DestroyDynamicCString(GAME_DynamicCString *pString) {
    FreeVSMemBlock(pString->m_pszText);
}

// FUNCTION: LEMBALL 0x0046E570
GAME_DynamicCString *AssignDynamicCString(GAME_DynamicCString *pString, const char *pszText) {
    const char *pszScan;
    const char *pszSource;
    char *pszDest;
    unsigned int cchText;
    unsigned int cDwords;
    unsigned int cBytes;

    cchText = 0xffffffff;
    pszScan = pszText;
    do {
        if (cchText == 0) {
            break;
        }
        --cchText;
    } while (*pszScan++ != '\0');
    cchText = ~cchText;
    if (pString->m_cchCapacity < (int)cchText) {
        FreeVSMemBlock(pString->m_pszText);
        pString->m_pszText = (char *)AllocateVSMemBlock((unsigned int)cchText);
        pString->m_cchCapacity = (int)cchText;
    }

    cchText = 0xffffffff;
    pszScan = pszText;
    do {
        pszSource = pszScan;
        if (cchText == 0) {
            break;
        }
        --cchText;
        pszSource = pszScan + 1;
    } while (*pszScan++ != '\0');
    cchText = ~cchText;
    pszSource -= cchText;
    pszDest = pString->m_pszText;
    cDwords = cchText >> 2;
    while (cDwords != 0) {
        *(unsigned int *)pszDest = *(const unsigned int *)pszSource;
        pszSource += 4;
        pszDest += 4;
        --cDwords;
    }
    cBytes = cchText & 3;
    while (cBytes != 0) {
        *pszDest++ = *pszSource++;
        --cBytes;
    }
    return pString;
}

// FUNCTION: LEMBALL 0x0045AC10
GAME_StatusEntry::GAME_StatusEntry(const char *pszName) {
    m_pVtable = g_GAME_StatusEntryVtable;
    ConstructDynamicCString(&m_Name);
    m_nReserved04 = 0;
    AssignDynamicCString(&m_Name, pszName);
    m_nMinimumValue = -1;
    m_nMaximumValue = 0;
    m_nTotalValue = 0;
    m_cSamples = 0;
}

// FUNCTION: LEMBALL 0x0045AC50
void DestroyNamedStatusEntry(void *pEntry) {
    GAME_StatusEntry *pStatusEntry;

    pStatusEntry = (GAME_StatusEntry *)pEntry;
    DestroyDynamicCString(&pStatusEntry->m_Name);
}

void UpdateNamedStatusEntry(void *pEntry, unsigned int nValue) {
    GAME_StatusEntry *pStatusEntry;

    pStatusEntry = (GAME_StatusEntry *)pEntry;
    if (pStatusEntry->m_cSamples != 0) {
        pStatusEntry->m_nTotalValue += nValue;
        if ((unsigned int)pStatusEntry->m_nMaximumValue <= nValue) {
            pStatusEntry->m_nMaximumValue = (int)nValue;
        }
        if ((unsigned int)pStatusEntry->m_nMinimumValue >= nValue) {
            pStatusEntry->m_nMinimumValue = (int)nValue;
        }
    }
    ++pStatusEntry->m_cSamples;
}

VSINIT_FormattedOutputStream *WriteNamedStatusEntry(void *pEntry, VSINIT_FormattedOutputStream *pStream) {
    GAME_StatusEntry *pStatusEntry;
    unsigned int nAverage;

    pStatusEntry = (GAME_StatusEntry *)pEntry;
    if (pStatusEntry->m_cSamples == 0) {
        AppendCStringToStream(pStream, "----\n");
        return pStream;
    }

    nAverage = (unsigned int)pStatusEntry->m_nTotalValue / (unsigned int)pStatusEntry->m_cSamples;
    AppendHexUIntToStream(pStream, nAverage);
    AppendCStringToStream(pStream, " ");
    AppendHexUIntToStream(pStream, (unsigned int)pStatusEntry->m_nTotalValue);
    AppendCStringToStream(pStream, " ");
    AppendHexUIntToStream(pStream, (unsigned int)pStatusEntry->m_nMaximumValue);
    AppendCStringToStream(pStream, " ");
    AppendHexUIntToStream(pStream, (unsigned int)pStatusEntry->m_nMinimumValue);
    AppendCStringToStream(pStream, " ");
    AppendHexUIntToStream(pStream, (unsigned int)pStatusEntry->m_cSamples);
    AppendCStringToStream(pStream, " ");
    AppendCStringToStream(pStream, pStatusEntry->m_Name.m_pszText);
    AppendCStringToStream(pStream, "\n");
    return pStream;
}

GAME_MainContext::GAME_MainContext(void) {
    m_szCaption[0] = '\0';
    m_pProcessingStatus = 0;
    m_pRefreshingStatus = 0;
    m_nModeTicks = 0;
    m_pActiveMode = 0;
    m_fQuitRequested = 0;
    m_pPrimaryContext = 0;
    m_nActiveMode = 0;
    m_pVariantMode = 0;
}

static GAME_StatusEntry *AllocateNamedStatusEntry(const char *pszName) {
    GAME_StatusEntry *pEntry;

    pEntry = new (AllocateVSMemBlock(0x20)) GAME_StatusEntry(pszName);
    if (pEntry == 0) {
        return 0;
    }
    return pEntry;
}

// FUNCTION: LEMBALL 0x00431F30
void ComputePrimaryContextCenteredScreenRect(GAME_PrimaryContext *pPrimaryContext,
                                             short *paRect,
                                             int nLeftOverride,
                                             int nTopOverride) {
    short cxScreen;
    short cyScreen;
    short cxTarget;
    short cyTarget;
    short xLeft;
    short yTop;

    g_fCompactPrimaryContextLayout = GetSelectedGraphicsDriverId() == 3;
    if (GetSelectedGraphicsDriverId() == 3) {
        cxTarget = pPrimaryContext->m_cxCompactLayout;
        cyTarget = pPrimaryContext->m_cyCompactLayout;
    } else {
        cxTarget = pPrimaryContext->m_cxWideLayout;
        cyTarget = pPrimaryContext->m_cyWideLayout;
    }

    cxScreen = GetDisplayState()->Width();
    cyScreen = GetDisplayState()->Height();
    xLeft = (short)((cxScreen - cxTarget) / 2);
    yTop = (short)((cyScreen - cyTarget) / 2);

    if (nLeftOverride != -1) {
        xLeft = (short)nLeftOverride;
    }
    if (nTopOverride != -1) {
        yTop = (short)nTopOverride;
    }

    paRect[0] = cxTarget;
    paRect[1] = cyTarget;
    paRect[2] = xLeft;
    paRect[3] = yTop;
}

// FUNCTION: LEMBALL 0x00431590
GAME_PrimaryContext *ConstructPrimaryContext(GAME_PrimaryContext *pPrimaryContext, GAME_MainContext *pMainContext) {
    ConstructWindowOwnerRenderContext(pPrimaryContext);
    InitializeRenderQueueNodeBase(&pPrimaryContext->m_pRenderQueueNodeVtable);

    pPrimaryContext->m_pFinalizeThunk = g_pQueuedRenderPointSinkFinalizeThunk;
    pPrimaryContext->m_cyCompactLayout = 0;
    pPrimaryContext->m_pVtable = &g_pPrimaryContextVtable;
    pPrimaryContext->m_cxCompactLayout = 0;
    pPrimaryContext->m_pRenderQueueNodeVtable = g_pPrimaryContextRenderQueueNodeVtable;
    pPrimaryContext->m_cyWideLayout = 0;
    *(int *)((char *)pPrimaryContext + 200) = 0;
    pPrimaryContext->m_cxWideLayout = 0;
    pPrimaryContext->m_nA4 = 0;
    pPrimaryContext->m_nB8 = 0;
    pPrimaryContext->m_pMainContext = pMainContext;
    pPrimaryContext->m_pActiveLevelMode = 0;
    pPrimaryContext->m_fA0 = 1;
    pPrimaryContext->m_nLevelScreenStatusIndicatorMode = 0;
    pPrimaryContext->m_pActiveScreen = 0;
    pPrimaryContext->m_fScreenLayoutDirty = 1;
    pPrimaryContext->m_nFrameInterval = 600;
    pPrimaryContext->m_nActiveScreenMode = 3;
    pPrimaryContext->m_pBackgroundZrle = LoadZrleResource(0xfc);
    pPrimaryContext->m_pPalette2E = LoadPalResource(0x2e);
    pPrimaryContext->m_pPalette2F = LoadPalResource(0x2f);

    SetLevelScreenStatusIndicatorMode(2, 0);
    RegisterOrderedRenderDispatchClient(g_pSharedRenderDispatchQueue, &pPrimaryContext->m_pRenderQueueNodeVtable, -0x19);

    pPrimaryContext->m_cxCompactLayout = 0x140;
    pPrimaryContext->m_cyCompactLayout = 0xf0;
    pPrimaryContext->m_cxWideLayout = 0x280;
    pPrimaryContext->m_cyWideLayout = 0x1e0;
    pPrimaryContext->m_fCompactLayout = g_fCompactPrimaryContextLayout;
    return pPrimaryContext;
}

// FUNCTION: LEMBALL 0x00431950
void DestroyPrimaryContextActiveScreen(void *pPrimaryContext) {
    GAME_PrimaryContext *pContext;

    pContext = (GAME_PrimaryContext *)pPrimaryContext;
    pContext->m_fScreenLayoutDirty = 1;
    if (pContext->m_pActiveScreen != 0) {
        CallNoArgVirtual(pContext->m_pActiveScreen, 0x18);
        if (pContext->m_pActiveScreen != 0) {
            CallDeleteVirtual(pContext->m_pActiveScreen, 0, 1);
        }
        pContext->m_pActiveScreen = 0;
    }
}

// FUNCTION: LEMBALL 0x00431990
void SwitchPrimaryContextScreen(void *pPrimaryContext, int nMode) {
    GAME_PrimaryContext *pContext;
    void *pScreenObject;
    short aRect[4];
    int nUploadState;
    void *pActiveLevelMode;

    pContext = (GAME_PrimaryContext *)pPrimaryContext;
    if (pContext->m_nActiveScreenMode == nMode) {
        return;
    }

    SetWindowOwnerScaleFactor(pContext, 1);
    aRect[0] = *(short *)((char *)pContext + 8);
    aRect[1] = *(short *)((char *)pContext + 10);
    aRect[2] = 0;
    aRect[3] = 0;

    nUploadState = CallNestedVirtualInt(pContext, 0x4c, 8);
    InitializeHelperUploadStatePending(nUploadState);
    PromoteHelperUploadStateToActive(nUploadState);

    pContext->m_nActiveScreenMode = nMode;
    switch (nMode) {
    case 1:
        pScreenObject = AllocateVSMemBlock(0x484);
        if (pScreenObject == 0) {
            pContext->m_pActiveScreen = 0;
            break;
        }
        pContext->m_pActiveScreen =
            ConstructIntroSequenceScreen(pScreenObject, pContext, pContext->m_pWindowOwnerContext, aRect, 0);
        break;

    case 2:
        pScreenObject = AllocateVSMemBlock(0x3bc);
        if (pScreenObject == 0) {
            pContext->m_pActiveScreen = 0;
            break;
        }
        pContext->m_pActiveScreen =
            ConstructMainMenuScreen(pScreenObject, pContext, pContext->m_pWindowOwnerContext, aRect);
        break;

    case 3:
        pScreenObject = AllocateVSMemBlock(0x3ac);
        if (pScreenObject == 0) {
            pContext->m_pActiveScreen = 0;
            break;
        }
        pContext->m_pActiveScreen =
            ConstructOptionsScreen(pScreenObject, pContext, pContext->m_pWindowOwnerContext, aRect);
        break;

    case 4:
        pScreenObject = AllocateVSMemBlock(0x488);
        if (pScreenObject == 0) {
            pContext->m_pActiveScreen = 0;
            break;
        }
        pContext->m_pActiveScreen =
            ConstructLevelSelectionScreen(pScreenObject, pContext, pContext->m_pWindowOwnerContext, aRect);
        break;

    case 5:
    case 0x13:
        pActiveLevelMode = 0;
        if (pContext->m_pMainContext->m_pActiveMode != 0) {
            pActiveLevelMode = (char *)pContext->m_pMainContext->m_pActiveMode - 0x10;
        }
        pContext->m_pActiveLevelMode = pActiveLevelMode;
        if (pActiveLevelMode != 0) {
            pContext->m_nLevelScreenStatusIndicatorMode = *(int *)((char *)pActiveLevelMode + 0x110);
        } else {
            pContext->m_nLevelScreenStatusIndicatorMode = 0;
        }
        pScreenObject = AllocateVSMemBlock(0x2428);
        if (pScreenObject == 0) {
            pContext->m_pActiveScreen = 0;
            break;
        }
        pContext->m_pActiveScreen = ConstructLevelScreen(pScreenObject,
                                                         pContext,
                                                         pContext->m_pActiveLevelMode,
                                                         pContext->m_pWindowOwnerContext,
                                                         pContext->m_nLevelScreenStatusIndicatorMode,
                                                         aRect);
        break;

    case 10:
        pScreenObject = AllocateVSMemBlock(0x9c);
        if (pScreenObject == 0) {
            pContext->m_pActiveScreen = 0;
            break;
        }
        pContext->m_pActiveScreen =
            ConstructRegistrationInfoScreen(pScreenObject, pContext, pContext->m_pWindowOwnerContext, aRect);
        break;

    case 0x0c:
        pScreenObject = AllocateVSMemBlock(0x438);
        if (pScreenObject == 0) {
            pContext->m_pActiveScreen = 0;
            break;
        }
        pContext->m_pActiveScreen =
            ConstructNetworkLobbyScreen(pScreenObject, pContext, pContext->m_pWindowOwnerContext, aRect);
        break;

    case 0x0e:
        pScreenObject = AllocateVSMemBlock(0x60c);
        if (pScreenObject == 0) {
            pContext->m_pActiveScreen = 0;
            break;
        }
        pContext->m_pActiveScreen =
            ConstructPaintballSequenceScreen(pScreenObject, pContext, pContext->m_pWindowOwnerContext, aRect, 1);
        break;

    case 0x0f:
        pScreenObject = AllocateVSMemBlock(0x60c);
        if (pScreenObject == 0) {
            pContext->m_pActiveScreen = 0;
            break;
        }
        pContext->m_pActiveScreen =
            ConstructPaintballSequenceScreen(pScreenObject, pContext, pContext->m_pWindowOwnerContext, aRect, 0);
        break;

    case 0x10:
        pScreenObject = AllocateVSMemBlock(0x494);
        if (pScreenObject == 0) {
            pContext->m_pActiveScreen = 0;
            break;
        }
        pContext->m_pActiveScreen =
            ConstructPasswordEntryScreen(pScreenObject, pContext, pContext->m_pWindowOwnerContext, aRect);
        break;

    case 0x12:
        pScreenObject = AllocateVSMemBlock(0x484);
        if (pScreenObject == 0) {
            pContext->m_pActiveScreen = 0;
            break;
        }
        pContext->m_pActiveScreen =
            ConstructIntroSequenceScreen(pScreenObject, pContext, pContext->m_pWindowOwnerContext, aRect, 1);
        break;

    default:
        break;
    }

    if (pContext->m_pActiveScreen != 0) {
        CallNoArgVirtual(pContext->m_pActiveScreen, 0x24);
        CallNoArgVirtual(pContext->m_pActiveScreen, 0x20);
    }
}

// FUNCTION: LEMBALL 0x004318C0
int IsPrimaryContextWindowReadyForPresent(int *pPrimaryContext) {
    if (!CallIntVirtual(pPrimaryContext, 0x68)) {
        return 0;
    }

    if ((short)pPrimaryContext[0x37] == (short)pPrimaryContext[2] &&
        *(short *)((char *)pPrimaryContext + 0xde) == *(short *)((char *)pPrimaryContext + 10)) {
        return 1;
    }

    if ((short)pPrimaryContext[0x38] == (short)pPrimaryContext[2] &&
        *(short *)((char *)pPrimaryContext + 0xe2) == *(short *)((char *)pPrimaryContext + 10)) {
        return 1;
    }

    return 0;
}

// FUNCTION: LEMBALL 0x004318A0
void ForwardPrimaryContextActiveScreenSlot0x14(void *pPrimaryContext) {
    if (*(void **)((char *)pPrimaryContext + 0xd4) != 0) {
        ((void (*)(void *))(*(void ***)*(void **)((char *)pPrimaryContext + 0xd4))[0x14 / sizeof(void *)])(
            (char *)pPrimaryContext + 8);
    }
}

// FUNCTION: LEMBALL 0x00431910
void PresentPrimaryContextScreen(void *pPrimaryContext) {
    int *pContext;
    void *pActiveScreen;

    pContext = (int *)pPrimaryContext;
    if (IsPrimaryContextWindowReadyForPresent(pContext) != 0) {
        if (pContext[1] == 1) {
            CallIntArgVirtual(pContext, 0x70, 0);
        }
        pActiveScreen = *(void **)((char *)pPrimaryContext + 0xd4);
        if (pActiveScreen != 0) {
            CallNoArgVirtual(pActiveScreen, 0x34);
        }
    }
}

// FUNCTION: LEMBALL 0x00431940
void UpdatePrimaryContextActiveScreen(void *pPrimaryContext) {
    if (*(void **)((char *)pPrimaryContext + 0xd4) != 0) {
        CallNoArgVirtual(*(void **)((char *)pPrimaryContext + 0xd4), 0x1c);
    }
}

// FUNCTION: LEMBALL 0x00431EB0
int GetPrimaryContextMenuDefinition(int *pnMenuId, void **ppMenuDefinition) {
    *pnMenuId = 0x73;
    *ppMenuDefinition = g_GAME_PrimaryContextMenuDefinitionTable;
    return 1;
}

// FUNCTION: LEMBALL 0x00431EE0
int PollPrimaryContextScreenStatus(void *pPrimaryContext) {
    if (*(int *)((char *)pPrimaryContext + 0xc8) != 0) {
        return 2;
    }
    if (*(void **)((char *)pPrimaryContext + 0xd4) != 0) {
        return CallIntVirtual(*(void **)((char *)pPrimaryContext + 0xd4), 0x2c);
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00431F10
int GetPrimaryContextRequestedScreen(void *pPrimaryContext) {
    if (*(int *)((char *)pPrimaryContext + 0xc8) == 0 && *(void **)((char *)pPrimaryContext + 0xd4) != 0) {
        return CallIntVirtual(*(void **)((char *)pPrimaryContext + 0xd4), 0x28);
    }
    return 0;
}

// FUNCTION: LEMBALL 0x004077E0
void UpdateMainGameActiveMode(void *pMainGameContext) {
    GAME_MainContext *pMainContext;
    int fTimedStatus;
    DWORD dwNow;
    int nScreenStatus;

    pMainContext = (GAME_MainContext *)pMainGameContext;
    UpdatePrimaryContextActiveScreen(pMainContext->m_pPrimaryContext);
    if (pMainContext->m_pActiveMode != 0) {
        fTimedStatus = 0;
        if ((pMainContext->m_nActiveMode == 5 || pMainContext->m_nActiveMode == 0x13) &&
            0x32 < pMainContext->m_nModeTicks) {
            fTimedStatus = 1;
            dwNow = timeGetTime();
            *(DWORD *)((char *)pMainContext->m_pProcessingStatus + 0x20) = dwNow;
            *(int *)((char *)pMainContext->m_pProcessingStatus + 0x24) = 1;
        }

        CallNoArgVirtual(pMainContext->m_pActiveMode, 4);

        if (fTimedStatus != 0 && *(int *)((char *)pMainContext->m_pProcessingStatus + 0x24) != 0) {
            dwNow = timeGetTime();
            ((void (*)(DWORD))(*(void ***)pMainContext->m_pProcessingStatus)[1])(
                dwNow - *(DWORD *)((char *)pMainContext->m_pProcessingStatus + 0x20));
            *(int *)((char *)pMainContext->m_pProcessingStatus + 0x24) = 0;
        }

        if (g_pNetworkLobbyVsnetRuntime != 0) {
            ServiceNetworkLobbySelectedPeerUpdates(g_pNetworkLobbyVsnetRuntime);
        }

        nScreenStatus = *(int *)((char *)pMainContext->m_pActiveMode + 8);
        if (nScreenStatus == 1) {
            SwitchMainGameMode(pMainContext, *(int *)((char *)pMainContext->m_pActiveMode + 4));
        } else if (nScreenStatus == 2) {
            pMainContext->m_fQuitRequested = 1;
        }
    }

    nScreenStatus = PollPrimaryContextScreenStatus(pMainContext->m_pPrimaryContext);
    if (nScreenStatus == 1) {
        SwitchMainGameMode(pMainContext, GetPrimaryContextRequestedScreen(pMainContext->m_pPrimaryContext));
    } else if (nScreenStatus == 2) {
        pMainContext->m_fQuitRequested = 1;
    }

    if (pMainContext->m_fQuitRequested != 0) {
        if (((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext)->m_pActiveScreen != 0) {
            CallNoArgVirtual(((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext)->m_pActiveScreen, 4);
        }
        if (pMainContext->m_pActiveMode != 0) {
            CallDeleteVirtual(pMainContext->m_pActiveMode, 0, 1);
            pMainContext->m_pActiveMode = 0;
        }
        DestroyPrimaryContextActiveScreen(pMainContext->m_pPrimaryContext);
    }
}

// FUNCTION: LEMBALL 0x004078F0
void PresentMainGameFrame(void *pMainGameContext) {
    GAME_MainContext *pMainContext;
    int fTimedStatus;
    DWORD dwNow;

    pMainContext = (GAME_MainContext *)pMainGameContext;
    fTimedStatus = 0;
    if ((pMainContext->m_nActiveMode == 5 || pMainContext->m_nActiveMode == 0x13) &&
        0x32 < pMainContext->m_nModeTicks) {
        fTimedStatus = 1;
        dwNow = timeGetTime();
        *(DWORD *)((char *)pMainContext->m_pRefreshingStatus + 0x20) = dwNow;
        *(int *)((char *)pMainContext->m_pRefreshingStatus + 0x24) = 1;
    }

    PresentPrimaryContextScreen(pMainContext->m_pPrimaryContext);

    if (fTimedStatus != 0 && *(int *)((char *)pMainContext->m_pRefreshingStatus + 0x24) != 0) {
        dwNow = timeGetTime();
        ((void (*)(DWORD))(*(void ***)pMainContext->m_pRefreshingStatus)[1])(
            dwNow - *(DWORD *)((char *)pMainContext->m_pRefreshingStatus + 0x20));
        *(int *)((char *)pMainContext->m_pRefreshingStatus + 0x24) = 0;
    }
}

// FUNCTION: LEMBALL 0x00407950
void RunMainGameLoop(GAME_MainContext *pMainContext) {
    if (g_pLevelDemoPlaybackController != 0 && g_fLevelDemoModeEnabled == 0) {
        *(int *)((char *)g_pLevelDemoPlaybackController + 0x28) = 0x14;
        *(int *)((char *)g_pLevelDemoPlaybackController + 0x2c) = 0x14;
        *(int *)((char *)g_pLevelDemoPlaybackController + 0x30) = 8;
    }

    for (;;) {
        if (pMainContext->m_fQuitRequested != 0) {
            return;
        }

        if (pMainContext->m_nActiveMode == 5 || pMainContext->m_nActiveMode == 0x13) {
            pMainContext->m_nModeTicks = pMainContext->m_nModeTicks + 1;
        }

        if (g_pLevelDemoPlaybackController != 0) {
            ServiceLevelDemoPlaybackThunk(g_pLevelDemoPlaybackController);
        }

        if (!PumpMessagesAndRunFrame()) {
            UpdateMainGameActiveMode(pMainContext);
            if (pMainContext->m_fQuitRequested != 0) {
                return;
            }
            PresentMainGameFrame(pMainContext);
        } else {
            pMainContext->m_fQuitRequested = 1;
        }
    }
}

// FUNCTION: LEMBALL 0x00406DF0
GAME_MainContext *InitializeMainGameContext(GAME_MainContext *pMainContext, const char *pszCmdLine) {
    (void)pszCmdLine;
    (void)GetSrcDiskRegistryValueBuffer;
    (void)g_GAME_NotInstalledTitle;
    (void)g_GAME_InstallPrompt;

    if (pMainContext == 0) {
        return 0;
    }

    *pMainContext = GAME_MainContext();
    g_pLevelProgressState = 0;
    g_pMainResourceArchive = 0;
    g_pVariantResourceEntryManager = 0;
    pMainContext->m_fQuitRequested = 1;
    pMainContext->m_pActiveMode = 0;
    SetVisualSciencesRegistryRunningState(g_GAME_WindowTitle, 1);
    if (FindCdromFilePathBySuffix(g_GAME_VsmemDllName) == 0) {
        ShowStartupMessage(g_GAME_CdromErrorTitle, g_GAME_CdromErrorText);
        return pMainContext;
    }

    g_pLevelProgressState = AllocateVSMemBlock(0x50);
    if (g_pLevelProgressState != 0) {
        g_pLevelProgressState = ClearLevelProgressPasswordState(g_pLevelProgressState);
    }

    pMainContext->m_pProcessingStatus = AllocateNamedStatusEntry(g_GAME_ProcessingName);
    pMainContext->m_pRefreshingStatus = AllocateNamedStatusEntry(g_GAME_RefreshingName);
    AppendStatusEntryToRegistry(g_pStatusEntryRegistry, pMainContext->m_pProcessingStatus);
    AppendStatusEntryToRegistry(g_pStatusEntryRegistry, pMainContext->m_pRefreshingStatus);

    g_pMainResourceArchive = AllocateResourceArchiveMemory(0x28);
    if (g_pMainResourceArchive != 0) {
        g_pMainResourceArchive = ConstructResourceArchive(g_pMainResourceArchive, g_GAME_MainArchiveName, 0x177000);
    }
    if (!ValidateResourceFileSignature()) {
        TriggerReleaseAssertFailure(g_GAME_IsValidResourceFileAssert, g_GAME_SourceFileName, 0x16e);
    }

    pMainContext->m_pPrimaryContext = AllocateVSMemBlock(0xe4);
    if (pMainContext->m_pPrimaryContext != 0) {
        pMainContext->m_pPrimaryContext =
            ConstructPrimaryContext((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext, pMainContext);
    }

    InitializeGlobalAudioManager(g_fMusicEnabled, g_fEffectsEnabled, 0x32, pMainContext->m_pPrimaryContext);
    if (g_fMusicEnabled != 0) {
        SetAudioManagerMusicEnabledFlag(g_pAudioManager, 1);
        SetAudioManagerStartupMusicName(g_pAudioManager, g_GAME_StartupMusicName);
    }

    g_pVariantResourceEntryManager = AllocateVSMemBlock(0x2cc);
    if (g_pVariantResourceEntryManager != 0) {
        g_pVariantResourceEntryManager = ConstructVariantResourceEntryManager(g_pVariantResourceEntryManager);
    }

    CopyCString(pMainContext->m_szCaption, sizeof(pMainContext->m_szCaption), g_GAME_WindowTitle);
    pMainContext->m_nModeTicks = 0;
    pMainContext->m_pActiveMode = 0;
    pMainContext->m_fQuitRequested = 0;
    pMainContext->m_nActiveMode = 1;
    pMainContext->m_pVariantMode = 0;
    SwitchMainGameMode(pMainContext, 1);

    return pMainContext;
}

// FUNCTION: LEMBALL 0x00407420
void SwitchMainGameMode(GAME_MainContext *pMainContext, int nMode) {
    void *pModeObject;

    CallNoArgVirtual(*(void **)((char *)pMainContext->m_pPrimaryContext + 0xd4), 4);

    if (pMainContext->m_pActiveMode != 0) {
        CallDeleteVirtual(pMainContext->m_pActiveMode, 0, 1);
        pMainContext->m_pActiveMode = 0;
    }

    if (nMode == 1 && g_fSkipStartupSequence == 1) {
        nMode = 2;
    }
    if (nMode == 0x12 && g_fSkipStartupSequence == 1) {
        nMode = 4;
    }

    switch (nMode) {
    case 1:
    case 0x12:
        ReleaseMainGameVariantResourceMode(pMainContext);
        pMainContext->m_nActiveMode = nMode;
        DestroyPrimaryContextActiveScreen(pMainContext->m_pPrimaryContext);
        pMainContext->m_pActiveMode =
            CreateRawBaseModeObject(0x28, pMainContext, g_pStartupModeVtable, g_pStartupModeRenderThunk);
        pMainContext->m_nModeTicks = 0;
        break;

    case 2:
        pMainContext->m_nActiveMode = nMode;
        DestroyPrimaryContextActiveScreen(pMainContext->m_pPrimaryContext);
        EnsureMainGameVariantResourceMode(pMainContext, 3);
        if (g_fLevelDemoModeEnabled != 0) {
            g_fLevelDemoModeEnabled = g_nStoredLevelDemoModeEnabled;
        }
        pMainContext->m_pActiveMode =
            CreateRawBaseModeObject(0x28, pMainContext, g_pMainMenuModeVtable, g_pMainMenuModeRenderThunk);
        break;

    case 3:
        pMainContext->m_nActiveMode = nMode;
        DestroyPrimaryContextActiveScreen(pMainContext->m_pPrimaryContext);
        pMainContext->m_pActiveMode =
            CreateRawBaseModeObject(0x28, pMainContext, g_pMode3Vtable, g_pMode3RenderThunk);
        break;

    case 4:
        pMainContext->m_nActiveMode = nMode;
        DestroyPrimaryContextActiveScreen(pMainContext->m_pPrimaryContext);
        pModeObject = AllocateVSMemBlock(0x28);
        pMainContext->m_pActiveMode =
            pModeObject == 0 ? 0 : ConstructLevelSelectionModeController(pModeObject, pMainContext);
        break;

    case 5:
        goto level_game_mode;

    case 10:
        pMainContext->m_nActiveMode = nMode;
        DestroyPrimaryContextActiveScreen(pMainContext->m_pPrimaryContext);
        pMainContext->m_pActiveMode = CreateSimpleModeObject(pMainContext);
        break;

    case 0x0c:
        pMainContext->m_nActiveMode = nMode;
        DestroyPrimaryContextActiveScreen(pMainContext->m_pPrimaryContext);
        pModeObject = AllocateVSMemBlock(0x38);
        pMainContext->m_pActiveMode =
            pModeObject == 0 ? 0 : ConstructNetworkLobbyTransportController(pModeObject, pMainContext);
        break;

    case 0x0e:
    case 0x0f:
        pMainContext->m_nActiveMode = nMode;
        DestroyPrimaryContextActiveScreen(pMainContext->m_pPrimaryContext);
        EnsureMainGameVariantResourceMode(pMainContext, 2);
        pModeObject = AllocateVSMemBlock(0x2c);
        if (pModeObject == 0) {
            pMainContext->m_pActiveMode = 0;
        } else {
            pMainContext->m_pActiveMode =
                ConstructPaintballSequenceModeController(pModeObject, pMainContext, nMode == 0x0e);
        }
        pMainContext->m_nModeTicks = 0;
        break;

    case 0x10:
        pMainContext->m_nActiveMode = nMode;
        DestroyPrimaryContextActiveScreen(pMainContext->m_pPrimaryContext);
        pMainContext->m_pActiveMode =
            CreateRawBaseModeObject(0x28, pMainContext, g_pMode16Vtable, g_pMode16RenderThunk);
        break;

    case 0x13:
        if (g_fLevelDemoModeEnabled == 0) {
            *(void **)((char *)g_pLevelDemoPlaybackController + 0x20) = 0;
        } else {
            *(void **)((char *)g_pLevelDemoPlaybackController + 0x20) = g_pLevelDemoPlaybackDescriptor;
        }
        g_fLevelDemoModeEnabled = 1;
level_game_mode:
        ReleaseMainGameVariantResourceMode(pMainContext);
        pMainContext->m_nActiveMode = nMode;
        DestroyPrimaryContextActiveScreen(pMainContext->m_pPrimaryContext);
        pModeObject = AllocateVSMemBlock(0x1f0);
        if (pModeObject != 0) {
            pModeObject = ConstructLevelGameMode(pModeObject, pMainContext);
            if (pModeObject != 0) {
                pMainContext->m_pActiveMode = (char *)pModeObject + 0x10;
                break;
            }
        }
        pMainContext->m_pActiveMode = 0;
        break;

    default:
        break;
    }

    SwitchPrimaryContextScreen(pMainContext->m_pPrimaryContext, pMainContext->m_nActiveMode);
}

// FUNCTION: LEMBALL 0x004071D0
void ShutdownMainGameContext(GAME_MainContext *pMainContext) {
    DWORD dwStartTime;

    if (pMainContext == 0) {
        return;
    }

    if (pMainContext->m_pVariantMode != 0) {
        DestroyMainGameVariantResourceMode(pMainContext->m_pVariantMode);
        FreeVSMemBlock(pMainContext->m_pVariantMode);
        pMainContext->m_pVariantMode = 0;
    }

    if (g_pNetworkLobbyVsnetRuntime != 0) {
        UnregisterNetworkLobbyVsnetRuntimeFromTransport(g_pNetworkLobbyVsnetRuntime);
    }
    if (g_pActiveNetworkRuntimeWindow != 0) {
        dwStartTime = timeGetTime();
        while (timeGetTime() - dwStartTime < 2000) {
            if (*(int *)((char *)g_pActiveNetworkRuntimeWindow + 0x48) == 0) {
                break;
            }
        }
    }
    if (g_pNetworkLobbyVsnetRuntime != 0) {
        void **pVsnetRuntimeVtable;

        pVsnetRuntimeVtable = *(void ***)g_pNetworkLobbyVsnetRuntime;
        ((void (*)(int))pVsnetRuntimeVtable[0])(1);
        g_pNetworkLobbyVsnetRuntime = 0;
    }

    if (pMainContext->m_pPrimaryContext != 0) {
        void *pPrimaryContext;
        void **pPrimaryContextVtable;

        pPrimaryContext = pMainContext->m_pPrimaryContext;
        pPrimaryContextVtable = *(void ***)pPrimaryContext;
        ((void (*)())pPrimaryContextVtable[0x74 / 4])();
        if (pMainContext->m_pPrimaryContext != 0) {
            void **pRenderQueueNodeVtable;

            pRenderQueueNodeVtable = *(void ***)((char *)pPrimaryContext + 0x90);
            ((void (*)(int))pRenderQueueNodeVtable[1])(1);
        }
    }
    if (pMainContext->m_pActiveMode != 0) {
        void **pActiveModeVtable;

        pActiveModeVtable = *(void ***)pMainContext->m_pActiveMode;
        ((void (*)(int))pActiveModeVtable[0])(1);
        pMainContext->m_pActiveMode = 0;
    }

    if (g_pVariantResourceEntryManager != 0) {
        ShutdownVariantResourceEntryManager(g_pVariantResourceEntryManager);
        FreeVSMemBlock(g_pVariantResourceEntryManager);
        ShutdownGlobalAudioManager();
        g_pVariantResourceEntryManager = 0;
    }
    if (g_pMainResourceArchive != 0) {
        DestroyResourceArchive(g_pMainResourceArchive);
        FreeResourceArchiveMemory(g_pMainResourceArchive);
        g_pMainResourceArchive = 0;
    }
    if (g_pLevelProgressState != 0) {
        FreeVSMemBlock(g_pLevelProgressState);
        g_pLevelProgressState = 0;
    }

    if (pMainContext->m_pRefreshingStatus != 0) {
        FreeVSMemBlock(pMainContext->m_pRefreshingStatus);
        pMainContext->m_pRefreshingStatus = 0;
    }

    if (pMainContext->m_pProcessingStatus != 0) {
        FreeVSMemBlock(pMainContext->m_pProcessingStatus);
        pMainContext->m_pProcessingStatus = 0;
    }

    SetVisualSciencesRegistryRunningState(g_GAME_WindowTitle, 0);
}

// FUNCTION: LEMBALL 0x00406310
int RunMainGameSession(int cArgs, const char *const *ppszArgs) {
    GAME_MainContext *pMainContext;
    int *pTrigTableBuffer;

    pTrigTableBuffer = (int *)AllocateVSMemBlock(0x800);
    if (pTrigTableBuffer == 0) {
        g_pSignedTrigTable = 0;
    } else {
        g_pSignedTrigTable = InitializeSignedTrigTable(pTrigTableBuffer);
    }

    g_pSessionRandomState = AllocateVSMemBlock(4);
    if (g_pSessionRandomState == 0) {
        g_pSessionRandomState = 0;
    } else {
        *(unsigned int *)g_pSessionRandomState = 0xad28;
    }

    CreateFrameTimerController(0x19000);
    InitializeStartupSwitchDefaults();
    if (ApplyStartupCommandLineSwitches(cArgs, ppszArgs) == 1) {
        pMainContext = (GAME_MainContext *)AllocateVSMemBlock(0x70);
        if (pMainContext == 0) {
            pMainContext = 0;
        } else {
            pMainContext = InitializeMainGameContext(pMainContext, 0);
        }

        if (g_fStartupEditLevelOverride != 0) {
            CopyCStringUnchecked(pMainContext->m_szCaption, (const char *)g_abOverrideLevelFilePathBuffer);
        }
        if (g_fStartupPlayLevelOverride != 0) {
            CopyCStringUnchecked(pMainContext->m_szCaption, (const char *)g_abOverrideLevelFilePathBuffer);
        }

        RunMainGameLoop(pMainContext);
        if (pMainContext != 0) {
            ShutdownMainGameContext(pMainContext);
            FreeVSMemBlock(pMainContext);
        }
    }

    DestroyFrameTimerController();
    FreeVSMemBlock(g_pSessionRandomState);
    FreeVSMemBlock(g_pSignedTrigTable);
    AppendStatusCString(g_GAME_ClosedDownText);
    return 0;
}
