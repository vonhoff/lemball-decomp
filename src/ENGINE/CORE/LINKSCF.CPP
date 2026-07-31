#include "ENGINE/CORE/LINKSCF.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/WIN32.H"
#include "ENGINE/GDI/VSGEOM.H"
#include "ENGINE/MEDIA/EFFSTRM.H"
#include "ENGINE/MEDIA/VSSTRM.H"
#include "GAME.H"
#include "LEVEL/LEVELRUN.H"

#include <string.h>

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nChildType, unsigned short nFlags);
};

struct InvsChunkObjectView : LevelChunkObjectBaseView {
	InvsChunkObjectView* ConstructInvsChunkObject(void);
};

struct RockChunkObjectView : LevelChunkObjectBaseView {
	RockChunkObjectView* ConstructRockChunkObject(void);
};

struct CaptureTriggerChunkObjectView : LevelChunkObjectBaseView {
	CaptureTriggerChunkObjectView* ConstructCaptureTriggerChunkObject(void);
};

struct LasrChunkObjectView : LevelChunkObjectBaseView {
	LasrChunkObjectView* ConstructLasrChunkObject(void);
};

struct TramChunkObjectView : LevelChunkObjectBaseView {
	TramChunkObjectView* ConstructTramChunkObject(void);
};

struct PgunChunkObjectView : LevelChunkObjectBaseView {
	PgunChunkObjectView* ConstructPgunChunkObject(void);
};

struct IceChunkObjectView : LevelChunkObjectBaseView {
	IceChunkObjectView* ConstructIceChunkObject(void);
};

// Recovered constructor and loader boundaries that are still being split into
// their original level modules.  Keep unverified addresses free of markers.

struct LevelBoundsPoint;
struct LevelThreeDwordBounds;
struct LevelBoundedRenderObjectView;
struct LevelType18ChunkStream;

struct LinkScfDeletableChild {
	virtual void Delete(BYTE fDelete);
};

extern void* g_pActiveNetworkRuntimeWindow;
extern int g_cbEffTransportMaxPacketBytes;
extern void* g_pActiveLevelGameMode;
extern void* g_pActiveManagedEntityOwner;
extern void* g_pLevelTileGrid;
extern int g_nLevelFrameClockTick;
extern int g_nLevelFrameClockTimeMs;
extern int g_nSelectedNetworkLobbyPeerId;
extern int g_GAME_ManagedEntityRegistryTable[1000];
extern unsigned short g_GAME_ManagedEntityRegistryCount;
extern int* LoadBinResource(int nResourceId);
extern void PASCAL noop_vtable_callback_0040a7f0(void* pUnused);
extern int PASCAL return_true_vtable_callback_0040a800(void* pUnused);
extern void* LEMBALL_FASTCALL get_object_field_0xf0(void* pObject);
extern int return_false_vtable_callback_0040a820(void);
extern void LEMBALL_FASTCALL copy_object_fields_0x9c_triplet(void* pObject, void* pUnused, LevelBoundsPoint* pPoint);
extern void LEMBALL_FASTCALL set_object_field_0x98(void* pObject, void* pUnused, int nValue);
extern int LEMBALL_FASTCALL get_object_field_0x98(void* pObject);
extern int LEMBALL_FASTCALL is_point_inside_3d_bounds(LevelBoundedRenderObjectView* pObject,
													  void* pUnused,
													  const LevelBoundsPoint* pPoint);
extern int LEMBALL_FASTCALL do_3d_bounds_intersect(LevelBoundedRenderObjectView* pObject,
												   void* pUnused,
												   const LevelThreeDwordBounds* pBounds);
extern void noop_vtable_callback_0040a990(void);
extern int return_true_vtable_callback_0040aa50(void);
extern int PASCAL return_false_vtable_callback_0040aa90(void* pUnused);
extern void PASCAL noop_vtable_callback_0040aa80(void* pUnused1, void* pUnused2);
extern void noop_vtable_callback_0040aaa0(void);
extern void noop_vtable_callback_0040aab0(void);
extern int PASCAL IsManagedEntityState18(int nState);
extern void LEMBALL_FASTCALL MarkObjectField178AndSetTimer3C(void* pObject);
extern void* LEMBALL_FASTCALL get_level_mode_chunk_loader_context(void* pLevelMode);
extern int LEMBALL_FASTCALL has_door_chunk_object_target_tile_latched(void* pObject);
extern void LEMBALL_FASTCALL schedule_door_chunk_object_update_in_10_ticks(void* pObject);
extern void LEMBALL_FASTCALL clear_door_chunk_object_target_tile_latch(void* pObject);
extern void* LEMBALL_FASTCALL get_object_field_0x30(void* pObject);
extern void* LEMBALL_FASTCALL get_level_mode_mine_chunk_manager(void* pLevelMode);
extern void* LEMBALL_FASTCALL get_level_mode_lift_chunk_manager(void* pLevelMode);
extern void LEMBALL_FASTCALL ReleaseTypedResourceObjectReference(void* pResourceObject);
extern int PASCAL ReturnTrueVtableCallbackThunk(void* pObject);
extern int ReturnTrueVtableCallbackSecondaryThunk(void);
extern void LEMBALL_FASTCALL DispatchType18ChunkStreamCommandsThunk(LevelType18ChunkStream* pStream);
extern void NoopVtableCallbackThunk(void);
extern void NoopType18ChunkStreamWriteThunk(void);
extern void* LEMBALL_FASTCALL DeleteLevelEffChunkStreamBaseThunk(void* pObject, void* pUnused, BYTE fDelete);
extern int PASCAL ReturnFalseChunkStreamCommandThunk(void* pUnused1, void* pUnused2, void* pUnused3);
extern int PASCAL ReturnFalseChunkStreamRenderThunk(void* pUnused);
extern void* LEMBALL_FASTCALL DeleteLevelChunkStreamAuto(void* pObject, void* pUnused, BYTE fDelete);
extern void* LEMBALL_FASTCALL DeleteCollChunkManagerAuto(void* pObject, void* pUnused, BYTE fDelete);
extern void* LEMBALL_FASTCALL DeleteCountedChildTableStreamAuto(void* pObject, void* pUnused, BYTE fDelete);
extern void* LEMBALL_FASTCALL DeleteRockChunkManagerAuto(void* pObject, void* pUnused, BYTE fDelete);
extern void* LEMBALL_FASTCALL DeletePgunChunkManagerAuto(void* pObject, void* pUnused, BYTE fDelete);
extern void* LEMBALL_FASTCALL DeleteIceChunkManagerAuto(void* pObject, void* pUnused, BYTE fDelete);
extern void* LEMBALL_FASTCALL DeleteMoveChunkManagerAuto(void* pObject, void* pUnused, BYTE fDelete);
extern void LEMBALL_FASTCALL update_projectile_pool_real(void* pPool);
extern int PASCAL return_zero_vtable_callback_0042dd30(void* pUnused);
void* LEMBALL_FASTCALL DeleteDoorChunkManagerAuto(void* pObject, void* pUnused, BYTE fDelete);
void* LEMBALL_FASTCALL DeleteProjectilePoolAuto(void* pObject, void* pUnused, BYTE fDelete);
void* LEMBALL_FASTCALL DeleteMineChunkManagerAuto(void* pObject, void* pUnused, BYTE fDelete);
void* LEMBALL_FASTCALL DeleteLiftChunkManagerAuto(void* pObject, void* pUnused, BYTE fDelete);
void* LEMBALL_FASTCALL DeleteCaptureTriggerChunkManagerAuto(void* pObject, void* pUnused, BYTE fDelete);
void* LEMBALL_FASTCALL DeleteLasrChunkManagerAuto(void* pObject, void* pUnused, BYTE fDelete);
void* LEMBALL_FASTCALL DeleteTramChunkManagerAuto(void* pObject, void* pUnused, BYTE fDelete);
void LEMBALL_FASTCALL ActivateOrStagePlasChunkObject(void* pObject);
void* ConstructManagedEntityQueueCursorThunk(void* pObject,
											 int nLevelMode,
											 int nPrimaryCapacity,
											 int nSecondaryCapacity);

void* LEMBALL_FASTCALL GetFirstCachedChunkObject(void* pObject);
void* LEMBALL_FASTCALL GetNextCachedChunkObject(void* pObject);
void* LEMBALL_FASTCALL GetCachedChunkObjectByIndex(void* pObject, void* pUnused, int nIndex);
void* LEMBALL_FASTCALL GetNextManagedEntityGroup(void* pObject);
void LEMBALL_FASTCALL AppendCachedChunkObject(void* pObject, void* pUnused, void* pChunkObject);
int LEMBALL_FASTCALL EmitCachedChunkManagerRenderEntries(void* pObject, void* pUnused, char* pEntries);
int LEMBALL_FASTCALL EmitCachedChunkManagerRenderEntriesThunk(void* pObject, void* pUnused, char* pEntries);
int LEMBALL_FASTCALL GetManagedEntityGroupCount(void* pObject);

#define DEFINE_LINKSCF_VTABLE_STUB_0(name)                                                                             \
	int LEMBALL_FASTCALL name(void* pObject)                                                                           \
	{                                                                                                                  \
		(void) pObject;                                                                                                \
		return 0;                                                                                                      \
	}

#define DEFINE_LINKSCF_VTABLE_STUB_1(name)                                                                             \
	int LEMBALL_FASTCALL name(void* pObject, void* pUnused, void* pArg0)                                               \
	{                                                                                                                  \
		(void) pObject;                                                                                                \
		(void) pUnused;                                                                                                \
		(void) pArg0;                                                                                                  \
		return 0;                                                                                                      \
	}

#define DEFINE_LINKSCF_VTABLE_STUB_2(name)                                                                             \
	int LEMBALL_FASTCALL name(void* pObject, void* pUnused, void* pArg0, void* pArg1)                                  \
	{                                                                                                                  \
		(void) pObject;                                                                                                \
		(void) pUnused;                                                                                                \
		(void) pArg0;                                                                                                  \
		(void) pArg1;                                                                                                  \
		return 0;                                                                                                      \
	}

typedef void(LEMBALL_FASTCALL* ActivateChunkObjectProc)(void* pObject);

void LEMBALL_FASTCALL ActivateRockChunkObjects(void* pManager);
void LEMBALL_FASTCALL ActivateCaptureTriggerChunkObjects(void* pManager);
void LEMBALL_FASTCALL ActivateLasrChunkObjects(void* pManager);
void LEMBALL_FASTCALL ActivateTramChunkObjects(void* pManager);
void LEMBALL_FASTCALL ActivatePgunChunkObjects(void* pManager);
void LEMBALL_FASTCALL ActivateIceChunkObjects(void* pManager);
void LEMBALL_FASTCALL ActivateMoveChunkObjects(void* pManager);
void LEMBALL_FASTCALL AdvanceIceChunkManagerObjects(void* pManager);
void LEMBALL_FASTCALL AdvanceActiveMoveChunkObjects(void* pManager);
void LEMBALL_FASTCALL AdvanceInvsChunkObjects(void* pManager);

