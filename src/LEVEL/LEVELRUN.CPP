#include "GAME.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/MEDIA/VSSTRM.H"
#include "ENGINE/CORE/SAFEVT.H"
#include "ENGINE/MEDIA/EFFSTRM.H"
#include "LEVEL/LEVELRUN.H"

#include <stdlib.h>

extern "C" DWORD WINAPI timeGetTime(void);

extern int g_nLevelFrameClockTick;
int g_nNetworkFrameClockLatestTimeMs = 0;
int g_nLevelFrameClockLastSampleMs = 0;
int g_nLevelFrameClockTimeMs = 0;
int g_nLevelFrameClockDemoTimeMs = 0;
int g_nLevelFrameClockTimeBaseMs = 0;
int g_nLevelFrameClockTickBase = 0;
void *LEMBALL_FASTCALL GetLevelGameModeOwnerThunk(void *pLevelMode);
int LEMBALL_FASTCALL GetSelectedLevelNumberThunk(void *pLevelProgressState);
void LEMBALL_FASTCALL ResetBallChunkEntriesThunk(void *pBallManager);
void InitializeManagedEntitySlotTablesEntryThunk(void *pLevelGameMode);
void LEMBALL_FASTCALL ResizeLevelTileReachabilityGridThunk(void *pReachabilityHelper);
void ResetLevelFrameClockEntryThunk(void);
void LEMBALL_FASTCALL ResetBoonChunkManagerObjectsThunk(void *pObject);
void UpdateNetworkFrameClockFromNetworkTimeThunk(unsigned int nTimeMs);
void DispatchType14CountedChildSlotC4Thunk(int nChunkManager);
void LEMBALL_FASTCALL FillReachabilityGridFromTileFlagsThunk(void *pReachabilityHelper);
void ResetLevelFrameClockThunk(void);
void UpdateNetworkFrameClockFromNetworkTime(unsigned int nTimeMs);
void AdvanceLevelFrameClock(void);
void LEMBALL_FASTCALL ServiceRegisteredLevelChunkStreams(void *pDispatcher);
void LEMBALL_FASTCALL ServiceCachedChunkObjects(void *pManager);
void LoadLevelTileGridChunk(void *pTileGrid, const unsigned short *pWords);
void LoadLevelAiChunk(void *pLevelMode, const unsigned short *pWords,
                      unsigned int cbPayload);
void LoadLevelFlagChunk(void *pLevelMode, const unsigned short *pWords);
void SetLevelTileGridDefaultVariantIds(void *pTileGrid,
                                       const unsigned short *pWords);
void SetLevelTileGridName(void *pTileGrid, const char *pszName);
void LoadAnimChunkRecords(void *pTimerBlock, const unsigned short *pWords);
void LEMBALL_FASTCALL AdvanceAnimChunkFrames(void *pTimerBlock);
void LoadNodeChunkRecords(void *pManager, const unsigned short *pWords,
                          unsigned int cbPayload, int fReuseState);
int LEMBALL_FASTCALL GetSelectedLevelNumber(void *pLevelProgressState);
static int __cdecl CompareLevelTileGridStaticRenderEntrySortKeys(
    const void *pLeft,
    const void *pRight);
void ResetRegisteredLevelChunkStreamsThunk(void *pDispatcher);
void RegisterLevelChunkStreamThunk(void *pDispatcher, void *pStreamObject);
void *LEMBALL_FASTCALL ReturnArgumentVtableCallback(void *pObject);
void *ConstructLevelGameStateStreamThunk(void *pObject);
void LEMBALL_FASTCALL LoadLevelGameStateStreamPayload(void *pObject);
void AssignNetworkPlayerManagedEntitySlotIdsThunk(int nLevelMode);
void InitializeLevelChunkTypeEnabledFlagsThunk(void *pLevelMode);
void *LEMBALL_FASTCALL GetLevelGameModeOwner(void *pLevelMode);
void AssignMissingManagedEntitySlotIdsThunk(int nLevelMode);
void SetLevelChunkTypeEnabledFlagThunk(void *pLevelMode, int nChunkType, int fEnabled);
void InitializeManagedEntitySlotTablesThunk(void *pLevelGameMode);
unsigned short LEMBALL_FASTCALL GetManagedEntitySlotId(
    int nManagedEntityObject);
void SetManagedEntitySlotId(void *pManagedEntityObject, unsigned short nSlotId);
unsigned int FindFirstFreeManagedEntitySlotIdForwardThunk(void);
unsigned int FindLastFreeManagedEntitySlotIdThunk(void);
void ClaimManagedEntitySlotId(int nManagedEntityObject);
void LEMBALL_FASTCALL ResetBallChunkEntries(void *pBallManager);
void LEMBALL_FASTCALL FillReachabilityGridFromTileFlags(void *pReachabilityHelper);
void LEMBALL_FASTCALL ResizeLevelTileReachabilityGrid(void *pReachabilityHelper);
void LEMBALL_FASTCALL ResetBoonChunkManagerObjects(void *pObject);
void ClearLevelTileGridCells(int nTileGridCells);
void *LEMBALL_FASTCALL ConstructLevelTileGridThunk(void *pObject);
void InitializeLevelTileGridThunk(void *pTileGrid);
void LEMBALL_FASTCALL ReadNetworkLevelChunkDeltaStream(void *pObject);
void ClearPrimaryLockedRecordTablePayloadFlags(void *pPayload);
void ClearSecondaryLockedRecordTablePayloadFlags(void *pPayload);

// GLOBAL: LEMBALL 0x0049e1bc
LevelBallChunkManager *g_pBallChunkManager = 0;
// GLOBAL: LEMBALL 0x004a63fc
char *g_pszActiveLevelFilePath = 0;
// GLOBAL: LEMBALL 0x004a6400
void *g_pLoadedLevelFileBuffer;

// GLOBAL: LEMBALL 0x0049e0d0
static int g_LEVEL_ProjectileGeometryPairSeeds[3][8][2] = {
    {
        {0, 0}, {-16, 0}, {-32, 0}, {-48, 0},
        {-64, 0}, {-80, 0}, {-96, 0}, {-112, 0}
    },
    {
        {0, 0}, {-24, -16}, {-24, 16}, {-48, -32},
        {-48, 32}, {-72, -48}, {-72, 48}, {-96, -64}
    },
    {
        {0, 0}, {-24, -24}, {-24, 0}, {-24, 24},
        {-48, -48}, {-48, -24}, {-48, 0}, {-48, 24}
    }
};

int g_fLevelEndStateRequestLatched = 0;
// GLOBAL: LEMBALL 0x004a6510
int g_GAME_ManagedEntityRegistryTable[1000];
// GLOBAL: LEMBALL 0x004a74b0
void *g_pActiveManagedEntityOwner;
// GLOBAL: LEMBALL 0x004a74bc
unsigned short g_GAME_ManagedEntityRegistryCount;
// GLOBAL: LEMBALL 0x0049d108
static unsigned char g_GAME_ManagedEntitySlotBitMasks[8] = {
    1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80
};
// GLOBAL: LEMBALL 0x004a6410
static unsigned char g_GAME_ManagedEntitySlotClaimBitset[0x100];

struct LevelLevelTileCell {
    int m_nType00;
    short m_nReserved04;
    unsigned short m_nFlags06;
    short m_nReserved08;
    short m_nReserved0A;
};

struct LevelLevelTileGridStaticView {
    char m_abReserved00[0x0c];
    LevelLevelTileCell *m_pCells0C;
    int m_cColumns10;
    int m_cRows14;

    unsigned short GetTileFlags(int x, int y) const {
        if (x < 0 || y < 0 || x >= m_cColumns10 || y >= m_cRows14) {
            return 3;
        }
        return m_pCells0C[y * m_cColumns10 + x].m_nFlags06;
    }
};



extern int g_nSelectedNetworkLobbyPeerId;

void *g_pLiftTileGrid = 0;
void *g_pAnimChunkTileGrid = 0;























#define LevelRESERVED_VIRTUAL(n) virtual void Reserved##n(void) = 0

