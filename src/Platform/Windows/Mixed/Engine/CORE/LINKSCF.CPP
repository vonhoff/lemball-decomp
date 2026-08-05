#include "AI/CGameObject.h"
#include "AI/CGenericGroup.h"
#include "AI/CHand.h"
#include "AI/CHandManager.h"
#include "AI/CInvisibleSwitch.h"
#include "AI/CInvisibleSwitchManager.h"
#include "AI/CIceManager.h"
#include "AI/CLaser.h"
#include "AI/CLaserManager.h"
#include "AI/CLift.h"
#include "AI/CLiftManager.h"
#include "AI/CMineManager.h"
#include "Control/CGame.h"
#include "Platform/Windows/Mixed/Engine/CORE/LINKSCF.H"
#include "Platform/Windows/Mixed/Engine/CORE/WIN32.H"
#include "Platform/Windows/Mixed/Engine/GDI/VSGEOM.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/VSSTRM.H"
#include "Platform/Windows/Mixed/Level/LEVELRUN.H"
#include "Visos/Generic/Memory.h"

#include <string.h>

extern int g_nSelectedNetworkLobbyPeerId;

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nChildType, unsigned short nFlags);
};

extern void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);

struct RockChunkObjectView : LevelChunkObjectBaseView {
	RockChunkObjectView* ConstructRockChunkObject(void);
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

struct LevelTileGridCellView {
	unsigned char m_abReserved00[6];
	unsigned short m_nFlags06;
	unsigned char m_abReserved08[4];
};

struct LevelTileGridCellArrayView {
	void* m_pVtable00;
	LevelTileGridCellView* m_pCells04;
	int m_cColumns08;
	int m_cRows0C;

