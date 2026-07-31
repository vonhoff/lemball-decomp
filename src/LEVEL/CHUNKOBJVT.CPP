#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/WIN32.H"
#include "LEVEL/CHUNKOBJVT.H"

extern int g_nLevelFrameClockTimeMs;
extern int g_nLevelFrameClockTick;
extern int g_nSelectedNetworkLobbyPeerId;
extern void* g_pLiftTileGrid;
extern void* g_pLevelTileGrid;
extern void* g_pActiveManagedEntityOwner;
extern int __fastcall has_level_plas_history_entries(void* pLevelMode);
extern int LEMBALL_FASTCALL PopLevelPlasHistoryChild(void* pLevelMode);
extern void LEMBALL_FASTCALL clear_managed_entity_child_pending_state_if_interruptible(void* pEntity);
extern void* LEMBALL_FASTCALL get_managed_entity_owner_group(void* pEntity);
extern int LEMBALL_FASTCALL AcquireLasrPlasTarget(void* pObject);
extern int LEMBALL_FASTCALL BeginLasrPlasReacquireDelay(void* pObject);
extern void LEMBALL_FASTCALL destroy_invs_chunk_object(void* pObject);
extern void LEMBALL_FASTCALL destroy_lift_chunk_object(void* pObject);
extern void LEMBALL_FASTCALL destroy_rock_chunk_object(void* pObject);
extern void LEMBALL_FASTCALL destroy_capture_trigger_chunk_object(void* pObject);
extern void LEMBALL_FASTCALL destroy_lasr_chunk_object_vtable_thunk(void* pObject);
extern void LEMBALL_FASTCALL destroy_tram_chunk_object(void* pObject);
extern void LEMBALL_FASTCALL destroy_pgun_chunk_object_vtable_thunk(void* pObject);
extern void LEMBALL_FASTCALL destroy_ice_chunk_object(void* pObject);
extern void LEMBALL_FASTCALL destroy_move_chunk_object(void* pObject);
extern void LEMBALL_FASTCALL RemoveInvsTrackedEntitiesOutsideBounds(void* pObject);

struct ManagedEntityPacketStreamView {
	void BindManagedEntityPacketTarget(void* pEntity);
};

struct ManagedEntityStateView {
	void RequestManagedEntityStateId(int nStateId);
};

struct DoorChunkObjectActionView {
	void RequestDoorChunkObjectTriggerTransition(void);
};

struct ManagedEntityLocalStateView {
	virtual void ReservedSlot0(int nValue);
	virtual void ReservedSlot1(int nValue);
	virtual void SetManagedEntityStateIdLocal(int nStateId);
};

struct LevelPoint3 {
	int m_nX;
	int m_nY;
	int m_nZ;
};

struct ManagedProjectileObjectView {
	void ActivateAtPosition(const LevelPoint3* pPosition);
};

struct ManagedEntityChildView;

struct LevelManagedEntityChildIterator {
	int AttachManagedEntityChild(ManagedEntityChildView* pChild);
};

struct RockChunkObjectStateView {
	virtual void ReservedSlot0(int nValue);
	virtual void ReservedSlot1(int nValue);
	virtual void SetManagedEntityStateIdLocal(int nStateId);
	virtual void ReservedSlot3(int nValue);
	virtual void ReservedSlot4(int nValue);
	virtual void ReservedSlot5(int nValue);
	virtual void ReservedSlot6(int nValue);
	virtual void ReservedSlot7(int nValue);
	virtual void ReservedSlot8(int nValue);
	virtual void ReservedSlot9(int nValue);
	virtual void ReservedSlot10(int nValue);
	virtual void ReservedSlot11(int nValue);
	virtual void ReservedSlot12(int nValue);
	virtual void SetActionState(int nStateId);
};

struct LevelTileRenderDescriptor {
	short SampleHeightOffset(int nLocalX, int nLocalY);
};

struct GmobChunkType14ActionView {
	virtual void ReservedSlot0(int nValue);
	virtual void ReservedSlot1(int nValue);
	virtual void SetManagedEntityStateId(int nStateId);
	virtual void ReservedSlot3(int nValue);
	virtual void ReservedSlot4(int nValue);
	virtual void ReservedSlot5(int nValue);
	virtual void ReservedSlot6(int nValue);
	virtual void ReservedSlot7(int nValue);
	virtual void ReservedSlot8(int nValue);
	virtual void ReservedSlot9(int nValue);
	virtual void ReservedSlot10(int nValue);
	virtual void ReservedSlot11(int nValue);
	virtual void ReservedSlot12(int nValue);
	virtual void SetActionState(int nStateId);
	void DispatchGmobChunkType14Actions(void);
};

struct LevelSlotActionDispatcherView {
	void DispatchLevelSlotActionByType(int nAction, unsigned int nSlotId, int nRangeEnd, int nUnused4);
};

