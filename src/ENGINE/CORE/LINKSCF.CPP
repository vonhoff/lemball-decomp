#include "ENGINE/CORE/LINKSCF.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/WIN32.H"
#include "ENGINE/GDI/VSGEOM.H"
#include "ENGINE/MEDIA/EFFSTRM.H"
#include "GAME.H"
#include "LEVEL/LEVELRUN.H"

#include <string.h>

// Recovered constructor and loader boundaries that are still being split into
// their original level modules.  Keep unverified addresses free of markers.

struct LevelBoundsPoint;
struct LevelThreeDwordBounds;
struct LevelBoundedRenderObjectView;
struct LevelType18ChunkStream;

extern void* g_pActiveNetworkRuntimeWindow;
extern int g_cbEffTransportMaxPacketBytes;
extern void* g_pActiveLevelGameMode;
extern void* g_pActiveManagedEntityOwner;
extern int g_nLevelFrameClockTick;
extern int g_nLevelFrameClockTimeMs;
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
void LEMBALL_FASTCALL ActivateOrStagePlasChunkObject(void* pObject);

void* LEMBALL_FASTCALL GetFirstCachedChunkObject(void* pObject);
void* LEMBALL_FASTCALL GetNextCachedChunkObject(void* pObject);
int LEMBALL_FASTCALL EmitCachedChunkManagerRenderEntries(void* pObject, void* pUnused, char* pEntries);
int LEMBALL_FASTCALL EmitCachedChunkManagerRenderEntriesThunk(void* pObject, void* pUnused, char* pEntries);

/* These tables retain the recovered common EFF stream interface while the
 * type-specific load and reset methods are reconstructed.  Keeping distinct
 * tables is important: constructors install concrete stream types, and later
 * recovery can replace individual slots without changing object layout. */
#define LINKSCF_SIMPLE_EFF_CHUNK_VTABLE                                                                                \
	{(void*) ReturnTrueVtableCallbackThunk,                                                                            \
	 (void*) ReturnTrueVtableCallbackSecondaryThunk,                                                                   \
	 (void*) DispatchType18ChunkStreamCommandsThunk,                                                                   \
	 (void*) NoopVtableCallbackThunk,                                                                                  \
	 (void*) NoopType18ChunkStreamWriteThunk,                                                                          \
	 (void*) DeleteLevelEffChunkStreamBaseThunk,                                                                       \
	 (void*) noop_vtable_callback_0040a990,                                                                            \
	 (void*) noop_vtable_callback_0040a990,                                                                            \
	 (void*) ReturnFalseChunkStreamCommandThunk,                                                                       \
	 (void*) ReturnFalseChunkStreamRenderThunk}

static void* g_LINKSCF_LiftChunkManagerVtable[10] = LINKSCF_SIMPLE_EFF_CHUNK_VTABLE;
static void* g_LINKSCF_MoveChunkManagerVtable[10] = LINKSCF_SIMPLE_EFF_CHUNK_VTABLE;
static void* g_LINKSCF_GmobChunkManagerVtable[10] = LINKSCF_SIMPLE_EFF_CHUNK_VTABLE;
static void* g_LINKSCF_PlasChunkManagerVtable[10];
static void* g_LINKSCF_PlasChunkStreamVtable[10] = LINKSCF_SIMPLE_EFF_CHUNK_VTABLE;
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
static void* g_LINKSCF_ProjectilePoolVtable[10] = LINKSCF_SIMPLE_EFF_CHUNK_VTABLE;
static void* g_LINKSCF_ManagedEntityQueueCursorVtable[4];
static void* g_LINKSCF_ShpgChunkManagerVtable[20] = {0,
													 (void*) GetFirstCachedChunkObject,
													 (void*) GetNextCachedChunkObject,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 (void*) EmitCachedChunkManagerRenderEntriesThunk};
static void* g_LINKSCF_EnmyChunkManagerVtable[20] = {0,
													 (void*) GetFirstCachedChunkObject,
													 (void*) GetNextCachedChunkObject,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 0,
													 (void*) EmitCachedChunkManagerRenderEntriesThunk};