DEFINE_LINKSCF_VTABLE_STUB_0(ResetAllInvsChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_0(DispatchDoorChildArrayVirtual104)
DEFINE_LINKSCF_VTABLE_STUB_0(ServiceActiveDoorChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitDoorChunkRenderEntries)
void LEMBALL_FASTCALL ResetProjectilePool(void* pPool);
int LEMBALL_FASTCALL EmitProjectilePoolRenderEntries(void* pPool, void* pUnused, void* pRenderEntry);
DEFINE_LINKSCF_VTABLE_STUB_0(ActivateManagedEntityGroupChildren)
DEFINE_LINKSCF_VTABLE_STUB_0(ServiceCountedChildPointerTable)
DEFINE_LINKSCF_VTABLE_STUB_2(ClearCountedChildBackReference)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitGmobChunkManagerRenderEntries)
DEFINE_LINKSCF_VTABLE_STUB_1(RemoveCachedChunkObjectAndDestroy)
DEFINE_LINKSCF_VTABLE_STUB_0(CountNestedIteratedChildEntries)
DEFINE_LINKSCF_VTABLE_STUB_0(GetFirstManagedEntityChildFromCachedGroup)
DEFINE_LINKSCF_VTABLE_STUB_0(GetNextManagedEntityChildFromCachedGroup)
DEFINE_LINKSCF_VTABLE_STUB_0(GetFirstIteratedChildStatus11C)
DEFINE_LINKSCF_VTABLE_STUB_1(AdvanceCachedGroupManagedEntityChildIterator)
DEFINE_LINKSCF_VTABLE_STUB_1(FindManagedEntityByPointer)
DEFINE_LINKSCF_VTABLE_STUB_2(AppendManagedEntityGroupFromIds)
DEFINE_LINKSCF_VTABLE_STUB_1(AttachAndActivateChunkObject)
DEFINE_LINKSCF_VTABLE_STUB_2(RemoveOwnerChildIfNoNestedEntries)
DEFINE_LINKSCF_VTABLE_STUB_1(ForwardOwnerChildCommandValue)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitIteratedChildBoundsRecords)
DEFINE_LINKSCF_VTABLE_STUB_0(ActivateCollChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_0(RefreshActiveCollChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitCollChunkManagerRenderEntries)
DEFINE_LINKSCF_VTABLE_STUB_0(ActivateMineChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_0(RefreshActiveMineChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitMineChunkManagerRenderEntries)
DEFINE_LINKSCF_VTABLE_STUB_0(InitializeLiftChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_0(ActivateLiftChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitLiftChunkManagerRenderEntries)
DEFINE_LINKSCF_VTABLE_STUB_0(RefreshActiveRockChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitRockChunkManagerRenderEntries)
DEFINE_LINKSCF_VTABLE_STUB_0(RefreshActiveCaptureTriggerChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitCaptureTriggerChunkManagerRenderEntries)
DEFINE_LINKSCF_VTABLE_STUB_0(RefreshActiveLasrChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitLasrChunkManagerRenderEntries)
DEFINE_LINKSCF_VTABLE_STUB_0(InitializeTramChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitTramChunkManagerRenderEntries)
DEFINE_LINKSCF_VTABLE_STUB_0(RefreshActivePgunChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitActivePgunChunkManagerRenderEntries)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitMoveChunkManagerRenderEntries)

#undef DEFINE_LINKSCF_VTABLE_STUB_0
#undef DEFINE_LINKSCF_VTABLE_STUB_1
#undef DEFINE_LINKSCF_VTABLE_STUB_2

void ResetCachedChunkManagerScratch(void);

// Compatibility shim retained for compiler-generated callers; original ILT slot
// 0x00401078 targets GetFirstCachedChunkObject directly.
void* LEMBALL_FASTCALL GetFirstCachedChunkObjectThunk(void* pObject)
{
	return GetFirstCachedChunkObject(pObject);
}

/* These tables retain the recovered common EFF stream interface while the
 * type-specific load and reset methods are reconstructed.  Keeping distinct
 * tables is important: constructors install concrete stream types, and later
 * recovery can replace individual slots without changing object layout. */
#define LINKSCF_EFF_CHUNK_VTABLE(deleteProc, activateProc, updateProc, commandProc, renderProc)                        \
	{(void*) ReturnTrueVtableCallbackThunk,                                                                            \
	 (void*) ReturnTrueVtableCallbackSecondaryThunk,                                                                   \
	 (void*) DispatchType18ChunkStreamCommandsThunk,                                                                   \
	 (void*) NoopVtableCallbackThunk,                                                                                  \
	 (void*) NoopType18ChunkStreamWriteThunk,                                                                          \
	 (void*) deleteProc,                                                                                               \
	 (void*) activateProc,                                                                                             \
	 (void*) updateProc,                                                                                               \
	 (void*) commandProc,                                                                                              \
	 (void*) renderProc}

// GLOBAL: LEMBALL 0x00495ea8
static void* g_LINKSCF_LiftChunkManagerVtable[10] = LINKSCF_EFF_CHUNK_VTABLE(DeleteLiftChunkManagerAuto,
																			 InitializeLiftChunkObjects,
																			 ActivateLiftChunkObjects,
																			 ReturnFalseChunkStreamCommandThunk,
																			 EmitLiftChunkManagerRenderEntries);
// GLOBAL: LEMBALL 0x00496b78
void* g_LINKSCF_MoveChunkManagerVtable[10] = LINKSCF_EFF_CHUNK_VTABLE(DeleteMoveChunkManagerAuto,
																	  ActivateMoveChunkObjects,
																	  AdvanceActiveMoveChunkObjects,
																	  ReturnFalseChunkStreamCommandThunk,
																	  EmitMoveChunkManagerRenderEntries);
// GLOBAL: LEMBALL 0x00494238
static void* g_LINKSCF_GmobChunkManagerVtable[10] = LINKSCF_EFF_CHUNK_VTABLE(DeleteCountedChildTableStreamAuto,
																			 ActivateManagedEntityGroupChildren,
																			 ServiceCountedChildPointerTable,
																			 ClearCountedChildBackReference,
																			 EmitGmobChunkManagerRenderEntries);
static void* g_LINKSCF_PlasChunkManagerVtable[10];
static void* g_LINKSCF_PlasChunkStreamVtable[10] = LINKSCF_EFF_CHUNK_VTABLE(DeleteLevelEffChunkStreamBaseThunk,
																			noop_vtable_callback_0040a990,
																			noop_vtable_callback_0040a990,
																			ReturnFalseChunkStreamCommandThunk,
																			ReturnFalseChunkStreamRenderThunk);
static void* g_LINKSCF_PlasChunkObjectVtable[66] = {
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a7f0,
	(void*) noop_vtable_callback_0040a990,
	(void*) return_true_vtable_callback_0040a800,
	(void*) get_object_field_0xf0,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) return_false_vtable_callback_0040a820,
	(void*) copy_object_fields_0x9c_triplet,
	(void*) noop_vtable_callback_0040a990,
	(void*) set_object_field_0x98,
	(void*) get_object_field_0x98,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) do_3d_bounds_intersect,
	(void*) is_point_inside_3d_bounds,
	(void*) noop_vtable_callback_0040a990,
	(void*) MarkObjectField178AndSetTimer3C,
	(void*) get_level_mode_chunk_loader_context,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) has_door_chunk_object_target_tile_latched,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) schedule_door_chunk_object_update_in_10_ticks,
	(void*) clear_door_chunk_object_target_tile_latch,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) return_true_vtable_callback_0040aa50,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040aa80,
	(void*) return_false_vtable_callback_0040aa90,
	(void*) noop_vtable_callback_0040aaa0,
	(void*) noop_vtable_callback_0040aab0,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) get_object_field_0x30,
	(void*) get_level_mode_mine_chunk_manager,
	(void*) noop_vtable_callback_0040a990,
	(void*) get_level_mode_lift_chunk_manager,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) IsManagedEntityState18,
	(void*) ActivateOrStagePlasChunkObject,
};
// GLOBAL: LEMBALL 0x00494008
static void* g_LINKSCF_ProjectilePoolVtable[10] = LINKSCF_EFF_CHUNK_VTABLE(DeleteProjectilePoolAuto,
																		   ResetProjectilePool,
																		   update_projectile_pool_real,
																		   ReturnFalseChunkStreamCommandThunk,
																		   EmitProjectilePoolRenderEntries);
static void* g_LINKSCF_ManagedEntityQueueCursorVtable[4];
// GLOBAL: LEMBALL 0x00494d08
static void* g_LINKSCF_CachedChunkObjectManagerBaseVtable[19] = {(void*) GetManagedEntityGroupCount,
																 (void*) GetFirstCachedChunkObject,
																 (void*) GetNextCachedChunkObject,
																 (void*) GetCachedChunkObjectByIndex,
																 (void*) GetNextManagedEntityGroup,
																 (void*) RemoveCachedChunkObjectAndDestroy,
																 (void*) CountNestedIteratedChildEntries,
																 (void*) GetFirstManagedEntityChildFromCachedGroup,
																 (void*) GetNextManagedEntityChildFromCachedGroup,
																 (void*) GetFirstIteratedChildStatus11C,
																 (void*) AdvanceCachedGroupManagedEntityChildIterator,
																 (void*) FindManagedEntityByPointer,
																 (void*) AppendCachedChunkObject,
																 (void*) AppendManagedEntityGroupFromIds,
																 (void*) ResetCachedChunkManagerScratch,
																 (void*) AttachAndActivateChunkObject,
																 (void*) RemoveOwnerChildIfNoNestedEntries,
																 (void*) ForwardOwnerChildCommandValue,
																 (void*) EmitIteratedChildBoundsRecords};
// GLOBAL: LEMBALL 0x00494d70
static void* g_LINKSCF_ShpgChunkManagerVtable[19] = {(void*) GetManagedEntityGroupCount,
													 (void*) GetFirstCachedChunkObject,
													 (void*) GetNextCachedChunkObject,
													 (void*) GetCachedChunkObjectByIndex,
													 (void*) GetNextManagedEntityGroup,
													 (void*) RemoveCachedChunkObjectAndDestroy,
													 (void*) CountNestedIteratedChildEntries,
													 (void*) GetFirstManagedEntityChildFromCachedGroup,
													 (void*) GetNextManagedEntityChildFromCachedGroup,
													 (void*) GetFirstIteratedChildStatus11C,
													 (void*) AdvanceCachedGroupManagedEntityChildIterator,
													 (void*) FindManagedEntityByPointer,
													 (void*) AppendCachedChunkObject,
													 (void*) AppendManagedEntityGroupFromIds,
													 (void*) ResetCachedChunkManagerScratch,
													 (void*) AttachAndActivateChunkObject,
													 (void*) RemoveOwnerChildIfNoNestedEntries,
													 (void*) ForwardOwnerChildCommandValue,
													 (void*) EmitIteratedChildBoundsRecords};
// GLOBAL: LEMBALL 0x004953f8
static void* g_LINKSCF_EnmyChunkManagerVtable[19] = {(void*) GetManagedEntityGroupCount,
													 (void*) GetFirstCachedChunkObject,
													 (void*) GetNextCachedChunkObject,
													 (void*) GetCachedChunkObjectByIndex,
													 (void*) GetNextManagedEntityGroup,
													 (void*) RemoveCachedChunkObjectAndDestroy,
													 (void*) CountNestedIteratedChildEntries,
													 (void*) GetFirstManagedEntityChildFromCachedGroup,
													 (void*) GetNextManagedEntityChildFromCachedGroup,
													 (void*) GetFirstIteratedChildStatus11C,
													 (void*) AdvanceCachedGroupManagedEntityChildIterator,
													 (void*) FindManagedEntityByPointer,
													 (void*) AppendCachedChunkObject,
													 (void*) AppendManagedEntityGroupFromIds,
													 (void*) ResetCachedChunkManagerScratch,
													 (void*) AttachAndActivateChunkObject,
													 (void*) RemoveOwnerChildIfNoNestedEntries,
													 (void*) ForwardOwnerChildCommandValue,
													 (void*) EmitIteratedChildBoundsRecords};
// GLOBAL: LEMBALL 0x004955a0
void* g_LINKSCF_CollChunkManagerVtable[10] = LINKSCF_EFF_CHUNK_VTABLE(DeleteCollChunkManagerAuto,
																	  ActivateCollChunkObjects,
																	  RefreshActiveCollChunkObjects,
																	  ReturnFalseChunkStreamCommandThunk,
																	  EmitCollChunkManagerRenderEntries);
// GLOBAL: LEMBALL 0x00495d30
static void* g_LINKSCF_MineChunkManagerVtable[10] = LINKSCF_EFF_CHUNK_VTABLE(DeleteMineChunkManagerAuto,
																			 ActivateMineChunkObjects,
																			 RefreshActiveMineChunkObjects,
																			 ReturnFalseChunkStreamCommandThunk,
																			 EmitMineChunkManagerRenderEntries);
// GLOBAL: LEMBALL 0x00493840
static void* g_LINKSCF_DoorChunkManagerVtable[10] = LINKSCF_EFF_CHUNK_VTABLE(DeleteDoorChunkManagerAuto,
																			 DispatchDoorChildArrayVirtual104,
																			 ServiceActiveDoorChunkObjects,
																			 ReturnFalseChunkStreamCommandThunk,
																			 EmitDoorChunkRenderEntries);
// GLOBAL: LEMBALL 0x00496020
void* g_LINKSCF_RockChunkManagerVtable[10] = LINKSCF_EFF_CHUNK_VTABLE(DeleteRockChunkManagerAuto,
																	  ActivateRockChunkObjects,
																	  RefreshActiveRockChunkObjects,
																	  ReturnFalseChunkStreamCommandThunk,
																	  EmitRockChunkManagerRenderEntries);
// GLOBAL: LEMBALL 0x004962e0
static void* g_LINKSCF_CaptureChunkManagerVtable[10] =
	LINKSCF_EFF_CHUNK_VTABLE(DeleteCaptureTriggerChunkManagerAuto,
							 ActivateCaptureTriggerChunkObjects,
							 RefreshActiveCaptureTriggerChunkObjects,
							 ReturnFalseChunkStreamCommandThunk,
							 EmitCaptureTriggerChunkManagerRenderEntries);
// GLOBAL: LEMBALL 0x00496458
static void* g_LINKSCF_LasrChunkManagerVtable[10] = LINKSCF_EFF_CHUNK_VTABLE(DeleteLasrChunkManagerAuto,
																			 ActivateLasrChunkObjects,
																			 RefreshActiveLasrChunkObjects,
																			 ReturnFalseChunkStreamCommandThunk,
																			 EmitLasrChunkManagerRenderEntries);
// GLOBAL: LEMBALL 0x00496710
static void* g_LINKSCF_TramChunkManagerVtable[10] = LINKSCF_EFF_CHUNK_VTABLE(DeleteTramChunkManagerAuto,
																			 InitializeTramChunkObjects,
																			 ActivateTramChunkObjects,
																			 ReturnFalseChunkStreamCommandThunk,
																			 EmitTramChunkManagerRenderEntries);
// GLOBAL: LEMBALL 0x00496888
void* g_LINKSCF_PgunChunkManagerVtable[10] = LINKSCF_EFF_CHUNK_VTABLE(DeletePgunChunkManagerAuto,
																	  ActivatePgunChunkObjects,
																	  RefreshActivePgunChunkObjects,
																	  ReturnFalseChunkStreamCommandThunk,
																	  EmitActivePgunChunkManagerRenderEntries);
// GLOBAL: LEMBALL 0x00496a00
void* g_LINKSCF_IceChunkManagerVtable[10] = LINKSCF_EFF_CHUNK_VTABLE(DeleteIceChunkManagerAuto,
																	 ActivateIceChunkObjects,
																	 AdvanceIceChunkManagerObjects,
																	 ReturnFalseChunkStreamCommandThunk,
																	 return_zero_vtable_callback_0042dd30);
// GLOBAL: LEMBALL 0x00493298
static void* g_LINKSCF_InvsChunkManagerVtable[10] = LINKSCF_EFF_CHUNK_VTABLE(DeleteLevelChunkStreamAuto,
																			 ResetAllInvsChunkObjects,
																			 AdvanceInvsChunkObjects,
																			 ReturnFalseChunkStreamCommandThunk,
																			 ReturnFalseChunkStreamRenderThunk);
static void* g_LINKSCF_BoonChunkObjectVtable[66];
static void* g_LINKSCF_LevelChunkStreamDispatcherVtable[10];
static char* g_pLINKSCF_PlasLevelMode;
static unsigned short g_nLINKSCF_LocalPlasOrdinal;

// GLOBAL: LEMBALL 0x004a74c0
void* g_pLINKSCF_GmobChunkManager;
// GLOBAL: LEMBALL 0x0049e1b8
unsigned short g_nLINKSCF_GmobObjectCount = 0;

