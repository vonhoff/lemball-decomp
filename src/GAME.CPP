#include "GAME.H"
#include "LEVEL/LEVELRUN.H"
#include "ENGINE/GDI/VSWINDOW.H"
#include "FRONTEND/CURSOR.H"
#include "LEVEL/TYPE18.H"
#include "FRONTEND/MAINMENU.H"
#include "ENGINE/CORE/SAFEVT.H"

#include "SHELL/SHELLUI.H"
#include "SHELL/OPTIONS.H"
#include "ENGINE/MEDIA/VSSOUND.H"
#include "LEVEL/DEMO.H"
#include "ENGINE/GDI/VSGDI.H"
#include "RESOURCE/MOGLOAD.H"
#include "ENGINE/CORE/VSMATH.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/VSINIT.H"

#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif
#include <stdio.h>
#include <string.h>

extern "C" BOOL WINAPI SetCurrentDirectoryA(LPCSTR lpPathName);

static void *g_pSignedTrigTable = 0;
void *g_pSessionRandomState = 0;
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
static const char g_GAME_StartingDemoModePrefix[] = "Starting demo mode for level ";
static const char g_GAME_OnSkillText[] = " on skill ";
static const char g_GAME_LineBreak[] = "\n";
static const char g_GAME_DefaultLevelDirectory[] = "level\\";
static const char g_GAME_DefaultLevelFileName[] = "testlvl.lvl";
static char g_GAME_ResourceSignatureDecodeBuffer[] = "01234567890123456789";
static const char g_GAME_ResourceSignatureKey[] = "John Ketley is a Weatherman, and so is Michael Fish";
static const char g_GAME_ResourceSignatureExpected[] = "Master v1.3";
static const unsigned int g_GAME_LevelPasswordPermutation[8] = {2, 0, 7, 4, 6, 1, 5, 3};
// GLOBAL: LEMBALL 0x004a818c
int g_fRootHelperGeometryDispatchSuppressed = 0;

struct GameResourceSignatureStringObject {
    virtual void Slot00(void) = 0;
    virtual void Slot04(void) = 0;
    virtual void Slot08(void) = 0;
    virtual void Slot0C(void) = 0;
    virtual void Slot10(void) = 0;
    virtual void Slot14(void) = 0;
    virtual void Slot18(void) = 0;
    virtual void PrepareStringResource(void) = 0;
    int m_nReserved04;
    int m_nLockCount08;
    int m_cReferences;
    int m_nLoadState10;
    int m_nReserved14;
    int m_nReserved18;
    int m_nReserved1C;
    int m_nReserved20;
    int m_nReserved24;
    int m_cbResourceData28;
    int m_lResourceOffset2C;
    int m_nResourceId30;
    unsigned int *m_padwResourceDescriptor34;
    char *m_pszText38;
};

extern void LEMBALL_FASTCALL ServiceNetworkLobbySelectedPeerUpdates(void *pVsnetRuntime);
extern int __stdcall PumpMessagesAndRunFrame(void);
extern int __stdcall ValidateResourceFileSignatureThunk(void);
extern void TriggerReleaseAssertFailure(const char *pszExpression, const char *pszFile, int nLine);
extern void *LEMBALL_FASTCALL ConstructVariantResourceEntryManagerThunk(void *pManager);
extern void LEMBALL_FASTCALL DestroyMainGameVariantResourceMode(void *pVariantMode);
extern void LEMBALL_FASTCALL UnregisterNetworkLobbyVsnetRuntimeFromTransport(void *pVsnetRuntime);
extern void LEMBALL_FASTCALL ShutdownVariantResourceEntryManager(void *pManager);
extern int g_fMusicEnabled;
extern int g_fEffectsEnabled;
  extern void *LEMBALL_FASTCALL ConstructWindowOwnerRenderContext(void *pPrimaryContext);
  extern int RegisterVSBaseWindowClass(void);
extern void *LEMBALL_FASTCALL InitializePackagedSpriteRenderEntryThunk(void *pObject);
extern void *LEMBALL_FASTCALL InitializeRenderPointRectSinkEntryThunk(void *pObject);
extern void *LEMBALL_FASTCALL InitializeStatusIndicatorPointSinkEntryThunk(void *pObject);
extern void LEMBALL_FASTCALL BuildGeometryHelperFromRenderRect(void *pOwner);
extern unsigned int __stdcall ComputePrimaryContextWindowStyleFlags(void);
extern int __stdcall GetPrimaryContextMenuDefinition(unsigned int *pMenuId,
                                                     void **ppMenuDefinition);
extern void LEMBALL_FASTCALL PrepareLevelScreenRootHelperForRuntime(
    GamePrimaryContext *pPrimaryContext);
extern void *LoadZrleResource(int nResourceId);
extern void *LoadPalResource(int nResourceId);
extern void SetLevelScreenStatusIndicatorMode(int nMode, int nValue);
typedef void (LEMBALL_FASTCALL *GamePrimaryContextSetRectProc)(
    void *pPrimaryContext, int nUnused, short *paRect, void *pWindowOwner,
    const char *pszTitle);
extern void *g_pPrimaryContextVtable;
extern void *g_pPrimaryContextRenderQueueNodeVtable;
extern void *g_pQueuedRenderPointSinkFinalizeThunk;
extern void *g_pSharedRenderDispatchQueue;
extern void *g_pSharedGeometryHelper;
extern void *g_pEffTransportSecondaryDispatchQueue;
extern int *g_pArrowCursorStatusIndicatorRenderClient;
extern int DrainRenderDispatchQueueEntries(void *pDispatchQueue, unsigned int cEntries);
extern void LEMBALL_FASTCALL DispatchAndClearPointerQueue(void *pQueue);
extern "C" DWORD WINAPI timeGetTime(void);
extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);

struct GameScreenObject {
    void *ConstructIntroSequenceScreen(void *pPrimaryContext,
                                       void *pWindowOwnerContext,
                                       short *paRect,
                                       int fSkipVariant);
    void *ConstructMainMenuScreen(void *pPrimaryContext,
                                  void *pWindowOwnerContext,
                                  short *paRect);
    void *ConstructOptionsScreen(void *pPrimaryContext,
                                 void *pWindowOwnerContext,
                                 short *paRect);
    void *ConstructLevelSelectionScreen(void *pPrimaryContext,
                                        void *pWindowOwnerContext,
                                        short *paRect);
    void *ConstructLevelScreen(void *pPrimaryContext,
                               void *pLevelModeObject,
                               void *pWindowOwnerContext,
                               int nStatusIndicatorMode,
                               short *paRect);
    void *ConstructRegistrationInfoScreen(void *pPrimaryContext,
                                          void *pWindowOwnerContext,
                                          short *paRect);
    void *ConstructNetworkLobbyScreen(void *pPrimaryContext,
                                      void *pWindowOwnerContext,
                                      short *paRect);
    void *ConstructPaintballSequenceScreen(void *pPrimaryContext,
                                           void *pWindowOwnerContext,
                                           short *paRect,
                                           int fFirstSequenceVariant);
    void *ConstructPasswordEntryScreen(void *pPrimaryContext,
                                       void *pWindowOwnerContext,
                                       short *paRect);
};

struct GameLevelProgressState {
    int m_nCurrentLevel;
    int m_nUnused04;
    int m_nCurrentPack;
    int m_nUnused0C;
    char m_szPassword[0x10];
    int m_nUnused20;
    int m_nUnused24;
    int m_anUnlockedPackCaps[5];
    int m_anSnapshotPackCaps[4];
    int m_nUnused4C;

    bool CanRetreat(void);
    GameLevelProgressState *Clear(void);
    unsigned int EncodePasswordValue(void);
    void ApplyPasswordValue(unsigned int uValue);
    void ApplyPasswordValueThunk(unsigned int uValue);
    unsigned int ComputeLevelPasswordChecksum(unsigned int uValue);
    unsigned int ComputeLevelPasswordChecksumThunk(unsigned int uValue);
    char *FormatPassword(void);
    int ValidateAndApplyPassword(char *pszPassword);
    int ParsePasswordDigits(void);
    int ParsePasswordDigitsThunk(void);
    void Snapshot(void);
    void SetUnlockedPackCap(int nPack, int nCap);
    void SetUnlockedPackCapThunk(int nPack, int nCap);
};

struct GameBaseModeObject {
    void **m_pVtable;
    int m_nRequestedMode;
    int m_nStatus;
    void **m_pRenderQueueNodeVtable;
    int m_nQueueMagic;
    int m_nQueueClientCount;
    int m_nQueueReserved;
    int m_fNetworkLobbyActive;
    void *m_pTransportClient;
    GameMainContext *m_pMainContext;
};

struct GameLevelSelectionModeController : public GameBaseModeObject {
};

struct GameNetworkLobbyTransportController : public GameBaseModeObject {
    int m_nReserved28;
    int m_nReserved2C;
    void *m_pPeerClearCloseStream;
    void *m_pPeerDirtyConfirmStream;
};

struct GamePaintballSequenceModeController : public GameBaseModeObject {
    int m_fFirstSequenceVariant;
};

struct GameLevelGameMode {
    int m_anHeaderDwords[4];
    void **m_pDeleteThunkVtable;
    void *m_pReserved14;
    void *m_pReserved18;
    void **m_pEffStreamVtable;
    void *m_pReserved20;
    char m_abUnknown24[0xf4];
    int m_nStatusIndicatorMode;
    int m_nReserved114;
    int m_nManagedEntityCount;
    int m_nManagedEntityCapacity;
    void *m_pManagedEntityTable;
    int m_nReserved124;
    int m_nReserved128;
    int m_nReserved12C;
    int m_nReserved134;
    int m_nReserved138;
    int m_nReserved13C;
    int m_nReserved140;
    int m_nReserved144;
    int m_nReserved148;
    char m_abUnknown14C[8];
    GameMainContext *m_pMainContext;
};

struct GameRenderQueueNode {
    void **m_pVtable;
    int m_uMagic;
    void *m_pReserved08;
    void *m_pReserved0C;
};

struct GameRenderDispatchClientNode {
    void *m_pClient;
    int m_nOrder;
    GameRenderDispatchClientNode *m_pNext;
};

static void StubNoOpVoid(void) {
}

struct GameModeDeleteVtableModel {
    virtual void Delete(int nDeleteFlag);
};
static GameModeDeleteVtableModel g_GAME_ModeDeleteVtableModel;
static void *g_pGAME_ModeDeleteVtableSlot =
    (*(void ***)&g_GAME_ModeDeleteVtableModel)[0];

void GameModeDeleteVtableModel::Delete(int nDeleteFlag) {
    (void)nDeleteFlag;
}

static int StubReturnZero(void) {
    return 0;
}

static void StubNoOpPtr(void *) {
}

static void LEMBALL_FASTCALL StubNoOpObjectArgument(
    void *, int, void *) {
}

/* The primary-context present path pushes one argument and the target
 * implementation returns with RET 4.  Keep that ABI even while the body is
 * still being reconstructed. */
static void __stdcall StubPrimaryContextSetPresentMode(int nMode) {
    (void)nMode;
}

