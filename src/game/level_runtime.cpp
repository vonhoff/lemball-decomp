#include "../game/game_app.h"
#include "../engine/memory_arena.h"
#include "../network/stream.h"
#include "../network/safe_vtable.h"
#include "eff_streams.h"
#include "level_runtime.h"

#include <stdlib.h>

extern "C" DWORD WINAPI timeGetTime(void);

extern int g_nLevelFrameClockTick;
int g_nNetworkFrameClockLatestTimeMs = 0;
int g_nLevelFrameClockLastSampleMs = 0;
int g_nLevelFrameClockTimeMs = 0;
int g_nLevelFrameClockDemoTimeMs = 0;
int g_nLevelFrameClockTimeBaseMs = 0;
int g_nLevelFrameClockTickBase = 0;
// GLOBAL: LEMBALL 0x0049e1bc
LEVEL_BallChunkManager *g_pBallChunkManager = 0;
// GLOBAL: LEMBALL 0x004a63fc
char *g_pszActiveLevelFilePath = 0;
// GLOBAL: LEMBALL 0x004a6400
void *g_pLoadedLevelFileBuffer = 0;

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
int g_GAME_ManagedEntityRegistryTable[1000] = {0};
// GLOBAL: LEMBALL 0x004a74b0
void *g_pActiveManagedEntityOwner = 0;
// GLOBAL: LEMBALL 0x004a74bc
unsigned short g_GAME_ManagedEntityRegistryCount = 0;
static unsigned char *g_GAME_ManagedEntitySlotBitMasks;
static unsigned char *g_GAME_ManagedEntitySlotClaimBitset;

struct LEVEL_LevelTileCell {
    int m_nType00;
    short m_nReserved04;
    unsigned short m_nFlags06;
    short m_nReserved08;
    short m_nReserved0A;
};

struct LEVEL_LevelTileGridStaticView {
    char m_abReserved00[0x0c];
    LEVEL_LevelTileCell *m_pCells0C;
    int m_cColumns10;
    int m_cRows14;

    unsigned short GetTileFlags(int x, int y) const {
        if (x < 0 || y < 0 || x >= m_cColumns10 || y >= m_cRows14) {
            return 3;
        }
        return m_pCells0C[y * m_cColumns10 + x].m_nFlags06;
    }
};

// FUNCTION: LEMBALL 0x00409910
static int __cdecl CompareLevelTileGridStaticRenderEntrySortKeys(
    const void *pLeft,
    const void *pRight)
{
    const LEVEL_TileGridStaticRenderEntry *pLeftEntry;
    const LEVEL_TileGridStaticRenderEntry *pRightEntry;

    pLeftEntry = (const LEVEL_TileGridStaticRenderEntry *)pLeft;
    pRightEntry = (const LEVEL_TileGridStaticRenderEntry *)pRight;
    return (unsigned int)pLeftEntry->m_nSortKey04 -
           (unsigned int)pRightEntry->m_nSortKey04;
}

// FUNCTION: LEMBALL 0x00409930
void LEVEL_SpecialTileGridStaticRenderList::BuildSpecialLevelTileGridStaticRenderEntries(
    LEVEL_LevelTileGridStaticView *pTileGrid)
{
    int cEntries;
    int cColumns;
    int cRows;
    int y;
    int x;
    int iEntry;
    int nSortOffset;
    LEVEL_LevelTileCell *pCell;

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
        m_pEntries00 = (LEVEL_TileGridStaticRenderEntry *)
            AllocateVSMemBlock(cEntries * sizeof(LEVEL_TileGridStaticRenderEntry));
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
          sizeof(LEVEL_TileGridStaticRenderEntry),
          CompareLevelTileGridStaticRenderEntrySortKeys);
}

// FUNCTION: LEMBALL 0x004012FD
void LEVEL_SpecialTileGridStaticRenderList::BuildSpecialLevelTileGridStaticRenderEntriesThunk(
    LEVEL_LevelTileGridStaticView *pTileGrid)
{
    BuildSpecialLevelTileGridStaticRenderEntries(pTileGrid);
}
static int *g_GAME_SelectedNetworkLobbyPeerId;