	void OrCellFlags(int x, int y, unsigned int nFlags);
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
extern int LEMBALL_FASTCALL IsRequestingFire(void* pObject);
extern void LEMBALL_FASTCALL StartFiring(void* pObject);
extern void LEMBALL_FASTCALL EndFiring(void* pObject);
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
extern void LEMBALL_FASTCALL CBulletManagerProcess(void* pPool);

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



int LEMBALL_FASTCALL EmitCachedChunkManagerRenderEntriesThunk(void* pObject, void* pUnused, char* pEntries);

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

int LEMBALL_FASTCALL GetFirstManagedEntityChildFromCachedGroup(void* pObject);
int LEMBALL_FASTCALL GetFirstIteratedChildStatus11C(void* pObject);

void LEMBALL_FASTCALL ActivateRockChunkObjects(void* pManager);
void LEMBALL_FASTCALL ActivateTramChunkObjects(void* pManager);
void LEMBALL_FASTCALL ActivatePgunChunkObjects(void* pManager);
void LEMBALL_FASTCALL ActivateMoveChunkObjects(void* pManager);
void LEMBALL_FASTCALL AdvanceActiveMoveChunkObjects(void* pManager);

DEFINE_LINKSCF_VTABLE_STUB_0(DispatchDoorChildArrayVirtual104)
DEFINE_LINKSCF_VTABLE_STUB_0(ServiceActiveDoorChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitDoorChunkRenderEntries)
void LEMBALL_FASTCALL ResetProjectilePool(void* pPool);
int LEMBALL_FASTCALL EmitProjectilePoolRenderEntries(void* pPool, void* pUnused, void* pRenderEntry);




DEFINE_LINKSCF_VTABLE_STUB_0(CountNestedIteratedChildEntries)
DEFINE_LINKSCF_VTABLE_STUB_0(GetFirstManagedEntityChildFromCachedGroupCompatibility)
DEFINE_LINKSCF_VTABLE_STUB_0(GetNextManagedEntityChildFromCachedGroup)
DEFINE_LINKSCF_VTABLE_STUB_0(GetFirstIteratedChildStatus11CCompatibility)
DEFINE_LINKSCF_VTABLE_STUB_1(AdvanceCachedGroupManagedEntityChildIterator)
DEFINE_LINKSCF_VTABLE_STUB_2(AppendManagedEntityGroupFromIds)
DEFINE_LINKSCF_VTABLE_STUB_0(ActivateCollChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_0(RefreshActiveCollChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitCollChunkManagerRenderEntries)

DEFINE_LINKSCF_VTABLE_STUB_0(RefreshActiveMineChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitMineChunkManagerRenderEntries)



DEFINE_LINKSCF_VTABLE_STUB_0(RefreshActiveRockChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitRockChunkManagerRenderEntries)
DEFINE_LINKSCF_VTABLE_STUB_0(InitializeTramChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitTramChunkManagerRenderEntries)
DEFINE_LINKSCF_VTABLE_STUB_0(RefreshActivePgunChunkObjects)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitActivePgunChunkManagerRenderEntries)
DEFINE_LINKSCF_VTABLE_STUB_1(EmitMoveChunkManagerRenderEntries)

#undef DEFINE_LINKSCF_VTABLE_STUB_0
#undef DEFINE_LINKSCF_VTABLE_STUB_1
#undef DEFINE_LINKSCF_VTABLE_STUB_2


struct tRect {
	int m_nLeft;
	int m_nTop;
	int m_nRight;
	int m_nBottom;
};
// Compatibility shim retained for compiler-generated callers; original ILT slot
// 0x00401078 targets GetFirstGroup directly.
void* LEMBALL_FASTCALL GetFirstCachedChunkObjectThunk(void* pObject)
{
	return ((CGenericGroupManager*) pObject)->GetFirstGroup();
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
LiftManagerVtableLayout g_LINKSCF_LiftChunkManagerVtable = {
	{(void*) ReturnTrueVtableCallbackThunk,
	 (void*) ReturnTrueVtableCallbackSecondaryThunk,
	 (void*) DispatchType18ChunkStreamCommandsThunk,
	 (void*) NoopVtableCallbackThunk,
	 (void*) NoopType18ChunkStreamWriteThunk,
	 (void*) DeleteLiftChunkManagerAuto},
	&CLiftManager::Restart,
	&CLiftManager::Process,
	(void*) ReturnFalseChunkStreamCommandThunk,
	&CLiftManager::GetViewData};
// GLOBAL: LEMBALL 0x00496b78
void* g_LINKSCF_MoveChunkManagerVtable[10] = LINKSCF_EFF_CHUNK_VTABLE(DeleteMoveChunkManagerAuto,
																	  ActivateMoveChunkObjects,
																	  AdvanceActiveMoveChunkObjects,
																	  ReturnFalseChunkStreamCommandThunk,
																	  EmitMoveChunkManagerRenderEntries);
struct ObjectManagerVtableLayout {
	void* m_apBase00[6];
	void (CObjectManager::*m_pRestart18)(void);
	void (CObjectManager::*m_pProcess1C)(void);
	int (CObjectManager::*m_pReceive20)(unsigned short, CGlobalGameObject*, CNetworkMessage*);
	int (CObjectManager::*m_pGetViewData24)(CViewData*);
};
typedef char ObjectManagerMemberPointerMustBeOneWord
	[sizeof(void (CObjectManager::*)(void)) == sizeof(void*) ? 1 : -1];
// GLOBAL: LEMBALL 0x00494238
static ObjectManagerVtableLayout g_LINKSCF_GmobChunkManagerVtable = {
	{(void*) ReturnTrueVtableCallbackThunk,
	 (void*) ReturnTrueVtableCallbackSecondaryThunk,
	 (void*) DispatchType18ChunkStreamCommandsThunk,
	 (void*) NoopVtableCallbackThunk,
	 (void*) NoopType18ChunkStreamWriteThunk,
	 (void*) DeleteCountedChildTableStreamAuto},
	&CObjectManager::Restart,
	&CObjectManager::Process,
	&CObjectManager::Receive,
	&CObjectManager::GetViewData};
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
	(void*) IsRequestingFire,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) noop_vtable_callback_0040a990,
	(void*) StartFiring,
	(void*) EndFiring,
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
																		   CBulletManagerProcess,
																		   ReturnFalseChunkStreamCommandThunk,
																		   EmitProjectilePoolRenderEntries);
static void* g_LINKSCF_ManagedEntityQueueCursorVtable[4];
struct CachedChunkObjectManagerVtable {
	int (CGenericGroupManager::*m_pGetNumberOfGroups00)(void);
	CGenericGroup* (CGenericGroupManager::*m_pGetFirstGroup04)(void);
	CGenericGroup* (CGenericGroupManager::*m_pGetNextGroup08)(void);
	CGenericGroup* (CGenericGroupManager::*m_pGetNthGroup0C)(int);
	CGenericGroup* (CGenericGroupManager::*m_pGetCurrentGroup10)(void);
	void (CGenericGroupManager::*m_pDeleteGroup14)(CGenericGroup*);
	int (CGenericGroupManager::*m_pGetNumberOfElements18)(void);
	CGameObject* (CGenericGroupManager::*m_pGetFirstElement1C)(void);
	CGameObject* (CGenericGroupManager::*m_pGetNextElement20)(void);
	CGameObject* (CGenericGroupManager::*m_pGetCurrentElement24)(void);
	CGameObject* (CGenericGroupManager::*m_pGetNthElement28)(int);
	CGenericGroup* (CGenericGroupManager::*m_pGetGroupElementIsMemberOf2C)(CGameObject*);
	void (CGenericGroupManager::*m_pAddNewGroup30)(CGenericGroup*);
	void* m_pCreateNewGroupFromIds34;
	void (CGenericGroupManager::*m_pCreateNewGroup38)(void);
	void (CGenericGroupManager::*m_pAddElementToGroup3C)(CGameObject*, CGenericGroup*);
	int (CGenericGroupManager::*m_pRemoveElementFromGroup40)(CGameObject*, CGenericGroup*);
	void (CGenericGroupManager::*m_pFindElementInGroupAndRemoveIt44)(CGameObject*);
	int (CGenericGroupManager::*m_pGetAllBoundingBoxes48)(tRect*);
};

#define LINKSCF_CACHED_CHUNK_MANAGER_VTABLE                                                                            \
	{                                                                                                                  \
		&CGenericGroupManager::GetNumberOfGroups,                                                              \
		&CGenericGroupManager::GetFirstGroup,                                                                  \
		&CGenericGroupManager::GetNextGroup,                                                                   \
		&CGenericGroupManager::GetNthGroup,                                                                    \
		&CGenericGroupManager::GetCurrentGroup,                                                                \
		&CGenericGroupManager::DeleteGroup,                                                                    \
		&CGenericGroupManager::GetNumberOfElements,                                                          \
		&CGenericGroupManager::GetFirstElement,                                                              \
		&CGenericGroupManager::GetNextElement,                                                               \
		&CGenericGroupManager::GetCurrentElement,                                                            \
		&CGenericGroupManager::GetNthElement,                                                                \
		&CGenericGroupManager::GetGroupElementIsMemberOf,                                                     \
		&CGenericGroupManager::AddNewGroup,                                                                    \
		(void*) AppendManagedEntityGroupFromIds,                                                               \
		&CGenericGroupManager::CreateNewGroup,                                                                 \
		&CGenericGroupManager::AddElementToGroup,                                                               \
		&CGenericGroupManager::RemoveElementFromGroup,                                                          \
		&CGenericGroupManager::FindElementInGroupAndRemoveIt,                                                  \
		&CGenericGroupManager::GetAllBoundingBoxes                                                              \
	}

// GLOBAL: LEMBALL 0x00494d08
static CachedChunkObjectManagerVtable g_LINKSCF_CachedChunkObjectManagerBaseVtable =
	LINKSCF_CACHED_CHUNK_MANAGER_VTABLE;
// GLOBAL: LEMBALL 0x00494d70
static CachedChunkObjectManagerVtable g_LINKSCF_ShpgChunkManagerVtable = LINKSCF_CACHED_CHUNK_MANAGER_VTABLE;
// GLOBAL: LEMBALL 0x004953f8
static CachedChunkObjectManagerVtable g_LINKSCF_EnmyChunkManagerVtable = LINKSCF_CACHED_CHUNK_MANAGER_VTABLE;
// GLOBAL: LEMBALL 0x004955a0
void* g_LINKSCF_CollChunkManagerVtable[10] = LINKSCF_EFF_CHUNK_VTABLE(DeleteCollChunkManagerAuto,
																	  ActivateCollChunkObjects,
																	  RefreshActiveCollChunkObjects,
																	  ReturnFalseChunkStreamCommandThunk,
																	  EmitCollChunkManagerRenderEntries);
// GLOBAL: LEMBALL 0x00495d30
MineManagerVtableLayout g_LINKSCF_MineChunkManagerVtable = {
	{(void*) ReturnTrueVtableCallbackThunk,
	 (void*) ReturnTrueVtableCallbackSecondaryThunk,
	 (void*) DispatchType18ChunkStreamCommandsThunk,
	 (void*) NoopVtableCallbackThunk,
	 (void*) NoopType18ChunkStreamWriteThunk,
	 (void*) DeleteMineChunkManagerAuto},
	&CMineManager::Restart,
	&CMineManager::Process,
	(void*) ReturnFalseChunkStreamCommandThunk,
	&CMineManager::GetViewData};
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
CHandManagerVtable g_LINKSCF_CaptureChunkManagerVtable = {
	{(void*) ReturnTrueVtableCallbackThunk,
	 (void*) ReturnTrueVtableCallbackSecondaryThunk,
	 (void*) DispatchType18ChunkStreamCommandsThunk,
	 (void*) NoopVtableCallbackThunk,
	 (void*) NoopType18ChunkStreamWriteThunk,
	 (void*) DeleteCaptureTriggerChunkManagerAuto},
	&CHandManager::Restart,
	&CHandManager::Process,
	(void*) ReturnFalseChunkStreamCommandThunk,
	&CHandManager::GetViewData};
struct LaserManagerVtableLayout {
	void* m_apSlots00[6];
	void (CLaserManager::*m_pRestart18)(void);
	void (CLaserManager::*m_pProcess1C)(void);
	void* m_pCommand20;
	int (CLaserManager::*m_pGetViewData24)(CViewData*);
};
typedef char LaserManagerMemberPointerMustBeOneWord
	[sizeof(void (CLaserManager::*)(void)) == sizeof(void*) ? 1 : -1];

// GLOBAL: LEMBALL 0x00496458
LaserManagerVtableLayout g_LINKSCF_LasrChunkManagerVtable = {
	{(void*) ReturnTrueVtableCallbackThunk,
	 (void*) ReturnTrueVtableCallbackSecondaryThunk,
	 (void*) DispatchType18ChunkStreamCommandsThunk,
	 (void*) NoopVtableCallbackThunk,
	 (void*) NoopType18ChunkStreamWriteThunk,
	 (void*) DeleteLasrChunkManagerAuto},
	&CLaserManager::Restart,
	&CLaserManager::Process,
	(void*) ReturnFalseChunkStreamCommandThunk,
	&CLaserManager::GetViewData};
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
CIceManagerVtable g_LINKSCF_IceChunkManagerVtable = {
	{(void*) ReturnTrueVtableCallbackThunk,
	 (void*) ReturnTrueVtableCallbackSecondaryThunk,
	 (void*) DispatchType18ChunkStreamCommandsThunk,
	 (void*) NoopVtableCallbackThunk,
	 (void*) NoopType18ChunkStreamWriteThunk,
	 (void*) DeleteIceChunkManagerAuto},
	&CIceManager::Restart,
	&CIceManager::Process,
	(void*) ReturnFalseChunkStreamCommandThunk,
	&CIceManager::GetViewData};
struct InvisibleSwitchManagerVtableLayout {
	void* m_apBaseSlots[6];
	void (CInvisibleSwitchManager::*m_pRestart)(void);
	void (CInvisibleSwitchManager::*m_pProcess)(void);
	void* m_apTailSlots[2];
};
typedef char InvisibleSwitchManagerMemberPointerMustFitVtableSlot
	[sizeof(void (CInvisibleSwitchManager::*)(void)) == sizeof(void*) ? 1 : -1];
// GLOBAL: LEMBALL 0x00493298
InvisibleSwitchManagerVtableLayout g_LINKSCF_InvsChunkManagerVtable = {
	{(void*) ReturnTrueVtableCallbackThunk,
	 (void*) ReturnTrueVtableCallbackSecondaryThunk,
	 (void*) DispatchType18ChunkStreamCommandsThunk,
	 (void*) NoopVtableCallbackThunk,
	 (void*) NoopType18ChunkStreamWriteThunk,
	 (void*) DeleteLevelChunkStreamAuto},
	&CInvisibleSwitchManager::Restart,
	&CInvisibleSwitchManager::Process,
	{(void*) ReturnFalseChunkStreamCommandThunk, (void*) ReturnFalseChunkStreamRenderThunk}};
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
CFormationManager* g_pGenericGroupFormationManager;


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
CGenericGroupManager* CGenericGroupManager::
	ConstructCachedChunkObjectManagerBaseThunk(void* pLevelMode, void* pEntityLookup, void* pFormationManager)
{
	return ConstructCachedChunkObjectManagerBase(pLevelMode, pEntityLookup, pFormationManager);
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
	return ((CGenericGroupManager*) pObject)->GetViewData((CViewData*) pEntries);
}

// LINKERILT: LEMBALL 0x0040204f
LinkScfBoonChunkManagerView* LinkScfBoonChunkManagerView::ConstructBoonChunkManagerThunk(void* pLevelMode,
																						 void* pTileGrid)
{
	return ConstructBoonChunkManager(pLevelMode, pTileGrid);
}


// LINKERILT: LEMBALL 0x00402f95
LinkScfPgunChunkManagerView* LinkScfPgunChunkManagerView::ConstructPgunChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructPgunChunkManager(pLevelMode, nCapacity);
}
// LINKERILT: LEMBALL 0x00402fdb
CCollectableManager* CCollectableManager::ConstructCollChunkManagerThunk(void* pLevelMode,
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
// MACINTOSH: CLevelLoader::LoadLevel(eSkill, int, unsigned char)
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
			((CMap*) *(void**) (pLevelMode + 0x110))
				->LoadLevel((const unsigned short*) (pChunk + 8), cbChunk - 8, fReuseState);
		}
		else if (nTag == 0x41492020) {
			LoadLevelAiChunk(pLevelMode, (const unsigned short*) (pChunk + 8), cbChunk - 8);
		}
		else if (nTag == 0x464c4147) {
			LoadLevelFlagChunk(pLevelMode, (const unsigned short*) (pChunk + 8));
		}
		else if (nTag == 0x44454654) {
			((CMap*) *(void**) (pLevelMode + 0x110))
				->LoadDefaultBlox((const unsigned short*) (pChunk + 8), cbChunk - 8);
		}
		else if (nTag == 0x4e414d45) {
			((CMap*) *(void**) (pLevelMode + 0x110))
				->LoadLevelName((const char*) (pChunk + 8), cbChunk - 8);
		}
		else if (nTag == 0x414e494d) {
			LoadAnimChunkRecords(*(void**) (pLevelMode + 0x194), (const unsigned short*) (pChunk + 8));
		}
		else if (nTag == 0x4e4f4445) {
			((LevelNodeChunkManager*) *(void**) (pLevelMode + 0x17c))
				->LoadLevel((const unsigned short*) (pChunk + 8), cbChunk - 8, fReuseState);
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
// MACINTOSH: CDoorManager::CDoorManager(CAI*, int)
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

// FUNCTION: LEMBALL 0x0040eb90
void LevelTileGridCellArrayView::OrCellFlags(int x, int y, unsigned int nFlags)
{
	if (x < 0 || y < 0 || x >= m_cColumns08 || y >= m_cRows0C) {
		return;
	}
	m_pCells04[y * m_cColumns08 + x].m_nFlags06 |= (unsigned short) nFlags;
}

// MACINTOSH: CPlayerLemming::CPlayerLemming(int, int, int, int, unsigned char, unsigned long)
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
// MACINTOSH: CPlayerLemming::Restart()
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
// MACINTOSH: CPlayerLemmingGroupManager::CPlayerLemmingGroupManager(CAI*, CObjectManager*, CFormationManager*)
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

// MACINTOSH: CPlayerLemmingGroupManager::LoadLevel(unsigned char*, unsigned long, unsigned char)
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
CObjectManager::CObjectManager(CAI* pAI, int nCapacity)
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
	*(void**) this = &g_LINKSCF_GmobChunkManagerVtable;
	g_pLINKSCF_GmobChunkManager = this;
	*(CAI**) ((char*) this + 0x30) = pAI;
	*(short*) ((char*) this + 0x36) = 0;
	*(int*) ((char*) this + 0x3c) = 0;
	*(short*) ((char*) this + 0x38) = 0;
	*(short*) ((char*) this + 0x34) = 0;
	g_nLINKSCF_GmobObjectCount = 0;
}