static void* g_LINKSCF_CollChunkManagerVtable[10] = LINKSCF_SIMPLE_EFF_CHUNK_VTABLE;
static void* g_LINKSCF_MineChunkManagerVtable[10] = LINKSCF_SIMPLE_EFF_CHUNK_VTABLE;
static void* g_LINKSCF_DoorChunkManagerVtable[10] = LINKSCF_SIMPLE_EFF_CHUNK_VTABLE;
static void* g_LINKSCF_RockChunkManagerVtable[10] = LINKSCF_SIMPLE_EFF_CHUNK_VTABLE;
static void* g_LINKSCF_CaptureChunkManagerVtable[10] = LINKSCF_SIMPLE_EFF_CHUNK_VTABLE;
static void* g_LINKSCF_LasrChunkManagerVtable[10] = LINKSCF_SIMPLE_EFF_CHUNK_VTABLE;
static void* g_LINKSCF_TramChunkManagerVtable[10] = LINKSCF_SIMPLE_EFF_CHUNK_VTABLE;
static void* g_LINKSCF_PgunChunkManagerVtable[10] = LINKSCF_SIMPLE_EFF_CHUNK_VTABLE;
static void* g_LINKSCF_IceChunkManagerVtable[10] = LINKSCF_SIMPLE_EFF_CHUNK_VTABLE;
static void* g_LINKSCF_InvsChunkManagerVtable[10] = LINKSCF_SIMPLE_EFF_CHUNK_VTABLE;
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