void *g_pLiftTileGrid = 0;
void *g_pAnimChunkTileGrid = 0;

// FUNCTION: LEMBALL 0x0041A3C0
LEVEL_ProjectileGeometryPair *LEVEL_ProjectileGeometryPair::InitializeSentinel(void) {
    m_nX = 0xaa55aa55;
    m_nY = 0xaa55aa55;
    return this;
}

// FUNCTION: LEMBALL 0x00402DE2
LEVEL_ProjectileGeometryPair *LEVEL_ProjectileGeometryPair::InitializeSentinelThunk(void) {
    return InitializeSentinel();
}

// FUNCTION: LEMBALL 0x0041A140
LEVEL_ProjectileGeometryPairTable *LEVEL_ProjectileGeometryPairTable::InitializeProjectileGeometryPairTable(void) {
    int i;
    int j;
    LEVEL_ProjectileGeometryPair *pPair;

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

// FUNCTION: LEMBALL 0x00402752
LEVEL_ProjectileGeometryPairTable *LEVEL_ProjectileGeometryPairTable::InitializeProjectileGeometryPairTableThunk(void) {
    return InitializeProjectileGeometryPairTable();
}

// FUNCTION: LEMBALL 0x0041A1B0
void LEVEL_ProjectileGeometryPairTable::ClearProjectileGeometryField100(void) {
    m_nReserved100 = 0;
}

// FUNCTION: LEMBALL 0x00402E78
void LEVEL_ProjectileGeometryPairTable::ClearProjectileGeometryField100Thunk(void) {
    ClearProjectileGeometryField100();
}

// FUNCTION: LEMBALL 0x00421300
void LEVEL_NodeChunkRecord::ResetNodeChunkRecordLinkCount(void) {
    m_cLinks10 = 0;
}

// FUNCTION: LEMBALL 0x00401230
void LEVEL_NodeChunkRecord::ResetNodeChunkRecordLinkCountThunk(void) {
    ResetNodeChunkRecordLinkCount();
}

// FUNCTION: LEMBALL 0x00421180
LEVEL_NodeChunkManager *LEVEL_NodeChunkManager::ConstructNodeChunkManager(int cCapacity) {
    m_pRecords00 = 0;
    m_cCapacity08 = cCapacity;
    return this;
}

// FUNCTION: LEMBALL 0x004015E1
LEVEL_NodeChunkManager *LEVEL_NodeChunkManager::ConstructNodeChunkManagerThunk(int cCapacity) {
    return ConstructNodeChunkManager(cCapacity);
}

// FUNCTION: LEMBALL 0x004211A0
void LEVEL_NodeChunkManager::ResetNodeChunkRecords(void) {
    int i;

    if (m_pRecords00 != 0 && 0 < m_cCapacity08) {
        for (i = 0; i < m_cCapacity08; ++i) {
            m_pRecords00[i].ResetNodeChunkRecordLinkCountThunk();
        }
    }
}

// FUNCTION: LEMBALL 0x00402748
void LEVEL_NodeChunkManager::ResetNodeChunkRecordsThunk(void) {
    ResetNodeChunkRecords();
}

// FUNCTION: LEMBALL 0x0040B8E0
LEVEL_SlnkChunkManager *LEVEL_SlnkChunkManager::ConstructSlnkChunkManager(void *pLevelMode, int cCapacity) {
    m_pLevelMode00 = pLevelMode;
    m_cCapacity08 = cCapacity;
    m_pObjects04 = 0;
    return this;
}

// FUNCTION: LEMBALL 0x00403454
LEVEL_SlnkChunkManager *LEVEL_SlnkChunkManager::ConstructSlnkChunkManagerThunk(void *pLevelMode, int cCapacity) {
    return ConstructSlnkChunkManager(pLevelMode, cCapacity);
}

// FUNCTION: LEMBALL 0x00421EA0
LEVEL_BallChunkManager *LEVEL_BallChunkManager::ConstructBallChunkManager(void *pLevelMode, int cCapacity) {
    m_pLevelMode00 = pLevelMode;
    g_pBallChunkManager = this;
    m_pObjects04 = 0;
    m_cCapacity0C = cCapacity;
    return this;
}

// FUNCTION: LEMBALL 0x004037D8
LEVEL_BallChunkManager *LEVEL_BallChunkManager::ConstructBallChunkManagerThunk(void *pLevelMode, int cCapacity) {
    return ConstructBallChunkManager(pLevelMode, cCapacity);
}

// FUNCTION: LEMBALL 0x0040CF10
void LEVEL_Field194TimerBlock::ResetLevelField194TimerBlock(void) {
    int nNextTick;

    nNextTick = g_nLevelFrameClockTick;
    m_nReserved04 = 0;
    m_fActive12C8 = 1;
    m_nNextTick00 = nNextTick + 2;
}

// FUNCTION: LEMBALL 0x004034EF
void LEVEL_Field194TimerBlock::ResetLevelField194TimerBlockThunk(void) {
    ResetLevelField194TimerBlock();
}

// FUNCTION: LEMBALL 0x00412F00
void *LEMBALL_FASTCALL GetLevelGameModeOwner(void *pLevelMode) {
    return *(void **)((char *)pLevelMode + 0x154);
}

// FUNCTION: LEMBALL 0x0040117C
void *LEMBALL_FASTCALL GetLevelGameModeOwnerThunk(void *pLevelMode) {
    return GetLevelGameModeOwner(pLevelMode);
}

// FUNCTION: LEMBALL 0x00408210
LEVEL_ChunkLoaderContext *LEVEL_ChunkLoaderContext::ConstructLevelChunkLoaderContext(void *pLevelMode) {
    m_pLevelMode00 = pLevelMode;
    m_nFallbackLevel04 = 0;
    g_pLoadedLevelFileBuffer = 0;
    g_pszActiveLevelFilePath =
        (char *)GetLevelGameModeOwnerThunk(m_pLevelMode00);
    return this;
}

// FUNCTION: LEMBALL 0x004024D2
LEVEL_ChunkLoaderContext *LEVEL_ChunkLoaderContext::ConstructLevelChunkLoaderContextThunk(void *pLevelMode) {
    return ConstructLevelChunkLoaderContext(pLevelMode);
}

#define LEVEL_RESERVED_VIRTUAL(n) virtual void Reserved##n(void) = 0

struct LEVEL_VirtualSlots00Through40 {
    LEVEL_RESERVED_VIRTUAL(00);
    LEVEL_RESERVED_VIRTUAL(01);
    LEVEL_RESERVED_VIRTUAL(02);
    LEVEL_RESERVED_VIRTUAL(03);
    LEVEL_RESERVED_VIRTUAL(04);
    LEVEL_RESERVED_VIRTUAL(05);
    LEVEL_RESERVED_VIRTUAL(06);
    LEVEL_RESERVED_VIRTUAL(07);
    LEVEL_RESERVED_VIRTUAL(08);
    LEVEL_RESERVED_VIRTUAL(09);
    LEVEL_RESERVED_VIRTUAL(0A);
    LEVEL_RESERVED_VIRTUAL(0B);
    LEVEL_RESERVED_VIRTUAL(0C);
    LEVEL_RESERVED_VIRTUAL(0D);
    LEVEL_RESERVED_VIRTUAL(0E);
    LEVEL_RESERVED_VIRTUAL(0F);
    LEVEL_RESERVED_VIRTUAL(10);
    LEVEL_RESERVED_VIRTUAL(11);
    LEVEL_RESERVED_VIRTUAL(12);
    LEVEL_RESERVED_VIRTUAL(13);
    LEVEL_RESERVED_VIRTUAL(14);
    LEVEL_RESERVED_VIRTUAL(15);
    LEVEL_RESERVED_VIRTUAL(16);
    LEVEL_RESERVED_VIRTUAL(17);
    LEVEL_RESERVED_VIRTUAL(18);
    LEVEL_RESERVED_VIRTUAL(19);
    LEVEL_RESERVED_VIRTUAL(1A);
    LEVEL_RESERVED_VIRTUAL(1B);
    LEVEL_RESERVED_VIRTUAL(1C);
    LEVEL_RESERVED_VIRTUAL(1D);
    LEVEL_RESERVED_VIRTUAL(1E);
    LEVEL_RESERVED_VIRTUAL(1F);
    LEVEL_RESERVED_VIRTUAL(20);
    LEVEL_RESERVED_VIRTUAL(21);
    LEVEL_RESERVED_VIRTUAL(22);
    LEVEL_RESERVED_VIRTUAL(23);
    LEVEL_RESERVED_VIRTUAL(24);
    LEVEL_RESERVED_VIRTUAL(25);
    LEVEL_RESERVED_VIRTUAL(26);
    LEVEL_RESERVED_VIRTUAL(27);
    LEVEL_RESERVED_VIRTUAL(28);
    LEVEL_RESERVED_VIRTUAL(29);
    LEVEL_RESERVED_VIRTUAL(2A);
    LEVEL_RESERVED_VIRTUAL(2B);
    LEVEL_RESERVED_VIRTUAL(2C);
    LEVEL_RESERVED_VIRTUAL(2D);
    LEVEL_RESERVED_VIRTUAL(2E);
    LEVEL_RESERVED_VIRTUAL(2F);
    LEVEL_RESERVED_VIRTUAL(30);
    LEVEL_RESERVED_VIRTUAL(31);
    LEVEL_RESERVED_VIRTUAL(32);
    LEVEL_RESERVED_VIRTUAL(33);
    LEVEL_RESERVED_VIRTUAL(34);
    LEVEL_RESERVED_VIRTUAL(35);
    LEVEL_RESERVED_VIRTUAL(36);
    LEVEL_RESERVED_VIRTUAL(37);
    LEVEL_RESERVED_VIRTUAL(38);
    LEVEL_RESERVED_VIRTUAL(39);
    LEVEL_RESERVED_VIRTUAL(3A);
    LEVEL_RESERVED_VIRTUAL(3B);
    LEVEL_RESERVED_VIRTUAL(3C);
    LEVEL_RESERVED_VIRTUAL(3D);
    LEVEL_RESERVED_VIRTUAL(3E);
    LEVEL_RESERVED_VIRTUAL(3F);
    LEVEL_RESERVED_VIRTUAL(40);
};

struct LEVEL_NestedChildInterface : public LEVEL_VirtualSlots00Through40 {
    virtual void Activate(void) = 0;
};

struct LEVEL_Type35ChunkObject : public LEVEL_VirtualSlots00Through40 {
    virtual void Reset(void) = 0;
    char m_abReserved04[0x14c];
};

struct LEVEL_NestedOwnerInterface : public LEVEL_VirtualSlots00Through40 {
    LEVEL_RESERVED_VIRTUAL(41);
    virtual int GetNestedChildCount(void) = 0;
    LEVEL_RESERVED_VIRTUAL(43);
    LEVEL_RESERVED_VIRTUAL(44);
    LEVEL_RESERVED_VIRTUAL(45);
    LEVEL_RESERVED_VIRTUAL(46);
    LEVEL_RESERVED_VIRTUAL(47);
    virtual LEVEL_NestedChildInterface *GetNestedChild(int nIndex) = 0;
};

#undef LEVEL_RESERVED_VIRTUAL

// FUNCTION: LEMBALL 0x0040B900
void LEVEL_SlnkChunkManager::ResetType35ChunkObjects(void) {
    int i;

    if (m_pObjects04 != 0) {
        for (i = 0; i < m_cCapacity08; ++i) {
            ((LEVEL_Type35ChunkObject *)m_pObjects04)[i].Reset();
        }
    }
}

// FUNCTION: LEMBALL 0x00402A86
void LEVEL_SlnkChunkManager::ResetType35ChunkObjectsThunk(void) {
    ResetType35ChunkObjects();
}

// FUNCTION: LEMBALL 0x0041F0E0
void LEVEL_ShpgChunkManager::ActivateNestedChildrenFromOwnerTableA4(void) {
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
                ((LEVEL_NestedOwnerInterface *)*ppObject)
                    ->GetNestedChildCount();
            if (0 < cChildren) {
                do {
                    ((LEVEL_NestedOwnerInterface *)*ppObject)
                        ->GetNestedChild(j++)
                        ->Activate();
                } while (j < cChildren);
            }
            ++ppObject;
            ++i;
        } while (i < m_cObjectsA4);
    }
}