struct LevelVirtualSlots00Through40 {
    LevelRESERVED_VIRTUAL(00);
    LevelRESERVED_VIRTUAL(01);
    LevelRESERVED_VIRTUAL(02);
    LevelRESERVED_VIRTUAL(03);
    LevelRESERVED_VIRTUAL(04);
    LevelRESERVED_VIRTUAL(05);
    LevelRESERVED_VIRTUAL(06);
    LevelRESERVED_VIRTUAL(07);
    LevelRESERVED_VIRTUAL(08);
    LevelRESERVED_VIRTUAL(09);
    LevelRESERVED_VIRTUAL(0A);
    LevelRESERVED_VIRTUAL(0B);
    LevelRESERVED_VIRTUAL(0C);
    LevelRESERVED_VIRTUAL(0D);
    LevelRESERVED_VIRTUAL(0E);
    LevelRESERVED_VIRTUAL(0F);
    LevelRESERVED_VIRTUAL(10);
    LevelRESERVED_VIRTUAL(11);
    LevelRESERVED_VIRTUAL(12);
    LevelRESERVED_VIRTUAL(13);
    LevelRESERVED_VIRTUAL(14);
    LevelRESERVED_VIRTUAL(15);
    LevelRESERVED_VIRTUAL(16);
    LevelRESERVED_VIRTUAL(17);
    LevelRESERVED_VIRTUAL(18);
    LevelRESERVED_VIRTUAL(19);
    LevelRESERVED_VIRTUAL(1A);
    LevelRESERVED_VIRTUAL(1B);
    LevelRESERVED_VIRTUAL(1C);
    LevelRESERVED_VIRTUAL(1D);
    LevelRESERVED_VIRTUAL(1E);
    LevelRESERVED_VIRTUAL(1F);
    LevelRESERVED_VIRTUAL(20);
    LevelRESERVED_VIRTUAL(21);
    LevelRESERVED_VIRTUAL(22);
    LevelRESERVED_VIRTUAL(23);
    LevelRESERVED_VIRTUAL(24);
    LevelRESERVED_VIRTUAL(25);
    LevelRESERVED_VIRTUAL(26);
    LevelRESERVED_VIRTUAL(27);
    LevelRESERVED_VIRTUAL(28);
    LevelRESERVED_VIRTUAL(29);
    LevelRESERVED_VIRTUAL(2A);
    LevelRESERVED_VIRTUAL(2B);
    LevelRESERVED_VIRTUAL(2C);
    LevelRESERVED_VIRTUAL(2D);
    LevelRESERVED_VIRTUAL(2E);
    LevelRESERVED_VIRTUAL(2F);
    LevelRESERVED_VIRTUAL(30);
    LevelRESERVED_VIRTUAL(31);
    LevelRESERVED_VIRTUAL(32);
    LevelRESERVED_VIRTUAL(33);
    LevelRESERVED_VIRTUAL(34);
    LevelRESERVED_VIRTUAL(35);
    LevelRESERVED_VIRTUAL(36);
    LevelRESERVED_VIRTUAL(37);
    LevelRESERVED_VIRTUAL(38);
    LevelRESERVED_VIRTUAL(39);
    LevelRESERVED_VIRTUAL(3A);
    LevelRESERVED_VIRTUAL(3B);
    LevelRESERVED_VIRTUAL(3C);
    LevelRESERVED_VIRTUAL(3D);
    LevelRESERVED_VIRTUAL(3E);
    LevelRESERVED_VIRTUAL(3F);
    LevelRESERVED_VIRTUAL(40);
};

struct LevelNestedChildInterface : public LevelVirtualSlots00Through40 {
    virtual void Activate(void) = 0;
};

struct LevelType35ChunkObject : public LevelVirtualSlots00Through40 {
    virtual void Reset(void) = 0;
    char m_abReserved04[0x14c];
};

struct LevelNestedOwnerInterface : public LevelVirtualSlots00Through40 {
    LevelRESERVED_VIRTUAL(41);
    virtual int GetNestedChildCount(void) = 0;
    LevelRESERVED_VIRTUAL(43);
    LevelRESERVED_VIRTUAL(44);
    LevelRESERVED_VIRTUAL(45);
    LevelRESERVED_VIRTUAL(46);
    LevelRESERVED_VIRTUAL(47);
    virtual LevelNestedChildInterface *GetNestedChild(int nIndex) = 0;
};

#undef LevelRESERVED_VIRTUAL







extern int PASCAL ReturnTrueVtableCallbackThunk(void *pObject);
extern int ReturnTrueVtableCallbackSecondaryThunk(void);
extern void NoopVtableCallbackThunk(void);
struct GameNetworkLevelChunkDeltaStream {
    void *ConstructNetworkLevelChunkDeltaStream(int nOwner);
    void *ConstructNetworkLevelChunkDeltaStreamEntry(int nOwner);
};
void LEMBALL_FASTCALL ReadNetworkLevelChunkDeltaStream(void *pObject);
void LEMBALL_FASTCALL LoadLevelGameStateStreamPayload(void *pObject);
void LEMBALL_FASTCALL ReadNetworkLevelChunkDeltaStreamThunk(void *pObject);
void *LEMBALL_FASTCALL DeleteNetworkLevelChunkDeltaStreamThunk(
    void *pObject, int nUnused, unsigned char fDelete);
void LEMBALL_FASTCALL WriteNetworkLevelChunkDeltaStreamThunk(void *pObject);

// GLOBAL: LEMBALL 0x00498610
static void *g_GAME_NetworkLevelChunkDeltaStreamVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)ReadNetworkLevelChunkDeltaStreamThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)WriteNetworkLevelChunkDeltaStreamThunk,
    (void *)DeleteNetworkLevelChunkDeltaStreamThunk,
};
static void *g_GAME_LevelGameStateStreamVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)LoadLevelGameStateStreamPayload,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
};
static void *g_pLevelGameStateStreamVtable =
    g_GAME_LevelGameStateStreamVtable;

struct GameLevelChunkStreamDispatcher {
    int m_nRenderQueueNode00;
    int m_nRenderQueueTag04;
    int m_nRenderQueueReserved08;
    int m_nRenderQueueReserved0C;
    void **m_ppRegisteredStreams10;
    int m_cChunkTypes14;
    int m_cRegisteredStreams18;
    int *m_pChunkTypeToIndex1C;
};

struct GameLevelTileReachabilityHelper {
    int m_pTileGrid00;
    int m_ppRows04;
    int m_nReserved08;
    char m_abReserved0C[0x1000];
    int m_cColumns100C;
    int m_cRows1010;
    int m_nReserved1014;
};

struct GameLevelTileGrid {
    int m_nReserved00;
    int m_pReserved04;
    int m_cColumns08;
    int m_cRows0C;
    int m_pCells10;
};

void ClaimManagedEntitySlotId(int nManagedEntityObject);
unsigned short LEMBALL_FASTCALL GetManagedEntitySlotId(
    int nManagedEntityObject);
void SetManagedEntitySlotId(void *pManagedEntityObject, unsigned short nSlotId);
unsigned int FindFirstFreeManagedEntitySlotIdForwardThunk(void);
unsigned int FindLastFreeManagedEntitySlotIdThunk(void);
void SetLevelChunkTypeEnabledFlagThunk(void *pLevelMode, int nChunkType, int fEnabled);
void DispatchType14CountedChildSlotC4Thunk(int nChunkManager);
void LEMBALL_FASTCALL FillReachabilityGridFromTileFlagsThunk(void *pReachabilityHelper);








void InitializeManagedEntitySlotTablesThunk(void *pLevelGameMode);



struct GameLockedRecordSlotTableView {
    int m_cRecords;
    int m_nReserved04;
    int *m_apRecords08;

    void ClearLockedRecordSlotPayloadFlags(void);
};










struct LevelTileReachabilityHelper {
    void *ConstructLevelTileReachabilityHelperThunk(void *pTileGrid);
};




















struct LevelChunkObjectResetView;

typedef void (LEMBALL_FASTCALL *LevelChunkObjectResetProc)(
    LevelChunkObjectResetView *pObject);

struct LevelChunkObjectResetVtable {
    void *m_apReserved00[0x41];
    LevelChunkObjectResetProc m_pResetObject104;
};

struct LevelChunkObjectResetView {
    LevelChunkObjectResetVtable *m_pVtable;
};

struct LevelBallChunkManagerResetView {
    int m_nReserved00;
    LevelChunkObjectResetView **m_ppObjects04;
    int m_nReserved08;
    int m_cObjects0c;
};



struct LevelBoonChunkManagerResetView {
    unsigned short m_nActiveMask00;
    unsigned char m_abReserved02[0x32];
    LevelChunkObjectResetView *m_apObjects34[4];
};


#include "ENGINE/CORE/SAFEVT.H"


// FUNCTION: LEMBALL 0x0040117c
void *LEMBALL_FASTCALL GetLevelGameModeOwnerThunk(void *pLevelMode) {
    return GetLevelGameModeOwner(pLevelMode);
}

// FUNCTION: LEMBALL 0x004011ea
int LEMBALL_FASTCALL GetSelectedLevelNumberThunk(void *pLevelProgressState) {
    return GetSelectedLevelNumber(pLevelProgressState);
}

// FUNCTION: LEMBALL 0x00401230
void LevelNodeChunkRecord::ResetNodeChunkRecordLinkCountThunk(void) {
    ResetNodeChunkRecordLinkCount();
}

// FUNCTION: LEMBALL 0x004012fd
void LevelSpecialTileGridStaticRenderList::BuildSpecialLevelTileGridStaticRenderEntriesThunk(
    LevelLevelTileGridStaticView *pTileGrid)

{
    BuildSpecialLevelTileGridStaticRenderEntries(pTileGrid);
}

// FUNCTION: LEMBALL 0x004015e1
LevelNodeChunkManager *LevelNodeChunkManager::ConstructNodeChunkManagerThunk(int cCapacity) {
    return ConstructNodeChunkManager(cCapacity);
}

// FUNCTION: LEMBALL 0x00401d48
void LEMBALL_FASTCALL ResetBallChunkEntriesThunk(void *pBallManager) {
    ResetBallChunkEntries(pBallManager);
}

// FUNCTION: LEMBALL 0x00401dfc
void LevelEnmyChunkManager::ActivateNestedChildrenFromOwnerTableVariantThunk(void) {
    ActivateNestedChildrenFromOwnerTableVariant();
}

// FUNCTION: LEMBALL 0x00401e8d
void InitializeManagedEntitySlotTablesEntryThunk(void *pLevelGameMode) {
    InitializeManagedEntitySlotTablesThunk(pLevelGameMode);
}

