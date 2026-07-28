#include "ENGINE/CORE/WIN32.H"
#include "LEVEL/CHUNKOBJVT.H"

// These entries recover the source-owned 16-slot chunk-object interface. Bodies marked STUB remain
// explicit decompilation targets; their distinct identities preserve real virtual dispatch and table ownership.

// GLOBAL: LEMBALL 0x00493120
void* g_LINKSCF_InvsChunkObjectVtable[16] =
	COMMON_LEVEL_CHUNK_OBJECT_VTABLE(DeleteInvsChunkObjectAuto, ServiceInvsChunkObject);

// GLOBAL: LEMBALL 0x00493a60
void* g_LEVELVT_LevelChunkObjectChildrenVariantVtable[16] =
	LEVEL_CHUNK_OBJECT_VTABLE(DeleteLevelChunkObjectChildrenVariant14C80,
							  SetManagedEntityStateIdWithVariantLocalThunk,
							  set_managed_entity_state_id_local,
							  EmitLevelChunkObjectRenderEntry,
							  UpdateManagedEntityGroupCommandState,
							  return_true_vtable_callback_0040a800,
							  AdvanceManagedEntityGravityMotion,
							  return_false_vtable_callback_0040a820,
							  copy_object_fields_0x9c_triplet,
							  set_object_field_0x98,
							  BeginManagedEntityReachablePathSearch);

// GLOBAL: LEMBALL 0x00494a18
void* g_LEVELVT_GmobChunkType14ObjectVtable[16] = LEVEL_CHUNK_OBJECT_VTABLE(DeleteGmobChunkType14ObjectAuto,
																			SetManagedEntityStateIdWithVariant,
																			SetManagedEntityStateId,
																			EmitGmobChunkType14RenderEntry,
																			AdvanceGmobChunkType14StateMachine,
																			ActivateGmobChunkType14Variant,
																			AdvanceManagedEntityGravityMotion,
																			return_two_vtable_callback_0041dc40,
																			CopyGmobChunkType14InteractionPoint,
																			set_object_field_0x98,
																			BeginManagedEntityReachablePathSearch);

// GLOBAL: LEMBALL 0x004959c0
void* g_LEVELVT_CollChunkObjectVtable[16] =
	COMMON_LEVEL_CHUNK_OBJECT_VTABLE(DeleteCollChunkObjectWrapper23060, AdvanceCollChunkObjectStateMachine);

// GLOBAL: LEMBALL 0x00495d60
void* g_LINKSCF_LiftChunkObjectVtable[16] =
	COMMON_LEVEL_CHUNK_OBJECT_VTABLE(DeleteLiftChunkObjectAuto, AdvanceLiftChunkObjectStateMachine);

// GLOBAL: LEMBALL 0x00495ed8
void* g_LINKSCF_RockChunkObjectVtable[16] = LEVEL_CHUNK_OBJECT_VTABLE(DeleteRockChunkObjectAuto,
																	  SetManagedEntityStateIdWithVariant,
																	  SetManagedEntityStateId,
																	  EmitRockChunkObjectRenderEntry,
																	  AdvanceRockChunkObjectStateMachine,
																	  return_true_vtable_callback_0040a800,
																	  AdvanceManagedEntityGravityMotion,
																	  return_false_vtable_callback_0040a820,
																	  copy_object_fields_0x9c_triplet,
																	  set_object_field_0x98,
																	  BeginManagedEntityReachablePathSearch);

// GLOBAL: LEMBALL 0x00496050
void* g_LEVELVT_TileFlag1CChunkObjectVtable[16] =
	LEVEL_CHUNK_OBJECT_VTABLE(DeleteTileFlag1CChunkObjectAuto,
							  SetManagedEntityStateIdWithVariant,
							  SetManagedEntityStateId,
							  EmitLevelChunkObjectRenderEntry,
							  UpdateRockHistoryChildSwapState,
							  RequestRockHistoryChildSwapState,
							  AdvanceManagedEntityGravityMotion,
							  get_tile_flag_0x1c_object_render_entry_count,
							  GetTileFlag1CObjectRenderPoint,
							  set_object_field_0x98,
							  BeginManagedEntityReachablePathSearch);

// GLOBAL: LEMBALL 0x00496198
void* g_LINKSCF_CaptureChunkObjectVtable[16] =
	COMMON_LEVEL_CHUNK_OBJECT_VTABLE(DeleteCaptureChunkObjectAuto, AdvanceCaptureChunkObjectStateMachine);

// GLOBAL: LEMBALL 0x00496310
void* g_LINKSCF_LasrChunkObjectVtable[16] =
	COMMON_LEVEL_CHUNK_OBJECT_VTABLE(DeleteLasrChunkObjectAuto, AdvanceLasrChunkObjectStateMachine);

// GLOBAL: LEMBALL 0x004965c8
void* g_LINKSCF_TramChunkObjectVtable[16] =
	COMMON_LEVEL_CHUNK_OBJECT_VTABLE(DeleteTramChunkObjectAuto, AdvanceTramChunkObjectStateMachine);