struct InvsChunkObjectActionView {
	virtual void ReservedSlot0(int nValue);
	virtual void ReservedSlot1(int nValue);
	virtual void SetManagedEntityStateId(int nStateId);
	virtual void ReservedSlot3(int nValue);
	virtual void ReservedSlot4(int nValue);
	virtual void ReservedSlot5(int nValue);
	virtual void ReservedSlot6(int nValue);
	virtual void ReservedSlot7(int nValue);
	virtual void ReservedSlot8(int nValue);
	virtual void ReservedSlot9(int nValue);
	virtual void ReservedSlot10(int nValue);
	virtual void ReservedSlot11(int nValue);
	virtual void ReservedSlot12(int nValue);
	virtual void SetActionState(int nStateId);
	void AddInvsTrackedEntity(void* pEntity);
};

struct InvsChunkActionRecord {
	int m_nAction;
	unsigned short m_nSlotId;
	unsigned short m_nReserved;
};

extern ManagedEntityPacketStreamView* g_pManagedEntityPacketBindStream;

#define DELETE_CHUNK_OBJECT_ARRAY_OR_SCALAR(pObject, fDelete, objectSize, destroyProc)                                 \
	char* pAllocation;                                                                                                 \
	char* pCursor;                                                                                                     \
	if (((fDelete) & 2) != 0) {                                                                                        \
		pAllocation = (char*) pObject - sizeof(int);                                                                   \
		(fDelete) = *(unsigned int*) pAllocation;                                                                      \
		pCursor = (char*) pObject + (fDelete) * (objectSize);                                                          \
		while ((int) --(fDelete) >= 0) {                                                                               \
			pCursor -= (objectSize);                                                                                   \
			destroyProc(pCursor);                                                                                      \
		}                                                                                                              \
		FreeVSMemBlock(pAllocation);                                                                                   \
		return pObject;                                                                                                \
	}                                                                                                                  \
	destroyProc(pObject);                                                                                              \
	if (((fDelete) & 1) != 0) {                                                                                        \
		FreeVSMemBlock(pObject);                                                                                       \
	}                                                                                                                  \
	return pObject

#define DELETE_CHUNK_OBJECT_ARRAY_OR_SCALAR_CURSOR_FIRST(pObject, fDelete, objectSize, destroyProc)                    \
	char* pCursor;                                                                                                     \
	char* pAllocation;                                                                                                 \
	if (((fDelete) & 2) != 0) {                                                                                        \
		pAllocation = (char*) pObject - sizeof(int);                                                                   \
		(fDelete) = *(unsigned int*) pAllocation;                                                                      \
		pCursor = (char*) pObject + (fDelete) * (objectSize);                                                          \
		while ((int) --(fDelete) >= 0) {                                                                               \
			pCursor -= (objectSize);                                                                                   \
			destroyProc(pCursor);                                                                                      \
		}                                                                                                              \
		FreeVSMemBlock(pAllocation);                                                                                   \
		return pObject;                                                                                                \
	}                                                                                                                  \
	destroyProc(pObject);                                                                                              \
	if (((fDelete) & 1) != 0) {                                                                                        \
		FreeVSMemBlock(pObject);                                                                                       \
	}                                                                                                                  \
	return pObject

// These entries recover the source-owned 16-slot chunk-object interface. Bodies marked STUB remain
// explicit decompilation targets; their distinct identities preserve real virtual dispatch and table ownership.

// GLOBAL: LEMBALL 0x00493120
void* g_LINKSCF_InvsChunkObjectVtable[16] =
	COMMON_LEVEL_CHUNK_OBJECT_VTABLE(DeleteInvsChunkObjectAuto, ServiceInvsChunkObject);

// GLOBAL: LEMBALL 0x004936f8
void* g_LEVELVT_DoorChunkObjectVtable[16] =
	COMMON_LEVEL_CHUNK_OBJECT_VTABLE(DeleteDoorChunkObjectArrayVtableThunk,
									 RequestDoorChunkObjectTriggerTransitionThunk);

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

// GLOBAL: LEMBALL 0x0049ce0c
int g_nNetworkFrameClockTick = 0;

// GLOBAL: LEMBALL 0x0049ce10
int g_nNetworkFrameClockTimeMs = 0;

