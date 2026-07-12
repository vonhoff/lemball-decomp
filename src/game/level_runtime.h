#ifndef LEMBALL_GAME_LEVEL_RUNTIME_H
#define LEMBALL_GAME_LEVEL_RUNTIME_H

#include "../engine/common.h"

struct LEVEL_LevelTileGridStaticView;

struct LEVEL_TileGridStaticRenderEntry {
    short m_nTileX00;
    short m_nTileY02;
    unsigned short m_nSortKey04;
    unsigned short m_nReserved06;
    void *m_pTileCell08;
};

struct LEVEL_SpecialTileGridStaticRenderList {
    LEVEL_TileGridStaticRenderEntry *m_pEntries00;
    int m_cEntries04;

    void BuildSpecialLevelTileGridStaticRenderEntries(
        LEVEL_LevelTileGridStaticView *pTileGrid);
    void BuildSpecialLevelTileGridStaticRenderEntriesThunk(
        LEVEL_LevelTileGridStaticView *pTileGrid);
};

struct LEVEL_ProjectileGeometryPair {
    int m_nX;
    int m_nY;

    LEVEL_ProjectileGeometryPair *InitializeSentinel(void);
    LEVEL_ProjectileGeometryPair *InitializeSentinelThunk(void);
};

struct LEVEL_ProjectileGeometryPairTable {
    LEVEL_ProjectileGeometryPair m_aPrimaryPairs00[24];
    LEVEL_ProjectileGeometryPair m_aSecondaryPairsC0[8];
    int m_nReserved100;

    LEVEL_ProjectileGeometryPairTable *InitializeProjectileGeometryPairTable(void);
    LEVEL_ProjectileGeometryPairTable *InitializeProjectileGeometryPairTableThunk(void);
    void ClearProjectileGeometryField100(void);
    void ClearProjectileGeometryField100Thunk(void);
};

struct LEVEL_CachedChunkObjectManagerBase {
    void **m_pVtable00;
    void *m_apObjects04[40];
    int m_cObjectsA4;
    int m_nReservedA8;
    int m_fEnabledAC;
};

struct LEVEL_ShpgChunkManager : public LEVEL_CachedChunkObjectManagerBase {
    void ActivateNestedChildrenFromOwnerTableA4(void);
    void ActivateNestedChildrenFromOwnerTableA4Thunk(void);
};

struct LEVEL_EnmyChunkManager : public LEVEL_CachedChunkObjectManagerBase {
    void ActivateNestedChildrenFromOwnerTableVariant(void);
    void ActivateNestedChildrenFromOwnerTableVariantThunk(void);
};

struct LEVEL_ChunkLoaderContext {
    void *m_pLevelMode00;
    int m_nFallbackLevel04;

    LEVEL_ChunkLoaderContext *ConstructLevelChunkLoaderContext(void *pLevelMode);
    LEVEL_ChunkLoaderContext *ConstructLevelChunkLoaderContextThunk(void *pLevelMode);
};

struct LEVEL_NodeChunkRecord {
    char m_abReserved00[0x10];
    int m_cLinks10;

    void ResetNodeChunkRecordLinkCount(void);
    void ResetNodeChunkRecordLinkCountThunk(void);
};

struct LEVEL_NodeChunkManager {
    LEVEL_NodeChunkRecord *m_pRecords00;
    int m_nReserved04;
    int m_cCapacity08;

    LEVEL_NodeChunkManager *ConstructNodeChunkManager(int cCapacity);
    LEVEL_NodeChunkManager *ConstructNodeChunkManagerThunk(int cCapacity);
    void ResetNodeChunkRecords(void);
    void ResetNodeChunkRecordsThunk(void);
};

struct LEVEL_SlnkChunkManager {
    void *m_pLevelMode00;
    void *m_pObjects04;
    int m_cCapacity08;

    LEVEL_SlnkChunkManager *ConstructSlnkChunkManager(void *pLevelMode, int cCapacity);
    LEVEL_SlnkChunkManager *ConstructSlnkChunkManagerThunk(void *pLevelMode, int cCapacity);
    void ResetType35ChunkObjects(void);
    void ResetType35ChunkObjectsThunk(void);
};

struct LEVEL_BallChunkManager {
    void *m_pLevelMode00;
    void *m_pObjects04;
    int m_nReserved08;
    int m_cCapacity0C;

    LEVEL_BallChunkManager *ConstructBallChunkManager(void *pLevelMode, int cCapacity);
    LEVEL_BallChunkManager *ConstructBallChunkManagerThunk(void *pLevelMode, int cCapacity);
};

struct LEVEL_Field194TimerBlock {
    int m_nNextTick00;
    int m_nReserved04;
    char m_abReserved08[0x12c0];
    int m_fActive12C8;

    void ResetLevelField194TimerBlock(void);
    void ResetLevelField194TimerBlockThunk(void);
};

void *LEMBALL_FASTCALL GetLevelGameModeOwner(void *pLevelMode);
void *LEMBALL_FASTCALL GetLevelGameModeOwnerThunk(void *pLevelMode);

extern LEVEL_BallChunkManager *g_pBallChunkManager;
extern char *g_pszActiveLevelFilePath;
extern void *g_pLoadedLevelFileBuffer;

#endif