struct ObjectManagerChildVtable;
struct ObjectManagerChildView {
	ObjectManagerChildVtable* m_pVtable00;
	void Delete(BYTE fDelete);
	void Activate(void);
};
typedef void (ObjectManagerChildView::*ObjectManagerChildDeleteProc)(BYTE);
typedef void (ObjectManagerChildView::*ObjectManagerChildActivateProc)(void);
struct ObjectManagerChildVtable {
	ObjectManagerChildDeleteProc m_pDelete00;
	void* m_apReserved04[64];
	ObjectManagerChildActivateProc m_pActivate104;
};

// FUNCTION: LEMBALL 0x0041afd0
void CObjectManager::Restart(void)
{
	int nOffset;
	int nIndex;
	int nPendingIndex;
	int nResolvedCount;
	void** ppPending;
	char* pObject;
	char* pLinked;
	ObjectManagerChildView* pChild;

	nOffset = 0;
	nResolvedCount = 0;
	if (m_apObjects3C != 0 && (nIndex = 0, m_nObjectCount34 > 0)) {
		do {
			ppPending = m_apObjects3C;
			pObject = *(char**) ((char*) ppPending + nOffset);
			if (pObject != 0) {
				if (*(int*) (pObject + 0x64) == 0x11 &&
					(pLinked = *(char**) (pObject + 0x148)) != 0 &&
					*(int*) (pObject + 0x14c) == 0xffff &&
					(nPendingIndex = 0, m_nPendingCount36 != 0)) {
					do {
						if (*ppPending == pLinked) {
							*(int*) (pObject + 0x14c) = *(int*) (pLinked + 0x64);
							pChild = (ObjectManagerChildView*) m_apObjects3C[nPendingIndex];
							(pChild->*pChild->m_pVtable00->m_pActivate104)();
							++nResolvedCount;
							*(int*) ((char*) m_apObjects3C[nPendingIndex] + 0x38) = 1;
							m_apObjects3C[nPendingIndex] = 0;
							break;
						}
						++ppPending;
						++nPendingIndex;
					} while (nPendingIndex < m_nPendingCount36);
				}
				pChild = (ObjectManagerChildView*) *(void**) ((char*) m_apObjects3C + nOffset);
				(pChild->*pChild->m_pVtable00->m_pActivate104)();
				*(int*) ((char*) *(void**) ((char*) m_apObjects3C + nOffset) + 0x38) = 1;
			}
			nOffset += 4;
			++nIndex;
		} while (nIndex < m_nObjectCount34);
	}
	m_nPendingCount36 -= (short) nResolvedCount;
}