// FUNCTION: LEMBALL 0x00402379
void LEMBALL_FASTCALL ResizeLevelTileReachabilityGridThunk(void *pReachabilityHelper) {
    ResizeLevelTileReachabilityGrid(pReachabilityHelper);
}

// FUNCTION: LEMBALL 0x00402423
void ResetLevelFrameClockEntryThunk(void) {
    ResetLevelFrameClockThunk();
}

// FUNCTION: LEMBALL 0x004024d2
LevelChunkLoaderContext *LevelChunkLoaderContext::ConstructLevelChunkLoaderContextThunk(void *pLevelMode) {
    return ConstructLevelChunkLoaderContext(pLevelMode);
}

// FUNCTION: LEMBALL 0x00402748
void LevelNodeChunkManager::ResetNodeChunkRecordsThunk(void) {
    ResetNodeChunkRecords();
}

// FUNCTION: LEMBALL 0x00402752
LevelProjectileGeometryPairTable *LevelProjectileGeometryPairTable::InitializeProjectileGeometryPairTableThunk(void) {
    return InitializeProjectileGeometryPairTable();
}

// FUNCTION: LEMBALL 0x004028f6
void *GameNetworkLevelChunkDeltaStream::ConstructNetworkLevelChunkDeltaStreamEntry(int nOwner) {
    return ConstructNetworkLevelChunkDeltaStream(nOwner);
}

// FUNCTION: LEMBALL 0x00402a86
void LevelSlnkChunkManager::ResetType35ChunkObjectsThunk(void) {
    ResetType35ChunkObjects();
}

// FUNCTION: LEMBALL 0x00402d51
void LEMBALL_FASTCALL ResetBoonChunkManagerObjectsThunk(void *pObject) {
    ResetBoonChunkManagerObjects(pObject);
}

// FUNCTION: LEMBALL 0x00402de2
LevelProjectileGeometryPair *LevelProjectileGeometryPair::InitializeSentinelThunk(void) {
    return InitializeSentinel();
}

// FUNCTION: LEMBALL 0x00402e78
void LevelProjectileGeometryPairTable::ClearProjectileGeometryField100Thunk(void) {
    ClearProjectileGeometryField100();
}

// FUNCTION: LEMBALL 0x00403107
void UpdateNetworkFrameClockFromNetworkTimeThunk(unsigned int nTimeMs) {
    UpdateNetworkFrameClockFromNetworkTime(nTimeMs);
}

// FUNCTION: LEMBALL 0x0040312f
void LevelShpgChunkManager::ActivateNestedChildrenFromOwnerTableA4Thunk(void) {
    ActivateNestedChildrenFromOwnerTableA4();
}

// FUNCTION: LEMBALL 0x004031bb
void DispatchType14CountedChildSlotC4Thunk(int nChunkManager) {
    int *pChildObject;
    int i;
    int nOffset;

    nOffset = 0;
    i = 0;
    if (*(short *)(nChunkManager + 0x36) != 0) {
        do {
            pChildObject = *(int **)(*(int *)(nChunkManager + 0x3c) + nOffset);
            if (pChildObject[0x19] == 0x14) {
                ((void (*)())*(void **)(*pChildObject + 0xc4))();
            }
            nOffset += 4;
            ++i;
        } while (i < (int)(unsigned int)*(unsigned short *)(nChunkManager + 0x36));
    }
}

// FUNCTION: LEMBALL 0x00403364
void LEMBALL_FASTCALL FillReachabilityGridFromTileFlagsThunk(void *pReachabilityHelper) {
    FillReachabilityGridFromTileFlags(pReachabilityHelper);
}

// FUNCTION: LEMBALL 0x00403454
LevelSlnkChunkManager *LevelSlnkChunkManager::ConstructSlnkChunkManagerThunk(void *pLevelMode, int cCapacity) {
    return ConstructSlnkChunkManager(pLevelMode, cCapacity);
}

// FUNCTION: LEMBALL 0x004034ef
void LevelField194TimerBlock::ResetLevelField194TimerBlockThunk(void) {
    ResetLevelField194TimerBlock();
}

// FUNCTION: LEMBALL 0x004037d8
LevelBallChunkManager *LevelBallChunkManager::ConstructBallChunkManagerThunk(void *pLevelMode, int cCapacity) {
    return ConstructBallChunkManager(pLevelMode, cCapacity);
}

// FUNCTION: LEMBALL 0x00408090
void ResetLevelFrameClockThunk(void) {
    g_nNetworkFrameClockLatestTimeMs = 0;
    g_nLevelFrameClockLastSampleMs = 0;
    g_nLevelFrameClockTimeMs = 0;
    g_nLevelFrameClockTick = 0;
    g_nLevelFrameClockDemoTimeMs = 0;
    g_nLevelFrameClockTimeBaseMs = (int)timeGetTime();
    g_nLevelFrameClockTickBase = g_nLevelFrameClockTimeBaseMs / 0x32;
}

// FUNCTION: LEMBALL 0x00408190
void UpdateNetworkFrameClockFromNetworkTime(unsigned int nTimeMs) {
    if ((unsigned int)g_nNetworkFrameClockLatestTimeMs < nTimeMs) {
        g_nLevelFrameClockTimeMs = (int)nTimeMs;
        g_nLevelFrameClockTick = (int)(nTimeMs / 0x32);
        g_nNetworkFrameClockLatestTimeMs = (int)nTimeMs;
    }
}

// FUNCTION: LEMBALL 0x00408210
LevelChunkLoaderContext *LevelChunkLoaderContext::ConstructLevelChunkLoaderContext(void *pLevelMode) {
    m_pLevelMode00 = pLevelMode;
    m_nFallbackLevel04 = 0;
    g_pLoadedLevelFileBuffer = 0;
    g_pszActiveLevelFilePath =
        (char *)GetLevelGameModeOwnerThunk(m_pLevelMode00);
    return this;
}

// FUNCTION: LEMBALL 0x00409080
int LEMBALL_FASTCALL GetSelectedLevelNumber(void *pLevelProgressState) {
    return *(int *)((char *)pLevelProgressState + 0x3c + *(int *)((char *)pLevelProgressState + 8) * 4);
}

// FUNCTION: LEMBALL 0x00409910
static int __cdecl CompareLevelTileGridStaticRenderEntrySortKeys(
    const void *pLeft,
    const void *pRight)

{
    const LevelTileGridStaticRenderEntry *pLeftEntry;
    const LevelTileGridStaticRenderEntry *pRightEntry;

    pLeftEntry = (const LevelTileGridStaticRenderEntry *)pLeft;
    pRightEntry = (const LevelTileGridStaticRenderEntry *)pRight;
    return (unsigned int)pLeftEntry->m_nSortKey04 -
           (unsigned int)pRightEntry->m_nSortKey04;
}

// FUNCTION: LEMBALL 0x00409930
void LevelSpecialTileGridStaticRenderList::BuildSpecialLevelTileGridStaticRenderEntries(
    LevelLevelTileGridStaticView *pTileGrid)

{
    int cEntries;
    int cColumns;
    int cRows;
    int y;
    int x;
    int iEntry;
    int nSortOffset;
    LevelLevelTileCell *pCell;

    cEntries = 0;
    cColumns = pTileGrid->m_cColumns10;
    cRows = pTileGrid->m_cRows14;
    y = 0;
    if (0 < cRows) {
        do {
            x = 0;
            while (x < cColumns) {
                pCell = pTileGrid->m_pCells0C +
                        y * pTileGrid->m_cColumns10 + x;
                switch (pCell->m_nType00) {
                case 0x210:
                case 0x215:
                case 0x216:
                case 0x219:
                case 0x21a:
                    ++cEntries;
                    break;
                default:
                    if ((pTileGrid->GetTileFlags(x, y) & 0x20) != 0) {
                        ++cEntries;
                    }
                    break;
                }
                ++x;
            }
            ++y;
        } while (y < cRows);
    }

    m_cEntries04 = cEntries;
    if (cEntries == 0) {
        m_pEntries00 = 0;
        return;
    }
    if (m_pEntries00 == 0) {
        m_pEntries00 = (LevelTileGridStaticRenderEntry *)
            AllocateVSMemBlock(cEntries * sizeof(LevelTileGridStaticRenderEntry));
    }

    iEntry = 0;
    y = 0;
    if (0 < cRows) {
        do {
            x = 0;
            while (x < cColumns) {
                pCell = pTileGrid->m_pCells0C +
                        y * pTileGrid->m_cColumns10 + x;
                switch (pCell->m_nType00) {
                case 0x210:
                case 0x215:
                case 0x216:
                case 0x219:
                case 0x21a:
                    m_pEntries00[iEntry].m_nTileX00 = (short)x;
                    m_pEntries00[iEntry].m_nTileY02 = (short)y;
                    m_pEntries00[iEntry].m_nSortKey04 =
                        (unsigned short)((x + y) << 6);
                    m_pEntries00[iEntry].m_pTileCell08 = pCell;
                    ++iEntry;
                    break;
                default:
                    if ((pTileGrid->GetTileFlags(x, y) & 0x20) == 0) {
                        break;
                    }
                    nSortOffset = 0;
                    switch (pCell->m_nType00) {
                    case 0x202:
                        nSortOffset = 0x20;
                        break;
                    case 0x206:
                        nSortOffset = 0x10;
                        break;
                    case 0x207:
                    case 0x20a:
                        nSortOffset = 8;
                        break;
                    }
                    m_pEntries00[iEntry].m_nTileX00 = (short)x;
                    m_pEntries00[iEntry].m_nTileY02 = (short)y;
                    m_pEntries00[iEntry].m_nSortKey04 =
                        (unsigned short)(((x + y) << 6) + nSortOffset);
                    m_pEntries00[iEntry].m_pTileCell08 = pCell;
                    ++iEntry;
                    break;
                }
                ++x;
            }
            ++y;
        } while (y < cRows);
    }

    qsort(m_pEntries00,
          m_cEntries04,
          sizeof(LevelTileGridStaticRenderEntry),
          CompareLevelTileGridStaticRenderEntrySortKeys);
}