static void *GetPrimaryContextFlushWindowOwnerDirtyRectAddress(void) {
    void (GamePrimaryContext::*pMethod)(int) =
        &GamePrimaryContext::FlushWindowOwnerDirtyRect;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextUpdateWindowOwnerRenderContextAddress(void) {
    void (GamePrimaryContext::*pMethod)(void) =
        &GamePrimaryContext::UpdateWindowOwnerRenderContext;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextSetWindowOwnerStateAddress(void) {
    void (GamePrimaryContext::*pMethod)(int) =
        &GamePrimaryContext::SetWindowOwnerState;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextGetWindowOwnerStateAddress(void) {
    int (GamePrimaryContext::*pMethod)(void) =
        &GamePrimaryContext::GetWindowOwnerState;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextDispatchActiveScreenFrameAddress(void) {
    void (GamePrimaryContext::*pMethod)(void *) =
        &GamePrimaryContext::DispatchPrimaryContextActiveScreenFrame;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextInitializeWindowOwnerFromRectAndActivateAddress(void) {
    void (GamePrimaryContext::*pMethod)(short *, void *, const char *, int) =
        &GamePrimaryContext::InitializeWindowOwnerFromRectAndActivate;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextApplyWindowOwnerPaletteResourceAddress(void) {
    void (GamePrimaryContext::*pMethod)(int) =
        &GamePrimaryContext::ApplyWindowOwnerPaletteResource;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextAppendQueuedRenderSinkValueNodeAddress(void) {
    void (GamePrimaryContext::*pMethod)(void *) =
        &GamePrimaryContext::AppendQueuedRenderSinkValueNode;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextMarkNestedContextDirtyAddress(void) {
    void (GamePrimaryContext::*pMethod)(void) =
        &GamePrimaryContext::MarkNestedContextDirtyIfField20;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextDispatchWindowOwnerRectInitializationAddress(void) {
    void (GamePrimaryContext::*pMethod)(short *, void *, const char *) =
        &GamePrimaryContext::DispatchWindowOwnerRectInitialization;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextRefreshWindowOwnerMenuStateAddress(void) {
    void (GamePrimaryContext::*pMethod)(void) =
        &GamePrimaryContext::RefreshWindowOwnerMenuState;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextSetWindowOwnerMenuDefinitionAddress(void) {
    void (GamePrimaryContext::*pMethod)(const unsigned int *, void *) =
        &GamePrimaryContext::SetWindowOwnerMenuDefinition;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextEnsureRootGeometryDispatchHelperAddress(void) {
    void (GamePrimaryContext::*pMethod)(void) =
        &GamePrimaryContext::EnsureRootGeometryDispatchHelper;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextOffsetRootGeometryPositionAddress(void) {
    void (GamePrimaryContext::*pMethod)(unsigned int) =
        &GamePrimaryContext::OffsetRootGeometryPosition;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextSetRootGeometryRectAddress(void) {
    void (GamePrimaryContext::*pMethod)(const VS_XYPair *) =
        &GamePrimaryContext::SetRootGeometryRect;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextFinalizeRootGeometryRectAddress(void) {
    void (GamePrimaryContext::*pMethod)(const VS_XYPair *) =
        &GamePrimaryContext::FinalizeRootGeometryRect;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextReservedWindowOwnerSlot19Address(void) {
    void (GamePrimaryContext::*pMethod)(void) =
        &GamePrimaryContext::ReservedWindowOwnerSlot19;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextReservedWindowOwnerSlot20Address(void) {
    void (GamePrimaryContext::*pMethod)(void) =
        &GamePrimaryContext::ReservedWindowOwnerSlot20;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextIsCurrentWindowOwnerContextAddress(void) {
    int (GamePrimaryContext::*pMethod)(void) =
        &GamePrimaryContext::IsCurrentWindowOwnerContext;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextDestroyWindowOwnerHwndAddress(void) {
    void (GamePrimaryContext::*pMethod)(void) =
        &GamePrimaryContext::DestroyWindowOwnerHwnd;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextRemoveQueuedRenderSinkValueNodeAddress(void) {
    void (GamePrimaryContext::*pMethod)(void *) =
        &GamePrimaryContext::RemoveQueuedRenderSinkValueNode;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextReservedWindowOwnerSlot34Address(void) {
    void (GamePrimaryContext::*pMethod)(int) =
        &GamePrimaryContext::ReservedWindowOwnerSlot34;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextPropagateWindowOwnerPositionAddress(void) {
    void (GamePrimaryContext::*pMethod)(const VS_XYPair *) =
        &GamePrimaryContext::PropagateWindowOwnerPosition;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextCacheCurrentContextWindowAddress(void) {
    void (GamePrimaryContext::*pMethod)(void) =
        &GamePrimaryContext::CacheCurrentContextWindow;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextReservedWindowOwnerSlot37Address(void) {
    void (GamePrimaryContext::*pMethod)(void) =
        &GamePrimaryContext::ReservedWindowOwnerSlot37;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextReservedWindowOwnerSlot38Address(void) {
    void (GamePrimaryContext::*pMethod)(void) =
        &GamePrimaryContext::ReservedWindowOwnerSlot38;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextReservedWindowOwnerSlot39Address(void) {
    void (GamePrimaryContext::*pMethod)(void) =
        &GamePrimaryContext::ReservedWindowOwnerSlot39;
    return *(void **)&pMethod;
}

void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x10(void *pPrimaryContext);
void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x14(void *pPrimaryContext);
void LEMBALL_FASTCALL DestroyPrimaryContextActiveScreenForVtable(
    void *pPrimaryContext);
void LEMBALL_FASTCALL DispatchPrimaryContextSlot23Thunk(
    void *pPrimaryContext);
void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x0C(
    void *pPrimaryContext, int nUnused, int nIgnored);
void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x30(
    void *pPrimaryContext);

static void *g_GAME_PrimaryContextVtableSlots[46];
// GLOBAL: LEMBALL 0x00493110
static void *g_GAME_RenderQueueNodeVtableSlots[4];
static void *g_GAME_GenericModeVtableSlots[6];
static void *g_GAME_GenericScreenVtableSlots[14];
struct GameSafeVtableInitializer {
    GameSafeVtableInitializer(void) {
        int i;
        for (i = 0; i < 46; ++i)
            g_GAME_PrimaryContextVtableSlots[i] = (void *)NetworkSafeVtableNoop;
        for (i = 0; i < 4; ++i)
            g_GAME_RenderQueueNodeVtableSlots[i] = (void *)NetworkSafeVtableNoop;
        for (i = 0; i < 2; ++i) {
            g_GAME_GenericModeVtableSlots[i] = (void *)NetworkSafeVtableNoop;
        }
        for (i = 0; i < 14; ++i)
            g_GAME_GenericScreenVtableSlots[i] = (void *)NetworkSafeVtableNoop;
    }
};
static GameSafeVtableInitializer g_GAME_SafeVtableInitializer;
// GLOBAL: LEMBALL 0x004a011c
int g_nSelectedNetworkLobbyPeerId = 0;
// GLOBAL: LEMBALL 0x0049f140
void *g_pActiveNetworkLobbyTransportController = 0;
void *g_pNetworkLobbyTransportController = 0;
// GLOBAL: LEMBALL 0x0049f144
void *g_pActiveNetworkLobbyScreen = 0;

extern void *ConstructEffStreamPayloadSize8(void *pObject);
struct GameEffTransportRuntimeWindow {
    void RegisterEffTransportEventClient(void *pClient);
    void UnregisterEffTransportEventClient(void);
};
extern void *ConstructNetworkLobbyPeerClearCloseStream(void *pObject);
extern void *ConstructNetworkLobbyPeerDirtyConfirmStream(void *pObject);
struct GameNetworkLevelChunkDeltaStream {
    void *ConstructNetworkLevelChunkDeltaStream(int nOwner);
    void *ConstructNetworkLevelChunkDeltaStreamEntry(int nOwner);
};
extern void *ConstructMainGameVariantResourceBundle(void *pBundle, void *pPrimaryContext, unsigned short nVariantMode);
extern void InitializeManagedEntitySlotTablesEntryThunk(void *pLevelGameMode);
extern void ResetLevelFrameClockEntryThunk(void);
extern void *ConstructLevelGameStateStreamThunk(void *pObject);
extern void *LEMBALL_FASTCALL ConstructLevelTileGridThunk(void *pObject);
extern void InitializeLevelTileGridThunk(void *pTileGrid);
struct LevelTileReachabilityHelper {
    void *ConstructLevelTileReachabilityHelperThunk(void *pTileGrid);
};
extern void ClearPrimaryLockedRecordTablePayloadFlags(void *pPayload);
extern void ClearSecondaryLockedRecordTablePayloadFlags(void *pPayload);
extern void *ConstructLevelChunkStreamDispatcherThunk(void *pObject, int nChunkTypeCount);
extern void ResetRegisteredLevelChunkStreamsThunk(void *pDispatcher);
extern void RegisterLevelChunkStreamThunk(void *pDispatcher, void *pStream);
extern void *ConstructLiftChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructMoveChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructGmobChunkManagerThunk(void *pObject, void *pLevelMode);
extern void *ConstructPlasChunkManagerThunk(void *pObject, void *pLevelMode, void *pGmobManager, void *pProjectileGeometryTable);
extern void *ConstructProjectilePoolThunk(void *pObject);
extern void *ConstructManagedEntityQueueCursorThunk(void *pObject, int nLevelMode, int nPrimaryCapacity, int nSecondaryCapacity);
extern void *ConstructShpgChunkManagerThunk(void *pObject, void *pLevelMode, void *pGmobManager, void *pProjectileGeometryTable);
extern void *ConstructEnmyChunkManagerThunk(void *pObject, void *pLevelMode, void *pGmobManager, void *pProjectileGeometryTable);
extern void LEMBALL_FASTCALL ResetBallChunkEntriesThunk(void *pBallManager);
extern void *ConstructCollChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructMineChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructDoorChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructRockChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructCaptureTriggerChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructLasrChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ReturnArgumentVtableCallback(void *pObject);
extern void *ConstructBoonChunkManagerThunk(void *pObject, void *pLevelMode, void *pTileGrid);
extern void LEMBALL_FASTCALL ResetBoonChunkManagerObjectsThunk(void *pObject);
extern void *ConstructTramChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructPgunChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructIceChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructInvsChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern int LEMBALL_FASTCALL GetSelectedLevelNumberThunk(void *pLevelProgressState);
extern void ReadLevelDemoLengthPrefixedRecordThunk(void *pPlaybackController, void *pRecordHeader, unsigned int *pcbRecord);
extern void LoadLevelChunksForSkillAndLevelThunk(void *pLoaderContext, unsigned int nSkill, unsigned int nLevel, int fReuseState);
extern void InitializeNetworkPlayerPlasChunkObjectPoolThunk(void *pPlasManager);
extern void AssignNetworkPlayerManagedEntitySlotIdsThunk(int nLevelMode);
extern void LEMBALL_FASTCALL ResizeLevelTileReachabilityGridThunk(void *pReachabilityHelper);
extern void LEMBALL_FASTCALL FillReachabilityGridFromTileFlagsThunk(void *pReachabilityHelper);
extern void AssignMissingManagedEntitySlotIdsThunk(int nLevelMode);
extern void InitializeLevelChunkTypeEnabledFlagsThunk(void *pLevelMode);
struct LevelGameMode {
    void InitializeLevelGameMode(void);
};

// GLOBAL: LEMBALL 0x004A1BD0
void *g_pStatusEntryRegistry = 0;
// GLOBAL: LEMBALL 0x0049cb68
void *g_pLevelProgressState = 0;
// GLOBAL: LEMBALL 0x004a1d58
void *g_pMainResourceArchive = 0;
// GLOBAL: LEMBALL 0x0049eb80
void *g_pVariantResourceEntryManager = 0;
void *g_pNetworkLobbyVsnetRuntime = 0;
// GLOBAL: LEMBALL 0x004a1e18
void *g_pActiveNetworkRuntimeWindow = 0;
// GLOBAL: LEMBALL 0x0049ce04
int g_nLevelFrameClockTick = 0;
// GLOBAL: LEMBALL 0x0049ce28
int g_fLevelFrameClockPaused = 0;
static void *g_GAME_StartupModeVtable[6];
static void *g_GAME_StartupModeEventClientVtable[4];
static void *g_GAME_MainMenuModeVtable[6];
static void *g_GAME_MainMenuModeEventClientVtable[4];
void *g_pStartupModeVtable = g_GAME_StartupModeVtable;
void *g_pStartupModeRenderThunk = g_GAME_StartupModeEventClientVtable;
void *g_pMainMenuModeVtable = g_GAME_MainMenuModeVtable;
void *g_pMainMenuModeRenderThunk = g_GAME_MainMenuModeEventClientVtable;
void *g_pMode3Vtable = g_GAME_GenericModeVtableSlots;
void *g_pMode3RenderThunk = (void *)StubNoOpPtr;
void *g_pSimpleModeDeleteVtable = g_GAME_GenericModeVtableSlots;
void *g_pSimpleModeVtable = g_GAME_GenericModeVtableSlots;
void *g_pMode16Vtable = g_GAME_GenericModeVtableSlots;
void *g_pMode16RenderThunk = (void *)StubNoOpPtr;
void *g_pPrimaryContextVtable = g_GAME_PrimaryContextVtableSlots;
void *g_pPrimaryContextRenderQueueNodeVtable = g_GAME_RenderQueueNodeVtableSlots;
extern void *g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage[8];
extern void LEMBALL_FASTCALL QueueQueuedRenderPointSinkThunk(
    void *pPointSink, void *pUnusedEdx, void *pQueue);
extern void __stdcall PromoteQueuedRenderPointSinkTargetStateThunk(
    void *pPointSink);

// FUNCTION: LEMBALL 0x00432320
void *LEMBALL_FASTCALL DeleteQueuedRenderPointSinkFinalize(
    void *pObject, int, int fDelete)
{
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00401FB4
void *LEMBALL_FASTCALL DeleteQueuedRenderPointSinkFinalizeThunk(
    void *pObject, int, int fDelete) {
    return DeleteQueuedRenderPointSinkFinalize(pObject, 0, fDelete);
}

// GLOBAL: LEMBALL 0x00496C98
static void *g_GAME_QueuedRenderPointSinkFinalizeVtable[4] = {
    (void *)DeleteQueuedRenderPointSinkFinalizeThunk,
    (void *)QueueQueuedRenderPointSinkThunk,
    (void *)PromoteQueuedRenderPointSinkTargetStateThunk,
    0,
};
void *g_pQueuedRenderPointSinkFinalizeThunk =
    g_GAME_QueuedRenderPointSinkFinalizeVtable;
// GLOBAL: LEMBALL 0x004a9360
void *g_pSharedRenderDispatchQueue = 0;
// GLOBAL: LEMBALL 0x004a279c
void *g_pSharedGeometryHelper = 0;
void *g_pActiveLevelGameMode = 0;
void *g_pLevelTileGrid = 0;
void *g_pLevelTileReachabilityHelper = 0;
void *g_pLevelChunkStreamDispatcher = 0;
unsigned short g_nGameAppState0 = 0;
unsigned short g_nGameAppState1 = 0;
int g_nGameAppState2 = 0;

extern void *g_pNonZrleVariantRenderEntryInitializeVtable[2];
extern void *DeleteBaseModeObjectAdjustedThunk(void *pObject, BYTE fDelete);
extern int LEMBALL_FASTCALL HandleBaseModeActionButtonEventThunk(
    void *pClient, int nUnused, void *pEvent);
extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);

// FUNCTION: LEMBALL 0x004467D0
void LEMBALL_FASTCALL DestroyBaseModeObject(void *pObject) {
    void **pMode;

    pMode = (void **)pObject;
    pMode[0] = g_GAME_GenericModeVtableSlots;
    pMode[3] = g_GAME_StartupModeEventClientVtable;
    g_pActiveNetworkLobbyTransportController = 0;
    if (g_pActiveNetworkRuntimeWindow != 0) {
        ((GameEffTransportRuntimeWindow *)g_pActiveNetworkRuntimeWindow)
            ->UnregisterEffTransportEventClient();
    }
    if (pMode[8] != 0) {
        ((void (LEMBALL_FASTCALL *)(void *, int, int))(*(void ***)pMode[8])[5])(
            pMode[8], 0, 1);
    }
    pMode[3] = g_pNonZrleVariantRenderEntryInitializeVtable;
    pMode[0] = g_GAME_GenericModeVtableSlots;
}

// FUNCTION: LEMBALL 0x00407F40
void *LEMBALL_FASTCALL DeleteStartupMode(
    void *pObject, int, BYTE fDelete)
{
    DestroyBaseModeObject(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x004026F3
void *LEMBALL_FASTCALL DeleteStartupModeThunk(
    void *pObject, int, BYTE fDelete)
{
    return DeleteStartupMode(pObject, 0, fDelete);
}

// FUNCTION: LEMBALL 0x00407F70
void *LEMBALL_FASTCALL DeleteMainMenuMode(
    void *pObject, int, BYTE fDelete)
{
    DestroyBaseModeObject(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x004027A2
void *LEMBALL_FASTCALL DeleteMainMenuModeThunk(
    void *pObject, int, BYTE fDelete)
{
    return DeleteMainMenuMode(pObject, 0, fDelete);
}

// FUNCTION: LEMBALL 0x004465C0
void LEMBALL_FASTCALL RequestFramedScreenTransitionMode2(void *pScreen) {
    *(int *)((char *)pScreen + 0x378) = 1;
    *(int *)((char *)pScreen + 0x374) = 2;
}

// FUNCTION: LEMBALL 0x00403814
void LEMBALL_FASTCALL RequestFramedScreenTransitionMode2Thunk(void *pScreen) {
    RequestFramedScreenTransitionMode2(pScreen);
}

// FUNCTION: LEMBALL 0x00402D24
void LEMBALL_FASTCALL UpdateBaseModeThunk(int *pMode) {
    if (pMode[7] != 0 && g_nSelectedNetworkLobbyPeerId == 0 &&
        g_pActiveNetworkLobbyScreen != 0) {
        RequestFramedScreenTransitionMode2Thunk(g_pActiveNetworkLobbyScreen);
    }
    ((void (LEMBALL_FASTCALL *)(void *))(*(void ***)pMode)[3])(pMode);
}

// FUNCTION: LEMBALL 0x00401884
int LEMBALL_FASTCALL ReturnZeroBaseModeThunk(void *) {
    return 0;
}

// FUNCTION: LEMBALL 0x00403378
void LEMBALL_FASTCALL NoopBaseModeThunk(void *) {
}

// FUNCTION: LEMBALL 0x00403774
int LEMBALL_FASTCALL ReturnFalseBaseModeThunk(void *) {
    return 0;
}

// FUNCTION: LEMBALL 0x0040227F
void *LEMBALL_FASTCALL DeleteStartupModeEventClientThunk(
    void *pObject, int, BYTE fDelete)
{
    return DeleteStartupMode((char *)pObject - 0x0c, 0, fDelete);
}

// FUNCTION: LEMBALL 0x00402D29
void *LEMBALL_FASTCALL DeleteMainMenuModeEventClientThunk(
    void *pObject, int, BYTE fDelete)
{
    return DeleteMainMenuMode((char *)pObject - 0x0c, 0, fDelete);
}

static void InitializeGameStubVtables(void) {
    static int g_fInitialized = 0;

    if (g_fInitialized != 0) {
        return;
    }

    g_GAME_PrimaryContextVtableSlots[0] =
        GetPrimaryContextRefreshWindowOwnerMenuStateAddress();
    g_GAME_PrimaryContextVtableSlots[1] =
        GetPrimaryContextDispatchWindowOwnerRectInitializationAddress();
    g_GAME_PrimaryContextVtableSlots[2] =
        (void *)UpdateWindowOwnerRootGeometrySourceRect;
    g_GAME_PrimaryContextVtableSlots[3] =
        (void *)GetPrimaryContextMenuDefinition;
    g_GAME_PrimaryContextVtableSlots[4] =
        GetPrimaryContextSetWindowOwnerMenuDefinitionAddress();
    g_GAME_PrimaryContextVtableSlots[5] =
        GetPrimaryContextEnsureRootGeometryDispatchHelperAddress();
    g_GAME_PrimaryContextVtableSlots[6] =
        (void *)BuildGeometryHelperFromRenderRect;
    g_GAME_PrimaryContextVtableSlots[7] =
        (void *)ReleaseWindowOwnerRenderResources;
    g_GAME_PrimaryContextVtableSlots[8] =
        (void *)CanonicalizeWindowOwnerRenderSubrect;
    g_GAME_PrimaryContextVtableSlots[9] =
        GetPrimaryContextOffsetRootGeometryPositionAddress();
    g_GAME_PrimaryContextVtableSlots[10] =
        (void *)PopulateWindowOwnerRenderAnchor;
    g_GAME_PrimaryContextVtableSlots[11] =
        (void *)RefreshWindowOwnerRenderScale;
    g_GAME_PrimaryContextVtableSlots[12] =
        GetPrimaryContextSetRootGeometryRectAddress();
    g_GAME_PrimaryContextVtableSlots[13] =
        GetPrimaryContextFinalizeRootGeometryRectAddress();
    g_GAME_PrimaryContextVtableSlots[14] =
        (void *)SetWindowOwnerRenderAnchorFromPoint;
    g_GAME_PrimaryContextVtableSlots[15] =
        (void *)PrepareLevelScreenRootHelperForRuntime;
    g_GAME_PrimaryContextVtableSlots[16] =
        (void *)DestroyPrimaryContextActiveScreenForVtable;
    g_GAME_PrimaryContextVtableSlots[17] =
        (void *)ForwardPrimaryContextActiveScreenSlot0x10;
    g_GAME_PrimaryContextVtableSlots[18] =
        (void *)ForwardPrimaryContextActiveScreenSlot0x14;
    g_GAME_PrimaryContextVtableSlots[19] =
        GetPrimaryContextReservedWindowOwnerSlot19Address();
    g_GAME_PrimaryContextVtableSlots[20] =
        GetPrimaryContextReservedWindowOwnerSlot20Address();
    g_GAME_PrimaryContextVtableSlots[21] =
        (void *)DispatchPrimaryContextSlot23Thunk;
    g_GAME_PrimaryContextVtableSlots[22] =
        (void *)ForwardPrimaryContextActiveScreenSlot0x0C;
    g_GAME_PrimaryContextVtableSlots[23] =
        (void *)ForwardPrimaryContextActiveScreenSlot0x30;
    g_GAME_PrimaryContextVtableSlots[24] =
        GetPrimaryContextIsCurrentWindowOwnerContextAddress();
    g_GAME_PrimaryContextVtableSlots[25] =
        (void *)ComputePrimaryContextWindowStyleFlags;
    g_GAME_PrimaryContextVtableSlots[26] =
        GetPrimaryContextGetWindowOwnerStateAddress();
    g_GAME_PrimaryContextVtableSlots[27] =
        GetPrimaryContextSetWindowOwnerStateAddress();
    g_GAME_PrimaryContextVtableSlots[28] =
        GetPrimaryContextFlushWindowOwnerDirtyRectAddress();
    g_GAME_PrimaryContextVtableSlots[29] =
        GetPrimaryContextDestroyWindowOwnerHwndAddress();
    g_GAME_PrimaryContextVtableSlots[30] =
        GetPrimaryContextAppendQueuedRenderSinkValueNodeAddress();
    g_GAME_PrimaryContextVtableSlots[31] =
        GetPrimaryContextRemoveQueuedRenderSinkValueNodeAddress();
    g_GAME_PrimaryContextVtableSlots[32] =
        GetPrimaryContextMarkNestedContextDirtyAddress();
    g_GAME_PrimaryContextVtableSlots[33] =
        (void *)ForwardNestedGeometryRectUpdate;
    g_GAME_PrimaryContextVtableSlots[34] =
        GetPrimaryContextReservedWindowOwnerSlot34Address();
    g_GAME_PrimaryContextVtableSlots[35] =
        GetPrimaryContextPropagateWindowOwnerPositionAddress();
    g_GAME_PrimaryContextVtableSlots[36] =
        GetPrimaryContextCacheCurrentContextWindowAddress();
    g_GAME_PrimaryContextVtableSlots[37] =
        GetPrimaryContextReservedWindowOwnerSlot37Address();
    g_GAME_PrimaryContextVtableSlots[38] =
        GetPrimaryContextReservedWindowOwnerSlot38Address();
    g_GAME_PrimaryContextVtableSlots[39] =
        GetPrimaryContextReservedWindowOwnerSlot39Address();
    g_GAME_PrimaryContextVtableSlots[40] =
        (void *)DispatchWindowOwnerRenderContextMessage;
    g_GAME_PrimaryContextVtableSlots[41] =
        GetPrimaryContextInitializeWindowOwnerFromRectAndActivateAddress();
    g_GAME_PrimaryContextVtableSlots[42] =
        GetPrimaryContextDispatchActiveScreenFrameAddress();
    g_GAME_PrimaryContextVtableSlots[43] =
        GetPrimaryContextApplyWindowOwnerPaletteResourceAddress();
    g_GAME_PrimaryContextVtableSlots[44] =
        GetPrimaryContextUpdateWindowOwnerRenderContextAddress();
    g_GAME_PrimaryContextVtableSlots[45] =
        (void *)PresentWindowOwnerRenderContext;
    g_GAME_RenderQueueNodeVtableSlots[0] =
        (void *)InitializeNonZrleVariantRenderEntry;
    g_GAME_RenderQueueNodeVtableSlots[1] = g_pGAME_ModeDeleteVtableSlot;
    g_GAME_RenderQueueNodeVtableSlots[2] =
        (void *)HandlePrimaryContextMenuCommandThunk;
    g_GAME_RenderQueueNodeVtableSlots[3] = 0;

    g_GAME_GenericModeVtableSlots[0] = g_pGAME_ModeDeleteVtableSlot;
    g_GAME_GenericModeVtableSlots[1] = (void *)StubNoOpVoid;

    g_GAME_StartupModeVtable[0] = (void *)DeleteStartupModeThunk;
    g_GAME_StartupModeVtable[1] = (void *)UpdateBaseModeThunk;
    g_GAME_StartupModeVtable[2] = (void *)ReturnZeroBaseModeThunk;
    g_GAME_StartupModeVtable[3] = (void *)NoopBaseModeThunk;
    g_GAME_StartupModeVtable[4] = (void *)ReturnFalseBaseModeThunk;
    g_GAME_StartupModeVtable[5] = 0;
    g_GAME_StartupModeEventClientVtable[0] =
        (void *)InitializeNonZrleVariantRenderEntry;
    g_GAME_StartupModeEventClientVtable[1] =
        (void *)DeleteStartupModeEventClientThunk;
    g_GAME_StartupModeEventClientVtable[2] =
        (void *)HandleBaseModeActionButtonEventThunk;
    g_GAME_StartupModeEventClientVtable[3] = 0;

    g_GAME_MainMenuModeVtable[0] = (void *)DeleteMainMenuModeThunk;
    g_GAME_MainMenuModeVtable[1] = (void *)UpdateBaseModeThunk;
    g_GAME_MainMenuModeVtable[2] = (void *)ReturnZeroBaseModeThunk;
    g_GAME_MainMenuModeVtable[3] = (void *)NoopBaseModeThunk;
    g_GAME_MainMenuModeVtable[4] = (void *)ReturnFalseBaseModeThunk;
    g_GAME_MainMenuModeVtable[5] = 0;
    g_GAME_MainMenuModeEventClientVtable[0] =
        (void *)InitializeNonZrleVariantRenderEntry;
    g_GAME_MainMenuModeEventClientVtable[1] =
        (void *)DeleteMainMenuModeEventClientThunk;
    g_GAME_MainMenuModeEventClientVtable[2] =
        (void *)HandleBaseModeActionButtonEventThunk;
    g_GAME_MainMenuModeEventClientVtable[3] = 0;

    g_GAME_GenericScreenVtableSlots[0] = g_pGAME_ModeDeleteVtableSlot;
    g_GAME_GenericScreenVtableSlots[0x08 / sizeof(void *)] =
        (void *)StubNoOpObjectArgument;
    g_GAME_GenericScreenVtableSlots[0x0c / sizeof(void *)] =
        (void *)StubNoOpObjectArgument;
    g_GAME_GenericScreenVtableSlots[0x10 / sizeof(void *)] =
        (void *)StubNoOpObjectArgument;
    g_GAME_GenericScreenVtableSlots[0x14 / sizeof(void *)] =
        (void *)StubNoOpObjectArgument;
    g_GAME_GenericScreenVtableSlots[0x18 / sizeof(void *)] = (void *)StubNoOpVoid;
    g_GAME_GenericScreenVtableSlots[0x1c / sizeof(void *)] = (void *)StubNoOpVoid;
    g_GAME_GenericScreenVtableSlots[0x20 / sizeof(void *)] = (void *)StubNoOpVoid;
    g_GAME_GenericScreenVtableSlots[0x24 / sizeof(void *)] = (void *)StubNoOpVoid;
    g_GAME_GenericScreenVtableSlots[0x28 / sizeof(void *)] = (void *)StubReturnZero;
    g_GAME_GenericScreenVtableSlots[0x2c / sizeof(void *)] = (void *)StubReturnZero;
    g_GAME_GenericScreenVtableSlots[0x34 / sizeof(void *)] = (void *)StubNoOpVoid;

    g_fInitialized = 1;
}

/* The primary-context table is populated by hand until its complete class
 * declaration is recovered.  Taking the address of an inherited virtual
 * member with MSVC 4.20 yields a virtual-call thunk, not the address stored in
 * the base table.  Preserve the concrete inherited entries left on the object
 * by ConstructWindowOwnerRenderContext before installing the final table. */
static void InstallPrimaryContextInheritedWindowOwnerSlots(
    void **pWindowOwnerVtable) {
    g_GAME_PrimaryContextVtableSlots[0] = pWindowOwnerVtable[0];
    g_GAME_PrimaryContextVtableSlots[4] = pWindowOwnerVtable[4];
    g_GAME_PrimaryContextVtableSlots[5] = pWindowOwnerVtable[5];
    g_GAME_PrimaryContextVtableSlots[9] = pWindowOwnerVtable[9];
    g_GAME_PrimaryContextVtableSlots[12] = pWindowOwnerVtable[12];
    g_GAME_PrimaryContextVtableSlots[13] = pWindowOwnerVtable[13];
    g_GAME_PrimaryContextVtableSlots[19] = pWindowOwnerVtable[19];
    g_GAME_PrimaryContextVtableSlots[20] = pWindowOwnerVtable[20];
    g_GAME_PrimaryContextVtableSlots[24] = pWindowOwnerVtable[24];
    g_GAME_PrimaryContextVtableSlots[26] = pWindowOwnerVtable[26];
    g_GAME_PrimaryContextVtableSlots[27] = pWindowOwnerVtable[27];
    g_GAME_PrimaryContextVtableSlots[29] = pWindowOwnerVtable[29];
    g_GAME_PrimaryContextVtableSlots[30] = pWindowOwnerVtable[30];
    g_GAME_PrimaryContextVtableSlots[31] = pWindowOwnerVtable[31];
    g_GAME_PrimaryContextVtableSlots[34] = pWindowOwnerVtable[34];
    g_GAME_PrimaryContextVtableSlots[35] = pWindowOwnerVtable[35];
    g_GAME_PrimaryContextVtableSlots[36] = pWindowOwnerVtable[36];
    g_GAME_PrimaryContextVtableSlots[37] = pWindowOwnerVtable[37];
    g_GAME_PrimaryContextVtableSlots[38] = pWindowOwnerVtable[38];
    g_GAME_PrimaryContextVtableSlots[39] = pWindowOwnerVtable[39];
}

// FUNCTION: LEMBALL 0x00456500
int __stdcall PumpMessagesAndRunFrame(void) {
    MSG Message;
    BOOL fMessagePending;

    PollCursorPositionEvent();
    g_fRootHelperGeometryDispatchSuppressed = 0;

    if (g_pActiveNetworkRuntimeWindow != 0 &&
        g_pEffTransportSecondaryDispatchQueue != 0) {
        while (*(unsigned int *)((char *)g_pEffTransportSecondaryDispatchQueue + 0x28) != 0) {
            DrainRenderDispatchQueueEntries(
                g_pEffTransportSecondaryDispatchQueue,
                *(unsigned int *)((char *)g_pEffTransportSecondaryDispatchQueue + 0x28));
        }
    }

    fMessagePending = PeekMessageA(&Message, 0, 0, 0, 0);
    if (fMessagePending != 0) {
        while (PeekMessageA(&Message, 0, 0, 0, 0) != 0) {
            GetMessageA(&Message, 0, 0, 0);
            TranslateMessage(&Message);
            DispatchMessageA(&Message);
        }
    }

    if (g_pSharedRenderDispatchQueue != 0) {
        DrainRenderDispatchQueueEntries(
            g_pSharedRenderDispatchQueue,
            *(unsigned int *)((char *)g_pSharedRenderDispatchQueue + 0x28));
    }
    if (g_pMainResourceArchive != 0) {
        ((MogLoadResourceArchive *)g_pMainResourceArchive)
            ->AdvanceCachedResourceObjectFrameCounters();
    }
    if (g_pArrowCursorStatusIndicatorRenderClient != 0) {
        ((GameCursorRenderClient *)
             g_pArrowCursorStatusIndicatorRenderClient)
            ->TickCursorRenderClientMotion();
    }
    return g_fRootHelperGeometryDispatchSuppressed == 1;
}

// FUNCTION: LEMBALL 0x00408080
void SetLevelFrameClockPauseFlag(int fPaused) {
    g_fLevelFrameClockPaused = fPaused;
}

// FUNCTION: LEMBALL 0x00408FD0
bool GameLevelProgressState::CanRetreat(void) {
    return m_nCurrentLevel != 0;
}

// FUNCTION: LEMBALL 0x00406A90
GameLevelProgressState *GameLevelProgressState::Clear(void) {
    int *pFields;

    m_nCurrentLevel = 0;
    pFields = &m_nUnused20;
    m_nUnused04 = 0;
    m_nCurrentPack = 0;
    m_nUnused0C = 0;
    pFields[0] = 0;
    pFields[1] = 0;
    pFields = m_anUnlockedPackCaps;
    pFields[0] = 0;
    pFields[1] = 0;
    pFields[2] = 0;
    pFields[3] = 0;
    pFields[4] = 0;
    pFields = m_anSnapshotPackCaps;
    pFields[0] = 0;
    pFields[1] = 0;
    pFields[2] = 0;
    pFields[3] = 0;
    pFields[4] = 0;
    return this;
}

// FUNCTION: LEMBALL 0x00406AD0
unsigned int GameLevelProgressState::EncodePasswordValue(void) {
    unsigned int auDigits[16];
    unsigned int *puCap;
    unsigned int *puDigit;
    unsigned int *puDigitEnd;
    unsigned int uValue;
    unsigned int uDigit;
    int i;

    puCap = (unsigned int *)m_anUnlockedPackCaps;
    puDigit = auDigits;
    puDigitEnd = auDigits + 8;
    while (puDigit < puDigitEnd) {
        uValue = *puCap++;
        *puDigit++ = (uValue & 0x38) >> 3;
        *puDigit++ = uValue & 7;
    }

    uValue = 0;
    for (i = 0; i < 8; ++i) {
        uDigit = auDigits[g_GAME_LevelPasswordPermutation[i]] ^
                 g_GAME_LevelPasswordPermutation[i];
        uValue = (uValue << 3) | uDigit;
        auDigits[i + 8] = uDigit;
    }
    return uValue;
}

// FUNCTION: LEMBALL 0x00406B30
void GameLevelProgressState::ApplyPasswordValue(unsigned int uValue) {
    unsigned int auDigits[16];
    int i;

    for (i = 15; i >= 8; --i) {
        auDigits[i] = uValue & 7;
        uValue >>= 3;
    }

    for (i = 0; i < 8; ++i) {
        auDigits[g_GAME_LevelPasswordPermutation[i]] = auDigits[i + 8] ^ g_GAME_LevelPasswordPermutation[i];
    }

    for (i = 0; i < 4; ++i) {
        SetUnlockedPackCapThunk(i, (int)((auDigits[i * 2] << 3) | auDigits[i * 2 + 1]));
    }
}

// FUNCTION: LEMBALL 0x00402130
void GameLevelProgressState::ApplyPasswordValueThunk(unsigned int uValue) {
    ApplyPasswordValue(uValue);
}

// FUNCTION: LEMBALL 0x00406BA0
unsigned int GameLevelProgressState::ComputeLevelPasswordChecksum(unsigned int uValue) {
    unsigned int uHigh;
    unsigned int uMiddle;

    uHigh = uValue >> 16;
    uMiddle = uValue >> 8;
    return (uHigh + uMiddle + uValue) & 0x1f;
}

// FUNCTION: LEMBALL 0x00403323
unsigned int GameLevelProgressState::ComputeLevelPasswordChecksumThunk(unsigned int uValue) {
    return ComputeLevelPasswordChecksum(uValue);
}

// FUNCTION: LEMBALL 0x00406BC0
int PASCAL ComputePowerOfTwo(int nExponent) {
    int nValue;

    nValue = 1;
    while (nExponent != 0) {
        nValue *= 2;
        --nExponent;
    }
    return nValue;
}

// FUNCTION: LEMBALL 0x004036E3
int PASCAL ComputePowerOfTwoThunk(int nExponent) {
    return ComputePowerOfTwo(nExponent);
}

// FUNCTION: LEMBALL 0x00406C00
char *GameLevelProgressState::FormatPassword(void) {
    char szDigits[12];
    unsigned int uValue;
    unsigned int cchDigits;

    uValue = EncodePasswordValue();
    uValue |= ComputeLevelPasswordChecksumThunk(uValue) << 0x18;
    FormatSignedIntToRadixString((int)uValue, szDigits, 10);
    strcpy(m_szPassword, "00000000");
    cchDigits = (unsigned int)strlen(szDigits);
    memcpy(m_szPassword + 10 - cchDigits, szDigits, cchDigits + 1);
    return m_szPassword;
}

// FUNCTION: LEMBALL 0x00406CA0
int GameLevelProgressState::ValidateAndApplyPassword(char *pszPassword) {
    int i;
    unsigned int uChecksum;
    unsigned int uValue;

    strcpy(m_szPassword, pszPassword);
    if (strcmp(m_szPassword, "9913454278") == 0) {
        g_pStatusOutputStream->AppendCStringToStream(
            "Oh, oh, someones cheating !!!\n");
        for (i = 0; i < 4; ++i) {
            ((GameLevelProgressState *)g_pLevelProgressState)
                ->SetUnlockedPackCapThunk(i, 0x40);
        }
        return 1;
    }

    uValue = (unsigned int)ParsePasswordDigitsThunk();
    if (uValue == 0) {
        return 0;
    }
    uChecksum = ComputeLevelPasswordChecksumThunk(uValue);
    if (((uValue & 0x1f000000) >> 0x18) != uChecksum) {
        return 0;
    }
    ApplyPasswordValueThunk(uValue & 0xffffff);
    return 1;
}

// FUNCTION: LEMBALL 0x00406D80
int GameLevelProgressState::ParsePasswordDigits(void) {
    int nValue;
    unsigned int i;
    unsigned int cchPassword;

    nValue = 0;
    cchPassword = (unsigned int)strlen(m_szPassword);
    for (i = 0; i < cchPassword; ++i) {
        nValue = m_szPassword[i] - '0' + nValue * 10;
    }
    return nValue;
}

// FUNCTION: LEMBALL 0x00406DD0
void GameLevelProgressState::Snapshot(void) {
    int i;
    int *pSnapshotCap;

    pSnapshotCap = m_anSnapshotPackCaps;
    i = 4;
    do {
        *(pSnapshotCap++) = pSnapshotCap[-5];
        --i;
    } while (i != 0);
}

// FUNCTION: LEMBALL 0x004034FE
int GameLevelProgressState::ParsePasswordDigitsThunk(void) {
    return ParsePasswordDigits();
}

// FUNCTION: LEMBALL 0x00408FE0
void GameLevelProgressState::SetUnlockedPackCap(int nPack, int nCap) {
    int nPackLevelCount;

    switch (nPack) {
    case 0:
        nPackLevelCount = 0x18;
        break;
    case 1:
        nPackLevelCount = 0x19;
        break;
    case 2:
        nPackLevelCount = 0x1c;
        break;
    case 3:
        nPackLevelCount = 0x15;
        break;
    case 4:
        nPackLevelCount = 0x0b;
        break;
    }

    if (m_nCurrentPack == nPack && nCap < m_nCurrentLevel) {
        nCap = m_nCurrentLevel;
    }
    if (nPackLevelCount < nCap) {
        m_anUnlockedPackCaps[nPack] = nPackLevelCount;
        return;
    }
    m_anUnlockedPackCaps[nPack] = nCap;
}

// FUNCTION: LEMBALL 0x00407300
int __stdcall ValidateResourceFileSignature(void) {
    const unsigned char *pszKey;
    GameResourceSignatureStringObject *pStringResource;
    const unsigned char *pszEncoded;
    int i;

    pszKey = (const unsigned char *)g_GAME_ResourceSignatureKey;
    pStringResource = (GameResourceSignatureStringObject *)LoadStringResource(0x100);
    if (pStringResource == 0) {
        return 0;
    }

    if (pStringResource->m_nLoadState10 != 0) {
        pStringResource->m_nReserved24 = 0;
    } else {
        pStringResource->PrepareStringResource();
    }

    ++pStringResource->m_nLockCount08;
    pszEncoded = (const unsigned char *)pStringResource->m_pszText38;
    i = 0;
    while (pszEncoded[i] != '\0') {
        g_GAME_ResourceSignatureDecodeBuffer[i] =
            (char)((pszEncoded[i] - 1) ^ *pszKey++);
        ++i;
    }
    g_GAME_ResourceSignatureDecodeBuffer[i] = '\0';

    --pStringResource->m_nLockCount08;
    ReleaseTypedResourceObjectReference(pStringResource);

    return strcmp(g_GAME_ResourceSignatureDecodeBuffer,
                  g_GAME_ResourceSignatureExpected) == 0;
}

// FUNCTION: LEMBALL 0x004014B0
void GameLevelProgressState::SetUnlockedPackCapThunk(int nPack, int nCap) {
    SetUnlockedPackCap(nPack, nCap);
}

// FUNCTION: LEMBALL 0x004734F0
void ShowReleaseAssertFailureAndAbort(const char *pszExpression, const char *pszFile, int nLine) {
    GameDynamicCString Message;
    GameDynamicCString Result;
    char szLine[16];

    Message.ConstructDynamicCString();
    Message.AssignDynamicCString("Release Version Assertion Failure\n");
    Message.AppendDynamicCStringAndCopyResult(&Result, "Reason: ");
    Result.DestroyDynamicCString();
    Message.AppendDynamicCStringAndCopyResult(&Result, pszExpression);
    Result.DestroyDynamicCString();
    Message.AppendDynamicCStringAndCopyResult(&Result, "\n");
    Result.DestroyDynamicCString();
    Message.AppendDynamicCStringAndCopyResult(&Result, "In File: ");
    Result.DestroyDynamicCString();
    Message.AppendDynamicCStringAndCopyResult(&Result, pszFile);
    Result.DestroyDynamicCString();
    Message.AppendDynamicCStringAndCopyResult(&Result, "At Line No.: ");
    Result.DestroyDynamicCString();
    FormatSignedIntToRadixString(nLine, szLine, 10);
    Message.AppendDynamicCStringAndCopyResult(&Result, szLine);
    Result.DestroyDynamicCString();
    MessageBoxA(0, Message.m_pszText, "Error", 0);
    LongjmpStartupExceptionFrame(0xaaaa);
    Message.DestroyDynamicCString();
}

// FUNCTION: LEMBALL 0x00403152
int __stdcall ValidateResourceFileSignatureThunk(void) {
    return ValidateResourceFileSignature();
}

// FUNCTION: LEMBALL 0x0045B8E0
void TriggerReleaseAssertFailure(const char *pszExpression, const char *pszFile, int nLine) {
    ShowReleaseAssertFailureAndAbort(pszExpression, pszFile, nLine);
}

// FUNCTION: LEMBALL 0x00401C6C
void ReleaseMainGameVariantResourceMode(GameMainContext *pMainContext) {
    if (pMainContext->m_pVariantMode != 0) {
        DestroyMainGameVariantResourceMode(pMainContext->m_pVariantMode);
        FreeVSMemBlock(pMainContext->m_pVariantMode);
        pMainContext->m_pVariantMode = 0;
    }
}

// FUNCTION: LEMBALL 0x004027A7
void EnsureMainGameVariantResourceMode(GameMainContext *pMainContext, int nVariantMode) {
    if (pMainContext->m_pVariantMode == 0) {
        pMainContext->m_pVariantMode = AllocateVSMemBlock(0x58);
        if (pMainContext->m_pVariantMode != 0) {
            pMainContext->m_pVariantMode =
                ConstructMainGameVariantResourceBundle(pMainContext->m_pVariantMode,
                                                       pMainContext->m_pPrimaryContext,
                                                       (unsigned short)nVariantMode);
            return;
        }
        pMainContext->m_pVariantMode = 0;
    }
}

// FUNCTION: LEMBALL 0x004015E6
void InitializeBaseModeObject(void *pModeObject, GameMainContext *pMainContext) {
    GameBaseModeObject *pMode;

    pMode = (GameBaseModeObject *)pModeObject;
    pMode->m_pVtable = (void **)g_pSimpleModeDeleteVtable;
    pMode->m_nRequestedMode = 0;
    pMode->m_nStatus = 0;
    InitializeRenderQueueNodeBase(&pMode->m_pRenderQueueNodeVtable);
    pMode->m_pVtable = g_GAME_GenericModeVtableSlots;
    pMode->m_pRenderQueueNodeVtable = (void **)g_pStartupModeRenderThunk;
    pMode->m_pMainContext = pMainContext;
    pMode->m_pTransportClient = AllocateVSMemBlock(0x34);
    if (pMode->m_pTransportClient != 0) {
        pMode->m_pTransportClient = ConstructEffStreamPayloadSize8(pMode->m_pTransportClient);
    }
    if (((GameLevelProgressState *)g_pLevelProgressState)->m_nCurrentPack == 4 && g_nSelectedNetworkLobbyPeerId != 0) {
        pMode->m_fNetworkLobbyActive = 1;
    } else {
        pMode->m_fNetworkLobbyActive = 0;
    }
    if (g_pActiveNetworkRuntimeWindow != 0) {
        ((GameEffTransportRuntimeWindow *)g_pActiveNetworkRuntimeWindow)
            ->RegisterEffTransportEventClient(&pMode->m_pRenderQueueNodeVtable);
    }
    g_pActiveNetworkLobbyTransportController = pMode;
}

// FUNCTION: LEMBALL 0x00401D5C
void *ConstructLevelSelectionModeController(void *pModeObject, GameMainContext *pMainContext) {
    GameLevelSelectionModeController *pMode;

    pMode = (GameLevelSelectionModeController *)pModeObject;
    InitializeBaseModeObject(pMode, pMainContext);
    pMode->m_pVtable = (void **)g_pStartupModeVtable;
    pMode->m_pRenderQueueNodeVtable = (void **)g_pStartupModeRenderThunk;
    return pModeObject;
}

// FUNCTION: LEMBALL 0x00401B90
void *ConstructNetworkLobbyTransportController(void *pModeObject, GameMainContext *pMainContext) {
    GameNetworkLobbyTransportController *pMode;

    pMode = (GameNetworkLobbyTransportController *)pModeObject;
    InitializeBaseModeObject(pMode, pMainContext);
    pMode->m_pVtable = (void **)g_pMode16Vtable;
    pMode->m_pRenderQueueNodeVtable = (void **)g_pMode16RenderThunk;
    pMode->m_pPeerClearCloseStream = AllocateVSMemBlock(0x30);
    if (pMode->m_pPeerClearCloseStream != 0) {
        pMode->m_pPeerClearCloseStream = ConstructNetworkLobbyPeerClearCloseStream(pMode->m_pPeerClearCloseStream);
    }
    pMode->m_pPeerDirtyConfirmStream = AllocateVSMemBlock(0x30);
    if (pMode->m_pPeerDirtyConfirmStream != 0) {
        pMode->m_pPeerDirtyConfirmStream = ConstructNetworkLobbyPeerDirtyConfirmStream(pMode->m_pPeerDirtyConfirmStream);
    }
    pMode->m_nReserved2C = 0;
    pMode->m_nReserved28 = 0;
    g_pNetworkLobbyTransportController = pMode;
    return pModeObject;
}

// FUNCTION: LEMBALL 0x004022AC
void *ConstructPaintballSequenceModeController(void *pModeObject,
                                               GameMainContext *pMainContext,
                                               int fSequenceVariant)
{
    GamePaintballSequenceModeController *pMode;

    pMode = (GamePaintballSequenceModeController *)pModeObject;
    InitializeBaseModeObject(pMode, pMainContext);
    pMode->m_pVtable = (void **)g_pMainMenuModeVtable;
    pMode->m_pRenderQueueNodeVtable = (void **)g_pMainMenuModeRenderThunk;
    pMode->m_fFirstSequenceVariant = fSequenceVariant;
    return pModeObject;
}

// FUNCTION: LEMBALL 0x00402B8F
void *ConstructLevelGameMode(void *pModeObject, GameMainContext *pMainContext) {
    GameLevelGameMode *pMode;

    pMode = (GameLevelGameMode *)pModeObject;
    InitializeRenderQueueNodeBase(pMode);
    pMode->m_pReserved14 = 0;
    pMode->m_pReserved18 = 0;
    pMode->m_pEffStreamVtable = (void **)&g_GAME_GenericScreenVtableSlots[10];
    pMode->m_pReserved20 = 0;
    pMode->m_pDeleteThunkVtable = (void **)g_pSimpleModeDeleteVtable;
    ((GameEffStream *)&pMode->m_pEffStreamVtable)->ResetStateFields();
    pMode->m_nReserved124 = 0;
    pMode->m_nReserved128 = 0;
    pMode->m_nReserved12C = 0;
    pMode->m_nReserved134 = 0;
    pMode->m_nReserved138 = 0;
    pMode->m_nReserved13C = 0;
    pMode->m_nReserved140 = -1;
    pMode->m_nReserved144 = -1;
    pMode->m_nReserved148 = -1;
    pMode->m_nManagedEntityCount = 0;
    pMode->m_nManagedEntityCapacity = 100;
    pMode->m_pManagedEntityTable = AllocateVSMemBlock(400);
    if (pMode->m_nManagedEntityCapacity > 0 && pMode->m_pManagedEntityTable != 0) {
        memset(pMode->m_pManagedEntityTable, 0, pMode->m_nManagedEntityCapacity * sizeof(void *));
    }
    pMode->m_anHeaderDwords[0] = (int)(unsigned long)g_pQueuedRenderPointSinkFinalizeThunk;
    pMode->m_pEffStreamVtable = (void **)g_pMode3Vtable;
    pMode->m_pDeleteThunkVtable = (void **)g_pMode3RenderThunk;
    pMode->m_pMainContext = pMainContext;
    pMode->m_pReserved20 = 0;
    ((LevelGameMode *)pMode)->InitializeLevelGameMode();
    return pModeObject;
}

// FUNCTION: LEMBALL 0x00410D00
void LevelGameMode::InitializeLevelGameMode(void) {
    char *pModeBytes;
    int i;
    int *pZeroCursor;
    int *pManagedSlots;
    int *pHeaderWords;
    unsigned short nDemoHeader;
    unsigned int cbDemoRecord;
    unsigned int nLevel;
    unsigned int nSkill;
    void *pObject;
    void *pChildObject;
    void *pDispatcherClient;
    int *pChunkTypeFlags;

    void *pLevelGameMode;

    pModeBytes = (char *)this;
    pLevelGameMode = (void *)this;
    InitializeManagedEntitySlotTablesEntryThunk(pLevelGameMode);
    g_pActiveLevelGameMode = pLevelGameMode;
    *(int *)(pModeBytes + 0x118) = 0;
    if (*(int *)(pModeBytes + 0x11c) > 0) {
        pManagedSlots = (int *)*(void **)(pModeBytes + 0x120);
        for (i = 0; i < *(int *)(pModeBytes + 0x11c); ++i) {
            pManagedSlots[i] = 0;
        }
    }

    g_nGameAppState0 = 0;
    g_nGameAppState1 = 0;
    *(int *)((char *)g_pLevelProgressState + 0x0c) = 0;
    *(int *)(pModeBytes + 0x10c) = 0;
    ResetLevelFrameClockEntryThunk();
    *(int *)(pModeBytes + 0x88) = 0;
    *(int *)(pModeBytes + 0x68) = 0;
    *(int *)(pModeBytes + 0x6c) = 0;
    *(int *)(pModeBytes + 0x7c) = 0;
    *(int *)(pModeBytes + 0x5c) = 0;
    *(int *)(pModeBytes + 0x58) = 0;
    *(int *)(pModeBytes + 0x70) = 1;
    *(int *)(pModeBytes + 0x34) += 0x60;
    *(int *)(pModeBytes + 0x64) = (((GameLevelProgressState *)g_pLevelProgressState)->m_nCurrentPack == 4);

    if (*(int *)(pModeBytes + 0x64) != 0) {
        if (*(int *)(pModeBytes + 0x4c) == 0) {
            pObject = AllocateVSMemBlock(0x34);
            if (pObject != 0) {
                pObject = ((GameNetworkLevelChunkDeltaStream *)pObject)
                    ->ConstructNetworkLevelChunkDeltaStreamEntry((int)(unsigned long)pLevelGameMode);
            }
            *(void **)(pModeBytes + 0x78) = pObject;
        }

        *(int *)(pModeBytes + 0x7c) = *(int *)(unsigned long)(g_nSelectedNetworkLobbyPeerId + 0x1c);
        if (*(int *)(pModeBytes + 0x4c) == 0) {
            pObject = AllocateVSMemBlock(0x3c);
            if (pObject != 0) {
                pObject = ConstructLevelGameStateStreamThunk(pObject);
            }
            *(void **)(pModeBytes + 0x74) = pObject;
        }

        ((GameLevelProgressState *)g_pLevelProgressState)->m_nUnused04 = 0;
    }

    pZeroCursor = (int *)(pModeBytes + 0xb0);
    for (i = 4; i != 0; --i) {
        *pZeroCursor = 0;
        pZeroCursor[-4] = 0;
        pZeroCursor[-8] = 0;
        pZeroCursor[4] = 0;
        ++pZeroCursor;
    }

    *(int *)(pModeBytes + 0xc0) = 4;
    *(unsigned short *)(pModeBytes + 0x54) = 0;
    *(int *)(pModeBytes + 0xd0) = 1;
    *(int *)(pModeBytes + 0xd4) = 4;
    *(int *)(pModeBytes + 0x100) = 1;
    *(int *)(pModeBytes + 0xe8) = 0;
    *(int *)(pModeBytes + 0xf8) = 4;
    *(int *)(pModeBytes + 0xfc) = 1;
    *(int *)(pModeBytes + 0xf0) = ((GameLevelProgressState *)g_pLevelProgressState)->m_nUnused04;
    *(int *)(pModeBytes + 0xf4) = ((GameLevelProgressState *)g_pLevelProgressState)->m_nUnused04;
    *(int *)(pModeBytes + 0xd8) = 0;
    g_nGameAppState2 = 0;
    *(int *)(pModeBytes + 0xdc) = 1;
    *(int *)(pModeBytes + 0x60) = 0;
    *(int *)(pModeBytes + 0xe4) = 0;
    *(int *)(pModeBytes + 0x108) = 0;
    *(int *)(pModeBytes + 0x18) = 0;
    *(int *)(pModeBytes + 0xec) = 0xb4;

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x58);
        if (pObject != 0) {
            pObject = ConstructLevelTileGridThunk(pObject);
        }
        *(void **)(pModeBytes + 0x110) = pObject;
    }

    InitializeLevelTileGridThunk(*(void **)(pModeBytes + 0x110));
    g_pLevelTileGrid = *(void **)(pModeBytes + 0x110);
    *(void **)((char *)g_pLevelTileGrid + 0x54) = pLevelGameMode;
    *(void **)((char *)g_pLevelTileGrid + 0x18) = pLevelGameMode;

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x103c);
        if (pObject != 0) {
            pObject = ((LevelTileReachabilityHelper *)pObject)
                ->ConstructLevelTileReachabilityHelperThunk(*(void **)(pModeBytes + 0x110));
        }
        *(void **)(pModeBytes + 0x114) = pObject;
    }
    g_pLevelTileReachabilityHelper = *(void **)(pModeBytes + 0x114);

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x58);
        if (pObject != 0) {
            pObject = ((RenderDispatchQueue *)pObject)->ConstructRenderDispatchQueueVariant(
                10, "AIQueue");
        }
        *(void **)(pModeBytes + 0x164) = pObject;
        ((GameRenderDispatchQueue *)*(void **)(pModeBytes + 0x164))
            ->RegisterOrderedRenderDispatchClient(pLevelGameMode, 0);
    }

    if (*(int *)(pModeBytes + 0x64) != 0 && g_nSelectedNetworkLobbyPeerId != 0) {
        pDispatcherClient = (void *)(unsigned long)(*(int *)(unsigned long)(*(int *)(unsigned long)(g_nSelectedNetworkLobbyPeerId + 4) + 0xc) +
                                                     4 + g_nSelectedNetworkLobbyPeerId);
        ClearPrimaryLockedRecordTablePayloadFlags(pDispatcherClient);
        ClearSecondaryLockedRecordTablePayloadFlags(pDispatcherClient);
    }

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x24);
        if (pObject != 0) {
            pObject = ConstructLevelChunkStreamDispatcherThunk(pObject, 0x14);
        }
        g_pLevelChunkStreamDispatcher = pObject;
    } else {
        ResetRegisteredLevelChunkStreamsThunk(g_pLevelChunkStreamDispatcher);
    }

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x40);
        if (pObject != 0) {
            pObject = ConstructLiftChunkManagerThunk(pObject, pLevelGameMode, 0x3c);
        }
        *(void **)(pModeBytes + 0x18c) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x18c));

        pObject = AllocateVSMemBlock(0x40);
        if (pObject != 0) {
            pObject = ConstructMoveChunkManagerThunk(pObject, pLevelGameMode, 0x28);
        }
        *(void **)(pModeBytes + 0x1c0) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x1c0));
    }

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x40);
        if (pObject != 0) {
            pObject = ConstructGmobChunkManagerThunk(pObject, pLevelGameMode);
        }
        *(void **)(pModeBytes + 0x158) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x158));
    }

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x104);
        if (pObject != 0) {
            pObject = ((LevelProjectileGeometryPairTable *)pObject)
                          ->InitializeProjectileGeometryPairTableThunk();
        }
        *(void **)(pModeBytes + 0x16c) = pObject;
    }
    ((LevelProjectileGeometryPairTable *)*(void **)(pModeBytes + 0x16c))
        ->ClearProjectileGeometryField100Thunk();

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x150);
        if (pObject != 0) {
            pObject = ConstructPlasChunkManagerThunk(
                pObject, pLevelGameMode, *(void **)(pModeBytes + 0x158), *(void **)(pModeBytes + 0x16c));
        }
        *(void **)(pModeBytes + 0x15c) = pObject;
        if (pObject != 0) {
            RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, (char *)pObject + 0xb0);
        } else {
            RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, 0);
        }

        pObject = AllocateVSMemBlock(0xe0);
        if (pObject != 0) {
            pObject = ConstructProjectilePoolThunk(pObject);
        }
        *(void **)(pModeBytes + 0x168) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x168));

        pObject = AllocateVSMemBlock(0x28);
        if (pObject != 0) {
            pObject = ConstructManagedEntityQueueCursorThunk(pObject, (int)(unsigned long)pLevelGameMode, 0x400, 0x400);
        }
        *(void **)(pModeBytes + 0x160) = pObject;
    }

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0xb0);
        if (pObject != 0) {
            pObject = ConstructShpgChunkManagerThunk(
                pObject, pLevelGameMode, *(void **)(pModeBytes + 0x158), *(void **)(pModeBytes + 0x16c));
        }
        *(void **)(pModeBytes + 0x170) = pObject;
    }
    ((LevelShpgChunkManager *)*(void **)(pModeBytes + 0x170))
        ->ActivateNestedChildrenFromOwnerTableA4Thunk();

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0xb0);
        if (pObject != 0) {
            pObject = ConstructEnmyChunkManagerThunk(
                pObject, pLevelGameMode, *(void **)(pModeBytes + 0x158), *(void **)(pModeBytes + 0x16c));
        }
        *(void **)(pModeBytes + 0x174) = pObject;
    }
    ((LevelEnmyChunkManager *)*(void **)(pModeBytes + 0x174))
        ->ActivateNestedChildrenFromOwnerTableVariantThunk();

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x0c);
        if (pObject != 0) {
            pObject = ((LevelNodeChunkManager *)pObject)->ConstructNodeChunkManagerThunk(300);
        }
        *(void **)(pModeBytes + 0x17c) = pObject;
    }
    ((LevelNodeChunkManager *)*(void **)(pModeBytes + 0x17c))->ResetNodeChunkRecordsThunk();

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x10);
        if (pObject != 0) {
            pObject = ((LevelBallChunkManager *)pObject)
                          ->ConstructBallChunkManagerThunk(pLevelGameMode, 0x14);
        }
        *(void **)(pModeBytes + 0x180) = pObject;
    }
    ResetBallChunkEntriesThunk(*(void **)(pModeBytes + 0x180));

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x40);
        if (pObject != 0) {
            pObject = ConstructCollChunkManagerThunk(pObject, pLevelGameMode, 0x1e);
        }
        *(void **)(pModeBytes + 0x184) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x184));

        pObject = AllocateVSMemBlock(0x44);
        if (pObject != 0) {
            pObject = ConstructMineChunkManagerThunk(pObject, pLevelGameMode, 0x28);
        }
        *(void **)(pModeBytes + 0x188) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x188));

        pObject = AllocateVSMemBlock(0x40);
        if (pObject != 0) {
            pObject = ConstructDoorChunkManagerThunk(pObject, pLevelGameMode, 0x14);
        }
        *(void **)(pModeBytes + 0x190) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x190));

        pObject = AllocateVSMemBlock(0x40);
        if (pObject != 0) {
            pObject = ConstructRockChunkManagerThunk(pObject, pLevelGameMode, 0x14);
        }
        *(void **)(pModeBytes + 0x198) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x198));

        pObject = AllocateVSMemBlock(0x40);
        if (pObject != 0) {
            pObject = ConstructCaptureTriggerChunkManagerThunk(pObject, pLevelGameMode, 0x14);
        }
        *(void **)(pModeBytes + 0x1a4) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x1a4));

        pObject = AllocateVSMemBlock(0x40);
        if (pObject != 0) {
            pObject = ConstructLasrChunkManagerThunk(pObject, pLevelGameMode, 0x14);
        }
        *(void **)(pModeBytes + 0x1a0) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x1a0));

        pObject = AllocateVSMemBlock(0x12cc);
        if (pObject != 0) {
            pObject = ReturnArgumentVtableCallback(pObject);
        }
        *(void **)(pModeBytes + 0x194) = pObject;
    }
    ((LevelField194TimerBlock *)*(void **)(pModeBytes + 0x194))
        ->ResetLevelField194TimerBlockThunk();

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x4c);
        if (pObject != 0) {
            pObject = ConstructBoonChunkManagerThunk(pObject, pLevelGameMode, *(void **)(pModeBytes + 0x110));
        }
        *(void **)(pModeBytes + 0x1a8) = pObject;
    }
    ResetBoonChunkManagerObjectsThunk(*(void **)(pModeBytes + 0x1a8));

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x40);
        if (pObject != 0) {
            pObject = ConstructTramChunkManagerThunk(pObject, pLevelGameMode, 0x14);
        }
        *(void **)(pModeBytes + 0x1b4) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x1b4));

        pObject = AllocateVSMemBlock(0x40);
        if (pObject != 0) {
            pObject = ConstructPgunChunkManagerThunk(pObject, pLevelGameMode, 0x14);
        }
        *(void **)(pModeBytes + 0x1b0) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x1b0));

        pObject = AllocateVSMemBlock(100);
        if (pObject != 0) {
            pObject = ConstructIceChunkManagerThunk(pObject, pLevelGameMode, 100);
        }
        *(void **)(pModeBytes + 0x1ac) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x1ac));

        pObject = AllocateVSMemBlock(0x54);
        if (pObject != 0) {
            pObject = ConstructType18ChunkStreamThunk(
                (LevelType18ChunkStream *)pObject);
        }
        *(void **)(pModeBytes + 0x1c4) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x1c4));
    }

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x10);
        if (pObject != 0) {
            pObject = ((LevelSlnkChunkManager *)pObject)
                          ->ConstructSlnkChunkManagerThunk(pLevelGameMode, 0x14);
        }
        *(void **)(pModeBytes + 0x1c8) = pObject;
    }
    ((LevelSlnkChunkManager *)*(void **)(pModeBytes + 0x1c8))
        ->ResetType35ChunkObjectsThunk();

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x40);
        if (pObject != 0) {
            pObject = ConstructInvsChunkManagerThunk(pObject, pLevelGameMode, 0x28);
        }
        *(void **)(pModeBytes + 0x1cc) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x1cc));

        pObject = AllocateVSMemBlock(8);
        if (pObject != 0) {
            pObject = ((LevelChunkLoaderContext *)pObject)
                          ->ConstructLevelChunkLoaderContextThunk(
                              pLevelGameMode);
        }
        *(void **)(pModeBytes + 0x178) = pObject;
    }

    if (g_fLevelDemoModeEnabled == 0) {
        nLevel = (unsigned int)GetSelectedLevelNumberThunk(g_pLevelProgressState);
        nSkill = (unsigned int)((GameLevelProgressState *)g_pLevelProgressState)->m_nCurrentPack;
    } else {
        nDemoHeader = 0;
        cbDemoRecord = 0;
        ((DemoLevelDemoPlaybackController *)g_pLevelDemoPlaybackController)
            ->SetLevelDemoPlaybackEnabled(1);
        ReadLevelDemoLengthPrefixedRecordThunk(g_pLevelDemoPlaybackController, &nDemoHeader, &cbDemoRecord);
        nLevel = (unsigned int)(nDemoHeader & 0xff);
        nSkill = (unsigned int)((nDemoHeader >> 8) & 0xff);
        AppendStartupCString(g_GAME_StartingDemoModePrefix);
        AppendStartupUInt(nLevel);
        AppendStartupCString(g_GAME_OnSkillText);
        AppendStartupUInt(nSkill);
        AppendStartupCString(g_GAME_LineBreak);
        *(unsigned int *)g_pSessionRandomState = 0xad28;
    }

    LoadLevelChunksForSkillAndLevelThunk(*(void **)(pModeBytes + 0x178), nSkill, nLevel, *(int *)(pModeBytes + 0x4c));
    InitializeNetworkPlayerPlasChunkObjectPoolThunk(*(void **)(pModeBytes + 0x15c));
    if (*(int *)(pModeBytes + 0x4c) == 0) {
        AssignNetworkPlayerManagedEntitySlotIdsThunk((int)(unsigned long)pLevelGameMode);
        ResizeLevelTileReachabilityGridThunk(*(void **)(pModeBytes + 0x114));
    } else {
        FillReachabilityGridFromTileFlagsThunk(*(void **)(pModeBytes + 0x114));
    }

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pHeaderWords = (int *)AllocateVSMemBlock(8);
        if (pHeaderWords != 0) {
            pHeaderWords[0] = 0;
            pHeaderWords[1] = 0;
        }
        *(int **)(pModeBytes + 0x8c) = pHeaderWords;
    }
    ((LevelSpecialTileGridStaticRenderList *)*(void **)(pModeBytes + 0x8c))
        ->BuildSpecialLevelTileGridStaticRenderEntriesThunk(
            (LevelLevelTileGridStaticView *)*(void **)(pModeBytes + 0x110));

    if (*(unsigned short *)(pModeBytes + 0x54) == 0) {
        AssignMissingManagedEntitySlotIdsThunk((int)(unsigned long)pLevelGameMode);
    }
    *(unsigned short *)(pModeBytes + 0x54) = 10;

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        *(void **)(pModeBytes + 0x50) = AllocateVSMemBlock(0xdc);
    }
    pChunkTypeFlags = (int *)*(void **)(pModeBytes + 0x50);
    for (i = 0; i < 0xdc / 4; ++i) {
        pChunkTypeFlags[i] = 0;
    }
    InitializeLevelChunkTypeEnabledFlagsThunk(pLevelGameMode);
    *(int *)(pModeBytes + 0x4c) = 1;
}

