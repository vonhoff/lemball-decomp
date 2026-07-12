#include "../platform/win32.h"

// Temporary link scaffold for report generation only.
// These definitions are not binary-backed and must not carry reccmp markers.

void InitializeNetworkPlayerPlasChunkObjectPoolThunk(void *pPlasManager) {
    (void)pPlasManager;
}

void LoadLevelChunksForSkillAndLevelThunk(void *pLoaderContext, unsigned int nSkill, unsigned int nLevel, int fReuseState) {
    (void)pLoaderContext;
    (void)nSkill;
    (void)nLevel;
    (void)fReuseState;
}

void *ConstructInvsChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    (void)pLevelMode;
    (void)nCapacity;
    return pObject;
}

void *ConstructType18ChunkStreamThunk(void *pObject) {
    return pObject;
}

void *ConstructIceChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    (void)pLevelMode;
    (void)nCapacity;
    return pObject;
}

void *ConstructPgunChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    (void)pLevelMode;
    (void)nCapacity;
    return pObject;
}

void *ConstructTramChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    (void)pLevelMode;
    (void)nCapacity;
    return pObject;
}

void *ConstructBoonChunkManagerThunk(void *pObject, void *pLevelMode, void *pTileGrid) {
    (void)pLevelMode;
    (void)pTileGrid;
    return pObject;
}

void *ConstructLasrChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    (void)pLevelMode;
    (void)nCapacity;
    return pObject;
}

void *ConstructCaptureTriggerChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    (void)pLevelMode;
    (void)nCapacity;
    return pObject;
}

void *ConstructRockChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    (void)pLevelMode;
    (void)nCapacity;
    return pObject;
}

void *ConstructDoorChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    (void)pLevelMode;
    (void)nCapacity;
    return pObject;
}

void *ConstructMineChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    (void)pLevelMode;
    (void)nCapacity;
    return pObject;
}

void *ConstructCollChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    (void)pLevelMode;
    (void)nCapacity;
    return pObject;
}

void *ConstructEnmyChunkManagerThunk(void *pObject, void *pLevelMode, void *pGmobManager, void *pProjectileGeometryTable) {
    (void)pLevelMode;
    (void)pGmobManager;
    (void)pProjectileGeometryTable;
    return pObject;
}

void *ConstructShpgChunkManagerThunk(void *pObject, void *pLevelMode, void *pGmobManager, void *pProjectileGeometryTable) {
    (void)pLevelMode;
    (void)pGmobManager;
    (void)pProjectileGeometryTable;
    return pObject;
}

void *ConstructManagedEntityQueueCursorThunk(void *pObject, int nLevelMode, int nPrimaryCapacity, int nSecondaryCapacity) {
    (void)nLevelMode;
    (void)nPrimaryCapacity;
    (void)nSecondaryCapacity;
    return pObject;
}

void *ConstructProjectilePoolThunk(void *pObject) {
    return pObject;
}

void *ConstructPlasChunkManagerThunk(void *pObject, void *pLevelMode, void *pGmobManager, void *pProjectileGeometryTable) {
    (void)pLevelMode;
    (void)pGmobManager;
    (void)pProjectileGeometryTable;
    return pObject;
}

void *ConstructGmobChunkManagerThunk(void *pObject, void *pLevelMode) {
    (void)pLevelMode;
    return pObject;
}

void *ConstructMoveChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    (void)pLevelMode;
    (void)nCapacity;
    return pObject;
}

void *ConstructLiftChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    (void)pLevelMode;
    (void)nCapacity;
    return pObject;
}

void *ConstructLevelChunkStreamDispatcherThunk(void *pObject, int nChunkTypeCount) {
    (void)nChunkTypeCount;
    return pObject;
}

void SetGeometryChildEnabled(void *pGeometryChild, int fEnabled) {
    (void)pGeometryChild;
    (void)fEnabled;
}
