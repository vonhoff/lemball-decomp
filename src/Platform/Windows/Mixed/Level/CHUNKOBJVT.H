#ifndef LEMBALL_LEVEL_CHUNKOBJVT_H
#define LEMBALL_LEVEL_CHUNKOBJVT_H

struct LevelBoundsPoint;

void* LEMBALL_FASTCALL DeleteInvsChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete);
void* LEMBALL_FASTCALL DeleteDoorChunkObjectAuto(void* pObject, void* pUnused, BYTE fDelete);
void* LEMBALL_FASTCALL DeleteDoorChunkObjectArrayVtableThunk(void* pObject, void* pUnused, BYTE fDelete);
void* LEMBALL_FASTCALL DeleteLevelChunkObjectChildrenVariant14C80(void* pObject, void* pUnused, BYTE fDelete);
void* LEMBALL_FASTCALL DeleteGmobChunkType14ObjectAuto(void* pObject, void* pUnused, BYTE fDelete);
void* LEMBALL_FASTCALL DeleteCollChunkObjectWrapper23060(void* pObject, void* pUnused, BYTE fDelete);
void* LEMBALL_FASTCALL DeleteLiftChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete);
void* LEMBALL_FASTCALL DeleteRockChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete);
void* LEMBALL_FASTCALL DeleteTileFlag1CChunkObjectAuto(void* pObject, void* pUnused, BYTE fDelete);
void* LEMBALL_FASTCALL DeleteCaptureChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete);
void* LEMBALL_FASTCALL DeleteLasrChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete);
void* LEMBALL_FASTCALL DeleteTramChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete);
void* LEMBALL_FASTCALL DeletePgunChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete);
void* LEMBALL_FASTCALL DeleteIceChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete);
void* LEMBALL_FASTCALL DeleteMoveChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete);

void LEMBALL_FASTCALL SetManagedEntityStateIdWithVariant(void* pObject, void* pUnused, int nStateId, int nVariant);
void LEMBALL_FASTCALL SetManagedEntityStateId(void* pObject, void* pUnused, int nStateId);
void LEMBALL_FASTCALL SetManagedEntityStateIdWithVariantLocal(void* pObject,
															  void* pUnused,
															  int nStateId,
															  unsigned short nVariant);
void LEMBALL_FASTCALL SetManagedEntityStateIdWithVariantLocalThunk(void* pObject,
																   void* pUnused,
																   int nStateId,
																   int nVariant);
void __fastcall set_managed_entity_state_id_local(void* pEntity, void* pUnused, int nStateId);
void LEMBALL_FASTCALL EmitLevelChunkObjectRenderEntry(void* pObject, void* pUnused, void* pRenderEntry);
void PASCAL noop_vtable_callback_0040a7f0(void* pUnused);
int PASCAL return_true_vtable_callback_0040a800(void* pUnused);
void* LEMBALL_FASTCALL get_object_field_0xf0(void* pObject);
void LEMBALL_FASTCALL SetLevelChunkObjectMotionVector(void* pObject,
													  void* pUnused,
													  const LevelBoundsPoint* pMotionVector,
													  const LevelBoundsPoint* pInitialPosition);
void LEMBALL_FASTCALL AdvanceManagedEntityGravityMotion(void* pObject);
int return_false_vtable_callback_0040a820(void);
void LEMBALL_FASTCALL copy_object_fields_0x9c_triplet(void* pObject, void* pUnused, LevelBoundsPoint* pPoint);
void noop_vtable_callback_0040a860(void);
void LEMBALL_FASTCALL set_object_field_0x98(void* pObject, void* pUnused, int nValue);
int LEMBALL_FASTCALL get_object_field_0x98(void* pObject);
int LEMBALL_FASTCALL BeginManagedEntityReachablePathSearch(void* pObject);

