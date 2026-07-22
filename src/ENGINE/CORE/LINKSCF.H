#ifndef LEMBALL_ENGINE_CORE_LINKSCF_H
#define LEMBALL_ENGINE_CORE_LINKSCF_H

struct LinkScfInvsChunkManagerView {
	LinkScfInvsChunkManagerView* ConstructInvsChunkManager(void* pLevelMode, int nCapacity);
	LinkScfInvsChunkManagerView* ConstructInvsChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfIceChunkManagerView {
	LinkScfIceChunkManagerView* ConstructIceChunkManager(void* pLevelMode, int nCapacity);
	LinkScfIceChunkManagerView* ConstructIceChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfPgunChunkManagerView {
	LinkScfPgunChunkManagerView* ConstructPgunChunkManager(void* pLevelMode, int nCapacity);
	LinkScfPgunChunkManagerView* ConstructPgunChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfTramChunkManagerView {
	LinkScfTramChunkManagerView* ConstructTramChunkManager(void* pLevelMode, int nCapacity);
	LinkScfTramChunkManagerView* ConstructTramChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfLasrChunkManagerView {
	LinkScfLasrChunkManagerView* ConstructLasrChunkManager(void* pLevelMode, int nCapacity);
	LinkScfLasrChunkManagerView* ConstructLasrChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfCaptureChunkManagerView {
	LinkScfCaptureChunkManagerView* ConstructCaptureTriggerChunkManager(void* pLevelMode, int nCapacity);
	LinkScfCaptureChunkManagerView* ConstructCaptureTriggerChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfRockChunkManagerView {
	LinkScfRockChunkManagerView* ConstructRockChunkManager(void* pLevelMode, int nCapacity);
	LinkScfRockChunkManagerView* ConstructRockChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfDoorChunkManagerView {
	LinkScfDoorChunkManagerView* ConstructDoorChunkManager(void* pLevelMode, int nCapacity);
	LinkScfDoorChunkManagerView* ConstructDoorChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfMineChunkManagerView {
	LinkScfMineChunkManagerView* ConstructMineChunkManager(void* pLevelMode, int nCapacity);
	LinkScfMineChunkManagerView* ConstructMineChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfCollChunkManagerView {
	LinkScfCollChunkManagerView* ConstructCollChunkManager(void* pLevelMode, int nCapacity);
	LinkScfCollChunkManagerView* ConstructCollChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfGmobChunkManagerView {
	LinkScfGmobChunkManagerView* ConstructGmobChunkManager(void* pLevelMode);
	LinkScfGmobChunkManagerView* ConstructGmobChunkManagerThunk(void* pLevelMode);
};

struct LinkScfMoveChunkManagerView {
	LinkScfMoveChunkManagerView* ConstructMoveChunkManager(void* pLevelMode, int nCapacity);
	LinkScfMoveChunkManagerView* ConstructMoveChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfLiftChunkManagerView {
	LinkScfLiftChunkManagerView* ConstructLiftChunkManager(void* pLevelMode, int nCapacity);
	LinkScfLiftChunkManagerView* ConstructLiftChunkManagerThunk(void* pLevelMode, int nCapacity);
};

struct LinkScfEnmyChunkManagerView {
	LinkScfEnmyChunkManagerView* ConstructEnmyChunkManager(void* pLevelMode,
														   void* pGmobManager,
														   void* pProjectileGeometryTable);
	LinkScfEnmyChunkManagerView* ConstructEnmyChunkManagerThunk(void* pLevelMode,
																void* pGmobManager,
																void* pProjectileGeometryTable);
};

struct LinkScfShpgChunkManagerView {
	LinkScfShpgChunkManagerView* ConstructShpgChunkManager(void* pLevelMode,
														   void* pGmobManager,
														   void* pProjectileGeometryTable);
	LinkScfShpgChunkManagerView* ConstructShpgChunkManagerThunk(void* pLevelMode,
																void* pGmobManager,
																void* pProjectileGeometryTable);
};

struct LinkScfPlasChunkManagerView {
	void LoadPlasChunkObjects(const unsigned short* pChunk, unsigned int cbChunk, int fReuseState);
	void LoadPlasChunkObjectsThunk(const unsigned short* pChunk, unsigned int cbChunk, int fReuseState);
};

struct LinkScfLevelChunkLoaderView {
	void LoadLevelChunksForSkillAndLevel(unsigned int nSkill, unsigned int nLevel, int fReuseState);
	void LoadLevelChunksForSkillAndLevelThunk(unsigned int nSkill, unsigned int nLevel, int fReuseState);
};

struct LinkScfBoonChunkManagerView {
	LinkScfBoonChunkManagerView* ConstructBoonChunkManager(void* pLevelMode, void* pTileGrid);
	LinkScfBoonChunkManagerView* ConstructBoonChunkManagerThunk(void* pLevelMode, void* pTileGrid);
	void DestroyBoonChunkManagerChildren(void);
};

#endif