// FUNCTION: LEMBALL 0x0040312F
void LEVEL_ShpgChunkManager::ActivateNestedChildrenFromOwnerTableA4Thunk(void) {
    ActivateNestedChildrenFromOwnerTableA4();
}

// FUNCTION: LEMBALL 0x00420BB0
void LEVEL_EnmyChunkManager::ActivateNestedChildrenFromOwnerTableVariant(void) {
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
                ((LEVEL_NestedOwnerInterface *)*ppObject)
                    ->GetNestedChildCount();
            if (0 < cChildren) {
                do {
                    ((LEVEL_NestedOwnerInterface *)*ppObject)
                        ->GetNestedChild(j++)
                        ->Activate();
                } while (j < cChildren);
            }
            ++ppObject;
            ++i;
        } while (i < m_cObjectsA4);
    }
}

// FUNCTION: LEMBALL 0x00401DFC
void LEVEL_EnmyChunkManager::ActivateNestedChildrenFromOwnerTableVariantThunk(void) {
    ActivateNestedChildrenFromOwnerTableVariant();
}

extern int ReturnTrueVtableCallbackThunk(void);
extern int ReturnTrueVtableCallbackSecondaryThunk(void);
extern void NoopVtableCallbackThunk(void);
struct GAME_NetworkLevelChunkDeltaStream {
    void *ConstructNetworkLevelChunkDeltaStream(int nOwner);
    void *ConstructNetworkLevelChunkDeltaStreamEntry(int nOwner);
};
void ReadNetworkLevelChunkDeltaStream(void *pObject);
void LoadLevelGameStateStreamPayload(void *pObject);