#define LINKSCF_INITIALIZE_EFF_CHUNK_STREAM(pObject, nEventCode, nPayloadType)                                         \
	do {                                                                                                               \
		*(void**) (pObject) = g_GAME_EffStreamConstructionVtable;                                                      \
		*(int*) ((char*) (pObject) + 4) = (nEventCode);                                                                \
		((GameEffStream*) (pObject))->ResetStateFields();                                                              \
		*(int*) ((char*) (pObject) + 0x2c) = (nPayloadType);                                                           \
		*(void**) (pObject) = g_LEVEL_EffChunkStreamBaseVtable;                                                        \
		if (g_pActiveNetworkRuntimeWindow != 0) {                                                                      \
			*(int*) ((char*) (pObject) + 0x24) = 1;                                                                    \
			*(int*) ((char*) (pObject) + 0x18) += g_cbEffTransportMaxPacketBytes;                                      \
		}                                                                                                              \
	} while (0)

// GLOBAL: LEMBALL 0x004a782c
void* g_pCachedChunkManagerLevelMode;
// GLOBAL: LEMBALL 0x004a7830
void* g_pCachedChunkManagerEntityLookup;
// GLOBAL: LEMBALL 0x004a7834
void* g_pCachedChunkManagerProjectileGeometryTable;

struct LinkScfCachedChunkObjectManagerBaseView {
	LinkScfCachedChunkObjectManagerBaseView* ConstructCachedChunkObjectManagerBase(void* pLevelMode,
																				   void* pEntityLookup,
																				   void* pProjectileGeometryTable);
	LinkScfCachedChunkObjectManagerBaseView* ConstructCachedChunkObjectManagerBaseThunk(void* pLevelMode,
																						void* pEntityLookup,
																						void* pProjectileGeometryTable);
};

void* LEMBALL_FASTCALL ConstructPlasChunkObjectForLevel(void* pObject,
														int nUnusedEdx,
														int nX,
														int nY,
														int nZ,
														unsigned short nVariant,
														int nNetworkPlayer,
														int nStartDelay);
void* LEMBALL_FASTCALL ConstructPlasChunkObjectForLevelThunk(void* pObject,
															 int nUnusedEdx,
															 int nX,
															 int nY,
															 int nZ,
															 unsigned short nVariant,
															 int nNetworkPlayer,
															 int nStartDelay);

struct ManagedEntityQueueCursorForwardThunks {
	void* ConstructManagedEntityQueueCursorForwardThunk(int nLevelMode,
														unsigned int nPrimaryCapacity,
														unsigned int nSecondaryCapacity);
};

// LINKERILT: LEMBALL 0x004010aa
void* ManagedEntityQueueCursorForwardThunks::ConstructManagedEntityQueueCursorForwardThunk(
	int nLevelMode,
	unsigned int nPrimaryCapacity,
	unsigned int nSecondaryCapacity)
{
	return ConstructManagedEntityQueueCursorThunk(this, nLevelMode, nPrimaryCapacity, nSecondaryCapacity);
}

// LINKERILT: LEMBALL 0x00401604
LinkScfCachedChunkObjectManagerBaseView* LinkScfCachedChunkObjectManagerBaseView::
	ConstructCachedChunkObjectManagerBaseThunk(void* pLevelMode, void* pEntityLookup, void* pProjectileGeometryTable)
{
	return ConstructCachedChunkObjectManagerBase(pLevelMode, pEntityLookup, pProjectileGeometryTable);
}
// LINKERILT: LEMBALL 0x00401677
LinkScfIceChunkManagerView* LinkScfIceChunkManagerView::ConstructIceChunkManagerThunk(void* pLevelMode, int nCapacity)
{
	return ConstructIceChunkManager(pLevelMode, nCapacity);
}
// LINKERILT: LEMBALL 0x00401799
LinkScfShpgChunkManagerView* LinkScfShpgChunkManagerView::ConstructShpgChunkManagerThunk(void* pLevelMode,
																						 void* pGmobManager,
																						 void* pProjectileGeometryTable)
{
	return ConstructShpgChunkManager(pLevelMode, pGmobManager, pProjectileGeometryTable);
}
// LINKERILT: LEMBALL 0x004017b2
void LinkScfLevelChunkLoaderView::LoadLevelChunksForSkillAndLevelThunk(unsigned int nSkill,
																	   unsigned int nLevel,
																	   int fReuseState)
{
	LoadLevelChunksForSkillAndLevel(nSkill, nLevel, fReuseState);
}
// LINKERILT: LEMBALL 0x0040196a
LinkScfDoorChunkManagerView* LinkScfDoorChunkManagerView::ConstructDoorChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructDoorChunkManager(pLevelMode, nCapacity);
}
// LINKERILT: LEMBALL 0x00401a0f
LinkScfMoveChunkManagerView* LinkScfMoveChunkManagerView::ConstructMoveChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructMoveChunkManager(pLevelMode, nCapacity);
}
// LINKERILT: LEMBALL 0x00401e10
LinkScfCaptureChunkManagerView* LinkScfCaptureChunkManagerView::ConstructCaptureTriggerChunkManagerThunk(
	void* pLevelMode,
	int nCapacity)
{
	return ConstructCaptureTriggerChunkManager(pLevelMode, nCapacity);
}
// LINKERILT: LEMBALL 0x00401e88
void* LEMBALL_FASTCALL ConstructPlasChunkObjectForLevelThunk(void* pObject,
															 int nUnusedEdx,
															 int nX,
															 int nY,
															 int nZ,
															 unsigned short nVariant,
															 int nNetworkPlayer,
															 int nStartDelay)
{
	return ConstructPlasChunkObjectForLevel(pObject, nUnusedEdx, nX, nY, nZ, nVariant, nNetworkPlayer, nStartDelay);
}
// LINKERILT: LEMBALL 0x00401ea6
LinkScfLiftChunkManagerView* LinkScfLiftChunkManagerView::ConstructLiftChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructLiftChunkManager(pLevelMode, nCapacity);
}
// LINKERILT: LEMBALL 0x00401ece
LinkScfEnmyChunkManagerView* LinkScfEnmyChunkManagerView::ConstructEnmyChunkManagerThunk(void* pLevelMode,
																						 void* pGmobManager,
																						 void* pProjectileGeometryTable)
{
	return ConstructEnmyChunkManager(pLevelMode, pGmobManager, pProjectileGeometryTable);
}
// LINKERILT: LEMBALL 0x00401f64
int LEMBALL_FASTCALL EmitCachedChunkManagerRenderEntriesThunk(void* pObject, void* pUnused, char* pEntries)
{
	return EmitCachedChunkManagerRenderEntries(pObject, pUnused, pEntries);
}
// LINKERILT: LEMBALL 0x0040204a
LinkScfLasrChunkManagerView* LinkScfLasrChunkManagerView::ConstructLasrChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructLasrChunkManager(pLevelMode, nCapacity);
}
// LINKERILT: LEMBALL 0x0040204f
LinkScfBoonChunkManagerView* LinkScfBoonChunkManagerView::ConstructBoonChunkManagerThunk(void* pLevelMode,
																						 void* pTileGrid)
{
	return ConstructBoonChunkManager(pLevelMode, pTileGrid);
}

// LINKERILT: LEMBALL 0x00402af4
LinkScfMineChunkManagerView* LinkScfMineChunkManagerView::ConstructMineChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructMineChunkManager(pLevelMode, nCapacity);
}
// LINKERILT: LEMBALL 0x00402f95
LinkScfPgunChunkManagerView* LinkScfPgunChunkManagerView::ConstructPgunChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructPgunChunkManager(pLevelMode, nCapacity);
}
// LINKERILT: LEMBALL 0x00402fdb
LinkScfCollChunkManagerView* LinkScfCollChunkManagerView::ConstructCollChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructCollChunkManager(pLevelMode, nCapacity);
}
// LINKERILT: LEMBALL 0x004030e9
LinkScfRockChunkManagerView* LinkScfRockChunkManagerView::ConstructRockChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructRockChunkManager(pLevelMode, nCapacity);
}
// LINKERILT: LEMBALL 0x004031a7
LinkScfTramChunkManagerView* LinkScfTramChunkManagerView::ConstructTramChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructTramChunkManager(pLevelMode, nCapacity);
}
// LINKERILT: LEMBALL 0x004032d8
void LinkScfPlasChunkManagerView::LoadPlasChunkObjectsThunk(const unsigned short* pChunk,
															unsigned int cbChunk,
															int fReuseState)
{
	LoadPlasChunkObjects(pChunk, cbChunk, fReuseState);
}
// LINKERILT: LEMBALL 0x0040349f
LinkScfInvsChunkManagerView* LinkScfInvsChunkManagerView::ConstructInvsChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructInvsChunkManager(pLevelMode, nCapacity);
}
// FUNCTION: LEMBALL 0x00408240
void LinkScfLevelChunkLoaderView::LoadLevelChunksForSkillAndLevel(unsigned int nSkill,
																  unsigned int nLevel,
																  int fReuseState)
{
	static const int anSkillResourceBases[5] = {0x244, 0x25d, 0x277, 0x294, 0x2aa};
	void* pLoaderContext;
	int* pResourceObject;
	unsigned char* pChunk;
	unsigned char* pEnd;
	char* pLevelMode;
	int nResourceId;

	pLoaderContext = this;
	if (nSkill > 4) {
		nSkill = 4;
	}
	if (nLevel == 9999) {
		nLevel = *(unsigned int*) ((char*) pLoaderContext + 4);
	}
	nResourceId = anSkillResourceBases[nSkill] + (int) nLevel;
	pResourceObject = LoadBinResource(nResourceId);
	if (pResourceObject == 0) {
		return;
	}
	if (pResourceObject[4] == 0) {
		((void (*)(void)) * (void**) (*pResourceObject + 0x1c))();
	}
	else {
		pResourceObject[9] = 0;
	}
	++pResourceObject[2];
	g_pLoadedLevelFileBuffer = ((void* (*) (void) ) * (void**) (*pResourceObject + 0x28))();
	pChunk = (unsigned char*) g_pLoadedLevelFileBuffer;
	pEnd = pChunk + pResourceObject[10];
	pLevelMode = *(char**) pLoaderContext;
	while (pChunk + 8 <= pEnd) {
		unsigned int nTag = *(unsigned int*) pChunk;
		unsigned int cbChunk = *(unsigned int*) (pChunk + 4);
		if (cbChunk < 8 || pChunk + cbChunk > pEnd) {
			break;
		}
		if (nTag == 0x47445346) {
			((LevelTileGridRuntimeView*) *(void**) (pLevelMode + 0x110))
				->LoadLevelTileGridChunk((const unsigned short*) (pChunk + 8), cbChunk - 8, fReuseState);
		}
		else if (nTag == 0x41492020) {
			LoadLevelAiChunk(pLevelMode, (const unsigned short*) (pChunk + 8), cbChunk - 8);
		}
		else if (nTag == 0x464c4147) {
			LoadLevelFlagChunk(pLevelMode, (const unsigned short*) (pChunk + 8));
		}
		else if (nTag == 0x44454654) {
			((LevelTileGridRuntimeView*) *(void**) (pLevelMode + 0x110))
				->SetLevelTileGridDefaultVariantIds((const unsigned short*) (pChunk + 8), cbChunk - 8);
		}
		else if (nTag == 0x4e414d45) {
			((LevelTileGridRuntimeView*) *(void**) (pLevelMode + 0x110))
				->SetLevelTileGridName((const char*) (pChunk + 8), cbChunk - 8);
		}
		else if (nTag == 0x414e494d) {
			LoadAnimChunkRecords(*(void**) (pLevelMode + 0x194), (const unsigned short*) (pChunk + 8));
		}
		else if (nTag == 0x4e4f4445) {
			((LevelNodeChunkManager*) *(void**) (pLevelMode + 0x17c))
				->LoadNodeChunkRecords((const unsigned short*) (pChunk + 8), cbChunk - 8, fReuseState);
		}
		else if (nTag == 0x504c4153) {
			((LinkScfPlasChunkManagerView*) *(void**) (pLevelMode + 0x15c))
				->LoadPlasChunkObjectsThunk((const unsigned short*) (pChunk + 8), cbChunk - 8, fReuseState);
		}
		if (nTag == 0x454e443f) {
			break;
		}
		pChunk += (cbChunk + 3) & ~3U;
	}
	--pResourceObject[2];
	ReleaseTypedResourceObjectReference(pResourceObject);
	(void) fReuseState;
}
extern void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);

extern void* g_LINKSCF_InvsChunkObjectVtable[16];

// FUNCTION: LEMBALL 0x00409ca0
InvsChunkObjectView* InvsChunkObjectView::ConstructInvsChunkObject(void)
{
	InitializeLevelChunkObjectBase(0x36, 0, 0);
	*(void**) this = g_LINKSCF_InvsChunkObjectVtable;
	return this;
}

// FUNCTION: LEMBALL 0x00409d10
void LEMBALL_FASTCALL destroy_invs_chunk_object(void* pObject)
{
	*(void**) pObject = g_LINKSCF_InvsChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
}

// FUNCTION: LEMBALL 0x0040a210
LinkScfInvsChunkManagerView* LinkScfInvsChunkManagerView::ConstructInvsChunkManager(void* pLevelMode, int nCapacity)
{
	LINKSCF_INITIALIZE_EFF_CHUNK_STREAM(this, 0x20, 0x15);
	*(void**) this = g_LINKSCF_InvsChunkManagerVtable;
	*(void**) ((char*) this + 0x38) = pLevelMode;
	*(int*) ((char*) this + 0x30) = nCapacity;
	*(int*) ((char*) this + 0x3c) = 0;
	return this;
}
// FUNCTION: LEMBALL 0x0040b020
void* ConstructLevelChunkStreamDispatcherThunk(void* pObject, int nChunkTypeCount)
{
	int i;

	InitializeRenderQueueNodeBase(pObject);
	*(void**) pObject = g_LINKSCF_LevelChunkStreamDispatcherVtable;
	*(int*) ((char*) pObject + 0x14) = nChunkTypeCount;
	*(int*) ((char*) pObject + 0x18) = 0;
	*(void**) ((char*) pObject + 0x10) = AllocateVSMemBlock((unsigned int) (nChunkTypeCount * 4));
	*(int**) ((char*) pObject + 0x1c) = (int*) AllocateVSMemBlock(0x60);
	for (i = 0; i < 24; ++i) {
		(*(int**) ((char*) pObject + 0x1c))[i] = -1;
	}
	*(void**) ((char*) pObject + 0x20) = 0;
	return pObject;
}
// FUNCTION: LEMBALL 0x0040df30
LinkScfDoorChunkManagerView* LinkScfDoorChunkManagerView::ConstructDoorChunkManager(void* pLevelMode, int nCapacity)
{
	LINKSCF_INITIALIZE_EFF_CHUNK_STREAM(this, 0x13, 8);
	*(void**) this = g_LINKSCF_DoorChunkManagerVtable;
	*(void**) ((char*) this + 0x30) = pLevelMode;
	*(int*) ((char*) this + 0x38) = nCapacity;
	*(int*) ((char*) this + 0x3c) = 0;
	return this;
}
void LEMBALL_FASTCALL DestroyLevelChunkStreamAuto(void* pObject)
{
	void* pChild;

	*(void**) pObject = g_LINKSCF_InvsChunkManagerVtable;
	pChild = *(void**) ((char*) pObject + 0x3c);
	if (pChild != 0) {
		((LinkScfDeletableChild*) pChild)->Delete(3);
	}
	((VsNetEffStreamCommon*) pObject)->VsNetEffStreamCommon::~VsNetEffStreamCommon();
}

