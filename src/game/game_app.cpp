#include "game/game_app.h"
#include "game/window_owner.h"
#include "../network/safe_vtable.h"

#include "platform/shell_ui.h"
#include "platform/startup_options.h"
#include "audio/audio_manager.h"
#include "game/demo_playback.h"
#include "engine/graphics_driver.h"
#include "resource/resource_archive.h"
#include "engine/fixed_math.h"
#include "engine/memory_arena.h"
#include "engine/runtime_init.h"

#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif
#include <stdio.h>
#include <string.h>

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
static const char g_GAME_LevelPasswordTemplate[] = "00000000";
static const char g_GAME_LevelPasswordCheatCode[] = "9913454278";
static const char g_GAME_LevelPasswordCheatText[] = "Oh, oh, someones cheating !!!";
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
static void *g_GAME_PrimaryContextMenuDefinitionTable;
static const unsigned int g_GAME_LevelPasswordPermutation[8] = {2, 0, 7, 4, 6, 1, 5, 3};
int g_fRootHelperGeometryDispatchSuppressed = 0;

extern void LEMBALL_FASTCALL ServiceNetworkLobbySelectedPeerUpdates(void *pVsnetRuntime);
extern int PumpMessagesAndRunFrame(void);
extern int LEMBALL_FASTCALL ValidateResourceFileSignatureThunk(void *pMainContext);
extern void TriggerReleaseAssertFailure(const char *pszExpression, const char *pszFile, int nLine);
extern void *LEMBALL_FASTCALL ConstructVariantResourceEntryManagerThunk(void *pManager);
extern void LEMBALL_FASTCALL DestroyMainGameVariantResourceMode(void *pVariantMode);
extern void LEMBALL_FASTCALL UnregisterNetworkLobbyVsnetRuntimeFromTransport(void *pVsnetRuntime);
extern void LEMBALL_FASTCALL ShutdownVariantResourceEntryManager(void *pManager);
extern int g_fMusicEnabled;
extern int g_fEffectsEnabled;
  extern void *LEMBALL_FASTCALL ConstructWindowOwnerRenderContext(void *pPrimaryContext);
  extern int RegisterBaseWindowClass(void);
extern void *LEMBALL_FASTCALL InitializePackagedSpriteRenderEntryThunk(void *pObject);
extern void *LEMBALL_FASTCALL InitializeRenderPointRectSinkEntryThunk(void *pObject);
extern void *LEMBALL_FASTCALL InitializeStatusIndicatorPointSinkEntryThunk(void *pObject);
extern void *LoadZrleResource(int nResourceId);
extern void *LoadPalResource(int nResourceId);
extern void SetLevelScreenStatusIndicatorMode(int nMode, int nValue);
extern void *g_pPrimaryContextVtable;
extern void *g_pPrimaryContextRenderQueueNodeVtable;
extern void *g_pQueuedRenderPointSinkFinalizeThunk;
extern void *g_pSharedRenderDispatchQueue;
extern void *g_pSharedGeometryHelper;
extern void *g_pEffTransportSecondaryDispatchQueue;
extern int *g_pArrowCursorStatusIndicatorRenderClient;
extern "C" DWORD WINAPI timeGetTime(void);

struct GAME_PrimaryContext {
    void *m_pVtable;
    char m_Reserved4[0x34];
    int m_nScaleFactor;
    char m_Reserved3c[0x10];
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

    GAME_PrimaryContext *ConstructPrimaryContext(GAME_MainContext *pMainContext);
    GAME_PrimaryContext *ConstructPrimaryContextThunk(GAME_MainContext *pMainContext);
    void SwitchPrimaryContextScreen(int nMode);
    void DestroyPrimaryContextActiveScreen(int nUnused);
    void SetWindowOwnerScaleFactor(int nScaleFactor);
    short *ComputePrimaryContextCenteredScreenRect(short *paRect,
                                                   int nLeftOverride,
                                                   int nTopOverride);
    short *ComputePrimaryContextCenteredScreenRectThunk(short *paRect,
                                                        int nLeftOverride,
                                                        int nTopOverride);
};

struct GAME_ScreenObject {
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

struct GAME_LevelProgressState {
    int m_nCurrentLevel;
    int m_nUnused04;
    int m_nCurrentPack;
    int m_nUnused0C;
    char m_szPassword[0x18];
    int m_anUnlockedPackCaps[5];
    int m_anSnapshotPackCaps[4];
    int m_nUnused4C;