// FUNCTION: LEMBALL 0x0040b120
void ResetRegisteredLevelChunkStreamsThunk(void *pDispatcher) {
    typedef void (LEMBALL_FASTCALL *ResetStreamProc)(void *, int);
    GameLevelChunkStreamDispatcher *pStreamDispatcher;
    int *pStream;
    int i;
    int nOffset;

    pStreamDispatcher = (GameLevelChunkStreamDispatcher *)pDispatcher;
    nOffset = 0;
    i = 0;
    if (0 < pStreamDispatcher->m_cRegisteredStreams18) {
        do {
            pStream = *(int **)((unsigned long)pStreamDispatcher->m_ppRegisteredStreams10 + nOffset);
            if (pStream != 0) {
                ((ResetStreamProc)*(void **)((unsigned long)*pStream + 0x18))(
                    pStream, 0);
            }
            nOffset += 4;
            ++i;
        } while (i < pStreamDispatcher->m_cRegisteredStreams18);
    }
}

// FUNCTION: LEMBALL 0x0040b150
void RegisterLevelChunkStreamThunk(void *pDispatcher, void *pStreamObject) {
    typedef void (LEMBALL_FASTCALL *ResetStreamProc)(void *, int);
    GameLevelChunkStreamDispatcher *pStreamDispatcher;
    int nVtable;
    int *pStream;

    pStreamDispatcher = (GameLevelChunkStreamDispatcher *)pDispatcher;
    pStream = (int *)pStreamObject;
    pStreamDispatcher->m_pChunkTypeToIndex1C[pStream[0xb]] = pStreamDispatcher->m_cRegisteredStreams18;
    nVtable = *pStream;
    pStreamDispatcher->m_ppRegisteredStreams10[pStreamDispatcher->m_cRegisteredStreams18] = pStreamObject;
    ++pStreamDispatcher->m_cRegisteredStreams18;
    ((ResetStreamProc)*(void **)((unsigned long)nVtable + 0x18))(
        pStream, 0);
}

void AdvanceLevelFrameClock(void) {
    DWORD dwNow;
    unsigned int nTargetTime;
    extern void *g_pLevelDemoPlaybackController;
    extern int g_fLevelFrameClockPaused;

    if (g_fLevelFrameClockPaused != 0) {
        return;
    }
    if (g_pLevelDemoPlaybackController != 0 &&
        *(int *)((char *)g_pLevelDemoPlaybackController + 0x4c) != 0) {
        nTargetTime = (unsigned int)g_nLevelFrameClockLastSampleMs + 0x50;
        do {
            dwNow = timeGetTime();
        } while (dwNow - (DWORD)g_nLevelFrameClockTimeBaseMs < nTargetTime);
        g_nLevelFrameClockDemoTimeMs += 0x50;
        g_nLevelFrameClockLastSampleMs =
            (int)(dwNow - (DWORD)g_nLevelFrameClockTimeBaseMs);
        g_nLevelFrameClockTick = g_nLevelFrameClockDemoTimeMs / 0x32;
        g_nLevelFrameClockTimeMs = g_nLevelFrameClockTick * 0x32;
        return;
    }

    dwNow = timeGetTime();
    g_nLevelFrameClockTick =
        (int)(dwNow / 0x32) - g_nLevelFrameClockTickBase;
    if (g_nLevelFrameClockLastSampleMs + 2 < g_nLevelFrameClockTick) {
        g_nLevelFrameClockTickBase +=
            g_nLevelFrameClockTick - g_nLevelFrameClockLastSampleMs - 2;
        g_nLevelFrameClockTick = g_nLevelFrameClockLastSampleMs + 2;
    }
    g_nLevelFrameClockTimeMs = g_nLevelFrameClockTick * 0x32;
    g_nLevelFrameClockLastSampleMs = g_nLevelFrameClockTick;
}

// FUNCTION: LEMBALL 0x0040b320
void LEMBALL_FASTCALL ServiceRegisteredLevelChunkStreams(void *pDispatcher) {
    int i;
    int cStreams;
    void **ppStreams;

    cStreams = *(int *)((char *)pDispatcher + 0x18);
    ppStreams = *(void ***)((char *)pDispatcher + 0x10);
    for (i = 0; i < cStreams; ++i) {
        void *pStream = ppStreams[i];
        if (pStream != 0) {
            ((void (LEMBALL_FASTCALL *)(void *))(*(void ***)pStream)[7])(
                pStream);
        }
    }
}

void LEMBALL_FASTCALL ServiceCachedChunkObjects(void *pManager) {
    void **pVtable;
    void *pChild;

    pVtable = *(void ***)pManager;
    pChild = ((void *(LEMBALL_FASTCALL *)(void *))pVtable[1])(pManager);
    while (pChild != 0) {
        ((void (LEMBALL_FASTCALL *)(void *))(*(void ***)pChild)[5])(pChild);
        pChild = ((void *(LEMBALL_FASTCALL *)(void *))pVtable[2])(pManager);
    }
}

// FUNCTION: LEMBALL 0x0040b8e0
LevelSlnkChunkManager *LevelSlnkChunkManager::ConstructSlnkChunkManager(void *pLevelMode, int cCapacity) {
    m_pLevelMode00 = pLevelMode;
    m_cCapacity08 = cCapacity;
    m_pObjects04 = 0;
    return this;
}

// FUNCTION: LEMBALL 0x0040b900
void LevelSlnkChunkManager::ResetType35ChunkObjects(void) {
    LevelSlnkChunkManager *pManager;
    int i;
    int cbObjectOffset;

    pManager = this;
    cbObjectOffset = 0;
    if (pManager->m_pObjects04 != 0) {
        i = 0;
        if (0 < pManager->m_cCapacity08) {
            do {
                ((LevelType35ChunkObject *)((char *)pManager->m_pObjects04 + cbObjectOffset))->Reset();
                ++i;
                cbObjectOffset += 0x150;
            } while (i < pManager->m_cCapacity08);
        }
    }
}

// FUNCTION: LEMBALL 0x0040cf00
void *LEMBALL_FASTCALL ReturnArgumentVtableCallback(void *pObject) {
    return pObject;
}

// FUNCTION: LEMBALL 0x0040cf10
void LevelField194TimerBlock::ResetLevelField194TimerBlock(void) {
    int nNextTick;

    nNextTick = g_nLevelFrameClockTick;
    m_nReserved04 = 0;
    m_fActive12C8 = 1;
    m_nNextTick00 = nNextTick + 2;
}

// FUNCTION: LEMBALL 0x00410b80
void *ConstructLevelGameStateStreamThunk(void *pObject) {
    int *pStream;

    pStream = (int *)pObject;
    *(void **)pStream = g_GAME_EffStreamConstructionVtable;
    pStream[1] = 10;
    ((GameEffStream *)pStream)->ResetStateFields();
    *(void **)pStream = g_pLevelGameStateStreamVtable;
    pStream[6] += 0x10;
    pStream[9] = 1;
    return pObject;
}

// FUNCTION: LEMBALL 0x00410be0
void LEMBALL_FASTCALL LoadLevelGameStateStreamPayload(void *pObject) {
    VsNetEffStreamBase *pStream;
    unsigned int nValue;

    pStream = (VsNetEffStreamBase *)pObject;
    nValue = pStream->ReadEffStreamU32BEValue();
    *(unsigned int *)((char *)pObject + 0x2c) = nValue;
    nValue = pStream->ReadEffStreamU32BEValue();
    *(unsigned int *)((char *)pObject + 0x30) = nValue;
    pStream->ReadEffStreamU32BE((unsigned char *)((char *)pObject + 0x34));
    pStream->ReadEffStreamU32BE((unsigned char *)((char *)pObject + 0x38));
}

// FUNCTION: LEMBALL 0x00412080
void AssignNetworkPlayerManagedEntitySlotIdsThunk(int nLevelMode) {
    unsigned int nSlotId;
    int i;
    int *pOffsets;
    void **ppManagedEntities;
    int anOffsets[2];

    if (g_nSelectedNetworkLobbyPeerId != 0) {
        anOffsets[0] = 0;
        anOffsets[1] = 0;
        if (*(int *)(g_nSelectedNetworkLobbyPeerId + 0x1c) == 0) {
            anOffsets[1] = 4;
        } else {
            anOffsets[0] = 4;
        }

        pOffsets = anOffsets;
        do {
            i = 4;
            ppManagedEntities = (void **)(nLevelMode + 0x1d0 + *pOffsets * 4);
            do {
                nSlotId = FindLastFreeManagedEntitySlotIdThunk();
                SetManagedEntitySlotId(*ppManagedEntities, (unsigned short)nSlotId);
                --i;
                ++ppManagedEntities;
            } while (i != 0);
            ++pOffsets;
        } while (pOffsets < &anOffsets[2]);
    }
}

