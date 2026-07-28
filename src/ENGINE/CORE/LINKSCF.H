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
	LinkScfGmobChunkManagerView* ConstructGmobChunkManager(void* pLevelMode, int nCapacity);
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

// Referenced real destinations that remain minimally implemented.
// STUB: LEMBALL 0x0040a270
// ResetAllInvsChunkObjects
// STUB: LEMBALL 0x0040a3b0
// AdvanceInvsChunkObjects
// STUB: LEMBALL 0x0040df90
// DispatchDoorChildArrayVirtual104
// STUB: LEMBALL 0x0040e550
// ServiceActiveDoorChunkObjects
// STUB: LEMBALL 0x0040e080
// EmitDoorChunkRenderEntries
// STUB: LEMBALL 0x00417e80
// ResetProjectilePool
// STUB: LEMBALL 0x004180e0
// EmitProjectilePoolRenderEntries
// STUB: LEMBALL 0x0041afd0
// ActivateManagedEntityGroupChildren
// STUB: LEMBALL 0x0041b7d0
// ServiceCountedChildPointerTable
// STUB: LEMBALL 0x0041bf00
// ClearCountedChildBackReference
// STUB: LEMBALL 0x0041b830
// EmitGmobChunkManagerRenderEntries
// STUB: LEMBALL 0x0041ecb0
// RemoveCachedChunkObjectAndDestroy
// STUB: LEMBALL 0x0041eab0
// CountNestedIteratedChildEntries
// STUB: LEMBALL 0x0041eae0
// GetFirstManagedEntityChildFromCachedGroup
// STUB: LEMBALL 0x0041eb00
// GetNextManagedEntityChildFromCachedGroup
// STUB: LEMBALL 0x0041eb40
// GetFirstIteratedChildStatus11C
// STUB: LEMBALL 0x0041eb60
// AdvanceCachedGroupManagedEntityChildIterator
// STUB: LEMBALL 0x0041eb90
// FindManagedEntityByPointer
// STUB: LEMBALL 0x0041ebe0
// AppendManagedEntityGroupFromIds
// STUB: LEMBALL 0x0041eca0
// ResetCachedChunkManagerScratch
// STUB: LEMBALL 0x0041ed20
// AttachAndActivateChunkObject
// STUB: LEMBALL 0x0041ed40
// RemoveOwnerChildIfNoNestedEntries
// STUB: LEMBALL 0x0041ede0
// ForwardOwnerChildCommandValue
// STUB: LEMBALL 0x0041ee00
// EmitIteratedChildBoundsRecords
// STUB: LEMBALL 0x00422420
// ActivateCollChunkObjects
// STUB: LEMBALL 0x00422550
// RefreshActiveCollChunkObjects
// STUB: LEMBALL 0x00422ef0
// EmitCollChunkManagerRenderEntries
// STUB: LEMBALL 0x00424080
// ActivateMineChunkObjects
// STUB: LEMBALL 0x004247b0
// RefreshActiveMineChunkObjects
// STUB: LEMBALL 0x00424800
// EmitMineChunkManagerRenderEntries
// STUB: LEMBALL 0x004256e0
// InitializeLiftChunkObjects
// STUB: LEMBALL 0x00425d30
// ActivateLiftChunkObjects
// STUB: LEMBALL 0x00425df0
// EmitLiftChunkManagerRenderEntries
// STUB: LEMBALL 0x00427010
// RefreshActiveRockChunkObjects
// STUB: LEMBALL 0x00427050
// EmitRockChunkManagerRenderEntries
// STUB: LEMBALL 0x004283c0
// RefreshActiveCaptureTriggerChunkObjects
// STUB: LEMBALL 0x00428410
// EmitCaptureTriggerChunkManagerRenderEntries
// STUB: LEMBALL 0x00429850
// RefreshActiveLasrChunkObjects
// STUB: LEMBALL 0x004298a0
// EmitLasrChunkManagerRenderEntries
// STUB: LEMBALL 0x0042afc0
// InitializeTramChunkObjects
// STUB: LEMBALL 0x0042b4f0
// EmitTramChunkManagerRenderEntries
// STUB: LEMBALL 0x0042c4d0
// RefreshActivePgunChunkObjects
// STUB: LEMBALL 0x0042c520
// EmitActivePgunChunkManagerRenderEntries
// STUB: LEMBALL 0x0042f540
// EmitMoveChunkManagerRenderEntries

#endif
