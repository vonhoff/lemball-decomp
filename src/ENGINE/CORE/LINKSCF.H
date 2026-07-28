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

// Original vtable entries point through these linker-generated ILT slots.
// LINKERILT: LEMBALL 0x00401b95
// ResetAllInvsChunkObjects
// LINKERILT: LEMBALL 0x0040333c
// AdvanceInvsChunkObjects
// LINKERILT: LEMBALL 0x00401e9c
// DispatchDoorChildArrayVirtual104
// LINKERILT: LEMBALL 0x00402a09
// ServiceActiveDoorChunkObjects
// LINKERILT: LEMBALL 0x0040369d
// EmitDoorChunkRenderEntries
// LINKERILT: LEMBALL 0x00401bea
// ResetProjectilePool
// LINKERILT: LEMBALL 0x00401019
// update_projectile_pool_real
// LINKERILT: LEMBALL 0x00402900
// EmitProjectilePoolRenderEntries
// LINKERILT: LEMBALL 0x0040240a
// ActivateManagedEntityGroupChildren
// LINKERILT: LEMBALL 0x00402a45
// ServiceCountedChildPointerTable
// LINKERILT: LEMBALL 0x0040295f
// ClearCountedChildBackReference
// LINKERILT: LEMBALL 0x00403675
// EmitGmobChunkManagerRenderEntries
// LINKERILT: LEMBALL 0x004011a9
// GetManagedEntityGroupCount
// LINKERILT: LEMBALL 0x00401078
// GetFirstCachedChunkObject
// LINKERILT: LEMBALL 0x00403549
// GetNextCachedChunkObject
// LINKERILT: LEMBALL 0x00402eb9
// GetCachedChunkObjectByIndex
// LINKERILT: LEMBALL 0x00402090
// GetNextManagedEntityGroup
// LINKERILT: LEMBALL 0x00402cd4
// RemoveCachedChunkObjectAndDestroy
// LINKERILT: LEMBALL 0x00401faf
// CountNestedIteratedChildEntries
// LINKERILT: LEMBALL 0x00403201
// GetFirstManagedEntityChildFromCachedGroup
// LINKERILT: LEMBALL 0x00402acc
// GetNextManagedEntityChildFromCachedGroup
// LINKERILT: LEMBALL 0x0040275c
// GetFirstIteratedChildStatus11C
// LINKERILT: LEMBALL 0x0040258b
// AdvanceCachedGroupManagedEntityChildIterator
// LINKERILT: LEMBALL 0x0040268a
// FindManagedEntityByPointer
// LINKERILT: LEMBALL 0x004027e8
// AppendCachedChunkObject
// LINKERILT: LEMBALL 0x004026cb
// AppendManagedEntityGroupFromIds
// LINKERILT: LEMBALL 0x00403058
// ResetCachedChunkManagerScratch
// LINKERILT: LEMBALL 0x004017df
// AttachAndActivateChunkObject
// LINKERILT: LEMBALL 0x00401cbc
// RemoveOwnerChildIfNoNestedEntries
// LINKERILT: LEMBALL 0x00402860
// ForwardOwnerChildCommandValue
// LINKERILT: LEMBALL 0x0040100f
// EmitIteratedChildBoundsRecords
// LINKERILT: LEMBALL 0x004032c4
// ActivateCollChunkObjects
// LINKERILT: LEMBALL 0x00402a72
// RefreshActiveCollChunkObjects
// LINKERILT: LEMBALL 0x00401956
// EmitCollChunkManagerRenderEntries
// LINKERILT: LEMBALL 0x00402167
// ActivateMineChunkObjects
// LINKERILT: LEMBALL 0x00401dc0
// RefreshActiveMineChunkObjects
// LINKERILT: LEMBALL 0x00403404
// EmitMineChunkManagerRenderEntries
// LINKERILT: LEMBALL 0x004032d3
// InitializeLiftChunkObjects
// LINKERILT: LEMBALL 0x00402518
// ActivateLiftChunkObjects
// LINKERILT: LEMBALL 0x0040340e
// EmitLiftChunkManagerRenderEntries
// LINKERILT: LEMBALL 0x00401fbe
// ActivateRockChunkObjects
// LINKERILT: LEMBALL 0x00401f2d
// RefreshActiveRockChunkObjects
// LINKERILT: LEMBALL 0x00401cad
// EmitRockChunkManagerRenderEntries
// LINKERILT: LEMBALL 0x00402360
// ActivateCaptureTriggerChunkObjects
// LINKERILT: LEMBALL 0x00401177
// RefreshActiveCaptureTriggerChunkObjects
// LINKERILT: LEMBALL 0x00403797
// EmitCaptureTriggerChunkManagerRenderEntries
// LINKERILT: LEMBALL 0x004014fb
// ActivateLasrChunkObjects
// LINKERILT: LEMBALL 0x00401942
// RefreshActiveLasrChunkObjects
// LINKERILT: LEMBALL 0x00401672
// EmitLasrChunkManagerRenderEntries
// LINKERILT: LEMBALL 0x004036d4
// InitializeTramChunkObjects
// LINKERILT: LEMBALL 0x004013de
// ActivateTramChunkObjects
// LINKERILT: LEMBALL 0x00403431
// EmitTramChunkManagerRenderEntries
// LINKERILT: LEMBALL 0x00401159
// ActivatePgunChunkObjects
// LINKERILT: LEMBALL 0x0040253b
// RefreshActivePgunChunkObjects
// LINKERILT: LEMBALL 0x004015dc
// EmitActivePgunChunkManagerRenderEntries
// LINKERILT: LEMBALL 0x00402559
// ActivateIceChunkObjects
// LINKERILT: LEMBALL 0x004018d9
// AdvanceIceChunkManagerObjects
// LINKERILT: LEMBALL 0x00402856
// return_zero_vtable_callback_0042dd30
// LINKERILT: LEMBALL 0x0040345e
// ActivateMoveChunkObjects
// LINKERILT: LEMBALL 0x00402ff4
// AdvanceActiveMoveChunkObjects
// LINKERILT: LEMBALL 0x00402da1
// EmitMoveChunkManagerRenderEntries

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
// STUB: LEMBALL 0x0042b4a0
// ActivateTramChunkObjects
// STUB: LEMBALL 0x0042b4f0
// EmitTramChunkManagerRenderEntries
// STUB: LEMBALL 0x0042c040
// ActivatePgunChunkObjects
// STUB: LEMBALL 0x0042c4d0
// RefreshActivePgunChunkObjects
// STUB: LEMBALL 0x0042c520
// EmitActivePgunChunkManagerRenderEntries
// STUB: LEMBALL 0x0042d800
// ActivateIceChunkObjects
// STUB: LEMBALL 0x0042dd00
// AdvanceIceChunkManagerObjects
// STUB: LEMBALL 0x0042f1f0
// ActivateMoveChunkObjects
// STUB: LEMBALL 0x0042f500
// AdvanceActiveMoveChunkObjects
// STUB: LEMBALL 0x0042f540
// EmitMoveChunkManagerRenderEntries

#endif
