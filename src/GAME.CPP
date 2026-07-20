#include "GAME.H"
#include "LEVEL/LEVELRUN.H"
#include "LEVEL/DRAWTEXT.H"
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
#include "RESOURCE/VARMGR.H"
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

void PASCAL SetAudioMusicAndEffectVolumeByte(unsigned char nVolume);
void PASCAL SetVariantResourceEffectMasterVolume(unsigned char nVolume);

class VsGdiAnimWindowOwner {
public:
    void *ConstructResourceGeometryHelperWindowOwner(void);
    void DestroyResourceGeometryHelperWindowOwner(void);
};

class GameIntroSequenceMediaState : public VsGdiAnimWindowOwner {
public:
    void ConfigureOwnerThunk(
        void *pScreen, void *pPrimaryContext, int fSkipVariant);
};

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
int g_fRootHelperGeometryDispatchSuppressed;

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
extern void PASCAL noop_embedded_mci_callback_0046e050(void *pUnused);
extern int g_fMusicEnabled;
extern int g_fEffectsEnabled;
  extern void RegisterVSBaseWindowClass(void);
extern void *LEMBALL_FASTCALL InitializePackagedSpriteRenderEntryThunk(void *pObject);
extern void *LEMBALL_FASTCALL InitializeRenderPointRectSinkEntryThunk(void *pObject);
extern void *LEMBALL_FASTCALL InitializeStatusIndicatorPointSinkEntryThunk(void *pObject);
extern void *LEMBALL_FASTCALL DeleteStatusPointSinkEntryThunk(
    void *pEntry, int nUnused, unsigned char fDelete);
extern void LEMBALL_FASTCALL QueueStatusPointPushThunk(
    void *pEntry, int nUnused, void *pQueue);
extern void LEMBALL_FASTCALL ExecuteStatusPointPushThunk(
    void *pEntry, int nUnused, void *pContext);
extern void *LEMBALL_FASTCALL InitializeRenderRectSinkEntryA(void *pObject);
extern void *LEMBALL_FASTCALL InitializeRenderRectSinkEntryB(void *pObject);
extern void *LEMBALL_FASTCALL InitializeLevelSurfaceRectSink(void *pObject);
extern void *LEMBALL_FASTCALL InitializeLevelNestedRectSink(void *pObject);
extern void *LEMBALL_FASTCALL InitializeHelperLocalRectUpdateEntry(void *pObject);
extern void *LEMBALL_FASTCALL InitializeGeometryOwnerRect(
    void *pOwner, int nUnused, const VS_XYPair *pRect);
extern void LEMBALL_FASTCALL AppendGeometryOwnerChild(
    void *pOwner, int nUnused, void *pChildOwner);
extern void LEMBALL_FASTCALL RemoveGeometryOwnerChild(
    void *pOwner, int nUnused, void *pChildOwner);
extern void *ConstructLevelScreenVariantRenderManager(void *pObject,
    int nRenderOwner, void *pPrimaryContext, void *pLevelMode);
extern void DestroyLevelScreenVariantRenderManager(void *pObject);
extern void RenderReconstructedLevelScreenTerrain(void *pObject);
extern void DrawReconstructedLevelScreenStatusText(void *pObject);
extern int EmitLevelGameModeDynamicRenderEntries(
    void *pObject, void *pOutputEntries);
extern void RebuildLevelScreenVariantResourceSet(void *pObject, int nMode);
extern void *ConstructLevelScreenActionPanelResources(
    void *pObject, void *pLevelScreen);
extern void DestroyLevelScreenActionPanelResources(void *pObject);
extern void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableA(void *pObject);
extern void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableD(void *pObject);
extern void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableE(void *pObject);
extern void LEMBALL_FASTCALL BuildGeometryHelperFromRenderRect(void *pOwner);
extern unsigned int __stdcall ComputePrimaryContextWindowStyleFlags(void);
extern int __stdcall GetPrimaryContextMenuDefinition(unsigned int *pMenuId,
                                                     void **ppMenuDefinition);
extern void LEMBALL_FASTCALL PrepareLevelScreenRootHelperForRuntime(
    GamePrimaryContext *pPrimaryContext);
extern void *LoadZrleResource(int nResourceId);
extern void *LoadPalResource(int nResourceId);
extern void *LoadBitmapResource(int nResourceId);
extern void SetLevelScreenStatusIndicatorMode(int nMode, int nValue);
void SetLevelScreenStatusIndicatorModeThunk(int nMode, int nValue);
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
extern int LEMBALL_FASTCALL DrainRenderDispatchQueueEntries(
    void *pDispatchQueue, int nUnused, unsigned int cEntries);
extern void LEMBALL_FASTCALL DispatchAndClearPointerQueue(void *pQueue);
extern "C" DWORD WINAPI timeGetTime(void);
extern void *LEMBALL_FASTCALL ConstructFramedScreenBaseFieldsThunk(
    void *pObject, int nUnusedEdx, void *pPrimaryContext,
    void *pWindowOwnerContext, short *paRect, int nFrameKind,
    int nVariantBase, int nResourceBase, int nLayout,
    int nOptionA, int nOptionB);
extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);
extern int LEMBALL_FASTCALL IsPrimaryContextWindowReadyForPresent(
    int *pPrimaryContext);
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

    void Advance(void);
    bool Retreat(void);
    bool IsCurrentLevelAvailable(void);
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
    GameBaseModeObject *InitializeThunk(GameMainContext *pMainContext);
    GameBaseModeObject *Initialize(GameMainContext *pMainContext);
    void Update(void);

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
    int m_nManagedEntityCount;
    int m_nManagedEntityCapacity;
    void *m_pManagedEntityTable;
    int m_nReserved124;
    int m_nReserved128;
    int m_nReserved12C;
    int m_nReserved130;
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
    void (GameWindowOwnerRenderContext::*pMethod)(void) =
        &GameWindowOwnerRenderContext::UpdateWindowOwnerRenderContext;
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
    void (GameWindowOwnerRenderContext::*pMethod)(short *, void *, const char *, int) =
        &GameWindowOwnerRenderContext::InitializeWindowOwnerFromRectAndActivate;
    return *(void **)&pMethod;
}

static void *GetPrimaryContextApplyWindowOwnerPaletteResourceAddress(void) {
    void (GameWindowOwnerRenderContext::*pMethod)(int) =
        &GameWindowOwnerRenderContext::ApplyWindowOwnerPaletteResource;
    return *(void **)&pMethod;
}

static void *GetWindowOwnerPresentAddress(void) {
    void (GameWindowOwnerRenderContext::*pMethod)(void) =
        &GameWindowOwnerRenderContext::PresentWindowOwnerRenderContext;
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
static void *g_GAME_WindowOwnerRenderContextVtableSlots[50];
static int g_fWindowOwnerRenderContextVtableInitialized;
extern void *LEMBALL_FASTCALL DeleteRenderQueueNodeThunk(
    void *pObject, int nUnused, unsigned char fDelete);
extern int LEMBALL_FASTCALL IncrementRenderQueueNodeCounterThunk(
    void *pRenderQueueNode, int nUnused, void *pEntry);
extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);
// GLOBAL: LEMBALL 0x00493110
void *g_GAME_RenderQueueNodeVtableSlots[4] = {
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)DeleteRenderQueueNodeThunk,
    (void *)IncrementRenderQueueNodeCounterThunk,
    0,
};
static void *g_GAME_GenericModeVtableSlots[6];
static void *g_GAME_GenericScreenVtableSlots[14];
static void *g_GAME_IntroSequenceMediaOwnerVtable[50];
static int g_fGAMEIntroSequenceMediaOwnerVtableInitialized;
struct GameSafeVtableInitializer {
    GameSafeVtableInitializer(void) {
        int i;
        for (i = 0; i < 46; ++i)
            g_GAME_PrimaryContextVtableSlots[i] = (void *)NetworkSafeVtableNoop;
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

struct GameEffTransportRuntimeWindow {
    void RegisterEffTransportEventClient(void *pClient);
    void UnregisterEffTransportEventClient(void);
};
extern void *LEMBALL_FASTCALL ConstructNetworkLobbyPeerClearCloseStream(
    void *pObject);
extern void *LEMBALL_FASTCALL ConstructNetworkLobbyPeerDirtyConfirmStream(
    void *pObject);
extern void LEMBALL_FASTCALL StopNetworkLobbyTransportSession(
    void *pController);
struct GameNetworkLevelChunkDeltaStream {
    void *ConstructNetworkLevelChunkDeltaStream(int nOwner);
    void *ConstructNetworkLevelChunkDeltaStreamEntry(int nOwner);
};
extern void *ConstructMainGameVariantResourceBundle(void *pBundle, void *pPrimaryContext, unsigned short nVariantMode);
extern void InitializeManagedEntitySlotTablesEntryThunk(void *pLevelGameMode);
extern unsigned short g_GAME_ManagedEntityRegistryCount;
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
extern void *LEMBALL_FASTCALL ReturnArgumentVtableCallback(void *pObject);
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

// GLOBAL: LEMBALL 0x004a1bd0
void *g_pStatusEntryRegistry = 0;
// GLOBAL: LEMBALL 0x0049cb68
void *g_pLevelProgressState = 0;
// GLOBAL: LEMBALL 0x004a1d58
void *g_pMainResourceArchive = 0;
// GLOBAL: LEMBALL 0x0049eb80
void *g_pVariantResourceEntryManager = 0;
// GLOBAL: LEMBALL 0x004a0120
void *g_pNetworkLobbyVsnetRuntime = 0;
// GLOBAL: LEMBALL 0x004a1e18
void *g_pActiveNetworkRuntimeWindow = 0;
// GLOBAL: LEMBALL 0x0049ce04
int g_nLevelFrameClockTick = 0;
extern int g_nLevelFrameClockTimeMs;
extern int g_nNetworkFrameClockLatestTimeMs;
extern int g_fLevelEndStateRequestLatched;
// GLOBAL: LEMBALL 0x0049ce28
int g_fLevelFrameClockPaused = 0;
static void *g_GAME_StartupModeVtable[6];
static void *g_GAME_StartupModeEventClientVtable[4];
static void *g_GAME_LevelSelectionModeVtable[6];
static void *g_GAME_LevelSelectionModeEventClientVtable[4];
static void *g_GAME_MainMenuModeVtable[6];
static void *g_GAME_MainMenuModeEventClientVtable[4];
static void *g_GAME_OptionsModeVtable[6];
static void *g_GAME_OptionsModeEventClientVtable[4];
static void *g_GAME_NetworkLobbyTransportControllerVtable[6];
static void *g_GAME_NetworkLobbyTransportEventClientVtable[4];
static void *g_GAME_LevelModeActiveInterfaceVtable[2];
void *g_pStartupModeVtable = g_GAME_StartupModeVtable;
void *g_pStartupModeRenderThunk = g_GAME_StartupModeEventClientVtable;
void *g_pMainMenuModeVtable = g_GAME_MainMenuModeVtable;
void *g_pMainMenuModeRenderThunk = g_GAME_MainMenuModeEventClientVtable;
void *g_pMode3Vtable = g_GAME_OptionsModeVtable;
void *g_pMode3RenderThunk = g_GAME_OptionsModeEventClientVtable;
void *g_pSimpleModeDeleteVtable = g_GAME_GenericModeVtableSlots;
void *g_pSimpleModeVtable = g_GAME_GenericModeVtableSlots;
void *g_pMode16Vtable = g_GAME_GenericModeVtableSlots;
void *g_pMode16RenderThunk = g_GAME_NetworkLobbyTransportEventClientVtable;
void *g_pPrimaryContextVtable = g_GAME_PrimaryContextVtableSlots;
void *g_pPrimaryContextRenderQueueNodeVtable = g_GAME_RenderQueueNodeVtableSlots;
extern void *g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage[8];
extern void LEMBALL_FASTCALL QueueQueuedRenderPointSinkThunk(
    void *pPointSink, void *pUnusedEdx, void *pQueue);
extern void __stdcall PromoteQueuedRenderPointSinkTargetStateThunk(
    void *pPointSink);



int RunMainGameSessionThunk(int cArgs, const char *const *ppszArgs);
int PASCAL ReturnZeroBaseModeThunk(void *, void *, void *);
extern int PASCAL return_zero_screen_callback_00446990(
    void *, void *, void *);
void *ConstructNetworkLobbyTransportController(void *pModeObject, GameMainContext *pMainContext);
void ReleaseMainGameVariantResourceMode(GameMainContext *pMainContext);
void *ConstructLevelSelectionModeController(void *pModeObject, GameMainContext *pMainContext);
void *LEMBALL_FASTCALL DeleteQueuedRenderPointSinkFinalizeThunk(
    void *pObject, int, int fDelete);
void *LEMBALL_FASTCALL DeleteStartupModeEventClientThunk(
    void *pObject, int, BYTE fDelete);
static void *LEMBALL_FASTCALL DeleteLevelSelectionModeEventClientThunk(
    void *pObject, int, BYTE fDelete);
void *ConstructPaintballSequenceModeController(void *pModeObject,
                                               GameMainContext *pMainContext,
                                               int fSequenceVariant);
int LEMBALL_FASTCALL IsPrimaryContextWindowReadyForPresentThunk(int *pPrimaryContext);
void *LEMBALL_FASTCALL DeleteStartupModeThunk(
    void *pObject, int nUnusedEdx, BYTE fDelete);
static void *LEMBALL_FASTCALL DeleteLevelSelectionModeThunk(
    void *pObject, int nUnusedEdx, BYTE fDelete);
void *LEMBALL_FASTCALL DeleteMainMenuModeThunk(
    void *pObject, int nUnusedEdx, BYTE fDelete);
static void *LEMBALL_FASTCALL DeleteOptionsModeThunk(
    void *pObject, int nUnusedEdx, BYTE fDelete);
static void *LEMBALL_FASTCALL DeleteOptionsModeEventClientThunk(
    void *pObject, int nUnusedEdx, BYTE fDelete);
void EnsureMainGameVariantResourceMode(GameMainContext *pMainContext, int nVariantMode);
void *ConstructLevelGameMode(void *pModeObject, GameMainContext *pMainContext);
void *LEMBALL_FASTCALL DeleteLevelModeActiveInterfaceThunk(
    void *pActiveInterface, int nUnusedEdx, BYTE fDelete);
void LEMBALL_FASTCALL UpdateLevelModeActiveInterfaceThunk(
    void *pActiveInterface);
void RequestLocalLevelGameStateChange(void *pLevelMode, int nState);
void LEMBALL_FASTCALL UpdateBaseModeThunk(GameBaseModeObject *pMode);
void *LEMBALL_FASTCALL DeleteMainMenuModeEventClientThunk(
    void *pObject, int, BYTE fDelete);
int __stdcall ValidateResourceFileSignatureThunk(void);
void LEMBALL_FASTCALL NoopBaseModeThunk(void *);
int PASCAL ComputePowerOfTwoThunk(int nExponent);
int LEMBALL_FASTCALL ReturnFalseBaseModeThunk(void *);
void LEMBALL_FASTCALL RequestFramedScreenTransitionMode2Thunk(void *pScreen);
int RunMainGameSession(int cArgs, const char *const *ppszArgs);
int PASCAL ComputePowerOfTwo(int nExponent);
void LEMBALL_FASTCALL ShutdownMainGameContext(GameMainContext *pMainContext);
int __stdcall ValidateResourceFileSignature(void);
void LEMBALL_FASTCALL UpdateMainGameActiveMode(void *pMainGameContext);
void LEMBALL_FASTCALL PresentMainGameFrame(void *pMainGameContext);
void LEMBALL_FASTCALL RunMainGameLoop(GameMainContext *pMainContext);
void *LEMBALL_FASTCALL DeleteStartupMode(
    void *pObject, int, BYTE fDelete);
void *LEMBALL_FASTCALL DeleteMainMenuMode(
    void *pObject, int, BYTE fDelete);
void SetLevelFrameClockPauseFlag(int fPaused);
void LEMBALL_FASTCALL DestroyPrimaryContextActiveScreenForVtable(
    void *pPrimaryContext);
void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x10(void *pPrimaryContext);
void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x0C(
    void *pPrimaryContext, int, int);
void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x14(void *pPrimaryContext);
int LEMBALL_FASTCALL IsPrimaryContextWindowReadyForPresent(int *pPrimaryContext);
void LEMBALL_FASTCALL PresentPrimaryContextScreen(void *pPrimaryContext);
void LEMBALL_FASTCALL UpdatePrimaryContextActiveScreen(void *pPrimaryContext);
void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x30(
    void *pPrimaryContext);
int LEMBALL_FASTCALL PollPrimaryContextScreenStatus(void *pPrimaryContext);
int LEMBALL_FASTCALL GetPrimaryContextRequestedScreen(void *pPrimaryContext);
void LEMBALL_FASTCALL DispatchPrimaryContextSlot23Thunk(
    void *pPrimaryContext);
void *LEMBALL_FASTCALL DeleteQueuedRenderPointSinkFinalize(
    void *pObject, int, int fDelete);
void LEMBALL_FASTCALL RequestFramedScreenTransitionMode2(void *pScreen);
void LEMBALL_FASTCALL DestroyBaseModeObject(void *pObject);
int __stdcall PumpMessagesAndRunFrame(void);
void TriggerReleaseAssertFailure(const char *pszExpression, const char *pszFile, int nLine);
int SetVisualSciencesRegistryRunningState(const char *pszProductName, int fRunning);
char *__stdcall FindCdromFilePathBySuffix(const char *pszSuffix);
void *LEMBALL_FASTCALL InitializeRenderQueueNodeBase(void *pRenderQueueNode);
void LEMBALL_FASTCALL DestroyWindowOwnerRenderContext(void *pPrimaryContext);
char *GetSrcDiskRegistryValueBuffer(void);
void ShowReleaseAssertFailureAndAbort(const char *pszExpression, const char *pszFile, int nLine);

// GLOBAL: LEMBALL 0x00496c98
static void *g_GAME_QueuedRenderPointSinkFinalizeVtable[4] = {
    (void *)DeleteQueuedRenderPointSinkFinalizeThunk,
    (void *)QueueQueuedRenderPointSinkThunk,
    (void *)PromoteQueuedRenderPointSinkTargetStateThunk,
    0,
};
void *g_pQueuedRenderPointSinkFinalizeThunk =
    g_GAME_QueuedRenderPointSinkFinalizeVtable;
// GLOBAL: LEMBALL 0x004a9360
void *g_pSharedRenderDispatchQueue;
// GLOBAL: LEMBALL 0x004a279c
void *g_pSharedGeometryHelper = 0;
// GLOBAL: LEMBALL 0x004a9bec
static void *g_pCursorWindowOwner;
// GLOBAL: LEMBALL 0x004a9bf0
static int g_uCursorWindowOwnerState;
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
extern int LEMBALL_FASTCALL HandleNetworkLobbyTransportPayloadEvent(
    void *pController, int nUnusedEdx, unsigned short nEventType,
    void *pPayloadObject, void *pPeer);
extern void LEMBALL_FASTCALL CloseNetworkLobbyTransportPeers(
    void *pController);
extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);

class GameNetworkLobbyTransportControllerVtableModel {
public:
    virtual void Reserved00(void) {}
    virtual void Reserved04(void) {}
    virtual int HandleTransportPayload(
        unsigned short nEventType, void *pPayloadObject, void *pPeer) {
        return HandleNetworkLobbyTransportPayloadEvent(
            this, 0, nEventType, pPayloadObject, pPeer);
    }
    virtual void Reserved0C(void) {}
    virtual int HandleTransportEvent(void *) { return 0; }
    virtual void Reserved14(void) {}
};

static GameNetworkLobbyTransportControllerVtableModel
    g_GAME_NetworkLobbyTransportControllerVtableModel;

static void DestroyNetworkLobbyTransportController(void *pObject);
static void *LEMBALL_FASTCALL DeleteNetworkLobbyTransportController(
    void *pObject, int nUnusedEdx, BYTE fDelete);
static void *LEMBALL_FASTCALL
DeleteNetworkLobbyTransportControllerEventClient(
    void *pObject, int nUnusedEdx, BYTE fDelete);














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
        GetWindowOwnerPresentAddress();
    g_GAME_GenericModeVtableSlots[0] = g_pGAME_ModeDeleteVtableSlot;
    g_GAME_GenericModeVtableSlots[1] = (void *)StubNoOpVoid;
    g_GAME_LevelModeActiveInterfaceVtable[0] =
        (void *)DeleteLevelModeActiveInterfaceThunk;
    g_GAME_LevelModeActiveInterfaceVtable[1] =
        (void *)UpdateLevelModeActiveInterfaceThunk;