static void *g_GAME_NetworkLevelChunkDeltaStreamVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)ReadNetworkLevelChunkDeltaStream,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
};
static void *g_GAME_LevelGameStateStreamVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)LoadLevelGameStateStreamPayload,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
};
static void *g_pReturnTrueVtableCallbackThunk =
    g_GAME_NetworkLevelChunkDeltaStreamVtable;
static void *g_pLevelGameStateStreamVtable =
    g_GAME_LevelGameStateStreamVtable;

struct GAME_LevelChunkStreamDispatcher {
    int m_nRenderQueueNode00;
    int m_nRenderQueueTag04;
    int m_nRenderQueueReserved08;
    int m_nRenderQueueReserved0C;
    void **m_ppRegisteredStreams10;
    int m_cChunkTypes14;
    int m_cRegisteredStreams18;
    int *m_pChunkTypeToIndex1C;
};

struct GAME_LevelTileReachabilityHelper {
    int m_pTileGrid00;
    int m_ppRows04;
    int m_nReserved08;
    char m_abReserved0C[0x1000];
    int m_cColumns100C;
    int m_cRows1010;
    int m_nReserved1014;
};

struct GAME_LevelTileGrid {
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

// FUNCTION: LEMBALL 0x00410BE0
void LoadLevelGameStateStreamPayload(void *pObject) {
    NETWORK_EffStreamBase *pStream;
    unsigned int nValue;

    pStream = (NETWORK_EffStreamBase *)pObject;
    nValue = pStream->ReadEffStreamU32BEValue();
    *(unsigned int *)((char *)pObject + 0x2c) = nValue;
    nValue = pStream->ReadEffStreamU32BEValue();
    *(unsigned int *)((char *)pObject + 0x30) = nValue;
    pStream->ReadEffStreamU32BE((unsigned char *)((char *)pObject + 0x34));
    pStream->ReadEffStreamU32BE((unsigned char *)((char *)pObject + 0x38));
}

// FUNCTION: LEMBALL 0x00408190
void UpdateNetworkFrameClockFromNetworkTime(unsigned int nTimeMs) {
    if ((unsigned int)g_nNetworkFrameClockLatestTimeMs < nTimeMs) {
        g_nLevelFrameClockTimeMs = (int)nTimeMs;
        g_nLevelFrameClockTick = (int)(nTimeMs / 0x32);
        g_nNetworkFrameClockLatestTimeMs = (int)nTimeMs;
    }
}

// FUNCTION: LEMBALL 0x00403107
void UpdateNetworkFrameClockFromNetworkTimeThunk(unsigned int nTimeMs) {
    UpdateNetworkFrameClockFromNetworkTime(nTimeMs);
}

// FUNCTION: LEMBALL 0x00453070
void ReadNetworkLevelChunkDeltaStream(void *pObject) {
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
            if (((GAME_EffStream *)pStream)->LoadEffStreamFromMemory(
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

// FUNCTION: LEMBALL 0x0040CF00
void *ReturnArgumentVtableCallback(void *pObject) {
    return pObject;
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

// FUNCTION: LEMBALL 0x00402423
void ResetLevelFrameClockEntryThunk(void) {
    ResetLevelFrameClockThunk();
}

void InitializeManagedEntitySlotTablesThunk(void *pLevelGameMode);

// FUNCTION: LEMBALL 0x00401E8D
void InitializeManagedEntitySlotTablesEntryThunk(void *pLevelGameMode) {
    InitializeManagedEntitySlotTablesThunk(pLevelGameMode);
}

// FUNCTION: LEMBALL 0x004165E0
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

struct GAME_LockedRecordSlotTableView {
    int m_cRecords;
    int m_nReserved04;
    int *m_apRecords08;

    void ClearLockedRecordSlotPayloadFlags(void);
};

// FUNCTION: LEMBALL 0x00461310
void GAME_LockedRecordSlotTableView::ClearLockedRecordSlotPayloadFlags(void) {
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

// FUNCTION: LEMBALL 0x0045FCC0
void ClearPrimaryLockedRecordTablePayloadFlags(void *pPayload) {
    ((GAME_LockedRecordSlotTableView *)*(int **)((char *)pPayload + 0x48))
        ->ClearLockedRecordSlotPayloadFlags();
}

// FUNCTION: LEMBALL 0x0045FCD0
void ClearSecondaryLockedRecordTablePayloadFlags(void *pPayload) {
    ((GAME_LockedRecordSlotTableView *)*(int **)((char *)pPayload + 0x50))
        ->ClearLockedRecordSlotPayloadFlags();
}

// FUNCTION: LEMBALL 0x00452FE0
void *GAME_NetworkLevelChunkDeltaStream::ConstructNetworkLevelChunkDeltaStream(int nOwner) {
    int nDispatcherPayload;

    *(void **)this = g_GAME_EffStreamConstructionVtable;
    *(int *)((char *)this + 4) = 3;
    ((GAME_EffStream *)this)->ResetStateFields();
    *(void **)this = g_pReturnTrueVtableCallbackThunk;
    *(int *)((char *)this + 0x2c) = nOwner;
    *(int *)((char *)this + 0x30) = nOwner + 0x1d0;
    nDispatcherPayload = *(int *)((unsigned long)nOwner + 0x34);
    *(int *)((char *)this + 0x24) = 0;
    *(int *)((char *)this + 0x18) += nDispatcherPayload + 8;
    return this;
}

// FUNCTION: LEMBALL 0x004028F6
void *GAME_NetworkLevelChunkDeltaStream::ConstructNetworkLevelChunkDeltaStreamEntry(int nOwner) {
    return ConstructNetworkLevelChunkDeltaStream(nOwner);
}

// FUNCTION: LEMBALL 0x00410B80
void *ConstructLevelGameStateStreamThunk(void *pObject) {
    int *pStream;

    pStream = (int *)pObject;
    *(void **)pStream = g_GAME_EffStreamConstructionVtable;
    pStream[1] = 10;
    ((GAME_EffStream *)pStream)->ResetStateFields();
    *(void **)pStream = g_pLevelGameStateStreamVtable;
    pStream[6] += 0x10;
    pStream[9] = 1;
    return pObject;
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

// FUNCTION: LEMBALL 0x004303C0
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

// FUNCTION: LEMBALL 0x004303F0
void InitializeLevelTileGridThunk(void *pTileGrid) {
    *(int *)((char *)pTileGrid + 4) = 0;
    *(unsigned char *)((char *)pTileGrid + 0x24) = 0;
    *(int *)((char *)pTileGrid + 0x4c) = 0x209;
    *(int *)((char *)pTileGrid + 0x50) = 0;
    ClearLevelTileGridCells((int)(unsigned long)pTileGrid + 8);
}

struct LEVEL_TileReachabilityHelper {
    void *ConstructLevelTileReachabilityHelperThunk(void *pTileGrid);
};

// FUNCTION: LEMBALL 0x00423090
void *LEVEL_TileReachabilityHelper::ConstructLevelTileReachabilityHelperThunk(void *pTileGrid) {
    *(void **)this = pTileGrid;
    *(int *)((char *)this + 4) = 0;
    *(unsigned char *)((char *)this + 0x100c) = 0;
    *(int *)((char *)this + 8) = 0;
    *(int *)((char *)this + 0x1010) = 0;
    *(int *)((char *)this + 0x1014) = 0;
    return this;
}

// FUNCTION: LEMBALL 0x00423190
void LEMBALL_FASTCALL ResizeLevelTileReachabilityGrid(void *pReachabilityHelper) {
    GAME_LevelTileReachabilityHelper *pHelper;
    GAME_LevelTileGrid *pTileGrid;
    int i;
    int cRows;
    int cAllocatedRows;
    int **ppRows;

    pHelper = (GAME_LevelTileReachabilityHelper *)pReachabilityHelper;
    pTileGrid = (GAME_LevelTileGrid *)(unsigned long)pHelper->m_pTileGrid00;
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

// FUNCTION: LEMBALL 0x00402379
void LEMBALL_FASTCALL ResizeLevelTileReachabilityGridThunk(void *pReachabilityHelper) {
    ResizeLevelTileReachabilityGrid(pReachabilityHelper);
}

// FUNCTION: LEMBALL 0x00423110
void LEMBALL_FASTCALL FillReachabilityGridFromTileFlags(void *pReachabilityHelper) {
    GAME_LevelTileReachabilityHelper *pHelper;
    GAME_LevelTileGrid *pTileGrid;
    int nTileFlags;
    unsigned short nCellValue;
    int y;
    int x;
    unsigned short *pReachabilityRow;

    pHelper = (GAME_LevelTileReachabilityHelper *)pReachabilityHelper;
    pTileGrid = (GAME_LevelTileGrid *)(unsigned long)pHelper->m_pTileGrid00;
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

// FUNCTION: LEMBALL 0x00403364
void LEMBALL_FASTCALL FillReachabilityGridFromTileFlagsThunk(void *pReachabilityHelper) {
    FillReachabilityGridFromTileFlags(pReachabilityHelper);
}

// FUNCTION: LEMBALL 0x0040B120
void ResetRegisteredLevelChunkStreamsThunk(void *pDispatcher) {
    GAME_LevelChunkStreamDispatcher *pStreamDispatcher;
    int *pStream;
    int i;
    int nOffset;

    pStreamDispatcher = (GAME_LevelChunkStreamDispatcher *)pDispatcher;
    nOffset = 0;
    i = 0;
    if (0 < pStreamDispatcher->m_cRegisteredStreams18) {
        do {
            pStream = *(int **)((unsigned long)pStreamDispatcher->m_ppRegisteredStreams10 + nOffset);
            if (pStream != 0) {
                ((void (*)())*(void **)((unsigned long)*pStream + 0x18))();
            }
            nOffset += 4;
            ++i;
        } while (i < pStreamDispatcher->m_cRegisteredStreams18);
    }
}

// FUNCTION: LEMBALL 0x0040B150
void RegisterLevelChunkStreamThunk(void *pDispatcher, void *pStreamObject) {
    GAME_LevelChunkStreamDispatcher *pStreamDispatcher;
    int nVtable;
    int *pStream;

    pStreamDispatcher = (GAME_LevelChunkStreamDispatcher *)pDispatcher;
    pStream = (int *)pStreamObject;
    pStreamDispatcher->m_pChunkTypeToIndex1C[pStream[0xb]] = pStreamDispatcher->m_cRegisteredStreams18;
    nVtable = *pStream;
    pStreamDispatcher->m_ppRegisteredStreams10[pStreamDispatcher->m_cRegisteredStreams18] = pStreamObject;
    ++pStreamDispatcher->m_cRegisteredStreams18;
    ((void (*)())*(void **)((unsigned long)nVtable + 0x18))();
}

// FUNCTION: LEMBALL 0x00409080
int LEMBALL_FASTCALL GetSelectedLevelNumber(void *pLevelProgressState) {
    return *(int *)((char *)pLevelProgressState + 0x3c + *(int *)((char *)pLevelProgressState + 8) * 4);
}

// FUNCTION: LEMBALL 0x004011EA
int LEMBALL_FASTCALL GetSelectedLevelNumberThunk(void *pLevelProgressState) {
    return GetSelectedLevelNumber(pLevelProgressState);
}

// FUNCTION: LEMBALL 0x00416610
unsigned short LEMBALL_FASTCALL GetManagedEntitySlotId(
    int nManagedEntityObject)
{
    return *(unsigned short *)(nManagedEntityObject + 0x6c);
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

// FUNCTION: LEMBALL 0x004166A0
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

// FUNCTION: LEMBALL 0x004130A0
void SetLevelChunkTypeEnabledFlagThunk(void *pLevelMode, int nChunkType, int fEnabled) {
    if (nChunkType < 0x211 || 0x212 < nChunkType) {
        *(int *)(*(int *)((char *)pLevelMode + 0x50) + nChunkType * 4) = fEnabled;
    }
}

// FUNCTION: LEMBALL 0x004031BB
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

// FUNCTION: LEMBALL 0x00412FB0
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

// FUNCTION: LEMBALL 0x00412080
void AssignNetworkPlayerManagedEntitySlotIdsThunk(int nLevelMode) {
    unsigned int nSlotId;
    int i;
    int *pOffsets;
    void **ppManagedEntities;
    int anOffsets[2];

    if (*g_GAME_SelectedNetworkLobbyPeerId != 0) {
        anOffsets[0] = 0;
        anOffsets[1] = 0;
        if (*(int *)(*g_GAME_SelectedNetworkLobbyPeerId + 0x1c) == 0) {
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

// FUNCTION: LEMBALL 0x00421EC0
void LEMBALL_FASTCALL ResetBallChunkEntries(void *pBallManager) {
    int i;
    int cEntries;

    cEntries = 0;
    if (*(int *)((char *)pBallManager + 4) != 0 && 0 < *(int *)((char *)pBallManager + 0xc)) {
        i = 0;
        do {
            cEntries += 4;
            ++i;
            ((void (*)())*(void **)(**(int **)(*(int *)((char *)pBallManager + 4) - 4 + cEntries) + 0x104))();
        } while (i < *(int *)((char *)pBallManager + 0xc));
    }
}

// FUNCTION: LEMBALL 0x00401D48
void LEMBALL_FASTCALL ResetBallChunkEntriesThunk(void *pBallManager) {
    ResetBallChunkEntries(pBallManager);
}

// FUNCTION: LEMBALL 0x0042A030
void LEMBALL_FASTCALL ResetBoonChunkManagerObjects(void *pObject) {
    unsigned short *pState;

    pState = (unsigned short *)pObject;
    *pState = 0;
    ((void (*)())*(void **)(**(int **)(pState + 0x1a) + 0x104))();
    ((void (*)())*(void **)(**(int **)(pState + 0x1c) + 0x104))();
    ((void (*)())*(void **)(**(int **)(pState + 0x1e) + 0x104))();
    ((void (*)())*(void **)(**(int **)(pState + 0x20) + 0x104))();
}

// FUNCTION: LEMBALL 0x00402D51
void LEMBALL_FASTCALL ResetBoonChunkManagerObjectsThunk(void *pObject) {
    ResetBoonChunkManagerObjects(pObject);
}
#include "../network/safe_vtable.h"