// FUNCTION: LEMBALL 0x00463B50
void *LEMBALL_FASTCALL ConstructWindowOwnerRenderContext(void *pPrimaryContext) {
    char *pEntry;
    void *pWindowOwnerBaseVtable;
    int i;

    new (pPrimaryContext) VS_WindowOwnerBase;
    pWindowOwnerBaseVtable = *(void **)pPrimaryContext;
    pEntry = (char *)pPrimaryContext + 0x58;
    for (i = 0; i >= 0; --i) {
        InitializePackagedSpriteRenderEntryThunk(pEntry);
        pEntry += 0x10;
    }
    pEntry = (char *)pPrimaryContext + 0x68;
    for (i = 0; i >= 0; --i) {
        InitializeRenderPointRectSinkEntryThunk(pEntry);
        pEntry += 0x10;
    }
    pEntry = (char *)pPrimaryContext + 0x78;
    for (i = 0; i >= 0; --i) {
        InitializeStatusIndicatorPointSinkEntryThunk(pEntry);
        pEntry += 0x10;
    }

    *(void **)pPrimaryContext = pWindowOwnerBaseVtable;
    *(unsigned short *)((char *)pPrimaryContext + 0x8a) = 0;
    *(void **)pPrimaryContext = pWindowOwnerBaseVtable;
    *(unsigned short *)((char *)pPrimaryContext + 0x88) = 0;
    *(int *)((char *)pPrimaryContext + 0x4c) = 0;
    *(unsigned short *)((char *)pPrimaryContext + 0x8e) = 0;
    *(int *)((char *)pPrimaryContext + 0x50) = 0;
    *(unsigned short *)((char *)pPrimaryContext + 0x8c) = 0;
    *(int *)((char *)pPrimaryContext + 0x54) = 0;
    return pPrimaryContext;
}