// FUNCTION: LEMBALL 0x00412100
void InitializeLevelChunkTypeEnabledFlagsThunk(void *pLevelMode) {
    int nChunkType;
    unsigned int i;
    unsigned int cManagedEntities;

    cManagedEntities = (unsigned int)g_GAME_ManagedEntityRegistryCount;
    for (i = 0; (int)i < (int)cManagedEntities; ++i) {
        if (g_GAME_ManagedEntityRegistryTable[i & 0xffff] != 0) {
            nChunkType = *(int *)(g_GAME_ManagedEntityRegistryTable[i & 0xffff] + 100);
            if (nChunkType != 0xffff) {
                SetLevelChunkTypeEnabledFlagThunk(pLevelMode, nChunkType, 1);
            }
        }
    }
    if (*(int *)(*(int *)((char *)pLevelMode + 0x180) + 8) == 0) {
        SetLevelChunkTypeEnabledFlagThunk(pLevelMode, 9, 0);
    }
    if (*(int *)(*(int *)((char *)pLevelMode + 400) + 0x34) == 0) {
        SetLevelChunkTypeEnabledFlagThunk(pLevelMode, 0x1a, 0);
        SetLevelChunkTypeEnabledFlagThunk(pLevelMode, 0x19, 0);
    }
    if (*(int *)(*(int *)((char *)pLevelMode + 0x1a0) + 0x34) == 0) {
        SetLevelChunkTypeEnabledFlagThunk(pLevelMode, 0x2f, 0);
        SetLevelChunkTypeEnabledFlagThunk(pLevelMode, 0x1e, 0);
    }
    if (*(int *)(*(int *)((char *)pLevelMode + 0x188) + 0x3c) == 0) {
        SetLevelChunkTypeEnabledFlagThunk(pLevelMode, 0x13, 0);
    }
    if (*(int *)(*(int *)((char *)pLevelMode + 0x198) + 0x34) == 0) {
        SetLevelChunkTypeEnabledFlagThunk(pLevelMode, 0x20, 0);
    }
    if (*(int *)(*(int *)((char *)pLevelMode + 0x1c8) + 0xc) == 0) {
        SetLevelChunkTypeEnabledFlagThunk(pLevelMode, 0x35, 0);
    }
}

// FUNCTION: LEMBALL 0x00412f00
void *LEMBALL_FASTCALL GetLevelGameModeOwner(void *pLevelMode) {
    return *(void **)((char *)pLevelMode + 0x154);
}

// FUNCTION: LEMBALL 0x00412fb0
void AssignMissingManagedEntitySlotIdsThunk(int nLevelMode) {
    void *pManagedEntityObject;
    unsigned short nSlotId;
    unsigned int nFreeSlotId;
    unsigned int i;
    unsigned int cManagedEntities;

    cManagedEntities = (unsigned int)g_GAME_ManagedEntityRegistryCount;
    i = 0;
    if (cManagedEntities != 0) {
        do {
            pManagedEntityObject = (void *)(unsigned long)g_GAME_ManagedEntityRegistryTable[i & 0xffff];
            nSlotId = GetManagedEntitySlotId((int)(unsigned long)pManagedEntityObject);
            if (nSlotId == 0xffff) {
                nFreeSlotId = FindFirstFreeManagedEntitySlotIdForwardThunk();
                SetManagedEntitySlotId(pManagedEntityObject, (unsigned short)nFreeSlotId);
            }
            ++i;
        } while ((int)i < (int)cManagedEntities);
    }
    DispatchType14CountedChildSlotC4Thunk(*(int *)(nLevelMode + 0x158));
}

// FUNCTION: LEMBALL 0x004130a0
void SetLevelChunkTypeEnabledFlagThunk(void *pLevelMode, int nChunkType, int fEnabled) {
    if (nChunkType < 0x211 || 0x212 < nChunkType) {
        *(int *)(*(int *)((char *)pLevelMode + 0x50) + nChunkType * 4) = fEnabled;
    }
}

// FUNCTION: LEMBALL 0x004165e0
void InitializeManagedEntitySlotTablesThunk(void *pLevelGameMode) {
    int i;
    unsigned char *pbBits;

    g_fLevelEndStateRequestLatched = 0;
    g_pActiveManagedEntityOwner = pLevelGameMode;
    pbBits = g_GAME_ManagedEntitySlotClaimBitset;
    for (i = 0x40; i != 0; --i) {
        pbBits[0] = 0;
        pbBits[1] = 0;
        pbBits[2] = 0;
        pbBits[3] = 0;
        pbBits += 4;
    }
    g_GAME_ManagedEntitySlotClaimBitset[0] |= g_GAME_ManagedEntitySlotBitMasks[0];
}

// FUNCTION: LEMBALL 0x00416610
unsigned short LEMBALL_FASTCALL GetManagedEntitySlotId(
    int nManagedEntityObject)

{
    return *(unsigned short *)(nManagedEntityObject + 0x6c);
}

// FUNCTION: LEMBALL 0x00416620
void SetManagedEntitySlotId(void *pManagedEntityObject, unsigned short nSlotId) {
    *(unsigned short *)((char *)pManagedEntityObject + 0x6c) = nSlotId;
    ClaimManagedEntitySlotId((int)(unsigned long)pManagedEntityObject);
}

// FUNCTION: LEMBALL 0x00416670
unsigned int FindFirstFreeManagedEntitySlotIdForwardThunk(void) {
    unsigned int iByte;
    int iBit;

    iByte = 0;
    do {
        if (g_GAME_ManagedEntitySlotClaimBitset[iByte] != 0xff) {
            iBit = 0;
            do {
                if ((g_GAME_ManagedEntitySlotBitMasks[iBit] & g_GAME_ManagedEntitySlotClaimBitset[iByte]) == 0) {
                    return (unsigned int)(iBit + iByte * 8);
                }
                ++iBit;
            } while (iBit < 8);
        }
        ++iByte;
    } while ((int)iByte < 0x100);
    return iByte & 0xffff0000;
}

// FUNCTION: LEMBALL 0x004166a0
unsigned int FindLastFreeManagedEntitySlotIdThunk(void) {
    unsigned int iByte;
    int iBit;

    iByte = 0xff;
    do {
        if (g_GAME_ManagedEntitySlotClaimBitset[iByte] != 0xff) {
            iBit = 7;
            do {
                if ((g_GAME_ManagedEntitySlotBitMasks[iBit] & g_GAME_ManagedEntitySlotClaimBitset[iByte]) == 0) {
                    return (unsigned int)(iBit + iByte * 8);
                }
                --iBit;
            } while (0 < iBit);
        }
        --iByte;
    } while (0 < (int)iByte);
    return iByte & 0xffff0000;
}

// FUNCTION: LEMBALL 0x00416740
void ClaimManagedEntitySlotId(int nManagedEntityObject) {
    unsigned char bClaimByte;
    unsigned short nSlotId;
    unsigned int i;
    unsigned int cManagedEntities;

    nSlotId = *(unsigned short *)(nManagedEntityObject + 0x6c);
    if (nSlotId != 0xffff) {
        bClaimByte = g_GAME_ManagedEntitySlotClaimBitset[nSlotId >> 3];
        if ((g_GAME_ManagedEntitySlotBitMasks[nSlotId & 7] & bClaimByte) != 0) {
            cManagedEntities =
                (unsigned int)g_GAME_ManagedEntityRegistryCount;
            for (i = 0; (int)i < (int)cManagedEntities; ++i) {
                if (g_GAME_ManagedEntityRegistryTable[i & 0xffff] != 0) {
                    GetManagedEntitySlotId(g_GAME_ManagedEntityRegistryTable[i & 0xffff]);
                }
            }
            *(unsigned short *)(nManagedEntityObject + 0x6c) = 0xffff;
            return;
        }
        g_GAME_ManagedEntitySlotClaimBitset[nSlotId >> 3] = g_GAME_ManagedEntitySlotBitMasks[nSlotId & 7] | bClaimByte;
    }
}

// FUNCTION: LEMBALL 0x0041a140
LevelProjectileGeometryPairTable *LevelProjectileGeometryPairTable::InitializeProjectileGeometryPairTable(void) {
    int i;
    int j;
    LevelProjectileGeometryPair *pPair;

    for (i = 0; i < 24; ++i) {
        m_aPrimaryPairs00[i].InitializeSentinelThunk();
    }
    for (i = 0; i < 8; ++i) {
        m_aSecondaryPairsC0[i].InitializeSentinelThunk();
    }

    pPair = m_aPrimaryPairs00;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 8; ++j) {
            pPair->m_nX = g_LEVEL_ProjectileGeometryPairSeeds[i][j][0] << 12;
            pPair->m_nY = g_LEVEL_ProjectileGeometryPairSeeds[i][j][1] << 12;
            ++pPair;
        }
    }
    return this;
}

// FUNCTION: LEMBALL 0x0041a1b0
void LevelProjectileGeometryPairTable::ClearProjectileGeometryField100(void) {
    m_nReserved100 = 0;
}