// FUNCTION: LEMBALL 0x0040a050
int LEMBALL_FASTCALL ServiceInvsChunkObject(void* pObject)
{
	char* pObjectBytes = (char*) pObject;
	int nStateId;

	RemoveInvsTrackedEntitiesOutsideBounds(pObject);
	if (*(int*) (pObjectBytes + 0x114) != 0) {
		nStateId = *(int*) (pObjectBytes + 0xb8);
		if (*(int*) (pObjectBytes + 0x128) != nStateId && nStateId != 0x1a) {
			return 1;
		}
	}

	if (*(int*) (pObjectBytes + 0xb8) == 0x1a) {
		if (*(int*) (pObjectBytes + 0x144) == 0) {
			*(int*) (pObjectBytes + 0x148) = 1;
		}
		if (*(void**) (pObjectBytes + 0x5c) != 0) {
			((InvsChunkObjectActionView*) pObject)->AddInvsTrackedEntity(*(void**) (pObjectBytes + 0x5c));
			*(void**) (pObjectBytes + 0x5c) = 0;
		}
		{
			InvsChunkActionRecord* pAction;
			int iAction = 0;
			if (*(unsigned short*) (pObjectBytes + 0x150) > iAction) {
				pAction = (InvsChunkActionRecord*) (pObjectBytes + 0x154);
				do {
					((LevelSlotActionDispatcherView*) g_pActiveManagedEntityOwner)
						->DispatchLevelSlotActionByType(pAction->m_nAction, pAction->m_nSlotId, 0, 0);
					++pAction;
					++iAction;
				} while (*(unsigned short*) (pObjectBytes + 0x150) > iAction);
			}
		}
		((InvsChunkObjectActionView*) pObject)->SetActionState(0x15);
		((InvsChunkObjectActionView*) pObject)->SetManagedEntityStateId(0x18);
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0040a6f0
void LEMBALL_FASTCALL EmitLevelChunkObjectRenderEntry(void* pObject, void* pUnused, void* pRenderEntry)
{
	typedef void(LEMBALL_FASTCALL * SetObjectFieldProc)(void*, void*, int);
	char* pEntryBytes = (char*) pRenderEntry;
	char* pObjectBytes = (char*) pObject;
	unsigned short nVariant;
	int nStateId;
	int nStateTimeMs;
	int nFrameClockTimeMs;

	(void) pUnused;
	*(unsigned short*) (pEntryBytes + 0x2c) = *(unsigned short*) (pObjectBytes + 0x6a);
	*(int*) (pEntryBytes + 0x28) = *(int*) (pObjectBytes + 0x64);
	*(unsigned short*) (pEntryBytes + 0x2e) = 0;
	*(int*) (pEntryBytes + 0x04) = *(int*) (pObjectBytes + 0x9c) >> 12;
	*(int*) (pEntryBytes + 0x08) = *(int*) (pObjectBytes + 0xa0) >> 12;
	*(int*) (pEntryBytes + 0x0c) = *(int*) (pObjectBytes + 0xa4) >> 12;
	*(unsigned short*) pEntryBytes = *(unsigned short*) (pObjectBytes + 0xb4);
	nVariant = *(unsigned short*) (pObjectBytes + 0xbc);
	nStateTimeMs = *(int*) (pObjectBytes + 0x94);
	nStateId = *(int*) (pObjectBytes + 0xb8);
	*(unsigned short*) (pEntryBytes + 0x1c) = nVariant;
	*(int*) (pEntryBytes + 0x18) = nStateId;
	*(int*) (pEntryBytes + 0x20) = nStateTimeMs;
	*(int*) (pEntryBytes + 0x14) = 0;
	*(int*) (pEntryBytes + 0x30) = *(int*) (pObjectBytes + 0xc0);
	*(int*) (pEntryBytes + 0x38) = *(int*) (pObjectBytes + 0xe4);
	*(int*) (pEntryBytes + 0x3c) = *(int*) (pObjectBytes + 0xe8);
	*(int*) (pEntryBytes + 0x40) = *(int*) (pObjectBytes + 0xec);
	*(int*) (pEntryBytes + 0x44) = *(int*) (pObjectBytes + 0x98);
	nFrameClockTimeMs = g_nLevelFrameClockTimeMs;
	if (*(int*) (pObjectBytes + 0x114) != 0) {
		nFrameClockTimeMs = g_nNetworkFrameClockTimeMs;
	}
	*(int*) (pEntryBytes + 0x24) = nFrameClockTimeMs;
	((SetObjectFieldProc) (*(void***) pObject)[13])(pObject, 0, 0);
	*(int*) (pEntryBytes + 0x34) = *(int*) (pObjectBytes + 0x10);
	*(int*) (pObjectBytes + 0x10) = 0;
}

// FUNCTION: LEMBALL 0x0040ac60
void* LEMBALL_FASTCALL DeleteInvsChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete)
{
	(void) pUnused;
	DELETE_CHUNK_OBJECT_ARRAY_OR_SCALAR(pObject, fDelete, 0x2b8, destroy_invs_chunk_object);
}

// STUB: LEMBALL 0x00414150
int LEMBALL_FASTCALL UpdateManagedEntityGroupCommandState(void* pObject)
{
	(void) pObject;
	return 1;
}

// STUB: LEMBALL 0x00415300
void LEMBALL_FASTCALL AdvanceManagedEntityGravityMotion(void* pObject)
{
	(void) pObject;
}

// STUB: LEMBALL 0x00415830
int LEMBALL_FASTCALL BeginManagedEntityReachablePathSearch(void* pObject)
{
	(void) pObject;
	return 0;
}

// FUNCTION: LEMBALL 0x00416db0
void LEMBALL_FASTCALL SetManagedEntityStateId(void* pObject, void* pUnused, int nStateId)
{
	char* pObjectBytes = (char*) pObject;
	(void) pUnused;
	*(int*) (pObjectBytes + 0xb8) = nStateId;
	if (g_nSelectedNetworkLobbyPeerId != 0) {
		g_pManagedEntityPacketBindStream->BindManagedEntityPacketTarget(pObject);
	}
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

// FUNCTION: LEMBALL 0x00416de0
void LEMBALL_FASTCALL SetManagedEntityStateIdWithVariant(void* pObject, void* pUnused, int nStateId, int nVariant)
{
	(void) pUnused;
	*(unsigned short*) ((char*) pObject + 0xbc) = (unsigned short) nVariant;
	((ManagedEntityLocalStateView*) pObject)->SetManagedEntityStateIdLocal(nStateId);
}

// FUNCTION: LEMBALL 0x0041d180
int LEMBALL_FASTCALL AdvanceGmobChunkType14StateMachine(void* pObject)
{
	char* pObjectBytes = (char*) pObject;
	char* pGrid = (char*) g_pLevelTileGrid;
	int nLocalY = *(int*) (pObjectBytes + 0xa0) >> 12;
	int nLocalX = *(int*) (pObjectBytes + 0x9c) >> 12;
	int nTileX = nLocalX >> 4;
	int nTileY = nLocalY >> 4;
	unsigned short nHeightOffset;
	int nStateId;

	if (nLocalX < 0 || nLocalY < 0 || nTileX >= *(int*) (pGrid + 0x10) || nTileY >= *(int*) (pGrid + 0x14)) {
		nHeightOffset = 0;
	}
	else {
		LevelTileRenderDescriptor* pTile =
			(LevelTileRenderDescriptor*) (*(char**) (pGrid + 0x0c) + (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 12);
		nHeightOffset = (unsigned short) pTile->SampleHeightOffset(nLocalX & 15, nLocalY & 15);
	}
	*(int*) (pObjectBytes + 0xa4) = nHeightOffset << 12;
	nStateId = *(int*) (pObjectBytes + 0xb8);

	if (*(int*) (pObjectBytes + 0x114) != 0) {
		if (*(int*) (pObjectBytes + 0x128) != nStateId) {
			if (nStateId == 7) {
				((GmobChunkType14ActionView*) pObject)->SetActionState(0x15);
			}
			*(int*) (pObjectBytes + 0x128) = *(int*) (pObjectBytes + 0xb8);
		}
		return 1;
	}

	switch (nStateId) {
	case 7:
		((GmobChunkType14ActionView*) pObject)->DispatchGmobChunkType14Actions();
		nStateId = 0x18;
		break;
	case 0x19:
		return 1;
	case 0x1a:
		if ((unsigned int) g_nLevelFrameClockTick <= *(unsigned int*) (pObjectBytes + 0xd4)) {
			return 1;
		}
		nStateId = 7;
		break;
	default:
		return 1;
	}
	((GmobChunkType14ActionView*) pObject)->SetManagedEntityStateId(nStateId);
	return 1;
}

// FUNCTION: LEMBALL 0x0041d280
int LEMBALL_FASTCALL ActivateGmobChunkType14Variant(void* pObject, void* pUnused, void* pEntity)
{
	char* pObjectBytes = (char*) pObject;
	unsigned int nVariant = *(unsigned short*) (pObjectBytes + 0xbc);

	(void) pUnused;
	(void) pEntity;
	switch (nVariant) {
	case 0:
		*(int*) (pObjectBytes + 0xd4) = 0x14;
		*(unsigned short*) (pObjectBytes + 0xbc) = 1;
		((ManagedEntityStateView*) pObject)->RequestManagedEntityStateId(0x1a);
		return 1;
	case 1:
		*(int*) (pObjectBytes + 0xd4) = 0x14;
		*(unsigned short*) (pObjectBytes + 0xbc) = 0;
		((ManagedEntityStateView*) pObject)->RequestManagedEntityStateId(0x1a);
		return 1;
	default:
		return 1;
	}
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

// FUNCTION: LEMBALL 0x0041dc50
void LEMBALL_FASTCALL EmitGmobChunkType14RenderEntry(void* pObject, void* pUnused, void* pRenderEntry)
{
	EmitLevelChunkObjectRenderEntry(pObject, pUnused, pRenderEntry);
	*(unsigned short*) ((char*) pRenderEntry + 0x2e) = *(unsigned short*) ((char*) pObject + 0x148);
}

// FUNCTION: LEMBALL 0x00422900
int LEMBALL_FASTCALL AdvanceCollChunkObjectStateMachine(void* pObject)
{
	typedef void(LEMBALL_FASTCALL * ObjectSlotProc)(void*, void*, int);
	typedef void(LEMBALL_FASTCALL * ObjectNoArgSlotProc)(void*);
	typedef int(LEMBALL_FASTCALL * CollisionTestProc)(void*, void*, const int*);
	typedef int(LEMBALL_FASTCALL * EntityTypeTestProc)(void*, void*, int);
	char* pObjectBytes = (char*) pObject;
	char* pOwnerBytes = (char*) g_pActiveManagedEntityOwner;
	int nStateId;

	if (*(int*) (pObjectBytes + 0x114) != 0) {
		nStateId = *(int*) (pObjectBytes + 0xb8);
		if (*(int*) (pObjectBytes + 0x128) != nStateId) {
			if (nStateId == 8) {
				*(int*) (pObjectBytes + 0x138) = 0;
			}
			else if (nStateId == 0x1a) {
				((ObjectNoArgSlotProc) (*(void***) pObject)[68])(pObject);
			}
			*(int*) (pObjectBytes + 0x128) = *(int*) (pObjectBytes + 0xb8);
		}
		return 1;
	}

	if (*(int*) (pObjectBytes + 0x138) == 0) {
		return 1;
	}

	switch (*(int*) (pObjectBytes + 0xb8)) {
	case 8:
		*(int*) (pObjectBytes + 0x138) = 0;
		break;
	case 0x18:
		if (g_nSelectedNetworkLobbyPeerId != 0 && *(int*) (pObjectBytes + 0x12c) != 0x18) {
			break;
		}
		if (*(int*) (pObjectBytes + 0x11c) == 0) {
			char* pGrid = (char*) g_pLevelTileGrid;
			int nLocalX = *(int*) (pObjectBytes + 0x9c) >> 12;
			int nLocalY = *(int*) (pObjectBytes + 0xa0) >> 12;
			short nHeight = 0;
			int nTileX = nLocalX >> 4;
			int nTileY = nLocalY >> 4;
			if (nLocalX >= 0 && nLocalY >= 0 && nTileX < *(int*) (pGrid + 0x10) && nTileY < *(int*) (pGrid + 0x14)) {
				LevelTileRenderDescriptor* pTile =
					(LevelTileRenderDescriptor*) (*(char**) (pGrid + 0x0c) +
												  (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 12);
				nHeight = pTile->SampleHeightOffset(nLocalX & 0x0f, nLocalY & 0x0f);
			}
			*(int*) (pObjectBytes + 0xa4) = (unsigned short) nHeight << 12;
		}
		{
			int* pPoint = (int*) (pOwnerBytes + 0x124);
			int iCandidate;
			void* pCandidate = 0;
			pPoint[0] = *(int*) (pObjectBytes + 0x9c) >> 12;
			pPoint[1] = *(int*) (pObjectBytes + 0xa0) >> 12;
			pPoint[2] = *(int*) (pObjectBytes + 0xa4) >> 12;
			*(void**) (pOwnerBytes + 0x150) = 0;
			*(int*) (pOwnerBytes + 0x130) = 0;
			while (*(int*) (pOwnerBytes + 0x130) < *(int*) (pOwnerBytes + 0x118)) {
				iCandidate = *(int*) (pOwnerBytes + 0x130);
				void* pCurrent = (*(void***) (pOwnerBytes + 0x120))[iCandidate];
				if (*(void**) (pOwnerBytes + 0x150) != pCurrent &&
					((CollisionTestProc) (*(void***) pCurrent)[20])(pCurrent, 0, pPoint) != 0) {
					pCandidate = pCurrent;
					*(void**) (pOwnerBytes + 0x150) = pCurrent;
					++*(int*) (pOwnerBytes + 0x130);
					break;
				}
				++*(int*) (pOwnerBytes + 0x130);
			}
			if (pCandidate != 0 && *(int*) ((char*) pCandidate + 0x64) == 2 &&
				((EntityTypeTestProc) (*(void***) pCandidate)[45])(pCandidate, 0, *(int*) (pObjectBytes + 0x64)) == 0) {
				*(void**) (pObjectBytes + 0x5c) = pCandidate;
				((ManagedEntityStateView*) pObject)->RequestManagedEntityStateId(0x1a);
			}
		}
		break;
	case 0x1a:
		((ObjectNoArgSlotProc) (*(void***) pObject)[69])(pObject);
		((ObjectNoArgSlotProc) (*(void***) pObject)[68])(pObject);
		((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 8);
		break;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00426710
void* LEMBALL_FASTCALL DeleteLiftChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete)
{
	(void) pUnused;
	DELETE_CHUNK_OBJECT_ARRAY_OR_SCALAR(pObject, fDelete, 0x190, destroy_lift_chunk_object);
}

// FUNCTION: LEMBALL 0x004268e0
int LEMBALL_FASTCALL AdvanceRockChunkObjectStateMachine(void* pObject)
{
	typedef void(LEMBALL_FASTCALL * ObjectSlotProc)(void*, void*, int);
	int* pFields = (int*) pObject;
	int nTick = pFields[0x45] != 0 ? g_nNetworkFrameClockTick : g_nLevelFrameClockTick;
	int nState = pFields[0x2e];

	if (nState == 4) {
		pFields[0x29] = ((nTick - pFields[0x32]) * 10 + pFields[0x50]) * 0x1000;
	}
	if (pFields[0x45] == 0) {
		if (nState == 4) {
			if ((pFields[0x29] & 0xfffff000) > 0xc8000) {
				((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 0x18);
			}
		}
		else if (nState == 0x1b && (unsigned int) pFields[0x32] < (unsigned int) g_nLevelFrameClockTick) {
			((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 4);
		}
	}
	else if (pFields[0x4a] != nState) {
		if (nState == 0x1b) {
			pFields[0x32] = nTick + 0x30;
			pFields[0x50] = pFields[0x29] >> 12;
			((ObjectSlotProc) (*(void***) pObject)[13])(pObject, 0, 0x12);
		}
		pFields[0x4a] = pFields[0x2e];
	}
	return 1;
}

// FUNCTION: LEMBALL 0x004273f0
void LEMBALL_FASTCALL EmitRockChunkObjectRenderEntry(void* pObject, void* pUnused, void* pRenderEntry)
{
	EmitLevelChunkObjectRenderEntry(pObject, pUnused, pRenderEntry);
}

// FUNCTION: LEMBALL 0x00427520
void* LEMBALL_FASTCALL DeleteRockChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete)
{
	(void) pUnused;
	DELETE_CHUNK_OBJECT_ARRAY_OR_SCALAR(pObject, fDelete, 0x144, destroy_rock_chunk_object);
}

// FUNCTION: LEMBALL 0x00427780
int LEMBALL_FASTCALL UpdateRockHistoryChildSwapState(void* pObject)
{
	char* pObjectBytes = (char*) pObject;
	char* pChildBytes;
	ManagedProjectileObjectView* pProjectile;
	LevelPoint3 activationPoint;
	int nState;

	if (*(int*) (pObjectBytes + 0x114) != 0) {
		*(unsigned short*) (pObjectBytes + 0xbc) = 1;
		if (*(int*) (pObjectBytes + 0x128) != *(int*) (pObjectBytes + 0xb8)) {
			if (*(int*) (pObjectBytes + 0xb8) == 0x1a) {
				((RockChunkObjectStateView*) pObject)->SetActionState(0x0c);
			}
			*(int*) (pObjectBytes + 0x128) = *(int*) (pObjectBytes + 0xb8);
		}
		return 1;
	}

	nState = *(int*) (pObjectBytes + 0xb8);
	*(unsigned short*) (pObjectBytes + 0xbc) = 0;
	if (nState == 0x1a && *(unsigned int*) (pObjectBytes + 0xcc) < (unsigned int) g_nLevelFrameClockTick) {
		pChildBytes = (char*) *(void**) (pObjectBytes + 0x140);
		*(int*) (pChildBytes + 0xc0) = 0;
		*(int*) (pChildBytes + 0xb8) = 0;
		((ManagedEntityLocalStateView*) *(void**) (pObjectBytes + 0x140))->SetManagedEntityStateIdLocal(0);
		clear_managed_entity_child_pending_state_if_interruptible(*(void**) (pObjectBytes + 0x140));
		pProjectile = (ManagedProjectileObjectView*) PopLevelPlasHistoryChild(g_pActiveManagedEntityOwner);
		if (pProjectile != 0) {
			activationPoint.m_nX = *(int*) (pObjectBytes + 0x9c);
			activationPoint.m_nY = *(int*) (pObjectBytes + 0xa0) - 0x34000;
			activationPoint.m_nZ = *(int*) (pObjectBytes + 0xa4);
			pProjectile->ActivateAtPosition(&activationPoint);
			((LevelManagedEntityChildIterator*) get_managed_entity_owner_group(*(void**) (pObjectBytes + 0x140)))
				->AttachManagedEntityChild((ManagedEntityChildView*) pProjectile);
		}
		((RockChunkObjectStateView*) pObject)->SetManagedEntityStateIdLocal(0x18);
	}
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

// FUNCTION: LEMBALL 0x004278c0
int LEMBALL_FASTCALL RequestRockHistoryChildSwapState(void* pObject, void* pUnused, void* pEntity)
{
	char* pObjectBytes = (char*) pObject;
	char* pEntityBytes = (char*) pEntity;

	(void) pUnused;
	if (has_level_plas_history_entries(g_pActiveManagedEntityOwner) == 0) {
		return 0;
	}
	if (*(int*) (pEntityBytes + 0x64) != 2 || *(int*) (pObjectBytes + 0xb8) != 0x18) {
		return 0;
	}
	*(int*) (pObjectBytes + 0xcc) = 0x52;
	*(void**) (pObjectBytes + 0x5c) = pEntity;
	((ManagedEntityStateView*) pObject)->RequestManagedEntityStateId(0x1a);
	return 1;
}

// FUNCTION: LEMBALL 0x00427c40
int LEMBALL_FASTCALL AdvanceCaptureChunkObjectStateMachine(void* pObject)
{
	typedef void(LEMBALL_FASTCALL * ObjectSlotProc)(void*, void*, int);
	int* pFields = (int*) pObject;
	int nState;

	if (pFields[0x45] != 0) {
		*(unsigned short*) ((char*) pObject + 0xbc) = 1;
		if (pFields[0x4a] != pFields[0x2e]) {
			if (pFields[0x2e] == 0x1a) {
				((ObjectSlotProc) (*(void***) pObject)[13])(pObject, 0, 0x1e);
			}
			pFields[0x4a] = pFields[0x2e];
		}
		return 1;
	}
	*(unsigned short*) ((char*) pObject + 0xbc) = 0;
	if (pFields[0x4e] == 0) {
		return 1;
	}
	nState = pFields[0x2e];
	if (nState == 0x17) {
		if ((unsigned int) pFields[0x33] < (unsigned int) g_nLevelFrameClockTick) {
			pFields[0x4f] = 1;
			pFields[0x4e] = 0;
			((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 0x18);
		}
	}
	else if (nState == 0x19) {
		if ((unsigned int) pFields[0x34] < (unsigned int) g_nLevelFrameClockTick) {
			void* pTarget = *(void**) ((char*) pObject + 0x140);
			((ObjectSlotProc) (*(void***) pTarget)[2])(pTarget, 0, 0x15);
			*(int*) ((char*) pTarget + 0xcc) = g_nLevelFrameClockTick + 0x28;
			((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 0x1a);
			((ObjectSlotProc) (*(void***) pObject)[13])(pObject, 0, 0x1e);
		}
	}
	else if (nState == 0x1a && (unsigned int) pFields[0x33] < (unsigned int) g_nLevelFrameClockTick) {
		pFields[0x4f] = 1;
		pFields[0x33] = g_nLevelFrameClockTick + 0x14;
		((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 0x17);
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00428cf0
int LEMBALL_FASTCALL AdvanceLasrChunkObjectStateMachine(void* pObject)
{
	typedef void(LEMBALL_FASTCALL * ObjectSlotProc)(void*, void*, int);
	int* pFields = (int*) pObject;
	void* pTarget;
	int nState;

	if (pFields[0x45] != 0) {
		pFields[0x4e] = pFields[0x2e] != 0x18;
		if (pFields[0x2e] == 0x1a && *(void**) ((char*) pObject + 0x144) == 0) {
			AcquireLasrPlasTarget(pObject);
		}
		nState = pFields[0x2e];
		if (pFields[0x4a] != nState) {
			if (nState == 0x17) {
				pTarget = *(void**) ((char*) pObject + 0x144);
				if (pTarget != 0) {
					*(int*) ((char*) pTarget + 0x2c) = 1;
					*(void**) ((char*) pObject + 0x144) = 0;
				}
				((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 0x18);
			}
			else if (nState == 0x19) {
				*(void**) ((char*) pObject + 0x144) = 0;
			}
			pFields[0x4a] = pFields[0x2e];
		}
		return 1;
	}

	if (g_nSelectedNetworkLobbyPeerId != 0 && *(int*) (g_nSelectedNetworkLobbyPeerId + 0x1c) != 0) {
		return 1;
	}
	if (pFields[0x4e] == 0) {
		return 1;
	}

	switch (pFields[0x2e]) {
	case 0x17:
		pTarget = *(void**) ((char*) pObject + 0x144);
		if (pTarget != 0) {
			((ObjectSlotProc) (*(void***) pTarget)[13])(pTarget, 0, 0x22);
			*(int*) ((char*) pTarget + 0x2c) = 1;
			*(void**) ((char*) pObject + 0x144) = 0;
		}
		((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 0x18);
		return 1;
	case 0x18:
		if ((unsigned int) pFields[0x33] < (unsigned int) g_nLevelFrameClockTick) {
			BeginLasrPlasReacquireDelay(pObject);
		}
		return 1;
	case 0x19:
		if ((unsigned int) pFields[0x34] < (unsigned int) g_nLevelFrameClockTick) {
			*(void**) ((char*) pObject + 0x144) = 0;
			((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 0x1a);
		}
		return 1;
	case 0x1a:
		if (*(void**) ((char*) pObject + 0x144) == 0) {
			AcquireLasrPlasTarget(pObject);
		}
		if ((unsigned int) pFields[0x33] < (unsigned int) g_nLevelFrameClockTick) {
			pFields[0x4f] = 1;
			pFields[0x4e] = pFields[0x50];
			pFields[0x33] = g_nLevelFrameClockTick + 0x3c;
			pTarget = *(void**) ((char*) pObject + 0x144);
			if (pTarget != 0) {
				*(int*) ((char*) pTarget + 0x2c) = 1;
				*(void**) ((char*) pObject + 0x144) = 0;
			}
			((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 0x17);
		}
		break;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00429ed0
void* LEMBALL_FASTCALL DeleteLasrChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete)
{
	(void) pUnused;
	DELETE_CHUNK_OBJECT_ARRAY_OR_SCALAR(pObject, fDelete, 0x148, destroy_lasr_chunk_object_vtable_thunk);
}

// FUNCTION: LEMBALL 0x0042aa80
int LEMBALL_FASTCALL AdvanceTramChunkObjectStateMachine(void* pObject)
{
	typedef void(LEMBALL_FASTCALL * ObjectSlotProc)(void*, void*, int);
	int* pFields = (int*) pObject;

	if (pFields[0x45] != 0) {
		if (pFields[0x4a] != pFields[0x2e]) {
			if (pFields[0x2e] == 0x1b) {
				((ObjectSlotProc) (*(void***) pObject)[13])(pObject, 0, 0x17);
			}
			pFields[0x4a] = pFields[0x2e];
		}
		return 1;
	}
	if (pFields[0x4e] == 0) {
		return 1;
	}
	if (pFields[0x2e] == 0x1b && (unsigned int) pFields[0x33] < (unsigned int) g_nLevelFrameClockTick) {
		((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 0x18);
	}
	return 1;
}

// STUB: LEMBALL 0x0042bbe0
int LEMBALL_FASTCALL AdvancePgunChunkObjectStateMachine(void* pObject)
{
	(void) pObject;
	return 1;
}

// FUNCTION: LEMBALL 0x0042c9e0
void* LEMBALL_FASTCALL DeletePgunChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete)
{
	(void) pUnused;
	DELETE_CHUNK_OBJECT_ARRAY_OR_SCALAR(pObject, fDelete, 0x144, destroy_pgun_chunk_object_vtable_thunk);
}

// STUB: LEMBALL 0x0042cd70
int LEMBALL_FASTCALL AdvanceIceChunkObjectStateMachine(void* pObject)
{
	(void) pObject;
	return 1;
}

// FUNCTION: LEMBALL 0x0042e500
void* LEMBALL_FASTCALL DeleteIceChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete)
{
	(void) pUnused;
	DELETE_CHUNK_OBJECT_ARRAY_OR_SCALAR(pObject, fDelete, 0x188, destroy_ice_chunk_object);
}

// FUNCTION: LEMBALL 0x0042e650
void LEMBALL_FASTCALL mark_move_chunk_tiles_occupied(void* pObject)
{
	char* pGrid = (char*) g_pLevelTileGrid;
	int nMinX = (*(int*) ((char*) pObject + 0x9c) >> 12) - 8;
	int nMaxX = nMinX + 15;
	int nMinY = (*(int*) ((char*) pObject + 0xa0) >> 12) - 8;
	int nMaxY = nMinY + 15;
	int nTileX;
	int nTileY;

	nMinX = (nMinX + ((nMinX >> 31) & 15)) >> 4;
	nMaxX = (nMaxX + ((nMaxX >> 31) & 15)) >> 4;
	nMinY = (nMinY + ((nMinY >> 31) & 15)) >> 4;
	nMaxY = (nMaxY + ((nMaxY >> 31) & 15)) >> 4;

	for (nTileY = nMinY; nTileY <= nMaxY; ++nTileY) {
		for (nTileX = nMinX; nTileX <= nMaxX; ++nTileX) {
			if (nTileX >= 0 && nTileY >= 0 && nTileX < *(int*) (pGrid + 0x10) && nTileY < *(int*) (pGrid + 0x14)) {
				char* pTile = *(char**) (pGrid + 0x0c) + (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 12;
				*(unsigned char*) (pTile + 6) |= 0x10;
			}
		}
	}
}

// FUNCTION: LEMBALL 0x0042ea40
void LEMBALL_FASTCALL
translate_attached_move_chunk_objects(void* pObject, void* pUnused, int nDeltaX, int nDeltaY, int nDeltaZ)
{
	int i;
	(void) pUnused;
	for (i = 0; i < *(int*) ((char*) pObject + 0x174); ++i) {
		char* pEntity = *(char**) ((char*) pObject + 0x178 + i * 4);
		*(int*) (pEntity + 0x9c) += nDeltaX << 12;
		*(int*) (pEntity + 0xa0) += nDeltaY << 12;
		*(int*) (pEntity + 0xa4) += nDeltaZ << 12;
	}
}

// STUB: LEMBALL 0x0042eb00
int LEMBALL_FASTCALL AdvanceMoveChunkObjectStateMachine(void* pObject)
{
	(void) pObject;
	return 1;
}
// FUNCTION: LEMBALL 0x0042fbd0
void* LEMBALL_FASTCALL DeleteMoveChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete)
{
	(void) pUnused;
	DELETE_CHUNK_OBJECT_ARRAY_OR_SCALAR(pObject, fDelete, 0x1a0, destroy_move_chunk_object);
}