// FUNCTION: LEMBALL 0x0040ecb0
void* LEMBALL_FASTCALL ConstructPlasChunkObjectForLevel(void* pObject,
														int nUnusedEdx,
														int nX,
														int nY,
														int nZ,
														unsigned short nVariant,
														int nNetworkPlayer,
														int nStartDelay)
{
	int i;
	int nRegistryIndex;
	unsigned short* pChildSlots;
	int* pChildRecords;

	(void) nUnusedEdx;

	for (i = 0; i < 0x22c / 4; ++i) {
		((int*) pObject)[i] = 0;
	}
	*(void**) pObject = g_LINKSCF_PlasChunkObjectVtable;
	*(int*) ((char*) pObject + 0x20) = -1;
	*(int*) ((char*) pObject + 0x24) = -1;
	*(int*) ((char*) pObject + 0x28) = -1;
	for (i = 0x40; i <= 0x54; i += 4) {
		*(int*) ((char*) pObject + i) = 0xaa55aa55;
	}
	for (i = 0x78; i <= 0x84; i += 4) {
		*(int*) ((char*) pObject + i) = 0xaa55aa55;
	}
	for (i = 0x9c; i <= 0xb0; i += 4) {
		*(int*) ((char*) pObject + i) = 0xaa55aa55;
	}
	for (i = 0xd8; i <= 0xec; i += 4) {
		*(int*) ((char*) pObject + i) = 0xaa55aa55;
	}
	for (i = 0xf4; i <= 0xfc; i += 4) {
		*(int*) ((char*) pObject + i) = 0xaa55aa55;
	}
	*(int*) ((char*) pObject + 0x64) = nNetworkPlayer == 0 ? 2 : 1;
	*(unsigned short*) ((char*) pObject + 0x68) = 0x17f;
	*(unsigned short*) ((char*) pObject + 0x6c) = 0xffff;
	pChildSlots = (unsigned short*) AllocateVSMemBlock(8);
	if (pChildSlots != 0) {
		pChildSlots[0] = 0;
		pChildSlots[1] = 0x14;
		pChildRecords = (int*) AllocateVSMemBlock(0x14 * 0x14);
		*(int**) (pChildSlots + 2) = pChildRecords;
		if (pChildRecords != 0) {
			for (i = 0; i < 0x14; ++i) {
				pChildRecords[i * 5] = 0;
				pChildRecords[i * 5 + 1] = 0xaa55aa55;
				pChildRecords[i * 5 + 2] = 0xaa55aa55;
				pChildRecords[i * 5 + 3] = 0xaa55aa55;
			}
		}
	}
	*(unsigned short**) ((char*) pObject + 0x70) = pChildSlots;
	nRegistryIndex = 0;
	while (nRegistryIndex < g_GAME_ManagedEntityRegistryCount &&
		   g_GAME_ManagedEntityRegistryTable[nRegistryIndex] != 0) {
		++nRegistryIndex;
	}
	if (nRegistryIndex == g_GAME_ManagedEntityRegistryCount) {
		++g_GAME_ManagedEntityRegistryCount;
	}
	*(unsigned short*) ((char*) pObject + 0x6a) = (unsigned short) nRegistryIndex;
	g_GAME_ManagedEntityRegistryTable[nRegistryIndex] = (int) (unsigned long) pObject;
	*(int*) ((char*) pObject + 0x40) = nX << 12;
	*(int*) ((char*) pObject + 0x44) = nY << 12;
	*(int*) ((char*) pObject + 0x48) = nZ << 12;
	*(unsigned short*) ((char*) pObject + 0xb6) = nVariant;
	*(int*) ((char*) pObject + 0x170) = nNetworkPlayer;
	*(int*) ((char*) pObject + 0x174) = nStartDelay;
	*(int*) ((char*) pObject + 0x194) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x198) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x19c) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x1b4) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x1b8) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x1bc) = 0xaa55aa55;
	return pObject;
}
// FUNCTION: LEMBALL 0x0040ed90
void LEMBALL_FASTCALL ActivateOrStagePlasChunkObject(void* pObject)
{
	char* pPlayer;
	char* pOwner;
	unsigned short nOrdinal;
	int nX;
	int nY;
	int nZ;
	int nManagedCount;

	pPlayer = (char*) pObject;
	pOwner = (char*) g_pActiveManagedEntityOwner;
	if (*(int*) (pPlayer + 0x170) != 0) {
		*(int*) (pPlayer + 0x114) = 1;
		*(int*) (pPlayer + 0xb8) = 8;
		return;
	}

	nOrdinal = g_nLINKSCF_LocalPlasOrdinal++;
	*(unsigned short*) (pPlayer + 0x168) = nOrdinal;
	if (pOwner != 0 && nOrdinal < 4) {
		*(void**) (pOwner + 0x1d0 + nOrdinal * 4) = pObject;
	}
	*(int*) (pPlayer + 0xb8) = 0x0c;
	*(int*) (pPlayer + 0x94) = g_nLevelFrameClockTimeMs;
	*(int*) (pPlayer + 0xcc) = *(int*) (pPlayer + 0x174) + g_nLevelFrameClockTick;

	nX = *(int*) (pPlayer + 0x40) >> 12;
	nY = *(int*) (pPlayer + 0x44) >> 12;
	nZ = *(int*) (pPlayer + 0x48) >> 12;
	*(int*) (pPlayer + 0x14) = nX - 8;
	*(int*) (pPlayer + 0x18) = nY - 8;
	*(int*) (pPlayer + 0x1c) = nZ;
	*(int*) (pPlayer + 0x20) = nX + 7;
	*(int*) (pPlayer + 0x24) = nY + 7;
	*(int*) (pPlayer + 0x28) = nZ + 0x0f;

	if (pOwner != 0 && *(void**) (pOwner + 0x120) != 0) {
		nManagedCount = *(int*) (pOwner + 0x118);
		if (nManagedCount < *(int*) (pOwner + 0x11c)) {
			*(void**) (*(char**) (pOwner + 0x120) + nManagedCount * 4) = pObject;
			*(int*) (pOwner + 0x118) = nManagedCount + 1;
		}
	}
	*(int*) (pPlayer + 0x4c) = 0x3000;
	*(int*) (pPlayer + 0x50) = 0;
	*(int*) (pPlayer + 0x54) = 0xa000;
	*(int*) (pPlayer + 0x7c) = 0;
	*(unsigned short*) (pPlayer + 0x6e) = *(unsigned short*) (pPlayer + 0xb6);
	*(int*) (pPlayer + 0x78) = 0;
	*(int*) (pPlayer + 0x74) = 0;
	*(int*) (pPlayer + 0x84) = 0;
	*(int*) (pPlayer + 0x9c) = *(int*) (pPlayer + 0x40);
	*(int*) (pPlayer + 0xa0) = *(int*) (pPlayer + 0x44);
	*(int*) (pPlayer + 0xa4) = *(int*) (pPlayer + 0x48) + 0x44000;
	*(int*) (pPlayer + 0x2c) = 0;
	*(int*) (pPlayer + 0x164) = 1;
	*(int*) (pPlayer + 0x224) = 0;
	*(int*) (pPlayer + 0x18c) = 0;
	*(int*) (pPlayer + 0x188) = 0;
	*(int*) (pPlayer + 0x218) = 0;
	*(unsigned short*) (pPlayer + 0x228) = 0x19;
	*(unsigned short*) (pPlayer + 0xb4) = *(unsigned short*) (pPlayer + 0xb6);
	*(int*) (pPlayer + 0x220) = 0;
}

// FUNCTION: LEMBALL 0x00414da0
void* ConstructManagedEntityQueueCursorThunk(void* pObject,
											 int nLevelMode,
											 int nPrimaryCapacity,
											 int nSecondaryCapacity)
{
	void* pDispatchQueue;

	InitializeRenderQueueNodeBase(pObject);
	*(void**) pObject = g_LINKSCF_ManagedEntityQueueCursorVtable;
	*(int*) ((char*) pObject + 0x20) = nLevelMode;
	*(int*) ((char*) pObject + 0x18) = nPrimaryCapacity;
	*(int*) ((char*) pObject + 0x1c) = nSecondaryCapacity;
	pDispatchQueue = *(void**) (nLevelMode + 0x164);
	*(void**) ((char*) pObject + 0x24) = pDispatchQueue;
	((GameRenderDispatchQueue*) pDispatchQueue)->RegisterOrderedRenderDispatchClient(pObject, 0);
	return pObject;
}

// FUNCTION: LEMBALL 0x00417d80
void* ConstructProjectilePoolThunk(void* pObject)
{
	int i;

	LINKSCF_INITIALIZE_EFF_CHUNK_STREAM(pObject, 0x21, 0x16);
	*(void**) pObject = g_LINKSCF_ProjectilePoolVtable;
	*(void**) ((char*) pObject + 0x30) = 0;
	for (i = 0; i < 40; ++i) {
		*(void**) ((char*) pObject + 0x34 + i * 4) = 0;
	}
	*(int*) ((char*) pObject + 0xdc) = 0;
	return pObject;
}
// FUNCTION: LEMBALL 0x00418400
void* ConstructPlasChunkManagerThunk(void* pObject,
									 void* pLevelMode,
									 void* pGmobManager,
									 void* pProjectileGeometryTable)
{
	int i;
	char* pStream;

	g_pLINKSCF_PlasLevelMode = (char*) pLevelMode;
	for (i = 0; i < 40; ++i) {
		((int*) pObject)[i] = 0;
	}
	*(void**) pObject = g_LINKSCF_PlasChunkManagerVtable;
	*(void**) ((char*) pObject + 0xa4) = 0;
	*(void**) ((char*) pObject + 0xa8) = 0;
	*(int*) ((char*) pObject + 0xac) = 0;
	pStream = (char*) pObject + 0xb0;
	LINKSCF_INITIALIZE_EFF_CHUNK_STREAM(pStream, 0x22, 0x17);
	*(void**) pStream = g_LINKSCF_PlasChunkStreamVtable;
	*(int*) ((char*) pObject + 0x128) = 0;
	*(int*) ((char*) pObject + 0xe0) = 0x112;
	*(int*) ((char*) pObject + 0xf0) = 0x34a;
	*(int*) ((char*) pObject + 0x100) = 0;
	*(int*) ((char*) pObject + 0x110) = 4;
	*(int*) ((char*) pObject + 0x114) = 0;
	*(int*) ((char*) pObject + 0x118) = 0;
	*(int*) ((char*) pObject + 0x11c) = 0;
	*(int*) ((char*) pObject + 0x124) = 1;
	*(int*) ((char*) pObject + 0x14c) = 0;
	(void) pLevelMode;
	(void) pGmobManager;
	(void) pProjectileGeometryTable;
	return pObject;
}