// FUNCTION: LEMBALL 0x00462EA0
void *LEMBALL_FASTCALL InitializeRenderQueueNodeBase(void *pRenderQueueNode) {
    *(void **)pRenderQueueNode = (void *)g_GAME_RenderQueueNodeVtableSlots;
    *(int *)((char *)pRenderQueueNode + 8) = 0;
    *(int *)((char *)pRenderQueueNode + 0xc) = 0;
    *(int *)((char *)pRenderQueueNode + 4) = 0x51484452;
    return pRenderQueueNode;
}

// FUNCTION: LEMBALL 0x004632A0
int GameRenderDispatchQueue::RegisterOrderedRenderDispatchClient(void *pClient, int nOrder) {
    GameRenderDispatchClientNode *pNode;
    GameRenderDispatchClientNode *pCurrent;
    GameRenderDispatchClientNode *pPrevious;
    unsigned int i;

    LockDispatchQueue();
    pNode = (GameRenderDispatchClientNode *)AllocateVSMemBlock(0xc);
    pNode->m_pClient = pClient;
    pNode->m_nOrder = nOrder;
    pNode->m_pNext = 0;

    pCurrent = (GameRenderDispatchClientNode *)m_pClientList;
    if (pCurrent == 0) {
        m_pClientList = pNode;
        m_cClients = 1;
        UnlockDispatchQueue();
        return 1;
    }

    if (m_cClients == 0) {
        m_pClientList = pNode;
        m_cClients = 1;
        UnlockDispatchQueue();
        return 1;
    }

    pPrevious = pCurrent;
    i = 0;
    while (i < (unsigned int)m_cClients) {
        if (nOrder < pCurrent->m_nOrder) {
            if (i == 0) {
                pNode->m_pNext = (GameRenderDispatchClientNode *)m_pClientList;
                m_pClientList = pNode;
            } else {
                pNode->m_pNext = pPrevious->m_pNext;
                pPrevious->m_pNext = pNode;
            }
            ++m_cClients;
            UnlockDispatchQueue();
            return 1;
        }
        if (pCurrent->m_pNext == 0) {
            pCurrent->m_pNext = pNode;
            ++m_cClients;
            UnlockDispatchQueue();
            return 1;
        }
        ++i;
        pPrevious = pCurrent;
        pCurrent = pCurrent->m_pNext;
    }
    UnlockDispatchQueue();
    return 0;
}