// GLOBAL: LEMBALL 0x00496740
void* g_LINKSCF_PgunChunkObjectVtable[16] =
	COMMON_LEVEL_CHUNK_OBJECT_VTABLE(DeletePgunChunkObjectAuto, AdvancePgunChunkObjectStateMachine);

// GLOBAL: LEMBALL 0x004968b8
void* g_LINKSCF_IceChunkObjectVtable[16] =
	COMMON_LEVEL_CHUNK_OBJECT_VTABLE(DeleteIceChunkObjectAuto, AdvanceIceChunkObjectStateMachine);

// GLOBAL: LEMBALL 0x00496a30
void* g_LEVELVT_MoveChunkObjectVtable[16] =
	COMMON_LEVEL_CHUNK_OBJECT_VTABLE(DeleteMoveChunkObjectAuto, AdvanceMoveChunkObjectStateMachine);

// STUB: LEMBALL 0x0040a050
int LEMBALL_FASTCALL ServiceInvsChunkObject(void* pObject)
{
	(void) pObject;
	return 1;
}

// STUB: LEMBALL 0x0040a6f0
void LEMBALL_FASTCALL EmitLevelChunkObjectRenderEntry(void* pObject, void* pUnused, void* pRenderEntry)
{
	(void) pObject;
	(void) pUnused;
	(void) pRenderEntry;
}

// STUB: LEMBALL 0x0040ac60
void* LEMBALL_FASTCALL DeleteInvsChunkObjectAuto(void* pObject, void* pUnused, BYTE fDelete)
{
	(void) pUnused;
	(void) fDelete;
	return pObject;
}

// STUB: LEMBALL 0x00414150
int LEMBALL_FASTCALL UpdateManagedEntityGroupCommandState(void* pObject)
{
	(void) pObject;
	return 1;
}

// STUB: LEMBALL 0x00415300
int LEMBALL_FASTCALL AdvanceManagedEntityGravityMotion(void* pObject)
{
	(void) pObject;
	return 1;
}

// STUB: LEMBALL 0x00415830
int LEMBALL_FASTCALL BeginManagedEntityReachablePathSearch(void* pObject)
{
	(void) pObject;
	return 0;
}

// STUB: LEMBALL 0x00416db0
void LEMBALL_FASTCALL SetManagedEntityStateId(void* pObject, void* pUnused, int nStateId)
{
	(void) pUnused;
	*(int*) ((char*) pObject + 0xb8) = nStateId;
}

void LEMBALL_FASTCALL SetManagedEntityStateIdWithVariantLocal(void* pObject,
															  void* pUnused,
															  int nStateId,
															  unsigned short nVariant)
{
	(void) pUnused;
	*(unsigned short*) ((char*) pObject + 0xbc) = nVariant;
	*(int*) ((char*) pObject + 0xb8) = nStateId;
}

// STUB: LEMBALL 0x00416de0
void LEMBALL_FASTCALL SetManagedEntityStateIdWithVariant(void* pObject,
														 void* pUnused,
														 int nStateId,
														 unsigned short nVariant)
{
	typedef void(LEMBALL_FASTCALL * SetStateProc)(void*, void*, int);
	(void) pUnused;
	*(unsigned short*) ((char*) pObject + 0xbc) = nVariant;
	((SetStateProc) (*(void***) pObject)[2])(pObject, 0, nStateId);
}

// STUB: LEMBALL 0x0041d180
int LEMBALL_FASTCALL AdvanceGmobChunkType14StateMachine(void* pObject)
{
	(void) pObject;
	return 1;
}

// STUB: LEMBALL 0x0041d280
int LEMBALL_FASTCALL ActivateGmobChunkType14Variant(void* pObject)
{
	(void) pObject;
	return 1;
}

// FUNCTION: LEMBALL 0x0041d320
void LEMBALL_FASTCALL CopyGmobChunkType14InteractionPoint(void* pObject, void* pUnused, LevelBoundsPoint* pPoint)
{
	int* pCoordinates = (int*) pPoint;
	(void) pUnused;
	pCoordinates[1] = *(int*) ((char*) pObject + 0xa0);
	pCoordinates[0] = *(int*) ((char*) pObject + 0x9c) - 0x8000;
	pCoordinates[2] = *(int*) ((char*) pObject + 0xa4);
}

// STUB: LEMBALL 0x0041dc50
void LEMBALL_FASTCALL EmitGmobChunkType14RenderEntry(void* pObject, void* pUnused, void* pRenderEntry)
{
	(void) pObject;
	(void) pUnused;
	(void) pRenderEntry;
}

// STUB: LEMBALL 0x00422900
int LEMBALL_FASTCALL AdvanceCollChunkObjectStateMachine(void* pObject)
{
	(void) pObject;
	return 1;
}