// FUNCTION: LEMBALL 0x00418ca0
void InitializeNetworkPlayerPlasChunkObjectPoolThunk(void* pPlasManager)
{
	(void) pPlasManager;
}
// FUNCTION: LEMBALL 0x00418d20
void LinkScfPlasChunkManagerView::LoadPlasChunkObjects(const unsigned short* pChunk,
													   unsigned int cbChunk,
													   int fReuseState)
{
	void* pManager;
	char* pManagerBytes;
	int nPlayerCount;
	int nPlayerIndex;

	pManager = this;
	(void) cbChunk;

	pManagerBytes = (char*) pManager;
	*(int*) (pManagerBytes + 0x124) = 1;
	*(int*) (pManagerBytes + 0xe0) = pChunk[0];
	*(int*) (pManagerBytes + 0xf0) = pChunk[1];
	*(int*) (pManagerBytes + 0x100) = pChunk[2];
	for (nPlayerIndex = 0; nPlayerIndex < 3; ++nPlayerIndex) {
		*(int*) (pManagerBytes + 0xe4 + nPlayerIndex * 4) = *(int*) (pManagerBytes + 0xe0);
		*(int*) (pManagerBytes + 0xf4 + nPlayerIndex * 4) = *(int*) (pManagerBytes + 0xf0);
		*(int*) (pManagerBytes + 0x104 + nPlayerIndex * 4) = *(int*) (pManagerBytes + 0x100);
	}

	nPlayerCount = 4 - *(int*) (g_pLINKSCF_PlasLevelMode + 0xf8);
	if (nPlayerCount < 0) {
		nPlayerCount = 0;
	}
	else if (nPlayerCount > 4) {
		nPlayerCount = 4;
	}
	*(int*) (pManagerBytes + 0x128) = nPlayerCount;
	g_nLINKSCF_LocalPlasOrdinal = 0;
	for (nPlayerIndex = 0; nPlayerIndex < 4; ++nPlayerIndex) {
		void* pPlayer;

		pPlayer = 0;
		if (fReuseState != 0) {
			pPlayer = *(void**) (g_pLINKSCF_PlasLevelMode + 0x1d0 + nPlayerIndex * 4);
		}
		if (pPlayer == 0) {
			pPlayer = AllocateVSMemBlock(0x22c);
			if (pPlayer != 0) {
				ConstructPlasChunkObjectForLevelThunk(
					pPlayer,
					0,
					*(int*) (pManagerBytes + 0xe0 + nPlayerIndex * 4),
					*(int*) (pManagerBytes + 0xf0 + nPlayerIndex * 4),
					*(int*) (pManagerBytes + 0x100 + nPlayerIndex * 4),
					0,
					0,
					nPlayerIndex < nPlayerCount ? 0 : (0xf3c + (nPlayerIndex - nPlayerCount) * 800) / 0x32);
			}
		}
		if (pPlayer != 0) {
			ActivateOrStagePlasChunkObject(pPlayer);
		}
		if (nPlayerIndex < nPlayerCount) {
			*(void**) (pManagerBytes + 0x12c + nPlayerIndex * 4) = pPlayer;
		}
		*(void**) (g_pLINKSCF_PlasLevelMode + 0x1d0 + nPlayerIndex * 4) = pPlayer;
	}
	*(int*) (pManagerBytes + 0x110) = *(int*) (g_pLINKSCF_PlasLevelMode + 0xf8);
	*(int*) (pManagerBytes + 0x114) = 0;
	*(int*) (pManagerBytes + 0x118) = 0;
	*(int*) (pManagerBytes + 0x11c) = 0;
}
// FUNCTION: LEMBALL 0x0041af60
LinkScfGmobChunkManagerView* LinkScfGmobChunkManagerView::ConstructGmobChunkManager(void* pLevelMode, int nCapacity)
{
	*(void**) this = g_GAME_EffStreamConstructionVtable;
	*(int*) ((char*) this + 4) = 0x0c;
	((GameEffStream*) this)->ResetStateFields();
	*(void**) this = g_LEVEL_EffChunkStreamBaseVtable;
	*(int*) ((char*) this + 0x2c) = 1;
	if (g_pActiveNetworkRuntimeWindow != 0) {
		*(int*) ((char*) this + 0x24) = 1;
		*(int*) ((char*) this + 0x18) += g_cbEffTransportMaxPacketBytes;
	}
	*(void**) this = g_LINKSCF_GmobChunkManagerVtable;
	g_pLINKSCF_GmobChunkManager = this;
	*(void**) ((char*) this + 0x30) = pLevelMode;
	*(short*) ((char*) this + 0x36) = 0;
	*(int*) ((char*) this + 0x3c) = 0;
	*(short*) ((char*) this + 0x38) = 0;
	*(short*) ((char*) this + 0x34) = 0;
	g_nLINKSCF_GmobObjectCount = 0;
	return this;
}

// FUNCTION: LEMBALL 0x0041e8f0
LinkScfCachedChunkObjectManagerBaseView* LinkScfCachedChunkObjectManagerBaseView::ConstructCachedChunkObjectManagerBase(
	void* pLevelMode,
	void* pEntityLookup,
	void* pProjectileGeometryTable)
{
	*(void**) this = g_LINKSCF_CachedChunkObjectManagerBaseVtable;
	g_pCachedChunkManagerLevelMode = pLevelMode;
	g_pCachedChunkManagerEntityLookup = pEntityLookup;
	g_pCachedChunkManagerProjectileGeometryTable = pProjectileGeometryTable;
	*(int*) ((char*) this + 0xa4) = 0;
	*(int*) ((char*) this + 0xa8) = 0;
	*(int*) ((char*) this + 0xac) = 1;
	memset((char*) this + 4, 0, 40 * sizeof(int));
	return this;
}
// FUNCTION: LEMBALL 0x0041ea20
int LEMBALL_FASTCALL GetManagedEntityGroupCount(void* pObject)
{
	return *(int*) ((char*) pObject + 0xa4);
}
// FUNCTION: LEMBALL 0x0041ea30
void* LEMBALL_FASTCALL GetFirstCachedChunkObject(void* pObject)
{
	*(int*) ((char*) pObject + 0xa8) = 0;
	if (*(int*) ((char*) pObject + 0xa4) == 0) {
		return 0;
	}
	return *(void**) ((char*) pObject + 4);
}
// FUNCTION: LEMBALL 0x0041ea50
void* LEMBALL_FASTCALL GetNextCachedChunkObject(void* pObject)
{
	int nIndex;

	nIndex = *(int*) ((char*) pObject + 0xa8) + 1;
	*(int*) ((char*) pObject + 0xa8) = nIndex;
	if (*(int*) ((char*) pObject + 0xa4) <= nIndex) {
		return 0;
	}
	return *(void**) ((char*) pObject + 4 + nIndex * 4);
}
// FUNCTION: LEMBALL 0x0041ea70
void* LEMBALL_FASTCALL GetCachedChunkObjectByIndex(void* pObject, void*, int nIndex)
{
	*(int*) ((char*) pObject + 0xa8) = nIndex;
	if (*(int*) ((char*) pObject + 0xa4) <= nIndex) {
		return 0;
	}
	return *(void**) ((char*) pObject + 4 + nIndex * 4);
}
// FUNCTION: LEMBALL 0x0041ea90
void* LEMBALL_FASTCALL GetNextManagedEntityGroup(void* pObject)
{
	int nIndex;

	nIndex = *(int*) ((char*) pObject + 0xa8);
	if (*(int*) ((char*) pObject + 0xa4) <= nIndex) {
		return 0;
	}
	return *(void**) ((char*) pObject + 4 + nIndex * 4);
}
// FUNCTION: LEMBALL 0x0041ec80
void LEMBALL_FASTCALL AppendCachedChunkObject(void* pObject, void*, void* pChunkObject)
{
	int nIndex;

	nIndex = *(int*) ((char*) pObject + 0xa4);
	if (nIndex < 40) {
		*(void**) ((char*) pObject + 4 + nIndex * 4) = pChunkObject;
		++*(int*) ((char*) pObject + 0xa4);
	}
}

// FUNCTION: LEMBALL 0x0041eca0
void ResetCachedChunkManagerScratch(void)
{
}

// FUNCTION: LEMBALL 0x0041ee90
int LEMBALL_FASTCALL EmitCachedChunkManagerRenderEntries(void* pObject, void*, char* pEntries)
{
	typedef void*(LEMBALL_FASTCALL * CachedChunkIteratorProc)(void*, void*);
	typedef int(LEMBALL_FASTCALL * CachedChunkRenderProc)(void*, void*, char*);

	void** pManagerVtable;
	CachedChunkIteratorProc pGetNext;
	void* pChunkObject;
	int cEntries;

	cEntries = 0;
	pManagerVtable = *(void***) pObject;
	pChunkObject = ((CachedChunkIteratorProc) pManagerVtable[1])(pObject, 0);
	if (pChunkObject != 0) {
		pGetNext = (CachedChunkIteratorProc) pManagerVtable[2];
		do {
			cEntries +=
				((CachedChunkRenderProc) (*(void***) pChunkObject)[86])(pChunkObject, 0, pEntries + cEntries * 0x4c);
			pChunkObject = pGetNext(pObject, 0);
		} while (pChunkObject != 0);
	}
	return cEntries;
}
// FUNCTION: LEMBALL 0x0041f0b0
LinkScfShpgChunkManagerView* LinkScfShpgChunkManagerView::ConstructShpgChunkManager(void* pLevelMode,
																					void* pGmobManager,
																					void* pProjectileGeometryTable)
{
	((LinkScfCachedChunkObjectManagerBaseView*) this)
		->ConstructCachedChunkObjectManagerBaseThunk(pLevelMode, pGmobManager, pProjectileGeometryTable);
	*(void**) this = g_LINKSCF_ShpgChunkManagerVtable;
	return this;
}

// FUNCTION: LEMBALL 0x00420b80
LinkScfEnmyChunkManagerView* LinkScfEnmyChunkManagerView::ConstructEnmyChunkManager(void* pLevelMode,
																					void* pGmobManager,
																					void* pProjectileGeometryTable)
{
	((LinkScfCachedChunkObjectManagerBaseView*) this)
		->ConstructCachedChunkObjectManagerBaseThunk(pLevelMode, pGmobManager, pProjectileGeometryTable);
	*(void**) this = g_LINKSCF_EnmyChunkManagerVtable;
	return this;
}
// FUNCTION: LEMBALL 0x004223c0
LinkScfCollChunkManagerView* LinkScfCollChunkManagerView::ConstructCollChunkManager(void* pLevelMode, int nCapacity)
{
	LINKSCF_INITIALIZE_EFF_CHUNK_STREAM(this, 0x11, 6);
	*(void**) this = g_LINKSCF_CollChunkManagerVtable;
	*(void**) ((char*) this + 0x30) = pLevelMode;
	*(int*) ((char*) this + 0x3c) = 0;
	*(int*) ((char*) this + 0x38) = nCapacity;
	*(int*) ((char*) this + 0x34) = 0;
	return this;
}
// FUNCTION: LEMBALL 0x00424020
LinkScfMineChunkManagerView* LinkScfMineChunkManagerView::ConstructMineChunkManager(void* pLevelMode, int nCapacity)
{
	LINKSCF_INITIALIZE_EFF_CHUNK_STREAM(this, 0x0d, 2);
	*(void**) this = g_LINKSCF_MineChunkManagerVtable;
	*(void**) ((char*) this + 0x30) = pLevelMode;
	*(int*) ((char*) this + 0x40) = nCapacity;
	*(int*) ((char*) this + 0x34) = 0;
	*(int*) ((char*) this + 0x38) = 0;
	return this;
}

extern void* g_LINKSCF_LiftChunkObjectVtable[16];

// FUNCTION: LEMBALL 0x00424d20
void LEMBALL_FASTCALL destroy_lift_chunk_object(void* pObject)
{
	*(void**) pObject = g_LINKSCF_LiftChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
}

// FUNCTION: LEMBALL 0x00425680
LinkScfLiftChunkManagerView* LinkScfLiftChunkManagerView::ConstructLiftChunkManager(void* pLevelMode, int nCapacity)
{
	LINKSCF_INITIALIZE_EFF_CHUNK_STREAM(this, 0x12, 7);
	*(void**) this = g_LINKSCF_LiftChunkManagerVtable;
	*(void**) ((char*) this + 0x30) = pLevelMode;
	*(int*) ((char*) this + 0x38) = nCapacity;
	*(int*) ((char*) this + 0x3c) = 0;
	return this;
}

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);

struct LiftChunkEndpointPoint {
	short m_x;
	short m_y;
	short m_z;
};

struct LiftChunkEndpointPair {
	LiftChunkEndpointPoint m_Start;
	LiftChunkEndpointPoint m_End;
};

struct LiftChunkManagerSerializationView {
	int SerializeLiftChunkEndpointPairs(LiftChunkEndpointPair* pPairs);
	unsigned short GetLiftChunkObjectSlotIdByIndex(int iObject);
};

struct LiftManagedEntitySlotView {
	inline unsigned short GetManagedEntitySlotId(void)
	{
		return GetManagedEntitySlotIdThunk((int) (unsigned long) this);
	}
};

// FUNCTION: LEMBALL 0x00425830
int LiftChunkManagerSerializationView::SerializeLiftChunkEndpointPairs(LiftChunkEndpointPair* pPairs)
{
	int nObjectOffset;
	int i;
	char* pObject;

	nObjectOffset = 0;
	i = 0;
	if (*(int*) ((char*) this + 0x34) > 0) {
		do {
			pObject = *(char**) ((char*) this + 0x3c) + nObjectOffset;
			++i;
			nObjectOffset += 0x190;
			pPairs->m_Start = *(LiftChunkEndpointPoint*) (pObject + 0x13a);
			pPairs->m_End = *(LiftChunkEndpointPoint*) (pObject + 0x140);
			++pPairs;
		} while (i < *(int*) ((char*) this + 0x34));
	}
	return *(int*) ((char*) this + 0x34);
}

// FUNCTION: LEMBALL 0x00425f90
unsigned short LiftChunkManagerSerializationView::GetLiftChunkObjectSlotIdByIndex(int iObject)
{
	if (*(int*) ((char*) this + 0x34) <= iObject) {
		return 0xffff;
	}
	return ((LiftManagedEntitySlotView*) (*(char**) ((char*) this + 0x3c) + iObject * 0x190))->GetManagedEntitySlotId();
}

extern void* g_LINKSCF_RockChunkObjectVtable[16];

// FUNCTION: LEMBALL 0x004267d0
RockChunkObjectView* RockChunkObjectView::ConstructRockChunkObject(void)
{
	InitializeLevelChunkObjectBase(0x20, 0, 0);
	*(void**) this = g_LINKSCF_RockChunkObjectVtable;
	return this;
}

// FUNCTION: LEMBALL 0x00426830
void LEMBALL_FASTCALL destroy_rock_chunk_object(void* pObject)
{
	*(void**) pObject = g_LINKSCF_RockChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
}

// FUNCTION: LEMBALL 0x00426ac0
LinkScfRockChunkManagerView* LinkScfRockChunkManagerView::ConstructRockChunkManager(void* pLevelMode, int nCapacity)
{
	LINKSCF_INITIALIZE_EFF_CHUNK_STREAM(this, 0x14, 9);
	*(void**) this = g_LINKSCF_RockChunkManagerVtable;
	*(void**) ((char*) this + 0x3c) = pLevelMode;
	*(int*) ((char*) this + 0x30) = nCapacity;
	*(int*) ((char*) this + 0x38) = 0;
	return this;
}

// FUNCTION: LEMBALL 0x00426b20
void LEMBALL_FASTCALL ActivateRockChunkObjects(void* pManager)
{
	char* pManagerBytes;
	void* pChunkObject;
	int i;
	int nOffset;

	pManagerBytes = (char*) pManager;
	nOffset = 0;
	if (*(char**) (pManagerBytes + 0x38) != 0) {
		if (*(int*) (pManagerBytes + 0x30) > 0) {
			i = 0;
			do {
				pChunkObject = *(char**) (pManagerBytes + 0x38) + nOffset;
				++i;
				nOffset += 0x144;
				((ActivateChunkObjectProc) (*(void***) pChunkObject)[65])(pChunkObject);
			} while (*(int*) (pManagerBytes + 0x30) > i);
		}
	}
}