// FUNCTION: LEMBALL 0x004633B0
int GameRenderDispatchQueue::UnregisterOrderedRenderDispatchClient(void *pClient, int nOrder) {
    GameRenderDispatchClientNode *pCurrent;
    GameRenderDispatchClientNode *pPrevious;
    unsigned int i;

    LockDispatchQueue();
    pCurrent = (GameRenderDispatchClientNode *)m_pClientList;
    pPrevious = pCurrent;
    i = 0;
    while (i < (unsigned int)m_cClients) {
        if (pCurrent->m_nOrder == nOrder && pCurrent->m_pClient == pClient) {
            if (i != 0) {
                pPrevious->m_pNext = pCurrent->m_pNext;
            } else {
                m_pClientList = pCurrent->m_pNext;
            }
            FreeVSMemBlock(pCurrent);
            --m_cClients;
            UnlockDispatchQueue();
            return 1;
        }
        ++i;
        pPrevious = pCurrent;
        pCurrent = pCurrent->m_pNext;
    }
    UnlockDispatchQueue();
    return 0;
}

struct GamePrimaryContextExtendedVirtualInterface {
    virtual void Reserved0(void) = 0;
    virtual void Reserved1(void) = 0;
    virtual void Reserved2(void) = 0;
    virtual void Reserved3(void) = 0;
    virtual void Reserved4(void) = 0;
    virtual void Reserved5(void) = 0;
    virtual void Reserved6(void) = 0;
    virtual void Reserved7(void) = 0;
    virtual void Reserved8(void) = 0;
    virtual void Reserved9(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved11(void) = 0;
    virtual void Reserved12(void) = 0;
    virtual void Reserved13(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved15(void) = 0;
    virtual void Reserved16(void) = 0;
    virtual void Reserved17(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void Reserved19(void) = 0;
    virtual void Reserved20(void) = 0;
    virtual void Reserved21(void) = 0;
    virtual void Reserved22(void) = 0;
    virtual void Reserved23(void) = 0;
    virtual void Reserved24(void) = 0;
    virtual void Reserved25(void) = 0;
    virtual void Reserved26(void) = 0;
    virtual void Reserved27(void) = 0;
    virtual void Reserved28(void) = 0;
    virtual void Reserved29(void) = 0;
    virtual void Reserved30(void) = 0;
    virtual void Reserved31(void) = 0;
    virtual void Reserved32(void) = 0;
    virtual void Reserved33(void) = 0;
    virtual void Reserved34(void) = 0;
    virtual void Reserved35(void) = 0;
    virtual void Reserved36(void) = 0;
    virtual void Reserved37(void) = 0;
    virtual void Reserved38(void) = 0;
    virtual void Reserved39(void) = 0;
    virtual void Reserved40(void) = 0;
    virtual void Reserved41(void) = 0;
    virtual void Reserved42(void) = 0;
    virtual void ApplyWindowOwnerActivation(int nActivate) = 0;
    virtual void UpdateWindowOwnerRenderContext(void) = 0;
    virtual void PresentWindowOwnerRenderContext(void) = 0;
};

struct GamePrimaryContextWindowInitializationInterface {
    virtual void Reserved0(void) = 0;
    virtual void Reserved1(void) = 0;
    virtual void Reserved2(void) = 0;
    virtual void Reserved3(void) = 0;
    virtual void Reserved4(void) = 0;
    virtual void Reserved5(void) = 0;
    virtual void Reserved6(void) = 0;
    virtual void Reserved7(void) = 0;
    virtual void Reserved8(void) = 0;
    virtual void Reserved9(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved11(void) = 0;
    virtual void Reserved12(void) = 0;
    virtual void Reserved13(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved15(void) = 0;
    virtual void Reserved16(void) = 0;
    virtual void Reserved17(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void Reserved19(void) = 0;
    virtual void Reserved20(void) = 0;
    virtual void Reserved21(void) = 0;
    virtual void Reserved22(void) = 0;
    virtual void Reserved23(void) = 0;
    virtual void Reserved24(void) = 0;
    virtual void Reserved25(void) = 0;
    virtual void Reserved26(void) = 0;
    virtual void Reserved27(void) = 0;
    virtual void Reserved28(void) = 0;
    virtual void Reserved29(void) = 0;
    virtual void Reserved30(void) = 0;
    virtual void Reserved31(void) = 0;
    virtual void Reserved32(void) = 0;
    virtual void Reserved33(void) = 0;
    virtual void Reserved34(void) = 0;
    virtual void Reserved35(void) = 0;
    virtual void Reserved36(void) = 0;
    virtual void Reserved37(void) = 0;
    virtual void Reserved38(void) = 0;
    virtual void Reserved39(void) = 0;
    virtual void Reserved40(void) = 0;
    virtual void InitializeWindowOwnerFromRectAndActivate(short *paRect,
                                                            void *pWindowOwner,
                                                            const char *pszTitle,
                                                            int nActivate) = 0;
};

struct GameGeometryRectSubmitInterface {
    virtual void Reserved(void) = 0;
    virtual void SubmitRect(short *paRect) = 0;
};

struct GamePrimaryContextGeometryVirtualInterface {
    virtual void Reserved0(void) = 0;
    virtual void Reserved1(void) = 0;
    virtual void Reserved2(void) = 0;
    virtual void Reserved3(void) = 0;
    virtual void Reserved4(void) = 0;
    virtual void Reserved5(void) = 0;
    virtual void Reserved6(void) = 0;
    virtual void Reserved7(void) = 0;
    virtual void Reserved8(void) = 0;
    virtual void Reserved9(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved11(void) = 0;
    virtual void Reserved12(void) = 0;
    virtual void Reserved13(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved15(void) = 0;
    virtual void Reserved16(void) = 0;
    virtual void Reserved17(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void Reserved19(void) = 0;
    virtual void Reserved20(void) = 0;
    virtual void Reserved21(void) = 0;
    virtual void Reserved22(void) = 0;
    virtual void Reserved23(void) = 0;
    virtual void Reserved24(void) = 0;
    virtual void Reserved25(void) = 0;
    virtual void Reserved26(void) = 0;
    virtual void Reserved27(void) = 0;
    virtual void Reserved28(void) = 0;
    virtual void Reserved29(void) = 0;
    virtual void Reserved30(void) = 0;
    virtual void Reserved31(void) = 0;
    virtual void Reserved32(void) = 0;
    virtual void Reserved33(void) = 0;
    virtual void Reserved34(void) = 0;
    virtual void Reserved35(void) = 0;
    virtual void Reserved36(void) = 0;
    virtual void Reserved37(void) = 0;
    virtual void Reserved38(void) = 0;
    virtual void Reserved39(void) = 0;
    virtual void Reserved40(void) = 0;
    virtual void Reserved41(void) = 0;
    virtual void DispatchPrimaryContextActiveScreenFrame(void *pFrameArgument) = 0;
};

// FUNCTION: LEMBALL 0x00464190
void GamePrimaryContext::UpdateWindowOwnerRenderContext(void) {
    short aRect[4];
    void *pQueue;
    void *pChild;

    aRect[0] = 0;
    aRect[1] = 0;
    aRect[2] = 0;
    aRect[3] = 0;
    if (*(int *)((char *)this + 4) == 0 || *(int *)((char *)this + 0x34) == 0) {
        return;
    }

    ((GamePrimaryContextGeometryVirtualInterface *)this)
        ->DispatchPrimaryContextActiveScreenFrame(aRect);

    pQueue = *(void **)((char *)this + 0x4c);
    if (pQueue != 0) {
        DispatchAndClearPointerQueue(pQueue);
        *(int *)((char *)pQueue + 4) = 0;
    }

    pChild = *(void **)((char *)this + 0x24);
    while (pChild != 0) {
        ((GamePrimaryContextExtendedVirtualInterface *)*(void **)pChild)
            ->UpdateWindowOwnerRenderContext();
        pChild = *(void **)((char *)pChild + 4);
    }

    /* Cursor-client synchronization is intentionally deferred until the
     * cursor render-client body is reconstructed; the queue flush above is
     * still the target's required ownership boundary. */
}

// FUNCTION: LEMBALL 0x004642C0
void GamePrimaryContext::FlushWindowOwnerDirtyRect(int nUnused) {
    short cxDirty;
    short cyDirty;
    short xDirty;
    short yDirty;
    short cxOwner;
    short cyOwner;
    short xOwner;
    short yOwner;
    short aRect[4];
    void *pGeometryHelper;
    void *pGeometryContext;
    GamePrimaryContextExtendedVirtualInterface *pExtendedInterface;

    (void)nUnused;
    pExtendedInterface = (GamePrimaryContextExtendedVirtualInterface *)this;
    pExtendedInterface->UpdateWindowOwnerRenderContext();

    cxDirty = *(short *)((char *)this + 0x88);
    cyDirty = *(short *)((char *)this + 0x8a);
    if ((int)cxDirty * (int)cyDirty > 0) {
        aRect[0] = cxDirty;
        aRect[1] = cyDirty;
        xDirty = *(short *)((char *)this + 0x8c);
        yDirty = *(short *)((char *)this + 0x8e);
        aRect[2] = xDirty;
        aRect[3] = yDirty;

        cxOwner = *(short *)((char *)this + 0x10);
        cyOwner = *(short *)((char *)this + 0x12);
        xOwner = *(short *)((char *)this + 0x14);
        yOwner = *(short *)((char *)this + 0x16);
        if ((int)cxOwner * (int)cyOwner != 0) {
            if (aRect[2] < xOwner) {
                aRect[0] = (short)(aRect[0] + aRect[2] - xOwner);
                aRect[2] = xOwner;
            }
            if ((short)(xOwner + cxOwner) < (short)(aRect[2] + aRect[0])) {
                aRect[0] = (short)(xOwner + cxOwner - aRect[2]);
            }
            if (aRect[3] < yOwner) {
                aRect[1] = (short)(aRect[1] + aRect[3] - yOwner);
                aRect[3] = yOwner;
            }
            if ((short)(yOwner + cyOwner) < (short)(aRect[3] + aRect[1])) {
                aRect[1] = (short)(yOwner + cyOwner - aRect[3]);
            }
            if (aRect[0] <= 0 || aRect[1] <= 0) {
                aRect[0] = 0;
                aRect[1] = 0;
                aRect[2] = 0;
                aRect[3] = 0;
            }
        }

        if ((int)aRect[0] * (int)aRect[1] > 0) {
            pGeometryHelper = *(void **)((char *)this + 0x4c);
            if (pGeometryHelper != 0) {
                pGeometryContext = *(void **)((char *)pGeometryHelper + 0x0c);
                if (pGeometryContext != 0) {
                    xOwner = *(short *)((char *)this + 0x14);
                    yOwner = *(short *)((char *)this + 0x16);
                    aRect[2] = (short)(aRect[2] - xOwner);
                    aRect[3] = (short)(aRect[3] - yOwner);
                    ((GameGeometryRectSubmitInterface *)pGeometryContext)->SubmitRect(aRect);
                }
            }
        }
    }

    ((GamePrimaryContextExtendedVirtualInterface *)this)
        ->PresentWindowOwnerRenderContext();
    *(short *)((char *)this + 0x88) = 0;
    *(short *)((char *)this + 0x8a) = 0;
    *(short *)((char *)this + 0x8c) = 0;
    *(short *)((char *)this + 0x8e) = 0;
}

// FUNCTION: LEMBALL 0x004323C0
void VS_RootGeometryOwner::SetWindowOwnerState(int nState) {
    m_nWindowOwnerState30 = nState;
}

// FUNCTION: LEMBALL 0x004323B0
int VS_RootGeometryOwner::GetWindowOwnerState(void) {
    return m_nWindowOwnerState30;
}

// FUNCTION: LEMBALL 0x004662B0
void VS_RootGeometryOwner::SetWindowOwnerScaleFactor(int nScaleFactor) {
    int nPreviousScaleFactor;

    nPreviousScaleFactor = m_nScaleFactor38;
    if (nScaleFactor != nPreviousScaleFactor) {
        m_nScaleFactor38 = nScaleFactor;
        SetRootGeometryScaleFactor(nPreviousScaleFactor);
        ReservedWindowOwnerSlot22(nPreviousScaleFactor);
        CanonicalizeRootGeometrySubrect();
        ReservedWindowOwnerSlot17();
    }
}

void *GameScreenObject::ConstructIntroSequenceScreen(void *pPrimaryContext,
                                                      void *pWindowOwnerContext,
                                                      short *paRect,
                                                      int fSkipVariant) {
    void *pScreenObject = this;
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    (void)fSkipVariant;
    InitializeGameStubVtables();
    memset(pScreenObject, 0, 0x484);
    *(void **)pScreenObject = g_GAME_GenericScreenVtableSlots;
    return pScreenObject;
}

// FUNCTION: LEMBALL 0x00448200
void *GameScreenObject::ConstructMainMenuScreen(void *pPrimaryContext, void *pWindowOwnerContext, short *paRect) {
    GameLevelProgressState *pProgress;
    DWORD dwNow;
    char *pScreen;
    int nPack;

    pScreen = (char *)this;
    ConstructFramedScreenBaseFields(
        pScreen, 0, pPrimaryContext, pWindowOwnerContext, paRect,
        2, 0, 0, 0, 0, 0);
    *(void ***)(pScreen + 4) = g_GAME_MainMenuQueueVtable;
    *(void ***)(pScreen + 0x14) = g_GAME_MainMenuCallbackVtable;
    *(void ***)pScreen = g_GAME_MainMenuScreenVtable;

    dwNow = timeGetTime();
    *(int *)(pScreen + 0x3b0) = 0;
    *(DWORD *)(pScreen + 0x398) = dwNow + 20000U;
    *(int *)(pScreen + 0x3b8) = 0;
    *(int *)(pScreen + 0x3ac) = 0;
    *(int *)(pScreen + 0x3a8) = 0;
    *(int *)(pScreen + 0x3a0) = 0;
    *(int *)(pScreen + 0x3a4) = 0;

    pProgress = (GameLevelProgressState *)g_pLevelProgressState;
    nPack = pProgress->m_nCurrentPack;
    if (nPack == 0 || nPack == 4) {
        *(int *)(pScreen + 0x3b4) = 0;
    } else if (nPack < 4) {
        *(int *)(pScreen + 0x3b4) = nPack;
    }
    pProgress->m_nUnused4C = 0;
    *(int *)(pScreen + 0xa8) = 1;
    *(int *)(pScreen + 0xac) = 1;
    *(int *)(pScreen + 0xb0) = 1;
    ActivateFramedScreenBase(pScreen);
    return pScreen;
}

void *GameScreenObject::ConstructOptionsScreen(void *pPrimaryContext, void *pWindowOwnerContext, short *paRect) {
    void *pScreenObject = this;
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    InitializeGameStubVtables();
    memset(pScreenObject, 0, 0x3ac);
    *(void **)pScreenObject = g_GAME_GenericScreenVtableSlots;
    return pScreenObject;
}

void *GameScreenObject::ConstructLevelSelectionScreen(void *pPrimaryContext,
                                                       void *pWindowOwnerContext,
                                                       short *paRect) {
    void *pScreenObject = this;
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    InitializeGameStubVtables();
    memset(pScreenObject, 0, 0x488);
    *(void **)pScreenObject = g_GAME_GenericScreenVtableSlots;
    return pScreenObject;
}

void *GameScreenObject::ConstructLevelScreen(void *pPrimaryContext,
                                              void *pLevelModeObject,
                                              void *pWindowOwnerContext,
                                              int nStatusIndicatorMode,
                                              short *paRect) {
    void *pScreenObject = this;
    (void)pPrimaryContext;
    (void)pLevelModeObject;
    (void)pWindowOwnerContext;
    (void)nStatusIndicatorMode;
    (void)paRect;
    InitializeGameStubVtables();
    memset(pScreenObject, 0, 0x2428);
    *(void **)pScreenObject = g_GAME_GenericScreenVtableSlots;
    return pScreenObject;
}

void *GameScreenObject::ConstructRegistrationInfoScreen(void *pPrimaryContext,
                                                         void *pWindowOwnerContext,
                                                         short *paRect) {
    void *pScreenObject = this;
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    InitializeGameStubVtables();
    memset(pScreenObject, 0, 0x9c);
    *(void **)pScreenObject = g_GAME_GenericScreenVtableSlots;
    return pScreenObject;
}

void *GameScreenObject::ConstructNetworkLobbyScreen(void *pPrimaryContext,
                                                     void *pWindowOwnerContext,
                                                     short *paRect) {
    void *pScreenObject = this;
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    InitializeGameStubVtables();
    memset(pScreenObject, 0, 0x438);
    *(void **)pScreenObject = g_GAME_GenericScreenVtableSlots;
    return pScreenObject;
}

void *GameScreenObject::ConstructPaintballSequenceScreen(void *pPrimaryContext,
                                                          void *pWindowOwnerContext,
                                                          short *paRect,
                                                          int fFirstSequenceVariant) {
    void *pScreenObject = this;
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    (void)fFirstSequenceVariant;
    InitializeGameStubVtables();
    memset(pScreenObject, 0, 0x60c);
    *(void **)pScreenObject = g_GAME_GenericScreenVtableSlots;
    return pScreenObject;
}

void *GameScreenObject::ConstructPasswordEntryScreen(void *pPrimaryContext,
                                                      void *pWindowOwnerContext,
                                                      short *paRect) {
    void *pScreenObject = this;
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    InitializeGameStubVtables();
    memset(pScreenObject, 0, 0x494);
    *(void **)pScreenObject = g_GAME_GenericScreenVtableSlots;
    return pScreenObject;
}

static void CallNoArgVirtual(void *pObject, unsigned int nByteOffset) {
    void **pVtable;

    pVtable = *(void ***)pObject;
    ((void (LEMBALL_FASTCALL *)(void *))
         pVtable[nByteOffset / sizeof(void *)])(pObject);
}

static void CallDeleteVirtual(void *pObject, unsigned int nByteOffset, int nDeleteFlag) {
    void **pVtable;

    pVtable = *(void ***)pObject;
    ((void (LEMBALL_FASTCALL *)(void *, int, int))
         pVtable[nByteOffset / sizeof(void *)])(
        pObject, 0, nDeleteFlag);
}

static int CallNestedVirtualInt(void *pObject, unsigned int nObjectOffset, unsigned int nVtableOffset) {
    void *pSubobject;
    void **pVtable;

    pSubobject = *(void **)((char *)pObject + nObjectOffset);
    pVtable = *(void ***)pSubobject;
    return ((int (LEMBALL_FASTCALL *)(void *))
                pVtable[nVtableOffset / sizeof(void *)])(pSubobject);
}

static int CallIntVirtual(void *pObject, unsigned int nByteOffset) {
    void **pVtable;

    pVtable = *(void ***)pObject;
    return ((int (LEMBALL_FASTCALL *)(void *))
                pVtable[nByteOffset / sizeof(void *)])(pObject);
}

static void CallIntArgVirtual(void *pObject, unsigned int nByteOffset, int nArg) {
    void **pVtable;

    pVtable = *(void ***)pObject;
    ((void (LEMBALL_FASTCALL *)(void *, int, int))
         pVtable[nByteOffset / sizeof(void *)])(
        pObject, 0, nArg);
}

int LEMBALL_FASTCALL IsPrimaryContextWindowReadyForPresent(int *pPrimaryContext);
int LEMBALL_FASTCALL IsPrimaryContextWindowReadyForPresentThunk(int *pPrimaryContext);
void LEMBALL_FASTCALL PresentPrimaryContextScreen(void *pPrimaryContext);
void LEMBALL_FASTCALL UpdatePrimaryContextActiveScreen(void *pPrimaryContext);
int LEMBALL_FASTCALL PollPrimaryContextScreenStatus(void *pPrimaryContext);
int LEMBALL_FASTCALL GetPrimaryContextRequestedScreen(void *pPrimaryContext);
void LEMBALL_FASTCALL UpdateMainGameActiveMode(void *pMainContext);
void LEMBALL_FASTCALL PresentMainGameFrame(void *pMainContext);
void LEMBALL_FASTCALL RunMainGameLoop(GameMainContext *pMainContext);
extern FILE *OpenFileWithMode(const char *pszPath, const char *pszMode);
extern void CloseCrtFilePointer(FILE *pFile);

// FUNCTION: LEMBALL 0x0045ECB0
int SetVisualSciencesRegistryRunningState(const char *pszProductName, int fRunning) {
#if defined(LEMBALL_SKIP_REGISTRY_RUNNING_STATE)
    (void)pszProductName;
    (void)fRunning;
    return 1;
#else
    char szKeyPath[256];
    HKEY hRegistryKey;
    const char *pszState;
    unsigned int cbState;
    LSTATUS nRegistryStatus;

    strcpy(szKeyPath, g_GAME_RegistryRoot);
    strcat(szKeyPath, pszProductName);

    if (RegOpenKeyExA((HKEY)0x80000002, szKeyPath, 0, KEY_ALL_ACCESS, &hRegistryKey) != 0) {
        return 0;
    }

    pszState = g_GAME_RunningState;
    if (!fRunning) {
        pszState = g_GAME_NotRunningState;
    }

    cbState = (unsigned int)strlen(pszState) + 1;

    nRegistryStatus = RegSetValueExA(hRegistryKey, g_GAME_RunningValueName, 0, 1,
                                     (const BYTE *)pszState, cbState);
    RegCloseKey(hRegistryKey);
    return nRegistryStatus == 0;
#endif
}

struct GameModeVirtualInterface {
    virtual void Delete(int nDeleteFlag) = 0;
    virtual void Update(void) = 0;
};

struct GameNetworkRuntimeVirtualInterface {
    virtual void Reserved(void) = 0;
    virtual void Close(int nCloseFlag) = 0;
};

struct GameWindowOwnerVirtualInterface {
    virtual void Reserved0(void) = 0;
    virtual void Reserved1(void) = 0;
    virtual int GetCurrentWindowOwnerState(void) = 0;
};

struct GameWindowOwnerContext {
    char m_Reserved00[0x0c];
    GameWindowOwnerVirtualInterface *m_pStateSubobject;
};

struct GameActiveScreenVirtualInterface {
    virtual void Reserved0(void) = 0;
    virtual void Reserved1(void) = 0;
    virtual void Reserved2(void) = 0;
    virtual void Reserved3(void) = 0;
    virtual void Reserved4(void) = 0;
    virtual void ForwardPrimaryContext(void *pPrimaryContextSubobject) = 0;
    virtual void ReleaseActiveScreen(void) = 0;
    virtual void UpdateActiveScreen(void) = 0;
    virtual void PrepareForPresent(void) = 0;
    virtual void FinalizePresent(void) = 0;
    virtual int GetRequestedScreen(void) = 0;
    virtual int GetTransitionStatus(void) = 0;
};

struct GameActiveScreenSlot0x0CInterface {
    virtual void Reserved0(void) = 0;
    virtual void Reserved1(void) = 0;
    virtual void Reserved2(void) = 0;
    virtual void ForwardPrimaryContext(void *pPrimaryContextSubobject) = 0;
};

struct GameActiveScreenSlot0x10Interface {
    virtual void Reserved0(void) = 0;
    virtual void Reserved1(void) = 0;
    virtual void Reserved2(void) = 0;
    virtual void Reserved3(void) = 0;
    virtual void ForwardPrimaryContext(void *pPrimaryContextSubobject) = 0;
};

struct GameActiveScreenSlot0x30Interface {
    virtual void Reserved0(void) = 0;
    virtual void Reserved1(void) = 0;
    virtual void Reserved2(void) = 0;
    virtual void Reserved3(void) = 0;
    virtual void Reserved4(void) = 0;
    virtual void Reserved5(void) = 0;
    virtual void Reserved6(void) = 0;
    virtual void Reserved7(void) = 0;
    virtual void Reserved8(void) = 0;
    virtual void Reserved9(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved11(void) = 0;
    virtual void ForwardSlot0x30(void) = 0;
};

// FUNCTION: LEMBALL 0x004317C0
void LEMBALL_FASTCALL DestroyPrimaryContextActiveScreenForVtable(
    void *pPrimaryContext) {
    void *pActiveScreen;

    SetLevelScreenStatusIndicatorMode(0, 0);
    pActiveScreen = *(void **)((char *)pPrimaryContext + 0xd4);
    if (pActiveScreen != 0) {
        ((GameActiveScreenVirtualInterface *)pActiveScreen)->Reserved1();
        ((GameActiveScreenVirtualInterface *)
             *(void **)((char *)pPrimaryContext + 0xd4))
            ->ReleaseActiveScreen();
        pActiveScreen = *(void **)((char *)pPrimaryContext + 0xd4);
        if (pActiveScreen != 0) {
            ((GameModeVirtualInterface *)pActiveScreen)->Delete(1);
        }
        *(void **)((char *)pPrimaryContext + 0xd4) = 0;
    }
}

// FUNCTION: LEMBALL 0x004322D0
void LEMBALL_FASTCALL DispatchPrimaryContextSlot23Thunk(
    void *pPrimaryContext) {
    ((VS_RootGeometryOwner *)pPrimaryContext)
        ->ReservedWindowOwnerSlot23();
}

// FUNCTION: LEMBALL 0x00431880
void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x0C(
    void *pPrimaryContext, int, int) {
    void *pActiveScreen;

    pActiveScreen = *(void **)((char *)pPrimaryContext + 0xd4);
    if (pActiveScreen != 0) {
        ((GameActiveScreenSlot0x0CInterface *)pActiveScreen)
            ->ForwardPrimaryContext((char *)pPrimaryContext + 8);
    }
}

// FUNCTION: LEMBALL 0x00431ED0
void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x30(
    void *pPrimaryContext) {
    void *pActiveScreen;

    pActiveScreen = *(void **)((char *)pPrimaryContext + 0xd4);
    if (pActiveScreen != 0) {
        ((GameActiveScreenSlot0x30Interface *)pActiveScreen)
            ->ForwardSlot0x30();
    }
}

struct GamePrimaryContextVirtualInterface {
    virtual void Reserved0(void) = 0;
    virtual void Reserved1(void) = 0;
    virtual void Reserved2(void) = 0;
    virtual void Reserved3(void) = 0;
    virtual void Reserved4(void) = 0;
    virtual void Reserved5(void) = 0;
    virtual void Reserved6(void) = 0;
    virtual void Reserved7(void) = 0;
    virtual void Reserved8(void) = 0;
    virtual void Reserved9(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved11(void) = 0;
    virtual void Reserved12(void) = 0;
    virtual void Reserved13(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved15(void) = 0;
    virtual void Reserved16(void) = 0;
    virtual void Reserved17(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void Reserved19(void) = 0;
    virtual void Reserved20(void) = 0;
    virtual void Reserved21(void) = 0;
    virtual void Reserved22(void) = 0;
    virtual void Reserved23(void) = 0;
    virtual void Reserved24(void) = 0;
    virtual void Reserved25(void) = 0;
    virtual int GetWindowOwnerState(void) = 0;
    virtual void SetWindowOwnerState(int nState) = 0;
    virtual void SetPresentMode(int nMode) = 0;
};

// FUNCTION: LEMBALL 0x00464440
void GamePrimaryContext::InitializeWindowOwnerFromRectAndActivate(
    short *paRect, void *pWindowOwner, const char *pszTitle, int nActivate)
{
    VS_WindowOwnerBase::InitializeWindowOwnerFromRect(
        paRect, pWindowOwner, pszTitle);
    ((GamePrimaryContextExtendedVirtualInterface *)this)
        ->ApplyWindowOwnerActivation(nActivate);
}

// FUNCTION: LEMBALL 0x00464470
void GamePrimaryContext::DispatchWindowOwnerRectInitialization(
    short *paRect, void *pWindowOwner, const char *pszTitle)
{
    ((GamePrimaryContextWindowInitializationInterface *)this)
        ->InitializeWindowOwnerFromRectAndActivate(paRect, pWindowOwner, pszTitle, 0);
}

struct GamePaletteResourceVirtualInterface {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved0C(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void PreparePaletteResource(void) = 0;
};

struct VsGdiPaletteTargetVirtualInterface {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved0C(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void Reserved1C(void) = 0;
    virtual void Reserved20(void) = 0;
    virtual void Reserved24(void) = 0;
    virtual void Reserved28(void) = 0;
    virtual void Reserved2C(void) = 0;
    virtual void ApplyPaletteResource(void *pPalette) = 0;
};

// FUNCTION: LEMBALL 0x00464490
void GamePrimaryContext::ApplyWindowOwnerPaletteResource(int nResourceId) {
    int *pPalette;
    int nBackendContext;
    VsGdiPaletteTargetVirtualInterface *pPaletteTarget;

    if (nResourceId == 0) {
        return;
    }

    pPalette = (int *)LoadPalResource(nResourceId);
    if (pPalette[4] != 0) {
        pPalette[9] = 0;
    } else {
        ((GamePaletteResourceVirtualInterface *)pPalette)
            ->PreparePaletteResource();
    }

    ++pPalette[2];
    nBackendContext = *(int *)(*(int *)((char *)this + 0x4c) + 0x0c);
    pPaletteTarget = (VsGdiPaletteTargetVirtualInterface *)(
        *(int *)(*(int *)(nBackendContext + 0x40) + 4) +
        nBackendContext + 0x40);
    pPaletteTarget->ApplyPaletteResource(pPalette);
    --pPalette[2];
    ReleaseTypedResourceObjectReference(pPalette);
    *(int *)((char *)this + 0x54) = nResourceId;
}

struct VS_QueuedRenderSinkValueNode {
    void *m_pValue;
    VS_QueuedRenderSinkValueNode *m_pNext;
    VS_QueuedRenderSinkValueNode *m_pPrevious;

    VS_QueuedRenderSinkValueNode(void *pValue)
        : m_pValue(pValue), m_pNext(0), m_pPrevious(0) {}
};

// FUNCTION: LEMBALL 0x004323D0
void VS_RootGeometryOwner::AppendQueuedRenderSinkValueNode(void *pValue) {
    VS_QueuedRenderSinkValueNode *pNode;
    VS_QueuedRenderSinkValueNode *pTail;

    pNode = new (AllocateVSMemBlock(sizeof(*pNode)))
        VS_QueuedRenderSinkValueNode(pValue);

    pNode->m_pPrevious = *(VS_QueuedRenderSinkValueNode **)((char *)this + 0x28);
    pTail = *(VS_QueuedRenderSinkValueNode **)((char *)this + 0x28);
    if (pTail != 0) {
        pTail->m_pNext = pNode;
    }
    *(VS_QueuedRenderSinkValueNode **)((char *)this + 0x28) = pNode;
    if (*(unsigned int *)((char *)this + 0x24) == 0) {
        *(VS_QueuedRenderSinkValueNode **)((char *)this + 0x24) = pNode;
    }
    ++*(unsigned int *)((char *)this + 0x2c);
}

// FUNCTION: LEMBALL 0x00432430
void VS_RootGeometryOwner::RemoveQueuedRenderSinkValueNode(void *pValue) {
    unsigned int *pNode;
    unsigned int *pNext;
    unsigned int *pPrevious;

    pNode = *(unsigned int **)((char *)this + 0x24);
    while (pNode != 0 && pNode[0] != (unsigned int)(unsigned long)pValue) {
        pNode = (unsigned int *)pNode[1];
    }
    if (pNode == 0) {
        return;
    }

    pNext = (unsigned int *)pNode[1];
    pPrevious = (unsigned int *)pNode[2];
    FreeVSMemBlock(pNode);
    if (pNext == 0) {
        *(unsigned int **)((char *)this + 0x28) = pPrevious;
    } else {
        pNext[2] = (unsigned int)(unsigned long)pPrevious;
    }
    if (pPrevious == 0) {
        *(unsigned int **)((char *)this + 0x24) = pNext;
    } else {
        pPrevious[1] = (unsigned int)(unsigned long)pNext;
    }
    --*(unsigned int *)((char *)this + 0x2c);
}

// FUNCTION: LEMBALL 0x004324A0
void VS_WindowOwnerBase::CacheCurrentContextWindow(void) {
    extern void *g_pCachedCurrentContextWindow;
    extern void *g_pCachedCurrentContextOwner;

    g_pCachedCurrentContextWindow = *(void **)((char *)this + 0x44);
    g_pCachedCurrentContextOwner = this;
}

// FUNCTION: LEMBALL 0x004324C0
void VS_WindowOwnerBase::ReservedWindowOwnerSlot37(void) {
}

// FUNCTION: LEMBALL 0x004324D0
int VS_WindowOwnerBase::IsCurrentWindowOwnerContext(void) {
    extern void *g_pCachedCurrentContextWindow;

    return *(void **)((char *)this + 0x44) ==
           g_pCachedCurrentContextWindow;
}

// FUNCTION: LEMBALL 0x004324F0
void VS_WindowOwnerBase::ReservedWindowOwnerSlot19(void) {
}

// FUNCTION: LEMBALL 0x00432500
void VS_WindowOwnerBase::ReservedWindowOwnerSlot20(void) {
}

// FUNCTION: LEMBALL 0x00432510
void VS_WindowOwnerBase::ReservedWindowOwnerSlot38(void) {
}

// FUNCTION: LEMBALL 0x00432520
void VS_WindowOwnerBase::ReservedWindowOwnerSlot39(void) {
}

struct GamePrimaryContextActiveScreenFrameInterface {
    virtual void Reserved0(void) = 0;
    virtual void Reserved1(void) = 0;
    virtual void ForwardFrameArgument(void *pFrameArgument) = 0;
    virtual void Reserved3(void) = 0;
    virtual void Reserved4(void) = 0;
    virtual void Reserved5(void) = 0;
    virtual void Reserved6(void) = 0;
    virtual void Reserved7(void) = 0;
    virtual void RenderFrame(void) = 0;
};

// FUNCTION: LEMBALL 0x00431810
void GamePrimaryContext::DispatchPrimaryContextActiveScreenFrame(
    void *pFrameArgument)
{
    void *pActiveScreen;

    if (m_pWindowOwnerContext == 0) {
        return;
    }

    if (IsPrimaryContextWindowReadyForPresentThunk((int *)this) == 0) {
        return;
    }

    if (m_nA4 != 0) {
        ((GamePrimaryContextActiveScreenFrameInterface *)(unsigned long)m_nA4)
            ->Reserved0();
    }

    pActiveScreen = *(void **)((char *)this + 0xd4);
    if (pActiveScreen != 0) {
        ((GamePrimaryContextActiveScreenFrameInterface *)pActiveScreen)
            ->ForwardFrameArgument(pFrameArgument);
    }

    pActiveScreen = *(void **)((char *)this + 0xd4);
    if (pActiveScreen != 0) {
        ++m_nB8;
        ((GamePrimaryContextActiveScreenFrameInterface *)pActiveScreen)
            ->RenderFrame();
    }
}

struct GameActiveScreenPresentInterface {
    virtual void Reserved0(void) = 0;
    virtual void Reserved1(void) = 0;
    virtual void Reserved2(void) = 0;
    virtual void Reserved3(void) = 0;
    virtual void Reserved4(void) = 0;
    virtual void Reserved5(void) = 0;
    virtual void Reserved6(void) = 0;
    virtual void Reserved7(void) = 0;
    virtual void Reserved8(void) = 0;
    virtual void Reserved9(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved11(void) = 0;
    virtual void Reserved12(void) = 0;
    virtual void Present(void) = 0;
};

struct GameActiveScreenDeleteInterface {
    virtual void Delete(int nDeleteFlag) = 0;
};

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

static char g_GAME_CdromDrivePath[256] = "A:\\";

// FUNCTION: LEMBALL 0x0045EDA0
char *FindCdromFilePathBySuffix(const char *pszSuffix) {
#if defined(LEMBALL_SKIP_CD_TEST)
    (void)pszSuffix;
    if (GetCurrentDirectoryA(sizeof(g_GAME_CdromDrivePath), g_GAME_CdromDrivePath) == 0)
        return 0;
    if (g_GAME_CdromDrivePath[strlen(g_GAME_CdromDrivePath) - 1] != '\\')
        strcat(g_GAME_CdromDrivePath, "\\");
    return g_GAME_CdromDrivePath;
#else
    char szCandidatePath[256];
    DWORD dwDrives;
    char chDrive;
    int i;

    chDrive = 'A';
    dwDrives = GetLogicalDrives();
    strcpy(szCandidatePath, g_GAME_CdromDrivePath);
    strcat(szCandidatePath, pszSuffix);
    i = 0;

    while (i < 32) {
        if ((dwDrives & 1u) != 0) {
            szCandidatePath[0] = chDrive;
            g_GAME_CdromDrivePath[0] = chDrive;
            if (GetDriveTypeA(g_GAME_CdromDrivePath) == DRIVE_CDROM) {
                FILE *pFile;

                pFile = OpenFileWithMode(szCandidatePath, "rb");
                if (pFile != 0) {
                    CloseCrtFilePointer(pFile);
                    return g_GAME_CdromDrivePath;
                }
            }
        }

        dwDrives >>= 1;
        ++chDrive;
        ++i;
    }

    return 0;
#endif
}

GameMainContext::GameMainContext(void) {
    m_szLevelPath[0] = '\0';
    m_pProcessingStatus = 0;
    m_pRefreshingStatus = 0;
    m_nModeTicks = 0;
    m_pActiveMode = 0;
    m_fQuitRequested = 0;
    m_pPrimaryContext = 0;
    m_nActiveMode = 0;
    m_pVariantMode = 0;
}

// FUNCTION: LEMBALL 0x00431F30
short *GamePrimaryContext::ComputePrimaryContextCenteredScreenRect(short *paRect,
                                                                     int nLeftOverride,
                                                                     int nTopOverride)
{
    short cxScreen;
    short cyScreen;
    short cxTarget;
    short cyTarget;
    short xLeft;
    short yTop;

    g_fCompactPrimaryContextLayout = GetSelectedGraphicsDriverId() == 3;
    if (GetSelectedGraphicsDriverId() == 3) {
        cxTarget = m_cxCompactLayout;
        cyTarget = m_cyCompactLayout;
    } else {
        cxTarget = m_cxWideLayout;
        cyTarget = m_cyWideLayout;
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
    return paRect;
}

// FUNCTION: LEMBALL 0x00401875
short *GamePrimaryContext::ComputePrimaryContextCenteredScreenRectThunk(short *paRect,
                                                                          int nLeftOverride,
                                                                          int nTopOverride)
{
    return ComputePrimaryContextCenteredScreenRect(paRect, nLeftOverride, nTopOverride);
}

// FUNCTION: LEMBALL 0x004010F0
GamePrimaryContext *GamePrimaryContext::ConstructPrimaryContextThunk(GameMainContext *pMainContext) {
    return ConstructPrimaryContext(pMainContext);
}

// FUNCTION: LEMBALL 0x00431590
GamePrimaryContext *GamePrimaryContext::ConstructPrimaryContext(GameMainContext *pMainContext) {
    GamePrimaryContext *pContext;
    void *pRenderQueueNode;

    pContext = this;
    InitializeGameStubVtables();
    pRenderQueueNode = (char *)pContext + 0x90;
    ConstructWindowOwnerRenderContext(this);
    InstallPrimaryContextInheritedWindowOwnerSlots(*(void ***)this);
    InitializeRenderQueueNodeBase(pRenderQueueNode);

    m_pFinalizeThunk = g_pQueuedRenderPointSinkFinalizeThunk;
    m_cyCompactLayout = 0;
    *(void **)this = g_pPrimaryContextVtable;
    m_cxCompactLayout = 0;
    m_pRenderQueueNodeVtable = g_pPrimaryContextRenderQueueNodeVtable;
    m_cyWideLayout = 0;
    *(int *)((char *)this + 200) = 0;
    m_cxWideLayout = 0;
    m_nA4 = 0;
    m_nB8 = 0;
    m_pMainContext = pMainContext;
    m_pActiveLevelMode = 0;
    m_fA0 = 1;
    m_nLevelScreenStatusIndicatorMode = 0;
    m_pActiveScreen = 0;
    m_fScreenLayoutDirty = 1;
    m_nFrameInterval = 600;
    m_nActiveScreenMode = 3;
    m_pBackgroundZrle = LoadZrleResource(0xfc);
    m_pPalette2E = LoadPalResource(0x2e);
    m_pPalette2F = LoadPalResource(0x2f);

    SetLevelScreenStatusIndicatorMode(2, 0);
    ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
        ->RegisterOrderedRenderDispatchClient(
            this != 0 ? pRenderQueueNode : 0, -0x19);

    m_cxCompactLayout = 0x140;
    m_cyCompactLayout = 0xf0;
    m_cxWideLayout = 0x280;
    m_cyWideLayout = 0x1e0;
    m_fCompactLayout = g_fCompactPrimaryContextLayout;
    return this;
}

// FUNCTION: LEMBALL 0x00431950
void GamePrimaryContext::DestroyPrimaryContextActiveScreen(int) {
    m_fScreenLayoutDirty = 1;
    if (m_pActiveScreen != 0) {
        ((GameActiveScreenVirtualInterface *)m_pActiveScreen)
            ->ReleaseActiveScreen();
        if (m_pActiveScreen != 0) {
            ((GameActiveScreenDeleteInterface *)m_pActiveScreen)->Delete(1);
        }
        m_pActiveScreen = 0;
    }
}

// FUNCTION: LEMBALL 0x00431990
void GamePrimaryContext::SwitchPrimaryContextScreen(int nMode) {
    GamePrimaryContext *pContext;
    void *pScreenObject;
    short aRect[4];
    int nUploadState;
    void *pActiveLevelMode;
    GameWindowOwnerContext *pWindowOwnerContext;

    pContext = this;
    if (pContext->m_nActiveScreenMode == nMode) {
        return;
    }

    pContext->SetWindowOwnerScaleFactor(1);
    aRect[0] = *(short *)((char *)pContext + 8);
    pWindowOwnerContext =
        (GameWindowOwnerContext *)pContext->m_pWindowOwnerContext;
    aRect[1] = *(short *)((char *)pContext + 10);
    aRect[2] = 0;
    aRect[3] = 0;

    nUploadState = pWindowOwnerContext->m_pStateSubobject
        ->GetCurrentWindowOwnerState();
    InitializeHelperUploadStatePending(nUploadState);
    PromoteHelperUploadStateToActive(nUploadState);

    pContext->m_nActiveScreenMode = nMode;
    switch (nMode) {
    case 1:
        pScreenObject = AllocateVSMemBlock(0x484);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GameScreenObject *)pScreenObject)->ConstructIntroSequenceScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect, 0);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 2:
        pScreenObject = AllocateVSMemBlock(0x3bc);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GameScreenObject *)pScreenObject)->ConstructMainMenuScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 3:
        pScreenObject = AllocateVSMemBlock(0x3ac);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GameScreenObject *)pScreenObject)->ConstructOptionsScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 4:
        pScreenObject = AllocateVSMemBlock(0x488);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GameScreenObject *)pScreenObject)->ConstructLevelSelectionScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 5:
    case 0x13:
        if (pContext->m_pMainContext->m_pActiveMode != 0) {
            pContext->m_pActiveLevelMode =
                (char *)pContext->m_pMainContext->m_pActiveMode - 0x10;
        } else {
            pContext->m_pActiveLevelMode = 0;
        }
        pActiveLevelMode = pContext->m_pMainContext->m_pActiveMode;
        if (pActiveLevelMode != 0) {
            pActiveLevelMode = (char *)pActiveLevelMode - 0x10;
        }
        pContext->m_nLevelScreenStatusIndicatorMode =
            *(int *)((char *)pActiveLevelMode + 0x110);
        pScreenObject = AllocateVSMemBlock(0x2428);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen = ((GameScreenObject *)pScreenObject)->ConstructLevelScreen(
                pContext,
                pContext->m_pActiveLevelMode,
                pContext->m_pWindowOwnerContext,
                pContext->m_nLevelScreenStatusIndicatorMode,
                aRect);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 10:
        pScreenObject = AllocateVSMemBlock(0x9c);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GameScreenObject *)pScreenObject)->ConstructRegistrationInfoScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 0x0c:
        pScreenObject = AllocateVSMemBlock(0x438);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GameScreenObject *)pScreenObject)->ConstructNetworkLobbyScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 0x0e:
        pScreenObject = AllocateVSMemBlock(0x60c);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GameScreenObject *)pScreenObject)->ConstructPaintballSequenceScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect, 1);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 0x0f:
        pScreenObject = AllocateVSMemBlock(0x60c);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GameScreenObject *)pScreenObject)->ConstructPaintballSequenceScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect, 0);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 0x10:
        pScreenObject = AllocateVSMemBlock(0x494);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GameScreenObject *)pScreenObject)->ConstructPasswordEntryScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 0x12:
        pScreenObject = AllocateVSMemBlock(0x484);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GameScreenObject *)pScreenObject)->ConstructIntroSequenceScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect, 1);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    default:
        break;
    }

    if (pContext->m_pActiveScreen != 0) {
        ((GameActiveScreenVirtualInterface *)pContext->m_pActiveScreen)->FinalizePresent();
        ((GameActiveScreenVirtualInterface *)pContext->m_pActiveScreen)->PrepareForPresent();
    }
}