    GAME_LevelProgressState *Clear(void);
    unsigned int EncodePasswordValue(void);
    void ApplyPasswordValue(unsigned int uValue);
    char *FormatPassword(void);
    int ValidateAndApplyPassword(char *pszPassword);
    int ParsePasswordDigits(void);
    void Snapshot(void);
    void SetUnlockedPackCap(int nPack, int nCap);
};

struct GAME_BaseModeObject {
    void **m_pVtable;
    int m_nRequestedMode;
    int m_nStatus;
    void **m_pRenderQueueNodeVtable;
    int m_nQueueMagic;
    int m_nQueueClientCount;
    int m_nQueueReserved;
    int m_fNetworkLobbyActive;
    void *m_pTransportClient;
    GAME_MainContext *m_pMainContext;
};

struct GAME_LevelSelectionModeController : public GAME_BaseModeObject {
};

struct GAME_NetworkLobbyTransportController : public GAME_BaseModeObject {
    int m_nReserved28;
    int m_nReserved2C;
    void *m_pPeerClearCloseStream;
    void *m_pPeerDirtyConfirmStream;
};

struct GAME_PaintballSequenceModeController : public GAME_BaseModeObject {
    int m_fFirstSequenceVariant;
};

struct GAME_LevelGameMode {
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
    GAME_MainContext *m_pMainContext;
};

struct GAME_RenderQueueNode {
    void **m_pVtable;
    int m_uMagic;
    void *m_pReserved08;
    void *m_pReserved0C;
};

struct GAME_RenderDispatchClientNode {
    void *m_pClient;
    int m_nOrder;
    GAME_RenderDispatchClientNode *m_pNext;
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

static void *g_GAME_PrimaryContextVtableSlots[30];
static void *g_GAME_RenderQueueNodeVtableSlots[2];
static void *g_GAME_GenericModeVtableSlots[2];
static void *g_GAME_GenericScreenVtableSlots[14];
struct GAME_SafeVtableInitializer {
    GAME_SafeVtableInitializer(void) {
        int i;
        for (i = 0; i < 30; ++i)
            g_GAME_PrimaryContextVtableSlots[i] = (void *)NetworkSafeVtableNoop;
        for (i = 0; i < 2; ++i) {
            g_GAME_RenderQueueNodeVtableSlots[i] = (void *)NetworkSafeVtableNoop;
            g_GAME_GenericModeVtableSlots[i] = (void *)NetworkSafeVtableNoop;
        }
        for (i = 0; i < 14; ++i)
            g_GAME_GenericScreenVtableSlots[i] = (void *)NetworkSafeVtableNoop;
    }
};
static GAME_SafeVtableInitializer g_GAME_SafeVtableInitializer;
int g_nSelectedNetworkLobbyPeerId = 0;
void *g_pActiveNetworkLobbyTransportController = 0;
void *g_pNetworkLobbyTransportController = 0;
void *g_pActiveNetworkLobbyScreen = 0;

extern void *ConstructEffStreamPayloadSize8(void *pObject);
struct GAME_EffTransportRuntimeWindow {
    void RegisterEffTransportEventClient(void *pClient);
};
extern void *ConstructNetworkLobbyPeerClearCloseStream(void *pObject);
extern void *ConstructNetworkLobbyPeerDirtyConfirmStream(void *pObject);
struct GAME_NetworkLevelChunkDeltaStream {
    void *ConstructNetworkLevelChunkDeltaStream(int nOwner);
    void *ConstructNetworkLevelChunkDeltaStreamEntry(int nOwner);
};
struct GAME_EffStream {
    void ResetStateFields(void);
};
extern void *ConstructMainGameVariantResourceBundle(void *pBundle, void *pPrimaryContext, unsigned short nVariantMode);
extern void InitializeManagedEntitySlotTablesEntryThunk(void *pLevelGameMode);
extern void ResetLevelFrameClockEntryThunk(void);
extern void *ConstructLevelGameStateStreamThunk(void *pObject);
extern void *ConstructLevelTileGridThunk(void *pObject);
extern void InitializeLevelTileGridThunk(void *pTileGrid);
extern void *ConstructLevelTileReachabilityHelperThunk(void *pObject, void *pTileGrid);
extern void ClearPrimaryLockedRecordTablePayloadFlags(void *pPayload);
extern void ClearSecondaryLockedRecordTablePayloadFlags(void *pPayload);
extern void *ConstructLevelChunkStreamDispatcherThunk(void *pObject, int nChunkTypeCount);
extern void ResetRegisteredLevelChunkStreamsThunk(void *pDispatcher);
extern void RegisterLevelChunkStreamThunk(void *pDispatcher, void *pStream);
extern void *ConstructLiftChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructMoveChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern int *InitializeProjectileGeometryPairTableThunk(int *pTable);
extern void ClearProjectileGeometryField100Thunk(void *pTable);
extern void *ConstructGmobChunkManagerThunk(void *pObject, void *pLevelMode);
extern void *ConstructPlasChunkManagerThunk(void *pObject, void *pLevelMode, void *pGmobManager, void *pProjectileGeometryTable);
extern void *ConstructProjectilePoolThunk(void *pObject);
extern void *ConstructManagedEntityQueueCursorThunk(void *pObject, int nLevelMode, int nPrimaryCapacity, int nSecondaryCapacity);
extern void *ConstructShpgChunkManagerThunk(void *pObject, void *pLevelMode, void *pGmobManager, void *pProjectileGeometryTable);
extern void ActivateNestedChildrenFromOwnerTableA4Thunk(void *pObject);
extern void *ConstructEnmyChunkManagerThunk(void *pObject, void *pLevelMode, void *pGmobManager, void *pProjectileGeometryTable);
extern void ActivateNestedChildrenFromOwnerTableVariantThunk(void *pObject);
extern void *ConstructNodeChunkManagerThunk(void *pObject, int nCapacity);
extern void ResetNodeChunkRecordsThunk(void *pNodeManager);
extern void *ConstructBallChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void ResetBallChunkEntriesThunk(void *pBallManager);
extern void *ConstructCollChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructMineChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructDoorChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructRockChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructCaptureTriggerChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructLasrChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ReturnArgumentVtableCallback(void *pObject);
extern void ResetLevelField194TimerBlockThunk(void *pObject);
extern void *ConstructBoonChunkManagerThunk(void *pObject, void *pLevelMode, void *pTileGrid);
extern void ResetBoonChunkManagerObjectsThunk(void *pObject);
extern void *ConstructTramChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructPgunChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructIceChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructType18ChunkStreamThunk(void *pObject);
extern void *ConstructSlnkChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void ResetType35ChunkObjectsThunk(void *pObject);
extern void *ConstructInvsChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity);
extern void *ConstructLevelChunkLoaderContextThunk(void *pObject, int nLevelMode);
extern int GetSelectedLevelNumberThunk(void *pLevelProgressState);
extern void ReadLevelDemoLengthPrefixedRecordThunk(void *pPlaybackController, void *pRecordHeader, unsigned int *pcbRecord);
extern void LoadLevelChunksForSkillAndLevelThunk(void *pLoaderContext, unsigned int nSkill, unsigned int nLevel, int fReuseState);
extern void InitializeNetworkPlayerPlasChunkObjectPoolThunk(void *pPlasManager);
extern void AssignNetworkPlayerManagedEntitySlotIdsThunk(int nLevelMode);
extern void ResizeLevelTileReachabilityGridThunk(void *pReachabilityHelper);
extern void FillReachabilityGridFromTileFlagsThunk(void *pReachabilityHelper);
extern void BuildSpecialLevelTileGridStaticRenderEntriesThunk(void *pRenderList, void *pTileGrid);
extern void AssignMissingManagedEntitySlotIdsThunk(int nLevelMode);
extern void InitializeLevelChunkTypeEnabledFlagsThunk(void *pLevelMode);
extern void AdvanceCachedResourceObjectFrameCounters(void *pArchive);
struct LEVEL_GameMode {
    void InitializeLevelGameMode(void);
};

void *g_pStatusEntryRegistry = 0;
void *g_pLevelProgressState = 0;
void *g_pMainResourceArchive = 0;
void *g_pVariantResourceEntryManager = 0;
void *g_pNetworkLobbyVsnetRuntime = 0;
void *g_pActiveNetworkRuntimeWindow = 0;
int g_nLevelFrameClockTick = 0;
void *g_pStartupModeVtable = g_GAME_GenericModeVtableSlots;
void *g_pStartupModeRenderThunk = (void *)StubNoOpPtr;
void *g_pMainMenuModeVtable = g_GAME_GenericModeVtableSlots;
void *g_pMainMenuModeRenderThunk = (void *)StubNoOpPtr;
void *g_pMode3Vtable = g_GAME_GenericModeVtableSlots;
void *g_pMode3RenderThunk = (void *)StubNoOpPtr;
void *g_pSimpleModeDeleteVtable = g_GAME_GenericModeVtableSlots;
void *g_pSimpleModeVtable = g_GAME_GenericModeVtableSlots;
void *g_pMode16Vtable = g_GAME_GenericModeVtableSlots;
void *g_pMode16RenderThunk = (void *)StubNoOpPtr;
void *g_pPrimaryContextVtable = g_GAME_PrimaryContextVtableSlots;
void *g_pPrimaryContextRenderQueueNodeVtable = g_GAME_RenderQueueNodeVtableSlots;
void *g_pQueuedRenderPointSinkFinalizeThunk = (void *)StubNoOpPtr;
void *g_pSharedRenderDispatchQueue = 0;
void *g_pSharedGeometryHelper = 0;
void *g_pActiveLevelGameMode = 0;
void *g_pLevelTileGrid = 0;
void *g_pLevelTileReachabilityHelper = 0;
void *g_pLevelChunkStreamDispatcher = 0;
unsigned short g_nGameAppState0 = 0;
unsigned short g_nGameAppState1 = 0;
int g_nGameAppState2 = 0;

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

int PumpMessagesAndRunFrame(void) {
    return 1;
}

// FUNCTION: LEMBALL 0x00406A90
GAME_LevelProgressState *GAME_LevelProgressState::Clear(void) {
    m_nCurrentLevel = 0;
    m_nUnused04 = 0;
    m_nCurrentPack = 0;
    m_nUnused0C = 0;
    m_anUnlockedPackCaps[0] = 0;
    m_anUnlockedPackCaps[1] = 0;
    m_anUnlockedPackCaps[2] = 0;
    m_anUnlockedPackCaps[3] = 0;
    m_anUnlockedPackCaps[4] = 0;
    m_anSnapshotPackCaps[0] = 0;
    m_anSnapshotPackCaps[1] = 0;
    m_anSnapshotPackCaps[2] = 0;
    m_anSnapshotPackCaps[3] = 0;
    m_nUnused4C = 0;
    return this;
}

// FUNCTION: LEMBALL 0x00406AD0
unsigned int GAME_LevelProgressState::EncodePasswordValue(void) {
    unsigned int auDigits[8];
    unsigned int uValue;
    int i;

    for (i = 0; i < 4; ++i) {
        auDigits[i * 2] = ((unsigned int)m_anUnlockedPackCaps[i] & 0x38) >> 3;
        auDigits[i * 2 + 1] = (unsigned int)m_anUnlockedPackCaps[i] & 7;
    }

    uValue = 0;
    for (i = 0; i < 8; ++i) {
        unsigned int uDigit;

        uDigit = auDigits[g_GAME_LevelPasswordPermutation[i]] ^ g_GAME_LevelPasswordPermutation[i];
        uValue = (uValue << 3) | uDigit;
    }
    return uValue;
}

// FUNCTION: LEMBALL 0x00406B30
void GAME_LevelProgressState::ApplyPasswordValue(unsigned int uValue) {
    unsigned int auDigits[16];
    int i;

    for (i = 7; i >= 0; --i) {
        auDigits[i] = uValue & 7;
        uValue >>= 3;
    }

    for (i = 0; i < 8; ++i) {
        auDigits[g_GAME_LevelPasswordPermutation[i]] = auDigits[i + 8] ^ g_GAME_LevelPasswordPermutation[i];
    }

    for (i = 0; i < 4; ++i) {
        SetUnlockedPackCap(i, (int)((auDigits[i * 2] << 3) | auDigits[i * 2 + 1]));
    }
}

// FUNCTION: LEMBALL 0x00406BA0
unsigned int ComputeLevelPasswordChecksum(unsigned int uValue) {
    return ((uValue >> 0x10) + (uValue >> 8) + uValue) & 0x1f;
}

// FUNCTION: LEMBALL 0x00406C00
char *GAME_LevelProgressState::FormatPassword(void) {
    char szDigits[12];
    unsigned int uValue;
    unsigned int cchDigits;

    uValue = EncodePasswordValue();
    uValue |= ComputeLevelPasswordChecksum(uValue) << 0x18;
    FormatSignedIntToRadixString((int)uValue, szDigits, 10);
    strcpy(m_szPassword, g_GAME_LevelPasswordTemplate);
    cchDigits = (unsigned int)strlen(szDigits);
    memcpy(m_szPassword + 10 - cchDigits, szDigits, cchDigits + 1);
    return m_szPassword;
}

// FUNCTION: LEMBALL 0x00406CA0
int GAME_LevelProgressState::ValidateAndApplyPassword(char *pszPassword) {
    unsigned int uValue;

    strcpy(m_szPassword, pszPassword);
    if (strcmp(m_szPassword, g_GAME_LevelPasswordCheatCode) == 0) {
        AppendCStringToStream(g_pStatusOutputStream, g_GAME_LevelPasswordCheatText);
        SetUnlockedPackCap(0, 0x40);
        SetUnlockedPackCap(1, 0x40);
        SetUnlockedPackCap(2, 0x40);
        SetUnlockedPackCap(3, 0x40);
        return 1;
    }

    uValue = (unsigned int)ParsePasswordDigits();
    if (uValue == 0) {
        return 0;
    }
    if (((uValue & 0x1f000000) >> 0x18) != ComputeLevelPasswordChecksum(uValue)) {
        return 0;
    }
    ApplyPasswordValue(uValue & 0xffffff);
    return 1;
}

// FUNCTION: LEMBALL 0x00406D80
int GAME_LevelProgressState::ParsePasswordDigits(void) {
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
void GAME_LevelProgressState::Snapshot(void) {
    m_anSnapshotPackCaps[0] = m_anUnlockedPackCaps[0];
    m_anSnapshotPackCaps[1] = m_anUnlockedPackCaps[1];
    m_anSnapshotPackCaps[2] = m_anUnlockedPackCaps[2];
    m_anSnapshotPackCaps[3] = m_anUnlockedPackCaps[3];
}

// FUNCTION: LEMBALL 0x00408FE0
void GAME_LevelProgressState::SetUnlockedPackCap(int nPack, int nCap) {
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
    default:
        nPackLevelCount = nCap;
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
int LEMBALL_FASTCALL ValidateResourceFileSignature(void *) {
    MOGLOAD_StringResourceObject *pStringResource;
    const char *pszEncoded;
    const char *pszKey;
    int i;

    pszKey = g_GAME_ResourceSignatureKey;
    pStringResource = LoadStringResource(0x100);
    if (pStringResource == 0) {
        return 0;
    }

    if (pStringResource->m_nLoadState10 == 0) {
        ((void (*)())pStringResource->m_pVtable[7])();
    } else {
        pStringResource->m_nReserved24 = 0;
    }

    ++pStringResource->m_nLockCount08;
    pszEncoded = pStringResource->m_pszText38;
    i = 0;
    while (pszEncoded[i] != '\0') {
        g_GAME_ResourceSignatureDecodeBuffer[i] =
            (char)(((unsigned char)(pszEncoded[i] - 1)) ^
                   (unsigned char)pszKey[i]);
        ++i;
    }
    g_GAME_ResourceSignatureDecodeBuffer[i] = '\0';

    --pStringResource->m_nLockCount08;
    ReleaseTypedResourceObjectReference(pStringResource);

    return strcmp(g_GAME_ResourceSignatureDecodeBuffer,
                  g_GAME_ResourceSignatureExpected) == 0;
}

// FUNCTION: LEMBALL 0x004734F0
void ShowReleaseAssertFailureAndAbort(const char *pszExpression, const char *pszFile, int nLine) {
    GAME_DynamicCString Message;
    GAME_DynamicCString Result;
    char szLine[16];

    Message.ConstructDynamicCString();
    Message.AssignDynamicCString("Release Version Assertion Failure\n");
    Message.AppendDynamicCStringAndCopyResult(&Result, "Reason: ");
    DestroyDynamicCString(&Result);
    Message.AppendDynamicCStringAndCopyResult(&Result, pszExpression);
    DestroyDynamicCString(&Result);
    Message.AppendDynamicCStringAndCopyResult(&Result, "\n");
    DestroyDynamicCString(&Result);
    Message.AppendDynamicCStringAndCopyResult(&Result, "In File: ");
    DestroyDynamicCString(&Result);
    Message.AppendDynamicCStringAndCopyResult(&Result, pszFile);
    DestroyDynamicCString(&Result);
    Message.AppendDynamicCStringAndCopyResult(&Result, "At Line No.: ");
    DestroyDynamicCString(&Result);
    FormatSignedIntToRadixString(nLine, szLine, 10);
    Message.AppendDynamicCStringAndCopyResult(&Result, szLine);
    DestroyDynamicCString(&Result);
    MessageBoxA(0, Message.m_pszText, "Error", 0);
    LongjmpStartupExceptionFrame(0xaaaa);
    DestroyDynamicCString(&Message);
}

// FUNCTION: LEMBALL 0x00403152
__declspec(naked) int LEMBALL_FASTCALL ValidateResourceFileSignatureThunk(void *) {
    __asm {
        jmp ValidateResourceFileSignature
    }
}

// FUNCTION: LEMBALL 0x0045B8E0
void TriggerReleaseAssertFailure(const char *pszExpression, const char *pszFile, int nLine) {
    ShowReleaseAssertFailureAndAbort(pszExpression, pszFile, nLine);
}

// FUNCTION: LEMBALL 0x00401C6C
void ReleaseMainGameVariantResourceMode(GAME_MainContext *pMainContext) {
    if (pMainContext->m_pVariantMode != 0) {
        DestroyMainGameVariantResourceMode(pMainContext->m_pVariantMode);
        FreeVSMemBlock(pMainContext->m_pVariantMode);
        pMainContext->m_pVariantMode = 0;
    }
}

// FUNCTION: LEMBALL 0x004027A7
void EnsureMainGameVariantResourceMode(GAME_MainContext *pMainContext, int nVariantMode) {
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
void InitializeBaseModeObject(void *pModeObject, GAME_MainContext *pMainContext) {
    GAME_BaseModeObject *pMode;

    pMode = (GAME_BaseModeObject *)pModeObject;
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
    if (((GAME_LevelProgressState *)g_pLevelProgressState)->m_nCurrentPack == 4 && g_nSelectedNetworkLobbyPeerId != 0) {
        pMode->m_fNetworkLobbyActive = 1;
    } else {
        pMode->m_fNetworkLobbyActive = 0;
    }
    if (g_pActiveNetworkRuntimeWindow != 0) {
        ((GAME_EffTransportRuntimeWindow *)g_pActiveNetworkRuntimeWindow)
            ->RegisterEffTransportEventClient(&pMode->m_pRenderQueueNodeVtable);
    }
    g_pActiveNetworkLobbyTransportController = pMode;
}

// FUNCTION: LEMBALL 0x00401D5C
void *ConstructLevelSelectionModeController(void *pModeObject, GAME_MainContext *pMainContext) {
    GAME_LevelSelectionModeController *pMode;

    pMode = (GAME_LevelSelectionModeController *)pModeObject;
    InitializeBaseModeObject(pMode, pMainContext);
    pMode->m_pVtable = (void **)g_pStartupModeVtable;
    pMode->m_pRenderQueueNodeVtable = (void **)g_pStartupModeRenderThunk;
    return pModeObject;
}

// FUNCTION: LEMBALL 0x00401B90
void *ConstructNetworkLobbyTransportController(void *pModeObject, GAME_MainContext *pMainContext) {
    GAME_NetworkLobbyTransportController *pMode;

    pMode = (GAME_NetworkLobbyTransportController *)pModeObject;
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
                                               GAME_MainContext *pMainContext,
                                               int fSequenceVariant) {
    GAME_PaintballSequenceModeController *pMode;

    pMode = (GAME_PaintballSequenceModeController *)pModeObject;
    InitializeBaseModeObject(pMode, pMainContext);
    pMode->m_pVtable = (void **)g_pMainMenuModeVtable;
    pMode->m_pRenderQueueNodeVtable = (void **)g_pMainMenuModeRenderThunk;
    pMode->m_fFirstSequenceVariant = fSequenceVariant;
    return pModeObject;
}

// FUNCTION: LEMBALL 0x00402B8F
void *ConstructLevelGameMode(void *pModeObject, GAME_MainContext *pMainContext) {
    GAME_LevelGameMode *pMode;

    pMode = (GAME_LevelGameMode *)pModeObject;
    InitializeRenderQueueNodeBase(pMode);
    pMode->m_pReserved14 = 0;
    pMode->m_pReserved18 = 0;
    pMode->m_pEffStreamVtable = (void **)&g_GAME_GenericScreenVtableSlots[10];
    pMode->m_pReserved20 = 0;
    pMode->m_pDeleteThunkVtable = (void **)g_pSimpleModeDeleteVtable;
    ((GAME_EffStream *)&pMode->m_pEffStreamVtable)->ResetStateFields();
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
    ((LEVEL_GameMode *)pMode)->InitializeLevelGameMode();
    return pModeObject;
}

// FUNCTION: LEMBALL 0x00410D00
void LEVEL_GameMode::InitializeLevelGameMode(void) {
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
    *(int *)(pModeBytes + 0x64) = (((GAME_LevelProgressState *)g_pLevelProgressState)->m_nCurrentPack == 4);

    if (*(int *)(pModeBytes + 0x64) != 0) {
        if (*(int *)(pModeBytes + 0x4c) == 0) {
            pObject = AllocateVSMemBlock(0x34);
            if (pObject != 0) {
                pObject = ((GAME_NetworkLevelChunkDeltaStream *)pObject)
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

        ((GAME_LevelProgressState *)g_pLevelProgressState)->m_nUnused04 = 0;
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
    *(int *)(pModeBytes + 0xf0) = ((GAME_LevelProgressState *)g_pLevelProgressState)->m_nUnused04;
    *(int *)(pModeBytes + 0xf4) = ((GAME_LevelProgressState *)g_pLevelProgressState)->m_nUnused04;
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
            pObject = ConstructLevelTileReachabilityHelperThunk(pObject, *(void **)(pModeBytes + 0x110));
        }
        *(void **)(pModeBytes + 0x114) = pObject;
    }
    g_pLevelTileReachabilityHelper = *(void **)(pModeBytes + 0x114);

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x58);
        if (pObject != 0) {
            pObject = ((RDISPATCH_Queue *)pObject)->ConstructRenderDispatchQueueVariant(
                10, "AIQueue");
        }
        *(void **)(pModeBytes + 0x164) = pObject;
        ((GAME_RenderDispatchQueue *)*(void **)(pModeBytes + 0x164))
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
            pObject = InitializeProjectileGeometryPairTableThunk((int *)pObject);
        }
        *(void **)(pModeBytes + 0x16c) = pObject;
    }
    ClearProjectileGeometryField100Thunk(*(void **)(pModeBytes + 0x16c));

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
    ActivateNestedChildrenFromOwnerTableA4Thunk(*(void **)(pModeBytes + 0x170));

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0xb0);
        if (pObject != 0) {
            pObject = ConstructEnmyChunkManagerThunk(
                pObject, pLevelGameMode, *(void **)(pModeBytes + 0x158), *(void **)(pModeBytes + 0x16c));
        }
        *(void **)(pModeBytes + 0x174) = pObject;
    }
    ActivateNestedChildrenFromOwnerTableVariantThunk(*(void **)(pModeBytes + 0x174));

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x0c);
        if (pObject != 0) {
            pObject = ConstructNodeChunkManagerThunk(pObject, 300);
        }
        *(void **)(pModeBytes + 0x17c) = pObject;
    }
    ResetNodeChunkRecordsThunk(*(void **)(pModeBytes + 0x17c));

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x10);
        if (pObject != 0) {
            pObject = ConstructBallChunkManagerThunk(pObject, pLevelGameMode, 0x14);
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
    ResetLevelField194TimerBlockThunk(*(void **)(pModeBytes + 0x194));

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
            pObject = ConstructType18ChunkStreamThunk(pObject);
        }
        *(void **)(pModeBytes + 0x1c4) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x1c4));
    }

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x10);
        if (pObject != 0) {
            pObject = ConstructSlnkChunkManagerThunk(pObject, pLevelGameMode, 0x14);
        }
        *(void **)(pModeBytes + 0x1c8) = pObject;
    }
    ResetType35ChunkObjectsThunk(*(void **)(pModeBytes + 0x1c8));

    if (*(int *)(pModeBytes + 0x4c) == 0) {
        pObject = AllocateVSMemBlock(0x40);
        if (pObject != 0) {
            pObject = ConstructInvsChunkManagerThunk(pObject, pLevelGameMode, 0x28);
        }
        *(void **)(pModeBytes + 0x1cc) = pObject;
        RegisterLevelChunkStreamThunk(g_pLevelChunkStreamDispatcher, *(void **)(pModeBytes + 0x1cc));

        pObject = AllocateVSMemBlock(8);
        if (pObject != 0) {
            pObject = ConstructLevelChunkLoaderContextThunk(pObject, (int)(unsigned long)pLevelGameMode);
        }
        *(void **)(pModeBytes + 0x178) = pObject;
    }

    if (g_fLevelDemoModeEnabled == 0) {
        nLevel = (unsigned int)GetSelectedLevelNumberThunk(g_pLevelProgressState);
        nSkill = (unsigned int)((GAME_LevelProgressState *)g_pLevelProgressState)->m_nCurrentPack;
    } else {
        nDemoHeader = 0;
        cbDemoRecord = 0;
        SetLevelDemoPlaybackEnabled(g_pLevelDemoPlaybackController, 1);
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
    BuildSpecialLevelTileGridStaticRenderEntriesThunk(*(void **)(pModeBytes + 0x8c), *(void **)(pModeBytes + 0x110));

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

// FUNCTION: LEMBALL 0x004651D0
void *LEMBALL_FASTCALL ConstructWindowOwnerBase(void *pOwner) {
    ConstructRootGeometryOwner(pOwner);
    *(void **)pOwner = (void *)0x004993d0;
    if (g_nLiveWindowOwnerBaseCount == 1) {
        RegisterBaseWindowClass();
    }
    *(int *)((char *)pOwner + 0x44) = 0;
    return pOwner;
}

// FUNCTION: LEMBALL 0x00463B50
void *LEMBALL_FASTCALL ConstructWindowOwnerRenderContext(void *pPrimaryContext) {
    char *pEntry;
    int i;

    ConstructWindowOwnerBase(pPrimaryContext);
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

    *(void **)pPrimaryContext = g_pRootGeometryOwnerVtable;
    *(unsigned short *)((char *)pPrimaryContext + 0x8a) = 0;
    *(void **)pPrimaryContext = g_pWindowOwnerBaseVtable;
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
void GAME_RenderDispatchQueue::RegisterOrderedRenderDispatchClient(void *pClient, int nOrder) {
    GAME_RenderDispatchClientNode *pNode;
    GAME_RenderDispatchClientNode *pCurrent;
    GAME_RenderDispatchClientNode *pPrevious;
    unsigned int i;

    LockDispatchQueue();
    pNode = (GAME_RenderDispatchClientNode *)AllocateVSMemBlock(0xc);
    pNode->m_pClient = pClient;
    pNode->m_nOrder = nOrder;
    pNode->m_pNext = 0;

    pCurrent = (GAME_RenderDispatchClientNode *)m_pClientList;
    if (pCurrent == 0) {
        m_pClientList = pNode;
        m_cClients = 1;
        UnlockDispatchQueue();
        return;
    }

    if (m_cClients == 0) {
        m_pClientList = pNode;
        m_cClients = 1;
        UnlockDispatchQueue();
        return;
    }

    pPrevious = pCurrent;
    i = 0;
    while (i < (unsigned int)m_cClients) {
        if (nOrder < pCurrent->m_nOrder) {
            if (i == 0) {
                pNode->m_pNext = (GAME_RenderDispatchClientNode *)m_pClientList;
                m_pClientList = pNode;
            } else {
                pNode->m_pNext = pPrevious->m_pNext;
                pPrevious->m_pNext = pNode;
            }
            ++m_cClients;
            UnlockDispatchQueue();
            return;
        }
        if (pCurrent->m_pNext == 0) {
            pCurrent->m_pNext = pNode;
            ++m_cClients;
            UnlockDispatchQueue();
            return;
        }
        ++i;
        pPrevious = pCurrent;
        pCurrent = pCurrent->m_pNext;
    }
    UnlockDispatchQueue();
}

// FUNCTION: LEMBALL 0x004633B0
void GAME_RenderDispatchQueue::UnregisterOrderedRenderDispatchClient(void *pClient, int nOrder) {
    GAME_RenderDispatchClientNode *pCurrent;
    GAME_RenderDispatchClientNode *pPrevious;
    unsigned int i;

    LockDispatchQueue();
    pCurrent = (GAME_RenderDispatchClientNode *)m_pClientList;
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
            return;
        }
        ++i;
        pPrevious = pCurrent;
        pCurrent = pCurrent->m_pNext;
    }
    UnlockDispatchQueue();
}