extern void* g_LINKSCF_CaptureChunkObjectVtable[16];

// FUNCTION: LEMBALL 0x00427ad0
CaptureTriggerChunkObjectView* CaptureTriggerChunkObjectView::ConstructCaptureTriggerChunkObject(void)
{
	InitializeLevelChunkObjectBase(0x1f, 0, 0);
	*(void**) this = g_LINKSCF_CaptureChunkObjectVtable;
	return this;
}

// FUNCTION: LEMBALL 0x00427b40
void LEMBALL_FASTCALL destroy_capture_trigger_chunk_object(void* pObject)
{
	*(void**) pObject = g_LINKSCF_CaptureChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
}

// FUNCTION: LEMBALL 0x00427e60
LinkScfCaptureChunkManagerView* LinkScfCaptureChunkManagerView::ConstructCaptureTriggerChunkManager(void* pLevelMode,
																									int nCapacity)
{
	LINKSCF_INITIALIZE_EFF_CHUNK_STREAM(this, 0x16, 0x0b);
	*(void**) this = g_LINKSCF_CaptureChunkManagerVtable;
	*(void**) ((char*) this + 0x3c) = pLevelMode;
	*(int*) ((char*) this + 0x30) = nCapacity;
	*(int*) ((char*) this + 0x38) = 0;
	return this;
}

// FUNCTION: LEMBALL 0x00427ec0
void LEMBALL_FASTCALL ActivateCaptureTriggerChunkObjects(void* pManager)
{
	char* pManagerBytes;
	void* pChunkObject;
	int i;
	int nOffset;

	pManagerBytes = (char*) pManager;
	nOffset = 0;
	if (*(char**) (pManagerBytes + 0x38) != 0) {
		if (*(int*) (pManagerBytes + 0x30) > 0) {
			i = 0;
			do {
				pChunkObject = *(char**) (pManagerBytes + 0x38) + nOffset;
				++i;
				nOffset += 0x144;
				((ActivateChunkObjectProc) (*(void***) pChunkObject)[65])(pChunkObject);
			} while (*(int*) (pManagerBytes + 0x30) > i);
		}
	}
}
// FUNCTION: LEMBALL 0x00427fc0
void LEMBALL_FASTCALL ClearManagedEntityArrayCount(void* pObject)
{
	*(int*) ((char*) pObject + 0x34) = 0;
}

extern void* g_LINKSCF_LasrChunkObjectVtable[16];

// FUNCTION: LEMBALL 0x00428890
LasrChunkObjectView* LasrChunkObjectView::ConstructLasrChunkObject(void)
{
	InitializeLevelChunkObjectBase(0x2f, 0, 0);
	*(void**) this = g_LINKSCF_LasrChunkObjectVtable;
	return this;
}

// FUNCTION: LEMBALL 0x004288f0
void LEMBALL_FASTCALL destroy_lasr_chunk_object_vtable_thunk(void* pObject)
{
	*(void**) pObject = g_LINKSCF_LasrChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
}

struct LevelTileRenderDescriptor {
	short SampleHeightOffset(int nLocalX, int nLocalY);
};

typedef int(LEMBALL_FASTCALL* LinkScfPointSlotProc)(void*, void*, const int*);
typedef void(LEMBALL_FASTCALL* LinkScfIntSlotProc)(void*, void*, int);

// FUNCTION: LEMBALL 0x00428ab0
int LEMBALL_FASTCALL AcquireLasrPlasTarget(void* pObject)
{
	int* pFields = (int*) pObject;
	int nX = pFields[0x27] >> 12;
	int nY = pFields[0x28] >> 12;
	int nZ = pFields[0x29] >> 12;
	int nStepX;
	int nStepY;
	int nSteps;
	void* pCandidate = 0;

	switch (pFields[0x19]) {
	case 0x1e:
	case 0x30:
		nStepX = 0x10;
		nStepY = 0;
		nX += 8;
		break;
	case 0x2f:
	case 0x31:
		nStepX = 0;
		nStepY = 0x10;
		nY += 8;
		break;
	default:
		return 0;
	}

	for (nSteps = 0; nSteps < 8; ++nSteps) {
		char* pGrid;
		char* pOwner;
		unsigned short nTerrain;

		nX += nStepX;
		nY += nStepY;
		pGrid = (char*) g_pLevelTileGrid;
		if (nX < 0 || nY < 0 || (nX >> 4) >= *(int*) (pGrid + 0x10) || (nY >> 4) >= *(int*) (pGrid + 0x14)) {
			nTerrain = 0;
		}
		else {
			LevelTileRenderDescriptor* pTile =
				(LevelTileRenderDescriptor*) (*(char**) (pGrid + 0x0c) +
											  (((nY >> 4) * *(int*) (pGrid + 0x10)) + (nX >> 4)) * 12);
			nTerrain = (unsigned short) pTile->SampleHeightOffset(nX & 15, nY & 15);
		}
		if ((int) nTerrain > nZ) {
			break;
		}

		pOwner = (char*) g_pActiveManagedEntityOwner;
		*(void**) (pOwner + 0x150) = 0;
		*(int*) (pOwner + 0x124) = nX;
		*(int*) (pOwner + 0x128) = nY;
		*(int*) (pOwner + 0x12c) = nZ;
		*(int*) (pOwner + 0x130) = 0;
		pCandidate = 0;
		while (*(int*) (pOwner + 0x130) < *(int*) (pOwner + 0x118)) {
			pCandidate = (*(void***) (pOwner + 0x120))[*(int*) (pOwner + 0x130)];
			if (pCandidate != *(void**) (pOwner + 0x150) &&
				((LinkScfPointSlotProc) (*(void***) pCandidate)[20])(pCandidate, 0, (int*) (pOwner + 0x124)) != 0) {
				++*(int*) (pOwner + 0x130);
				break;
			}
			pCandidate = 0;
			++*(int*) (pOwner + 0x130);
		}
		if (pCandidate != 0 && *(int*) ((char*) pCandidate + 0x64) == 2) {
			break;
		}
		pCandidate = 0;
	}

	if (pCandidate == 0) {
		return 0;
	}
	*(void**) ((char*) pObject + 0x144) = pCandidate;
	*(int*) ((char*) pCandidate + 0xb8) = 0x0f;
	*(short*) ((char*) pCandidate + 0xbc) = 1;
	*(int*) ((char*) pCandidate + 0xcc) = g_nLevelFrameClockTick + 0x1a;
	((LinkScfIntSlotProc) (*(void***) pObject)[13])(pObject, 0, 0x22);
	return 1;
}

// FUNCTION: LEMBALL 0x00428ec0
int LEMBALL_FASTCALL BeginLasrPlasReacquireDelay(void* pObject)
{
	int* pFields = (int*) pObject;
	pFields[0x4e] = 1;
	if (g_nSelectedNetworkLobbyPeerId != 0 && *(int*) (g_nSelectedNetworkLobbyPeerId + 0x1c) != 0) {
		return 0;
	}
	pFields[0x32] = g_nLevelFrameClockTick;
	pFields[0x34] = g_nLevelFrameClockTick + 6;
	pFields[0x33] = g_nLevelFrameClockTick + 0x18;
	*(void**) ((char*) pObject + 0x144) = 0;
	pFields[0x25] = g_nLevelFrameClockTimeMs;
	((LinkScfIntSlotProc) (*(void***) pObject)[2])(pObject, 0, 0x19);
	return 1;
}

// FUNCTION: LEMBALL 0x00429320
LinkScfLasrChunkManagerView* LinkScfLasrChunkManagerView::ConstructLasrChunkManager(void* pLevelMode, int nCapacity)
{
	LINKSCF_INITIALIZE_EFF_CHUNK_STREAM(this, 0x15, 10);
	*(void**) this = g_LINKSCF_LasrChunkManagerVtable;
	*(void**) ((char*) this + 0x3c) = pLevelMode;
	*(int*) ((char*) this + 0x30) = nCapacity;
	*(int*) ((char*) this + 0x38) = 0;
	return this;
}

// FUNCTION: LEMBALL 0x00429380
void LEMBALL_FASTCALL ActivateLasrChunkObjects(void* pManager)
{
	char* pManagerBytes;
	void* pChunkObject;
	int i;
	int nOffset;

	pManagerBytes = (char*) pManager;
	nOffset = 0;
	if (*(char**) (pManagerBytes + 0x38) != 0) {
		if (*(int*) (pManagerBytes + 0x30) > 0) {
			i = 0;
			do {
				pChunkObject = *(char**) (pManagerBytes + 0x38) + nOffset;
				++i;
				nOffset += 0x148;
				((ActivateChunkObjectProc) (*(void***) pChunkObject)[65])(pChunkObject);
			} while (*(int*) (pManagerBytes + 0x30) > i);
		}
	}
}

// FUNCTION: LEMBALL 0x004297e0
void LEMBALL_FASTCALL reset_lasr_chunk_object_count(void* pManager)
{
	*(int*) ((char*) pManager + 0x34) = 0;
}

// FUNCTION: LEMBALL 0x00429f50
LinkScfBoonChunkManagerView* LinkScfBoonChunkManagerView::ConstructBoonChunkManager(void* pLevelMode, void* pTileGrid)
{
	void* pObject;
	int i;
	int j;

	pObject = this;
	for (i = 0; i < 4; ++i) {
		int* pPoint = (int*) ((char*) pObject + 4 + i * 0x0c);
		pPoint[0] = 0xaa55aa55;
		pPoint[1] = 0xaa55aa55;
		pPoint[2] = 0xaa55aa55;
	}
	*(void**) ((char*) pObject + 0x44) = pLevelMode;
	*(void**) ((char*) pObject + 0x48) = pTileGrid;
	g_LINKSCF_BoonChunkObjectVtable[65] = (void*) noop_vtable_callback_0040a990;
	for (i = 0; i < 4; ++i) {
		void* pChild = AllocateVSMemBlock(0x128);
		if (pChild != 0) {
			for (j = 0; j < 0x128 / 4; ++j) {
				((int*) pChild)[j] = 0;
			}
			*(void**) pChild = g_LINKSCF_BoonChunkObjectVtable;
			*(int*) ((char*) pChild + 4) = 0x28 + i * 2;
		}
		*(void**) ((char*) pObject + 0x34 + i * 4) = pChild;
	}
	return this;
}
// FUNCTION: LEMBALL 0x0042a070
void LinkScfBoonChunkManagerView::DestroyBoonChunkManagerChildren(void)
{
	void** ppChildren;
	int i;

	ppChildren = (void**) ((char*) this + 0x34);
	for (i = 0; i < 4; ++i) {
		if (ppChildren[i] != 0) {
			((void(LEMBALL_FASTCALL*)(void*, int, unsigned char))(*(void***) ppChildren[i])[0])(ppChildren[i], 0, 1);
		}
	}
}