// FUNCTION: LEMBALL 0x004318C0
int LEMBALL_FASTCALL IsPrimaryContextWindowReadyForPresent(int *pPrimaryContext) {
    short nHeight;
    short nWidth;

    if (!((GamePrimaryContextVirtualInterface *)pPrimaryContext)->GetWindowOwnerState()) {
        return 0;
    }

    nHeight = *(short *)((char *)pPrimaryContext + 10);
    nWidth = *(short *)((char *)pPrimaryContext + 8);
    if (*(short *)((char *)pPrimaryContext + 0xdc) == nWidth &&
        *(short *)((char *)pPrimaryContext + 0xde) == nHeight) {
        return 1;
    }

    if (*(short *)((char *)pPrimaryContext + 0xe0) == nWidth &&
        *(short *)((char *)pPrimaryContext + 0xe2) == nHeight) {
        return 1;
    }

    return 0;
}

// FUNCTION: LEMBALL 0x00402630
int LEMBALL_FASTCALL IsPrimaryContextWindowReadyForPresentThunk(int *pPrimaryContext) {
    return IsPrimaryContextWindowReadyForPresent(pPrimaryContext);
}

// FUNCTION: LEMBALL 0x00431860
void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x10(void *pPrimaryContext) {
    void *pActiveScreen;

    pActiveScreen = *(void **)((char *)pPrimaryContext + 0xd4);
    if (pActiveScreen != 0) {
        ((GameActiveScreenSlot0x10Interface *)pActiveScreen)
            ->ForwardPrimaryContext((char *)pPrimaryContext + 8);
    }
}