// FUNCTION: LEMBALL 0x0041b0c0
void CObjectManager::Initialise(int nCount)
{
	int nOffset;
	int nIndex;

	nOffset = 0;
	*(unsigned short*) ((char*) this + 0x34) = nCount + 4;
	if (m_apObjects3C == 0) {
		m_apObjects3C = (void**) AllocateVSMemBlock((unsigned int) m_nObjectCount34 * sizeof(void*));
		nIndex = 0;
		if (m_nObjectCount34 > 0) {
			do {
				nOffset += sizeof(void*);
				++nIndex;
				*(void**) ((char*) m_apObjects3C + nOffset - sizeof(void*)) = 0;
			} while (nIndex < m_nObjectCount34);
		}
	}
}


// FUNCTION: LEMBALL 0x0041b110
CObjectManager::~CObjectManager()
{
	int nIndex;
	int nOffset;
	ObjectManagerChildView* pChild;

	nOffset = 0;
	*(void**) this = &g_LINKSCF_GmobChunkManagerVtable;
	nIndex = 0;
	if (m_nPendingCount36 > nOffset) {
		do {
			pChild = (ObjectManagerChildView*) *(void**) ((char*) m_apObjects3C + nOffset);
			if (pChild != 0) {
				(pChild->*pChild->m_pVtable00->m_pDelete00)(1);
			}
			nOffset += sizeof(void*);
			++nIndex;
		} while (nIndex < m_nPendingCount36);
	}
	FreeVSMemBlock(m_apObjects3C);
	((VsNetEffStreamCommon*) this)->VsNetEffStreamCommon::~VsNetEffStreamCommon();
}