// FUNCTION: LEMBALL 0x004662B0
void GAME_PrimaryContext::SetWindowOwnerScaleFactor(int nScaleFactor) {
    int nPreviousScaleFactor;
    void **pVtable;

    nPreviousScaleFactor = m_nScaleFactor;
    if (nScaleFactor != nPreviousScaleFactor) {
        pVtable = *(void ***)this;
        m_nScaleFactor = nScaleFactor;
        ((void (*)(int))pVtable[0x2c / sizeof(void *)])(nPreviousScaleFactor);
        ((void (*)(int))pVtable[0x58 / sizeof(void *)])(nPreviousScaleFactor);
        ((void (*)())pVtable[0x20 / sizeof(void *)])();
        ((void (*)())pVtable[0x44 / sizeof(void *)])();
    }
}

void *GAME_ScreenObject::ConstructIntroSequenceScreen(void *pPrimaryContext,
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

void *GAME_ScreenObject::ConstructMainMenuScreen(void *pPrimaryContext, void *pWindowOwnerContext, short *paRect) {
    void *pScreenObject = this;
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    InitializeGameStubVtables();
    memset(pScreenObject, 0, 0x3bc);
    *(void **)pScreenObject = g_GAME_GenericScreenVtableSlots;
    return pScreenObject;
}

void *GAME_ScreenObject::ConstructOptionsScreen(void *pPrimaryContext, void *pWindowOwnerContext, short *paRect) {
    void *pScreenObject = this;
    (void)pPrimaryContext;
    (void)pWindowOwnerContext;
    (void)paRect;
    InitializeGameStubVtables();
    memset(pScreenObject, 0, 0x3ac);
    *(void **)pScreenObject = g_GAME_GenericScreenVtableSlots;
    return pScreenObject;
}

void *GAME_ScreenObject::ConstructLevelSelectionScreen(void *pPrimaryContext,
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

void *GAME_ScreenObject::ConstructLevelScreen(void *pPrimaryContext,
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

void *GAME_ScreenObject::ConstructRegistrationInfoScreen(void *pPrimaryContext,
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

void *GAME_ScreenObject::ConstructNetworkLobbyScreen(void *pPrimaryContext,
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

void *GAME_ScreenObject::ConstructPaintballSequenceScreen(void *pPrimaryContext,
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

void *GAME_ScreenObject::ConstructPasswordEntryScreen(void *pPrimaryContext,
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

void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x14(void *pPrimaryContext);
int LEMBALL_FASTCALL IsPrimaryContextWindowReadyForPresent(int *pPrimaryContext);
int LEMBALL_FASTCALL IsPrimaryContextWindowReadyForPresentThunk(int *pPrimaryContext);
void LEMBALL_FASTCALL PresentPrimaryContextScreen(void *pPrimaryContext);
void LEMBALL_FASTCALL UpdatePrimaryContextActiveScreen(void *pPrimaryContext);
int GetPrimaryContextMenuDefinition(int *pnMenuId, void **ppMenuDefinition);
int LEMBALL_FASTCALL PollPrimaryContextScreenStatus(void *pPrimaryContext);
int LEMBALL_FASTCALL GetPrimaryContextRequestedScreen(void *pPrimaryContext);
void LEMBALL_FASTCALL UpdateMainGameActiveMode(void *pMainContext);
void LEMBALL_FASTCALL PresentMainGameFrame(void *pMainContext);
void LEMBALL_FASTCALL RunMainGameLoop(GAME_MainContext *pMainContext);
extern FILE *OpenFileWithMode(const char *pszPath, const char *pszMode);
extern void CloseCrtFilePointer(FILE *pFile);

// FUNCTION: LEMBALL 0x0045ECB0
int SetVisualSciencesRegistryRunningState(const char *pszProductName, int fRunning) {
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
}

struct GAME_ModeVirtualInterface {
    virtual void Delete(int nDeleteFlag) = 0;
    virtual void Update(void) = 0;
};

struct GAME_NetworkRuntimeVirtualInterface {
    virtual void Reserved(void) = 0;
    virtual void Close(int nCloseFlag) = 0;
};

struct GAME_WindowOwnerVirtualInterface {
    virtual void Reserved0(void) = 0;
    virtual void Reserved1(void) = 0;
    virtual int GetCurrentWindowOwnerState(void) = 0;
};

struct GAME_WindowOwnerContext {
    char m_Reserved00[0x0c];
    GAME_WindowOwnerVirtualInterface *m_pStateSubobject;
};

struct GAME_ActiveScreenVirtualInterface {
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
};

struct GAME_PrimaryContextVirtualInterface {
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
    virtual int IsWindowReady(void) = 0;
    virtual void Reserved27(void) = 0;
    virtual void SetPresentMode(int nMode) = 0;
};

struct GAME_ActiveScreenPresentInterface {
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

struct GAME_ActiveScreenDeleteInterface {
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
    char szCandidatePath[256];
    DWORD dwDrives;
    char chDrive;
    int i;

    strcpy(szCandidatePath, g_GAME_CdromDrivePath);
    strcat(szCandidatePath, pszSuffix);
    dwDrives = GetLogicalDrives();
    chDrive = 'A';
    i = 0;

    while (i < 32) {
        if ((dwDrives & 1u) != 0) {
            szCandidatePath[0] = chDrive;
            g_GAME_CdromDrivePath[0] = chDrive;
            if (GetDriveTypeA(szCandidatePath) == DRIVE_CDROM) {
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
}

GAME_MainContext::GAME_MainContext(void) {
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
short *GAME_PrimaryContext::ComputePrimaryContextCenteredScreenRect(short *paRect,
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
short *GAME_PrimaryContext::ComputePrimaryContextCenteredScreenRectThunk(short *paRect,
                                                                          int nLeftOverride,
                                                                          int nTopOverride) {
    return ComputePrimaryContextCenteredScreenRect(paRect, nLeftOverride, nTopOverride);
}

// FUNCTION: LEMBALL 0x004010F0
GAME_PrimaryContext *GAME_PrimaryContext::ConstructPrimaryContextThunk(GAME_MainContext *pMainContext) {
    return ConstructPrimaryContext(pMainContext);
}

// FUNCTION: LEMBALL 0x00431590
GAME_PrimaryContext *GAME_PrimaryContext::ConstructPrimaryContext(GAME_MainContext *pMainContext) {
    GAME_PrimaryContext *pContext;
    void *pRenderQueueNode;

    pContext = this;
    pRenderQueueNode = (char *)pContext + 0x90;
    ConstructWindowOwnerRenderContext(this);
    InitializeRenderQueueNodeBase(pRenderQueueNode);

    m_pFinalizeThunk = g_pQueuedRenderPointSinkFinalizeThunk;
    m_cyCompactLayout = 0;
    m_pVtable = g_pPrimaryContextVtable;
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
    ((GAME_RenderDispatchQueue *)g_pSharedRenderDispatchQueue)
        ->RegisterOrderedRenderDispatchClient(pRenderQueueNode, -0x19);

    m_cxCompactLayout = 0x140;
    m_cyCompactLayout = 0xf0;
    m_cxWideLayout = 0x280;
    m_cyWideLayout = 0x1e0;
    m_fCompactLayout = g_fCompactPrimaryContextLayout;
    return this;
}

// FUNCTION: LEMBALL 0x00431950
void GAME_PrimaryContext::DestroyPrimaryContextActiveScreen(int) {
    m_fScreenLayoutDirty = 1;
    if (m_pActiveScreen != 0) {
        ((GAME_ActiveScreenVirtualInterface *)m_pActiveScreen)
            ->ReleaseActiveScreen();
        if (m_pActiveScreen != 0) {
            ((GAME_ActiveScreenDeleteInterface *)m_pActiveScreen)->Delete(1);
        }
        m_pActiveScreen = 0;
    }
}

// FUNCTION: LEMBALL 0x00431990
void GAME_PrimaryContext::SwitchPrimaryContextScreen(int nMode) {
    GAME_PrimaryContext *pContext;
    void *pScreenObject;
    short aRect[4];
    int nUploadState;
    void *pActiveLevelMode;

    pContext = this;
    if (pContext->m_nActiveScreenMode == nMode) {
        return;
    }

    pContext->SetWindowOwnerScaleFactor(1);
    aRect[0] = *(short *)((char *)pContext + 8);
    aRect[1] = *(short *)((char *)pContext + 10);
    aRect[2] = 0;
    aRect[3] = 0;

    nUploadState = ((GAME_WindowOwnerContext *)pContext->m_pWindowOwnerContext)
        ->m_pStateSubobject->GetCurrentWindowOwnerState();
    InitializeHelperUploadStatePending(nUploadState);
    PromoteHelperUploadStateToActive(nUploadState);

    pContext->m_nActiveScreenMode = nMode;
    switch (nMode) {
    case 1:
        pScreenObject = AllocateVSMemBlock(0x484);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GAME_ScreenObject *)pScreenObject)->ConstructIntroSequenceScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect, 0);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 2:
        pScreenObject = AllocateVSMemBlock(0x3bc);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GAME_ScreenObject *)pScreenObject)->ConstructMainMenuScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 3:
        pScreenObject = AllocateVSMemBlock(0x3ac);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GAME_ScreenObject *)pScreenObject)->ConstructOptionsScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 4:
        pScreenObject = AllocateVSMemBlock(0x488);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GAME_ScreenObject *)pScreenObject)->ConstructLevelSelectionScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect);
        } else {
            pContext->m_pActiveScreen = 0;
        }
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
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen = ((GAME_ScreenObject *)pScreenObject)->ConstructLevelScreen(
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
                ((GAME_ScreenObject *)pScreenObject)->ConstructRegistrationInfoScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 0x0c:
        pScreenObject = AllocateVSMemBlock(0x438);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GAME_ScreenObject *)pScreenObject)->ConstructNetworkLobbyScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 0x0e:
        pScreenObject = AllocateVSMemBlock(0x60c);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GAME_ScreenObject *)pScreenObject)->ConstructPaintballSequenceScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect, 1);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 0x0f:
        pScreenObject = AllocateVSMemBlock(0x60c);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GAME_ScreenObject *)pScreenObject)->ConstructPaintballSequenceScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect, 0);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 0x10:
        pScreenObject = AllocateVSMemBlock(0x494);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GAME_ScreenObject *)pScreenObject)->ConstructPasswordEntryScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    case 0x12:
        pScreenObject = AllocateVSMemBlock(0x484);
        if (pScreenObject != 0) {
            pContext->m_pActiveScreen =
                ((GAME_ScreenObject *)pScreenObject)->ConstructIntroSequenceScreen(
                    pContext, pContext->m_pWindowOwnerContext, aRect, 1);
        } else {
            pContext->m_pActiveScreen = 0;
        }
        break;

    default:
        break;
    }

    if (pContext->m_pActiveScreen != 0) {
        ((GAME_ActiveScreenVirtualInterface *)pContext->m_pActiveScreen)->FinalizePresent();
        ((GAME_ActiveScreenVirtualInterface *)pContext->m_pActiveScreen)->PrepareForPresent();
    }
}