// FUNCTION: LEMBALL 0x004318A0
void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x14(void *pPrimaryContext) {
    void *pActiveScreen;

    pActiveScreen = *(void **)((char *)pPrimaryContext + 0xd4);
    if (pActiveScreen != 0) {
        ((GameActiveScreenVirtualInterface *)pActiveScreen)
            ->ForwardPrimaryContext((char *)pPrimaryContext + 8);
    }
}

// FUNCTION: LEMBALL 0x00431910
void LEMBALL_FASTCALL PresentPrimaryContextScreen(void *pPrimaryContext) {
    int *pContext;
    void *pActiveScreen;

    pContext = (int *)pPrimaryContext;
    if (IsPrimaryContextWindowReadyForPresentThunk(pContext) != 0) {
        if (pContext[1] == 1) {
            ((GamePrimaryContextVirtualInterface *)pContext)->SetPresentMode(0);
        }
        pActiveScreen = *(void **)((char *)pPrimaryContext + 0xd4);
        if (pActiveScreen != 0) {
            ((GameActiveScreenPresentInterface *)pActiveScreen)->Present();
        }
    }
}

// FUNCTION: LEMBALL 0x00431940
void LEMBALL_FASTCALL UpdatePrimaryContextActiveScreen(void *pPrimaryContext) {
    void *pActiveScreen;

    pActiveScreen = *(void **)((char *)pPrimaryContext + 0xd4);
    if (pActiveScreen != 0) {
        ((GameActiveScreenVirtualInterface *)pActiveScreen)->UpdateActiveScreen();
    }
}

