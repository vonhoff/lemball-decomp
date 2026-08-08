#include "AI/CHand.h"
#include "AI/AICoord.h"
#include "AI/LevelPoint3.h"
#include "AI/CLaser.h"
#include "AI/CLift.h"
#include "AI/CGameObject.h"
#include "AI/CAI.h"
#include "AI/CGenericGroup.h"
#include "AI/CPlayerLemmingGroup.h"
#include "AI/CPlayerLemming.h"
#include "AI/CMover.h"
#include "AI/LevelNodePoint.h"
#include "Visos/Generic/Memory.h"
#include "Platform/Windows/Mixed/Engine/CORE/WIN32.H"
#include "Platform/Windows/Mixed/Level/CHUNKOBJVT.H"

extern int g_nLevelFrameClockTimeMs;
extern int g_nLevelFrameClockTick;
extern int g_nSelectedNetworkLobbyPeerId;
extern void* g_pLiftTileGrid;
extern void* g_pLevelTileGrid;
extern void* g_pActiveManagedEntityOwner;
extern int __fastcall nDead(void* pLevelMode);
extern CPlayerLemming* LEMBALL_FASTCALL PopLevelPlasHistoryChild(void* pLevelMode);

extern void LEMBALL_FASTCALL destroy_lift_chunk_object(void* pObject);
extern void LEMBALL_FASTCALL DestroyCRocket(void* pObject);
extern void LEMBALL_FASTCALL destroy_lasr_chunk_object_vtable_thunk(void* pObject);
extern void LEMBALL_FASTCALL DestroyCTrampoline(void* pObject);
extern void LEMBALL_FASTCALL DestroyCPaintGun(void* pObject);
extern void LEMBALL_FASTCALL DestroyCIce(void* pObject);
extern void LEMBALL_FASTCALL destroy_move_chunk_object(void* pObject);
extern void* LEMBALL_FASTCALL DeleteCompositeLevelChunkChildrenWrapper20B20(void*, void*, BYTE);

#include "AI/CInvisibleSwitch.h"

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