// FUNCTION: LEMBALL 0x004318C0
int LEMBALL_FASTCALL IsPrimaryContextWindowReadyForPresent(int *pPrimaryContext) {
    short nHeight;
    short nWidth;

    if (!((GAME_PrimaryContextVirtualInterface *)pPrimaryContext)->IsWindowReady()) {
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
__declspec(naked) int LEMBALL_FASTCALL IsPrimaryContextWindowReadyForPresentThunk(int *) {
    __asm {
        jmp IsPrimaryContextWindowReadyForPresent
    }
}

// FUNCTION: LEMBALL 0x004318A0
void LEMBALL_FASTCALL ForwardPrimaryContextActiveScreenSlot0x14(void *pPrimaryContext) {
    void *pActiveScreen;

    pActiveScreen = *(void **)((char *)pPrimaryContext + 0xd4);
    if (pActiveScreen != 0) {
        ((GAME_ActiveScreenVirtualInterface *)pActiveScreen)
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
            ((GAME_PrimaryContextVirtualInterface *)pContext)->SetPresentMode(0);
        }
        pActiveScreen = *(void **)((char *)pPrimaryContext + 0xd4);
        if (pActiveScreen != 0) {
            ((GAME_ActiveScreenPresentInterface *)pActiveScreen)->Present();
        }
    }
}

// FUNCTION: LEMBALL 0x00431940
void LEMBALL_FASTCALL UpdatePrimaryContextActiveScreen(void *pPrimaryContext) {
    void *pActiveScreen;

    pActiveScreen = *(void **)((char *)pPrimaryContext + 0xd4);
    if (pActiveScreen != 0) {
        ((GAME_ActiveScreenVirtualInterface *)pActiveScreen)->UpdateActiveScreen();
    }
}

// FUNCTION: LEMBALL 0x00431EB0
int GetPrimaryContextMenuDefinition(int *pnMenuId, void **ppMenuDefinition) {
    *pnMenuId = 0x73;
    *ppMenuDefinition = g_GAME_PrimaryContextMenuDefinitionTable;
    return 1;
}

// FUNCTION: LEMBALL 0x00431EE0
int LEMBALL_FASTCALL PollPrimaryContextScreenStatus(void *pPrimaryContext) {
    if (*(int *)((char *)pPrimaryContext + 0xc8) != 0) {
        return 2;
    }
    if (*(void **)((char *)pPrimaryContext + 0xd4) != 0) {
        return CallIntVirtual(*(void **)((char *)pPrimaryContext + 0xd4), 0x2c);
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00431F10
int LEMBALL_FASTCALL GetPrimaryContextRequestedScreen(void *pPrimaryContext) {
    if (*(int *)((char *)pPrimaryContext + 0xc8) == 0 && *(void **)((char *)pPrimaryContext + 0xd4) != 0) {
        return CallIntVirtual(*(void **)((char *)pPrimaryContext + 0xd4), 0x28);
    }
    return 0;
}

// FUNCTION: LEMBALL 0x004077E0
void LEMBALL_FASTCALL UpdateMainGameActiveMode(void *pMainGameContext) {
    GAME_MainContext *pMainContext;
    GAME_StatusEntry *pProcessingStatus;
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
            pProcessingStatus = pMainContext->m_pProcessingStatus;
            dwNow = timeGetTime();
            *(DWORD *)((char *)pProcessingStatus + 0x20) = dwNow;
            *(int *)((char *)pProcessingStatus + 0x24) = 1;
        }

        ((GAME_ModeVirtualInterface *)pMainContext->m_pActiveMode)->Update();

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
        if (nScreenStatus == 1) {
            pMainContext->SwitchMainGameMode(*(int *)((char *)pMainContext->m_pActiveMode + 4));
        } else if (nScreenStatus == 2) {
            pMainContext->m_fQuitRequested = 1;
        }
    }

    nScreenStatus = PollPrimaryContextScreenStatus(pMainContext->m_pPrimaryContext);
    if (nScreenStatus == 1) {
        pMainContext->SwitchMainGameMode(GetPrimaryContextRequestedScreen(pMainContext->m_pPrimaryContext));
    } else if (nScreenStatus == 2) {
        pMainContext->m_fQuitRequested = 1;
    }

    if (pMainContext->m_fQuitRequested != 0) {
        if (((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext)->m_pActiveScreen != 0) {
            ((GAME_ModeVirtualInterface *)
                ((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext)->m_pActiveScreen)->Update();
        }
        if (pMainContext->m_pActiveMode != 0) {
            ((GAME_ModeVirtualInterface *)pMainContext->m_pActiveMode)->Delete(1);
            pMainContext->m_pActiveMode = 0;
        }
        ((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext)
            ->DestroyPrimaryContextActiveScreen(0);
    }
}

// FUNCTION: LEMBALL 0x004078F0
void LEMBALL_FASTCALL PresentMainGameFrame(void *pMainGameContext) {
    GAME_MainContext *pMainContext;
    GAME_StatusEntry *pRefreshingStatus;
    int fTimedStatus;
    DWORD dwNow;

    pMainContext = (GAME_MainContext *)pMainGameContext;
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
        ((void (*)(DWORD))(*(void ***)pRefreshingStatus)[1])(
            dwNow - *(DWORD *)((char *)pRefreshingStatus + 0x20));
        *(int *)((char *)pRefreshingStatus + 0x24) = 0;
    }
}

// FUNCTION: LEMBALL 0x00407950
void LEMBALL_FASTCALL RunMainGameLoop(GAME_MainContext *pMainContext) {
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
GAME_MainContext *GAME_MainContext::InitializeMainGameContext(const char *pszCmdLine) {
    char *pszCdromPath;
    GAME_MainContext *pMainContext;
    GAME_StatusEntry *pProcessingStatusEntry;
    GAME_StatusEntry *pRefreshingStatusEntry;
    void *pPrimaryStorage;

    pMainContext = this;

    pMainContext->m_pVariantMode = 0;
    g_pLevelProgressState = 0;
    pMainContext->m_pPrimaryContext = 0;
    g_pMainResourceArchive = 0;
    g_pVariantResourceEntryManager = 0;
    pMainContext->m_fQuitRequested = 1;
    pMainContext->m_pActiveMode = 0;
    SetVisualSciencesRegistryRunningState(g_GAME_WindowTitle, 1);

    pszCdromPath = FindCdromFilePathBySuffix(g_GAME_VsmemDllName);
    while (pszCdromPath == 0) {
        if (MessageBoxA(0, g_GAME_CdromErrorText, g_GAME_CdromErrorTitle, 1) != 1) {
            return pMainContext;
        }
        pszCdromPath = FindCdromFilePathBySuffix(g_GAME_VsmemDllName);
    }

    g_pLevelProgressState = AllocateVSMemBlock(0x50);
    if (g_pLevelProgressState != 0) {
        g_pLevelProgressState = ((GAME_LevelProgressState *)g_pLevelProgressState)->Clear();
    }

    pProcessingStatusEntry = (GAME_StatusEntry *)AllocateVSMemBlock(0x28);
    if (pProcessingStatusEntry != 0) {
        pProcessingStatusEntry = new (pProcessingStatusEntry) GAME_StatusEntry(g_GAME_ProcessingName);
        *(int *)((char *)pProcessingStatusEntry + 0x20) = 0;
        *(int *)((char *)pProcessingStatusEntry + 0x24) = 0;
        pMainContext->m_pProcessingStatus = pProcessingStatusEntry;
    } else {
        pMainContext->m_pProcessingStatus = 0;
    }

    pRefreshingStatusEntry = (GAME_StatusEntry *)AllocateVSMemBlock(0x28);
    if (pRefreshingStatusEntry != 0) {
        pRefreshingStatusEntry = new (pRefreshingStatusEntry) GAME_StatusEntry(g_GAME_RefreshingName);
        *(int *)((char *)pRefreshingStatusEntry + 0x20) = 0;
        *(int *)((char *)pRefreshingStatusEntry + 0x24) = 0;
        pMainContext->m_pRefreshingStatus = pRefreshingStatusEntry;
    } else {
        pMainContext->m_pRefreshingStatus = 0;
    }
    pMainContext->m_nModeTicks = 0;
    ((VSINIT_StatusEntryPointerArray *)g_pStatusEntryRegistry)
        ->AppendStatusEntry(pMainContext->m_pProcessingStatus);
    ((VSINIT_StatusEntryPointerArray *)g_pStatusEntryRegistry)
        ->AppendStatusEntry(pMainContext->m_pRefreshingStatus);

    pPrimaryStorage = AllocateResourceArchiveMemory(0x28);
    if (pPrimaryStorage != 0) {
        g_pMainResourceArchive =
            ((MOGLOAD_ResourceArchive *)pPrimaryStorage)->ConstructResourceArchive(
                g_GAME_MainArchiveName, 0x177000);
    } else {
        g_pMainResourceArchive = 0;
    }
    if (!ValidateResourceFileSignatureThunk(pMainContext)) {
        TriggerReleaseAssertFailure(g_GAME_IsValidResourceFileAssert, g_GAME_SourceFileName, 0x16e);
    }

    pPrimaryStorage = AllocateVSMemBlock(0xe4);
    if (pPrimaryStorage != 0) {
        char abTitleBuffer[0x50];
        short aScreenRect[4];
        VSINIT_FixedBufferStream TitleBufferStream;
        VSINIT_FormattedOutputStream TitleStream;

        pMainContext->m_pPrimaryContext =
            ((GAME_PrimaryContext *)pPrimaryStorage)
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
        ((void (*)(void *, short *, int, void *))(
            (*(void ***)pMainContext->m_pPrimaryContext)[1]))(
            pMainContext->m_pPrimaryContext,
            ((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext)
                ->ComputePrimaryContextCenteredScreenRectThunk(aScreenRect, -1, -1),
            0, &TitleStream);
        DestroyFixedBufferStream(&TitleBufferStream);
    } else {
        pMainContext->m_pPrimaryContext = 0;
    }

    InitializeGlobalAudioManager(g_fMusicEnabled, g_fEffectsEnabled, 0x32, pMainContext->m_pPrimaryContext);
    if (g_fMusicEnabled != 0) {
        ((AUDIO_Manager *)g_pAudioManager)->SetAudioManagerMusicEnabledFlag(1);
        ((AUDIO_Manager *)g_pAudioManager)
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
GAME_MainContext *GAME_MainContext::InitializeMainGameContextThunk(
    const char *pszCmdLine) {
    return InitializeMainGameContext(pszCmdLine);
}

// FUNCTION: LEMBALL 0x00407420
void GAME_MainContext::SwitchMainGameMode(int nMode) {
    GAME_MainContext *pMainContext;
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
        ((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext)
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
        ((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext)
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
        ((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext)
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
        ((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext)
            ->DestroyPrimaryContextActiveScreen(nMode);
        pModeObject = AllocateVSMemBlock(0x28);
        pMainContext->m_pActiveMode =
            pModeObject == 0 ? 0 : ConstructLevelSelectionModeController(pModeObject, pMainContext);
        break;

    case 5:
        goto level_game_mode;

    case 10:
        pMainContext->m_nActiveMode = nMode;
        ((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext)
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
        ((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext)
            ->DestroyPrimaryContextActiveScreen(nMode);
        pModeObject = AllocateVSMemBlock(0x38);
        pMainContext->m_pActiveMode =
            pModeObject == 0 ? 0 : ConstructNetworkLobbyTransportController(pModeObject, pMainContext);
        break;

    case 0x0e:
    case 0x0f:
        pMainContext->m_nActiveMode = nMode;
        ((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext)
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
        ((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext)
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
        ((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext)
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

    ((GAME_PrimaryContext *)pMainContext->m_pPrimaryContext)
        ->SwitchPrimaryContextScreen(pMainContext->m_nActiveMode);
}

// FUNCTION: LEMBALL 0x004071D0
void LEMBALL_FASTCALL ShutdownMainGameContext(GAME_MainContext *pMainContext) {
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
        ((GAME_NetworkRuntimeVirtualInterface *)g_pNetworkLobbyVsnetRuntime)->Close(1);
        g_pNetworkLobbyVsnetRuntime = 0;
    }

    if (pMainContext->m_pPrimaryContext != 0) {
        ((void (LEMBALL_FASTCALL *)(void *))
            (*(void ***)pMainContext->m_pPrimaryContext)[0x74 / 4])(
            pMainContext->m_pPrimaryContext);
        if (pMainContext->m_pPrimaryContext != 0) {
            ((GAME_NetworkRuntimeVirtualInterface *)
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
__declspec(naked) int RunMainGameSessionThunk(int cArgs, const char *const *ppszArgs) {
    (void)cArgs;
    (void)ppszArgs;
    __asm {
        jmp RunMainGameSession
    }
}

// FUNCTION: LEMBALL 0x00406310
int RunMainGameSession(int cArgs, const char *const *ppszArgs) {
    GAME_MainContext *pMainContext;
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
        pMainContext = (GAME_MainContext *)AllocateVSMemBlock(0x70);
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