    g_GAME_StartupModeVtable[0] = (void *)DeleteStartupModeThunk;
    g_GAME_StartupModeVtable[1] = (void *)UpdateBaseModeThunk;
    g_GAME_StartupModeVtable[2] =
        (void *)return_zero_screen_callback_00446990;
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

    g_GAME_LevelSelectionModeVtable[0] =
        (void *)DeleteLevelSelectionModeThunk;
    g_GAME_LevelSelectionModeVtable[1] = (void *)UpdateBaseModeThunk;
    g_GAME_LevelSelectionModeVtable[2] =
        (void *)return_zero_screen_callback_00446990;
    g_GAME_LevelSelectionModeVtable[3] = (void *)NoopBaseModeThunk;
    g_GAME_LevelSelectionModeVtable[4] = (void *)ReturnFalseBaseModeThunk;
    g_GAME_LevelSelectionModeVtable[5] = 0;
    g_GAME_LevelSelectionModeEventClientVtable[0] =
        (void *)InitializeNonZrleVariantRenderEntry;
    g_GAME_LevelSelectionModeEventClientVtable[1] =
        (void *)DeleteLevelSelectionModeEventClientThunk;
    g_GAME_LevelSelectionModeEventClientVtable[2] =
        (void *)HandleBaseModeActionButtonEventThunk;
    g_GAME_LevelSelectionModeEventClientVtable[3] = 0;

    g_GAME_MainMenuModeVtable[0] = (void *)DeleteMainMenuModeThunk;
    g_GAME_MainMenuModeVtable[1] = (void *)UpdateBaseModeThunk;
    g_GAME_MainMenuModeVtable[2] =
        (void *)return_zero_screen_callback_00446990;
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

    g_GAME_OptionsModeVtable[0] = (void *)DeleteOptionsModeThunk;
    g_GAME_OptionsModeVtable[1] = (void *)UpdateBaseModeThunk;
    g_GAME_OptionsModeVtable[2] =
        (void *)return_zero_screen_callback_00446990;
    g_GAME_OptionsModeVtable[3] = (void *)NoopBaseModeThunk;
    g_GAME_OptionsModeVtable[4] = (void *)ReturnFalseBaseModeThunk;
    g_GAME_OptionsModeVtable[5] = 0;
    g_GAME_OptionsModeEventClientVtable[0] =
        (void *)InitializeNonZrleVariantRenderEntry;
    g_GAME_OptionsModeEventClientVtable[1] =
        (void *)DeleteOptionsModeEventClientThunk;
    g_GAME_OptionsModeEventClientVtable[2] =
        (void *)HandleBaseModeActionButtonEventThunk;
    g_GAME_OptionsModeEventClientVtable[3] = 0;

