#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/WIN32.H"
#include "ENGINE/GDI/VSWINDOW.H"
#include "ENGINE/MEDIA/VSSTRM.H"

void* LEMBALL_FASTCALL DeleteSimpleVtableObject407EF0(void* pObject, void*, BYTE fDelete);

extern void* g_LINKSCF_RockChunkManagerVtable[10];
extern void* g_LINKSCF_CollChunkManagerVtable[10];
extern void* g_LINKSCF_PgunChunkManagerVtable[10];
extern void* g_LINKSCF_IceChunkManagerVtable[10];
extern void* g_LINKSCF_MoveChunkManagerVtable[10];

struct LevelVtDeletableChild {
	virtual void Delete(BYTE fDelete);
};

// SYNTHETIC: LEMBALL 0x00407e10
// DeleteStreamFormatStateAuto
void* LEMBALL_FASTCALL DeleteStreamFormatStateAuto(void* pObject, void*, BYTE fDelete)
{
	ConstructStreamFormatState((VsInitStreamFormatTargetState*) pObject);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// STRONG INFERENCE: callbacks relocated from the artificial aggregate by
// vtable owner, semantic role, and neighboring address/call-graph cluster.

struct LevelBoundsPoint {
	int m_nX;
	int m_nY;
	int m_nZ;
};

struct LevelThreeDwordBounds {
	LevelBoundsPoint m_Minimum;
	LevelBoundsPoint m_Maximum;
};

struct LevelBoundedRenderObjectView {
	char m_abReserved00[0x14];
	LevelThreeDwordBounds m_Bounds;
};

struct LevelManagedEntityStateIdView {
	char m_abReserved00[0xb8];
	int m_nStateIdB8;
};

struct LevelManagedEntityDirtyStateView {
	char m_abReserved00[0x98];
	int m_nCurrentDirtyValue98;
	char m_abReserved9C[0xc8];
	int m_fChildDirty164;
	char m_abReserved168[0x48];
	int m_nCachedDirtyValue1B0;

	int PollChildDirtyFlag(void);
	int PollChildDirtyFlagThunk(void);
};

struct LevelManagedEntityHistoryView {
	char m_abReserved00[0x128];
	int m_cHistoryIds128;
	int m_anHistoryIds12C[1];

	int PopHistoryId(void);
	int PopHistoryIdThunk(void);
};

struct LevelManagedEntityChildIterator;

struct LevelManagedEntityActiveGroupView {
	void* m_pReserved00;
	LevelManagedEntityChildIterator* m_apManagedEntities04[40];
	int m_cManagedEntitiesA4;

	LevelManagedEntityChildIterator* GetFirstActiveEntity(void);
	LevelManagedEntityChildIterator* GetFirstActiveEntityThunk(void);
};

struct LevelManagedEntityChildIterator {
	char m_abReserved00[0x124];
	int m_cChildren124;
	void* m_apChildren128[10];
	int m_iCurrentChild150;
	char m_abReserved154[0x10];
	int m_fChildrenDirty164;
	int m_nActiveState168;

	int GetChildCount(void);
	void* GetFirstChild(void);
	void* GetNextChild(void);
	void* GetCurrentChild(void);
	void* GetChildAt(int iChild);
	void AppendChild(void* pChild);
	int ContainsChild(void* pChild);
	void* GetFirstChildThunk(void);
	void* GetNextChildThunk(void);
	void* GetCurrentChildThunk(void);
	int GetActiveState(void);
	int GetActiveStateThunk(void);
	void* GetChildWithState8(void);
	void* GetChildWithState8Thunk(void);
	void* GetCurrentChildWithState8(void);
	int HasDirtyChildren(void);
	int HasDirtyChildrenThunk(void);
};

extern int g_nLevelFrameClockTick;
extern void* g_pSharedRenderDispatchQueue;

struct LevelManagedEntityStateFields {
	char m_abReserved00[0xb4];
	short m_nHeadingOctantB4;
	char m_abReservedB6[2];
	int m_nStateIdB8;
	unsigned short m_nStateVariantBC;

	void IncrementHeadingOctant(void);
	void DecrementHeadingOctant(void);
	int IsStateNotInactive(void);
	int IsStateNotInactiveThunk(void);
	int IsStateAvailableForVariant(void);
	void SetStateIdWithVariantLocal(int nStateId, int nVariant);
	void SetStateIdWithVariantLocalThunk(int nStateId, int nVariant);
};

struct LevelThreeDwordPoint {
	int m_nX;
	int m_nY;
	int m_nZ;

	LevelThreeDwordPoint* Copy(const LevelThreeDwordPoint* pSource);
	LevelThreeDwordPoint* CopyThunk(const LevelThreeDwordPoint* pSource);
};

struct LevelTwoDwordPair {
	int m_nFirst;
	int m_nSecond;

	LevelTwoDwordPair* Copy(const LevelTwoDwordPair* pSource);
	LevelTwoDwordPair* CopyThunk(const LevelTwoDwordPair* pSource);
};

struct LevelThreeDwordPointVariant {
	int m_nX;
	int m_nY;
	int m_nZ;

	LevelThreeDwordPointVariant* Copy(const LevelThreeDwordPointVariant* pSource);
	LevelThreeDwordPointVariant* CopyThunk(const LevelThreeDwordPointVariant* pSource);
};

struct LevelQueuedProjectileRequestTable {
	char m_abReserved00[0x1c0];
	int m_anRequestCodes1C0[24];
	int m_cRequestCodes220;

	int GetRequestCode(int nIndex);
	int GetRequestCodeThunk(int nIndex);
};

struct LevelThreeDwordPointVariantB {
	int m_nX;
	int m_nY;
	int m_nZ;

	LevelThreeDwordPointVariantB* Copy(const LevelThreeDwordPointVariantB* pSource);
	LevelThreeDwordPointVariantB* CopyThunk(const LevelThreeDwordPointVariantB* pSource);
};

struct LevelModeChunkEntryArrayView {
	char m_abReserved00[0xE0];
	int m_anArrayE0[4];
	int m_anArrayF0[4];
	int m_anArray100[4];

	void CopyTripletToFixedPoint(int* pOutput, int nIndex);
	void CopyTripletToFixedPointThunk(int* pOutput, int nIndex);
};

struct LevelManagedEntityPositionView {
	void* m_pVtable00;
	LevelThreeDwordPoint m_WorldPosition04;

	LevelThreeDwordPoint* CopyWorldPosition(LevelThreeDwordPoint* pPosition);
	LevelThreeDwordPoint* CopyWorldPositionThunk(LevelThreeDwordPoint* pPosition);
};

struct LevelManagedEntityCommandEntry {
	int m_nField00;
	int m_nField04;
	int m_nField08;
	int m_nField0C;
	unsigned short m_nField10;
	unsigned short m_nReserved12;
};

struct LevelManagedEntityCommandQueue {
	unsigned short m_cEntries00;
	unsigned short m_nReserved02;
	LevelManagedEntityCommandEntry* m_pEntries04;

	void PopFront(void);
	void PopFrontThunk(void);
	void PushFront(void);
	void PushFrontThunk(void);
};

struct LevelManagedEntityQueueCursor {
	char m_abReserved00[0x10];
	int m_nX10;
	int m_nY14;
	int m_nMaximumX18;
	int m_nMaximumY1C;

	void SetPosition(int nX, int nY);
	void ClampPosition(void);
	void ClampPositionThunk(void);
	void GetPosition(int* pnX, int* pnY);
	void GetPositionThunk(int* pnX, int* pnY);
};

struct LevelManagedEntityCommandQueueView {
	char m_abReserved00[0x70];
	unsigned short* m_pCommandCount70;

	int HasPendingCommands(void);
	int HasPendingCommandsThunk(void);
};

typedef void(LEMBALL_FASTCALL* LevelManagedEntityNoArgVirtualProc)(void* pEntity);
typedef void(LEMBALL_FASTCALL* LevelManagedEntityIntVirtualProc)(void* pEntity, void* pUnused, int nValue);

void __fastcall ClearManagedEntityMoveCommandsThunk(void* pEntity);
int __fastcall GetManagedEntityGroupCommandStateThunk(void* pEntity);
void NoopManagedEntityGroupDetachHookThunk(void);
int PASCAL IsManagedEntityState18Thunk(int nState);
void noop_vtable_callback_00407f20(void);
int PASCAL return_false_vtable_callback_00407f30(void* pUnused);
void noop_vtable_callback_00408000(void);
void PASCAL noop_vtable_callback_0040a7f0(void* pUnused);
int PASCAL return_true_vtable_callback_0040a800(void* pUnused);
void* __fastcall get_object_field_0xf0(void* pObject);
int return_false_vtable_callback_0040a820(void);
void __fastcall copy_object_fields_0x9c_triplet(void* pObject, void* pUnused, LevelBoundsPoint* pPoint);
void __fastcall set_object_field_0x98(void* pObject, void* pUnused, int nValue);
int __fastcall get_object_field_0x98(void* pObject);
int __fastcall is_point_inside_3d_bounds(LevelBoundedRenderObjectView* pObject,
										 void* pUnused,
										 const LevelBoundsPoint* pPoint);
int __fastcall do_3d_bounds_intersect(LevelBoundedRenderObjectView* pObject,
									  void* pUnused,
									  const LevelThreeDwordBounds* pBounds);
void PASCAL noop_vtable_callback_0040a930(void* pUnused);
void noop_vtable_callback_0040a940(void);
int return_false_vtable_callback_0040a950(void);
void noop_vtable_callback_0040a960(void);
void noop_vtable_callback_0040a970(void);
void noop_vtable_callback_0040a980(void);
void noop_vtable_callback_0040a990(void);
int return_false_vtable_callback_0040a9c0(void);
int return_false_vtable_callback_0040a9d0(void);
void noop_vtable_callback_0040a9f0(void);
void noop_vtable_callback_0040aa00(void);
void noop_vtable_callback_0040aa10(void);
int return_false_vtable_callback_0040aa30(void);
void noop_vtable_callback_0040aa40(void);
int return_true_vtable_callback_0040aa50(void);
int PASCAL return_false_vtable_callback_0040aa60(void* pUnused);
int PASCAL return_false_vtable_callback_0040aa70(void* pUnused1, void* pUnused2);
void PASCAL noop_vtable_callback_0040aa80(void* pUnused1, void* pUnused2);
int PASCAL return_false_vtable_callback_0040aa90(void* pUnused);
void noop_vtable_callback_0040aaa0(void);
void noop_vtable_callback_0040aab0(void);
void noop_callback_0040aad0(void);
void __fastcall set_object_field_0x30_true(void* pObject);
void __fastcall set_object_field_0x30_false(void* pObject);
int __fastcall get_plas_child_runtime_flag_0x30(void* pObject);
void PASCAL noop_callback_0040ab20(void* pUnused1, void* pUnused2, void* pUnused3);
int return_false_vtable_callback_0040ab30(void);
int return_false_vtable_callback_0040ab40(void);
int PASCAL IsManagedEntityState18(int nState);
int PASCAL return_false_vtable_callback_0040ab80(void* pUnused1, void* pUnused2, void* pUnused3);
int PASCAL return_false_vtable_callback_0040ab90(void* pUnused);
void noop_vtable_callback_0040adc0(void);
void __fastcall set_managed_entity_state_id_local(void* pEntity, void* pUnused, int nStateId);
void noop_managed_entity_virtual_hook(void);
void noop_callback_0040c1b0(void);
int return_two_vtable_callback(void);
void noop_vtable_callback_0040ce40(void);
int return_two_callback(void);
void noop_vtable_callback_0040cf30(void);
int PASCAL IsDoorChunkIdleOrClosedState(int nState);
void __fastcall set_managed_entity_owner_group(void* pEntity, void* pUnused, void* pOwnerGroup);
void* __fastcall get_managed_entity_owner_group(void* pEntity);
void __fastcall set_managed_entity_primary_active_flag(void* pEntity, void* pUnused, int fActive);
void __fastcall set_managed_entity_secondary_active_flag(void* pEntity, void* pUnused, int fActive);
int __fastcall has_door_chunk_object_target_tile_latched(void* pObject);
void __fastcall schedule_door_chunk_object_update_in_10_ticks(void* pObject);
void __fastcall clear_door_chunk_object_target_tile_latch(void* pObject);
void* __fastcall get_level_mode_chunk_loader_context(void* pLevelMode);
void LEMBALL_FASTCALL MarkObjectField178AndSetTimer3C(void* pObject);
void* __fastcall get_level_mode_mine_chunk_manager(void* pLevelMode);
void* __fastcall get_level_mode_lift_chunk_manager(void* pLevelMode);
void* __fastcall get_object_field_0x30(void* pObject);
void __fastcall add_level_time_bonus_counter(void* pLevelMode, void* pUnused, int nBonus);
void __fastcall set_level_mode_field_0xf8(void* pLevelMode, void* pUnused, int nValue);
int __fastcall has_level_plas_history_entries(void* pLevelMode);
int __fastcall get_managed_entity_group_command_state(void* pEntity);
void __fastcall set_managed_entity_group_command_state(void* pEntity, void* pUnused, int nState);
void __fastcall set_entity_field_0x08_if_flags_bit0(void* pEntity);
void __fastcall clear_managed_entity_move_commands(void* pEntity);
int return_false_virtual_hook(void);
void* __fastcall get_first_projectile_pool_object(void* pPool);
void noop_managed_entity_group_detach_hook(void);
void PASCAL ResetManagedEntityGroupState(void* pEntity);
void __cdecl ForwardManagedEntityVslot7C(void* pUnused, void* pEntity);
int __cdecl CallHasPendingManagedEntityCommands(void* pUnused, LevelManagedEntityCommandQueueView* pEntity);
void __cdecl ForwardManagedEntityVslot1C(void* pUnused, void* pEntity);
void __cdecl ForwardManagedEntityVslot30(void* pUnused, void* pEntity);
void __cdecl ForwardManagedEntityVslot3C(void* pUnused, void* pEntity);
void __cdecl ForwardManagedEntityVslot40(void* pUnused, void* pEntity);
void __cdecl ForwardManagedEntityVslot68(void* pUnused, void* pEntity);
void __cdecl ForwardManagedEntityVslot24(void* pUnused, void* pEntity);
void __cdecl ForwardManagedEntityVslot44(void* pUnused, void* pEntity);
void __cdecl ForwardManagedEntityVslotD0(void* pUnused, void* pEntity);
void __cdecl ClearPlasChildRuntimeFlag30(void* pUnused, void* pEntity);
void __cdecl ForwardManagedEntityVslot9C(void* pUnused, void* pEntity);
void __cdecl ForwardManagedEntityVslot8C(void* pUnused, void* pEntity);
void __cdecl ForwardManagedEntityVslot88(void* pUnused, void* pEntity);
void __cdecl ForwardManagedEntityVslot90(void* pUnused, void* pEntity);
void noop_projectile_geometry_callback(void);
void* __fastcall initialize_three_dword_point_sentinel(void* pPoint);
void __fastcall deactivate_projectile_object(void* pObject);
void* __fastcall get_next_projectile_pool_object(void* pPool);
void __fastcall update_projectile_pool_real(void* pPool);

struct ProjectilePool {
	char m_abReserved00[0x34];
	void* m_apObjects[20];
	char m_abReserved84[0x50];
	int m_nCountD4;
	int m_nIndexD8;

	void RemoveObject(void* pObject);
};

struct ProjectileObjectProxy {
	virtual void vf0(void);
	virtual void vf1(void);
	virtual void SetState(int nState);
};
int return_true_vtable_callback_0041c3f0(void);
int return_two_vtable_callback_0041c430(void);
int return_true_vtable_callback_0041c5b0(void);
int return_two_vtable_callback_0041c630(void);
int return_two_vtable_callback_0041dc40(void);
void PASCAL noop_vtable_callback_0041e3c0(void* pUnused1, void* pUnused2, void* pUnused3);
void __fastcall set_object_field_0x160(void* pObject, void* pUnused, int nValue);
int __fastcall get_object_field_0x160(void* pObject);
int return_zero_vtable_callback_0041f870(void);
int __fastcall is_delayed_move_to_saved_target_armed(void* pObject);
void __fastcall set_chunk_short_delay_timer(void* pObject);
int __fastcall get_object_field_0x124(void* pObject);
void LEMBALL_FASTCALL CallBallChunkEntryVfuncC8(void* pEntry, void* pUnused, int nUnused);
void LEMBALL_FASTCALL DispatchVtableSlot34WithValue2(void* pObject, void* pUnused);
void LEMBALL_FASTCALL DispatchVtableSlot34WithValue2Variant(void* pObject, void* pUnused);
void LEMBALL_FASTCALL SendCollMessage16(void* pObject, void* pUnused);
void noop_vtable_callback_00422fa0(void);
void noop_vtable_callback_00423040(void);
int return_true_vtable_callback_00423050(void);
int PASCAL IsManagedEntityState8Or18(int nState);
void __fastcall clear_rock_chunk_object_count(void* pManager);
int PASCAL get_tile_flag_0x1c_object_render_entry_count(void);
void noop_vtable_callback_0042c9a0(void);
void __fastcall reset_ice_chunk_object_count(void* pManager);
int PASCAL return_zero_vtable_callback_0042dd30(void* pUnused);
void __fastcall set_move_chunk_object_trigger_latch(void* pObject);
void __fastcall reset_move_chunk_object_count(void* pManager);
int __fastcall return_object_field_0x0c_00435880(void* pObject);
void __fastcall set_object_field_0x10_00435890(void* pObject, void* pUnused, int nValue);
void __fastcall set_object_field_0x0c_004358a0(void* pObject, void* pUnused, int nValue);
void __fastcall set_object_field_0x18_004358b0(void* pObject, void* pUnused, int nValue);
unsigned int return_hresult_80000000_callback(void);
void* __fastcall clear_two_word_point(void* pPoint);
void noop_queued_variant_child_slot_manager_update(void);
unsigned int return_hresult_40000001_callback(void);
void noop_network_lobby_vsnet_runtime_slot_3(void);
void noop_callback_00456600(void);

// LINKERILT: LEMBALL 0x00401019
void __fastcall update_projectile_pool(void* pPool)
{
	update_projectile_pool_real(pPool);
}

// LINKERILT: LEMBALL 0x00401046
LevelThreeDwordPointVariant* LevelThreeDwordPointVariant::CopyThunk(const LevelThreeDwordPointVariant* pSource)
{
	return Copy(pSource);
}

// LINKERILT: LEMBALL 0x00401050
int thunk_return_false_vtable_callback_0040aa70(void* pUnused1, void* pUnused2)
{
	return return_false_vtable_callback_0040aa70(pUnused1, pUnused2);
}

// Compatibility shim for ILT slot 0x00401055 targeting 0x0040a960.
// Removing it perturbs MSVC 4.20 code generation and regresses 0x00414e20.
void thunk_noop_vtable_callback_0040a960(void)
{
	noop_vtable_callback_0040a960();
}

// LINKERILT: LEMBALL 0x004010be
LevelThreeDwordPoint* LevelManagedEntityPositionView::CopyWorldPositionThunk(LevelThreeDwordPoint* pPosition)
{
	return CopyWorldPosition(pPosition);
}

// LINKERILT: LEMBALL 0x004011ef
void __fastcall ClearManagedEntityMoveCommandsThunk(void* pEntity)
{
	clear_managed_entity_move_commands(pEntity);
}

// LINKERILT: LEMBALL 0x0040142e
int LevelManagedEntityDirtyStateView::PollChildDirtyFlagThunk(void)
{
	return PollChildDirtyFlag();
}

// LINKERILT: LEMBALL 0x004015cd
void LevelModeChunkEntryArrayView::CopyTripletToFixedPointThunk(int* pOutput, int nIndex)
{
	CopyTripletToFixedPoint(pOutput, nIndex);
}

// LINKERILT: LEMBALL 0x0040161d
LevelThreeDwordPointVariantB* LevelThreeDwordPointVariantB::CopyThunk(const LevelThreeDwordPointVariantB* pSource)
{
	return Copy(pSource);
}

// LINKERILT: LEMBALL 0x0040173f
void* LevelManagedEntityChildIterator::GetChildWithState8Thunk(void)
{
	return GetChildWithState8();
}

// LINKERILT: LEMBALL 0x00401816
void* LevelManagedEntityChildIterator::GetNextChildThunk(void)
{
	return GetNextChild();
}

// LINKERILT: LEMBALL 0x0040185c
int __fastcall GetManagedEntityGroupCommandStateThunk(void* pEntity)
{
	return get_managed_entity_group_command_state(pEntity);
}

// LINKERILT: LEMBALL 0x004018a7
LevelThreeDwordPoint* LevelThreeDwordPoint::CopyThunk(const LevelThreeDwordPoint* pSource)
{
	return Copy(pSource);
}

// LINKERILT: LEMBALL 0x00401bf4
void LevelManagedEntityCommandQueue::PopFrontThunk(void)
{
	PopFront();
}

// LINKERILT: LEMBALL 0x00401e65
void LevelManagedEntityQueueCursor::GetPositionThunk(int* pnX, int* pnY)
{
	GetPosition(pnX, pnY);
}

// LINKERILT: LEMBALL 0x00401eab
void* LevelManagedEntityChildIterator::GetCurrentChildThunk(void)
{
	return GetCurrentChild();
}

// LINKERILT: LEMBALL 0x00401ed3
LevelTwoDwordPair* LevelTwoDwordPair::CopyThunk(const LevelTwoDwordPair* pSource)
{
	return Copy(pSource);
}

// LINKERILT: LEMBALL 0x00401f37
int LevelManagedEntityCommandQueueView::HasPendingCommandsThunk(void)
{
	return HasPendingCommands();
}

// LINKERILT: LEMBALL 0x0040202c
void LevelManagedEntityCommandQueue::PushFrontThunk(void)
{
	PushFront();
}

// LINKERILT: LEMBALL 0x004020c2
void NoopManagedEntityGroupDetachHookThunk(void)
{
	noop_managed_entity_group_detach_hook();
}

// LINKERILT: LEMBALL 0x004021d0
// DestroyLevelChunkObjectBaseAutoThunk

// LINKERILT: LEMBALL 0x004021df
LevelManagedEntityChildIterator* LevelManagedEntityActiveGroupView::GetFirstActiveEntityThunk(void)
{
	return GetFirstActiveEntity();
}

// LINKERILT: LEMBALL 0x0040241e
void* LevelManagedEntityChildIterator::GetFirstChildThunk(void)
{
	return GetFirstChild();
}

// LINKERILT: LEMBALL 0x004024be
void LevelManagedEntityStateFields::SetStateIdWithVariantLocalThunk(int nStateId, int nVariant)
{
	SetStateIdWithVariantLocal(nStateId, nVariant);
}

// LINKERILT: LEMBALL 0x00402676
// DestroyCollChunkObjectAutoThunk

// LINKERILT: LEMBALL 0x0040269e
int LevelQueuedProjectileRequestTable::GetRequestCodeThunk(int nIndex)
{
	return GetRequestCode(nIndex);
}

// LINKERILT: LEMBALL 0x00402833
int LevelManagedEntityHistoryView::PopHistoryIdThunk(void)
{
	return PopHistoryId();
}

// LINKERILT: LEMBALL 0x00403215
void LevelManagedEntityQueueCursor::ClampPositionThunk(void)
{
	ClampPosition();
}

// LINKERILT: LEMBALL 0x00403251
void* LEMBALL_FASTCALL DeleteSimpleVtableObject407EF0Thunk(void* pObject, void* pUnusedEdx, BYTE fDelete)
{
	return DeleteSimpleVtableObject407EF0(pObject, pUnusedEdx, fDelete);
}

// LINKERILT: LEMBALL 0x0040335f
int PASCAL IsManagedEntityState18Thunk(int nState)
{
	return IsManagedEntityState18(nState);
}

// LINKERILT: LEMBALL 0x00403477
int LevelManagedEntityChildIterator::GetActiveStateThunk(void)
{
	return GetActiveState();
}

// LINKERILT: LEMBALL 0x0040367a
int LevelManagedEntityStateFields::IsStateNotInactiveThunk(void)
{
	return IsStateNotInactive();
}

// LINKERILT: LEMBALL 0x004037c4
// DestroyCompositeLevelChunkChildrenAutoThunk

// LINKERILT: LEMBALL 0x0040380a
int LevelManagedEntityChildIterator::HasDirtyChildrenThunk(void)
{
	return HasDirtyChildren();
}

// FUNCTION: LEMBALL 0x00407f20
void noop_vtable_callback_00407f20(void)
{
}

// FUNCTION: LEMBALL 0x00407f30
int PASCAL return_false_vtable_callback_00407f30(void* pUnused)
{
	(void) pUnused;
	return 0;
}

// FUNCTION: LEMBALL 0x00408000
void noop_vtable_callback_00408000(void)
{
}

// FUNCTION: LEMBALL 0x0040a7f0
void PASCAL noop_vtable_callback_0040a7f0(void* pUnused)
{
	(void) pUnused;
}

// FUNCTION: LEMBALL 0x0040a800
int PASCAL return_true_vtable_callback_0040a800(void* pUnused)
{
	(void) pUnused;
	return 1;
}

// FUNCTION: LEMBALL 0x0040a810
void* __fastcall get_object_field_0xf0(void* pObject)
{
	return *(void**) ((char*) pObject + 0xf0);
}

// FUNCTION: LEMBALL 0x0040a820
int return_false_vtable_callback_0040a820(void)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040a830
void __fastcall copy_object_fields_0x9c_triplet(void* pObject, void* pUnused, LevelBoundsPoint* pPoint)
{
	(void) pUnused;
	*pPoint = *(LevelBoundsPoint*) ((char*) pObject + 0x9c);
}

// FUNCTION: LEMBALL 0x0040a870
void __fastcall set_object_field_0x98(void* pObject, void* pUnused, int nValue)
{
	(void) pUnused;
	*(int*) ((char*) pObject + 0x98) = nValue;
}

// FUNCTION: LEMBALL 0x0040a880
int __fastcall get_object_field_0x98(void* pObject)
{
	return *(int*) ((char*) pObject + 0x98);
}

// FUNCTION: LEMBALL 0x0040a890
int __fastcall is_point_inside_3d_bounds(LevelBoundedRenderObjectView* pObject,
										 void* pUnused,
										 const LevelBoundsPoint* pPoint)
{
	(void) pUnused;
	return pObject->m_Bounds.m_Minimum.m_nX <= pPoint->m_nX && pPoint->m_nX <= pObject->m_Bounds.m_Maximum.m_nX &&
		   pObject->m_Bounds.m_Minimum.m_nY <= pPoint->m_nY && pPoint->m_nY <= pObject->m_Bounds.m_Maximum.m_nY &&
		   pObject->m_Bounds.m_Minimum.m_nZ <= pPoint->m_nZ && pPoint->m_nZ <= pObject->m_Bounds.m_Maximum.m_nZ;
}

// FUNCTION: LEMBALL 0x0040a8e0
int __fastcall do_3d_bounds_intersect(LevelBoundedRenderObjectView* pObject,
									  void* pUnused,
									  const LevelThreeDwordBounds* pBounds)
{
	(void) pUnused;
	return pObject->m_Bounds.m_Minimum.m_nX <= pBounds->m_Maximum.m_nX &&
		   pBounds->m_Minimum.m_nX <= pObject->m_Bounds.m_Maximum.m_nX &&
		   pObject->m_Bounds.m_Minimum.m_nY <= pBounds->m_Maximum.m_nY &&
		   pBounds->m_Minimum.m_nY <= pObject->m_Bounds.m_Maximum.m_nY &&
		   pObject->m_Bounds.m_Minimum.m_nZ <= pBounds->m_Maximum.m_nZ &&
		   pBounds->m_Minimum.m_nZ <= pObject->m_Bounds.m_Maximum.m_nZ;
}

// FUNCTION: LEMBALL 0x0040a930
void PASCAL noop_vtable_callback_0040a930(void* pUnused)
{
	(void) pUnused;
}

// FUNCTION: LEMBALL 0x0040a940
void noop_vtable_callback_0040a940(void)
{
}

// FUNCTION: LEMBALL 0x0040a950
int return_false_vtable_callback_0040a950(void)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040a960
void noop_vtable_callback_0040a960(void)
{
}

// FUNCTION: LEMBALL 0x0040a970
void noop_vtable_callback_0040a970(void)
{
}

// FUNCTION: LEMBALL 0x0040a980
void noop_vtable_callback_0040a980(void)
{
}

// FUNCTION: LEMBALL 0x0040a990
void noop_vtable_callback_0040a990(void)
{
}

// FUNCTION: LEMBALL 0x0040a9c0
int return_false_vtable_callback_0040a9c0(void)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040a9d0
int return_false_vtable_callback_0040a9d0(void)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040a9f0
void noop_vtable_callback_0040a9f0(void)
{
}

// FUNCTION: LEMBALL 0x0040aa00
void noop_vtable_callback_0040aa00(void)
{
}

// FUNCTION: LEMBALL 0x0040aa10
void noop_vtable_callback_0040aa10(void)
{
}

// FUNCTION: LEMBALL 0x0040aa30
int return_false_vtable_callback_0040aa30(void)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040aa40
void noop_vtable_callback_0040aa40(void)
{
}

// FUNCTION: LEMBALL 0x0040aa50
int return_true_vtable_callback_0040aa50(void)
{
	return 1;
}

// FUNCTION: LEMBALL 0x0040aa60
int PASCAL return_false_vtable_callback_0040aa60(void* pUnused)
{
	(void) pUnused;
	return 0;
}

// FUNCTION: LEMBALL 0x0040aa70
int PASCAL return_false_vtable_callback_0040aa70(void* pUnused1, void* pUnused2)
{
	(void) pUnused1;
	(void) pUnused2;
	return 0;
}

// FUNCTION: LEMBALL 0x0040aa80
void PASCAL noop_vtable_callback_0040aa80(void* pUnused1, void* pUnused2)
{
	(void) pUnused1;
	(void) pUnused2;
}

// FUNCTION: LEMBALL 0x0040aa90
int PASCAL return_false_vtable_callback_0040aa90(void* pUnused)
{
	(void) pUnused;
	return 0;
}

// FUNCTION: LEMBALL 0x0040aaa0
void noop_vtable_callback_0040aaa0(void)
{
}

// FUNCTION: LEMBALL 0x0040aab0
void noop_vtable_callback_0040aab0(void)
{
}

// FUNCTION: LEMBALL 0x0040aad0
void noop_callback_0040aad0(void)
{
}

// FUNCTION: LEMBALL 0x0040aae0
void __fastcall set_object_field_0x30_true(void* pObject)
{
	*(int*) ((char*) pObject + 0x30) = 1;
}

// FUNCTION: LEMBALL 0x0040ab00
void __fastcall set_object_field_0x30_false(void* pObject)
{
	*(int*) ((char*) pObject + 0x30) = 0;
}

// FUNCTION: LEMBALL 0x0040ab10
int __fastcall get_plas_child_runtime_flag_0x30(void* pObject)
{
	return *(int*) ((char*) pObject + 0x30);
}

// FUNCTION: LEMBALL 0x0040ab20
void PASCAL noop_callback_0040ab20(void* pUnused1, void* pUnused2, void* pUnused3)
{
	(void) pUnused1;
	(void) pUnused2;
	(void) pUnused3;
}

// FUNCTION: LEMBALL 0x0040ab30
int return_false_vtable_callback_0040ab30(void)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040ab40
int return_false_vtable_callback_0040ab40(void)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0040ab50
int PASCAL IsManagedEntityState18(int nState)
{
	return nState == 0x18;
}

// FUNCTION: LEMBALL 0x0040ab80
int PASCAL return_false_vtable_callback_0040ab80(void* pUnused1, void* pUnused2, void* pUnused3)
{
	(void) pUnused1;
	(void) pUnused2;
	(void) pUnused3;
	return 0;
}

// FUNCTION: LEMBALL 0x0040ab90
int PASCAL return_false_vtable_callback_0040ab90(void* pUnused)
{
	(void) pUnused;
	return 0;
}

// FUNCTION: LEMBALL 0x0040adc0
void noop_vtable_callback_0040adc0(void)
{
}

// FUNCTION: LEMBALL 0x0040c170
void __fastcall set_managed_entity_state_id_local(void* pEntity, void* pUnused, int nStateId)
{
	(void) pUnused;
	*(int*) ((char*) pEntity + 0xb8) = nStateId;
}

// FUNCTION: LEMBALL 0x0040c180
void LevelManagedEntityStateFields::SetStateIdWithVariantLocal(int nStateId, int nVariant)
{
	m_nStateIdB8 = nStateId;
	m_nStateVariantBC = (unsigned short) nVariant;
}

// FUNCTION: LEMBALL 0x0040c1a0
void noop_managed_entity_virtual_hook(void)
{
}

// FUNCTION: LEMBALL 0x0040c1b0
void noop_callback_0040c1b0(void)
{
}

// FUNCTION: LEMBALL 0x0040c1c0
int return_two_vtable_callback(void)
{
	return 2;
}

// FUNCTION: LEMBALL 0x0040c270
LevelThreeDwordPoint* LevelThreeDwordPoint::Copy(const LevelThreeDwordPoint* pSource)
{
	m_nX = pSource->m_nX;
	m_nY = pSource->m_nY;
	m_nZ = pSource->m_nZ;
	return this;
}

// FUNCTION: LEMBALL 0x0040c290
LevelTwoDwordPair* LevelTwoDwordPair::Copy(const LevelTwoDwordPair* pSource)
{
	m_nFirst = pSource->m_nFirst;
	m_nSecond = pSource->m_nSecond;
	return this;
}

// FUNCTION: LEMBALL 0x0040c2b0
LevelThreeDwordPointVariant* LevelThreeDwordPointVariant::Copy(const LevelThreeDwordPointVariant* pSource)
{
	m_nX = pSource->m_nX;
	m_nY = pSource->m_nY;
	m_nZ = pSource->m_nZ;
	return this;
}

// FUNCTION: LEMBALL 0x0040ce40
void noop_vtable_callback_0040ce40(void)
{
}

// FUNCTION: LEMBALL 0x0040ce80
int return_two_callback(void)
{
	return 2;
}

// FUNCTION: LEMBALL 0x0040cf30
void noop_vtable_callback_0040cf30(void)
{
}

// FUNCTION: LEMBALL 0x0040dd30
int PASCAL IsDoorChunkIdleOrClosedState(int nState)
{
	if (nState != 0x18 && (nState < 0x1d || nState > 0x1e)) {
		return 0;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0040f0d0
void __fastcall set_managed_entity_owner_group(void* pEntity, void* pUnused, void* pOwnerGroup)
{
	(void) pUnused;
	*(void**) ((char*) pEntity + 0x224) = pOwnerGroup;
}

// FUNCTION: LEMBALL 0x0040f0e0
void* __fastcall get_managed_entity_owner_group(void* pEntity)
{
	return *(void**) ((char*) pEntity + 0x224);
}

// FUNCTION: LEMBALL 0x0040f0f0
void __fastcall set_managed_entity_primary_active_flag(void* pEntity, void* pUnused, int fActive)
{
	(void) pUnused;
	*(int*) ((char*) pEntity + 0x17c) = fActive;
}

// FUNCTION: LEMBALL 0x0040f100
void __fastcall set_managed_entity_secondary_active_flag(void* pEntity, void* pUnused, int fActive)
{
	(void) pUnused;
	*(int*) ((char*) pEntity + 0x180) = fActive;
}

// FUNCTION: LEMBALL 0x0040f110
int __fastcall get_level_mode_field_0x17c(void* pLevelMode)
{
	return *(int*) ((char*) pLevelMode + 0x17c);
}

// FUNCTION: LEMBALL 0x0040f2b0
int __fastcall has_door_chunk_object_target_tile_latched(void* pObject)
{
	return *(int*) ((char*) pObject + 0x184) == 1;
}

// FUNCTION: LEMBALL 0x0040f410
void __fastcall schedule_door_chunk_object_update_in_10_ticks(void* pObject)
{
	*(int*) ((char*) pObject + 0xcc) = g_nLevelFrameClockTick + 10;
}

// FUNCTION: LEMBALL 0x0040f420
void __fastcall clear_door_chunk_object_target_tile_latch(void* pObject)
{
	*(int*) ((char*) pObject + 0x184) = 0;
}

// FUNCTION: LEMBALL 0x0040f7a0
int LevelManagedEntityDirtyStateView::PollChildDirtyFlag(void)
{
	int* pfDirty;
	int nCurrentValue;

	nCurrentValue = m_nCurrentDirtyValue98;
	if (m_nCachedDirtyValue1B0 == nCurrentValue && (pfDirty = &m_fChildDirty164, *pfDirty == 0)) {
		*pfDirty = 0;
		m_nCachedDirtyValue1B0 = nCurrentValue;
		return *pfDirty;
	}
	pfDirty = &m_fChildDirty164;
	m_nCachedDirtyValue1B0 = nCurrentValue;
	*pfDirty = 1;
	return *pfDirty;
}

// FUNCTION: LEMBALL 0x0040fcb0
int LevelQueuedProjectileRequestTable::GetRequestCode(int nIndex)
{
	if (nIndex >= m_cRequestCodes220) {
		return 0xffff;
	}
	return m_anRequestCodes1C0[nIndex];
}

// FUNCTION: LEMBALL 0x004102d0
int LevelManagedEntityStateFields::IsStateAvailableForVariant(void)
{
	int nVariant;

	if (!IsStateNotInactiveThunk()) {
		return 0;
	}
	if (m_nStateIdB8 == 0x0f) {
		nVariant = m_nStateVariantBC;
		if (nVariant >= 1 && nVariant <= 2) {
			return 0;
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00410a10
void* __fastcall get_level_mode_chunk_loader_context(void* pLevelMode)
{
	return *(void**) ((char*) pLevelMode + 0x178);
}

// FUNCTION: LEMBALL 0x00410aa0
void LEMBALL_FASTCALL MarkObjectField178AndSetTimer3C(void* pObject)
{
	*(int*) ((char*) pObject + 0x178) = 1;
	*(int*) ((char*) pObject + 0xcc) = g_nLevelFrameClockTick + 0x3c;
}

// SYNTHETIC: LEMBALL 0x00410ac0
// LevelVtSmallFunctionView::AddClampedU16Field228

// FUNCTION: LEMBALL 0x00410af0
void* __fastcall get_level_mode_mine_chunk_manager(void* pLevelMode)
{
	return *(void**) ((char*) pLevelMode + 0x188);
}

// FUNCTION: LEMBALL 0x00410b00
void* __fastcall get_level_mode_lift_chunk_manager(void* pLevelMode)
{
	return *(void**) ((char*) pLevelMode + 0x18c);
}

// FUNCTION: LEMBALL 0x00410b10
void* __fastcall get_object_field_0x30(void* pObject)
{
	return *(void**) ((char*) pObject + 0x30);
}

// FUNCTION: LEMBALL 0x00410b50
LevelThreeDwordPointVariantB* LevelThreeDwordPointVariantB::Copy(const LevelThreeDwordPointVariantB* pSource)
{
	m_nX = pSource->m_nX;
	m_nY = pSource->m_nY;
	m_nZ = pSource->m_nZ;
	return this;
}

// FUNCTION: LEMBALL 0x004121e0
void __fastcall add_level_time_bonus_counter(void* pLevelMode, void* pUnused, int nBonus)
{
	(void) pUnused;
	*(int*) ((char*) pLevelMode + 0xe8) += nBonus;
}

// FUNCTION: LEMBALL 0x004130f0
void __fastcall set_level_mode_field_0xf8(void* pLevelMode, void* pUnused, int nValue)
{
	(void) pUnused;
	*(int*) ((char*) pLevelMode + 0xf8) = nValue;
}

// FUNCTION: LEMBALL 0x00413370
int __fastcall has_level_plas_history_entries(void* pLevelMode)
{
	void* pPlasManager = *(void**) ((char*) pLevelMode + 0x15c);
	return *(int*) ((char*) pPlasManager + 0x128);
}

// SYNTHETIC: LEMBALL 0x00413390
// LevelVtSmallFunctionView::AddLevelScoreClamped

// FUNCTION: LEMBALL 0x00414880
int LevelManagedEntityChildIterator::GetActiveState(void)
{
	return m_nActiveState168;
}

// FUNCTION: LEMBALL 0x00414890
void* LevelManagedEntityChildIterator::GetChildWithState8(void)
{
	LevelManagedEntityStateIdView* pChild;
	int nStateId;

	nStateId = 8;
	pChild = (LevelManagedEntityStateIdView*) GetFirstChildThunk();
	for (;;) {
		if (pChild == 0) {
			return 0;
		}
		if (pChild->m_nStateIdB8 == nStateId) {
			return pChild;
		}
		pChild = (LevelManagedEntityStateIdView*) GetNextChildThunk();
	}
}

// FUNCTION: LEMBALL 0x004148c0
void* LevelManagedEntityChildIterator::GetCurrentChildWithState8(void)
{
	LevelManagedEntityStateIdView* pChild;
	int nStateId;

	nStateId = 8;
	pChild = (LevelManagedEntityStateIdView*) GetCurrentChildThunk();
	for (;;) {
		if (pChild == 0) {
			return 0;
		}
		if (pChild->m_nStateIdB8 == nStateId) {
			return pChild;
		}
		pChild = (LevelManagedEntityStateIdView*) GetNextChildThunk();
	}
}

// FUNCTION: LEMBALL 0x004149a0
int LevelManagedEntityChildIterator::HasDirtyChildren(void)
{
	LevelManagedEntityDirtyStateView* pChild;
	int fDirty;

	fDirty = 0;
	pChild = (LevelManagedEntityDirtyStateView*) GetFirstChildThunk();
	while (pChild != 0) {
		fDirty = pChild->PollChildDirtyFlagThunk() || fDirty;
		pChild = (LevelManagedEntityDirtyStateView*) GetNextChildThunk();
	}
	return fDirty;
}

// FUNCTION: LEMBALL 0x00414c60
int __fastcall get_managed_entity_group_command_state(void* pEntity)
{
	return *(int*) ((char*) pEntity + 0x154);
}

// FUNCTION: LEMBALL 0x00414c70
void __fastcall set_managed_entity_group_command_state(void* pEntity, void* pUnused, int nState)
{
	(void) pUnused;
	*(int*) ((char*) pEntity + 0x154) = nState;
}

// FUNCTION: LEMBALL 0x00414cb0
LevelThreeDwordPoint* LevelManagedEntityPositionView::CopyWorldPosition(LevelThreeDwordPoint* pPosition)
{
	pPosition->m_nX = m_WorldPosition04.m_nX;
	pPosition->m_nY = m_WorldPosition04.m_nY;
	pPosition->m_nZ = m_WorldPosition04.m_nZ;
	return pPosition;
}

// FUNCTION: LEMBALL 0x00414cd0
void LevelManagedEntityCommandQueue::PopFront(void)
{
	LevelManagedEntityCommandEntry* pDestination;
	LevelManagedEntityCommandEntry* pSource;
	int nOffset;
	int iEntry;

	nOffset = 0;
	if (m_cEntries00 != 0) {
		for (iEntry = 0; iEntry < m_cEntries00 - 1; ++iEntry) {
			pDestination = (LevelManagedEntityCommandEntry*) ((char*) m_pEntries04 + nOffset);
			nOffset += sizeof(LevelManagedEntityCommandEntry);
			pSource = pDestination + 1;
			pDestination->m_nField00 = pSource->m_nField00;
			pDestination->m_nField04 = pSource->m_nField04;
			pDestination->m_nField08 = pSource->m_nField08;
			pDestination->m_nField0C = pSource->m_nField0C;
			pDestination->m_nField10 = pSource->m_nField10;
		}
		--m_cEntries00;
	}
}

// FUNCTION: LEMBALL 0x00414d40
void LevelManagedEntityCommandQueue::PushFront(void)
{
	LevelManagedEntityCommandEntry* pDestination;
	LevelManagedEntityCommandEntry* pSource;
	int cEntries;
	int nOffset;

	cEntries = m_cEntries00;
	if (cEntries > 0) {
		nOffset = cEntries * sizeof(LevelManagedEntityCommandEntry);
		do {
			pDestination = (LevelManagedEntityCommandEntry*) ((char*) m_pEntries04 + nOffset);
			nOffset -= sizeof(LevelManagedEntityCommandEntry);
			pSource = pDestination - 1;
			pDestination->m_nField00 = pSource->m_nField00;
			pDestination->m_nField04 = pSource->m_nField04;
			pDestination->m_nField08 = pSource->m_nField08;
			pDestination->m_nField0C = pSource->m_nField0C;
			pDestination->m_nField10 = pSource->m_nField10;
		} while (nOffset != 0);
	}
	++m_cEntries00;
}

// FUNCTION: LEMBALL 0x00414e00
void LevelManagedEntityQueueCursor::SetPosition(int nX, int nY)
{
	m_nX10 = nX;
	m_nY14 = nY;
	ClampPositionThunk();
}

// FUNCTION: LEMBALL 0x00414e20
void LevelManagedEntityQueueCursor::ClampPosition(void)
{
	if (m_nX10 > m_nMaximumX18) {
		m_nX10 = m_nMaximumX18;
	}
	else if (m_nX10 < 0) {
		m_nX10 = 0;
	}

	if (m_nY14 > m_nMaximumY1C) {
		m_nY14 = m_nMaximumY1C;
	}
	else if (m_nY14 < 0) {
		m_nY14 = 0;
	}
}

// FUNCTION: LEMBALL 0x00414e60
void LevelManagedEntityQueueCursor::GetPosition(int* pnX, int* pnY)
{
	*pnX = m_nX10;
	*pnY = m_nY14;
}

// STUB: LEMBALL 0x00415160
// DestroyLevelChunkObjectBaseAuto

// FUNCTION: LEMBALL 0x00415520
void LevelManagedEntityStateFields::IncrementHeadingOctant(void)
{
	unsigned short nMultiple;
	short nHeading = m_nHeadingOctantB4;
	++nHeading;
	m_nHeadingOctantB4 = nHeading;
	if (nHeading >= 8) {
		nMultiple = (unsigned short) nHeading;
		nMultiple >>= 3;
		nMultiple <<= 3;
		m_nHeadingOctantB4 = nHeading - nMultiple;
	}
}

// FUNCTION: LEMBALL 0x00415550
void LevelManagedEntityStateFields::DecrementHeadingOctant(void)
{
	short nHeading = m_nHeadingOctantB4;
	unsigned short nMultiple;
	--nHeading;
	m_nHeadingOctantB4 = nHeading;
	if (nHeading < 0) {
		nMultiple = 7;
		nMultiple -= nHeading;
		nMultiple >>= 3;
		nHeading += nMultiple * 8;
		m_nHeadingOctantB4 = nHeading;
	}
}

// FUNCTION: LEMBALL 0x00415a20
void __fastcall set_entity_field_0x08_if_flags_bit0(void* pEntity)
{
	if ((*(unsigned char*) ((char*) pEntity + 0x68) & 1) != 0) {
		*(int*) ((char*) pEntity + 8) = 1;
	}
}

// FUNCTION: LEMBALL 0x004160c0
int LevelManagedEntityCommandQueueView::HasPendingCommands(void)
{
	return 0 < *m_pCommandCount70;
}

// FUNCTION: LEMBALL 0x004160e0
void __fastcall clear_managed_entity_move_commands(void* pEntity)
{
	**(unsigned short**) ((char*) pEntity + 0x70) = 0;
}

// FUNCTION: LEMBALL 0x00416570
int LevelManagedEntityStateFields::IsStateNotInactive(void)
{
	int nState = m_nStateIdB8;
	if (nState >= 7) {
		if (nState <= 8 || nState == 0x15) {
			goto inactive;
		}
	}
	return 1;
inactive:
	return 0;
}

// FUNCTION: LEMBALL 0x00417aa0
int return_false_virtual_hook(void)
{
	return 0;
}

// SYNTHETIC: LEMBALL 0x00417ab0
// LevelVtSmallFunctionView::InitializeManagedEntityChildSlotRecord
// SYNTHETIC: LEMBALL 0x00417b00
// LevelVtSmallFunctionView::SetRectFromBounds
// SYNTHETIC: LEMBALL 0x00417b30
// LevelTwoDwordPairVariantView::copy_two_dword_pair_variant
// SYNTHETIC: LEMBALL 0x00417b50
// LevelVtAutoEffDeleteView::Delete17B50
// SYNTHETIC: LEMBALL 0x00417b80
// LevelVtAutoEffDeleteView::Delete17B80
// SYNTHETIC: LEMBALL 0x00417bb0
// LevelVtAutoEffDeleteView::Delete17BB0
// SYNTHETIC: LEMBALL 0x00417be0
// LevelVtAutoEffDeleteView::Delete17BE0
// SYNTHETIC: LEMBALL 0x00417c10
// LevelVtAutoEffDeleteView::Delete17C10

// FUNCTION: LEMBALL 0x00417f40
void* __fastcall get_first_projectile_pool_object(void* pPool)
{
	*(int*) ((char*) pPool + 0xd8) = 0;
	return *(void**) ((char*) pPool + 0x34);
}

// FUNCTION: LEMBALL 0x00417f50
void* __fastcall get_next_projectile_pool_object(void* pPool)
{
	int nIndex;
	int nLimit;

	nIndex = *(int*) ((char*) pPool + 0xd8);
	nIndex++;
	*(int*) ((char*) pPool + 0xd8) = nIndex;
	nLimit = *(int*) ((char*) pPool + 0xd4);
	if (nLimit <= nIndex) {
		return NULL;
	}
	return *(void**) ((char*) pPool + nIndex * 4 + 0x34);
}

// SYNTHETIC: LEMBALL 0x00417f80
// LevelVtSmallFunctionView::AppendProjectilePoolObject

// FUNCTION: LEMBALL 0x00418040
void __fastcall update_projectile_pool_real(void* pPool)
{
	void* pObj;
	void** vtable;
	int (*vfunc)(void*);
	int fKeep;

	pObj = get_first_projectile_pool_object(pPool);
	while (pObj != NULL) {
		vtable = *(void***) pObj;
		vfunc = (int (*)(void*)) vtable[5];
		fKeep = vfunc(pObj);
		if (fKeep == 0) {
			((ProjectilePool*) pPool)->RemoveObject(pObj);
		}
		pObj = get_next_projectile_pool_object(pPool);
	}
}

// FUNCTION: LEMBALL 0x00418080
void ProjectilePool::RemoveObject(void* pObject)
{
	void** pArray;
	int i;
	void** pEntry;
	int nRemaining;

	pArray = m_apObjects;
	for (i = 0; i < 20; i++) {
		if (pArray[i] == pObject) {
			pEntry = &pArray[i];
			deactivate_projectile_object(pEntry[0]);
			if (i < 19) {
				nRemaining = 19 - i;
				i = 19;
				do {
					pEntry[0] = pEntry[1];
					pEntry++;
					nRemaining--;
				} while (nRemaining != 0);
			}
			pArray[i] = NULL;
			m_nCountD4--;
			return;
		}
	}
}

// FUNCTION: LEMBALL 0x004185d0
int LevelManagedEntityHistoryView::PopHistoryId(void)
{
	int iHistoryId;

	if (m_cHistoryIds128 == 0) {
		return 0;
	}
	iHistoryId = --m_cHistoryIds128;
	return m_anHistoryIds12C[iHistoryId];
}

// FUNCTION: LEMBALL 0x00418640
void noop_managed_entity_group_detach_hook(void)
{
}

// FUNCTION: LEMBALL 0x00418720
void PASCAL ResetManagedEntityGroupState(void* pEntity)
{
	void** pVtable;

	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x104 / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x004189f0
LevelManagedEntityChildIterator* LevelManagedEntityActiveGroupView::GetFirstActiveEntity(void)
{
	LevelManagedEntityChildIterator* pEntity;
	int iEntity;

	for (iEntity = 0; iEntity < m_cManagedEntitiesA4; ++iEntity) {
		pEntity = m_apManagedEntities04[iEntity];
		if (pEntity != 0 && pEntity->GetActiveStateThunk() == 1) {
			return m_apManagedEntities04[iEntity];
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00418BA0
void LevelModeChunkEntryArrayView::CopyTripletToFixedPoint(int* pOutput, int nIndex)
{
	pOutput[0] = m_anArrayE0[nIndex] << 12;
	pOutput[1] = m_anArrayF0[nIndex] << 12;
	pOutput[2] = m_anArray100[nIndex] << 12;
}

// FUNCTION: LEMBALL 0x00419b00
void __cdecl ForwardManagedEntityVslot7C(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x7c / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x00419c30
int __cdecl CallHasPendingManagedEntityCommands(void* pUnused, LevelManagedEntityCommandQueueView* pEntity)
{
	(void) pUnused;
	return pEntity->HasPendingCommandsThunk();
}

// FUNCTION: LEMBALL 0x00419c80
void __cdecl ForwardManagedEntityVslot1C(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x1c / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x00419d80
void __cdecl ForwardManagedEntityVslot30(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x30 / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x00419e50
void __cdecl ForwardManagedEntityVslot3C(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x3c / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x00419e60
void __cdecl ForwardManagedEntityVslot40(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x40 / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x00419e70
void __cdecl ForwardManagedEntityVslot68(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x68 / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x00419e80
void __cdecl ForwardManagedEntityVslot24(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x24 / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x00419eb0
void __cdecl ForwardManagedEntityVslot44(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x44 / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x00419f20
void __cdecl ForwardManagedEntityVslotD0(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0xd0 / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x00419f60
void __cdecl ClearPlasChildRuntimeFlag30(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0xdc / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x00419f70
void __cdecl ForwardManagedEntityVslot9C(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x9c / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x00419f80
void __cdecl ForwardManagedEntityVslot8C(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x8c / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x00419f90
void __cdecl ForwardManagedEntityVslot88(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x88 / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x00419fa0
void __cdecl ForwardManagedEntityVslot90(void* pUnused, void* pEntity)
{
	void** pVtable;

	(void) pUnused;
	pVtable = *(void***) pEntity;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0x90 / sizeof(void*)])(pEntity);
}

// FUNCTION: LEMBALL 0x0041a1c0
void noop_projectile_geometry_callback(void)
{
}

// SYNTHETIC: LEMBALL 0x0041a320
// LevelVtSmallFunctionView::GetProjectileGeometryRotatedPair

// FUNCTION: LEMBALL 0x0041ac70
void __fastcall deactivate_projectile_object(void* pObject)
{
	*(int*) ((char*) pObject + 0x164) = 0;
	if (*(int*) ((char*) pObject + 0xb8) != 8) {
		((ProjectileObjectProxy*) pObject)->SetState(8);
	}
	*(int*) ((char*) pObject + 0x114) = 0;
}

// FUNCTION: LEMBALL 0x0041c360
void* __fastcall initialize_three_dword_point_sentinel(void* pPoint)
{
	int* pnPoint = (int*) pPoint;
	pnPoint[0] = (int) 0xaa55aa55;
	pnPoint[1] = (int) 0xaa55aa55;
	pnPoint[2] = (int) 0xaa55aa55;
	return pPoint;
}

// FUNCTION: LEMBALL 0x0041c3f0
int return_true_vtable_callback_0041c3f0(void)
{
	return 1;
}

// FUNCTION: LEMBALL 0x0041c430
int return_two_vtable_callback_0041c430(void)
{
	return 2;
}

// FUNCTION: LEMBALL 0x0041c530
int return_two_linked_type_0x11_slot_10(void)
{
	return 2;
}

// FUNCTION: LEMBALL 0x0041c5b0
int return_true_vtable_callback_0041c5b0(void)
{
	return 1;
}

// FUNCTION: LEMBALL 0x0041c630
int return_two_vtable_callback_0041c630(void)
{
	return 2;
}

// SYNTHETIC: LEMBALL 0x0041cc70
// LevelVtSmallFunctionView::CopyGmobChunkObjectCurrentPosition
// SYNTHETIC: LEMBALL 0x0041d010
// LevelVtSmallFunctionView::GetManagedEntityInteractionPoint
// SYNTHETIC: LEMBALL 0x0041d7b0
// LevelVtSmallFunctionView::CopyGmobChunkObjectCurrentPositionVariant

// FUNCTION: LEMBALL 0x0041dc40
int return_two_vtable_callback_0041dc40(void)
{
	return 2;
}

// STUB: LEMBALL 0x0041de40
// DestroyCompositeLevelChunkChildrenAuto

// FUNCTION: LEMBALL 0x0041def0
int LevelManagedEntityChildIterator::GetChildCount(void)
{
	return m_cChildren124;
}

// FUNCTION: LEMBALL 0x0041df00
void* LevelManagedEntityChildIterator::GetFirstChild(void)
{
	m_iCurrentChild150 = 0;
	return m_apChildren128[0];
}

// FUNCTION: LEMBALL 0x0041df20
void* LevelManagedEntityChildIterator::GetNextChild(void)
{
	++m_iCurrentChild150;
	if (m_iCurrentChild150 >= m_cChildren124) {
		return 0;
	}
	return m_apChildren128[m_iCurrentChild150];
}

// FUNCTION: LEMBALL 0x0041df40
void* LevelManagedEntityChildIterator::GetCurrentChild(void)
{
	if (m_iCurrentChild150 >= m_cChildren124) {
		return 0;
	}
	return m_apChildren128[m_iCurrentChild150];
}

// FUNCTION: LEMBALL 0x0041df60
void* LevelManagedEntityChildIterator::GetChildAt(int iChild)
{
	m_iCurrentChild150 = iChild;
	if (iChild >= m_cChildren124) {
		return 0;
	}
	return m_apChildren128[iChild];
}

// SYNTHETIC: LEMBALL 0x0041df90
// LevelVtSmallFunctionView::SwapObjectChildSlots128

// FUNCTION: LEMBALL 0x0041e020
void LevelManagedEntityChildIterator::AppendChild(void* pChild)
{
	m_apChildren128[m_cChildren124] = pChild;
	m_fChildrenDirty164 = 1;
	++m_cChildren124;
}

// FUNCTION: LEMBALL 0x0041e0c0
int LevelManagedEntityChildIterator::ContainsChild(void* pChild)
{
	int i;
	void** ppChild;

	i = 0;
	if (0 < m_cChildren124) {
		ppChild = m_apChildren128;
		do {
			if (*ppChild == pChild) {
				return 1;
			}
			++ppChild;
			++i;
		} while (i < m_cChildren124);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041e3c0
void PASCAL noop_vtable_callback_0041e3c0(void* pUnused1, void* pUnused2, void* pUnused3)
{
	(void) pUnused1;
	(void) pUnused2;
	(void) pUnused3;
}

// FUNCTION: LEMBALL 0x0041e400
void __fastcall set_object_field_0x160(void* pObject, void* pUnused, int nValue)
{
	(void) pUnused;
	*(int*) ((char*) pObject + 0x160) = nValue;
}

// FUNCTION: LEMBALL 0x0041e410
int __fastcall get_object_field_0x160(void* pObject)
{
	return *(int*) ((char*) pObject + 0x160);
}

// FUNCTION: LEMBALL 0x0041f870
int return_zero_vtable_callback_0041f870(void)
{
	return 0;
}

// FUNCTION: LEMBALL 0x004203d0
int __fastcall is_delayed_move_to_saved_target_armed(void* pObject)
{
	return *(int*) ((char*) pObject + 0x168) == 1;
}

// FUNCTION: LEMBALL 0x004204d0
void __fastcall set_chunk_short_delay_timer(void* pObject)
{
	*(int*) ((char*) pObject + 0xcc) = g_nLevelFrameClockTick + 10;
}

// FUNCTION: LEMBALL 0x00420aa0
int __fastcall get_object_field_0x124(void* pObject)
{
	return *(int*) ((char*) pObject + 0x124);
}

// FUNCTION: LEMBALL 0x00421aa0
void LEMBALL_FASTCALL CallBallChunkEntryVfuncC8(void* pEntry, void* pUnused, int nUnused)
{
	void** pVtable;

	(void) pUnused;
	(void) nUnused;
	pVtable = *(void***) pEntry;
	((LevelManagedEntityNoArgVirtualProc) pVtable[0xc8 / sizeof(void*)])(pEntry);
}

// STUB: LEMBALL 0x004228f0
// DestroyCollChunkObjectAuto

// FUNCTION: LEMBALL 0x00422b30
void LEMBALL_FASTCALL DispatchVtableSlot34WithValue2(void* pObject, void* pUnused)
{
	void** pVtable;

	pVtable = *(void***) pObject;
	((LevelManagedEntityIntVirtualProc) pVtable[0x34 / sizeof(void*)])(pObject, pUnused, 2);
}

// FUNCTION: LEMBALL 0x00422c60
void LEMBALL_FASTCALL DispatchVtableSlot34WithValue2Variant(void* pObject, void* pUnused)
{
	void** pVtable;

	pVtable = *(void***) pObject;
	((LevelManagedEntityIntVirtualProc) pVtable[0x34 / sizeof(void*)])(pObject, pUnused, 2);
}

// FUNCTION: LEMBALL 0x00422c70
void LEMBALL_FASTCALL SendCollMessage16(void* pObject, void* pUnused)
{
	void** pVtable;

	pVtable = *(void***) pObject;
	((LevelManagedEntityIntVirtualProc) pVtable[0x34 / sizeof(void*)])(pObject, pUnused, 0x16);
}

// FUNCTION: LEMBALL 0x00422fa0
void noop_vtable_callback_00422fa0(void)
{
}

// FUNCTION: LEMBALL 0x00423040
void noop_vtable_callback_00423040(void)
{
}

// FUNCTION: LEMBALL 0x00423050
int return_true_vtable_callback_00423050(void)
{
	return 1;
}

// FUNCTION: LEMBALL 0x00423e90
int PASCAL IsManagedEntityState8Or18(int nState)
{
	if (nState == 8 || nState == 0x18) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00426c20
void __fastcall clear_rock_chunk_object_count(void* pManager)
{
	*(int*) ((char*) pManager + 0x34) = 0;
}

// SYNTHETIC: LEMBALL 0x00427890
// LevelVtSmallFunctionView::GetTileFlag1cObjectRenderPoint

// FUNCTION: LEMBALL 0x00427a90
int PASCAL get_tile_flag_0x1c_object_render_entry_count(void)
{
	return 2;
}

// FUNCTION: LEMBALL 0x0042c9a0
void noop_vtable_callback_0042c9a0(void)
{
}

// FUNCTION: LEMBALL 0x0042d900
void __fastcall reset_ice_chunk_object_count(void* pManager)
{
	*(int*) ((char*) pManager + 0x34) = 0;
}

// FUNCTION: LEMBALL 0x0042dd30
int PASCAL return_zero_vtable_callback_0042dd30(void* pUnused)
{
	(void) pUnused;
	return 0;
}

// FUNCTION: LEMBALL 0x0042eeb0
void __fastcall set_move_chunk_object_trigger_latch(void* pObject)
{
	*(int*) ((char*) pObject + 0x148) = 1;
}

// SYNTHETIC: LEMBALL 0x0042eec0
// return_zero_level_callback_0042eec0

// FUNCTION: LEMBALL 0x0042f2e0
void __fastcall reset_move_chunk_object_count(void* pManager)
{
	*(int*) ((char*) pManager + 0x34) = 0;
}

// FUNCTION: LEMBALL 0x00435880
int __fastcall return_object_field_0x0c_00435880(void* pObject)
{
	return *(int*) ((char*) pObject + 0x0c);
}

// FUNCTION: LEMBALL 0x00435890
void __fastcall set_object_field_0x10_00435890(void* pObject, void* pUnused, int nValue)
{
	(void) pUnused;
	*(int*) ((char*) pObject + 0x10) = nValue;
}

// FUNCTION: LEMBALL 0x004358a0
void __fastcall set_object_field_0x0c_004358a0(void* pObject, void* pUnused, int nValue)
{
	(void) pUnused;
	*(int*) ((char*) pObject + 0x0c) = nValue;
}

// FUNCTION: LEMBALL 0x004358b0
void __fastcall set_object_field_0x18_004358b0(void* pObject, void* pUnused, int nValue)
{
	(void) pUnused;
	*(int*) ((char*) pObject + 0x18) = nValue;
}

// FUNCTION: LEMBALL 0x004453a0
unsigned int return_hresult_80000000_callback(void)
{
	return 0x80000000U;
}

// FUNCTION: LEMBALL 0x0044b5d0
void* __fastcall clear_two_word_point(void* pPoint)
{
	*(unsigned short*) pPoint = 0;
	*(unsigned short*) ((char*) pPoint + 2) = 0;
	return pPoint;
}

// FUNCTION: LEMBALL 0x0044fbc0
void noop_queued_variant_child_slot_manager_update(void)
{
}

// FUNCTION: LEMBALL 0x004510d0
unsigned int return_hresult_40000001_callback(void)
{
	return 0x40000001U;
}

// FUNCTION: LEMBALL 0x00452b80
void noop_network_lobby_vsnet_runtime_slot_3(void)
{
}

// FUNCTION: LEMBALL 0x00456600
void noop_callback_00456600(void)
{
}

int PASCAL return_zero_level_callback_0042eec0(void* pUnused1, void* pUnused2)
{
	(void) pUnused1;
	(void) pUnused2;
	return 0;
}

struct LevelTwoDwordPairVariantView {
	int m_anValues[2];

	void* copy_two_dword_pair_variant(const int* pSource);
};

void* LevelTwoDwordPairVariantView::copy_two_dword_pair_variant(const int* pSource)
{
	m_anValues[0] = pSource[0];
	m_anValues[1] = pSource[1];
	return this;
}

struct LevelVtSmallFunctionView {
	void AddClampedU16Field228(unsigned short nValue);
	void AddLevelScoreClamped(int nValue);
	void* InitializeManagedEntityChildSlotRecord(void);
	void SetRectFromBounds(int nLeft, int nTop, int nRight, int nBottom);
	void AppendProjectilePoolObject(void* pObject);
	void* GetProjectileGeometryRotatedPair(int nIndex);
	void CopyGmobChunkObjectCurrentPosition(int* pPoint);
	void GetManagedEntityInteractionPoint(int* pPoint);
	void CopyGmobChunkObjectCurrentPositionVariant(int* pPoint);
	void SwapObjectChildSlots128(int iFirst, int iSecond);
	void GetTileFlag1cObjectRenderPoint(int* pPoint);
};

void LevelVtSmallFunctionView::AddClampedU16Field228(unsigned short nValue)
{
	unsigned short nResult;

	nResult = (unsigned short) (*(unsigned short*) ((char*) this + 0x228) + nValue);
	*(unsigned short*) ((char*) this + 0x228) = nResult;
	if (nResult > 0x32) {
		*(unsigned short*) ((char*) this + 0x228) = 0x32;
	}
}

void LevelVtSmallFunctionView::AddLevelScoreClamped(int nValue)
{
	int nResult;

	nResult = *(int*) ((char*) this + 0xf0) + nValue;
	*(int*) ((char*) this + 0xf0) = nResult;
	if (nResult > 9999999) {
		*(int*) ((char*) this + 0xf0) = 9999999;
	}
}

void* LevelVtSmallFunctionView::InitializeManagedEntityChildSlotRecord(void)
{
	int nSentinel;

	nSentinel = (int) 0xaa55aa55;
	*(int*) this = 0;
	*(int*) ((char*) this + 4) = nSentinel;
	*(int*) ((char*) this + 8) = nSentinel;
	*(int*) ((char*) this + 0x0c) = nSentinel;
	return this;
}

void LevelVtSmallFunctionView::SetRectFromBounds(int nLeft, int nTop, int nRight, int nBottom)
{
	int nLeftValue;
	int nTopValue;
	int nWidth;
	int nHeight;

	nWidth = *(volatile int*) &nRight;
	nLeftValue = *(volatile int*) &nLeft;
	nHeight = *(volatile int*) &nBottom;
	nWidth -= nLeftValue;
	*(int*) this = nLeftValue;
	*(int*) ((char*) this + 8) = nWidth;
	nTopValue = *(volatile int*) &nTop;
	nHeight -= nTopValue;
	*(int*) ((char*) this + 0x0c) = nHeight;
	*(int*) ((char*) this + 4) = nTopValue;
}

void LevelVtSmallFunctionView::AppendProjectilePoolObject(void* pObject)
{
	int nIndex;

	nIndex = *(int*) ((char*) this + 0xd4);
	*(void**) ((char*) this + 0x34 + nIndex * 4) = pObject;
	++*(int*) ((char*) this + 0xd4);
}

void* LevelVtSmallFunctionView::GetProjectileGeometryRotatedPair(int nIndex)
{
	if (nIndex >= 8) {
		nIndex -= 8;
	}
	return (char*) this + 0xc0 + nIndex * 8;
}

void LevelVtSmallFunctionView::CopyGmobChunkObjectCurrentPosition(int* pPoint)
{
	pPoint[0] = *(int*) ((char*) this + 0x9c);
	pPoint[1] = *(int*) ((char*) this + 0xa0);
	pPoint[2] = *(int*) ((char*) this + 0xa4);
}

void LevelVtSmallFunctionView::GetManagedEntityInteractionPoint(int* pPoint)
{
	pPoint[1] = *(int*) ((char*) this + 0xa0) - 0x8000;
	pPoint[0] = *(int*) ((char*) this + 0x9c) - 0x30000;
	pPoint[2] = *(int*) ((char*) this + 0xa4);
}

void LevelVtSmallFunctionView::CopyGmobChunkObjectCurrentPositionVariant(int* pPoint)
{
	pPoint[0] = *(int*) ((char*) this + 0x9c);
	pPoint[1] = *(int*) ((char*) this + 0xa0);
	pPoint[2] = *(int*) ((char*) this + 0xa4);
}

void LevelVtSmallFunctionView::SwapObjectChildSlots128(int iFirst, int iSecond)
{
	void** ppFirst;
	void** ppSecond;
	void* pValue;

	ppSecond = (void**) ((char*) this + 0x128 + iSecond * 4);
	ppFirst = (void**) ((char*) this + 0x128 + iFirst * 4);
	pValue = *ppSecond;
	*ppSecond = *ppFirst;
	*ppFirst = pValue;
}

void LevelVtSmallFunctionView::GetTileFlag1cObjectRenderPoint(int* pPoint)
{
	pPoint[0] = *(int*) ((char*) this + 0x9c);
	pPoint[1] = *(int*) ((char*) this + 0xa0) + 0x8000;
	pPoint[2] = *(int*) ((char*) this + 0xa4);
}

struct LevelVtAutoEffDeleteView {
	void* Delete17B50(BYTE fDelete);
	void* Delete17B80(BYTE fDelete);
	void* Delete17BB0(BYTE fDelete);
	void* Delete17BE0(BYTE fDelete);
	void* Delete17C10(BYTE fDelete);
};

#define DEFINE_LEVELVT_EFF_DELETE(name)                                                                                \
	void* LevelVtAutoEffDeleteView::name(BYTE fDelete)                                                                 \
	{                                                                                                                  \
		((VsNetEffStreamCommon*) this)->VsNetEffStreamCommon::~VsNetEffStreamCommon();                                 \
		if ((fDelete & 1) != 0) {                                                                                      \
			FreeVSMemBlock(this);                                                                                      \
		}                                                                                                              \
		return this;                                                                                                   \
	}

DEFINE_LEVELVT_EFF_DELETE(Delete17B50)
DEFINE_LEVELVT_EFF_DELETE(Delete17B80)
DEFINE_LEVELVT_EFF_DELETE(Delete17BB0)
DEFINE_LEVELVT_EFF_DELETE(Delete17BE0)
DEFINE_LEVELVT_EFF_DELETE(Delete17C10)

#undef DEFINE_LEVELVT_EFF_DELETE

// Comparison-only compatibility stub.
void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAuto(void*)
{
}

// Compatibility shim for original ILT call site.
void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject)
{
	DestroyLevelChunkObjectBaseAuto(pObject);
}

// SYNTHETIC: LEMBALL 0x0040ce50
// DeleteLevelChunkObjectBaseScalarA
// SYNTHETIC: LEMBALL 0x00417ad0
// DeleteLevelChunkObjectBaseWrapper17AD0
// SYNTHETIC: LEMBALL 0x0041c400
// DeleteLevelChunkObjectBaseWrapper1C400
// SYNTHETIC: LEMBALL 0x0041c440
// DeleteLevelChunkObjectBaseWrapper1C440
// SYNTHETIC: LEMBALL 0x0041c5c0
// DeleteLevelChunkObjectBaseWrapper1C5C0
// SYNTHETIC: LEMBALL 0x0041c600
// DeleteLevelChunkObjectBaseWrapper1C600
// SYNTHETIC: LEMBALL 0x0041c640
// DeleteLevelChunkObjectBaseWrapper1C640
// SYNTHETIC: LEMBALL 0x0041fb70
// DeleteLevelChunkObjectBaseWrapper1FB70

#define DEFINE_LEVELVT_CHUNK_BASE_DELETE(name)                                                                         \
	void* LEMBALL_FASTCALL name(void* pObject, void*, BYTE fDelete)                                                    \
	{                                                                                                                  \
		DestroyLevelChunkObjectBaseAutoThunk(pObject);                                                                 \
		if ((fDelete & 1) != 0) {                                                                                      \
			FreeVSMemBlock(pObject);                                                                                   \
		}                                                                                                              \
		return pObject;                                                                                                \
	}

DEFINE_LEVELVT_CHUNK_BASE_DELETE(DeleteLevelChunkObjectBaseScalarA)
DEFINE_LEVELVT_CHUNK_BASE_DELETE(DeleteLevelChunkObjectBaseWrapper17AD0)
DEFINE_LEVELVT_CHUNK_BASE_DELETE(DeleteLevelChunkObjectBaseWrapper1C400)
DEFINE_LEVELVT_CHUNK_BASE_DELETE(DeleteLevelChunkObjectBaseWrapper1C440)
DEFINE_LEVELVT_CHUNK_BASE_DELETE(DeleteLevelChunkObjectBaseWrapper1C5C0)
DEFINE_LEVELVT_CHUNK_BASE_DELETE(DeleteLevelChunkObjectBaseWrapper1C600)
DEFINE_LEVELVT_CHUNK_BASE_DELETE(DeleteLevelChunkObjectBaseWrapper1C640)
DEFINE_LEVELVT_CHUNK_BASE_DELETE(DeleteLevelChunkObjectBaseWrapper1FB70)

#undef DEFINE_LEVELVT_CHUNK_BASE_DELETE

// Comparison-only compatibility stub.
void LEMBALL_FASTCALL DestroyCompositeLevelChunkChildrenAuto(void*)
{
}

// Compatibility shim for original ILT call site.
void LEMBALL_FASTCALL DestroyCompositeLevelChunkChildrenAutoThunk(void* pObject)
{
	DestroyCompositeLevelChunkChildrenAuto(pObject);
}

// Comparison-only compatibility stub.
void LEMBALL_FASTCALL DestroyCollChunkObjectAuto(void*)
{
}

// Compatibility shim for original ILT call site.
void LEMBALL_FASTCALL DestroyCollChunkObjectAutoThunk(void* pObject)
{
	DestroyCollChunkObjectAuto(pObject);
}

// SYNTHETIC: LEMBALL 0x0041f960
// DeleteCompositeLevelChunkChildrenWrapper1F960
// SYNTHETIC: LEMBALL 0x00422fb0
// DeleteCollChunkObject0B0C
// SYNTHETIC: LEMBALL 0x00422fe0
// DeleteCollChunkObject12
// SYNTHETIC: LEMBALL 0x00423010
// DeleteCollChunkObject1B
// SYNTHETIC: LEMBALL 0x00423060
// DeleteCollChunkObjectWrapper23060

#define DEFINE_LEVELVT_SCALAR_DELETE(name, destroy)                                                                    \
	void* LEMBALL_FASTCALL name(void* pObject, void*, BYTE fDelete)                                                    \
	{                                                                                                                  \
		destroy(pObject);                                                                                              \
		if ((fDelete & 1) != 0) {                                                                                      \
			FreeVSMemBlock(pObject);                                                                                   \
		}                                                                                                              \
		return pObject;                                                                                                \
	}

DEFINE_LEVELVT_SCALAR_DELETE(DeleteCompositeLevelChunkChildrenWrapper1F960, DestroyCompositeLevelChunkChildrenAutoThunk)
DEFINE_LEVELVT_SCALAR_DELETE(DeleteCollChunkObject0B0C, DestroyCollChunkObjectAutoThunk)
DEFINE_LEVELVT_SCALAR_DELETE(DeleteCollChunkObject12, DestroyCollChunkObjectAutoThunk)
DEFINE_LEVELVT_SCALAR_DELETE(DeleteCollChunkObject1B, DestroyCollChunkObjectAutoThunk)
DEFINE_LEVELVT_SCALAR_DELETE(DeleteCollChunkObjectWrapper23060, DestroyCollChunkObjectAutoThunk)

#undef DEFINE_LEVELVT_SCALAR_DELETE

#define DEFINE_LEVELVT_DESTROY_PAIR(realName, thunkName)                                                               \
	void LEMBALL_FASTCALL realName(void*)                                                                              \
	{                                                                                                                  \
	}                                                                                                                  \
	void LEMBALL_FASTCALL thunkName(void* pObject)                                                                     \
	{                                                                                                                  \
		realName(pObject);                                                                                             \
	}

#define DEFINE_LEVELVT_MANAGER_DESTROY_PAIR(realName, thunkName, vtable, childOffset)                                  \
	void LEMBALL_FASTCALL realName(void* pObject)                                                                      \
	{                                                                                                                  \
		void* pChild;                                                                                                  \
		*(void**) pObject = vtable;                                                                                    \
		pChild = *(void**) ((char*) pObject + childOffset);                                                            \
		if (pChild != 0) {                                                                                             \
			((LevelVtDeletableChild*) pChild)->Delete(3);                                                              \
		}                                                                                                              \
		((VsNetEffStreamCommon*) pObject)->VsNetEffStreamCommon::~VsNetEffStreamCommon();                              \
	}                                                                                                                  \
	void LEMBALL_FASTCALL thunkName(void* pObject)                                                                     \
	{                                                                                                                  \
		realName(pObject);                                                                                             \
	}

// STUB: LEMBALL 0x00414070
// DestroyLevelChunkObjectChildrenVariantAuto
// LINKERILT: LEMBALL 0x004020ef
// DestroyLevelChunkObjectChildrenVariantAutoThunk
DEFINE_LEVELVT_DESTROY_PAIR(DestroyLevelChunkObjectChildrenVariantAuto, DestroyLevelChunkObjectChildrenVariantAutoThunk)

// STUB: LEMBALL 0x00414de0
// DestroyManagedEntityQueueCursorAuto
// LINKERILT: LEMBALL 0x00402f09
// DestroyManagedEntityQueueCursorAutoThunk
DEFINE_LEVELVT_DESTROY_PAIR(DestroyManagedEntityQueueCursorAuto, DestroyManagedEntityQueueCursorAutoThunk)

// STUB: LEMBALL 0x0041b110
// DestroyCountedChildTableStreamAuto
// LINKERILT: LEMBALL 0x004025fe
// DestroyCountedChildTableStreamAutoThunk
DEFINE_LEVELVT_DESTROY_PAIR(DestroyCountedChildTableStreamAuto, DestroyCountedChildTableStreamAutoThunk)

// STUB: LEMBALL 0x0041d120
// DestroyGmobChunkType14ObjectAuto
// LINKERILT: LEMBALL 0x00402be9
// DestroyGmobChunkType14ObjectAutoThunk
DEFINE_LEVELVT_DESTROY_PAIR(DestroyGmobChunkType14ObjectAuto, DestroyGmobChunkType14ObjectAutoThunk)

// STUB: LEMBALL 0x0041fda0
// DestroyThreeOptionalRecordChunkObjectAuto
// LINKERILT: LEMBALL 0x00403472
// DestroyThreeOptionalRecordChunkObjectAutoThunk
DEFINE_LEVELVT_DESTROY_PAIR(DestroyThreeOptionalRecordChunkObjectAuto, DestroyThreeOptionalRecordChunkObjectAutoThunk)

// FUNCTION: LEMBALL 0x004224c0
// DestroyCollChunkManagerAuto
// LINKERILT: LEMBALL 0x004026e9
// DestroyCollChunkManagerAutoThunk
void LEMBALL_FASTCALL DestroyCollChunkManagerAuto(void* pObject)
{
	int i;
	int nOffset;

	*(void**) pObject = g_LINKSCF_CollChunkManagerVtable;
	if (*(void**) ((char*) pObject + 0x34) != 0) {
		i = 0;
		for (nOffset = 0; nOffset < *(int*) ((char*) pObject + 0x3c); ++nOffset) {
			void* pChild = *(void**) ((char*) *(void**) ((char*) pObject + 0x34) + i);
			if (pChild != 0) {
				((LevelVtDeletableChild*) pChild)->Delete(1);
			}
			i += 4;
		}
		FreeVSMemBlock(*(void**) ((char*) pObject + 0x34));
	}
	((VsNetEffStreamCommon*) pObject)->VsNetEffStreamCommon::~VsNetEffStreamCommon();
}
void LEMBALL_FASTCALL DestroyCollChunkManagerAutoThunk(void* pObject)
{
	DestroyCollChunkManagerAuto(pObject);
}

// FUNCTION: LEMBALL 0x00426c00
// DestroyRockChunkManagerAuto
// LINKERILT: LEMBALL 0x00401cc1
// DestroyRockChunkManagerAutoThunk
DEFINE_LEVELVT_MANAGER_DESTROY_PAIR(DestroyRockChunkManagerAuto,
									DestroyRockChunkManagerAutoThunk,
									g_LINKSCF_RockChunkManagerVtable,
									0x38)

// STUB: LEMBALL 0x004275e0
// DestroyTileFlag1CChunkObjectAuto
// LINKERILT: LEMBALL 0x00402df1
// DestroyTileFlag1CChunkObjectAutoThunk
DEFINE_LEVELVT_DESTROY_PAIR(DestroyTileFlag1CChunkObjectAuto, DestroyTileFlag1CChunkObjectAutoThunk)

// FUNCTION: LEMBALL 0x0042c120
// DestroyPgunChunkManagerAuto
// LINKERILT: LEMBALL 0x0040363e
// DestroyPgunChunkManagerAutoThunk
DEFINE_LEVELVT_MANAGER_DESTROY_PAIR(DestroyPgunChunkManagerAuto,
									DestroyPgunChunkManagerAutoThunk,
									g_LINKSCF_PgunChunkManagerVtable,
									0x38)

// FUNCTION: LEMBALL 0x0042d8e0
// DestroyIceChunkManagerAuto
// LINKERILT: LEMBALL 0x00402743
// DestroyIceChunkManagerAutoThunk
DEFINE_LEVELVT_MANAGER_DESTROY_PAIR(DestroyIceChunkManagerAuto,
									DestroyIceChunkManagerAutoThunk,
									g_LINKSCF_IceChunkManagerVtable,
									0x38)

// FUNCTION: LEMBALL 0x0042f2c0
// DestroyMoveChunkManagerAuto
void LEMBALL_FASTCALL DestroyMoveChunkManagerAuto(void* pObject)
{
	void* pChild;

	*(void**) pObject = g_LINKSCF_MoveChunkManagerVtable;
	pChild = *(void**) ((char*) pObject + 0x38);
	if (pChild != 0) {
		((LevelVtDeletableChild*) pChild)->Delete(3);
	}
	((VsNetEffStreamCommon*) pObject)->VsNetEffStreamCommon::~VsNetEffStreamCommon();
}

#undef DEFINE_LEVELVT_MANAGER_DESTROY_PAIR
#undef DEFINE_LEVELVT_DESTROY_PAIR

// SYNTHETIC: LEMBALL 0x00414c80
// DeleteLevelChunkObjectChildrenVariant14C80
// SYNTHETIC: LEMBALL 0x00414f00
// DeleteManagedEntityQueueCursorAuto
// SYNTHETIC: LEMBALL 0x0041c330
// DeleteCountedChildTableStreamAuto
// SYNTHETIC: LEMBALL 0x0041dd70
// DeleteGmobChunkType14ObjectAuto
// SYNTHETIC: LEMBALL 0x00420ab0
// DeleteThreeOptionalRecordChunkObjectAuto
// SYNTHETIC: LEMBALL 0x00422f70
// DeleteCollChunkManagerAuto
// SYNTHETIC: LEMBALL 0x004274f0
// DeleteRockChunkManagerAuto
// SYNTHETIC: LEMBALL 0x00427aa0
// DeleteTileFlag1CChunkObjectAuto
// SYNTHETIC: LEMBALL 0x0042c9b0
// DeletePgunChunkManagerAuto
// SYNTHETIC: LEMBALL 0x0042e4d0
// DeleteIceChunkManagerAuto

#define DEFINE_LEVELVT_NAMED_SCALAR_DELETE(name, destroy)                                                              \
	void* LEMBALL_FASTCALL name(void* pObject, void*, BYTE fDelete)                                                    \
	{                                                                                                                  \
		destroy(pObject);                                                                                              \
		if ((fDelete & 1) != 0) {                                                                                      \
			FreeVSMemBlock(pObject);                                                                                   \
		}                                                                                                              \
		return pObject;                                                                                                \
	}

DEFINE_LEVELVT_NAMED_SCALAR_DELETE(DeleteLevelChunkObjectChildrenVariant14C80,
								   DestroyLevelChunkObjectChildrenVariantAutoThunk)
DEFINE_LEVELVT_NAMED_SCALAR_DELETE(DeleteManagedEntityQueueCursorAuto, DestroyManagedEntityQueueCursorAutoThunk)
DEFINE_LEVELVT_NAMED_SCALAR_DELETE(DeleteCountedChildTableStreamAuto, DestroyCountedChildTableStreamAutoThunk)
DEFINE_LEVELVT_NAMED_SCALAR_DELETE(DeleteGmobChunkType14ObjectAuto, DestroyGmobChunkType14ObjectAutoThunk)
DEFINE_LEVELVT_NAMED_SCALAR_DELETE(DeleteThreeOptionalRecordChunkObjectAuto,
								   DestroyThreeOptionalRecordChunkObjectAutoThunk)
DEFINE_LEVELVT_NAMED_SCALAR_DELETE(DeleteCollChunkManagerAuto, DestroyCollChunkManagerAutoThunk)
DEFINE_LEVELVT_NAMED_SCALAR_DELETE(DeleteRockChunkManagerAuto, DestroyRockChunkManagerAutoThunk)
DEFINE_LEVELVT_NAMED_SCALAR_DELETE(DeleteTileFlag1CChunkObjectAuto, DestroyTileFlag1CChunkObjectAutoThunk)
DEFINE_LEVELVT_NAMED_SCALAR_DELETE(DeletePgunChunkManagerAuto, DestroyPgunChunkManagerAutoThunk)
DEFINE_LEVELVT_NAMED_SCALAR_DELETE(DeleteIceChunkManagerAuto, DestroyIceChunkManagerAutoThunk)

#undef DEFINE_LEVELVT_NAMED_SCALAR_DELETE

extern void LEMBALL_FASTCALL DestroyDoorChunkObjectAutoThunk(void* pObject);

// SYNTHETIC: LEMBALL 0x00410b20
// DeleteDoorChunkObjectAuto
void* LEMBALL_FASTCALL DeleteDoorChunkObjectAuto(void* pObject, void*, BYTE fDelete)
{
	DestroyDoorChunkObjectAutoThunk(pObject);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

extern "C" int _purecall(void);

// GLOBAL: LEMBALL 0x004930a0
static void* g_LEVEL_SimpleVtableObjectVtable[4] = {
	(void*) DeleteSimpleVtableObject407EF0Thunk,
	(void*) _purecall,
	0,
	0,
};

// SYNTHETIC: LEMBALL 0x00407ef0
// DeleteSimpleVtableObject407EF0
// SYNTHETIC: LEMBALL 0x00408010
// DeleteSimpleVtableObject408010

#define DEFINE_LEVELVT_SIMPLE_VTABLE_DELETE(name)                                                                      \
	void* LEMBALL_FASTCALL name(void* pObject, void*, BYTE fDelete)                                                    \
	{                                                                                                                  \
		*(void**) pObject = g_LEVEL_SimpleVtableObjectVtable;                                                          \
		if ((fDelete & 1) != 0) {                                                                                      \
			FreeVSMemBlock(pObject);                                                                                   \
		}                                                                                                              \
		return pObject;                                                                                                \
	}

DEFINE_LEVELVT_SIMPLE_VTABLE_DELETE(DeleteSimpleVtableObject407EF0)
DEFINE_LEVELVT_SIMPLE_VTABLE_DELETE(DeleteSimpleVtableObject408010)

#undef DEFINE_LEVELVT_SIMPLE_VTABLE_DELETE
