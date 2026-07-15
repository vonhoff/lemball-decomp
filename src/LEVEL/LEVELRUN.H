#ifndef LEMBALL_GAME_LEVEL_RUNTIME_H
#define LEMBALL_GAME_LEVEL_RUNTIME_H

#include "ENGINE/CORE/COMMON.H"

struct LevelLevelTileGridStaticView;

struct LevelTileGridStaticRenderEntry {
    short m_nTileX00;
    short m_nTileY02;
    unsigned short m_nSortKey04;
    unsigned short m_nReserved06;
    void *m_pTileCell08;
};

struct LevelSpecialTileGridStaticRenderList {
    LevelTileGridStaticRenderEntry *m_pEntries00;
    int m_cEntries04;

    void BuildSpecialLevelTileGridStaticRenderEntries(
        LevelLevelTileGridStaticView *pTileGrid);
    void BuildSpecialLevelTileGridStaticRenderEntriesThunk(
        LevelLevelTileGridStaticView *pTileGrid);
};

struct LevelProjectileGeometryPair {
    int m_nX;
    int m_nY;

    LevelProjectileGeometryPair *InitializeSentinel(void);
    LevelProjectileGeometryPair *InitializeSentinelThunk(void);
};

struct LevelProjectileGeometryPairTable {
    LevelProjectileGeometryPair m_aPrimaryPairs00[24];
    LevelProjectileGeometryPair m_aSecondaryPairsC0[8];
    int m_nReserved100;

    LevelProjectileGeometryPairTable *InitializeProjectileGeometryPairTable(void);
    LevelProjectileGeometryPairTable *InitializeProjectileGeometryPairTableThunk(void);
    void ClearProjectileGeometryField100(void);
    void ClearProjectileGeometryField100Thunk(void);
};

struct LevelCachedChunkObjectManagerBase {
    void **m_pVtable00;
    void *m_apObjects04[40];
    int m_cObjectsA4;
    int m_nReservedA8;
    int m_fEnabledAC;
};

struct LevelShpgChunkManager : public LevelCachedChunkObjectManagerBase {
    void ActivateNestedChildrenFromOwnerTableA4(void);
    void ActivateNestedChildrenFromOwnerTableA4Thunk(void);
};

struct LevelEnmyChunkManager : public LevelCachedChunkObjectManagerBase {
    void ActivateNestedChildrenFromOwnerTableVariant(void);
    void ActivateNestedChildrenFromOwnerTableVariantThunk(void);
};

struct LevelChunkLoaderContext {
    void *m_pLevelMode00;
    int m_nFallbackLevel04;

    LevelChunkLoaderContext *ConstructLevelChunkLoaderContext(void *pLevelMode);
    LevelChunkLoaderContext *ConstructLevelChunkLoaderContextThunk(void *pLevelMode);
};

struct LevelNodeChunkRecord {
    char m_abReserved00[0x10];
    int m_cLinks10;

    void ResetNodeChunkRecordLinkCount(void);
    void ResetNodeChunkRecordLinkCountThunk(void);
};

struct LevelNodeChunkManager {
    LevelNodeChunkRecord *m_pRecords00;
    int m_nReserved04;
    int m_cCapacity08;

    LevelNodeChunkManager *ConstructNodeChunkManager(int cCapacity);
    LevelNodeChunkManager *ConstructNodeChunkManagerThunk(int cCapacity);
    void ResetNodeChunkRecords(void);
    void ResetNodeChunkRecordsThunk(void);
};

struct LevelSlnkChunkManager {
    void *m_pLevelMode00;
    void *m_pObjects04;
    int m_cCapacity08;

    LevelSlnkChunkManager *ConstructSlnkChunkManager(void *pLevelMode, int cCapacity);
    LevelSlnkChunkManager *ConstructSlnkChunkManagerThunk(void *pLevelMode, int cCapacity);
    void ResetType35ChunkObjects(void);
    void ResetType35ChunkObjectsThunk(void);
};

struct LevelBallChunkManager {
    void *m_pLevelMode00;
    void *m_pObjects04;
    int m_nReserved08;
    int m_cCapacity0C;

    LevelBallChunkManager *ConstructBallChunkManager(void *pLevelMode, int cCapacity);
    LevelBallChunkManager *ConstructBallChunkManagerThunk(void *pLevelMode, int cCapacity);
};

struct LevelField194TimerBlock {
    int m_nNextTick00;
    int m_nReserved04;
    char m_abReserved08[0x12c0];
    int m_fActive12C8;

    void ResetLevelField194TimerBlock(void);
    void ResetLevelField194TimerBlockThunk(void);
};

void *LEMBALL_FASTCALL GetLevelGameModeOwner(void *pLevelMode);
void *LEMBALL_FASTCALL GetLevelGameModeOwnerThunk(void *pLevelMode);

extern LevelBallChunkManager *g_pBallChunkManager;
extern char *g_pszActiveLevelFilePath;
extern void *g_pLoadedLevelFileBuffer;

#endif