    g_GAME_NetworkLobbyTransportControllerVtable[0] =
        (void *)DeleteNetworkLobbyTransportController;
    g_GAME_NetworkLobbyTransportControllerVtable[1] =
        (void *)UpdateBaseModeThunk;
    g_GAME_NetworkLobbyTransportControllerVtable[2] =
        (*(void ***)&g_GAME_NetworkLobbyTransportControllerVtableModel)[2];
    g_GAME_NetworkLobbyTransportControllerVtable[3] =
        (void *)NoopBaseModeThunk;
    g_GAME_NetworkLobbyTransportControllerVtable[4] =
        (void *)ReturnFalseBaseModeThunk;
    g_GAME_NetworkLobbyTransportControllerVtable[5] = 0;
    g_GAME_NetworkLobbyTransportEventClientVtable[0] =
        (void *)InitializeNonZrleVariantRenderEntry;
    g_GAME_NetworkLobbyTransportEventClientVtable[1] =
        (void *)DeleteNetworkLobbyTransportControllerEventClient;
    g_GAME_NetworkLobbyTransportEventClientVtable[2] =
        (void *)HandleBaseModeActionButtonEventThunk;
    g_GAME_NetworkLobbyTransportEventClientVtable[3] = 0;

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

static void *GetWindowOwnerActivationHookAddress(void) {
    void (GameWindowOwnerRenderContext::*pMethod)(int) =
        &GameWindowOwnerRenderContext::ApplyWindowOwnerActivation;
    return *(void **)&pMethod;
}

static void InitializeWindowOwnerRenderContextVtable(
    void **pWindowOwnerBaseVtable) {
    int i;

    if (g_fWindowOwnerRenderContextVtableInitialized != 0) {
        return;
    }
    for (i = 0; i < 46; ++i) {
        g_GAME_WindowOwnerRenderContextVtableSlots[i] =
            pWindowOwnerBaseVtable[i];
    }
    g_GAME_WindowOwnerRenderContextVtableSlots[1] = g_GAME_PrimaryContextVtableSlots[1];
    g_GAME_WindowOwnerRenderContextVtableSlots[2] = g_GAME_PrimaryContextVtableSlots[2];
    g_GAME_WindowOwnerRenderContextVtableSlots[6] = g_GAME_PrimaryContextVtableSlots[6];
    g_GAME_WindowOwnerRenderContextVtableSlots[7] = g_GAME_PrimaryContextVtableSlots[7];
    g_GAME_WindowOwnerRenderContextVtableSlots[8] = g_GAME_PrimaryContextVtableSlots[8];
    g_GAME_WindowOwnerRenderContextVtableSlots[10] = g_GAME_PrimaryContextVtableSlots[10];
    g_GAME_WindowOwnerRenderContextVtableSlots[11] = g_GAME_PrimaryContextVtableSlots[11];
    g_GAME_WindowOwnerRenderContextVtableSlots[14] = g_GAME_PrimaryContextVtableSlots[14];
    g_GAME_WindowOwnerRenderContextVtableSlots[15] = (void *)StubNoOpVoid;
    g_GAME_WindowOwnerRenderContextVtableSlots[16] = (void *)StubNoOpVoid;
    g_GAME_WindowOwnerRenderContextVtableSlots[17] = (void *)StubNoOpVoid;
    g_GAME_WindowOwnerRenderContextVtableSlots[25] = g_GAME_PrimaryContextVtableSlots[25];
    g_GAME_WindowOwnerRenderContextVtableSlots[28] = g_GAME_PrimaryContextVtableSlots[28];
    g_GAME_WindowOwnerRenderContextVtableSlots[32] = g_GAME_PrimaryContextVtableSlots[32];
    g_GAME_WindowOwnerRenderContextVtableSlots[33] = g_GAME_PrimaryContextVtableSlots[33];
    g_GAME_WindowOwnerRenderContextVtableSlots[40] =
        g_GAME_PrimaryContextVtableSlots[40];
    g_GAME_WindowOwnerRenderContextVtableSlots[41] =
        g_GAME_PrimaryContextVtableSlots[41];
    g_GAME_WindowOwnerRenderContextVtableSlots[42] =
        GetWindowOwnerActivationHookAddress();
    g_GAME_WindowOwnerRenderContextVtableSlots[43] =
        g_GAME_PrimaryContextVtableSlots[43];
    g_GAME_WindowOwnerRenderContextVtableSlots[44] =
        g_GAME_PrimaryContextVtableSlots[44];
    g_GAME_WindowOwnerRenderContextVtableSlots[45] =
        g_GAME_PrimaryContextVtableSlots[45];
    g_GAME_WindowOwnerRenderContextVtableSlots[46] =
        (void *)noop_embedded_mci_callback_0046e050;
    g_GAME_WindowOwnerRenderContextVtableSlots[47] =
        (void *)noop_embedded_mci_callback_0046e050;
    g_GAME_WindowOwnerRenderContextVtableSlots[48] = (void *)StubNoOpVoid;
    g_GAME_WindowOwnerRenderContextVtableSlots[49] = (void *)StubNoOpVoid;
    g_fWindowOwnerRenderContextVtableInitialized = 1;
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

extern void LEMBALL_FASTCALL RequestIntroSequenceScreenTransition(
    char *pScreen);

extern void LEMBALL_FASTCALL HandleIntroSequenceMediaCompletion(
    void *pOwner);

void *GameScreenObject::ConstructOptionsScreen(void *pPrimaryContext, void *pWindowOwnerContext, short *paRect) {
    return ConstructFunctionalOptionsScreen(
        this, 0, pPrimaryContext, pWindowOwnerContext, paRect);
}

void *GameScreenObject::ConstructLevelSelectionScreen(void *pPrimaryContext,
                                                       void *pWindowOwnerContext,
                                                       short *paRect) {
    return ConstructFunctionalLevelSelectionScreen(
        this, 0, pPrimaryContext, pWindowOwnerContext, paRect);
}

static void *g_GAME_LevelScreenVtableSlots[14];
static int g_fGAMELevelScreenVtableInitialized;
static void *g_GAME_LevelScreenRenderStatePopVtable[4];
static int g_fGAMELevelScreenRenderStatePopVtableInitialized;
static void *g_GAME_LevelScreenRenderStatePushVtable[4] = {
    (void *)DeleteStatusPointSinkEntryThunk,
    (void *)QueueStatusPointPushThunk,
    (void *)ExecuteStatusPointPushThunk,
    0
};
static void RegisterLevelScreenPaletteRemaps(char *pScreen);
static void ReleaseLevelScreenPaletteRemaps(char *pScreen);

static void *g_GAME_RegistrationInfoScreenVtableSlots[14];
static int g_fGAMERegistrationInfoScreenVtableInitialized;
static char g_GAME_RegistrationNameBuffer[0x40];

static int LEMBALL_FASTCALL GetRegistrationInfoRequestedMode(void *pObject) {
    return *(int *)((char *)pObject + 0x14);
}

static int LEMBALL_FASTCALL PollRegistrationInfoTimeout(void *pObject) {
    char *pScreen;

    pScreen = (char *)pObject;
    if (*(DWORD *)(pScreen + 0x98) < timeGetTime()) {
        *(int *)(pScreen + 0x38) = 1;
        *(int *)(pScreen + 0x14) = 1;
    }
    return *(int *)(pScreen + 0x38);
}

static void LEMBALL_FASTCALL DrawRegistrationInfoScreen(void *pObject) {
    char *pScreen;
    MappedListResourceTable *pMappedList;
    short Point[2];
    short Relative[2];
    const char *pszBuild;
    const char *pszName;

    pScreen = (char *)pObject;
    pMappedList = *(MappedListResourceTable **)(pScreen + 0x90);
    if (pMappedList == 0 || *(void **)(pScreen + 0x20) == 0) {
        return;
    }
    pMappedList->ResetDrawEntryPool();
    Relative[0] = 0;
    Relative[1] = 0;
    Point[0] = (short)(*(short *)(pScreen + 0x18) / 2 - 56);
    Point[1] = (short)(*(short *)(pScreen + 0x1a) / 2 - 18);
    pMappedList->SubmitRenderEntry(
        *(void **)(pScreen + 0x20), Point, Relative,
        0x32, (void *)"Registered to", 0x20, 0);
    pszName = g_GAME_RegistrationNameBuffer[0] != 0
        ? g_GAME_RegistrationNameBuffer : "Lemmings Paintball";
    Point[0] = (short)(*(short *)(pScreen + 0x18) / 2 -
                       (int)strlen(pszName) * 4);
    Point[1] = (short)(*(short *)(pScreen + 0x1a) / 2 + 17);
    pMappedList->SubmitRenderEntry(
        *(void **)(pScreen + 0x20), Point, Relative,
        0x32, (void *)pszName, 0x20, 0);
    pszBuild = "ViSOS Build 201";
    Point[0] = (short)(*(short *)(pScreen + 0x18) / 2 -
                       (int)strlen(pszBuild) * 4);
    Point[1] = (short)(*(short *)(pScreen + 0x1a) / 2 + 70);
    pMappedList->SubmitRenderEntry(
        *(void **)(pScreen + 0x20), Point, Relative,
        0x32, (void *)pszBuild, 0x20, 0);
}

static void LEMBALL_FASTCALL DestroyRegistrationInfoScreen(void *pObject) {
    char *pScreen;
    MappedListResourceTable *pMappedList;

    pScreen = (char *)pObject;
    if (g_pSharedRenderDispatchQueue != 0) {
        ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
            ->UnregisterOrderedRenderDispatchClient(pScreen + 4, 0);
    }
    if (*(void **)(pScreen + 0x60) != 0) {
        ReleaseTypedResourceObjectReference(*(void **)(pScreen + 0x60));
        *(void **)(pScreen + 0x60) = 0;
    }
    pMappedList = *(MappedListResourceTable **)(pScreen + 0x90);
    if (pMappedList != 0) {
        pMappedList->ReleaseListResourceSlot(0x32);
        pMappedList->Destroy();
        FreeVSMemBlock(pMappedList);
        *(void **)(pScreen + 0x90) = 0;
    }
    if (*(void **)(pScreen + 0x84) != 0) {
        ReleaseTypedResourceObjectReference(*(void **)(pScreen + 0x84));
        *(void **)(pScreen + 0x84) = 0;
    }
}

static void *LEMBALL_FASTCALL DeleteRegistrationInfoScreen(
    void *pObject, int, int fDelete) {
    DestroyRegistrationInfoScreen(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

static int LEMBALL_FASTCALL GetLevelScreenRequestedMode(void *pObject) {
    return *(int *)((char *)pObject + 0x8fc);
}

static int LEMBALL_FASTCALL GetLevelScreenTransitionStatus(void *pObject) {
    return *(int *)((char *)pObject + 0xa78);
}

static void LEMBALL_FASTCALL DispatchLevelScreenRenderStatePop(
    void *, int, void *pContext)
{
    typedef int (LEMBALL_FASTCALL *GetUploadStateProc)(void *pObject);
    int nUploadState;
    int nDepth;

    nUploadState = ((GetUploadStateProc)(*(void ***)
        *(void **)((char *)pContext + 0x0c))[2])(
            *(void **)((char *)pContext + 0x0c));
    nDepth = *(int *)((char *)(unsigned long)nUploadState + 0x44) - 1;
    *(int *)((char *)(unsigned long)nUploadState + 0x44) = nDepth;
    *(unsigned char *)((char *)(unsigned long)nUploadState + 0x48) =
        *(unsigned char *)((char *)(unsigned long)nUploadState +
                           0x38 + nDepth);
}

static void LEMBALL_FASTCALL DrawReconstructedLevelScreen(
    void *pObject, int nUnusedEdx, void *pFrameArgument) {
    typedef void (LEMBALL_FASTCALL *DrawProc)(void *pObject);
    char *pScreen;
    void *pManager;
    void *pPanel;
    void *pOverlay;
    void *pRenderQueue;
    void *pGeometryContext;
    int nUploadState;

    (void)nUnusedEdx;
    (void)pFrameArgument;
    pScreen = (char *)pObject;
    if (*(void **)(pScreen + 0x970) == 0 ||
        *(short *)(pScreen + 0x958) <= 0 ||
        *(short *)(pScreen + 0x95a) <= 0) {
        return;
    }
    pManager = *(void **)(pScreen + 0xa40);
    if (pManager == 0 || *(int *)((char *)pManager + 0x84) == 0) {
        return;
    }
    ++*(int *)(pScreen + 0x190);
    *(short *)(pScreen + 0x166) =
        (short)(g_nLevelFrameClockTimeMs / 100);
    {
        typedef int (LEMBALL_FASTCALL *GetUploadStateProc)(void *pObject);
        typedef void (LEMBALL_FASTCALL *QueueProc)(
            void *pObject, int nUnusedEdx, void *pQueue);

        pRenderQueue = *(void **)(pScreen + 0x970);
        pGeometryContext = *(void **)((char *)pRenderQueue + 0x0c);
        nUploadState = ((GetUploadStateProc)(*(void ***)pGeometryContext)[2])(
            pGeometryContext);
        ((QueueProc)(*(void ***)(pScreen + 0x990))[1])(
            pScreen + 0x990, 0, pRenderQueue);
        InitializeHelperUploadStatePending(nUploadState);
        *(unsigned short *)(pScreen + 0x964) = (unsigned short)
            EmitLevelGameModeDynamicRenderEntries(
                *(void **)(pScreen + 0x96c),
                *(void **)(pScreen + 0x95c));
        ((QueueProc)(*(void ***)(pScreen + 0x900))[1])(
            pScreen + 0x900, 0, pRenderQueue);
        RenderReconstructedLevelScreenTerrain(pScreen);
        DrawReconstructedLevelScreenStatusText(pScreen);

        pPanel = *(void **)(pScreen + 0x97c);
        pOverlay = pPanel != 0 ? *(void **)((char *)pPanel + 0x2c) : 0;
        if (pOverlay != 0) {
            ((DrawProc)(*(void ***)pOverlay)[47])(pOverlay);
        }
        ((QueueProc)(*(void ***)(pScreen + 0x908))[1])(
            pScreen + 0x908, 0, pRenderQueue);
        {
            char *pFrameEntry;
            char *pOwner;
            char *pDispatchObject;
            int iFrameEntry;

            iFrameEntry = *(int *)(pScreen + 0x2188);
            if (iFrameEntry < 210) {
                *(int *)(pScreen + 0x2188) = iFrameEntry + 1;
                pFrameEntry = pScreen + 0x1468 + iFrameEntry * 0x10;
                pOwner = *(char **)((char *)pRenderQueue + 0x0c);
                pDispatchObject = pOwner + 0x40 +
                    *(int *)(*(char **)(pOwner + 0x40) + 4);
                *(short *)(pFrameEntry + 4) =
                    *(short *)(pDispatchObject + 0x14);
                *(short *)(pFrameEntry + 6) =
                    *(short *)(pDispatchObject + 0x16);
                *(short *)(pFrameEntry + 8) = 0;
                *(short *)(pFrameEntry + 0x0a) = 0;
                *(int *)(pFrameEntry + 0x0c) = 0;
                ((QueueProc)(*(void ***)pFrameEntry)[1])(
                    pFrameEntry, 0, pRenderQueue);
            }
        }
    }
}

static void ActivateReconstructedLevelScreen(char *pScreen) {
    GamePrimaryContext *pPrimaryContext;
    char *pLevelMode;
    int nScale;

    pPrimaryContext = *(GamePrimaryContext **)(pScreen + 0x978);
    pLevelMode = *(char **)(pScreen + 0x96c);
    SetLevelScreenStatusIndicatorModeThunk(1, 0);
    nScale = 2;
    if (g_fCompactPrimaryContextLayout != 0 ||
        g_fStartupEditLevelOverride != 0 ||
        g_fZoomedViewportEnabled != 0) {
        nScale = 1;
    }
    *(unsigned short *)(pScreen + 0x160) = (unsigned short)nScale;
    *(short *)(pScreen + 0x940) =
        (short)(*(short *)((char *)pPrimaryContext + 8) / nScale);
    *(short *)(pScreen + 0x942) =
        (short)(*(short *)((char *)pPrimaryContext + 10) / nScale);
    if (g_fZoomedViewportEnabled == 0 ||
        g_fCompactPrimaryContextLayout != 0) {
        *(short *)(pScreen + 0x958) = *(short *)(pScreen + 0x940);
        *(short *)(pScreen + 0x95a) = *(short *)(pScreen + 0x942);
        *(int *)(pScreen + 0x950) = 0;
        *(int *)(pScreen + 0x954) = 0;
    } else {
        *(short *)(pScreen + 0x958) = 0x140;
        *(short *)(pScreen + 0x95a) = 0xf0;
        *(int *)(pScreen + 0x950) = 0xa0;
        *(int *)(pScreen + 0x954) = 0x78;
    }
    *(short *)(pScreen + 0x8d2) = 0x33;
    *(short *)(pScreen + 0x8d4) = 0x20;
    *(short *)(pScreen + 0x8d6) =
        (short)(*(short *)(pScreen + 0x958) - 0x43);
    *(short *)(pScreen + 0x8d8) = 8;
    *(short *)(pScreen + 0x8da) = 0x60;
    *(short *)(pScreen + 0x8dc) = 0x20;
    *(short *)(pScreen + 0x8de) = 0x10;
    *(short *)(pScreen + 0x8e0) = 8;
    *(int *)(pScreen + 0x994) = 1;
    *(int *)(pScreen + 0x2214) = 1;
    *(int *)(pScreen + 0x2218) = 0;
    pPrimaryContext->SetWindowOwnerScaleFactor(nScale);
    if (g_pLevelDemoPlaybackController != 0) {
        *(void **)((char *)g_pLevelDemoPlaybackController + 0x34) =
            pPrimaryContext;
        if (*(int *)((char *)g_pLevelDemoPlaybackController + 0x4c) != 0) {
            *(DWORD *)((char *)g_pLevelDemoPlaybackController + 0x38) =
                timeGetTime();
            *(int *)((char *)g_pLevelDemoPlaybackController + 0x3c) = 0;
        }
    }
    pPrimaryContext->SampleRootHelperGeometryAndDispatchRenderGroups(0);
    if (*(void **)(pScreen + 0x97c) == 0) {
        void *pActionPanel = AllocateVSMemBlock(0x58);
        if (pActionPanel != 0) {
            *(void **)(pScreen + 0x97c) =
                ConstructLevelScreenActionPanelResources(
                    pActionPanel, pScreen);
        }
    }
    if (*(int *)(pLevelMode + 0x64) == 0) {
        RequestLocalLevelGameStateChange(pLevelMode, 2);
        *(int *)(pLevelMode + 0x68) = 1;
    } else if (g_pNetworkLobbyVsnetRuntime != 0) {
        *(int *)(pLevelMode + 0x6c) = 1;
        *(int *)(pLevelMode + 0x70) = 0;
        *(int *)((char *)g_pNetworkLobbyVsnetRuntime + 0x48) = 3;
        *(int *)((char *)g_pNetworkLobbyVsnetRuntime + 0x4c) = 0;
    }
}

static void LEMBALL_FASTCALL UpdateReconstructedLevelScreen(void *pObject) {
    char *pScreen;
    void *pManager;
    void *pLevelMode;

    pScreen = (char *)pObject;
    if (*(void **)(pScreen + 0x97c) != 0) {
        DestroyLevelScreenActionPanelResources(
            *(void **)(pScreen + 0x97c));
        FreeVSMemBlock(*(void **)(pScreen + 0x97c));
        *(void **)(pScreen + 0x97c) = 0;
    }
    pManager = *(void **)(pScreen + 0xa40);
    pLevelMode = *(void **)(pScreen + 0x96c);
    if (pManager != 0 && pLevelMode != 0 &&
        *(int *)((char *)pManager + 0x84) == 0) {
        RebuildLevelScreenVariantResourceSet(
            pManager, *(int *)((char *)pLevelMode + 0x60));
        ActivateReconstructedLevelScreen(pScreen);
    }
}

static void LEMBALL_FASTCALL DestroyReconstructedLevelScreen(void *pObject) {
    char *pScreen;
    MappedListResourceTable *pMappedList;
    int *pGrid;
    void *pPrimaryContext;

    pScreen = (char *)pObject;
    pMappedList = *(MappedListResourceTable **)(pScreen + 0x78);
    if (pMappedList != 0) {
        pMappedList->ReleaseListResourceSlot(0x115);
        pMappedList->ReleaseListResourceSlot(0xf8);
        pMappedList->Destroy();
        FreeVSMemBlock(pMappedList);
        *(void **)(pScreen + 0x78) = 0;
    }
    if (*(void **)(pScreen + 0xa40) != 0) {
        DestroyLevelScreenVariantRenderManager(
            *(void **)(pScreen + 0xa40));
        FreeVSMemBlock(*(void **)(pScreen + 0xa40));
        *(void **)(pScreen + 0xa40) = 0;
    }
    pGrid = *(int **)(pScreen + 0x934);
    if (pGrid != 0) {
        FreeVSMemBlock((void *)(unsigned long)pGrid[3]);
        FreeVSMemBlock(pGrid);
        *(void **)(pScreen + 0x934) = 0;
    }
    if (g_pSharedRenderDispatchQueue != 0) {
        ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
            ->UnregisterOrderedRenderDispatchClient(pScreen + 4, 0);
    }
    pPrimaryContext = *(void **)(pScreen + 0x978);
    if (pPrimaryContext != 0 && *(void **)((char *)pPrimaryContext + 0x1c) != 0) {
        RemoveGeometryOwnerChild(
            *(void **)((char *)pPrimaryContext + 0x1c), 0,
            pScreen + 0x14);
    }
    FreeVSMemBlock(*(void **)(pScreen + 0x95c));
    FreeVSMemBlock(*(void **)(pScreen + 0x960));
    *(void **)(pScreen + 0x95c) = 0;
    *(void **)(pScreen + 0x960) = 0;
    ReleaseLevelScreenPaletteRemaps(pScreen);
    SetLevelScreenStatusIndicatorModeThunk(0, 0);
}

static void *LEMBALL_FASTCALL DeleteReconstructedLevelScreen(
    void *pObject, int, int fDelete)
{
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

static void InitializeLevelScreenHotspotEntry(void *pObject) {
    char *pEntry;

    pEntry = (char *)pObject;
    *(unsigned short *)(pEntry + 0x2c) = 0xffff;
    *(int *)(pEntry + 0x38) = 0xaa55aa55;
    *(int *)(pEntry + 0x3c) = 0xaa55aa55;
    *(int *)(pEntry + 0x40) = 0xaa55aa55;
    *(int *)(pEntry + 0x34) = 0;
    *(int *)(pEntry + 0x30) = 0;
    *(int *)(pEntry + 0x18) = 0;
    *(int *)(pEntry + 0x20) = 0;
}

static const unsigned char g_GAME_LevelPaletteRemapSourceIndices[17] = {
    250, 204, 205, 206, 118, 107, 101, 95, 85,
    75, 69, 59, 49, 46, 44, 37, 48
};
static const unsigned char g_GAME_LevelPaletteRemapTargetIndices[4][17] = {
    {224,225,226,227,228,229,230,231,232,232,233,234,234,234,234,235,235},
    {192,193,194,195,196,197,198,199,200,200,201,202,202,202,202,203,203},
    {208,209,210,211,212,213,214,215,216,216,217,218,218,218,218,219,219},
    {179,180,181,182,183,184,185,186,187,187,188,189,189,189,189,190,191}
};
static unsigned char g_GAME_LevelPaletteSparsePairs[5] = {
    2, 0xf1, 0x51, 0xa8, 0x6c
};

static void RegisterLevelScreenPaletteRemaps(char *pScreen) {
    void *pPalette;
    unsigned int cbPalette;
    unsigned char *pRemap;
    int i;
    int j;

    pPalette = LoadPalResource(0x2e);
    cbPalette = *(unsigned int *)((char *)pPalette + 0x48);
    for (i = 0; i < 4; ++i) {
        pRemap = (unsigned char *)AllocateVSMemBlock(cbPalette);
        *(unsigned char **)(pScreen + 0x54 + i * 4) = pRemap;
        if (pRemap != 0) {
            for (j = 0; j < (int)cbPalette; ++j) {
                pRemap[j] = (unsigned char)j;
            }
            for (j = 0; j < 17; ++j) {
                pRemap[g_GAME_LevelPaletteRemapSourceIndices[j]] =
                    g_GAME_LevelPaletteRemapTargetIndices[i][j];
            }
        }
        *(void **)(pScreen + 0x64 + i * 4) =
            RegisterPaletteRemapVariant(0x2e, pRemap, 0);
    }
    *(void **)(pScreen + 0x74) = RegisterPaletteRemapVariant(
        0x2e, g_GAME_LevelPaletteSparsePairs, 2);
    ReleaseTypedResourceObjectReference(pPalette);

    pRemap = (unsigned char *)AllocateVSMemBlock(0x100);
    if (pRemap != 0) {
        for (i = 0; i < 0x100; ++i) {
            pRemap[i] = (unsigned char)i;
        }
        pRemap[0x37] = 0x52;
        pRemap[0x5c] = 0x74;
        pRemap[0x71] = 0x74;
        pRemap[0x75] = 0x74;
        pRemap[0x80] = 0x8c;
    }
    *(void **)(pScreen + 0x968) =
        RegisterPaletteRemapVariant(0x2e, pRemap, 0);
}

static void ReleaseLevelScreenPaletteRemaps(char *pScreen) {
    int i;

    for (i = 0; i < 4; ++i) {
        ReleasePaletteRemapVariant(*(void **)(pScreen + 0x64 + i * 4));
        *(void **)(pScreen + 0x64 + i * 4) = 0;
        *(void **)(pScreen + 0x54 + i * 4) = 0;
    }
    ReleasePaletteRemapVariant(*(void **)(pScreen + 0x74));
    ReleasePaletteRemapVariant(*(void **)(pScreen + 0x968));
    *(void **)(pScreen + 0x74) = 0;
    *(void **)(pScreen + 0x968) = 0;
}

void *GameScreenObject::ConstructLevelScreen(void *pPrimaryContext,
                                              void *pLevelModeObject,
                                              void *pWindowOwnerContext,
                                              int nStatusIndicatorMode,
                                              short *paRect) {
    char *pScreenObject;
    char *pEntry;
    int *pGrid;
    int cGridEntries;
    int i;
    int nScale;

    pScreenObject = (char *)this;
    InitializeGameStubVtables();
    memset(pScreenObject, 0, 0x2428);

    if (g_fGAMELevelScreenVtableInitialized == 0) {
        for (i = 0; i < 14; ++i) {
            g_GAME_LevelScreenVtableSlots[i] =
                g_GAME_GenericScreenVtableSlots[i];
        }
        g_GAME_LevelScreenVtableSlots[0] =
            (void *)DeleteReconstructedLevelScreen;
        g_GAME_LevelScreenVtableSlots[1] =
            (void *)DestroyReconstructedLevelScreen;
        g_GAME_LevelScreenVtableSlots[2] =
            (void *)DrawReconstructedLevelScreen;
        g_GAME_LevelScreenVtableSlots[7] =
            (void *)UpdateReconstructedLevelScreen;
        g_GAME_LevelScreenVtableSlots[10] =
            (void *)GetLevelScreenTransitionStatus;
        g_GAME_LevelScreenVtableSlots[11] =
            (void *)GetLevelScreenRequestedMode;
        g_fGAMELevelScreenVtableInitialized = 1;
    }

    *(void ***)pScreenObject = g_GAME_LevelScreenVtableSlots;
    InitializeRenderQueueNodeBase(pScreenObject + 4);
    InitializeGeometryOwnerRect(
        pScreenObject + 0x14, 0, (const VS_XYPair *)paRect);

    *(unsigned short *)(pScreenObject + 0xa8) = 0xffff;
    *(int *)(pScreenObject + 0xb4) = 0xaa55aa55;
    *(int *)(pScreenObject + 0xb8) = 0xaa55aa55;
    *(int *)(pScreenObject + 0xbc) = 0xaa55aa55;
    *(unsigned short *)(pScreenObject + 0xf4) = 0xffff;
    *(unsigned short *)(pScreenObject + 0x140) = 0xffff;

    pEntry = pScreenObject + 0x9c0;
    for (i = 0; i < 4; ++i, pEntry += 0x10) {
        InitializePackagedSpriteRenderEntryThunk(pEntry);
    }
    pEntry = pScreenObject + 0xa00;
    for (i = 0; i < 4; ++i, pEntry += 0x10) {
        InitializeRenderRectSinkEntryA(pEntry);
    }
    pEntry = pScreenObject + 0x1468;
    for (i = 0; i < 210; ++i, pEntry += 0x10) {
        InitializeHelperLocalRectUpdateEntry(pEntry);
    }
    InitializeHelperLocalRectUpdateEntry(pScreenObject + 0x2190);
    pEntry = pScreenObject + 0x21d0;
    for (i = 0; i < 2; ++i, pEntry += 0x10) {
        InitializeRenderRectSinkEntryB(pEntry);
    }
    pEntry = pScreenObject + 0x21f4;
    for (i = 0; i < 2; ++i, pEntry += 0x10) {
        InitializeRenderPointRectSinkEntryThunk(pEntry);
    }
    *(void **)(pScreenObject + 0x990) =
        g_pQueuedRenderPointSinkFinalizeThunk;
    *(void ***)(pScreenObject + 0x900) =
        g_GAME_LevelScreenRenderStatePushVtable;
    InitializePackagedSpriteRenderEntryThunk(pScreenObject + 0x998);
    InitializePackagedSpriteRenderEntryThunk(pScreenObject + 0x9a8);
    InitializeLevelSurfaceRectSink(pScreenObject + 0x21a0);
    InitializeLevelNestedRectSink(pScreenObject + 0x21b0);
    InitializeRenderRectSinkEntryB(pScreenObject + 0x21c0);
    if (g_fGAMELevelScreenRenderStatePopVtableInitialized == 0) {
        for (i = 0; i < 4; ++i) {
            g_GAME_LevelScreenRenderStatePopVtable[i] =
                ((void **)g_pQueuedRenderPointSinkFinalizeThunk)[i];
        }
        g_GAME_LevelScreenRenderStatePopVtable[2] =
            (void *)DispatchLevelScreenRenderStatePop;
        g_fGAMELevelScreenRenderStatePopVtableInitialized = 1;
    }
    *(void ***)(pScreenObject + 0x908) =
        g_GAME_LevelScreenRenderStatePopVtable;

    *(int *)(pScreenObject + 0x8ec) = 1;
    *(int *)(pScreenObject + 0x8fc) = 2;
    *(int *)(pScreenObject + 0x1a8) = g_nLevelFrameClockTimeMs;
    *(void **)(pScreenObject + 0x96c) = pLevelModeObject;
    *(void **)(pScreenObject + 0x970) = pWindowOwnerContext;
    *(void **)(pScreenObject + 0x978) = pPrimaryContext;
    *(void **)(pScreenObject + 0x914) =
        (void *)(unsigned long)nStatusIndicatorMode;
    *(int *)(pScreenObject + 0x3c) = 1;
    *(void **)(pScreenObject + 0x974) =
        *(void **)((char *)pLevelModeObject + 0x164);
    if (nStatusIndicatorMode != 0) {
        *(int *)((char *)(unsigned long)nStatusIndicatorMode + 4) = 0;
    }

    if (*(void **)((char *)pPrimaryContext + 0x1c) != 0) {
        AppendGeometryOwnerChild(
            *(void **)((char *)pPrimaryContext + 0x1c), 0,
            pScreenObject + 0x14);
    }

    *(void **)(pScreenObject + 0xa40) = AllocateVSMemBlock(0xb4);
    if (*(void **)(pScreenObject + 0xa40) != 0) {
        ConstructLevelScreenVariantRenderManager(
            *(void **)(pScreenObject + 0xa40),
            (int)(unsigned long)pWindowOwnerContext,
            pPrimaryContext, pLevelModeObject);
    }
    {
        MappedListResourceTable *pMappedList;
        pMappedList = (MappedListResourceTable *)AllocateVSMemBlock(
            sizeof(MappedListResourceTable));
        if (pMappedList != 0) {
            pMappedList = new (pMappedList)
                MappedListResourceTable(0x2b6, 2, 2, 10);
        }
        *(MappedListResourceTable **)(pScreenObject + 0x78) = pMappedList;
        if (pMappedList != 0) {
            pMappedList->LoadListResourceIntoSlot(0x115);
        }
    }
    if (*(void **)(pScreenObject + 0xa40) != 0) {
        ((LevelVariantRenderManagerView *)
             *(void **)(pScreenObject + 0xa40))
            ->ResetOrFlipVariantRenderBuffers();
    }
    if (*(MappedListResourceTable **)(pScreenObject + 0x78) != 0) {
        (*(MappedListResourceTable **)(pScreenObject + 0x78))
            ->ResetDrawEntryPool();
    }
    *(int *)(pScreenObject + 0xc90) = 0;
    *(int *)(pScreenObject + 0x2188) = 0;
    RegisterLevelScreenPaletteRemaps(pScreenObject);

    *(int *)(pScreenObject + 0x994) = 1;
    *(void **)(pScreenObject + 0x95c) = AllocateVSMemBlock(0x3b60);
    if (*(void **)(pScreenObject + 0x95c) != 0) {
        pEntry = (char *)*(void **)(pScreenObject + 0x95c);
        for (i = 0; i < 200; ++i, pEntry += 0x4c) {
            InitializeLevelScreenHotspotEntry(pEntry);
        }
    }
    *(void **)(pScreenObject + 0x960) = AllocateVSMemBlock(0x800);
    if (g_pSharedRenderDispatchQueue != 0) {
        ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
            ->RegisterOrderedRenderDispatchClient(pScreenObject + 4, 0);
    }
    SetLevelFrameClockPauseFlag(0);

    pGrid = (int *)AllocateVSMemBlock(0x1a4);
    *(int **)(pScreenObject + 0x934) = pGrid;
    if (pGrid != 0) {
        memset(pGrid, 0, 0x1a4);
        *(short *)(pGrid + 2) = 0x10;
        *(short *)((char *)pGrid + 10) = 0x10;
        *(short *)(pGrid + 1) = (short)
            *(int *)((char *)(unsigned long)nStatusIndicatorMode + 0x10);
        *(short *)((char *)pGrid + 6) = (short)
            *(int *)((char *)(unsigned long)nStatusIndicatorMode + 0x14);
        cGridEntries = (int)*(short *)(pGrid + 1) *
            (int)*(short *)((char *)pGrid + 6);
        pGrid[0] = cGridEntries;
        pGrid[3] = (int)(unsigned long)
            AllocateVSMemBlock((unsigned int)cGridEntries * 10);
        if (pGrid[3] != 0) {
            memset((void *)(unsigned long)pGrid[3], 0,
                   (unsigned int)cGridEntries * 10);
        }
    }

    *(short *)(pScreenObject + 0x162) = (short)
        *(int *)((char *)(unsigned long)nStatusIndicatorMode + 0x10);
    *(short *)(pScreenObject + 0x164) = (short)
        *(int *)((char *)(unsigned long)nStatusIndicatorMode + 0x14);
    nScale = *(int *)((char *)pPrimaryContext + 0x38);
    if (nScale == 0) nScale = 1;
    *(unsigned short *)(pScreenObject + 0x160) = (unsigned short)nScale;
    *(short *)(pScreenObject + 0x940) = (short)(paRect[0] / nScale);
    *(short *)(pScreenObject + 0x942) = (short)(paRect[1] / nScale);
    *(short *)(pScreenObject + 0x958) =
        *(short *)(pScreenObject + 0x940);
    *(short *)(pScreenObject + 0x95a) =
        *(short *)(pScreenObject + 0x942);
    *(int *)(pScreenObject + 0x8f8) = g_nLevelFrameClockTick;
    *(int *)(pScreenObject + 0x8f0) =
        *(int *)((char *)pLevelModeObject + 0xf0);
    *(int *)(pScreenObject + 0x8f4) =
        *(int *)(pScreenObject + 0x8f0);
    return pScreenObject;
}

void *GameScreenObject::ConstructRegistrationInfoScreen(void *pPrimaryContext,
                                                         void *pWindowOwnerContext,
                                                         short *paRect) {
    char *pScreenObject;
    MappedListResourceTable *pMappedList;
    MogLoadStringResourceObject *pStringResource;
    const char *pszEncoded;
    unsigned int i;

    pScreenObject = (char *)this;
    InitializeGameStubVtables();
    memset(pScreenObject, 0, 0x9c);

    if (g_fGAMERegistrationInfoScreenVtableInitialized == 0) {
        for (i = 0; i < 14; ++i) {
            g_GAME_RegistrationInfoScreenVtableSlots[i] =
                g_GAME_GenericScreenVtableSlots[i];
        }
        g_GAME_RegistrationInfoScreenVtableSlots[0] =
            (void *)DeleteRegistrationInfoScreen;
        g_GAME_RegistrationInfoScreenVtableSlots[1] =
            (void *)DestroyRegistrationInfoScreen;
        g_GAME_RegistrationInfoScreenVtableSlots[2] =
            (void *)DrawRegistrationInfoScreen;
        g_GAME_RegistrationInfoScreenVtableSlots[10] =
            (void *)GetRegistrationInfoRequestedMode;
        g_GAME_RegistrationInfoScreenVtableSlots[11] =
            (void *)PollRegistrationInfoTimeout;
        g_fGAMERegistrationInfoScreenVtableInitialized = 1;
    }
    *(void ***)pScreenObject = g_GAME_RegistrationInfoScreenVtableSlots;
    InitializeRenderQueueNodeBase(pScreenObject + 4);
    *(void **)(pScreenObject + 0x1c) = pPrimaryContext;
    *(void **)(pScreenObject + 0x20) = pWindowOwnerContext;
    *(short *)(pScreenObject + 0x18) = paRect[0];
    *(short *)(pScreenObject + 0x1a) = paRect[1];
    *(void **)(pScreenObject + 0x60) = LoadBitmapResource(0x101);

    pMappedList = (MappedListResourceTable *)
        AllocateVSMemBlock(sizeof(MappedListResourceTable));
    if (pMappedList != 0) {
        pMappedList = new (pMappedList)
            MappedListResourceTable(0x2b6, 1, 10, 0);
        pMappedList->LoadListResourceIntoSlot(0x32);
    }
    *(MappedListResourceTable **)(pScreenObject + 0x90) = pMappedList;

    g_GAME_RegistrationNameBuffer[0] = 0;
    pStringResource = LoadStringResource(0x100);
    *(MogLoadStringResourceObject **)(pScreenObject + 0x84) =
        pStringResource;
    pszEncoded = pStringResource == 0 ? 0 : pStringResource->m_pszText38;
    if (pszEncoded != 0) {
        for (i = 0;
             pszEncoded[i] != 0 &&
             i + 1 < sizeof(g_GAME_RegistrationNameBuffer) &&
             g_GAME_ResourceSignatureKey[i] != 0;
             ++i) {
            g_GAME_RegistrationNameBuffer[i] =
                (char)(((unsigned char)pszEncoded[i] - 1U) ^
                       (unsigned char)g_GAME_ResourceSignatureKey[i]);
        }
        g_GAME_RegistrationNameBuffer[i] = 0;
    }
    *(char **)(pScreenObject + 0x88) = g_GAME_RegistrationNameBuffer;
    *(DWORD *)(pScreenObject + 0x94) = timeGetTime();
    *(DWORD *)(pScreenObject + 0x98) =
        *(DWORD *)(pScreenObject + 0x94) + 5000U;
    if (g_pSharedRenderDispatchQueue != 0) {
        ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
            ->RegisterOrderedRenderDispatchClient(pScreenObject + 4, 0);
    }
    return pScreenObject;
}

void *GameScreenObject::ConstructNetworkLobbyScreen(void *pPrimaryContext,
                                                     void *pWindowOwnerContext,
                                                     short *paRect) {
    return ConstructFunctionalNetworkLobbyScreen(
        this, 0, pPrimaryContext, pWindowOwnerContext, paRect);
}

void *GameScreenObject::ConstructPaintballSequenceScreen(void *pPrimaryContext,
                                                          void *pWindowOwnerContext,
                                                          short *paRect,
                                                          int fFirstSequenceVariant) {
    return ConstructFunctionalPaintballSequenceScreen(
        this, 0, pPrimaryContext, pWindowOwnerContext,
        paRect, fFirstSequenceVariant);
}

void *GameScreenObject::ConstructPasswordEntryScreen(void *pPrimaryContext,
                                                      void *pWindowOwnerContext,
                                                      short *paRect) {
    return ConstructFunctionalPasswordEntryScreen(
        this, 0, pPrimaryContext, pWindowOwnerContext, paRect);
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


struct VS_QueuedRenderSinkValueNode {
    void *m_pValue;
    VS_QueuedRenderSinkValueNode *m_pNext;
    VS_QueuedRenderSinkValueNode *m_pPrevious;

    VS_QueuedRenderSinkValueNode(void *pValue)
        : m_pValue(pValue), m_pNext(0), m_pPrevious(0) {}
};










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

typedef void (LEMBALL_FASTCALL *GamePrimaryContextActiveScreenRenderProc)(void *);
struct GamePrimaryContextActiveScreenRenderVtable {
    void *m_apSlots00[8];
    GamePrimaryContextActiveScreenRenderProc m_pRenderFrame20;
};


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


static char g_GAME_CdromDrivePath[256] = "A:\\";


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



int VS_RootGeometryOwner::GetWindowOwnerState(void) {
    return m_nWindowOwnerState30;
}

// STUB: LEMBALL 0x004010f0
GamePrimaryContext *GamePrimaryContext::ConstructPrimaryContextThunk(GameMainContext *pMainContext) {
    return ConstructPrimaryContext(pMainContext);
}

void VS_WindowOwnerBase::ReservedWindowOwnerSlot37(void) {
}

// STUB: LEMBALL 0x004014b0
void GameLevelProgressState::SetUnlockedPackCapThunk(int nPack, int nCap) {
    SetUnlockedPackCap(nPack, nCap);
}

extern "C" void LEMBALL_FASTCALL AdvanceCurrentLevelProgressThunk(
    void *pProgress) {
    ((GameLevelProgressState *)pProgress)->Advance();
}

extern "C" bool LEMBALL_FASTCALL RetreatCurrentLevelProgressThunk(
    void *pProgress) {
    return ((GameLevelProgressState *)pProgress)->Retreat();
}

extern "C" bool LEMBALL_FASTCALL CanRetreatCurrentLevelProgressThunk(
    void *pProgress) {
    return ((GameLevelProgressState *)pProgress)->CanRetreat();
}

extern "C" bool LEMBALL_FASTCALL IsCurrentLevelProgressAvailableThunk(
    void *pProgress) {
    return ((GameLevelProgressState *)pProgress)->IsCurrentLevelAvailable();
}

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

// STUB: LEMBALL 0x004015e6
GameBaseModeObject *GameBaseModeObject::InitializeThunk(GameMainContext *pMainContext) {
    return Initialize(pMainContext);
}

// STUB: LEMBALL 0x00401839
int RunMainGameSessionThunk(int cArgs, const char *const *ppszArgs) {
    return RunMainGameSession(cArgs, ppszArgs);
}

// STUB: LEMBALL 0x00401875
short *GamePrimaryContext::ComputePrimaryContextCenteredScreenRectThunk(short *paRect,
                                                                          int nLeftOverride,
                                                                          int nTopOverride)

{
    return ComputePrimaryContextCenteredScreenRect(paRect, nLeftOverride, nTopOverride);
}

// STUB: LEMBALL 0x00401884
int PASCAL ReturnZeroBaseModeThunk(
    void *pObject, void *pArgument1, void *pArgument2)
{
    return return_zero_screen_callback_00446990(
        pObject, pArgument1, pArgument2);
}

int VS_WindowOwnerBase::IsCurrentWindowOwnerContext(void) {
    extern void *g_pCachedCurrentContextWindow;

    return *(void **)((char *)this + 0x44) ==
           g_pCachedCurrentContextWindow;
}

// STUB: LEMBALL 0x00401b72
void SetLevelScreenStatusIndicatorModeThunk(int nMode, int nValue) {
    SetLevelScreenStatusIndicatorMode(nMode, nValue);
}

// STUB: LEMBALL 0x00401b90
void *ConstructNetworkLobbyTransportController(void *pModeObject, GameMainContext *pMainContext) {
    GameNetworkLobbyTransportController *pMode;

    pMode = (GameNetworkLobbyTransportController *)pModeObject;
    pMode->InitializeThunk(pMainContext);
    pMode->m_pVtable = g_GAME_NetworkLobbyTransportControllerVtable;
    pMode->m_pRenderQueueNodeVtable =
        g_GAME_NetworkLobbyTransportEventClientVtable;
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

static void DestroyNetworkLobbyTransportController(void *pObject) {
    GameNetworkLobbyTransportController *pMode;
    typedef int (LEMBALL_FASTCALL *GetLobbyScreenStateProc)(
        void *, int);
    void *pStream;

    pMode = (GameNetworkLobbyTransportController *)pObject;
    pMode->m_pVtable = g_GAME_NetworkLobbyTransportControllerVtable;
    pMode->m_pRenderQueueNodeVtable =
        g_GAME_NetworkLobbyTransportEventClientVtable;
    g_pNetworkLobbyTransportController = 0;
    if (g_pActiveNetworkLobbyScreen != 0) {
        if (((GetLobbyScreenStateProc)
             (*(void ***)g_pActiveNetworkLobbyScreen)[10])(
                g_pActiveNetworkLobbyScreen, 0) == 0) {
            StopNetworkLobbyTransportSession(pMode);
        } else {
            CloseNetworkLobbyTransportPeers(pMode);
        }
    }
    pStream = pMode->m_pPeerClearCloseStream;
    if (pStream != 0) {
        ((void (LEMBALL_FASTCALL *)(void *, int, int))
            (*(void ***)pStream)[5])(pStream, 0, 1);
        pMode->m_pPeerClearCloseStream = 0;
    }
    pStream = pMode->m_pPeerDirtyConfirmStream;
    if (pStream != 0) {
        ((void (LEMBALL_FASTCALL *)(void *, int, int))
            (*(void ***)pStream)[5])(pStream, 0, 1);
        pMode->m_pPeerDirtyConfirmStream = 0;
    }
    DestroyBaseModeObject(pMode);
}

static void *LEMBALL_FASTCALL DeleteNetworkLobbyTransportController(
    void *pObject, int, BYTE fDelete) {
    DestroyNetworkLobbyTransportController(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

static void *LEMBALL_FASTCALL
DeleteNetworkLobbyTransportControllerEventClient(
    void *pObject, int nUnusedEdx, BYTE fDelete) {
    return DeleteNetworkLobbyTransportController(
        (char *)pObject - 0x0c, nUnusedEdx, fDelete);
}

// STUB: LEMBALL 0x00401c6c
void ReleaseMainGameVariantResourceMode(GameMainContext *pMainContext) {
    if (pMainContext->m_pVariantMode != 0) {
        DestroyMainGameVariantResourceMode(pMainContext->m_pVariantMode);
        FreeVSMemBlock(pMainContext->m_pVariantMode);
        pMainContext->m_pVariantMode = 0;
    }
}

// STUB: LEMBALL 0x00401d5c
void *ConstructLevelSelectionModeController(void *pModeObject, GameMainContext *pMainContext) {
    GameLevelSelectionModeController *pMode;

    pMode = (GameLevelSelectionModeController *)pModeObject;
    pMode->InitializeThunk(pMainContext);
    pMode->m_pVtable = g_GAME_LevelSelectionModeVtable;
    pMode->m_pRenderQueueNodeVtable =
        g_GAME_LevelSelectionModeEventClientVtable;
    return pModeObject;
}

static void *LEMBALL_FASTCALL DeleteLevelSelectionModeEventClientThunk(
    void *pObject, int, BYTE fDelete) {
    return DeleteLevelSelectionModeThunk(
        (char *)pObject - 0x0c, 0, fDelete);
}

static void *LEMBALL_FASTCALL DeleteLevelSelectionModeThunk(
    void *pObject, int, BYTE fDelete) {
    DestroyBaseModeObject(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

void VS_WindowOwnerBase::ReservedWindowOwnerSlot19(void) {
}

// STUB: LEMBALL 0x00401fb4
void *LEMBALL_FASTCALL DeleteQueuedRenderPointSinkFinalizeThunk(
    void *pObject, int, int fDelete)
{
    return DeleteQueuedRenderPointSinkFinalize(pObject, 0, fDelete);
}

// STUB: LEMBALL 0x00402130
void GameLevelProgressState::ApplyPasswordValueThunk(unsigned int uValue) {
    ApplyPasswordValue(uValue);
}

// STUB: LEMBALL 0x0040227f
void *LEMBALL_FASTCALL DeleteStartupModeEventClientThunk(
    void *pObject, int, BYTE fDelete)

{
    return DeleteStartupMode((char *)pObject - 0x0c, 0, fDelete);
}

// STUB: LEMBALL 0x004022ac
void *ConstructPaintballSequenceModeController(void *pModeObject,
                                               GameMainContext *pMainContext,
                                               int fSequenceVariant)

{
    GamePaintballSequenceModeController *pMode;

    pMode = (GamePaintballSequenceModeController *)pModeObject;
    pMode->InitializeThunk(pMainContext);
    pMode->m_pVtable = (void **)g_pMainMenuModeVtable;
    pMode->m_pRenderQueueNodeVtable = (void **)g_pMainMenuModeRenderThunk;
    pMode->m_fFirstSequenceVariant = fSequenceVariant;
    return pModeObject;
}

// STUB: LEMBALL 0x00402630
int LEMBALL_FASTCALL IsPrimaryContextWindowReadyForPresentThunk(int *pPrimaryContext) {
    return IsPrimaryContextWindowReadyForPresent(pPrimaryContext);
}

// STUB: LEMBALL 0x004026f3
void *LEMBALL_FASTCALL DeleteStartupModeThunk(
    void *pObject, int nUnusedEdx, BYTE fDelete)

{
    return DeleteStartupMode(pObject, nUnusedEdx, fDelete);
}

void GameVariantResourceEntryManager::SetVariantResourceEffectMasterVolumeThunk(unsigned char nVolume) {
    SetVariantResourceEffectMasterVolume(nVolume);
}

// STUB: LEMBALL 0x004027a2
void *LEMBALL_FASTCALL DeleteMainMenuModeThunk(
    void *pObject, int nUnusedEdx, BYTE fDelete)

{
    return DeleteMainMenuMode(pObject, nUnusedEdx, fDelete);
}

// STUB: LEMBALL 0x004027a7
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

void VS_WindowOwnerBase::ReservedWindowOwnerSlot39(void) {
}

void GameVariantResourceEntryManager::SetAudioMusicAndEffectVolumeByteThunk(unsigned char nVolume) {
    SetAudioMusicAndEffectVolumeByte(nVolume);
}

// STUB: LEMBALL 0x00402a7c
GameMainContext *GameMainContext::InitializeMainGameContextThunk(
    const char *pszCmdLine)

{
    return InitializeMainGameContext(pszCmdLine);
}

// STUB: LEMBALL 0x00402b8f
void *ConstructLevelGameMode(void *pModeObject, GameMainContext *pMainContext) {
    GameLevelGameMode *pMode;
    int i;

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
    pMode->m_nReserved130 = 0;
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
    for (i = 0; i < 4; ++i) {
        *(void **)((char *)pMode + 0x1d0 + i * 4) = 0;
    }
    pMode->m_anHeaderDwords[0] = (int)(unsigned long)g_pQueuedRenderPointSinkFinalizeThunk;
    pMode->m_pEffStreamVtable = (void **)g_pMode3Vtable;
    pMode->m_pDeleteThunkVtable = (void **)g_pMode3RenderThunk;
    pMode->m_pMainContext = pMainContext;
    *(int *)((char *)pMode + 0x4c) = 0;
    g_GAME_ManagedEntityRegistryCount = 0;
    ((LevelGameMode *)pMode)->InitializeLevelGameMode();
    return pModeObject;
}

void *LEMBALL_FASTCALL DeleteLevelModeActiveInterfaceThunk(
    void *pActiveInterface, int, BYTE)
{
    return (char *)pActiveInterface - 0x10;
}

void LEMBALL_FASTCALL UpdateLevelModeActiveInterfaceThunk(
    void *pActiveInterface)
{
    char *pMode;
    int nFrameTick;
    int nState;

    pMode = (char *)pActiveInterface - 0x10;
    if (*(int *)(pMode + 0x64) != 0 && g_nSelectedNetworkLobbyPeerId == 0) {
        return;
    }
    if (*(void **)(pMode + 0x164) != 0) {
        void *pDispatchQueue = *(void **)(pMode + 0x164);
        DrainRenderDispatchQueueEntries(
            pDispatchQueue, 0,
            *(unsigned int *)((char *)pDispatchQueue + 0x28));
    }
    if (*(int *)(pMode + 0xd8) != 0) {
        AdvanceLevelFrameClock();
        return;
    }

    nState = *(int *)(pMode + 0x108);
    if (nState != 0 && nState != 2 && nState != 4 && nState != 6) {
        AdvanceLevelFrameClock();
        return;
    }
    if (*(int *)(pMode + 0x68) == 0 || *(int *)(pMode + 0x6c) != 0) {
        return;
    }

    AdvanceLevelFrameClock();
    nFrameTick = g_nLevelFrameClockTick;
    *(int *)(pMode + 0x88) = 1;
    if (*(int *)(pMode + 0xdc) != 0) {
        *(int *)(pMode + 0xe0) = nFrameTick;
        *(int *)(pMode + 0xdc) = 0;
        *(int *)(pMode + 0xe4) = *(int *)(pMode + 0xec);
    } else {
        if (g_fLevelEndStateRequestLatched == 0 &&
            *(int *)(pMode + 0xe4) < 600) {
            *(int *)(pMode + 0xe4) = *(int *)(pMode + 0xec) -
                (nFrameTick - *(int *)(pMode + 0xe0)) / 0x14;
        }
        if (*(int *)(pMode + 0xe4) + *(int *)(pMode + 0xe8) < 0) {
            RequestLocalLevelGameStateChange(pMode, 7);
            *(int *)(pMode + 0xe4) = -1 - *(int *)(pMode + 0xe8);
        }
        if (*(int *)(pMode + 0x58) == 0 &&
            (unsigned int)*(int *)(pMode + 0x5c) <
                (unsigned int)(nFrameTick - *(int *)(pMode + 0xe0))) {
            *(int *)(pMode + 0x58) = 1;
        }
    }
    if (*(void **)(pMode + 0x174) != 0) {
        ServiceCachedChunkObjects(*(void **)(pMode + 0x174));
    }
    if (*(void **)(pMode + 0x170) != 0) {
        ServiceCachedChunkObjects(*(void **)(pMode + 0x170));
    }
    if (*(void **)(pMode + 0x194) != 0) {
        AdvanceAnimChunkFrames(*(void **)(pMode + 0x194));
    }
    if (g_pLevelChunkStreamDispatcher != 0) {
        ServiceRegisteredLevelChunkStreams(g_pLevelChunkStreamDispatcher);
    }
}

void RequestLocalLevelGameStateChange(void *pLevelMode, int nState) {
    char *pMode;

    pMode = (char *)pLevelMode;
    if (*(int *)(pMode + 0x64) != 0) {
        return;
    }
    if (nState == 3) {
        *(int *)((char *)g_pLevelProgressState + 0x0c) = 2;
        *(int *)(pMode + 0x108) = 3;
    } else if (nState == 5) {
        if (*(int *)((char *)g_pLevelProgressState + 0x0c) == 0) {
            *(int *)((char *)g_pLevelProgressState + 0x0c) = 3;
        }
        *(int *)(pMode + 0x108) = 5;
    } else if (nState == 7) {
        *(int *)((char *)g_pLevelProgressState + 0x0c) = 4;
        *(int *)(pMode + 0x108) = 5;
    } else {
        *(int *)(pMode + 0x108) = nState;
    }
}

// STUB: LEMBALL 0x00402d24
void LEMBALL_FASTCALL UpdateBaseModeThunk(GameBaseModeObject *pMode) {
    pMode->Update();
}

// STUB: LEMBALL 0x00402d29
void *LEMBALL_FASTCALL DeleteMainMenuModeEventClientThunk(
    void *pObject, int, BYTE fDelete)

{
    return DeleteMainMenuMode((char *)pObject - 0x0c, 0, fDelete);
}

void VS_WindowOwnerBase::CacheCurrentContextWindow(void) {
    extern void *g_pCachedCurrentContextWindow;
    extern void *g_pCachedCurrentContextOwner;

    g_pCachedCurrentContextWindow = *(void **)((char *)this + 0x44);
    g_pCachedCurrentContextOwner = this;
}

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

// STUB: LEMBALL 0x00403152
int __stdcall ValidateResourceFileSignatureThunk(void) {
    return ValidateResourceFileSignature();
}

// STUB: LEMBALL 0x00403323
unsigned int GameLevelProgressState::ComputeLevelPasswordChecksumThunk(unsigned int uValue) {
    return ComputeLevelPasswordChecksum(uValue);
}

// STUB: LEMBALL 0x00403378
void LEMBALL_FASTCALL NoopBaseModeThunk(void *) {
}

void VS_WindowOwnerBase::ReservedWindowOwnerSlot20(void) {
}

// STUB: LEMBALL 0x004034fe
int GameLevelProgressState::ParsePasswordDigitsThunk(void) {
    return ParsePasswordDigits();
}

void VS_RootGeometryOwner::SetWindowOwnerState(int nState) {
    m_nWindowOwnerState30 = nState;
}

// STUB: LEMBALL 0x004036e3
int PASCAL ComputePowerOfTwoThunk(int nExponent) {
    return ComputePowerOfTwo(nExponent);
}

// STUB: LEMBALL 0x00403774
int LEMBALL_FASTCALL ReturnFalseBaseModeThunk(void *) {
    return 0;
}

// STUB: LEMBALL 0x00403814
void LEMBALL_FASTCALL RequestFramedScreenTransitionMode2Thunk(void *pScreen) {
    RequestFramedScreenTransitionMode2(pScreen);
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

// FUNCTION: LEMBALL 0x00406a90
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

// FUNCTION: LEMBALL 0x00406ad0
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

// FUNCTION: LEMBALL 0x00406b30
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

// FUNCTION: LEMBALL 0x00406ba0
unsigned int GameLevelProgressState::ComputeLevelPasswordChecksum(unsigned int uValue) {
    unsigned int uHigh;
    unsigned int uMiddle;

    uHigh = uValue >> 16;
    uMiddle = uValue >> 8;
    return (uHigh + uMiddle + uValue) & 0x1f;
}

// FUNCTION: LEMBALL 0x00406bc0
int PASCAL ComputePowerOfTwo(int nExponent) {
    int nValue;

    nValue = 1;
    while (nExponent != 0) {
        nValue *= 2;
        --nExponent;
    }
    return nValue;
}

// FUNCTION: LEMBALL 0x00406c00
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

// FUNCTION: LEMBALL 0x00406ca0
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

// FUNCTION: LEMBALL 0x00406d80
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

// FUNCTION: LEMBALL 0x00406dd0
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

// FUNCTION: LEMBALL 0x00406df0
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

// FUNCTION: LEMBALL 0x004071d0
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
        CallDeleteVirtual(pMainContext->m_pActiveMode, 0, 1);
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
        g_pLevelProgressState = 0;
    }

    SetVisualSciencesRegistryRunningState(g_GAME_WindowTitle, 0);
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
            ((GameBaseModeObject *)pModeObject)->InitializeThunk(pMainContext);
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
            ((GameBaseModeObject *)pModeObject)->InitializeThunk(pMainContext);
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
            ((GameBaseModeObject *)pModeObject)->InitializeThunk(pMainContext);
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
            ((GameBaseModeObject *)pModeObject)->InitializeThunk(pMainContext);
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

// FUNCTION: LEMBALL 0x004077e0
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
            ((void (*)(void *, DWORD))(*(void ***)pProcessingStatus)[1])(
                pProcessingStatus,
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

// FUNCTION: LEMBALL 0x004078f0
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
        ((void (*)(void *, DWORD))(*(void ***)pRefreshingStatus)[1])(
            pRefreshingStatus,
            dwNow - *(DWORD *)((char *)pRefreshingStatus + 0x20));
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

// FUNCTION: LEMBALL 0x00407f40
void *LEMBALL_FASTCALL DeleteStartupMode(
    void *pObject, int, BYTE fDelete)

{
    DestroyBaseModeObject(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00407f70
void *LEMBALL_FASTCALL DeleteMainMenuMode(
    void *pObject, int, BYTE fDelete)

{
    DestroyBaseModeObject(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

/* OptionsMode has the same base teardown as the startup and main-menu modes,
 * but its two vtables use distinct scalar-delete entry points. */
static void *LEMBALL_FASTCALL DeleteOptionsModeThunk(
    void *pObject, int, BYTE fDelete)
{
    DestroyBaseModeObject(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

static void *LEMBALL_FASTCALL DeleteOptionsModeEventClientThunk(
    void *pObject, int, BYTE fDelete)
{
    return DeleteOptionsModeThunk((char *)pObject - 0x0c, 0, fDelete);
}

// FUNCTION: LEMBALL 0x00408080
void SetLevelFrameClockPauseFlag(int fPaused) {
    g_fLevelFrameClockPaused = fPaused;
}

// FUNCTION: LEMBALL 0x00408dc0
void GameLevelProgressState::Advance(void) {
    int nPack;
    int nLevelCount;

    nPack = m_nCurrentPack;
    switch (nPack) {
    case 0:
        nLevelCount = 0x18;
        break;
    case 1:
        nLevelCount = 0x19;
        break;
    case 2:
        nLevelCount = 0x1c;
        break;
    case 3:
        nLevelCount = 0x15;
        break;
    case 4:
        nLevelCount = 0x0b;
        break;
    }
    if (m_nCurrentLevel < nLevelCount) {
        ++m_nCurrentLevel;
    }
    if (nPack != 4 &&
        m_anUnlockedPackCaps[nPack] < m_nCurrentLevel) {
        m_anUnlockedPackCaps[nPack] = m_nCurrentLevel;
    }
    m_anSnapshotPackCaps[m_nCurrentPack] = m_nCurrentLevel;
    IsCurrentLevelAvailable();
}

// FUNCTION: LEMBALL 0x00408e40
bool GameLevelProgressState::Retreat(void) {
    if (0 < m_nCurrentLevel) {
        --m_nCurrentLevel;
    }
    m_anSnapshotPackCaps[m_nCurrentPack] = m_nCurrentLevel;
    return m_nCurrentLevel != 0;
}

// FUNCTION: LEMBALL 0x00408fa0
bool GameLevelProgressState::IsCurrentLevelAvailable(void) {
    bool fAvailable;

    if (m_nCurrentPack == 4) {
        fAvailable = true;
        if (11 <= m_nCurrentLevel) {
            return false;
        }
    } else {
        fAvailable =
            m_anUnlockedPackCaps[m_nCurrentPack] != m_nCurrentLevel;
    }
    return fAvailable;
}

// FUNCTION: LEMBALL 0x00408fd0
bool GameLevelProgressState::CanRetreat(void) {
    return m_nCurrentLevel != 0;
}

// FUNCTION: LEMBALL 0x00408fe0
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

    if (m_nCurrentPack == nPack && m_nCurrentLevel > nCap) {
        nCap = m_nCurrentLevel;
    }
    if (nPackLevelCount >= nCap) {
        m_anUnlockedPackCaps[nPack] = nCap;
        return;
    }
    m_anUnlockedPackCaps[nPack] = nPackLevelCount;
}

// FUNCTION: LEMBALL 0x00410d00
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

// FUNCTION: LEMBALL 0x00431590
GamePrimaryContext *GamePrimaryContext::ConstructPrimaryContext(GameMainContext *pMainContext) {
    GamePrimaryContext *pContext;
    void *pRenderQueueNode;

    pContext = this;
    InitializeGameStubVtables();
    pRenderQueueNode = (char *)pContext + 0x90;
    ((GameWindowOwnerRenderContext *)this)->ConstructWindowOwnerRenderContext();
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

    SetLevelScreenStatusIndicatorModeThunk(2, 0);
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

// FUNCTION: LEMBALL 0x004317c0
void LEMBALL_FASTCALL DestroyPrimaryContextActiveScreenForVtable(
    void *pPrimaryContext)
{
    void *pActiveScreen;

    SetLevelScreenStatusIndicatorModeThunk(0, 0);
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

// FUNCTION: LEMBALL 0x00431810
void GamePrimaryContext::DispatchPrimaryContextActiveScreenFrame(
    void *pFrameArgument)

{
    void *pActiveScreen;
    GamePrimaryContextActiveScreenRenderVtable *pRenderVtable;

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
        pRenderVtable =
            *(GamePrimaryContextActiveScreenRenderVtable **)pActiveScreen;
        ++m_nB8;
        pRenderVtable->m_pRenderFrame20(pActiveScreen);
    }
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

// FUNCTION: LEMBALL 0x00431880
void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x0C(
    void *pPrimaryContext, int, int)
{
    void *pActiveScreen;

    pActiveScreen = *(void **)((char *)pPrimaryContext + 0xd4);
    if (pActiveScreen != 0) {
        ((GameActiveScreenSlot0x0CInterface *)pActiveScreen)
            ->ForwardPrimaryContext((char *)pPrimaryContext + 8);
    }
}

// FUNCTION: LEMBALL 0x004318a0
void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x14(void *pPrimaryContext) {
    void *pActiveScreen;

    pActiveScreen = *(void **)((char *)pPrimaryContext + 0xd4);
    if (pActiveScreen != 0) {
        ((GameActiveScreenVirtualInterface *)pActiveScreen)
            ->ForwardPrimaryContext((char *)pPrimaryContext + 8);
    }
}

// FUNCTION: LEMBALL 0x004318c0
int LEMBALL_FASTCALL IsPrimaryContextWindowReadyForPresent(int *pPrimaryContext) {
    short nWidth;
    short nHeight;
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

struct GameLevelModePrefixInterface {
    virtual void Reserved00(void) = 0;
    char m_abReserved04[0x0c];
};

struct GameLevelModeActiveInterface {
    virtual void Reserved00(void) = 0;
};

struct GameLevelModeObjectView : public GameLevelModePrefixInterface,
                                 public GameLevelModeActiveInterface {
    char m_abReserved14[0xfc];
    int m_nStatusIndicatorMode110;
};
DECOMP_SIZE_ASSERT(GameLevelModeObjectView, 0x114);

// FUNCTION: LEMBALL 0x00431990
void GamePrimaryContext::SwitchPrimaryContextScreen(int nMode) {
    GamePrimaryContext *pContext;
    void *pScreenObject;
    short aRect[4];
    int nUploadState;
    GameLevelModeActiveInterface **ppActiveLevelMode;
    GameLevelModeObjectView *pActiveLevelMode;

    pContext = this;
    if (pContext->m_nActiveScreenMode == nMode) {
        return;
    }

    pContext->SetWindowOwnerScaleFactor(1);
    aRect[0] = pContext->m_Size08.m_x;
    aRect[1] = pContext->m_Size08.m_y;
    aRect[3] = 0;
    aRect[2] = 0;

    nUploadState = ((GameWindowOwnerContext *)pContext->m_pWindowOwnerContext)
        ->m_pStateSubobject->GetCurrentWindowOwnerState();
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
        ppActiveLevelMode = (GameLevelModeActiveInterface **)
            &pContext->m_pMainContext->m_pActiveMode;
        pContext->m_pActiveLevelMode =
            (GameLevelModeObjectView *)
                *ppActiveLevelMode;
        pActiveLevelMode =
            (GameLevelModeObjectView *)
                *ppActiveLevelMode;
        pContext->m_nLevelScreenStatusIndicatorMode =
            pActiveLevelMode->m_nStatusIndicatorMode110;
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

char *FormatCurrentLevelPassword(void) {
    return ((GameLevelProgressState *)g_pLevelProgressState)->FormatPassword();
}

int ValidateCurrentLevelPassword(char *pszPassword) {
    return ((GameLevelProgressState *)g_pLevelProgressState)
        ->ValidateAndApplyPassword(pszPassword);
}

void SnapshotCurrentLevelProgress(void) {
    ((GameLevelProgressState *)g_pLevelProgressState)->Snapshot();
}

// FUNCTION: LEMBALL 0x00431ed0
void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x30(
    void *pPrimaryContext)
{
    void *pActiveScreen;

    pActiveScreen = *(void **)((char *)pPrimaryContext + 0xd4);
    if (pActiveScreen != 0) {
        ((GameActiveScreenSlot0x30Interface *)pActiveScreen)
            ->ForwardSlot0x30();
    }
}

// FUNCTION: LEMBALL 0x00431ee0
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

// FUNCTION: LEMBALL 0x00431f10
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

// FUNCTION: LEMBALL 0x00431f30
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

// FUNCTION: LEMBALL 0x004322d0
void LEMBALL_FASTCALL DispatchPrimaryContextSlot23Thunk(
    void *pPrimaryContext)
{
    ((VS_RootGeometryOwner *)pPrimaryContext)
        ->ReservedWindowOwnerSlot23();
}

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

// FUNCTION: LEMBALL 0x00439f50
void PASCAL SetAudioMusicAndEffectVolumeByte(unsigned char nVolume) {
    ((VsSoundManager *)g_pAudioManager)
        ->SetAudioManagerMusicAndEffectVolume(nVolume, -1);
}

// FUNCTION: LEMBALL 0x00439f70
void PASCAL SetVariantResourceEffectMasterVolume(unsigned char nVolume) {
    ((VsSoundManager *)g_pAudioManager)
        ->SetAudioManagerMusicAndEffectVolume(-1, nVolume);
}

// FUNCTION: LEMBALL 0x004465c0
void LEMBALL_FASTCALL RequestFramedScreenTransitionMode2(void *pScreen) {
    *(int *)((char *)pScreen + 0x378) = 1;
    *(int *)((char *)pScreen + 0x374) = 2;
}

// FUNCTION: LEMBALL 0x00446720
GameBaseModeObject *GameBaseModeObject::Initialize(GameMainContext *pMainContext) {
    void *pTransportClient;

    m_pVtable = g_GAME_GenericModeVtableSlots;
    m_nStatus = 0;
    m_nRequestedMode = 0;
    InitializeRenderQueueNodeBase(&m_pRenderQueueNodeVtable);
    m_pVtable = g_GAME_GenericModeVtableSlots;
    m_pRenderQueueNodeVtable = g_GAME_StartupModeEventClientVtable;
    m_pMainContext = pMainContext;
    pTransportClient = AllocateVSMemBlock(0x34);
    m_pTransportClient = pTransportClient == 0
        ? 0
        : ((GameEffStreamPayloadSize8 *)pTransportClient)
              ->ConstructPayloadSize8Thunk();
    if (((GameLevelProgressState *)g_pLevelProgressState)->m_nCurrentPack == 4 &&
        g_nSelectedNetworkLobbyPeerId != 0) {
        m_fNetworkLobbyActive = 1;
    } else {
        m_fNetworkLobbyActive = 0;
    }
    if (g_pActiveNetworkRuntimeWindow != 0) {
        ((GameEffTransportRuntimeWindow *)g_pActiveNetworkRuntimeWindow)
            ->RegisterEffTransportEventClient(&m_pRenderQueueNodeVtable);
    }
    g_pActiveNetworkLobbyTransportController = this;
    return this;
}

// FUNCTION: LEMBALL 0x004467d0
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

// FUNCTION: LEMBALL 0x00446830
void GameBaseModeObject::Update(void) {
    if (m_fNetworkLobbyActive != 0 &&
        g_nSelectedNetworkLobbyPeerId == 0) {
        RequestFramedScreenTransitionMode2Thunk(
            g_pActiveNetworkLobbyScreen);
    }
    ((void (LEMBALL_FASTCALL *)(void *))m_pVtable[3])(this);
}

static void InitializeIntroSequenceMediaOwnerVtable(void *pMediaOwner) {
    int i;

    if (g_fGAMEIntroSequenceMediaOwnerVtableInitialized != 0) {
        return;
    }
    for (i = 0; i < 50; ++i) {
        g_GAME_IntroSequenceMediaOwnerVtable[i] =
            (*(void ***)pMediaOwner)[i];
    }
    g_GAME_IntroSequenceMediaOwnerVtable[25] =
        (void *)GetIntroSequenceMediaNotifyFlags;
    g_GAME_IntroSequenceMediaOwnerVtable[46] =
        (void *)IgnoreIntroSequenceMediaStateChange;
    g_GAME_IntroSequenceMediaOwnerVtable[47] =
        (void *)IgnoreIntroSequenceMediaPositionChange;
    g_GAME_IntroSequenceMediaOwnerVtable[48] =
        (void *)IgnoreIntroSequenceMediaNotificationThunk;
    g_GAME_IntroSequenceMediaOwnerVtable[49] =
        (void *)HandleIntroSequenceMediaCompletionThunk;
    g_fGAMEIntroSequenceMediaOwnerVtableInitialized = 1;
}

// FUNCTION: LEMBALL 0x00447270
GameScreenFrameRect::GameScreenFrameRect(short x, short y, short cx, short cy) {
    m_cx = cx;
    m_cy = cy;
    m_x = x;
    m_y = y;
}

// FUNCTION: LEMBALL 0x00447410
void *GameScreenObject::ConstructIntroSequenceScreen(void *pPrimaryContext, void *pWindowOwnerContext, short *paRect, int fSkipVariant) {
    char *pScreen;
    char *pMediaOwner;

    pScreen = (char *)this;
    ((GameFramedScreenBase *)pScreen)->ConstructFramedScreenBaseFields(
        pPrimaryContext, pWindowOwnerContext, paRect,
        1, 0, 0, 0, 0, 0);
    *(void ***)(pScreen + 0x3a0) =
        g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage + 4;
    *(unsigned short *)(pScreen + 0x3a4) = 0;
    *(unsigned short *)(pScreen + 0x3a6) = 0;
    *(unsigned short *)(pScreen + 0x3a8) = 0;
    *(unsigned short *)(pScreen + 0x3aa) = 0;

    pMediaOwner = pScreen + 0x3bc;
    ((VsGdiAnimWindowOwner *)pMediaOwner)
        ->ConstructResourceGeometryHelperWindowOwner();
    InitializeIntroSequenceMediaOwnerVtable(pMediaOwner);
    *(void ***)pMediaOwner = g_GAME_IntroSequenceMediaOwnerVtable;
    *(void ***)pScreen = g_GAME_IntroSequenceScreenVtable;
    *(void ***)(pScreen + 4) = g_GAME_IntroSequenceQueueVtable;
    *(void ***)(pScreen + 0x14) = g_GAME_IntroSequenceCallbackVtable;
    *(int *)(pScreen + 0x398) = fSkipVariant;
    *(int *)(pScreen + 0x39c) = fSkipVariant != 0 ? 0xe : 2;
    *(int *)(pScreen + 0x3b8) = 0;
    *(int *)(pScreen + 0x3b4) = 10;

    ((GamePrimaryContext *)pPrimaryContext)
        ->SampleRootHelperGeometryAndDispatchRenderGroups(-1);
    ((GameIntroSequenceMediaState *)pMediaOwner)
        ->ConfigureOwnerThunk(pScreen, pPrimaryContext, fSkipVariant);
    *(int *)(pScreen + 0x450) = 1;
    ((GameDynamicCString *)(pScreen + 0x454))
        ->AssignDynamicCString("lemball");
    *(int *)(pScreen + 0x44c) = 1;
    *(int *)(pScreen + 0xa8) = 0;
    *(int *)(pScreen + 0xac) = 0;
    *(int *)(pScreen + 0xb0) = 0;
    ActivateFramedScreenBaseThunk(pScreen);
    return pScreen;
}

// FUNCTION: LEMBALL 0x00447550
void LEMBALL_FASTCALL DestroyIntroSequenceScreen(void *pObject) {
    char *pScreen;

    pScreen = (char *)pObject;
    *(void ***)pScreen = g_GAME_IntroSequenceScreenVtable;
    *(void ***)(pScreen + 4) = g_GAME_IntroSequenceQueueVtable;
    *(void ***)(pScreen + 0x14) = g_GAME_IntroSequenceCallbackVtable;
    if (*(int *)(pScreen + 0x9c) != 0) {
        NoopIntroSequenceScreenHookThunk(pScreen);
    }
    ((GamePrimaryContext *)*(void **)(pScreen + 0x84))
        ->SampleRootHelperGeometryAndDispatchRenderGroups(-1);
    ((VsGdiAnimWindowOwner *)(pScreen + 0x3bc))
        ->DestroyResourceGeometryHelperWindowOwner();
    *(void **)(pScreen + 0x3a0) =
        g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    DestroyFramedScreenBaseThunk00402081(pScreen);
}

// FUNCTION: LEMBALL 0x004479a0
void *LEMBALL_FASTCALL DeleteIntroSequenceScreen(void *pObject, int, int fDelete) {
    DestroyIntroSequenceScreenThunk(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x004479d0
void *LEMBALL_FASTCALL DeleteAdjustedIntroSequenceScreen(void *pObject, int nUnusedEdx, int fDelete) {
    return DeleteIntroSequenceScreen(
        (char *)pObject - 4, nUnusedEdx, fDelete);
}

// FUNCTION: LEMBALL 0x00448200
void *GameScreenObject::ConstructMainMenuScreen(void *pPrimaryContext, void *pWindowOwnerContext, short *paRect) {
    GameLevelProgressState *pProgress;
    DWORD dwNow;
    char *pScreen;
    int nPack;

    pScreen = (char *)this;
    ConstructFramedScreenBaseFieldsThunk(
        pScreen, (int)(unsigned long)pWindowOwnerContext,
        pPrimaryContext, pWindowOwnerContext, paRect,
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
    switch (nPack) {
    case 1:
        *(int *)(pScreen + 0x3b4) = 1;
        break;
    case 2:
        *(int *)(pScreen + 0x3b4) = 2;
        break;
    case 3:
        *(int *)(pScreen + 0x3b4) = 3;
        break;
    case 0:
        *(int *)(pScreen + 0x3b4) = 0;
        break;
    case 4:
        *(int *)(pScreen + 0x3b4) = 0;
        break;
    }
    ((GameLevelProgressState *)g_pLevelProgressState)->m_nUnused4C = 0;
    *(int *)(pScreen + 0xa8) = 1;
    *(int *)(pScreen + 0xac) = 1;
    *(int *)(pScreen + 0xb0) = 1;
    ActivateFramedScreenBaseThunk(pScreen);
    return pScreen;
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
                g_pEffTransportSecondaryDispatchQueue, 0,
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
            g_pSharedRenderDispatchQueue, 0,
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

// FUNCTION: LEMBALL 0x0045b8e0
void TriggerReleaseAssertFailure(const char *pszExpression, const char *pszFile, int nLine) {
    ShowReleaseAssertFailureAndAbort(pszExpression, pszFile, nLine);
}

// FUNCTION: LEMBALL 0x0045ecb0
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

// FUNCTION: LEMBALL 0x0045eda0
char *__stdcall FindCdromFilePathBySuffix(const char *pszSuffix) {
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

// FUNCTION: LEMBALL 0x00462ea0
void *LEMBALL_FASTCALL InitializeRenderQueueNodeBase(void *pRenderQueueNode) {
    *(void **)pRenderQueueNode = (void *)g_GAME_RenderQueueNodeVtableSlots;
    *(int *)((char *)pRenderQueueNode + 8) = 0;
    *(int *)((char *)pRenderQueueNode + 0xc) = 0;
    *(int *)((char *)pRenderQueueNode + 4) = 0x51484452;
    return pRenderQueueNode;
}

// FUNCTION: LEMBALL 0x004632a0
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

// FUNCTION: LEMBALL 0x004633b0
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

// FUNCTION: LEMBALL 0x00463b50
void *GameWindowOwnerRenderContext::ConstructWindowOwnerRenderContext(void) {
    GameWindowOwnerRenderContext *pRenderContext;
    char *pEntry;
    void *pWindowOwnerBaseVtable;
    int i;

    pRenderContext = this;
    InitializeGameStubVtables();
    new (pRenderContext) VS_WindowOwnerBase;
    pWindowOwnerBaseVtable = *(void **)pRenderContext;
    InitializeWindowOwnerRenderContextVtable(
        (void **)pWindowOwnerBaseVtable);
    pEntry = pRenderContext->m_PackagedSpriteRenderEntry58;
    for (i = 0; i >= 0; --i) {
        InitializePackagedSpriteRenderEntryThunk(pEntry);
        pEntry += 0x10;
    }
    pEntry = pRenderContext->m_RenderPointRectSinkEntry68;
    for (i = 0; i >= 0; --i) {
        InitializeRenderPointRectSinkEntryThunk(pEntry);
        pEntry += 0x10;
    }
    pEntry = pRenderContext->m_StatusPointSinkEntry78;
    for (i = 0; i >= 0; --i) {
        InitializeStatusIndicatorPointSinkEntryThunk(pEntry);
        pEntry += 0x10;
    }

    *(void **)pRenderContext = g_GAME_WindowOwnerRenderContextVtableSlots;
    pRenderContext->m_aDirtyRect88[1] = 0;
    *(void **)pRenderContext = g_GAME_WindowOwnerRenderContextVtableSlots;
    pRenderContext->m_aDirtyRect88[0] = 0;
    pRenderContext->m_pWindowOwnerContext = 0;
    pRenderContext->m_aDirtyRectTail8C[1] = 0;
    pRenderContext->m_nFrameInterval = 0;
    pRenderContext->m_aDirtyRectTail8C[0] = 0;
    pRenderContext->m_nPaletteResourceId54 = 0;
    return pRenderContext;
}

// FUNCTION: LEMBALL 0x00463bd0
void LEMBALL_FASTCALL DestroyWindowOwnerRenderContext(void *pPrimaryContext) {
    GameWindowOwnerRenderContext *pRenderContext;
    char *pEntry;
    int i;

    pRenderContext = (GameWindowOwnerRenderContext *)pPrimaryContext;
    pEntry = (char *)pRenderContext->m_aDirtyRect88;
    for (i = 0; i >= 0; --i) {
        pEntry -= 0x10;
        RestorePackagedSpriteRenderEntryVtableD(pEntry);
    }
    pEntry = pRenderContext->m_StatusPointSinkEntry78;
    for (i = 0; i >= 0; --i) {
        pEntry -= 0x10;
        RestorePackagedSpriteRenderEntryVtableE(pEntry);
    }
    pEntry = pRenderContext->m_RenderPointRectSinkEntry68;
    for (i = 0; i >= 0; --i) {
        pEntry -= 0x10;
        RestorePackagedSpriteRenderEntryVtableA(pEntry);
    }
    pRenderContext->VS_WindowOwnerBase::~VS_WindowOwnerBase();
}

// FUNCTION: LEMBALL 0x00463f60
void GameWindowOwnerRenderContext::ApplyWindowOwnerActivation(int nActivate) {
    (void)nActivate;
}

// FUNCTION: LEMBALL 0x00464190
void GameWindowOwnerRenderContext::UpdateWindowOwnerRenderContext(void) {
    short aRect[4];

    aRect[0] = 0;
    aRect[1] = 0;
    if (*(int *)((char *)this + 4) != 0) {
        aRect[3] = 0;
        aRect[2] = 0;
        if (*(int *)((char *)this + 0x34) != 0) {
            ((GamePrimaryContextGeometryVirtualInterface *)this)
                ->DispatchPrimaryContextActiveScreenFrame(aRect);

            DispatchAndClearPointerQueue(m_pWindowOwnerContext);
            *(int *)((char *)m_pWindowOwnerContext + 4) = 0;

            void *pChild = *(void **)((char *)this + 0x24);
            while (pChild != 0) {
                ((GamePrimaryContextExtendedVirtualInterface *)*(void **)pChild)
                    ->UpdateWindowOwnerRenderContext();
                pChild = *(void **)((char *)pChild + 4);
            }

            if (*(int *)((char *)this + 0x20) == 0 &&
                g_pArrowCursorStatusIndicatorRenderClient != 0) {
                ((GameCursorRenderClient *)
                     g_pArrowCursorStatusIndicatorRenderClient)
                    ->UpdateForWindowOwner(this);
                DispatchAndClearPointerQueue(m_pWindowOwnerContext);
                *(int *)((char *)m_pWindowOwnerContext + 4) = 0;
            }
        }
    }
}

// FUNCTION: LEMBALL 0x004642c0
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

// FUNCTION: LEMBALL 0x00464440
void GameWindowOwnerRenderContext::InitializeWindowOwnerFromRectAndActivate(
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

// FUNCTION: LEMBALL 0x00464490
void GameWindowOwnerRenderContext::ApplyWindowOwnerPaletteResource(int nResourceId) {
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

// FUNCTION: LEMBALL 0x004662b0
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

struct GameCursorRenderEntryInterface {
    virtual void Reserved00(void) = 0;
    virtual void AppendToQueue(void *pQueue) = 0;
};

// FUNCTION: LEMBALL 0x0046b5c0
void GameCursorRenderClient::UpdateForWindowOwner(void *pOwner)
{
    typedef void (LEMBALL_FASTCALL *VoidProc)(void *);
    short aRect[4];
    short aRenderPosition[2];
    short *pOwnerClip;
    void *pGeometryQueue;
    void *pGeometryContext;
    void *pCompactGeometry;
    void *pRenderEntry;
    int nCompactOffset;
    int nScale;

    GameCursorRenderClient *pCursor = this;

    if ((*(int *)((char *)pCursor + 0x2c) == 0 ||
         (*(unsigned char *)((char *)g_pSharedGeometryHelper + 0x14) & 1) == 0) &&
        (*(int *)((char *)pCursor + 0x30) == 0 ||
         (*(unsigned char *)((char *)g_pSharedGeometryHelper + 0x14) & 6) == 0))
        return;

    if ((g_uCursorWindowOwnerState & 1) == 0) {
        *(unsigned char *)&g_uCursorWindowOwnerState |= 1;
        g_pCursorWindowOwner = pOwner;
    }
    if (((VS_WindowOwnerBase *)pOwner)->IsCurrentWindowOwnerContext() == 0) {
        if (pOwner == g_pCursorWindowOwner) {
            pCursor->ShowNativeCursor();
            g_pCursorWindowOwner = 0;
        }
    } else {
        g_pCursorWindowOwner = pOwner;
    }

    if (*(int *)((char *)pCursor + 0x18) == 0)
        return;

    aRect[0] = *(short *)((char *)pOwner + 8);
    aRect[1] = *(short *)((char *)pOwner + 10);
    aRect[2] = *(short *)((char *)pOwner + 12);
    aRect[3] = *(short *)((char *)pOwner + 14);
    pOwnerClip = (short *)((char *)pOwner + 0x10);
    if ((int)pOwnerClip[0] * (int)pOwnerClip[1] != 0) {
        short xClip = (short)(*(short *)((char *)pOwner + 0x14) + aRect[2]);
        short yClip = (short)(*(short *)((char *)pOwner + 0x16) + aRect[3]);
        if (aRect[2] < xClip) {
            aRect[0] = (short)(aRect[0] + aRect[2] - xClip);
            aRect[2] = xClip;
        }
        if ((short)(xClip + pOwnerClip[0]) < (short)(aRect[2] + aRect[0]))
            aRect[0] = (short)(xClip + pOwnerClip[0] - aRect[2]);
        if (aRect[3] < yClip) {
            aRect[1] = (short)(aRect[1] + aRect[3] - yClip);
            aRect[3] = yClip;
        }
        if ((short)(yClip + pOwnerClip[1]) < (short)(aRect[3] + aRect[1]))
            aRect[1] = (short)(yClip + pOwnerClip[1] - aRect[3]);
        if (aRect[0] < 1 || aRect[1] < 1) {
            aRect[0] = 0;
            aRect[1] = 0;
            aRect[2] = 0;
            aRect[3] = 0;
        }
    }

    if (pCursor->PrepareForWindowOwner(pOwner) == 0)
        return;
    if (*(int *)((char *)pCursor + 0x3c) != 0)
        pCursor->HideNativeCursor();
    *(int *)((char *)pCursor + 0x38) = 1;
    if (pCursor->m_pRenderResource20 == 0)
        return;

    nScale = *(int *)((char *)pOwner + 0x38);
    aRenderPosition[0] =
        (short)((*(short *)((char *)pCursor + 0x10) - aRect[2]) / nScale) -
        *(short *)((char *)pCursor + 0x14);
    aRenderPosition[1] =
        (short)((*(short *)((char *)pCursor + 0x12) - aRect[3]) / nScale) -
        *(short *)((char *)pCursor + 0x16);
    pGeometryQueue = *(void **)((char *)pOwner + 0x4c);
    pGeometryContext = *(void **)((char *)pGeometryQueue + 0x0c);
    ((VoidProc)(*(void ***)pGeometryContext)[2])(pGeometryContext);
    nCompactOffset = *(int *)((char *)*(void **)((char *)pGeometryContext + 0x40) + 4);
    pCompactGeometry = (char *)pGeometryContext + 0x40 + nCompactOffset;
    ((VoidProc)(*(void ***)pCompactGeometry)[14])(pCompactGeometry);
    pRenderEntry = *(void **)((char *)pCursor + 0x1c);
    *(short *)((char *)pRenderEntry + 4) = aRenderPosition[0];
    *(short *)((char *)pRenderEntry + 6) = aRenderPosition[1];
    ((VoidProc)(*(void ***)pCompactGeometry)[14])(pCompactGeometry);
    ((GameCursorRenderEntryInterface *)pRenderEntry)
        ->AppendToQueue(pGeometryQueue);
}

// FUNCTION: LEMBALL 0x0046dcd0
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

// FUNCTION: LEMBALL 0x004734f0
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