// FUNCTION: LEMBALL 0x0041a3c0
LevelProjectileGeometryPair *LevelProjectileGeometryPair::InitializeSentinel(void) {
    m_nX = 0xaa55aa55;
    m_nY = 0xaa55aa55;
    return this;
}

// FUNCTION: LEMBALL 0x0041f0e0
void LevelShpgChunkManager::ActivateNestedChildrenFromOwnerTableA4(void) {
    void **ppObject;
    int i;
    int j;
    int cChildren;

    i = 0;
    if (0 < m_cObjectsA4) {
        ppObject = m_apObjects04;
        do {
            j = 0;
            cChildren =
                ((LevelNestedOwnerInterface *)*ppObject)
                    ->GetNestedChildCount();
            if (0 < cChildren) {
                do {
                    ((LevelNestedOwnerInterface *)*ppObject)
                        ->GetNestedChild(j++)
                        ->Activate();
                } while (j < cChildren);
            }
            ++ppObject;
            ++i;
        } while (i < m_cObjectsA4);
    }
}

// FUNCTION: LEMBALL 0x00420bb0
void LevelEnmyChunkManager::ActivateNestedChildrenFromOwnerTableVariant(void) {
    void **ppObject;
    int i;
    int j;
    int cChildren;

    i = 0;
    if (0 < m_cObjectsA4) {
        ppObject = m_apObjects04;
        do {
            j = 0;
            cChildren =
                ((LevelNestedOwnerInterface *)*ppObject)
                    ->GetNestedChildCount();
            if (0 < cChildren) {
                do {
                    ((LevelNestedOwnerInterface *)*ppObject)
                        ->GetNestedChild(j++)
                        ->Activate();
                } while (j < cChildren);
            }
            ++ppObject;
            ++i;
        } while (i < m_cObjectsA4);
    }
}

// FUNCTION: LEMBALL 0x00421180
LevelNodeChunkManager *LevelNodeChunkManager::ConstructNodeChunkManager(int cCapacity) {
    m_pRecords00 = 0;
    m_cCapacity08 = cCapacity;
    return this;
}

// FUNCTION: LEMBALL 0x004211a0
void LevelNodeChunkManager::ResetNodeChunkRecords(void) {
    int i;

    if (m_pRecords00 != 0 && 0 < m_cCapacity08) {
        for (i = 0; i < m_cCapacity08; ++i) {
            m_pRecords00[i].ResetNodeChunkRecordLinkCountThunk();
        }
    }
}

// FUNCTION: LEMBALL 0x00421300
void LevelNodeChunkRecord::ResetNodeChunkRecordLinkCount(void) {
    m_cLinks10 = 0;
}

// FUNCTION: LEMBALL 0x00421ea0
LevelBallChunkManager *LevelBallChunkManager::ConstructBallChunkManager(void *pLevelMode, int cCapacity) {
    m_pLevelMode00 = pLevelMode;
    g_pBallChunkManager = this;
    m_pObjects04 = 0;
    m_cCapacity0C = cCapacity;
    return this;
}

// FUNCTION: LEMBALL 0x00421ec0
void LEMBALL_FASTCALL ResetBallChunkEntries(void *pBallManager) {
    LevelChunkObjectResetView *pChunkObject;
    int i;
    int cEntries;

    cEntries = 0;
    if (((LevelBallChunkManagerResetView *)pBallManager)->m_ppObjects04 != 0 &&
        0 < ((LevelBallChunkManagerResetView *)pBallManager)->m_cObjects0c) {
        i = 0;
        do {
            cEntries += 4;
            ++i;
            pChunkObject = *(LevelChunkObjectResetView **)(
                (char *)((LevelBallChunkManagerResetView *)pBallManager)->m_ppObjects04 - 4 + cEntries);
            pChunkObject->m_pVtable->m_pResetObject104(pChunkObject);
        } while (i < ((LevelBallChunkManagerResetView *)pBallManager)->m_cObjects0c);
    }
}

// FUNCTION: LEMBALL 0x00423090
void *LevelTileReachabilityHelper::ConstructLevelTileReachabilityHelperThunk(void *pTileGrid) {
    *(void **)this = pTileGrid;
    *(int *)((char *)this + 4) = 0;
    *(unsigned char *)((char *)this + 0x100c) = 0;
    *(int *)((char *)this + 8) = 0;
    *(int *)((char *)this + 0x1010) = 0;
    *(int *)((char *)this + 0x1014) = 0;
    return this;
}

// FUNCTION: LEMBALL 0x00423110
void LEMBALL_FASTCALL FillReachabilityGridFromTileFlags(void *pReachabilityHelper) {
    GameLevelTileReachabilityHelper *pHelper;
    GameLevelTileGrid *pTileGrid;
    int nTileFlags;
    unsigned short nCellValue;
    int y;
    int x;
    unsigned short *pReachabilityRow;

    pHelper = (GameLevelTileReachabilityHelper *)pReachabilityHelper;
    pTileGrid = (GameLevelTileGrid *)(unsigned long)pHelper->m_pTileGrid00;
    y = 0;
    if (0 < pHelper->m_cRows1010) {
        do {
            x = 0;
            pReachabilityRow = (unsigned short *)(unsigned long)(*(int *)(pHelper->m_ppRows04 + y * 4));
            if (0 < pHelper->m_cColumns100C) {
                do {
                    if (x < 0 || y < 0) {
                        nCellValue = 3;
                    } else {
                        nTileFlags = pHelper->m_pTileGrid00;
                        if (pTileGrid->m_cColumns08 <= x || pTileGrid->m_cRows0C <= y) {
                            nCellValue = 3;
                        } else {
                            nCellValue = *(unsigned short *)(pTileGrid->m_pCells10 + 6 +
                                                             (y * pTileGrid->m_cColumns08 + x) * 0xc);
                        }
                    }

                    if ((nCellValue & 1) == 0) {
                        pReachabilityRow[x] = 0xff00;
                    } else {
                        pReachabilityRow[x] = 0xffff;
                    }
                    ++x;
                } while (x < pHelper->m_cColumns100C);
            }
            ++y;
        } while (y < pHelper->m_cRows1010);
    }
}

// FUNCTION: LEMBALL 0x00423190
void LEMBALL_FASTCALL ResizeLevelTileReachabilityGrid(void *pReachabilityHelper) {
    GameLevelTileReachabilityHelper *pHelper;
    GameLevelTileGrid *pTileGrid;
    int i;
    int cRows;
    int cAllocatedRows;
    int **ppRows;

    pHelper = (GameLevelTileReachabilityHelper *)pReachabilityHelper;
    pTileGrid = (GameLevelTileGrid *)(unsigned long)pHelper->m_pTileGrid00;
    i = 0;
    if (pHelper->m_ppRows04 != 0) {
        ppRows = (int **)(unsigned long)(pHelper->m_ppRows04 - 4);
        if (0 < pHelper->m_cRows1010) {
            cAllocatedRows = 0;
            do {
                ++ppRows;
                ++cAllocatedRows;
                FreeVSMemBlock(*ppRows);
            } while (cAllocatedRows < pHelper->m_cRows1010);
        }
        FreeVSMemBlock((void *)(unsigned long)pHelper->m_ppRows04);
    }

    pHelper->m_cColumns100C = pTileGrid->m_cColumns08;
    cRows = pTileGrid->m_cRows0C;
    i = 0;
    cAllocatedRows = 0;
    pHelper->m_cRows1010 = cRows;
    pHelper->m_ppRows04 = (int)(unsigned long)AllocateVSMemBlock((unsigned int)cRows << 2);
    ppRows = (int **)(unsigned long)(pHelper->m_ppRows04 - 4);
    if (0 < pHelper->m_cRows1010) {
        do {
            ++ppRows;
            ++cAllocatedRows;
            *ppRows = (int *)AllocateVSMemBlock((unsigned int)pHelper->m_cColumns100C * 2);
        } while (cAllocatedRows < pHelper->m_cRows1010);
    }
    FillReachabilityGridFromTileFlagsThunk(pReachabilityHelper);
}

// FUNCTION: LEMBALL 0x0042a030
void LEMBALL_FASTCALL ResetBoonChunkManagerObjects(void *pObject) {
    LevelBoonChunkManagerResetView *pManager;

    pManager = (LevelBoonChunkManagerResetView *)pObject;
    pManager->m_nActiveMask00 = 0;
    pManager->m_apObjects34[0]->m_pVtable->m_pResetObject104(
        pManager->m_apObjects34[0]);
    pManager->m_apObjects34[1]->m_pVtable->m_pResetObject104(
        pManager->m_apObjects34[1]);
    pManager->m_apObjects34[2]->m_pVtable->m_pResetObject104(
        pManager->m_apObjects34[2]);
    pManager->m_apObjects34[3]->m_pVtable->m_pResetObject104(
        pManager->m_apObjects34[3]);
}

// FUNCTION: LEMBALL 0x00430370
void ClearLevelTileGridCells(int nTileGridCells) {
    int y;
    int x;
    int *pCell;
    int *pTileGridFields;

    pTileGridFields = (int *)(unsigned long)nTileGridCells;
    y = 0;
    if (0 < pTileGridFields[3]) {
        do {
            for (x = 0; x < pTileGridFields[2]; ++x) {
                pCell = (int *)(unsigned long)((y * pTileGridFields[2] + x) * 0xc + pTileGridFields[1]);
                pCell[0] = 0x209;
                *(short *)(pCell + 1) = 0;
                *(short *)((char *)pCell + 6) = 0;
                *(short *)(pCell + 2) = 0;
                *(short *)((char *)pCell + 10) = 0;
            }
            ++y;
        } while (y < pTileGridFields[3]);
    }
}