// FUNCTION: LEMBALL 0x0042a7a0
void* LEMBALL_FASTCALL destroy_lasr_chunk_object(void* pObject, void*, BYTE fDelete)
{
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

extern void* g_LINKSCF_TramChunkObjectVtable[16];

// FUNCTION: LEMBALL 0x0042a990
TramChunkObjectView* TramChunkObjectView::ConstructTramChunkObject(void)
{
	InitializeLevelChunkObjectBase(0x22, 0, 0);
	*(void**) this = g_LINKSCF_TramChunkObjectVtable;
	return this;
}

// FUNCTION: LEMBALL 0x0042a9d0
void LEMBALL_FASTCALL destroy_tram_chunk_object(void* pObject)
{
	*(void**) pObject = g_LINKSCF_TramChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
}

// FUNCTION: LEMBALL 0x0042af60
LinkScfTramChunkManagerView* LinkScfTramChunkManagerView::ConstructTramChunkManager(void* pLevelMode, int nCapacity)
{
	LINKSCF_INITIALIZE_EFF_CHUNK_STREAM(this, 0x18, 0x0d);
	*(void**) this = g_LINKSCF_TramChunkManagerVtable;
	*(void**) ((char*) this + 0x3c) = pLevelMode;
	*(int*) ((char*) this + 0x30) = nCapacity;
	*(int*) ((char*) this + 0x38) = 0;
	return this;
}

// FUNCTION: LEMBALL 0x0042b0b0
void LEMBALL_FASTCALL clear_tram_chunk_object_count(void* pManager)
{
	*(int*) ((char*) pManager + 0x34) = 0;
}

// FUNCTION: LEMBALL 0x0042b4a0
void LEMBALL_FASTCALL ActivateTramChunkObjects(void* pManager)
{
	char* pManagerBytes;
	char* pChunkObject;
	int nOffset;
	int i;

	pManagerBytes = (char*) pManager;
	i = 0;
	if (*(int*) (pManagerBytes + 0x34) > 0) {
		nOffset = 0;
		do {
			*(int*) (*(char**) (pManagerBytes + 0x38) + nOffset + 0x124) = 1;
			pChunkObject = *(char**) (pManagerBytes + 0x38) + nOffset;
			if (*(int*) (pChunkObject + 0x138) != 0) {
				((ActivateChunkObjectProc) (*(void***) pChunkObject)[5])(pChunkObject);
			}
			nOffset += 0x144;
			++i;
		} while (*(int*) (pManagerBytes + 0x34) > i);
	}
}

// FUNCTION: LEMBALL 0x0042b9b0
void noop_tram_runtime_callback(void)
{
}

extern void* g_LINKSCF_PgunChunkObjectVtable[16];

// FUNCTION: LEMBALL 0x0042bad0
PgunChunkObjectView* PgunChunkObjectView::ConstructPgunChunkObject(void)
{
	InitializeLevelChunkObjectBase(0x21, 0, 0);
	*(void**) this = g_LINKSCF_PgunChunkObjectVtable;
	return this;
}

// FUNCTION: LEMBALL 0x0042bb20
void LEMBALL_FASTCALL destroy_pgun_chunk_object_vtable_thunk(void* pObject)
{
	*(void**) pObject = g_LINKSCF_PgunChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
}

// FUNCTION: LEMBALL 0x0042bfc0
int PASCAL return_zero_pgun_chunk_callback_a(void* pUnused1, void* pUnused2)
{
	(void) pUnused1;
	(void) pUnused2;
	return 0;
}

// FUNCTION: LEMBALL 0x0042bfd0
int PASCAL return_zero_pgun_chunk_object_callback(void* pUnused1, void* pUnused2)
{
	(void) pUnused1;
	(void) pUnused2;
	return 0;
}

// FUNCTION: LEMBALL 0x0042bfe0
LinkScfPgunChunkManagerView* LinkScfPgunChunkManagerView::ConstructPgunChunkManager(void* pLevelMode, int nCapacity)
{
	LINKSCF_INITIALIZE_EFF_CHUNK_STREAM(this, 0x1f, 0x14);
	*(void**) this = g_LINKSCF_PgunChunkManagerVtable;
	*(void**) ((char*) this + 0x3c) = pLevelMode;
	*(int*) ((char*) this + 0x30) = nCapacity;
	*(int*) ((char*) this + 0x38) = 0;
	return this;
}

// FUNCTION: LEMBALL 0x0042c040
void LEMBALL_FASTCALL ActivatePgunChunkObjects(void* pManager)
{
	char* pManagerBytes;
	void* pChunkObject;
	int i;
	int nOffset;

	pManagerBytes = (char*) pManager;
	nOffset = 0;
	if (*(char**) (pManagerBytes + 0x38) != 0) {
		if (*(int*) (pManagerBytes + 0x30) > 0) {
			i = 0;
			do {
				pChunkObject = *(char**) (pManagerBytes + 0x38) + nOffset;
				++i;
				nOffset += 0x144;
				((ActivateChunkObjectProc) (*(void***) pChunkObject)[65])(pChunkObject);
			} while (*(int*) (pManagerBytes + 0x30) > i);
		}
	}
}

// FUNCTION: LEMBALL 0x0042c140
void LEMBALL_FASTCALL clear_pgun_chunk_object_count(void* pManager)
{
	*(int*) ((char*) pManager + 0x34) = 0;
}

// FUNCTION: LEMBALL 0x0042c4c0
int PASCAL return_zero_pgun_chunk_callback_b(void* pUnused1, void* pUnused2)
{
	(void) pUnused1;
	(void) pUnused2;
	return 0;
}

// FUNCTION: LEMBALL 0x0042c580
int PASCAL return_zero_pgun_chunk_manager_callback(void* pUnused1, void* pUnused2)
{
	(void) pUnused1;
	(void) pUnused2;
	return 0;
}

extern void* g_LINKSCF_IceChunkObjectVtable[16];

// FUNCTION: LEMBALL 0x0042ca70
IceChunkObjectView* IceChunkObjectView::ConstructIceChunkObject(void)
{
	InitializeLevelChunkObjectBase(0x33, 0, 0);
	*(void**) this = g_LINKSCF_IceChunkObjectVtable;
	return this;
}

// FUNCTION: LEMBALL 0x0042caf0
void LEMBALL_FASTCALL destroy_ice_chunk_object(void* pObject)
{
	*(void**) pObject = g_LINKSCF_IceChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
}

// FUNCTION: LEMBALL 0x0042d7a0
LinkScfIceChunkManagerView* LinkScfIceChunkManagerView::ConstructIceChunkManager(void* pLevelMode, int nCapacity)
{
	LINKSCF_INITIALIZE_EFF_CHUNK_STREAM(this, 0x19, 0x0e);
	*(void**) this = g_LINKSCF_IceChunkManagerVtable;
	*(void**) ((char*) this + 0x3c) = pLevelMode;
	*(int*) ((char*) this + 0x30) = nCapacity;
	*(int*) ((char*) this + 0x38) = 0;
	return this;
}

// FUNCTION: LEMBALL 0x0042d800
void LEMBALL_FASTCALL ActivateIceChunkObjects(void* pManager)
{
	char* pManagerBytes;
	void* pChunkObject;
	int i;
	int nOffset;

	pManagerBytes = (char*) pManager;
	nOffset = 0;
	if (*(char**) (pManagerBytes + 0x38) != 0) {
		if (*(int*) (pManagerBytes + 0x30) > 0) {
			i = 0;
			do {
				pChunkObject = *(char**) (pManagerBytes + 0x38) + nOffset;
				++i;
				nOffset += 0x188;
				((ActivateChunkObjectProc) (*(void***) pChunkObject)[65])(pChunkObject);
			} while (*(int*) (pManagerBytes + 0x30) > i);
		}
	}
}

// FUNCTION: LEMBALL 0x0042dd00
void LEMBALL_FASTCALL AdvanceIceChunkManagerObjects(void* pManager)
{
	char* pManagerBytes;
	void* pChunkObject;
	int i;
	int nOffset;

	pManagerBytes = (char*) pManager;
	nOffset = 0;
	i = 0;
	if (*(int*) (pManagerBytes + 0x34) > 0) {
		do {
			pChunkObject = *(char**) (pManagerBytes + 0x38) + nOffset;
			++i;
			nOffset += 0x188;
			((ActivateChunkObjectProc) (*(void***) pChunkObject)[5])(pChunkObject);
		} while (*(int*) (pManagerBytes + 0x34) > i);
	}
}

// FUNCTION: LEMBALL 0x0042eee0
int LEMBALL_FASTCALL is_point_inside_move_chunk_bounds(void* pObject, void* pUnused, const int* pPoint)
{
	int nMinX = (*(int*) ((char*) pObject + 0x9c) >> 12) - 8;
	int nMinY = (*(int*) ((char*) pObject + 0xa0) >> 12) - 8;
	int nX = pPoint[0] >> 12;
	int nY = pPoint[1] >> 12;
	(void) pUnused;
	return nMinX <= nX && nX <= nMinX + 15 && nMinY <= nY && nY <= nMinY + 15;
}

// FUNCTION: LEMBALL 0x0042ef40
void LEMBALL_FASTCALL detach_entities_outside_move_chunk_bounds(void* pObject)
{
	char* pBytes = (char*) pObject;
	int nMinX = (*(int*) (pBytes + 0x9c) >> 12) - 8;
	int nMinY = (*(int*) (pBytes + 0xa0) >> 12) - 8;
	int i = 0;

	while (i < *(int*) (pBytes + 0x174)) {
		char* pEntity = *(char**) (pBytes + 0x178 + i * 4);
		int nX = *(int*) (pEntity + 0x9c) >> 12;
		int nY = *(int*) (pEntity + 0xa0) >> 12;
		if (nX < nMinX || nMinX + 15 < nX || nY < nMinY || nMinY + 15 < nY) {
			int j;
			*(int*) (pEntity + 0x11c) = 0;
			for (j = i + 1; j < *(int*) (pBytes + 0x174); ++j) {
				*(void**) (pBytes + 0x178 + (j - 1) * 4) = *(void**) (pBytes + 0x178 + j * 4);
			}
			--i;
			--*(int*) (pBytes + 0x174);
		}
		++i;
	}
}

extern void* LEMBALL_FASTCALL get_managed_entity_owner_group(void* pEntity);
extern void LEMBALL_FASTCALL clear_managed_entity_child_pending_state_if_interruptible(void* pEntity);

// FUNCTION: LEMBALL 0x0042f140
void LEMBALL_FASTCALL release_attached_move_chunk_entities(void* pObject)
{
	typedef void(LEMBALL_FASTCALL * OwnerResetSlotProc)(void*);
	char* pBytes = (char*) pObject;
	int i;

	for (i = 0; i < *(int*) (pBytes + 0x174); ++i) {
		void* pEntity = *(void**) (pBytes + 0x178 + i * 4);
		if (*(int*) ((char*) pEntity + 0x64) == 2) {
			void* pOwnerGroup = get_managed_entity_owner_group(pEntity);
			((OwnerResetSlotProc) (*(void***) pOwnerGroup)[84])(pOwnerGroup);
		}
		else {
			clear_managed_entity_child_pending_state_if_interruptible(pEntity);
		}
	}
}

// FUNCTION: LEMBALL 0x0042f190
LinkScfMoveChunkManagerView* LinkScfMoveChunkManagerView::ConstructMoveChunkManager(void* pLevelMode, int nCapacity)
{
	LINKSCF_INITIALIZE_EFF_CHUNK_STREAM(this, 0x1a, 0x0f);
	*(void**) this = g_LINKSCF_MoveChunkManagerVtable;
	*(void**) ((char*) this + 0x3c) = pLevelMode;
	*(int*) ((char*) this + 0x30) = nCapacity;
	*(int*) ((char*) this + 0x38) = 0;
	return this;
}

// FUNCTION: LEMBALL 0x0042f1f0
void LEMBALL_FASTCALL ActivateMoveChunkObjects(void* pManager)
{
	char* pManagerBytes;
	void* pChunkObject;
	int nOffset;
	int i;

	nOffset = 0;
	if (*(char**) ((char*) pManager + 0x38) != 0) {
		pManagerBytes = (char*) pManager;
		i = 0;
		if (*(int*) (pManagerBytes + 0x30) > 0) {
			do {
				pChunkObject = *(char**) (pManagerBytes + 0x38) + nOffset;
				++i;
				nOffset += 0x1a0;
				((ActivateChunkObjectProc) (*(void***) pChunkObject)[65])(pChunkObject);
			} while (*(int*) (pManagerBytes + 0x30) > i);
		}
	}
}

// FUNCTION: LEMBALL 0x0042f500
void LEMBALL_FASTCALL AdvanceActiveMoveChunkObjects(void* pManager)
{
	char* pManagerBytes;
	char* pChunkObject;
	int nOffset;
	int i;

	i = 0;
	pManagerBytes = (char*) pManager;
	if (*(int*) (pManagerBytes + 0x34) > 0) {
		nOffset = 0;
		do {
			pChunkObject = *(char**) (pManagerBytes + 0x38) + nOffset;
			if (*(int*) (pChunkObject + 0x138) != 0) {
				((ActivateChunkObjectProc) (*(void***) pChunkObject)[5])(pChunkObject);
			}
			nOffset += 0x1a0;
			++i;
		} while (*(int*) (pManagerBytes + 0x34) > i);
	}
}

void LEMBALL_FASTCALL ReadEffStreamU16ToField30(void* pObject)
{
	*(unsigned short*) ((char*) pObject + 0x30) = ((VsNetEffStreamBase*) pObject)->ReadEffStreamU16BEValue();
}

int LEMBALL_FASTCALL GetIndexedLevelModeValue110(void* pObject, int, int nIndex)
{
	return *(int*) ((char*) pObject + 0x110 + nIndex * 4);
}

void add_two_dword_values(int* pDestination, const int* pLeft, const int* pRight)
{
	*pDestination = *pRight + *pLeft;
}

struct LinkScfSmallFunctionView {
	void* GetLevelTileGridCellPointer(int x, int y);
	void ClearManagedEntityGroupChildActiveFlag(int iChild);
	void* CopyFourDwordBlock(const int* pSource);
	void ResetRockChunkObjectRuntimeState(void);
	void ResetCaptureTriggerChunkObjectRuntimeState(void);
	void ResetLasrChunkObjectRuntimeState(void);
	void CopyFixedPointPointToPixelPoint(const int* pSource);
	void SetObjectFields18_1c_20(int nField18, int nField1c, int nField20);
	void* InitializeThreeDwordPointFromValues(const int* pX, const int* pY, const int* pZ);
};

void* LinkScfSmallFunctionView::GetLevelTileGridCellPointer(int x, int y)
{
	return (char*) *(void**) ((char*) this + 4) + (x + *(int*) ((char*) this + 8) * y) * 12;
}

void LinkScfSmallFunctionView::ClearManagedEntityGroupChildActiveFlag(int iChild)
{
	void** ppChildren;
	void* pChild;

	ppChildren = *(void***) ((char*) this + 0x3c);
	pChild = ppChildren[iChild];
	*(int*) ((char*) pChild + 0x38) = 0;
}

unsigned int ReadLittleEndianU32FromBytes(const unsigned char* pBytes)
{
	unsigned int nValue;

	nValue = pBytes[3];
	nValue <<= 16;
	nValue |= pBytes[1];
	nValue <<= 8;
	nValue |= (unsigned int) pBytes[2] << 16;
	nValue |= pBytes[0];
	return nValue;
}

void* LinkScfSmallFunctionView::CopyFourDwordBlock(const int* pSource)
{
	*(int*) this = pSource[0];
	*(int*) ((char*) this + 4) = pSource[1];
	*(int*) ((char*) this + 8) = pSource[2];
	*(int*) ((char*) this + 0x0c) = pSource[3];
	return this;
}

void LinkScfSmallFunctionView::ResetRockChunkObjectRuntimeState(void)
{
	int nZero;

	nZero = 0;
	*(int*) ((char*) this + 0x94) = nZero;
	*(int*) ((char*) this + 0x138) = nZero;
	*(int*) ((char*) this + 0x13c) = nZero;
}

void LinkScfSmallFunctionView::ResetCaptureTriggerChunkObjectRuntimeState(void)
{
	int nZero;

	nZero = 0;
	*(int*) ((char*) this + 0x94) = nZero;
	*(int*) ((char*) this + 0x138) = nZero;
	*(int*) ((char*) this + 0x13c) = nZero;
}

void LinkScfSmallFunctionView::ResetLasrChunkObjectRuntimeState(void)
{
	int nZero;

	*(int*) ((char*) this + 0xb8) = 0x18;
	nZero = 0;
	*(int*) ((char*) this + 0x94) = nZero;
	*(int*) ((char*) this + 0x138) = nZero;
	*(int*) ((char*) this + 0x13c) = nZero;
}

void LinkScfSmallFunctionView::CopyFixedPointPointToPixelPoint(const int* pSource)
{
	*(int*) this = pSource[0] >> 12;
	*(int*) ((char*) this + 4) = pSource[1] >> 12;
	*(int*) ((char*) this + 8) = pSource[2] >> 12;
}

void LinkScfSmallFunctionView::SetObjectFields18_1c_20(int nField18, int nField1c, int nField20)
{
	int nValue18;
	int nValue1c;

	nValue18 = *(volatile int*) &nField18;
	nValue1c = *(volatile int*) &nField1c;
	*(short*) ((char*) this + 0x1c) = (short) nValue1c;
	*(int*) ((char*) this + 0x18) = nValue18;
	*(int*) ((char*) this + 0x20) = nField20;
}

void* LinkScfSmallFunctionView::InitializeThreeDwordPointFromValues(const int* pX, const int* pY, const int* pZ)
{
	*(int*) this = *pX;
	*(int*) ((char*) this + 4) = *pY;
	*(int*) ((char*) this + 8) = *pZ;
	return this;
}

struct LinkScfAutoEffDeleteView {
	void* Delete17C40(BYTE fDelete);
	void* Delete17C70(BYTE fDelete);
	void* Delete17CA0(BYTE fDelete);
	void* Delete17CD0(BYTE fDelete);
	void* Delete17D50(BYTE fDelete);
};

#define DEFINE_LINKSCF_EFF_DELETE(name)                                                                                \
	void* LinkScfAutoEffDeleteView::name(BYTE fDelete)                                                                 \
	{                                                                                                                  \
		((VsNetEffStreamCommon*) this)->VsNetEffStreamCommon::~VsNetEffStreamCommon();                                 \
		if ((fDelete & 1) != 0) {                                                                                      \
			FreeVSMemBlock(this);                                                                                      \
		}                                                                                                              \
		return this;                                                                                                   \
	}

DEFINE_LINKSCF_EFF_DELETE(Delete17C40)
DEFINE_LINKSCF_EFF_DELETE(Delete17C70)
DEFINE_LINKSCF_EFF_DELETE(Delete17CA0)
DEFINE_LINKSCF_EFF_DELETE(Delete17CD0)
DEFINE_LINKSCF_EFF_DELETE(Delete17D50)

#undef DEFINE_LINKSCF_EFF_DELETE

#define DEFINE_LINKSCF_CHUNK_BASE_DELETE(name)                                                                         \
	void* LEMBALL_FASTCALL name(void* pObject, void*, BYTE fDelete)                                                    \
	{                                                                                                                  \
		DestroyLevelChunkObjectBaseAutoThunk(pObject);                                                                 \
		if ((fDelete & 1) != 0) {                                                                                      \
			FreeVSMemBlock(pObject);                                                                                   \
		}                                                                                                              \
		return pObject;                                                                                                \
	}

DEFINE_LINKSCF_CHUNK_BASE_DELETE(DeleteLevelChunkObjectBaseWrapper17D00)
DEFINE_LINKSCF_CHUNK_BASE_DELETE(DeleteBallChunkEntryAuto)

#undef DEFINE_LINKSCF_CHUNK_BASE_DELETE

extern void LEMBALL_FASTCALL DestroyCompositeLevelChunkChildrenAutoThunk(void* pObject);

#define DEFINE_LINKSCF_COMPOSITE_CHILDREN_DELETE(name)                                                                 \
	void* LEMBALL_FASTCALL name(void* pObject, void*, BYTE fDelete)                                                    \
	{                                                                                                                  \
		DestroyCompositeLevelChunkChildrenAutoThunk(pObject);                                                          \
		if ((fDelete & 1) != 0) {                                                                                      \
			FreeVSMemBlock(pObject);                                                                                   \
		}                                                                                                              \
		return pObject;                                                                                                \
	}

DEFINE_LINKSCF_COMPOSITE_CHILDREN_DELETE(DeleteCompositeLevelChunkChildrenWrapper1E8C0)
DEFINE_LINKSCF_COMPOSITE_CHILDREN_DELETE(DeleteCompositeLevelChunkChildrenWrapper20B20)

#undef DEFINE_LINKSCF_COMPOSITE_CHILDREN_DELETE

#define DEFINE_LINKSCF_DESTROY_PAIR(realName, thunkName)                                                               \
	void LEMBALL_FASTCALL realName(void*)                                                                              \
	{                                                                                                                  \
	}                                                                                                                  \
	void LEMBALL_FASTCALL thunkName(void* pObject)                                                                     \
	{                                                                                                                  \
		realName(pObject);                                                                                             \
	}

#define DEFINE_LINKSCF_MANAGER_DESTROY_PAIR(realName, thunkName, vtable, childOffset)                                  \
	void LEMBALL_FASTCALL realName(void* pObject)                                                                      \
	{                                                                                                                  \
		void* pChild;                                                                                                  \
		*(void**) pObject = vtable;                                                                                    \
		pChild = *(void**) ((char*) pObject + childOffset);                                                            \
		if (pChild != 0) {                                                                                             \
			((LinkScfDeletableChild*) pChild)->Delete(3);                                                              \
		}                                                                                                              \
		((VsNetEffStreamCommon*) pObject)->VsNetEffStreamCommon::~VsNetEffStreamCommon();                              \
	}                                                                                                                  \
	void LEMBALL_FASTCALL thunkName(void* pObject)                                                                     \
	{                                                                                                                  \
		realName(pObject);                                                                                             \
	}

DEFINE_LINKSCF_MANAGER_DESTROY_PAIR(DestroyDoorChunkManagerAuto,
									DestroyDoorChunkManagerAutoThunk,
									g_LINKSCF_DoorChunkManagerVtable,
									0x3c)

DEFINE_LINKSCF_MANAGER_DESTROY_PAIR(DestroyProjectilePoolAuto,
									DestroyProjectilePoolAutoThunk,
									g_LINKSCF_ProjectilePoolVtable,
									0x30)

void LEMBALL_FASTCALL DestroyMineChunkManagerAuto(void* pObject)
{
	void* pChild;

	*(void**) pObject = g_LINKSCF_MineChunkManagerVtable;
	pChild = *(void**) ((char*) pObject + 0x34);
	if (pChild != 0) {
		((LinkScfDeletableChild*) pChild)->Delete(3);
		FreeVSMemBlock(*(void**) ((char*) pObject + 0x38));
	}
	((VsNetEffStreamCommon*) pObject)->VsNetEffStreamCommon::~VsNetEffStreamCommon();
}
void LEMBALL_FASTCALL DestroyMineChunkManagerAutoThunk(void* pObject)
{
	DestroyMineChunkManagerAuto(pObject);
}

DEFINE_LINKSCF_MANAGER_DESTROY_PAIR(DestroyLiftChunkManagerAuto,
									DestroyLiftChunkManagerAutoThunk,
									g_LINKSCF_LiftChunkManagerVtable,
									0x3c)

DEFINE_LINKSCF_MANAGER_DESTROY_PAIR(DestroyCaptureTriggerChunkManagerAuto,
									DestroyCaptureTriggerChunkManagerAutoThunk,
									g_LINKSCF_CaptureChunkManagerVtable,
									0x38)

DEFINE_LINKSCF_MANAGER_DESTROY_PAIR(DestroyLasrChunkManagerAuto,
									DestroyLasrChunkManagerAutoThunk,
									g_LINKSCF_LasrChunkManagerVtable,
									0x38)

DEFINE_LINKSCF_MANAGER_DESTROY_PAIR(DestroyTramChunkManagerAuto,
									DestroyTramChunkManagerAutoThunk,
									g_LINKSCF_TramChunkManagerVtable,
									0x38)

#undef DEFINE_LINKSCF_MANAGER_DESTROY_PAIR
#undef DEFINE_LINKSCF_DESTROY_PAIR

#define DEFINE_LINKSCF_NAMED_SCALAR_DELETE(name, destroy)                                                              \
	void* LEMBALL_FASTCALL name(void* pObject, void*, BYTE fDelete)                                                    \
	{                                                                                                                  \
		destroy(pObject);                                                                                              \
		if ((fDelete & 1) != 0) {                                                                                      \
			FreeVSMemBlock(pObject);                                                                                   \
		}                                                                                                              \
		return pObject;                                                                                                \
	}

DEFINE_LINKSCF_NAMED_SCALAR_DELETE(DeleteDoorChunkManagerAuto, DestroyDoorChunkManagerAutoThunk)
DEFINE_LINKSCF_NAMED_SCALAR_DELETE(DeleteProjectilePoolAuto, DestroyProjectilePoolAutoThunk)
DEFINE_LINKSCF_NAMED_SCALAR_DELETE(DeleteMineChunkManagerAuto, DestroyMineChunkManagerAutoThunk)
DEFINE_LINKSCF_NAMED_SCALAR_DELETE(DeleteLiftChunkManagerAuto, DestroyLiftChunkManagerAutoThunk)
DEFINE_LINKSCF_NAMED_SCALAR_DELETE(DeleteCaptureTriggerChunkManagerAuto, DestroyCaptureTriggerChunkManagerAutoThunk)
DEFINE_LINKSCF_NAMED_SCALAR_DELETE(DeleteLasrChunkManagerAuto, DestroyLasrChunkManagerAutoThunk)
DEFINE_LINKSCF_NAMED_SCALAR_DELETE(DeleteTramChunkManagerAuto, DestroyTramChunkManagerAutoThunk)

#undef DEFINE_LINKSCF_NAMED_SCALAR_DELETE

// Compatibility shim for original ILT call site.
void LEMBALL_FASTCALL DestroyLevelChunkStreamAutoThunk(void* pObject)
{
	DestroyLevelChunkStreamAuto(pObject);
}

// Comparison-only compatibility stub.
void LEMBALL_FASTCALL DestroyDoorChunkObjectAuto(void*)
{
}

// Compatibility shim for original ILT call site.
void LEMBALL_FASTCALL DestroyDoorChunkObjectAutoThunk(void* pObject)
{
	DestroyDoorChunkObjectAuto(pObject);
}

// Compatibility shim for original ILT 0x0040153c while the base initializer
// body at 0x00416d20 remains unreconstructed.
void* LevelChunkObjectBaseView::InitializeLevelChunkObjectBase(int nType,
															   unsigned short nChildType,
															   unsigned short nFlags)
{
	(void) nType;
	(void) nChildType;
	(void) nFlags;
	return this;
}

void LEMBALL_FASTCALL ResetInvsChunkObjectState(void* pObject)
{
	char* pObjectBytes;

	pObjectBytes = (char*) pObject;
	*(int*) (pObjectBytes + 0xb8) = 0x18;
	*(unsigned short*) (pObjectBytes + 0x150) = 0;
	*(int*) (pObjectBytes + 0x148) = 0;
	*(int*) (pObjectBytes + 0x254) = 0;
	*(int*) (pObjectBytes + 0x144) = 0;
	*(int*) (pObjectBytes + 0x5c) = 0;
	*(int*) (pObjectBytes + 0x14c) = 0;
}

void LEMBALL_FASTCALL ResetIceChunkObjectRuntimeState(void* pObject)
{
	char* pObjectBytes;
	int nFrameClockTick;

	pObjectBytes = (char*) pObject;
	*(int*) (pObjectBytes + 0x94) = 0;
	*(int*) (pObjectBytes + 0x138) = 0;
	*(int*) (pObjectBytes + 0x13c) = 0;
	*(int*) (pObjectBytes + 0x150) = 0;
	nFrameClockTick = g_nLevelFrameClockTick;
	*(int*) (pObjectBytes + 0xb8) = 0x18;
	*(int*) (pObjectBytes + 0xc8) = nFrameClockTick;
}

extern void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);