// STUB: LEMBALL 0x00425100
int LEMBALL_FASTCALL AdvanceLiftChunkObjectStateMachine(void* pObject)
{
	(void) pObject;
	return 1;
}

// STUB: LEMBALL 0x00426710
void* LEMBALL_FASTCALL DeleteLiftChunkObjectAuto(void* pObject, void* pUnused, BYTE fDelete)
{
	(void) pUnused;
	(void) fDelete;
	return pObject;
}

// STUB: LEMBALL 0x004268e0
int LEMBALL_FASTCALL AdvanceRockChunkObjectStateMachine(void* pObject)
{
	(void) pObject;
	return 1;
}

// STUB: LEMBALL 0x004273f0
void LEMBALL_FASTCALL EmitRockChunkObjectRenderEntry(void* pObject, void* pUnused, void* pRenderEntry)
{
	(void) pObject;
	(void) pUnused;
	(void) pRenderEntry;
}

// STUB: LEMBALL 0x00427520
void* LEMBALL_FASTCALL DeleteRockChunkObjectAuto(void* pObject, void* pUnused, BYTE fDelete)
{
	(void) pUnused;
	(void) fDelete;
	return pObject;
}

// STUB: LEMBALL 0x00427780
int LEMBALL_FASTCALL UpdateRockHistoryChildSwapState(void* pObject)
{
	(void) pObject;
	return 1;
}

// FUNCTION: LEMBALL 0x00427890
void LEMBALL_FASTCALL GetTileFlag1CObjectRenderPoint(void* pObject, void* pUnused, LevelBoundsPoint* pPoint)
{
	int* pCoordinates = (int*) pPoint;
	(void) pUnused;
	pCoordinates[0] = *(int*) ((char*) pObject + 0x9c);
	pCoordinates[1] = *(int*) ((char*) pObject + 0xa0) + 0x8000;
	pCoordinates[2] = *(int*) ((char*) pObject + 0xa4);
}

// STUB: LEMBALL 0x004278c0
int LEMBALL_FASTCALL RequestRockHistoryChildSwapState(void* pObject, void* pUnused, void* pEntity)
{
	(void) pObject;
	(void) pUnused;
	(void) pEntity;
	return 0;
}

// STUB: LEMBALL 0x00427c40
int LEMBALL_FASTCALL AdvanceCaptureChunkObjectStateMachine(void* pObject)
{
	(void) pObject;
	return 1;
}

// STUB: LEMBALL 0x00428800
void* LEMBALL_FASTCALL DeleteCaptureChunkObjectAuto(void* pObject, void* pUnused, BYTE fDelete)
{
	(void) pUnused;
	(void) fDelete;
	return pObject;
}

// STUB: LEMBALL 0x00428cf0
int LEMBALL_FASTCALL AdvanceLasrChunkObjectStateMachine(void* pObject)
{
	(void) pObject;
	return 1;
}

// STUB: LEMBALL 0x00429ed0
void* LEMBALL_FASTCALL DeleteLasrChunkObjectAuto(void* pObject, void* pUnused, BYTE fDelete)
{
	(void) pUnused;
	(void) fDelete;
	return pObject;
}

// STUB: LEMBALL 0x0042aa80
int LEMBALL_FASTCALL AdvanceTramChunkObjectStateMachine(void* pObject)
{
	(void) pObject;
	return 1;
}

// STUB: LEMBALL 0x0042ba40
void* LEMBALL_FASTCALL DeleteTramChunkObjectAuto(void* pObject, void* pUnused, BYTE fDelete)
{
	(void) pUnused;
	(void) fDelete;
	return pObject;
}

// STUB: LEMBALL 0x0042bbe0
int LEMBALL_FASTCALL AdvancePgunChunkObjectStateMachine(void* pObject)
{
	(void) pObject;
	return 1;
}

// STUB: LEMBALL 0x0042c9e0
void* LEMBALL_FASTCALL DeletePgunChunkObjectAuto(void* pObject, void* pUnused, BYTE fDelete)
{
	(void) pUnused;
	(void) fDelete;
	return pObject;
}

// STUB: LEMBALL 0x0042cd70
int LEMBALL_FASTCALL AdvanceIceChunkObjectStateMachine(void* pObject)
{
	(void) pObject;
	return 1;
}

// STUB: LEMBALL 0x0042e500
void* LEMBALL_FASTCALL DeleteIceChunkObjectAuto(void* pObject, void* pUnused, BYTE fDelete)
{
	(void) pUnused;
	(void) fDelete;
	return pObject;
}

// STUB: LEMBALL 0x0042eb00
int LEMBALL_FASTCALL AdvanceMoveChunkObjectStateMachine(void* pObject)
{
	(void) pObject;
	return 1;
}
// STUB: LEMBALL 0x0042fbd0
void* LEMBALL_FASTCALL DeleteMoveChunkObjectAuto(void* pObject, void* pUnused, BYTE fDelete)
{
	(void) pUnused;
	(void) fDelete;
	return pObject;
}