void LEMBALL_FASTCALL DoActivateCDoorThunk(void* pObject);
int LEMBALL_FASTCALL UpdateManagedEntityGroupCommandState(void* pObject);
int LEMBALL_FASTCALL AdvanceGmobChunkType14StateMachine(void* pObject);
int LEMBALL_FASTCALL AdvanceCollChunkObjectStateMachine(void* pObject);
int LEMBALL_FASTCALL AdvanceLiftChunkObjectStateMachine(void* pObject);
int LEMBALL_FASTCALL AdvanceRockChunkObjectStateMachine(void* pObject);
int LEMBALL_FASTCALL UpdateRockHistoryChildSwapState(void* pObject);
int LEMBALL_FASTCALL AdvanceCaptureChunkObjectStateMachine(void* pObject);
int LEMBALL_FASTCALL AdvanceTramChunkObjectStateMachine(void* pObject);
int LEMBALL_FASTCALL AdvancePgunChunkObjectStateMachine(void* pObject);
int LEMBALL_FASTCALL AdvanceIceChunkObjectStateMachine(void* pObject);
int LEMBALL_FASTCALL AdvanceMoveChunkObjectStateMachine(void* pObject);

void LEMBALL_FASTCALL EmitGmobChunkType14RenderEntry(void* pObject, void* pUnused, void* pRenderEntry);
void LEMBALL_FASTCALL EmitRockChunkObjectRenderEntry(void* pObject, void* pUnused, void* pRenderEntry);
int LEMBALL_FASTCALL ActivateGmobChunkType14Variant(void* pObject, void* pUnused, void* pEntity);
void LEMBALL_FASTCALL CopyGmobChunkType14InteractionPoint(void* pObject, void* pUnused, LevelBoundsPoint* pPoint);
int LEMBALL_FASTCALL RequestRockHistoryChildSwapState(void* pObject, void* pUnused, void* pEntity);
void LEMBALL_FASTCALL GetTileFlag1CObjectRenderPoint(void* pObject, void* pUnused, LevelBoundsPoint* pPoint);
int return_two_vtable_callback_0041dc40(void);
int PASCAL get_tile_flag_0x1c_object_render_entry_count(void);

#define LEVEL_CHUNK_OBJECT_VTABLE(deleteProc,                                                                          \
								  setVariantProc,                                                                      \
								  setStateProc,                                                                        \
								  renderProc,                                                                          \
								  advanceProc,                                                                         \
								  slot6Proc,                                                                           \
								  slot9Proc,                                                                           \
								  slot10Proc,                                                                          \
								  slot11Proc,                                                                          \
								  slot13Proc,                                                                          \
								  slot15Proc)                                                                          \
	{(void*) deleteProc,                                                                                               \
	 (void*) setVariantProc,                                                                                           \
	 (void*) setStateProc,                                                                                             \
	 (void*) renderProc,                                                                                               \
	 (void*) noop_vtable_callback_0040a7f0,                                                                            \
	 (void*) advanceProc,                                                                                              \
	 (void*) slot6Proc,                                                                                                \
	 (void*) get_object_field_0xf0,                                                                                    \
	 (void*) SetLevelChunkObjectMotionVector,                                                                          \
	 (void*) slot9Proc,                                                                                                \
	 (void*) slot10Proc,                                                                                               \
	 (void*) slot11Proc,                                                                                               \
	 (void*) noop_vtable_callback_0040a860,                                                                            \
	 (void*) slot13Proc,                                                                                               \
	 (void*) get_object_field_0x98,                                                                                    \
	 (void*) slot15Proc}

#define COMMON_LEVEL_CHUNK_OBJECT_VTABLE(deleteProc, advanceProc)                                                      \
	LEVEL_CHUNK_OBJECT_VTABLE(deleteProc,                                                                              \
							  SetManagedEntityStateIdWithVariant,                                                      \
							  SetManagedEntityStateId,                                                                 \
							  EmitLevelChunkObjectRenderEntry,                                                         \
							  advanceProc,                                                                             \
							  return_true_vtable_callback_0040a800,                                                    \
							  AdvanceManagedEntityGravityMotion,                                                       \
							  return_false_vtable_callback_0040a820,                                                   \
							  copy_object_fields_0x9c_triplet,                                                         \
							  set_object_field_0x98,                                                                   \
							  BeginManagedEntityReachablePathSearch)

#endif