// FUNCTION: LEMBALL 0x00431EE0
int LEMBALL_FASTCALL PollPrimaryContextScreenStatus(void *pPrimaryContext) {
    GameActiveScreenVirtualInterface *pActiveScreen;

    if (*(int *)((char *)pPrimaryContext + 0xc8) != 0) {
        return 2;
    }
    pActiveScreen = *(GameActiveScreenVirtualInterface **)
        ((char *)pPrimaryContext + 0xd4);
    if (pActiveScreen != 0) {
        return pActiveScreen->GetTransitionStatus();
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00431F10
int LEMBALL_FASTCALL GetPrimaryContextRequestedScreen(void *pPrimaryContext) {
    GameActiveScreenVirtualInterface *pActiveScreen;

    if (*(int *)((char *)pPrimaryContext + 0xc8) == 0) {
        pActiveScreen = *(GameActiveScreenVirtualInterface **)
            ((char *)pPrimaryContext + 0xd4);
        if (pActiveScreen != 0) {
            return pActiveScreen->GetRequestedScreen();
        }
    }
    return 0;
}

// FUNCTION: LEMBALL 0x004077E0
void LEMBALL_FASTCALL UpdateMainGameActiveMode(void *pMainGameContext) {
    GameMainContext *pMainContext;
    GameStatusEntry *pProcessingStatus;
    int fTimedStatus;
    DWORD dwNow;
    int nScreenStatus;

    pMainContext = (GameMainContext *)pMainGameContext;
    UpdatePrimaryContextActiveScreen(pMainContext->m_pPrimaryContext);
    if (pMainContext->m_pActiveMode != 0) {
        fTimedStatus = 0;
        if ((pMainContext->m_nActiveMode == 5 || pMainContext->m_nActiveMode == 0x13) &&
            0x32 < pMainContext->m_nModeTicks) {
            fTimedStatus = 1;
            pProcessingStatus = pMainContext->m_pProcessingStatus;
            dwNow = timeGetTime();
            *(DWORD *)((char *)pProcessingStatus + 0x20) = dwNow;
            *(int *)((char *)pProcessingStatus + 0x24) = 1;
        }

        ((GameModeVirtualInterface *)pMainContext->m_pActiveMode)->Update();

        pProcessingStatus = pMainContext->m_pProcessingStatus;
        if (fTimedStatus != 0 && *(int *)((char *)pProcessingStatus + 0x24) != 0) {
            dwNow = timeGetTime();
            ((void (*)(DWORD))(*(void ***)pProcessingStatus)[1])(
                dwNow - *(DWORD *)((char *)pProcessingStatus + 0x20));
            *(int *)((char *)pProcessingStatus + 0x24) = 0;
        }

        if (g_pNetworkLobbyVsnetRuntime != 0) {
            ServiceNetworkLobbySelectedPeerUpdates(g_pNetworkLobbyVsnetRuntime);
        }

        nScreenStatus = *(int *)((char *)pMainContext->m_pActiveMode + 8);
        switch (nScreenStatus) {
        case 1:
            pMainContext->SwitchMainGameMode(*(int *)((char *)pMainContext->m_pActiveMode + 4));
            break;
        case 2:
            pMainContext->m_fQuitRequested = 1;
            break;
        }
    }

    nScreenStatus = PollPrimaryContextScreenStatus(pMainContext->m_pPrimaryContext);
    switch (nScreenStatus) {
    case 1:
        pMainContext->SwitchMainGameMode(GetPrimaryContextRequestedScreen(pMainContext->m_pPrimaryContext));
        break;
    case 2:
        pMainContext->m_fQuitRequested = 1;
        break;
    }

    if (pMainContext->m_fQuitRequested != 0) {
        if (((GamePrimaryContext *)pMainContext->m_pPrimaryContext)->m_pActiveScreen != 0) {
            ((GameModeVirtualInterface *)
                ((GamePrimaryContext *)pMainContext->m_pPrimaryContext)->m_pActiveScreen)->Update();
        }
        if (pMainContext->m_pActiveMode != 0) {
            ((GameModeVirtualInterface *)pMainContext->m_pActiveMode)->Delete(1);
            pMainContext->m_pActiveMode = 0;
        }
        ((GamePrimaryContext *)pMainContext->m_pPrimaryContext)
            ->DestroyPrimaryContextActiveScreen(0);
    }
}

// FUNCTION: LEMBALL 0x004078F0
void LEMBALL_FASTCALL PresentMainGameFrame(void *pMainGameContext) {
    GameMainContext *pMainContext;
    GameStatusEntry *pRefreshingStatus;
    int fTimedStatus;
    DWORD dwNow;

    pMainContext = (GameMainContext *)pMainGameContext;
    fTimedStatus = 0;
    if ((pMainContext->m_nActiveMode == 5 || pMainContext->m_nActiveMode == 0x13) &&
        0x32 < pMainContext->m_nModeTicks) {
        fTimedStatus = 1;
        pRefreshingStatus = pMainContext->m_pRefreshingStatus;
        dwNow = timeGetTime();
        *(DWORD *)((char *)pRefreshingStatus + 0x20) = dwNow;
        *(int *)((char *)pRefreshingStatus + 0x24) = 1;
    }

    PresentPrimaryContextScreen(pMainContext->m_pPrimaryContext);

    pRefreshingStatus = pMainContext->m_pRefreshingStatus;
    if (fTimedStatus != 0 && *(int *)((char *)pRefreshingStatus + 0x24) != 0) {
        dwNow = timeGetTime();
        ((void (LEMBALL_FASTCALL *)(void *, int, DWORD))(*(void ***)pRefreshingStatus)[1])(
            pRefreshingStatus, 0, dwNow - *(DWORD *)((char *)pRefreshingStatus + 0x20));
        *(int *)((char *)pRefreshingStatus + 0x24) = 0;
    }
}

// FUNCTION: LEMBALL 0x00407950
void LEMBALL_FASTCALL RunMainGameLoop(GameMainContext *pMainContext) {
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
GameMainContext *GameMainContext::InitializeMainGameContext(const char *pszCmdLine) {
    char *pszCdromPath;
    int nCdromResponse;
    GameMainContext *pMainContext;
    GameStatusEntry *pProcessingStatusEntry;
    GameStatusEntry *pRefreshingStatusEntry;
    void *pPrimaryStorage;

    pMainContext = this;

    pMainContext->m_pVariantMode = 0;
    g_pLevelProgressState = 0;
    pMainContext->m_pPrimaryContext = 0;
    g_pMainResourceArchive = 0;
    g_pVariantResourceEntryManager = 0;
    pMainContext->m_fQuitRequested = 1;
    pMainContext->m_pActiveMode = 0;
    if (!SetVisualSciencesRegistryRunningState(g_GAME_WindowTitle, 1)) {
        MessageBoxA(0, g_GAME_InstallPrompt, g_GAME_NotInstalledTitle, 0);
        return pMainContext;
    }

#if defined(LEMBALL_SKIP_CD_TEST)
    nCdromResponse = 0;
#endif
    do {
        pszCdromPath = FindCdromFilePathBySuffix(g_GAME_VsmemDllName);
        if (pszCdromPath != 0) {
            break;
        }
        nCdromResponse = MessageBoxA(0, g_GAME_CdromErrorText, g_GAME_CdromErrorTitle, 1);
    } while (nCdromResponse == 1);
    if (nCdromResponse == 2) {
        return pMainContext;
    }

    g_pLevelProgressState = AllocateVSMemBlock(0x50);
    if (g_pLevelProgressState != 0) {
        g_pLevelProgressState = ((GameLevelProgressState *)g_pLevelProgressState)->Clear();
    }

    pProcessingStatusEntry = (GameStatusEntry *)AllocateVSMemBlock(0x28);
    if (pProcessingStatusEntry != 0) {
        pProcessingStatusEntry = new (pProcessingStatusEntry) GameStatusEntry(g_GAME_ProcessingName);
        *(int *)((char *)pProcessingStatusEntry + 0x20) = 0;
        *(int *)((char *)pProcessingStatusEntry + 0x24) = 0;
        pMainContext->m_pProcessingStatus = pProcessingStatusEntry;
    } else {
        pMainContext->m_pProcessingStatus = 0;
    }

    pRefreshingStatusEntry = (GameStatusEntry *)AllocateVSMemBlock(0x28);
    if (pRefreshingStatusEntry != 0) {
        pRefreshingStatusEntry = new (pRefreshingStatusEntry) GameStatusEntry(g_GAME_RefreshingName);
        *(int *)((char *)pRefreshingStatusEntry + 0x20) = 0;
        *(int *)((char *)pRefreshingStatusEntry + 0x24) = 0;
        pMainContext->m_pRefreshingStatus = pRefreshingStatusEntry;
    } else {
        pMainContext->m_pRefreshingStatus = 0;
    }
    pMainContext->m_nModeTicks = 0;
    ((VsInitStatusEntryPointerArray *)g_pStatusEntryRegistry)
        ->AppendStatusEntry(pMainContext->m_pProcessingStatus);
    ((VsInitStatusEntryPointerArray *)g_pStatusEntryRegistry)
        ->AppendStatusEntry(pMainContext->m_pRefreshingStatus);

    pPrimaryStorage = AllocateResourceArchiveMemory(0x28);
    if (pPrimaryStorage != 0) {
        g_pMainResourceArchive =
            ((MogLoadResourceArchive *)pPrimaryStorage)->ConstructResourceArchive(
                g_GAME_MainArchiveName, 0x177000);
    } else {
        g_pMainResourceArchive = 0;
    }
    if (!ValidateResourceFileSignatureThunk()) {
        TriggerReleaseAssertFailure(g_GAME_IsValidResourceFileAssert, g_GAME_SourceFileName, 0x16e);
    }

    pPrimaryStorage = AllocateVSMemBlock(0xe4);
    if (pPrimaryStorage != 0) {
        VsInitFormattedOutputStream TitleStream;
        VsInitFixedBufferStream TitleBufferStream;
        short aScreenRect[4];
        char abTitleBuffer[0x50];

        pMainContext->m_pPrimaryContext =
            ((GamePrimaryContext *)pPrimaryStorage)
                ->ConstructPrimaryContextThunk(pMainContext);

        TitleStream.m_pVtable = g_FormattedOutputStreamVtable;
        TitleStream.m_TargetState.m_pVtable = g_StreamBaseVtable;
        TitleStream.m_TargetState.m_nReserved0c = (int)(unsigned long)g_StreamFormatSubobjectVtable;
        TitleStream.m_TargetState.m_dwFlags = 0x14;
        TitleStream.m_TargetState.m_chFill = ' ';
        TitleStream.m_TargetState.m_nWidth = 0;
        TitleStream.m_TargetState.m_nRadix = 10;
        TitleStream.m_TargetState.m_pDownstream = &TitleBufferStream;
        TitleBufferStream.ConstructFixedBufferStream(abTitleBuffer, sizeof(abTitleBuffer), 0);
        TitleStream.AppendCStringToStream(g_GAME_WindowTitle);
        GamePrimaryContext *pPrimaryContext =
            (GamePrimaryContext *)pMainContext->m_pPrimaryContext;
        short *pCenteredRect =
            pPrimaryContext->ComputePrimaryContextCenteredScreenRectThunk(aScreenRect, -1, -1);
        ((GamePrimaryContextSetRectProc)(*(void ***)pPrimaryContext)[1])(
            pPrimaryContext, 0, pCenteredRect, 0, abTitleBuffer);
        DestroyFixedBufferStream(&TitleBufferStream);
        RestoreStreamFormatSubobjectVtable(&TitleStream.m_TargetState);
        ConstructStreamFormatState(&TitleStream.m_TargetState);
    } else {
        pMainContext->m_pPrimaryContext = 0;
    }

    InitializeGlobalAudioManager(g_fMusicEnabled, g_fEffectsEnabled, 0x32, pMainContext->m_pPrimaryContext);
    if (g_fMusicEnabled != 0) {
        ((VsSoundManager *)g_pAudioManager)->SetAudioManagerMusicEnabledFlag(1);
        ((VsSoundManager *)g_pAudioManager)
            ->SetAudioManagerStartupMusicName(g_GAME_StartupMusicName);
    }

    pPrimaryStorage = AllocateVSMemBlock(0x2cc);
    if (pPrimaryStorage != 0) {
        g_pVariantResourceEntryManager =
            ConstructVariantResourceEntryManagerThunk(pPrimaryStorage);
    } else {
        g_pVariantResourceEntryManager = 0;
    }

    pMainContext->m_nModeTicks = 0;
    pMainContext->m_pActiveMode = 0;
    pMainContext->m_nActiveMode = 1;
    pMainContext->SwitchMainGameMode(1);

    strcpy(pMainContext->m_szLevelPath, g_GAME_DefaultLevelDirectory);
    if (pszCmdLine == 0) {
        strcat(pMainContext->m_szLevelPath, g_GAME_DefaultLevelFileName);
    } else {
        strcat(pMainContext->m_szLevelPath, pszCmdLine);
    }
    pMainContext->m_fQuitRequested = 0;

    return pMainContext;
}

// FUNCTION: LEMBALL 0x00402A7C
GameMainContext *GameMainContext::InitializeMainGameContextThunk(
    const char *pszCmdLine)
{
    return InitializeMainGameContext(pszCmdLine);
}

// FUNCTION: LEMBALL 0x00407420
void GameMainContext::SwitchMainGameMode(int nMode) {
    GameMainContext *pMainContext;
    void *pModeObject;
    void *pPrimaryActiveScreen;
    void **pSimpleModeObject;

    pMainContext = this;
    pPrimaryActiveScreen = *(void **)((char *)pMainContext->m_pPrimaryContext + 0xd4);
    if (pPrimaryActiveScreen != 0) {
        CallNoArgVirtual(pPrimaryActiveScreen, 4);
    }

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
        ((GamePrimaryContext *)pMainContext->m_pPrimaryContext)
            ->DestroyPrimaryContextActiveScreen(nMode);
        pModeObject = AllocateVSMemBlock(0x28);
        if (pModeObject == 0) {
            pMainContext->m_pActiveMode = 0;
        } else {
            InitializeBaseModeObject(pModeObject, pMainContext);
            *(void **)pModeObject = g_pStartupModeVtable;
            *(void **)((char *)pModeObject + 0xc) = g_pStartupModeRenderThunk;
            pMainContext->m_pActiveMode = pModeObject;
        }
        pMainContext->m_nModeTicks = 0;
        break;

    case 2:
        pMainContext->m_nActiveMode = nMode;
        ((GamePrimaryContext *)pMainContext->m_pPrimaryContext)
            ->DestroyPrimaryContextActiveScreen(nMode);
        EnsureMainGameVariantResourceMode(pMainContext, 3);
        if (g_fLevelDemoModeEnabled != 0) {
            g_fLevelDemoModeEnabled = g_nStoredLevelDemoModeEnabled;
        }
        pModeObject = AllocateVSMemBlock(0x28);
        if (pModeObject == 0) {
            pMainContext->m_pActiveMode = 0;
        } else {
            InitializeBaseModeObject(pModeObject, pMainContext);
            *(void **)pModeObject = g_pMainMenuModeVtable;
            *(void **)((char *)pModeObject + 0xc) = g_pMainMenuModeRenderThunk;
            pMainContext->m_pActiveMode = pModeObject;
        }
        break;

    case 3:
        pMainContext->m_nActiveMode = nMode;
        ((GamePrimaryContext *)pMainContext->m_pPrimaryContext)
            ->DestroyPrimaryContextActiveScreen(nMode);
        pModeObject = AllocateVSMemBlock(0x28);
        if (pModeObject == 0) {
            pMainContext->m_pActiveMode = 0;
        } else {
            InitializeBaseModeObject(pModeObject, pMainContext);
            *(void **)pModeObject = g_pMode3Vtable;
            *(void **)((char *)pModeObject + 0xc) = g_pMode3RenderThunk;
            pMainContext->m_pActiveMode = pModeObject;
        }
        break;

    case 4:
        pMainContext->m_nActiveMode = nMode;
        ((GamePrimaryContext *)pMainContext->m_pPrimaryContext)
            ->DestroyPrimaryContextActiveScreen(nMode);
        pModeObject = AllocateVSMemBlock(0x28);
        pMainContext->m_pActiveMode =
            pModeObject == 0 ? 0 : ConstructLevelSelectionModeController(pModeObject, pMainContext);
        break;

    case 5:
        goto level_game_mode;

    case 10:
        pMainContext->m_nActiveMode = nMode;
        ((GamePrimaryContext *)pMainContext->m_pPrimaryContext)
            ->DestroyPrimaryContextActiveScreen(nMode);
        pSimpleModeObject = (void **)AllocateVSMemBlock(0x10);
        if (pSimpleModeObject == 0) {
            pMainContext->m_pActiveMode = 0;
        } else {
            pSimpleModeObject[0] = g_pSimpleModeDeleteVtable;
            pSimpleModeObject[2] = 0;
            pSimpleModeObject[1] = 0;
            pSimpleModeObject[0] = g_pSimpleModeVtable;
            pSimpleModeObject[3] = pMainContext;
            pMainContext->m_pActiveMode = pSimpleModeObject;
        }
        break;

    case 0x0c:
        pMainContext->m_nActiveMode = nMode;
        ((GamePrimaryContext *)pMainContext->m_pPrimaryContext)
            ->DestroyPrimaryContextActiveScreen(nMode);
        pModeObject = AllocateVSMemBlock(0x38);
        pMainContext->m_pActiveMode =
            pModeObject == 0 ? 0 : ConstructNetworkLobbyTransportController(pModeObject, pMainContext);
        break;

    case 0x0e:
    case 0x0f:
        pMainContext->m_nActiveMode = nMode;
        ((GamePrimaryContext *)pMainContext->m_pPrimaryContext)
            ->DestroyPrimaryContextActiveScreen(nMode);
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
        ((GamePrimaryContext *)pMainContext->m_pPrimaryContext)
            ->DestroyPrimaryContextActiveScreen(nMode);
        pModeObject = AllocateVSMemBlock(0x28);
        if (pModeObject == 0) {
            pMainContext->m_pActiveMode = 0;
        } else {
            InitializeBaseModeObject(pModeObject, pMainContext);
            *(void **)pModeObject = g_pMode16Vtable;
            *(void **)((char *)pModeObject + 0xc) = g_pMode16RenderThunk;
            pMainContext->m_pActiveMode = pModeObject;
        }
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
        ((GamePrimaryContext *)pMainContext->m_pPrimaryContext)
            ->DestroyPrimaryContextActiveScreen(nMode);
        pModeObject = AllocateVSMemBlock(0x1f0);
        pMainContext->m_pActiveMode = 0;
        if (pModeObject != 0) {
            pModeObject = ConstructLevelGameMode(pModeObject, pMainContext);
            if (pModeObject != 0) {
                pMainContext->m_pActiveMode = (char *)pModeObject + 0x10;
                break;
            }
        }
        break;

    default:
        break;
    }

    ((GamePrimaryContext *)pMainContext->m_pPrimaryContext)
        ->SwitchPrimaryContextScreen(pMainContext->m_nActiveMode);
}

// FUNCTION: LEMBALL 0x004071D0
void LEMBALL_FASTCALL ShutdownMainGameContext(GameMainContext *pMainContext) {
    DWORD dwStartTime;
    void *pVariantMode;
    void *pMainResourceArchive;

    pVariantMode = pMainContext->m_pVariantMode;
    if (pVariantMode != 0) {
        DestroyMainGameVariantResourceMode(pVariantMode);
        FreeVSMemBlock(pVariantMode);
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
        ((GameNetworkRuntimeVirtualInterface *)g_pNetworkLobbyVsnetRuntime)->Close(1);
        g_pNetworkLobbyVsnetRuntime = 0;
    }

    if (pMainContext->m_pPrimaryContext != 0) {
        ((void (LEMBALL_FASTCALL *)(void *))
            (*(void ***)pMainContext->m_pPrimaryContext)[0x74 / 4])(
            pMainContext->m_pPrimaryContext);
        if (pMainContext->m_pPrimaryContext != 0) {
            ((GameNetworkRuntimeVirtualInterface *)
                ((char *)pMainContext->m_pPrimaryContext + 0x90))->Close(1);
        }
    }
    if (pMainContext->m_pActiveMode != 0) {
        void **pActiveModeVtable;

        pActiveModeVtable = *(void ***)pMainContext->m_pActiveMode;
        ((void (*)(int))pActiveModeVtable[0])(1);
        pMainContext->m_pActiveMode = 0;
    }

    if (g_pVariantResourceEntryManager != 0) {
        void *pVariantResourceEntryManager;

        pVariantResourceEntryManager = g_pVariantResourceEntryManager;
        ShutdownVariantResourceEntryManager(pVariantResourceEntryManager);
        FreeVSMemBlock(pVariantResourceEntryManager);
        ShutdownGlobalAudioManager();
    }
    pMainResourceArchive = g_pMainResourceArchive;
    if (pMainResourceArchive != 0) {
        DestroyResourceArchive(pMainResourceArchive);
        FreeResourceArchiveMemory(pMainResourceArchive);
        g_pMainResourceArchive = 0;
    }
    if (g_pLevelProgressState != 0) {
        FreeVSMemBlock(g_pLevelProgressState);
    }

    SetVisualSciencesRegistryRunningState(g_GAME_WindowTitle, 0);
}

// FUNCTION: LEMBALL 0x00401839
int RunMainGameSessionThunk(int cArgs, const char *const *ppszArgs) {
    return RunMainGameSession(cArgs, ppszArgs);
}

// FUNCTION: LEMBALL 0x00406310
int RunMainGameSession(int cArgs, const char *const *ppszArgs) {
    GameMainContext *pMainContext;
    int *pTrigTableBuffer;
    void *pSessionRandomState;

    pTrigTableBuffer = (int *)AllocateVSMemBlock(0x800);
    if (pTrigTableBuffer != 0) {
        g_pSignedTrigTable = InitializeSignedTrigTable(pTrigTableBuffer);
    } else {
        g_pSignedTrigTable = 0;
    }

    pSessionRandomState = AllocateVSMemBlock(4);
    if (pSessionRandomState != 0) {
        *(unsigned int *)pSessionRandomState = 0xad28;
        g_pSessionRandomState = pSessionRandomState;
    } else {
        g_pSessionRandomState = 0;
    }

    CreateFrameTimerController(0x19000);
    InitializeStartupSwitchDefaults();
    if (ApplyStartupCommandLineSwitches(cArgs, ppszArgs) == 1) {
        pMainContext = (GameMainContext *)AllocateVSMemBlock(0x70);
        if (pMainContext == 0) {
            pMainContext = 0;
        } else {
            pMainContext = pMainContext->InitializeMainGameContextThunk(0);
        }

        if (g_fStartupEditLevelOverride != 0) {
            strcpy(pMainContext->m_szLevelPath,
                (const char *)g_abOverrideLevelFilePathBuffer);
        }
        if (g_fStartupPlayLevelOverride != 0) {
            strcpy(pMainContext->m_szLevelPath,
                (const char *)g_abOverrideLevelFilePathBuffer);
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
    g_pStatusOutputStream->AppendCStringToStream(g_GAME_ClosedDownText);
    return 0;
}