// FUNCTION: LEMBALL 0x0041b2a0
CGlobalGameObject* CObjectManager::AddObject(
	unsigned short nId, CGlobalGameObject* pObject, unsigned char nState)
{
	if (m_nPendingCount36 < m_nObjectCount34) {
		m_apObjects3C[m_nPendingCount36] = pObject;
		if (nId != 0xffff) {
			((CGameObject*) pObject)->SetId(nId);
		}
		*(int*) ((char*) *(void**) ((char*) m_apObjects3C + (unsigned int) m_nPendingCount36 * 4) + 0x38) = 1;
		*(int*) ((char*) *(void**) ((char*) m_apObjects3C + (unsigned int) m_nPendingCount36 * 4) + 0x3c) =
			*(int*) &nState;
		return (CGlobalGameObject*) m_apObjects3C[m_nPendingCount36++];
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041b7d0
void CObjectManager::Process(void)
{
	typedef void(LEMBALL_FASTCALL * ChildProcessProc)(void* pObject);
	int iObject;
	int nOffset;
	void* pObject;

	iObject = 0;
	if (m_nPendingCount36 > 0) {
		nOffset = 0;
		do {
			pObject = *(void**) ((char*) m_apObjects3C + nOffset);
			*(int*) ((char*) pObject + 0x124) = 1;
			pObject = *(void**) ((char*) m_apObjects3C + nOffset);
			if (*(int*) ((char*) pObject + 0x38) != 0) {
				((ChildProcessProc) (*(void***) pObject)[0x14 / sizeof(void*)])(pObject);
				if (g_nSelectedNetworkLobbyPeerId != 0) {
					pObject = *(void**) ((char*) m_apObjects3C + nOffset);
					if (*(int*) ((char*) pObject + 0x38) == 0) {
						((ChildProcessProc) (*(void***) pObject)[0xf4 / sizeof(void*)])(pObject);
					}
				}
			}
			nOffset += 4;
			++iObject;
		} while (iObject < (int) (unsigned int) m_nPendingCount36);
	}
}

// FUNCTION: LEMBALL 0x0041b830
int CObjectManager::GetViewData(CViewData* pViewData)
{
	typedef int(LEMBALL_FASTCALL * ChildVisibleProc)(void* pObject);
	typedef void(LEMBALL_FASTCALL * ChildViewDataProc)(void* pObject, void* pUnused, CViewData* pViewData);
	int cViewData;
	volatile int iObject;
	int nOffset;
	void* pObject;

	cViewData = 0;
	iObject = 0;
	if (m_nPendingCount36 > 0) {
		nOffset = 0;
		do {
			pObject = *(void**) ((char*) m_apObjects3C + nOffset);
			if (*(int*) ((char*) pObject + 0x38) != 0 ||
				((ChildVisibleProc) (*(void***) pObject)[0x38 / sizeof(void*)])(pObject) != 0) {
				pObject = *(void**) ((char*) m_apObjects3C + nOffset);
				if (*(int*) ((char*) pObject + 0x64) != 5 || *(int*) ((char*) pObject + 0xb8) != 0x1b) {
					++cViewData;
					((ChildViewDataProc) (*(void***) pObject)[0x0c / sizeof(void*)])(pObject, 0, pViewData);
					pViewData = (CViewData*) ((char*) pViewData + 0x4c);
				}
			}
			nOffset += 4;
			++iObject;
		} while (iObject < (int) (unsigned int) m_nPendingCount36);
	}
	return cViewData;
}

// FUNCTION: LEMBALL 0x0041b8f0
CGlobalGameObject* CObjectManager::FindObject(int nObjectId)
{
	int nObjectCount;
	int iObject;
	char* pObject;

	nObjectCount = m_nPendingCount36;
	iObject = 0;
	while (1) {
		if (nObjectCount <= iObject) {
			return 0;
		}
		if (*(unsigned short*) ((char*) m_apObjects3C[iObject] + 0x6a) == nObjectId) {
			break;
		}
		++iObject;
	}
	pObject = (char*) m_apObjects3C[iObject];
	return *(int*) (pObject + 0x38) != 0 ? (CGlobalGameObject*) pObject : 0;
}

// FUNCTION: LEMBALL 0x0041b9b0
void CObjectManager::Remove(CGlobalGameObject* pObject)
{
	int iObject;
	void** ppObjects;
	void** ppObject;

	iObject = 0;
	if (m_nPendingCount36 > iObject) {
		ppObjects = m_apObjects3C;
		ppObject = ppObjects;
		while (*ppObject != pObject) {
			++ppObject;
			++iObject;
			if (m_nPendingCount36 <= iObject) {
				return;
			}
		}
		*(int*) ((char*) ppObjects[iObject] + 0x38) = 0;
	}
}

// FUNCTION: LEMBALL 0x0041bf00
int CObjectManager::Receive(
	unsigned short nMessage, CGlobalGameObject* pObject, CNetworkMessage* pMessage)
{
	(void) pMessage;
	if ((*(unsigned int*) &nMessage & 0xffff) != 0x2a) {
		return 0;
	}
	Remove(pObject);
	return 1;
}

// MACINTOSH: CGenericGroupManager::CGenericGroupManager(CAI*, CObjectManager*, CFormationManager*)
// FUNCTION: LEMBALL 0x0041e8f0
CGenericGroupManager* CGenericGroupManager::ConstructCachedChunkObjectManagerBase(
	void* pLevelMode,
	void* pEntityLookup,
	void* pFormationManager)
{
	*(void**) this = &g_LINKSCF_CachedChunkObjectManagerBaseVtable;
	g_pCachedChunkManagerLevelMode = pLevelMode;
	g_pCachedChunkManagerEntityLookup = pEntityLookup;
	g_pGenericGroupFormationManager = (CFormationManager*) pFormationManager;
	*(int*) ((char*) this + 0xa4) = 0;
	*(int*) ((char*) this + 0xa8) = 0;
	*(int*) ((char*) this + 0xac) = 1;
	memset((char*) this + 4, 0, 40 * sizeof(int));
	return this;
}
// FUNCTION: LEMBALL 0x0041ea20
int CGenericGroupManager::GetNumberOfGroups(void)
{
	return m_nGroupCountA4;
}
// FUNCTION: LEMBALL 0x0041ea30
CGenericGroup* CGenericGroupManager::GetFirstGroup(void)
{
	m_nCurrentGroupA8 = 0;
	if (m_nGroupCountA4 == 0) {
		return 0;
	}
	return m_apGroups04[0];
}
// FUNCTION: LEMBALL 0x0041ea50
CGenericGroup* CGenericGroupManager::GetNextGroup(void)
{
	int nIndex;

	nIndex = m_nCurrentGroupA8 + 1;
	m_nCurrentGroupA8 = nIndex;
	if (m_nGroupCountA4 <= nIndex) {
		return 0;
	}
	return m_apGroups04[nIndex];
}
// FUNCTION: LEMBALL 0x0041ea70
CGenericGroup* CGenericGroupManager::GetNthGroup(int nIndex)
{
	m_nCurrentGroupA8 = nIndex;
	if (m_nGroupCountA4 <= nIndex) {
		return 0;
	}
	return m_apGroups04[nIndex];
}
// FUNCTION: LEMBALL 0x0041ea90
CGenericGroup* CGenericGroupManager::GetCurrentGroup(void)
{
	int nIndex = m_nCurrentGroupA8;
	if (m_nGroupCountA4 <= nIndex) {
		return 0;
	}
	return m_apGroups04[nIndex];
}

// FUNCTION: LEMBALL 0x0041eab0
int CGenericGroupManager::GetNumberOfElements(void)
{
	typedef CGenericGroup*(LEMBALL_FASTCALL * ManagerGroupProc)(void* pManager);
	typedef int(LEMBALL_FASTCALL * GroupCountProc)(void* pGroup);
	void** pVtable;
	ManagerGroupProc pGetNextGroup;
	CGenericGroup* pGroup;
	int nElements;

	nElements = 0;
	pVtable = *(void***) this;
	pGroup = ((ManagerGroupProc) pVtable[1])(this);
	if (pGroup != 0) {
		pGetNextGroup = (ManagerGroupProc) pVtable[2];
		do {
			nElements += ((GroupCountProc) (*(void***) pGroup)[0x108 / sizeof(void*)])(pGroup);
			pGroup = pGetNextGroup(this);
		} while (pGroup != 0);
	}
	return nElements;
}

// FUNCTION: LEMBALL 0x0041eae0
CGameObject* CGenericGroupManager::GetFirstElement(void)
{
	typedef CGenericGroup*(LEMBALL_FASTCALL * ManagerGroupProc)(void* pManager);
	typedef CGameObject*(LEMBALL_FASTCALL * GroupElementProc)(void* pGroup);
	CGenericGroup* pGroup;

	pGroup = ((ManagerGroupProc) (*(void***) this)[1])(this);
	if (pGroup != 0) {
		return ((GroupElementProc) (*(void***) pGroup)[0x114 / sizeof(void*)])(pGroup);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041eb00
CGameObject* CGenericGroupManager::GetNextElement(void)
{
	typedef CGenericGroup*(LEMBALL_FASTCALL * ManagerGroupProc)(void* pManager);
	typedef CGameObject*(LEMBALL_FASTCALL * GroupElementProc)(void* pGroup);
	void** pVtable;
	CGenericGroup* pGroup;
	CGameObject* pElement;

	pVtable = *(void***) this;
	pGroup = ((ManagerGroupProc) pVtable[4])(this);
	if (pGroup == 0) {
		pElement = 0;
	}
	else {
		pElement = ((GroupElementProc) (*(void***) pGroup)[0x118 / sizeof(void*)])(pGroup);
		if (pElement == 0) {
			pGroup = ((ManagerGroupProc) pVtable[2])(this);
			if (pGroup != 0) {
				return ((GroupElementProc) (*(void***) pGroup)[0x114 / sizeof(void*)])(pGroup);
			}
			return 0;
		}
	}
	return pElement;
}

// FUNCTION: LEMBALL 0x0041eb40
CGameObject* CGenericGroupManager::GetCurrentElement(void)
{
	typedef CGenericGroup*(LEMBALL_FASTCALL * ManagerGroupProc)(void* pManager);
	typedef CGameObject*(LEMBALL_FASTCALL * GroupElementProc)(void* pGroup);
	CGenericGroup* pGroup;
	CGameObject* pElement;

	pElement = 0;
	pGroup = ((ManagerGroupProc) (*(void***) this)[4])(this);
	if (pGroup != 0) {
		pElement = ((GroupElementProc) (*(void***) pGroup)[0x11c / sizeof(void*)])(pGroup);
	}
	return pElement;
}

// FUNCTION: LEMBALL 0x0041eb60
CGameObject* CGenericGroupManager::GetNthElement(int nIndex)
{
	typedef CGameObject*(LEMBALL_FASTCALL * ManagerElementProc)(void* pManager);
	void** pVtable;
	CGameObject* pElement;
	int i;

	pVtable = *(void***) this;
	pElement = ((ManagerElementProc) pVtable[7])(this);
	for (i = 0; pElement != 0 && i < nIndex; ++i) {
		pElement = ((ManagerElementProc) pVtable[8])(this);
	}
	return pElement;
}

// FUNCTION: LEMBALL 0x0041eb90
CGenericGroup* CGenericGroupManager::GetGroupElementIsMemberOf(CGameObject* pObject)
{
	typedef CGenericGroup*(LEMBALL_FASTCALL * ManagerGroupProc)(void* pManager);
	void** pVtable;
	CGenericGroup* pGroup;

	pVtable = *(void***) this;
	pGroup = ((ManagerGroupProc) pVtable[1])(this);
	while (pGroup != 0) {
		if ((pGroup->*pGroup->m_pVtable00->m_pContains130)(pObject) == 1) {
			return pGroup;
		}
		pGroup = ((ManagerGroupProc) pVtable[2])(this);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041ec80
void CGenericGroupManager::AddNewGroup(CGenericGroup* pGroup)
{
	if (m_nGroupCountA4 < 40) {
		m_apGroups04[m_nGroupCountA4] = pGroup;
		++m_nGroupCountA4;
	}
}

// FUNCTION: LEMBALL 0x0041eca0
void CGenericGroupManager::CreateNewGroup(void)
{
}

// FUNCTION: LEMBALL 0x0041ecb0
void CGenericGroupManager::DeleteGroup(CGenericGroup* pGroup)
{
	int i;
	CGenericGroup** ppGroup;

	i = 0;
	if (m_nGroupCountA4 > 0) {
		while (m_apGroups04[i] != pGroup) {
			++i;
			if (m_nGroupCountA4 <= i) {
				return;
			}
		}
		if (pGroup != 0) {
			(pGroup->*pGroup->m_pVtable00->m_pDelete00)(1);
		}
		--m_nGroupCountA4;
		if (i < m_nGroupCountA4) {
			ppGroup = &m_apGroups04[i];
			do {
				++i;
				*ppGroup = ppGroup[1];
				++ppGroup;
			} while (i < m_nGroupCountA4);
		}
		m_apGroups04[i] = 0;
	}
}

// FUNCTION: LEMBALL 0x0041ed20
void CGenericGroupManager::AddElementToGroup(CGameObject* pObject, CGenericGroup* pGroup)
{
	CachedChunkObjectManagerVtable* pVtable = (CachedChunkObjectManagerVtable*) m_pVtable00;
	(this->*pVtable->m_pFindElementInGroupAndRemoveIt44)(pObject);
	(pGroup->*pGroup->m_pVtable00->m_pAddElement124)(pObject);
}

// FUNCTION: LEMBALL 0x0041ed40
int CGenericGroupManager::RemoveElementFromGroup(CGameObject* pObject, CGenericGroup* pGroup)
{
	int fRet;
	int i;
	int nLast;
	CGenericGroup** ppGroup;

	fRet = 1;
	if (pGroup != 0) {
		(pGroup->*pGroup->m_pVtable00->m_pRemoveElement128)(pObject);
		if (m_fActiveAC != 0 && (pGroup->*pGroup->m_pVtable00->m_pGetNumberOfElements108)() < 1) {
			(pGroup->*pGroup->m_pVtable00->m_pDelete00)(1);
			i = 0;
			--m_nGroupCountA4;
			fRet = 0;
			do {
				ppGroup = &m_apGroups04[i];
				if (*ppGroup == pGroup) {
					nLast = i;
					if (i < 39) {
						int nRemaining = 39 - i;
						nLast = nRemaining + i;
						do {
							--nRemaining;
							*ppGroup = ppGroup[1];
							++ppGroup;
						} while (nRemaining != 0);
					}
					m_apGroups04[nLast] = 0;
				}
				++i;
			} while (i < 40);
		}
	}
	return fRet;
}

// FUNCTION: LEMBALL 0x0041ede0
void CGenericGroupManager::FindElementInGroupAndRemoveIt(CGameObject* pObject)
{
	CachedChunkObjectManagerVtable* pVtable = (CachedChunkObjectManagerVtable*) m_pVtable00;
	CGenericGroup* pGroup = (this->*pVtable->m_pGetGroupElementIsMemberOf2C)(pObject);
	(this->*pVtable->m_pRemoveElementFromGroup40)(pObject, pGroup);
}

// FUNCTION: LEMBALL 0x0041ee00
int CGenericGroupManager::GetAllBoundingBoxes(tRect* pRects)
{
	CachedChunkObjectManagerVtable* pVtable;
	CGenericGroup* pGroup;
	short anPosition[4];
	int nCount;

	pVtable = (CachedChunkObjectManagerVtable*) m_pVtable00;
	nCount = 0;
	anPosition[0] = 0;
	anPosition[1] = 0;
	anPosition[2] = 0;
	anPosition[3] = 0;
	pGroup = (this->*pVtable->m_pGetFirstGroup04)();
	if (pGroup != 0) {
		do {
			++nCount;
			(pGroup->*pGroup->m_pVtable00->m_pGetBoundingBox48)(anPosition);
			pRects->m_nLeft = anPosition[2];
			pRects->m_nTop = anPosition[3];
			pRects->m_nRight = anPosition[2] + anPosition[0];
			pRects->m_nBottom = anPosition[3] + anPosition[1];
			pGroup = (this->*pVtable->m_pGetNextGroup08)();
			++pRects;
		} while (pGroup != 0);
	}
	return nCount;
}

// FUNCTION: LEMBALL 0x0041ee90
int CGenericGroupManager::GetViewData(CViewData* pViewData)
{
	CachedChunkObjectManagerVtable* pVtable;
	CGenericGroup* pGroup;
	int nCount;

	nCount = 0;
	pVtable = (CachedChunkObjectManagerVtable*) m_pVtable00;
	pGroup = (this->*pVtable->m_pGetFirstGroup04)();
	if (pGroup != 0) {
		do {
			nCount += (pGroup->*pGroup->m_pVtable00->m_pGetViewData158)(
				(CViewData*) ((char*) pViewData + nCount * 0x4c));
			pGroup = (this->*pVtable->m_pGetNextGroup08)();
		} while (pGroup != 0);
	}
	return nCount;
}

// FUNCTION: LEMBALL 0x0041eed0
int CGenericGroupManager::CheckGroupIntersection(CVSRect* pRect, AICOORD* pPosition)
{
	CachedChunkObjectManagerVtable* pVtable;
	CGenericGroup* pGroup;

	pVtable = (CachedChunkObjectManagerVtable*) m_pVtable00;
	pGroup = (this->*pVtable->m_pGetFirstGroup04)();
	while (pGroup != 0) {
		if ((pGroup->*pGroup->m_pVtable00->m_pCheckIntersection15C)(pRect, pPosition) == 1) {
			return 1;
		}
		pGroup = (this->*pVtable->m_pGetNextGroup08)();
	}
	return 0;
}
// FUNCTION: LEMBALL 0x0041f0b0
LinkScfShpgChunkManagerView* LinkScfShpgChunkManagerView::ConstructShpgChunkManager(void* pLevelMode,
																					void* pGmobManager,
																					void* pProjectileGeometryTable)
{
	((CGenericGroupManager*) this)
		->ConstructCachedChunkObjectManagerBaseThunk(pLevelMode, pGmobManager, pProjectileGeometryTable);
	*(void**) this = &g_LINKSCF_ShpgChunkManagerVtable;
	return this;
}

// FUNCTION: LEMBALL 0x00420b80
LinkScfEnmyChunkManagerView* LinkScfEnmyChunkManagerView::ConstructEnmyChunkManager(void* pLevelMode,
																					void* pGmobManager,
																					void* pProjectileGeometryTable)
{
	((CGenericGroupManager*) this)
		->ConstructCachedChunkObjectManagerBaseThunk(pLevelMode, pGmobManager, pProjectileGeometryTable);
	*(void**) this = &g_LINKSCF_EnmyChunkManagerVtable;
	return this;
}
// MACINTOSH: CCollectableManager::CCollectableManager(CAI*, int)
// FUNCTION: LEMBALL 0x004223c0
CCollectableManager* CCollectableManager::ConstructCollChunkManager(void* pLevelMode, int nCapacity)
{
	LINKSCF_INITIALIZE_EFF_CHUNK_STREAM(this, 0x11, 6);
	*(void**) this = g_LINKSCF_CollChunkManagerVtable;
	*(void**) ((char*) this + 0x30) = pLevelMode;
	*(int*) ((char*) this + 0x3c) = 0;
	*(int*) ((char*) this + 0x38) = nCapacity;
	*(int*) ((char*) this + 0x34) = 0;
	return this;
}


void LEMBALL_FASTCALL destroy_lift_chunk_object(void* pObject)
{
	((CLift*) pObject)->~CLift();
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


extern void* g_LINKSCF_RockChunkObjectVtable[16];

// FUNCTION: LEMBALL 0x004267d0
RockChunkObjectView* RockChunkObjectView::ConstructRockChunkObject(void)
{
	InitializeLevelChunkObjectBase(0x20, 0, 0);
	*(void**) this = g_LINKSCF_RockChunkObjectVtable;
	return this;
}

// FUNCTION: LEMBALL 0x00426830
void LEMBALL_FASTCALL DestroyCRocket(void* pObject)
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
	if (*(char**) (pManagerBytes + 0x38) != 0) {
		i = 0;
		if (*(int*) (pManagerBytes + 0x30) > 0) {
			nOffset = 0;
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




// FUNCTION: LEMBALL 0x004297e0
void LEMBALL_FASTCALL reset_lasr_chunk_object_count(void* pManager)
{
	*(int*) ((char*) pManager + 0x34) = 0;
}

// MACINTOSH: CBalloonPost::CBalloonPost(CAI*)
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
// MACINTOSH: CBalloonPost::~CBalloonPost()
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

// MACINTOSH: CTrampoline::CTrampoline()
// FUNCTION: LEMBALL 0x0042a990
TramChunkObjectView* TramChunkObjectView::ConstructTramChunkObject(void)
{
	InitializeLevelChunkObjectBase(0x22, 0, 0);
	*(void**) this = g_LINKSCF_TramChunkObjectVtable;
	return this;
}

// MACINTOSH: CTrampoline::~CTrampoline()
// FUNCTION: LEMBALL 0x0042a9d0
void LEMBALL_FASTCALL DestroyCTrampoline(void* pObject)
{
	*(void**) pObject = g_LINKSCF_TramChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
}

// MACINTOSH: CTrampolineManager::Initialise(int)
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

// FUNCTION: LEMBALL 0x0042afc0
void LinkScfTramChunkManagerView::Restart(void)
{
	int nOffset;
	int i;
	void** pVtable;
	char* pObject;

	if (m_pObjectArray38 == 0) {
		return;
	}
	for (i = 0, nOffset = 0; m_nCapacity30 > i; ++i, nOffset += 0x144) {
		pObject = (char*) m_pObjectArray38 + nOffset;
		pVtable = *(void***) pObject;
		((void(LEMBALL_FASTCALL*)(void*)) pVtable[0x104 / sizeof(void*)])(pObject);
	}
}

// MACINTOSH: CTrampolineManager::Hit(AICOORD&, CGameObject*)
// FUNCTION: LEMBALL 0x0042aff0
void LinkScfTramChunkManagerView::Initialise(int nCapacity)
{
	char* pBase;
	char* pObject;
	int i;

	m_nCapacity30 = nCapacity;
	m_nObjectCount34 = 0;
	if (nCapacity != 0) {
		if (m_pObjectArray38 == 0) {
			pBase = (char*) AllocateVSMemBlock((unsigned int) (nCapacity * 0x144 + 4)) + 4;
			if (pBase != 0) {
				*(int*) (pBase - 4) = nCapacity;
				for (i = nCapacity - 1; i >= 0; --i) {
					((void(__fastcall*)(char*)) 0x403053)(pBase + i * 0x144);
				}
				m_pObjectArray38 = pBase;
			}
			else {
				m_pObjectArray38 = 0;
			}
		}
		for (i = 0; i < m_nCapacity30; ++i) {
			pObject = (char*) m_pObjectArray38 + i * 0x144;
			*(int*) (pObject + 0x60) = (int) this;
			((void(LEMBALL_FASTCALL*)(void*)) (*(void***) pObject)[0x104 / sizeof(void*)])(pObject);
		}
	}
}

// MACINTOSH: CTrampolineManager::Restart()
// FUNCTION: LEMBALL 0x0042b0b0
void LEMBALL_FASTCALL clear_tram_chunk_object_count(void* pManager)
{
	*(int*) ((char*) pManager + 0x34) = 0;
}

// MACINTOSH: CTrampolineManager::Process()
// FUNCTION: LEMBALL 0x0042b4a0
void LEMBALL_FASTCALL ActivateTramChunkObjects(void* pManager)
{
	char* pChunkObject;
	int nOffset;
	int i;

	i = 0;
	if (*(int*) ((char*) pManager + 0x34) > 0) {
		nOffset = 0;
		do {
			*(int*) (*(char**) ((char*) pManager + 0x38) + nOffset + 0x124) = 1;
			pChunkObject = *(char**) ((char*) pManager + 0x38) + nOffset;
			if (*(int*) (pChunkObject + 0x138) != 0) {
				((ActivateChunkObjectProc) (*(void***) pChunkObject)[5])(pChunkObject);
			}
			nOffset += 0x144;
			++i;
		} while (*(int*) ((char*) pManager + 0x34) > i);
	}
}

// FUNCTION: LEMBALL 0x0042b550
int LinkScfTramChunkManagerView::Hit(void* pCoord, void* pGameObject)
{
	int nOffset;
	int i;
	char* pObject;

	if (m_nObjectCount34 <= 0) {
		return 0;
	}
	for (i = 0, nOffset = 0; i < m_nObjectCount34; ++i, nOffset += 0x144) {
		pObject = (char*) m_pObjectArray38 + nOffset;
		if (((int(__stdcall*)(void*, void*, void*)) 0x401ee2)(pObject, pCoord, pGameObject) != 0) {
			return 1;
		}
	}
	return 0;
}

// MACINTOSH: CTrampolineManager::CTrampolineManager(CAI*, int)
// FUNCTION: LEMBALL 0x0042b9b0
void noop_tram_runtime_callback(void)
{
}

extern void* g_LINKSCF_PgunChunkObjectVtable[16];

// MACINTOSH: CPaintGun::CPaintGun()
// FUNCTION: LEMBALL 0x0042bad0
PgunChunkObjectView* PgunChunkObjectView::ConstructPgunChunkObject(void)
{
	InitializeLevelChunkObjectBase(0x21, 0, 0);
	*(void**) this = g_LINKSCF_PgunChunkObjectVtable;
	return this;
}

// MACINTOSH: CPaintGun::~CPaintGun()
// FUNCTION: LEMBALL 0x0042bb20
void LEMBALL_FASTCALL DestroyCPaintGun(void* pObject)
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

// MACINTOSH: CPaintGunManager::CPaintGunManager(CAI*, int)
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

// MACINTOSH: CPaintGunManager::Initialise(int)
// FUNCTION: LEMBALL 0x0042c040
void LEMBALL_FASTCALL ActivatePgunChunkObjects(void* pManager)
{
	char* pManagerBytes;
	void* pChunkObject;
	int i;
	int nOffset;

	pManagerBytes = (char*) pManager;
	if (*(char**) (pManagerBytes + 0x38) != 0) {
		i = 0;
		if (*(int*) (pManagerBytes + 0x30) > 0) {
			nOffset = 0;
			do {
				pChunkObject = *(char**) (pManagerBytes + 0x38) + nOffset;
				++i;
				nOffset += 0x144;
				((ActivateChunkObjectProc) (*(void***) pChunkObject)[65])(pChunkObject);
			} while (*(int*) (pManagerBytes + 0x30) > i);
		}
	}
}

// FUNCTION: LEMBALL 0x0042c070
void LinkScfPgunChunkManagerView::Initialise(int nCapacity)
{
	char* pBase;
	char* pObject;
	int i;

	m_nCapacity30 = nCapacity;
	m_nObjectCount34 = 0;
	if (nCapacity != 0) {
		if (m_pObjectArray38 == 0) {
			pBase = (char*) AllocateVSMemBlock((unsigned int) (nCapacity * 0x144 + 4)) + 4;
			if (pBase != 0) {
				*(int*) (pBase - 4) = nCapacity;
				for (i = nCapacity - 1; i >= 0; --i) {
					((void(__fastcall*)(char*)) 0x401fb9)(pBase + i * 0x144);
				}
				m_pObjectArray38 = pBase;
			}
			else {
				m_pObjectArray38 = 0;
			}
		}
		for (i = 0; i < m_nCapacity30; ++i) {
			pObject = (char*) m_pObjectArray38 + i * 0x144;
			*(int*) (pObject + 0x60) = (int) this;
			((void(LEMBALL_FASTCALL*)(void*)) (*(void***) pObject)[0x104 / sizeof(void*)])(pObject);
		}
	}
}

// MACINTOSH: CPaintGunManager::Restart()
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

// MACINTOSH: CIce::CIce()
// FUNCTION: LEMBALL 0x0042ca70
IceChunkObjectView* IceChunkObjectView::ConstructIceChunkObject(void)
{
	InitializeLevelChunkObjectBase(0x33, 0, 0);
	*(void**) this = g_LINKSCF_IceChunkObjectVtable;
	return this;
}

// MACINTOSH: CIce::~CIce()
// FUNCTION: LEMBALL 0x0042caf0
void LEMBALL_FASTCALL DestroyCIce(void* pObject)
{
	*(void**) pObject = g_LINKSCF_IceChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
}





// MACINTOSH: CMoverManager::CMoverManager(CAI*, int)
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

// MACINTOSH: CMoverManager::Restart()
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

// MACINTOSH: CMoverManager::Process()
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

struct LinkScfSmallFunctionView {
	void SetObjectFields18_1c_20(int nField18, int nField1c, int nField20);
};

struct ChunkRuntimeStateView {
	void ResetRockState(void);
};

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
// WINDOWS: Generated CBall scalar deleting wrapper at 0x00422350; not a portable method.
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
	((CMineManager*) pObject)->~CMineManager();
}
void LEMBALL_FASTCALL DestroyMineChunkManagerAutoThunk(void* pObject)
{
	DestroyMineChunkManagerAuto(pObject);
}

void LEMBALL_FASTCALL DestroyLiftChunkManagerAutoThunk(void* pObject)
{
	((CLiftManager*) pObject)->~CLiftManager();
}

void LEMBALL_FASTCALL DestroyLasrChunkManagerAutoThunk(void* pObject)
{
	((CLaserManager*) pObject)->~CLaserManager();
}

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
void* LEMBALL_FASTCALL DeleteCaptureTriggerChunkManagerAuto(void* pObject, void*, BYTE fDelete)
{
	((CHandManager*) pObject)->~CHandManager();
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}
DEFINE_LINKSCF_NAMED_SCALAR_DELETE(DeleteLasrChunkManagerAuto, DestroyLasrChunkManagerAutoThunk)
DEFINE_LINKSCF_NAMED_SCALAR_DELETE(DeleteTramChunkManagerAuto, DestroyTramChunkManagerAutoThunk)

#undef DEFINE_LINKSCF_NAMED_SCALAR_DELETE

// Compatibility shim for original ILT call site.
void LEMBALL_FASTCALL DestroyLevelChunkStreamAutoThunk(void* pObject)
{
	((CInvisibleSwitchManager*) pObject)->~CInvisibleSwitchManager();
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
	((CInvisibleSwitch*) pObject)->Initialise();
}

void LEMBALL_FASTCALL ResetRockChunkObjectRuntimeStateThunk(void* pObject)
{
	((ChunkRuntimeStateView*) pObject)->ResetRockState();
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


void LEMBALL_FASTCALL ResetLasrChunkObjectRuntimeStateThunk(void* pObject)
{
	((CLaser*) pObject)->Initialise();
}

void LEMBALL_FASTCALL ResetLasrChunkObject(void* pObject)
{
	ResetManagedEntityRuntimeStateThunk(pObject);
	ResetLasrChunkObjectRuntimeStateThunk(pObject);
}