struct ManagedProjectileObjectView {
	void ActivateAtPosition(const LevelPoint3* pPosition);
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

struct CGround {
	short GetZ(int nLocalX, int nLocalY);
	short GetZThunk(int nLocalX, int nLocalY);
};

struct IceContactDispatcherView {
	void StepOn(LevelPoint3* pPosition, void* pEntity, unsigned short nTileFlags);
};

struct IceObjectStateView {
	virtual void ReservedSlot0(int nValue);
	virtual void ReservedSlot1(int nValue);
	virtual void SetState(int nState);
};

struct LevelModeNodeView {
	LevelNodePoint* CopyNodePointByIndex(LevelNodePoint* pOutput, int nIndex);
};

void LEMBALL_FASTCALL ReleaseIceChunkObjectEntities(void* pObject);

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

struct InvisibleSwitchVtableLayout {
	void* m_apSlots00[5];
	int (CInvisibleSwitch::*m_pProcess14)(void);
	void* m_apSlots18[10];
};
typedef char
	InvisibleSwitchMemberPointerMustBeOneWord[sizeof(int (CInvisibleSwitch::*)(void)) == sizeof(void*) ? 1 : -1];

// GLOBAL: LEMBALL 0x00493120
InvisibleSwitchVtableLayout g_LINKSCF_InvsChunkObjectVtable = {{(void*) DeleteInvsChunkObjectAuto,
																(void*) SetManagedEntityStateIdWithVariant,
																(void*) SetManagedEntityStateId,
																(void*) EmitLevelChunkObjectRenderEntry,
																(void*) noop_vtable_callback_0040a7f0},
															   &CInvisibleSwitch::Process,
															   {(void*) return_true_vtable_callback_0040a800,
																(void*) get_object_field_0xf0,
																(void*) SetLevelChunkObjectMotionVector,
																(void*) AdvanceManagedEntityGravityMotion,
																(void*) return_false_vtable_callback_0040a820,
																(void*) copy_object_fields_0x9c_triplet,
																(void*) noop_vtable_callback_0040a860,
																(void*) set_object_field_0x98,
																(void*) get_object_field_0x98,
																(void*) BeginManagedEntityReachablePathSearch}};

// GLOBAL: LEMBALL 0x004936f8
void* g_LEVELVT_DoorChunkObjectVtable[16] =
	COMMON_LEVEL_CHUNK_OBJECT_VTABLE(DeleteDoorChunkObjectArrayVtableThunk,
									 DoActivateCDoorThunk);

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

// GLOBAL: LEMBALL 0x00495250
CGenericGroupChunkVtable g_LEVELVT_EnmyManagedEntityGroupVtable = {
	{(void*) DeleteCompositeLevelChunkChildrenWrapper20B20,
	 (void*) SetManagedEntityStateIdWithVariantLocalThunk,
	 (void*) set_managed_entity_state_id_local,
	 (void*) EmitLevelChunkObjectRenderEntry,
	 (void*) noop_vtable_callback_0040a7f0},
	&CGenericGroup::Process,
	{(void*) return_true_vtable_callback_0040a800,
	 (void*) get_object_field_0xf0,
	 (void*) SetLevelChunkObjectMotionVector,
	 (void*) AdvanceManagedEntityGravityMotion,
	 (void*) return_false_vtable_callback_0040a820,
	 (void*) copy_object_fields_0x9c_triplet,
	 (void*) noop_vtable_callback_0040a860,
	 (void*) set_object_field_0x98,
	 (void*) get_object_field_0x98,
	 (void*) BeginManagedEntityReachablePathSearch}};

// GLOBAL: LEMBALL 0x004959c0
void* g_LEVELVT_CollChunkObjectVtable[16] =
	COMMON_LEVEL_CHUNK_OBJECT_VTABLE(DeleteCollChunkObjectWrapper23060, AdvanceCollChunkObjectStateMachine);

typedef char LiftMemberPointerMustBeOneWord[sizeof(int (CLift::*)(void)) == sizeof(void*) ? 1 : -1];

// GLOBAL: LEMBALL 0x00495d60
LiftVtableLayout g_LINKSCF_LiftChunkObjectVtable = {{(void*) DeleteLiftChunkObjectAuto,
													(void*) SetManagedEntityStateIdWithVariant,
													(void*) SetManagedEntityStateId,
													(void*) EmitLevelChunkObjectRenderEntry,
													(void*) noop_vtable_callback_0040a7f0},
													&CLift::Process,
													{(void*) return_true_vtable_callback_0040a800,
													 (void*) get_object_field_0xf0,
													 (void*) SetLevelChunkObjectMotionVector,
													 (void*) AdvanceManagedEntityGravityMotion,
													 (void*) return_false_vtable_callback_0040a820,
													 (void*) copy_object_fields_0x9c_triplet,
													 (void*) noop_vtable_callback_0040a860,
													 (void*) set_object_field_0x98,
													 (void*) get_object_field_0x98,
													 (void*) BeginManagedEntityReachablePathSearch}};

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
CHandVtable g_LINKSCF_CaptureChunkObjectVtable = {{(void*) DeleteCaptureChunkObjectAuto,
														 (void*) SetManagedEntityStateIdWithVariant,
														 (void*) SetManagedEntityStateId,
														 (void*) EmitLevelChunkObjectRenderEntry,
														 (void*) noop_vtable_callback_0040a7f0},
														&CHand::Process,
														{(void*) return_true_vtable_callback_0040a800,
														 (void*) get_object_field_0xf0,
														 (void*) SetLevelChunkObjectMotionVector,
														 (void*) AdvanceManagedEntityGravityMotion,
														 (void*) return_false_vtable_callback_0040a820,
														 (void*) copy_object_fields_0x9c_triplet,
														 (void*) noop_vtable_callback_0040a860,
														 (void*) set_object_field_0x98,
														 (void*) get_object_field_0x98,
														 (void*) BeginManagedEntityReachablePathSearch}};

struct LaserVtableLayout {
	void* m_apSlots00[5];
	int (CLaser::*m_pProcess14)(void);
	void* m_apSlots18[10];
};
typedef char LaserMemberPointerMustBeOneWord[sizeof(int (CLaser::*)(void)) == sizeof(void*) ? 1 : -1];

// GLOBAL: LEMBALL 0x00496310
LaserVtableLayout g_LINKSCF_LasrChunkObjectVtable = {{(void*) DeleteLasrChunkObjectAuto,
													 (void*) SetManagedEntityStateIdWithVariant,
													 (void*) SetManagedEntityStateId,
													 (void*) EmitLevelChunkObjectRenderEntry,
													 (void*) noop_vtable_callback_0040a7f0},
													&CLaser::Process,
													{(void*) return_true_vtable_callback_0040a800,
													 (void*) get_object_field_0xf0,
													 (void*) SetLevelChunkObjectMotionVector,
													 (void*) AdvanceManagedEntityGravityMotion,
													 (void*) return_false_vtable_callback_0040a820,
													 (void*) copy_object_fields_0x9c_triplet,
													 (void*) noop_vtable_callback_0040a860,
													 (void*) set_object_field_0x98,
													 (void*) get_object_field_0x98,
													 (void*) BeginManagedEntityReachablePathSearch}};

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
#define DESTROY_INVS_CHUNK_OBJECT(pItem) ((CInvisibleSwitch*) (pItem))->~CInvisibleSwitch()
	DELETE_CHUNK_OBJECT_ARRAY_OR_SCALAR(pObject, fDelete, 0x2b8, DESTROY_INVS_CHUNK_OBJECT);
#undef DESTROY_INVS_CHUNK_OBJECT
}