void LEMBALL_FASTCALL ResetInvsChunkObjectStateThunk(void* pObject)
{
	ResetInvsChunkObjectState(pObject);
}

void LEMBALL_FASTCALL ResetInvsChunkObject(void* pObject)
{
	ResetManagedEntityRuntimeStateThunk(pObject);
	ResetInvsChunkObjectStateThunk(pObject);
}

void LEMBALL_FASTCALL ResetRockChunkObjectRuntimeStateThunk(void* pObject)
{
	((LinkScfSmallFunctionView*) pObject)->ResetRockChunkObjectRuntimeState();
}

void LEMBALL_FASTCALL ResetRockChunkObject(void* pObject)
{
	ResetManagedEntityRuntimeStateThunk(pObject);
	ResetRockChunkObjectRuntimeStateThunk(pObject);
}

void LEMBALL_FASTCALL ResetIceChunkObjectRuntimeStateThunk(void* pObject)
{
	ResetIceChunkObjectRuntimeState(pObject);
}

void LEMBALL_FASTCALL ResetIceChunkObject(void* pObject)
{
	ResetManagedEntityRuntimeStateThunk(pObject);
	ResetIceChunkObjectRuntimeStateThunk(pObject);
}

void LEMBALL_FASTCALL ResetCaptureTriggerChunkObjectRuntimeStateThunk(void* pObject)
{
	((LinkScfSmallFunctionView*) pObject)->ResetCaptureTriggerChunkObjectRuntimeState();
}

void LEMBALL_FASTCALL ResetCaptureTriggerChunkObject(void* pObject)
{
	char* pObjectBytes;
	int nX;
	int nY;
	int nZ;

	ResetManagedEntityRuntimeStateThunk(pObject);
	ResetCaptureTriggerChunkObjectRuntimeStateThunk(pObject);
	pObjectBytes = (char*) pObject;
	nZ = *(int*) (pObjectBytes + 0x48);
	nX = *(int*) (pObjectBytes + 0x40);
	nY = *(int*) (pObjectBytes + 0x44);
	*(int*) (pObjectBytes + 0x9c) = nX;
	*(int*) (pObjectBytes + 0xa0) = nY;
	*(int*) (pObjectBytes + 0xa4) = nZ;
}

void LEMBALL_FASTCALL ResetLasrChunkObjectRuntimeStateThunk(void* pObject)
{
	((LinkScfSmallFunctionView*) pObject)->ResetLasrChunkObjectRuntimeState();
}

void LEMBALL_FASTCALL ResetLasrChunkObject(void* pObject)
{
	ResetManagedEntityRuntimeStateThunk(pObject);
	ResetLasrChunkObjectRuntimeStateThunk(pObject);
}

// Original body is emitted last to preserve MSVC 4.20 compiler state.
int PASCAL return_false_ice_chunk_callback(void* pUnused1, void* pUnused2)
{
	(void) pUnused1;
	(void) pUnused2;
	return 0;
}

void noop_lift_callback(void)
{
}

void noop_lasr_callback(void)
{
}