// FUNCTION: LEMBALL 0x00401604
LinkScfCachedChunkObjectManagerBaseView* LinkScfCachedChunkObjectManagerBaseView::
	ConstructCachedChunkObjectManagerBaseThunk(void* pLevelMode, void* pEntityLookup, void* pProjectileGeometryTable)
{
	return ConstructCachedChunkObjectManagerBase(pLevelMode, pEntityLookup, pProjectileGeometryTable);
}
// FUNCTION: LEMBALL 0x00401677
LinkScfIceChunkManagerView* LinkScfIceChunkManagerView::ConstructIceChunkManagerThunk(void* pLevelMode, int nCapacity)
{
	return ConstructIceChunkManager(pLevelMode, nCapacity);
}
// FUNCTION: LEMBALL 0x00401799
LinkScfShpgChunkManagerView* LinkScfShpgChunkManagerView::ConstructShpgChunkManagerThunk(void* pLevelMode,
																						 void* pGmobManager,
																						 void* pProjectileGeometryTable)
{
	return ConstructShpgChunkManager(pLevelMode, pGmobManager, pProjectileGeometryTable);
}
// FUNCTION: LEMBALL 0x004017b2
void LinkScfLevelChunkLoaderView::LoadLevelChunksForSkillAndLevelThunk(unsigned int nSkill,
																	   unsigned int nLevel,
																	   int fReuseState)
{
	LoadLevelChunksForSkillAndLevel(nSkill, nLevel, fReuseState);
}
// FUNCTION: LEMBALL 0x0040196a
LinkScfDoorChunkManagerView* LinkScfDoorChunkManagerView::ConstructDoorChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructDoorChunkManager(pLevelMode, nCapacity);
}
// FUNCTION: LEMBALL 0x00401a0f
LinkScfMoveChunkManagerView* LinkScfMoveChunkManagerView::ConstructMoveChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructMoveChunkManager(pLevelMode, nCapacity);
}
// FUNCTION: LEMBALL 0x00401e10
LinkScfCaptureChunkManagerView* LinkScfCaptureChunkManagerView::ConstructCaptureTriggerChunkManagerThunk(
	void* pLevelMode,
	int nCapacity)
{
	return ConstructCaptureTriggerChunkManager(pLevelMode, nCapacity);
}
// FUNCTION: LEMBALL 0x00401e88
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
// FUNCTION: LEMBALL 0x00401ea6
LinkScfLiftChunkManagerView* LinkScfLiftChunkManagerView::ConstructLiftChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructLiftChunkManager(pLevelMode, nCapacity);
}
// FUNCTION: LEMBALL 0x00401ece
LinkScfEnmyChunkManagerView* LinkScfEnmyChunkManagerView::ConstructEnmyChunkManagerThunk(void* pLevelMode,
																						 void* pGmobManager,
																						 void* pProjectileGeometryTable)
{
	return ConstructEnmyChunkManager(pLevelMode, pGmobManager, pProjectileGeometryTable);
}
// FUNCTION: LEMBALL 0x00401f64
int LEMBALL_FASTCALL EmitCachedChunkManagerRenderEntriesThunk(void* pObject, void* pUnused, char* pEntries)
{
	return EmitCachedChunkManagerRenderEntries(pObject, pUnused, pEntries);
}
// FUNCTION: LEMBALL 0x0040204a
LinkScfLasrChunkManagerView* LinkScfLasrChunkManagerView::ConstructLasrChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructLasrChunkManager(pLevelMode, nCapacity);
}
// FUNCTION: LEMBALL 0x0040204f
LinkScfBoonChunkManagerView* LinkScfBoonChunkManagerView::ConstructBoonChunkManagerThunk(void* pLevelMode,
																						 void* pTileGrid)
{
	return ConstructBoonChunkManager(pLevelMode, pTileGrid);
}
// FUNCTION: LEMBALL 0x00402af4
LinkScfMineChunkManagerView* LinkScfMineChunkManagerView::ConstructMineChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructMineChunkManager(pLevelMode, nCapacity);
}
// FUNCTION: LEMBALL 0x00402f95
LinkScfPgunChunkManagerView* LinkScfPgunChunkManagerView::ConstructPgunChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructPgunChunkManager(pLevelMode, nCapacity);
}
// FUNCTION: LEMBALL 0x00402fdb
LinkScfCollChunkManagerView* LinkScfCollChunkManagerView::ConstructCollChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructCollChunkManager(pLevelMode, nCapacity);
}
// FUNCTION: LEMBALL 0x004030e9
LinkScfRockChunkManagerView* LinkScfRockChunkManagerView::ConstructRockChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructRockChunkManager(pLevelMode, nCapacity);
}
// FUNCTION: LEMBALL 0x004031a7
LinkScfTramChunkManagerView* LinkScfTramChunkManagerView::ConstructTramChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructTramChunkManager(pLevelMode, nCapacity);
}
// FUNCTION: LEMBALL 0x004032d8
void LinkScfPlasChunkManagerView::LoadPlasChunkObjectsThunk(const unsigned short* pChunk,
															unsigned int cbChunk,
															int fReuseState)
{
	LoadPlasChunkObjects(pChunk, cbChunk, fReuseState);
}
// FUNCTION: LEMBALL 0x0040349f
LinkScfInvsChunkManagerView* LinkScfInvsChunkManagerView::ConstructInvsChunkManagerThunk(void* pLevelMode,
																						 int nCapacity)
{
	return ConstructInvsChunkManager(pLevelMode, nCapacity);
}
// FUNCTION: LEMBALL 0x004034b8
LinkScfGmobChunkManagerView* LinkScfGmobChunkManagerView::ConstructGmobChunkManagerThunk(void* pLevelMode)
{
	return ConstructGmobChunkManager(pLevelMode);
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
LinkScfGmobChunkManagerView* LinkScfGmobChunkManagerView::ConstructGmobChunkManager(void* pLevelMode)
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
	*(void**) this = g_LINKSCF_ShpgChunkManagerVtable;
	g_pCachedChunkManagerLevelMode = pLevelMode;
	g_pCachedChunkManagerEntityLookup = pEntityLookup;
	g_pCachedChunkManagerProjectileGeometryTable = pProjectileGeometryTable;
	*(int*) ((char*) this + 0xa4) = 0;
	*(int*) ((char*) this + 0xa8) = 0;
	*(int*) ((char*) this + 0xac) = 1;
	memset((char*) this + 4, 0, 40 * sizeof(int));
	return this;
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