// FUNCTION: LEMBALL 0x004303c0
void *LEMBALL_FASTCALL ConstructLevelTileGridThunk(void *pObject) {
    *(int *)((char *)pObject + 0xc) = 0;
    *(int *)((char *)pObject + 0x10) = 0;
    *(int *)((char *)pObject + 0x14) = 0;
    g_pLiftTileGrid = pObject;
    g_pAnimChunkTileGrid = pObject;
    *(int *)((char *)pObject + 0x20) = 0;
    *(int *)((char *)pObject + 0x1c) = 0;
    *(int *)((char *)pObject + 0x48) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x004303f0
void InitializeLevelTileGridThunk(void *pTileGrid) {
    *(int *)((char *)pTileGrid + 4) = 0;
    *(unsigned char *)((char *)pTileGrid + 0x24) = 0;
    *(int *)((char *)pTileGrid + 0x4c) = 0x209;
    *(int *)((char *)pTileGrid + 0x50) = 0;
    ClearLevelTileGridCells((int)(unsigned long)pTileGrid + 8);
}

// FUNCTION: LEMBALL 0x00452fe0
void *GameNetworkLevelChunkDeltaStream::ConstructNetworkLevelChunkDeltaStream(int nOwner) {
    int nDispatcherPayload;

    *(void **)this = g_GAME_EffStreamConstructionVtable;
    *(int *)((char *)this + 4) = 3;
    ((GameEffStream *)this)->ResetStateFields();
    *(void **)this = g_GAME_NetworkLevelChunkDeltaStreamVtable;
    *(int *)((char *)this + 0x2c) = nOwner;
    *(int *)((char *)this + 0x30) = nOwner + 0x1d0;
    nDispatcherPayload = *(int *)((unsigned long)nOwner + 0x34);
    *(int *)((char *)this + 0x24) = 0;
    *(int *)((char *)this + 0x18) += nDispatcherPayload + 8;
    return this;
}

// FUNCTION: LEMBALL 0x00453070
void LEMBALL_FASTCALL ReadNetworkLevelChunkDeltaStream(void *pObject) {
    unsigned char *pbObject;
    unsigned short nTag;
    int nSourceBuffer;

    pbObject = (unsigned char *)pObject;
    nTag = (unsigned short)(((unsigned int) *(unsigned char *)((unsigned long) *(int *)(pbObject + 0x20)) << 8) |
                            *(unsigned char *)((unsigned long) *(int *)(pbObject + 0x20) + 1));
    *(int *)(pbObject + 0x20) += 2;
    while (nTag != 0x2f) {
        if (nTag == 0x2c) {
            unsigned char nStreamIndex;
            int pOwner;
            int pStream;
            nSourceBuffer = *(int *)(pbObject + 0x20);
            nStreamIndex = *(unsigned char *)((unsigned long)nSourceBuffer);
            *(int *)(pbObject + 0x20) = nSourceBuffer + 1;
            pOwner = *(int *)(pbObject + 0x30);
            pStream = *(int *)(pOwner + 0x10 + (unsigned int)nStreamIndex * 4);
            pStream += 0x138;
            if (((GameEffStream *)pStream)->LoadEffStreamFromMemory(
                    *(int *)(pbObject + 0x20)) != 0) {
                *(int *)(pbObject + 0x20) = *(int *)(pStream + 0x20);
            }
        } else if (nTag == 0x2d) {
            unsigned int nTimeMs;
            nSourceBuffer = *(int *)(pbObject + 0x20);
            nTimeMs = ((unsigned int) *(unsigned char *)((unsigned long)nSourceBuffer) << 24) |
                      ((unsigned int) *(unsigned char *)((unsigned long)nSourceBuffer + 1) << 16) |
                      ((unsigned int) *(unsigned char *)((unsigned long)nSourceBuffer + 2) << 8) |
                      *(unsigned char *)((unsigned long)nSourceBuffer + 3);
            *(int *)(pbObject + 0x20) = nSourceBuffer + 4;
            UpdateNetworkFrameClockFromNetworkTimeThunk(nTimeMs);
        }

        nSourceBuffer = *(int *)(pbObject + 0x20);
        nTag = (unsigned short)(((unsigned int) *(unsigned char *)((unsigned long)nSourceBuffer) << 8) |
                                *(unsigned char *)((unsigned long)nSourceBuffer + 1));
        *(int *)(pbObject + 0x20) = nSourceBuffer + 2;
    }
}

// FUNCTION: LEMBALL 0x0045fcc0
void ClearPrimaryLockedRecordTablePayloadFlags(void *pPayload) {
    ((GameLockedRecordSlotTableView *)*(int **)((char *)pPayload + 0x48))
        ->ClearLockedRecordSlotPayloadFlags();
}

// FUNCTION: LEMBALL 0x0045fcd0
void ClearSecondaryLockedRecordTablePayloadFlags(void *pPayload) {
    ((GameLockedRecordSlotTableView *)*(int **)((char *)pPayload + 0x50))
        ->ClearLockedRecordSlotPayloadFlags();
}

// FUNCTION: LEMBALL 0x00461310
void GameLockedRecordSlotTableView::ClearLockedRecordSlotPayloadFlags(void) {
    int i;
    int nSlots;
    int nOffset;
    int *pRecordSlot;

    nOffset = 0;
    nSlots = 0;
    if (0 < m_cRecords) {
        do {
            nOffset += 4;
            ++nSlots;
            pRecordSlot = *(int **)((unsigned long)m_apRecords08 - 4 + nOffset);
            *(int *)((char *)pRecordSlot + 0x24) = 0;
        } while (nSlots < m_cRecords);
    }
}

static void InitializeLevelTileGridCellRenderFlags(LevelLevelTileCell *pCell) {
    switch (pCell->m_nType00) {
    case 0x202:
    case 0x204:
    case 0x210:
        pCell->m_nFlags06 = 3;
        break;
    case 0x205:
        pCell->m_nFlags06 = 2;
        break;
    case 0x20a:
        pCell->m_nFlags06 = 1;
        break;
    case 0x215:
    case 0x216:
    case 0x217:
        pCell->m_nFlags06 = 4;
        break;
    default:
        pCell->m_nFlags06 = 0;
        break;
    }
}

static void ResizeLevelTileGrid(void *pTileGrid, int cColumns, int cRows) {
    char *pGrid;
    int cCells;

    pGrid = (char *)pTileGrid;
    if (*(int *)(pGrid + 0x1c) == cColumns &&
        *(int *)(pGrid + 0x20) == cRows) {
        return;
    }
    *(int *)(pGrid + 0x1c) = cColumns;
    *(int *)(pGrid + 0x20) = cRows;
    if (*(void **)(pGrid + 0x0c) != 0) {
        FreeVSMemBlock(*(void **)(pGrid + 0x0c));
    }
    *(int *)(pGrid + 0x10) = cColumns;
    *(int *)(pGrid + 0x14) = cRows;
    cCells = cColumns * cRows;
    *(void **)(pGrid + 0x0c) = AllocateVSMemBlock((unsigned int)cCells * 0x0c);
    if (*(void **)(pGrid + 0x0c) != 0) {
        ClearLevelTileGridCells((int)(unsigned long)pGrid + 8);
    }
    if (*(void **)(pGrid + 0x48) != 0) {
        FreeVSMemBlock(*(void **)(pGrid + 0x48));
    }
    *(void **)(pGrid + 0x48) = AllocateVSMemBlock((unsigned int)cCells);
}

static void ComputeLevelTileGridRepeatedColumnCounts(void *pTileGrid) {
    char *pGrid;
    int x;
    int y;

    pGrid = (char *)pTileGrid;
    for (y = 0; y < *(int *)(pGrid + 0x20); ++y) {
        for (x = 0; x < *(int *)(pGrid + 0x1c); ++x) {
            LevelLevelTileCell *pCell =
                (LevelLevelTileCell *)((char *)*(void **)(pGrid + 0x0c) +
                    (y * *(int *)(pGrid + 0x10) + x) * 0x0c);
            int nHeight = (unsigned short)pCell->m_nReserved08;
            if (x < *(int *)(pGrid + 0x1c) - 1 &&
                y < *(int *)(pGrid + 0x20) - 1) {
                LevelLevelTileCell *pRight = pCell + 1;
                LevelLevelTileCell *pBelow =
                    pCell + *(int *)(pGrid + 0x10);
                int nNeighborHeight =
                    (unsigned short)pRight->m_nReserved08;
                if ((unsigned short)pBelow->m_nReserved08 < nNeighborHeight) {
                    nNeighborHeight =
                        (unsigned short)pBelow->m_nReserved08;
                }
                nHeight -= nNeighborHeight;
            }
            pCell->m_nReserved0A =
                (short)((nHeight + 0x0f + ((nHeight + 0x0f) >> 31 & 0x0f)) >> 4);
        }
    }
}

void LoadLevelTileGridChunk(void *pTileGrid, const unsigned short *pWords) {
    int cColumns;
    int cRows;
    int x;
    int y;
    LevelLevelTileCell *pCells;

    cColumns = pWords[0];
    cRows = pWords[1];
    pWords += 2;
    ClearLevelTileGridCells((int)(unsigned long)pTileGrid + 8);
    ResizeLevelTileGrid(pTileGrid, cColumns, cRows);
    pCells = *(LevelLevelTileCell **)((char *)pTileGrid + 0x0c);
    for (y = 0; y < cRows; ++y) {
        for (x = 0; x < cColumns; ++x) {
            LevelLevelTileCell *pCell = &pCells[y * cColumns + x];
            pCell->m_nType00 = pWords[0];
            pCell->m_nReserved04 = (short)pWords[1];
            InitializeLevelTileGridCellRenderFlags(pCell);
            pCell->m_nReserved08 = (short)pWords[2];
            pWords += 3;
        }
    }
    ComputeLevelTileGridRepeatedColumnCounts(pTileGrid);
}

void LoadLevelAiChunk(void *pLevelMode, const unsigned short *pWords,
                      unsigned int cbPayload) {
    char *pMode;
    void *pTileGrid;

    pMode = (char *)pLevelMode;
    if (cbPayload < 5) {
        *(unsigned short *)(pMode + 0x54) = 0;
    } else {
        *(unsigned short *)(pMode + 0x54) = *pWords++;
    }
    pTileGrid = *(void **)(pMode + 0x110);
    *(int *)(pMode + 0x60) = pWords[0];
    *(int *)(pMode + 0xec) = pWords[1];
    *(int *)pTileGrid = pWords[0];
    ClearLevelTileGridCells((int)(unsigned long)pTileGrid + 8);
    if (*(unsigned short *)(pMode + 0x54) < 4) {
        *(int *)(pMode + 0xf8) = 4;
        *(int *)(pMode + 0xfc) = 1;
    } else {
        *(int *)(pMode + 0xf8) = pWords[2];
        *(int *)(pMode + 0xfc) = pWords[3];
    }
    *(int *)(pMode + 0xe4) = *(int *)(pMode + 0xec);
}

void LoadLevelFlagChunk(void *pLevelMode, const unsigned short *pWords) {
    char *pMode;

    pMode = (char *)pLevelMode;
    if (*(int *)(pMode + 0x64) == 1 && *(int *)(pMode + 0x7c) != 1) {
        *(int *)(pMode + 0x100) = pWords[0];
        *(int *)(pMode + 0xfc) = pWords[1];
    } else {
        *(int *)(pMode + 0xfc) = pWords[0];
        *(int *)(pMode + 0x100) = pWords[1];
    }
}

void SetLevelTileGridDefaultVariantIds(void *pTileGrid,
                                       const unsigned short *pWords) {
    *(int *)((char *)pTileGrid + 0x4c) = pWords[0];
    *(int *)((char *)pTileGrid + 0x50) = pWords[1];
}

void SetLevelTileGridName(void *pTileGrid, const char *pszName) {
    int i;
    char ch;

    i = 0;
    do {
        ch = *pszName++;
        *((char *)pTileGrid + 0x24 + i) = ch;
        if (ch == '\0') {
            break;
        }
        ++i;
    } while (i < 0x20);
    *((char *)pTileGrid + 0x44) = '\0';
}

static int HasAnimChunkRecordAtTile(void *pTimerBlock,
                                    short nX, short nY) {
    int i;

    for (i = 0; i < *(int *)((char *)pTimerBlock + 4); ++i) {
        short *pRecord = (short *)((char *)pTimerBlock + 8 + i * 0x18);
        if (pRecord[0] == nX && pRecord[1] == nY) {
            return 1;
        }
    }
    return 0;
}

void LoadAnimChunkRecords(void *pTimerBlock, const unsigned short *pWords) {
    unsigned int cRecords;

    cRecords = *pWords++;
    while (cRecords-- != 0) {
        short nX = (short)pWords[0];
        short nY = (short)pWords[1];
        short nReserved = (short)pWords[2];
        unsigned short nFirstFrame = pWords[3];
        unsigned short nLastFrame = pWords[4];
        int nIndex = *(int *)((char *)pTimerBlock + 4);

        if (!HasAnimChunkRecordAtTile(pTimerBlock, nX, nY) && nIndex < 200) {
            char *pRecord = (char *)pTimerBlock + 8 + nIndex * 0x18;
            int nColumn = ((int)nX + ((int)nX >> 31 & 0x0f)) >> 4;
            int nRow = ((int)nY + ((int)nY >> 31 & 0x0f)) >> 4;
            *(short *)(pRecord + 0) = nX;
            *(short *)(pRecord + 2) = nY;
            *(short *)(pRecord + 4) = nReserved;
            *(void **)(pRecord + 8) =
                (char *)(unsigned long)*(int *)((char *)g_pAnimChunkTileGrid + 0x0c) +
                (nRow * *(int *)((char *)g_pAnimChunkTileGrid + 0x10) +
                 nColumn) * 0x0c;
            *(unsigned short *)(pRecord + 0x0c) = nFirstFrame;
            *(unsigned short *)(pRecord + 0x0e) = nFirstFrame;
            *(unsigned short *)(pRecord + 0x10) = nLastFrame;
            *(short *)(pRecord + 0x12) =
                (short)(nLastFrame < nFirstFrame ? -1 : 1);
            *(int *)(pRecord + 0x14) = 1;
            ++*(int *)((char *)pTimerBlock + 4);
        }
        pWords += 5;
    }
}

void LEMBALL_FASTCALL AdvanceAnimChunkFrames(void *pTimerBlock) {
    int i;
    char *pTimer;

    pTimer = (char *)pTimerBlock;
    if (*(int *)pTimer > g_nLevelFrameClockTick) {
        return;
    }
    *(int *)pTimer = g_nLevelFrameClockTick + 2;
    for (i = 0; i < *(int *)(pTimer + 4); ++i) {
        char *pRecord = pTimer + 8 + i * 0x18;
        short nFrame;
        if (*(int *)(pRecord + 0x14) == 0) {
            continue;
        }
        nFrame = *(short *)(pRecord + 0x0c) + *(short *)(pRecord + 0x12);
        if (nFrame < *(short *)(pRecord + 0x0e) ||
            *(short *)(pRecord + 0x10) < nFrame) {
            nFrame = *(short *)(pRecord + 0x0e);
        }
        *(short *)(pRecord + 0x0c) = nFrame;
        if (*(void **)(pRecord + 8) != 0) {
            *(short *)((char *)*(void **)(pRecord + 8) + 4) = nFrame;
        }
    }
}

void LoadNodeChunkRecords(void *pManager, const unsigned short *pWords,
                          unsigned int cbPayload, int fReuseState) {
    char *pManagerBytes;
    char *pRecord;
    const unsigned short *pEnd;
    unsigned int cRecords;
    unsigned int i;

    pManagerBytes = (char *)pManager;
    cRecords = *pWords++;
    *(int *)(pManagerBytes + 4) = 0;
    if (cRecords == 0) {
        *(void **)pManagerBytes = 0;
        return;
    }
    if (*(void **)pManagerBytes == 0) {
        int cCapacity = *(int *)(pManagerBytes + 8);
        char *pAllocation =
            (char *)AllocateVSMemBlock((unsigned int)cCapacity * 0x14 + 4);
        if (pAllocation == 0) {
            return;
        }
        *(int *)pAllocation = cCapacity;
        *(void **)pManagerBytes = pAllocation + 4;
        for (i = 0; i < (unsigned int)cCapacity; ++i) {
            pRecord = pAllocation + 4 + i * 0x14;
            *(int *)(pRecord + 0) = 0xaa55aa55;
            *(int *)(pRecord + 4) = 0xaa55aa55;
            *(int *)(pRecord + 8) = 0;
            *(void **)(pRecord + 0x0c) = 0;
            *(int *)(pRecord + 0x10) = 0;
        }
    }
    for (i = 0; i < (unsigned int)*(int *)(pManagerBytes + 8); ++i) {
        *(int *)((char *)*(void **)pManagerBytes + i * 0x14 + 0x10) = 0;
    }
    *(int *)(pManagerBytes + 4) = cRecords;
    if (fReuseState != 0) {
        return;
    }

    pEnd = (const unsigned short *)((const char *)pWords + cbPayload - 2);
    pRecord = (char *)*(void **)pManagerBytes;
    for (i = 0; i < cRecords && pWords + 3 <= pEnd; ++i) {
        unsigned int cLinks = pWords[2];
        *(int *)(pRecord + 0) = (unsigned int)pWords[0] << 12;
        *(int *)(pRecord + 4) = (unsigned int)pWords[1] << 12;
        *(int *)(pRecord + 8) = cLinks;
        *(int *)(pRecord + 0x10) = 0;
        pWords += 3;
        if (cLinks != 0 && *(void **)(pRecord + 0x0c) == 0) {
            *(void **)(pRecord + 0x0c) =
                AllocateVSMemBlock(cLinks * 8);
        }
        while (cLinks-- != 0 && pWords + 2 <= pEnd) {
            int nLink = *(int *)(pRecord + 0x10);
            int *pLinks = *(int **)(pRecord + 0x0c);
            pLinks[nLink * 2] = pWords[0];
            pLinks[nLink * 2 + 1] = pWords[1];
            ++*(int *)(pRecord + 0x10);
            pWords += 2;
        }
        pRecord += 0x14;
    }
}