// STUB: LEMBALL 0x00414150
int LEMBALL_FASTCALL UpdateManagedEntityGroupCommandState(void* pObject)
{
	(void) pObject;
	return 1;
}

// Macintosh: CGameObject::Fly()
// STUB: LEMBALL 0x00415300
void LEMBALL_FASTCALL AdvanceManagedEntityGravityMotion(void* pObject)
{
	(void) pObject;
}

// Macintosh: CGameObject::StartRoute()
// FUNCTION: LEMBALL 0x00415830
int LEMBALL_FASTCALL BeginManagedEntityReachablePathSearch(void* pObject)
{
	int* piVar1 = (int*) (*(int*) 0x49cf58 + 8);
	*(int*) ((char*) pObject + 0xc) = (*piVar1 == 0);
	*piVar1 = 1;
	if (*(int*) ((char*) pObject + 0xc) != 0) {
		((void (__fastcall*)(void*, int, int, int, int, int)) 0x4035e4)(*(void**) 0x49cf58, 0,
			((*(int*) ((char*) pObject + 0x9c) >> 12) + (*(int*) ((char*) pObject + 0x9c) >> 31 & 0xf)) >> 4,
			((*(int*) ((char*) pObject + 0xa0) >> 12) + (*(int*) ((char*) pObject + 0xa0) >> 31 & 0xf)) >> 4,
			((*(int*) ((char*) pObject + 0xa8) >> 12) + (*(int*) ((char*) pObject + 0xa8) >> 31 & 0xf)) >> 4,
			((*(int*) ((char*) pObject + 0xac) >> 12) + (*(int*) ((char*) pObject + 0xac) >> 31 & 0xf)) >> 4);
	}
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
		CGround* pTile =
			(CGround*) (*(char**) (pGrid + 0x0c) + (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 12);
		nHeightOffset = (unsigned short) pTile->GetZ(nLocalX & 15, nLocalY & 15);
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
	int nY;
	int nZ;
	int nX;

	(void) pUnused;
	nY = *(int*) ((char*) pObject + 0xa0);
	nZ = *(int*) ((char*) pObject + 0xa4);
	nX = *(int*) ((char*) pObject + 0x9c) - 0x8000;
	((int*) pPoint)[1] = nY;
	((int*) pPoint)[0] = nX;
	((int*) pPoint)[2] = nZ;
}

// FUNCTION: LEMBALL 0x0041dc50
void LEMBALL_FASTCALL EmitGmobChunkType14RenderEntry(void* pObject, void* pUnused, void* pRenderEntry)
{
	EmitLevelChunkObjectRenderEntry(pObject, pUnused, pRenderEntry);
	*(unsigned short*) ((char*) pRenderEntry + 0x2e) = *(unsigned short*) ((char*) pObject + 0x148);
}

// MACINTOSH: CCollectable::Process()
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
				CGround* pTile =
					(CGround*) (*(char**) (pGrid + 0x0c) +
												  (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 12);
				nHeight = pTile->GetZ(nLocalX & 0x0f, nLocalY & 0x0f);
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
	char* pEntryBytes;
	char* pObjectBytes;
	unsigned int nShortValue;
	int nStateId;
	int nStateTimeMs;
	int nFrameClockTimeMs;

	(void) pUnused;
	pEntryBytes = (char*) pRenderEntry;
	pObjectBytes = (char*) pObject;
	*(unsigned short*) (pEntryBytes + 0x2c) = *(unsigned short*) (pObjectBytes + 0x6a);
	*(int*) (pEntryBytes + 0x28) = *(int*) (pObjectBytes + 0x64);
	*(unsigned short*) (pEntryBytes + 0x2e) = 0;
	*(int*) (pEntryBytes + 0x04) = *(int*) (pObjectBytes + 0x9c) >> 12;
	*(int*) (pEntryBytes + 0x08) = *(int*) (pObjectBytes + 0xa0) >> 12;
	*(int*) (pEntryBytes + 0x0c) = *(int*) (pObjectBytes + 0xa4) >> 12;
	nShortValue = *(unsigned short*) (pObjectBytes + 0xb4);
	*(unsigned short*) pEntryBytes = (unsigned short) nShortValue;
	nShortValue = *(unsigned short*) (pObjectBytes + 0xbc);
	nStateTimeMs = *(int*) (pObjectBytes + 0x94);
	nStateId = *(int*) (pObjectBytes + 0xb8);
	*(unsigned short*) (pEntryBytes + 0x1c) = (unsigned short) nShortValue;
	*(int*) (pEntryBytes + 0x18) = nStateId;
	*(int*) (pEntryBytes + 0x20) = nStateTimeMs;
	*(int*) (pEntryBytes + 0x14) = 0;
	*(int*) (pEntryBytes + 0x30) = *(int*) (pObjectBytes + 0xc0);
	*(int*) (pEntryBytes + 0x38) = *(int*) (pObjectBytes + 0xe4);
	*(int*) (pEntryBytes + 0x3c) = *(int*) (pObjectBytes + 0xe8);
	*(int*) (pEntryBytes + 0x40) = *(int*) (pObjectBytes + 0xec);
	*(int*) (pEntryBytes + 0x44) = *(int*) (pObjectBytes + 0x98);
	if (*(int*) (pObjectBytes + 0x114) != 0) {
		nFrameClockTimeMs = g_nNetworkFrameClockTimeMs;
	}
	else {
		nFrameClockTimeMs = g_nLevelFrameClockTimeMs;
	}
	*(int*) (pEntryBytes + 0x24) = nFrameClockTimeMs;
	((RockChunkObjectStateView*) pObject)->SetActionState(0);
	*(int*) (pEntryBytes + 0x34) = *(int*) (pObjectBytes + 0x10);
	*(int*) (pObjectBytes + 0x10) = 0;
}

// FUNCTION: LEMBALL 0x00427520
void* LEMBALL_FASTCALL DeleteRockChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete)
{
	(void) pUnused;
	DELETE_CHUNK_OBJECT_ARRAY_OR_SCALAR(pObject, fDelete, 0x144, DestroyCRocket);
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
		((CGameObject*) *(void**) (pObjectBytes + 0x140))->ResetInstructions();
		pProjectile = (ManagedProjectileObjectView*) PopLevelPlasHistoryChild(g_pActiveManagedEntityOwner);
		if (pProjectile != 0) {
			activationPoint.m_nX = *(int*) (pObjectBytes + 0x9c);
			activationPoint.m_nY = *(int*) (pObjectBytes + 0xa0) - 0x34000;
			activationPoint.m_nZ = *(int*) (pObjectBytes + 0xa4);
			pProjectile->ActivateAtPosition(&activationPoint);
			((CPlayerLemming*) *(void**) (pObjectBytes + 0x140))
				->GetGroup()
				->AddLemmingToGroup((CPlayerLemming*) pProjectile);
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
	if (nDead(g_pActiveManagedEntityOwner) == 0) {
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


// MACINTOSH: CTrampoline::Process()
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

// MACINTOSH: CPaintGun::Process()
// FUNCTION: LEMBALL 0x0042bbe0
int LEMBALL_FASTCALL AdvancePgunChunkObjectStateMachine(void* pObject)
{
	typedef void(LEMBALL_FASTCALL * ObjectSlotProc)(void*, void*, int);
	int* pFields;
	int nState;
	int nFrame;
	int nCoordinate;
	AICOORD source;
	AICOORD target;
	CAI* pSpawner;

	pFields = (int*) pObject;
	if (pFields[0x45] != 0) {
		if (pFields[0x4a] != pFields[0x2e]) {
			if (pFields[0x2e] == 3) {
				((ObjectSlotProc) (*(void***) pObject)[0x34 / sizeof(void*)])(pObject, 0, 7);
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

	nState = pFields[0x2e];
	if (nState == 3) {
		((IceObjectStateView*) pObject)->SetState(0x1b);
	}
	else if (nState == 0x18) {
		nFrame = pFields[0x50];
		pFields[0x32] = g_nLevelFrameClockTick + ((0x1c - nFrame) * 1000) / 400;
		pFields[0x25] = g_nLevelFrameClockTimeMs + (-nFrame * 1000) / 8;
		pFields[0x33] = g_nLevelFrameClockTick + ((0x3a - nFrame) * 1000) / 400;
		((IceObjectStateView*) pObject)->SetState(0x1b);
	}
	else if (nState == 0x1b) {
		if ((unsigned int) pFields[0x33] < (unsigned int) g_nLevelFrameClockTick) {
			pFields[0x25] = g_nLevelFrameClockTimeMs;
			pFields[0x32] = g_nLevelFrameClockTick + 0x46;
			pFields[0x33] = g_nLevelFrameClockTick + 0x91;
			((IceObjectStateView*) pObject)->SetState(0x1b);
		}
		else if ((unsigned int) pFields[0x32] < (unsigned int) g_nLevelFrameClockTick) {
			pSpawner = (CAI*) g_pActiveManagedEntityOwner;
			source.x = pFields[0x27] + 0x10000;
			source.y = pFields[0x28];
			source.z = pFields[0x29] + 0x8000;
			nCoordinate = (pFields[0x27] >> 12) + 200;
			if (0x400 < nCoordinate) {
				nCoordinate = 0x3ff;
			}
			target.x = nCoordinate << 12;
			target.y = source.y;
			target.z = source.z;
			pSpawner->FireBullet(
				*(unsigned short*) ((char*) pObject + 0x6c), 0, 1, 2, source, target);

			source.x = pFields[0x27] - 0x10000;
			nCoordinate = (pFields[0x27] >> 12) - 200;
			if (nCoordinate < 0) {
				nCoordinate = 0;
			}
			target.x = nCoordinate << 12;
			pSpawner->FireBullet(
				*(unsigned short*) ((char*) pObject + 0x6c), 0, 1, 6, source, target);

			source.x = pFields[0x27];
			source.y = pFields[0x28] + 0x10000;
			target.x = source.x;
			nCoordinate = (pFields[0x28] >> 12) + 200;
			if (0x400 < nCoordinate) {
				nCoordinate = 0x3ff;
			}
			target.y = nCoordinate << 12;
			pSpawner->FireBullet(
				*(unsigned short*) ((char*) pObject + 0x6c), 0, 1, 8, source, target);

			source.y = pFields[0x28] - 0x10000;
			nCoordinate = (pFields[0x28] >> 12) - 200;
			if (nCoordinate < 0) {
				nCoordinate = 0;
			}
			target.y = nCoordinate << 12;
			pSpawner->FireBullet(
				*(unsigned short*) ((char*) pObject + 0x6c), 0, 1, 4, source, target);

			((ObjectSlotProc) (*(void***) pObject)[0x34 / sizeof(void*)])(pObject, 0, 7);
			pFields[0x32] = pFields[0x33];
			((IceObjectStateView*) pObject)->SetState(3);
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0042c9e0
void* LEMBALL_FASTCALL DeletePgunChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete)
{
	(void) pUnused;
	DELETE_CHUNK_OBJECT_ARRAY_OR_SCALAR(pObject, fDelete, 0x144, DestroyCPaintGun);
}

// MACINTOSH: CIce::Process()
// FUNCTION: LEMBALL 0x0042cd70
int LEMBALL_FASTCALL AdvanceIceChunkObjectStateMachine(void* pObject)
{
	typedef void(LEMBALL_FASTCALL * ObjectSlotProc)(void*, void*, int);
	typedef void(LEMBALL_FASTCALL * ObjectThreeValueSlotProc)(void*, void*, int, int, int);
	typedef void(LEMBALL_FASTCALL * ObjectVectorSlotProc)(void*, void*, LevelPoint3*, int);
	int* pFields;
	int nTickDelta;
	int nMinX;
	int nMinY;
	int nMaxX;
	int nMaxY;
	int i;
	char** ppEntity;
	char* pEntity;
	LevelPoint3 position;
	LevelPoint3 velocity;
	int nXMotion;
	int nYMotion;
	int nXLimit;
	int nYLimit;
	int nHeight;
	int nGroundHeight;
	int nLocalX;
	int nLocalY;
	int nTileX;
	int nTileY;
	char* pGrid;
	CGround* pGround;

	pFields = (int*) pObject;
	if (pFields[0x45] != 0) {
		if (pFields[0x4a] != pFields[0x2e]) {
			if (pFields[0x2e] == 0x1a) {
				pFields[0x4e] = pFields[0x4e] == 0;
				ReleaseIceChunkObjectEntities(pObject);
			}
			pFields[0x4a] = pFields[0x2e];
		}
	}
	else if (pFields[0x2e] == 0x1a) {
		pFields[0x4e] = pFields[0x4e] == 0;
		ReleaseIceChunkObjectEntities(pObject);
		((IceObjectStateView*) pObject)->SetState(0x18);
	}
	if (pFields[0x4e] == 0) {
		return 1;
	}

	nTickDelta = g_nLevelFrameClockTick - pFields[0x32];
	if (nTickDelta == 0) {
		return 1;
	}
	pFields[0x32] = g_nLevelFrameClockTick;
	nMinX = *(short*) ((char*) pObject + 0x17c) - 8;
	nMinY = *(short*) ((char*) pObject + 0x17e) - 8;
	nMaxX = *(short*) ((char*) pObject + 0x182) + 7;
	nMaxY = *(short*) ((char*) pObject + 0x184) + 7;

	i = 0;
	if (0 < pFields[0x54]) {
		ppEntity = (char**) pObject + 0x55;
		do {
			pEntity = *ppEntity;
			position.m_nX = *(int*) (pEntity + 0x9c);
			position.m_nY = *(int*) (pEntity + 0xa0);
			position.m_nZ = *(int*) (pEntity + 0xa4);
			nXMotion = pFields[0x51] * nTickDelta * 0x1000 / 8;
			nYMotion = pFields[0x52] * nTickDelta * 0x1000 / 8;
			nXLimit = nXMotion >> 12;
			if (nXLimit < 0) {
				nXLimit = -nXLimit;
			}
			nYLimit = nYMotion >> 12;
			if (nYLimit < 0) {
				nYLimit = -nYLimit;
			}
			while (15 < nXLimit || 15 < nYLimit) {
				nXLimit /= 2;
				nYLimit /= 2;
				nXMotion /= 2;
				nYMotion /= 2;
			}
			position.m_nX += nXMotion;
			position.m_nY += nYMotion;

			pGrid = (char*) g_pLevelTileGrid;
			nLocalX = position.m_nX >> 12;
			nLocalY = position.m_nY >> 12;
			nTileX = nLocalX >> 4;
			nTileY = nLocalY >> 4;
			if (nLocalX < 0 || nLocalY < 0 || *(int*) (pGrid + 0x10) <= nTileX ||
				*(int*) (pGrid + 0x14) <= nTileY) {
				nGroundHeight = 0;
			}
			else {
				pGround = (CGround*) (*(char**) (pGrid + 0xc) +
												 (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 0xc);
				nGroundHeight = (unsigned short) pGround->GetZThunk(nLocalX & 0xf, nLocalY & 0xf);
			}
			nHeight = position.m_nZ >> 12;
			if (nHeight < nGroundHeight) {
				position.m_nZ = nGroundHeight << 12;
			}
			else if (nGroundHeight < nHeight) {
				nHeight -= nTickDelta * 4;
				if (nHeight < nGroundHeight) {
					nHeight = nGroundHeight;
				}
				position.m_nZ = nHeight << 12;
			}
			if (pFields[0x51] == 0) {
				nXLimit = (position.m_nX & 0xf000) >> 12;
				if (nXLimit < 8) {
					position.m_nX += 0x1000;
				}
				else if (8 < nXLimit) {
					position.m_nX -= 0x1000;
				}
			}
			else if (pFields[0x52] == 0) {
				nYLimit = (position.m_nY & 0xf000) >> 12;
				if (nYLimit < 8) {
					position.m_nY += 0x1000;
				}
				else if (8 < nYLimit) {
					position.m_nY -= 0x1000;
				}
			}
			++i;
			++ppEntity;
			*(int*) (pEntity + 0x9c) = position.m_nX;
			*(int*) (pEntity + 0xa0) = position.m_nY;
			*(int*) (pEntity + 0xa4) = position.m_nZ;
		} while (i < pFields[0x54]);
	}

	i = 0;
	while (i < pFields[0x54]) {
		pEntity = *(char**) ((char*) pObject + 0x154 + i * sizeof(void*));
		position.m_nX = *(int*) (pEntity + 0x9c);
		position.m_nY = *(int*) (pEntity + 0xa0);
		position.m_nZ = *(int*) (pEntity + 0xa4);
		if (position.m_nX >> 12 < nMinX || nMaxX < position.m_nX >> 12 ||
			position.m_nY >> 12 < nMinY || nMaxY < position.m_nY >> 12) {
			*(int*) (pEntity + 0xc0) = 0;
			*(int*) (pEntity + 0xb8) = 0;
			*(int*) (pEntity + 0xcc) = g_nLevelFrameClockTick;
			if (*(int*) (pEntity + 0x64) == 2) {
				((ObjectSlotProc) (*(void***) pEntity)[0x84 / sizeof(void*)])(pEntity, 0, 4000);
				((ObjectThreeValueSlotProc) (*(void***) pEntity)[0xe8 / sizeof(void*)])(
					pEntity, 0, 0, 0, 0);
				pGrid = (char*) g_pLevelTileGrid;
				nLocalX = position.m_nX >> 12;
				nLocalY = position.m_nY >> 12;
				nTileX = nLocalX >> 4;
				nTileY = nLocalY >> 4;
				if (nLocalX < 0 || nLocalY < 0 || *(int*) (pGrid + 0x10) <= nTileX ||
					*(int*) (pGrid + 0x14) <= nTileY) {
					nGroundHeight = 0;
				}
				else {
					pGround = (CGround*) (*(char**) (pGrid + 0xc) +
													 (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 0xc);
					nGroundHeight =
						(unsigned short) pGround->GetZThunk(nLocalX & 0xf, nLocalY & 0xf);
				}
				if (nGroundHeight < position.m_nZ >> 12) {
					velocity.m_nX = (pFields[0x51] << 12) / 6;
					velocity.m_nY = (pFields[0x52] << 12) / 6;
					velocity.m_nZ = 0;
					((ObjectVectorSlotProc) (*(void***) pEntity)[0x20 / sizeof(void*)])(
						pEntity, 0, &velocity, 0);
				}
			}
			if (i + 1 < pFields[0x54]) {
				ppEntity = (char**) pObject + i + 0x55;
				nXLimit = i + 1;
				do {
					*ppEntity = ppEntity[1];
					++ppEntity;
					++nXLimit;
				} while (nXLimit < pFields[0x54]);
			}
			--i;
			--pFields[0x54];

			pGrid = (char*) g_pLevelTileGrid;
			nLocalX = position.m_nX >> 12;
			nLocalY = position.m_nY >> 12;
			nTileX = nLocalX >> 4;
			nTileY = nLocalY >> 4;
			if (nLocalX < 0 || nLocalY < 0 || *(int*) (pGrid + 0x10) <= nTileX ||
				*(int*) (pGrid + 0x14) <= nTileY) {
				nGroundHeight = 0;
			}
			else {
				pGround = (CGround*) (*(char**) (pGrid + 0xc) +
												 (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 0xc);
				nGroundHeight = (unsigned short) pGround->GetZThunk(nLocalX & 0xf, nLocalY & 0xf);
			}
			if (position.m_nZ >> 12 <= nGroundHeight) {
				((IceContactDispatcherView*) g_pActiveManagedEntityOwner)
					->StepOn(&position, pEntity, *(unsigned short*) (pEntity + 0x68));
			}
		}
		++i;
	}

	i = 0;
	if (0 < pFields[0x54]) {
		ppEntity = (char**) pObject + 0x55;
		do {
			pEntity = *ppEntity;
			position.m_nX = *(int*) (pEntity + 0x9c);
			position.m_nY = *(int*) (pEntity + 0xa0);
			position.m_nZ = *(int*) (pEntity + 0xa4);
			pGrid = (char*) g_pLevelTileGrid;
			nLocalX = position.m_nX >> 12;
			nLocalY = position.m_nY >> 12;
			nTileX = nLocalX >> 4;
			nTileY = nLocalY >> 4;
			if (nLocalX < 0 || nLocalY < 0 || *(int*) (pGrid + 0x10) <= nTileX ||
				*(int*) (pGrid + 0x14) <= nTileY) {
				nGroundHeight = 0;
			}
			else {
				pGround = (CGround*) (*(char**) (pGrid + 0xc) +
												 (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 0xc);
				nGroundHeight = (unsigned short) pGround->GetZThunk(nLocalX & 0xf, nLocalY & 0xf);
			}
			if (position.m_nZ >> 12 <= nGroundHeight) {
				((IceContactDispatcherView*) g_pActiveManagedEntityOwner)
					->StepOn(&position, pEntity, *(unsigned short*) (pEntity + 0x68));
			}
			++ppEntity;
			++i;
		} while (i < pFields[0x54]);
	}
	return 1;
}

// STUB: LEMBALL 0x0042d560
void LEMBALL_FASTCALL ReleaseIceChunkObjectEntities(void* pObject)
{
	(void) pObject;
}

// STUB: LEMBALL 0x00412890
void IceContactDispatcherView::StepOn(LevelPoint3* pPosition, void* pEntity, unsigned short nTileFlags)
{
	(void) pPosition;
	(void) pEntity;
	(void) nTileFlags;
}

// FUNCTION: LEMBALL 0x0042e500
void* LEMBALL_FASTCALL DeleteIceChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete)
{
	(void) pUnused;
	DELETE_CHUNK_OBJECT_ARRAY_OR_SCALAR(pObject, fDelete, 0x188, DestroyCIce);
}



// MACINTOSH: CMover::Process()
// FUNCTION: LEMBALL 0x0042eb00
int LEMBALL_FASTCALL AdvanceMoveChunkObjectStateMachine(void* pObject)
{
	typedef void(LEMBALL_FASTCALL * ObjectSlotProc)(void*, void*, int);
	int* pFields = (int*) pObject;
	CMover* pMover = (CMover*) pObject;
	unsigned long nTick;
	int fLocalAuthority;
	int nState;
	int nNextNode;
	int nWidth;
	unsigned short nHeight;
	LevelNodePoint point;
	LevelNodePoint oldPoint;
	LevelNodePoint* pPoint;
	CPt3 position;
	char* pGrid;
	CGround* pGround;

	if (pFields[0x4e] == 0) {
		return 1;
	}
	if (pFields[0x50] != 0) {
		pFields[0x50] = 0;
	}

	nTick = g_nNetworkFrameClockTick;
	fLocalAuthority = g_nSelectedNetworkLobbyPeerId == 0 ||
		*(int*) (g_nSelectedNetworkLobbyPeerId + 0x1c) != 0;
	nState = pFields[0x2e];
	if (fLocalAuthority) {
		nTick = g_nLevelFrameClockTick;
	}
	else if (pFields[0x4a] != nState) {
		if (nState == 2) {
			pMover->StopObjectsMoving();
			pMover->SetUpNextNode(nTick);
		}
		nState = pFields[0x2e];
		pFields[0x4a] = nState;
	}
	else if (nState != 2 && nState != 0x24) {
		nState = 0x18;
	}

	pMover->VerifyObjects();
	switch (nState) {
	case 0:
		nNextNode = pFields[0x5c] + 1;
		if (pFields[0x5b] <= nNextNode) {
			nNextNode = 0;
		}
		oldPoint.m_anValues[0] = pFields[0x27];
		oldPoint.m_anValues[1] = pFields[0x28];
		oldPoint.m_anValues[2] = pFields[0x29];
		pFields[0x5c] = nNextNode;
		pPoint = ((LevelModeNodeView*) g_pActiveManagedEntityOwner)
			->CopyNodePointByIndex(&point, pFields[0x5a] + nNextNode);
		pFields[0x27] = pPoint->m_anValues[0];
		pFields[0x28] = pPoint->m_anValues[1];
		pFields[0x29] = pPoint->m_anValues[2];

		pGrid = (char*) g_pLevelTileGrid;
		if ((pFields[0x27] >> 12) < 0 || (pFields[0x28] >> 12) < 0 ||
			(pFields[0x27] >> 16) >= *(int*) (pGrid + 0x10) ||
			(pFields[0x28] >> 16) >= *(int*) (pGrid + 0x14)) {
			nHeight = 0;
		}
		else {
			nWidth = *(int*) (pGrid + 0x10);
			pGround = (CGround*) (*(char**) (pGrid + 0x0c) +
				(((pFields[0x28] >> 16) * nWidth + (pFields[0x27] >> 16)) * 12));
			nHeight = (unsigned short) pGround->GetZThunk(
				(pFields[0x27] >> 12) & 15, (pFields[0x28] >> 12) & 15);
		}
		pFields[0x29] = (int) nHeight << 12;
		pMover->MoveObjects(
			(pFields[0x27] >> 12) - (oldPoint.m_anValues[0] >> 12),
			(pFields[0x28] >> 12) - (oldPoint.m_anValues[1] >> 12),
			(pFields[0x29] >> 12) - (oldPoint.m_anValues[2] >> 12));
		if (pFields[0x51] == 0) {
			pFields[0x32] = g_nLevelFrameClockTick + 0x14;
			if (fLocalAuthority) {
				((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 1);
			}
		}
		else {
			pFields[0x32] = g_nLevelFrameClockTick;
			if (fLocalAuthority) {
				((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 0x24);
			}
		}
		break;
	case 1:
		if (fLocalAuthority) {
			if ((unsigned int) g_nLevelFrameClockTick < (unsigned int) pFields[0x32]) {
				return 1;
			}
			pMover->StopObjectsMoving();
			((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 2);
		}
		pMover->SetUpNextNode(nTick);
		break;
	case 2:
		pMover->SetPos();
		if (fLocalAuthority && (unsigned int) pFields[0x33] < (unsigned int) g_nLevelFrameClockTick) {
			((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 0);
		}
		else if ((unsigned int) nTick <= (unsigned int) pFields[0x33]) {
			position.x = 0;
			position.y = 0;
			position.z = 0;
			((CMove3d*) ((char*) pObject + 0x14c))->Position(position, nTick);
			pMover->MoveObjects(
				position.x - (pFields[0x27] >> 12),
				position.y - (pFields[0x28] >> 12),
				position.z - (pFields[0x29] >> 12));
			pFields[0x27] = position.x << 12;
			pFields[0x28] = position.y << 12;
			pFields[0x29] = position.z << 12;
		}
		break;
	case 0x14:
		pMover->SetUpNextNode(nTick);
		pMover->SetPos();
		if (fLocalAuthority) {
			((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 1);
		}
		break;
	case 0x18:
		if (fLocalAuthority) {
			((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 0x14);
		}
		break;
	case 0x24:
		if (pFields[0x52] != 0) {
			((ObjectSlotProc) (*(void***) pObject)[2])(pObject, 0, 1);
			pFields[0x52] = 0;
		}
		break;
	}
	return 1;
}
// FUNCTION: LEMBALL 0x0042fbd0
void* LEMBALL_FASTCALL DeleteMoveChunkObjectAuto(void* pObject, void* pUnused, unsigned int fDelete)
{
	(void) pUnused;
	DELETE_CHUNK_OBJECT_ARRAY_OR_SCALAR(pObject, fDelete, 0x1a0, destroy_move_chunk_object);
}
