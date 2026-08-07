// Cluster reconstruction supplemental TU (avoids append-corruption on the large ClusterScanOps.cpp).
#include "Visos/Generic/Memory.h"

extern void __fastcall AppendType18ChunkObject(void* pStream, int nUnused, unsigned short param_1, void* param_2, int param_3, int param_4);
extern void __fastcall DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void __fastcall ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void __fastcall ReleaseTypedResourceObjectIfLoaded(void* pObject, void* pUnusedEdx, int fReleaseMode);
extern void* g_pActiveManagedEntityOwner;
struct PlasChildStateEntityView;
extern void __cdecl DispatchPlasChildStateTableVariant0(void* pContext, PlasChildStateEntityView* pEntity);
extern void __cdecl DispatchPlasChildStateTableVariant1(void* pContext, PlasChildStateEntityView* pEntity);
// Palette remap pointer table view (mirrors VSINIT.CPP's PaletteRemapPointerTableMemberView for member-symbol linking).
struct PaletteRemapPointerTableMemberView {
	void** m_ppItems;
	int m_nCursor;
	void ReleasePaletteRemapVariant(void* pVariant);
};

// MACINTOSH: append_type_0x18_chunk_object_from_tile_coords(int, int, int, int)
// FUNCTION: LEMBALL 0x00412eb0
void __fastcall AppendType18ChunkObjectFromTileCoords(void* pObject, int nUnused, int param_1, int param_2, int param_3, int param_4)
{
	int nSlot = ((int(__fastcall*)()) 0x40227a)();
	int local_c = param_1 << 12;
	int local_8 = param_2 << 12;
	int local_4 = param_3 << 12;
	AppendType18ChunkObject(*(void**) ((char*) pObject + 0x1c4), nUnused, (unsigned short) nSlot, &local_c, 0, param_4);
}

// MACINTOSH: start_door_chunk_object_vertical_motion()
// FUNCTION: LEMBALL 0x0040f600
void __fastcall StartDoorChunkObjectVerticalMotion(void* pObject)
{
	int vec[3];
	vec[0] = 0;
	vec[1] = 0;
	*(int*) ((char*) pObject + 0x10c) = 1;
	vec[2] = 0xa000;
	(*( void(**)(void*, int)) (*(void***) pObject + 0x20 / 4))(vec, 0);
	*(int*) ((char*) pObject + 0x2c) = 1;
}

// MACINTOSH: count_active_network_lobby_peer_streams()
// FUNCTION: LEMBALL 0x00452bc0
int __fastcall CountActiveNetworkLobbyPeerStreams(void* pObject)
{
	int iVar1 = 0;
	int i;
	for (i = 0; i < 10; i++) {
		if (*(int*) ((char*) pObject + 0x20 + i * 4) != 0 && *(int*) (*(int*) ((char*) pObject + 0x14) + 0x4c + i * 0x50) != 0) {
			iVar1++;
		}
	}
	return iVar1;
}

// MACINTOSH: has_projectile_request_code_queued(int)
// FUNCTION: LEMBALL 0x0040f960
int __fastcall HasProjectileRequestCodeQueued(void* pObject, int nUnused, int param_1)
{
	if (param_1 == 5) {
		if (*(short*) ((char*) pObject + 0x228) == 0x32) {
			return 1;
		}
	} else {
		int iVar1 = *(int*) ((char*) pObject + 0x220);
		if (iVar1 != 0xc && iVar1 > 0) {
			int* piVar3 = (int*) ((char*) pObject + 0x1c0);
			int iVar2 = 0;
			do {
				if (*piVar3 == param_1) {
					return 1;
				}
				piVar3++;
				iVar2++;
			} while (iVar2 < iVar1);
		}
	}
	return 0;
}

// MACINTOSH: forward_queued_zrle_variant_range_point(ushort, ushort)
// FUNCTION: LEMBALL 0x0044f240
void __fastcall ForwardQueuedZrleVariantRangePoint(void* pObject, int nUnused, unsigned short param_2, unsigned short param_3)
{
	if (*(void**) ((char*) pObject + 0x4c) != 0) {
		struct Pt { unsigned short x; unsigned short y; } pt;
		pt.x = param_2;
		pt.y = param_3;
		(*( void(**)(void*)) (*(void***) *(void**) ((char*) pObject + 0x4c) + 0x38 / 4))(&pt);
	}
}

// MACINTOSH: construct_palette_remap_variant(int, byte*, int)
// FUNCTION: LEMBALL 0x0046aa80
void* __fastcall ConstructPaletteRemapVariant(void* pThis, int nUnused, int param_1, unsigned char* param_2, int param_3)
{
	*(void**) ((char*) pThis + 4) = (void*) ((int(__cdecl*)(int)) 0x45dd90)(param_1);
	if (param_3 == 1) {
		((void(__fastcall*)(void*)) 0x46ab70)(pThis);
		return pThis;
	}
	if (param_3 != 2) {
		*(unsigned char**) pThis = param_2;
		return pThis;
	}
	((void(__fastcall*)(void*, unsigned char*)) 0x46aaf0)(pThis, param_2);
	return pThis;
}

// MACINTOSH: finish_paintball_sequence_to_level_selection()
// FUNCTION: LEMBALL 0x00450a10
void __fastcall FinishPaintballSequenceToLevelSelection(void* pObject)
{
	if (*(int*) 0x4a62f8 != 0) {
		(*( void(**)(int)) *(void***) *(void**) 0x4a1bd0)(*(int*) 0x4a97b8);
	}
	if (*(int*) 0x4a62fc != 0) {
		(*( void(**)(int)) *(void***) *(void**) 0x4aa100)(*(int*) 0x4a97b8);
	}
	*(int*) ((char*) pObject + 0x378) = 1;
	*(int*) ((char*) pObject + 0x374) = 4;
}

// MACINTOSH: deactivate_embedded_media_if_active()
// FUNCTION: LEMBALL 0x00450820
void __fastcall DeactivateEmbeddedMediaIfActive(void* pObject)
{
	if (*(int*) ((char*) pObject + 0x5fc) != 0 && *(int*) ((char*) pObject + 0x524) == 1) {
		(*( void(**)(void)) (*(void***) *(void**) ((char*) pObject + 0x520) + 0x74 / 4))();
		*(int*) ((char*) pObject + 0x5fc) = 0;
		*(int*) ((char*) pObject + 0x5f8) = ((unsigned(*)(void)) 0x462e80)() + 0x28;
	}
}

// MACINTOSH: point_in_helper_clip_rect(int, int)
// FUNCTION: LEMBALL 0x00475f60
int __fastcall PointInHelperClipRect(void* pObject, int nUnused, int param_1, int param_2)
{
	int iVar1 = *(int*) (*(int*) ((char*) pObject + 0x40) + 4);
	int iVar2 = (int) *(short*) ((char*) pObject + iVar1 + 0x68);
	if (iVar2 <= param_1 && param_1 <= *(short*) ((char*) pObject + iVar1 + 100) + iVar2 - 1) {
		iVar2 = (int) *(short*) ((char*) pObject + iVar1 + 0x6a);
		if (iVar2 <= param_2 && param_2 <= *(short*) ((char*) pObject + iVar1 + 0x66) + iVar2 - 1) {
			return 1;
		}
	}
	return 0;
}

// MACINTOSH: destroy_lift_chunk_manager_0x190_stride_array()
// FUNCTION: LEMBALL 0x00425dc0
void __fastcall DestroyLiftChunkManager0x190StrideArray(void* pObject)
{
	int i;
	if (*(int*) ((char*) pObject + 0x34) > 0) {
		for (i = 0; i < *(int*) ((char*) pObject + 0x34); i++) {
			((void(__fastcall*)(void*)) 0x402a77)((void*) (*(int*) ((char*) pObject + 0x3c) + i * 0x190));
		}
	}
}

// MACINTOSH: handle_queue_cursor_event(short*)
// FUNCTION: LEMBALL 0x00414e80
int __fastcall HandleQueueCursorEvent(void* pObject, int nUnused, short* param_1)
{
	if (*param_1 != 1) {
		*(int*) ((char*) pObject + 0xc) = *(int*) ((char*) pObject + 0xc) + 1;
		return 0;
	}
	((void(__fastcall*)(void*, int, int)) 0x40281f)(pObject, *(int*) ((char*) param_1 + 8), *(int*) ((char*) param_1 + 0xc));
	return 1;
}

// MACINTOSH: service_type_0x35_chunk_objects()
// FUNCTION: LEMBALL 0x0040be20
void __fastcall ServiceType35ChunkObjects(void* pObject)
{
	int i;
	if (*(int*) ((char*) pObject + 0xc) > 0) {
		for (i = 0; i < *(int*) ((char*) pObject + 0xc); i++) {
			(*( void(**)(void)) (*(void***) (*(int*) ((char*) pObject + 4) + i * 0x150) + 0x14 / 4))();
		}
	}
}

// MACINTOSH: find_resource_geometry_helper_slot_by_resource_owner(int)
// FUNCTION: LEMBALL 0x0046bfa0
int __fastcall FindResourceGeometryHelperSlotByResourceOwner(void* pObject, int nUnused, int param_2)
{
	if (*(int*) ((char*) pObject + 0x10) > 0) {
		int i;
		for (i = 0; i < *(int*) ((char*) pObject + 0x10); i++) {
			if (*(int*) (*(int*) ((char*) pObject + 4) + 4 + i * 28) == param_2) {
				return i;
			}
		}
	}
	return -1;
}

// MACINTOSH: delete_fixed_buffer_formatted_output_stream(byte) [scalar-dtor]
// FUNCTION: LEMBALL 0x00407e80
void* __fastcall DeleteFixedBufferFormattedOutputStream(void* pThis, int nUnused, unsigned char param_1)
{
	char* pi = (char*) pThis - 0x20;
	*(int*) (*(int*) (*(int*) pi + 4) + ((int) pThis - 0x20)) = 0x493020;
	((void(__fastcall*)(void*)) 0x45adc0)((char*) pThis - 0x1c);
	((void(__fastcall*)(void*)) 0x4584a0)((char*) pThis + 0x14c);
	((void(__fastcall*)(void*)) 0x458440)(pThis);
	if ((param_1 & 1) != 0) {
		FreeVSMemBlock(pi);
	}
	return pi;
}

// MACINTOSH: delete_projectile_object_scalar_wrapper(byte) [scalar-dtor]
// FUNCTION: LEMBALL 0x0041af10
void* __fastcall DeleteProjectileObjectScalarWrapper(void* pThis, int nUnused, unsigned char param_1)
{
	void* pu = ((char*) pThis + 0x138);
	if (pThis == 0) {
		pu = 0;
	}
	((void(__fastcall*)(void*)) 0x45eea0)(pu);
	DestroyLevelChunkObjectBaseAutoThunk(pThis);
	if ((param_1 & 1) != 0) {
		FreeVSMemBlock(pThis);
	}
	return pThis;
}

// MACINTOSH: get_helper_upload_rect_entry_and_state(int, undefined4*)
// FUNCTION: LEMBALL 0x00466fd0
int __fastcall GetHelperUploadRectEntryAndState(void* pObject, int nUnused, int param_2, int* param_3)
{
	if (*(int*) ((char*) pObject + 4) == -1) {
		((void(__fastcall*)(void*)) 0x466ef0)(pObject);
	}
	*param_3 = *(int*) (*(int*) ((char*) pObject + 0xc) + 8 + param_2 * 0xc);
	return param_2 * 0xc + *(int*) ((char*) pObject + 0xc);
}

// MACINTOSH: is_point_inside_level_screen_action_panel(short*)
// FUNCTION: LEMBALL 0x00443360
int __fastcall IsPointInsideLevelScreenActionPanel(void* pObject, int nUnused, short* param_1)
{
	if (*(short*) ((char*) pObject + 0x54) <= param_1[0] &&
		param_1[0] < (*(short*) ((char*) pObject + 0x50) + *(short*) ((char*) pObject + 0x54)) &&
		*(short*) ((char*) pObject + 0x56) <= param_1[1] &&
		param_1[1] < (*(short*) ((char*) pObject + 0x56) + *(short*) ((char*) pObject + 0x52))) {
		return 1;
	}
	return 0;
}

// MACINTOSH: activate_shpg_chunk_object()
// FUNCTION: LEMBALL 0x0041fa00
void __fastcall ActivateShpgChunkObject(void* pObject)
{
	ResetManagedEntityRuntimeStateThunk(pObject);
	*(int*) ((char*) pObject + 0x9c) = *(int*) ((char*) pObject + 0x40);
	int iVar1 = *(int*) ((char*) pObject + 0x40) >> 12;
	*(int*) ((char*) pObject + 0xa0) = *(int*) ((char*) pObject + 0x44);
	int iVar3 = *(int*) ((char*) pObject + 0x44) >> 12;
	*(int*) ((char*) pObject + 0xa4) = *(int*) ((char*) pObject + 0x48);
	int local_18[6];
	local_18[2] = *(int*) ((char*) pObject + 0x48) >> 12;
	local_18[0] = iVar1 - 8;
	local_18[1] = iVar3 - 8;
	local_18[3] = iVar1 + 7;
	local_18[4] = iVar3 + 7;
	local_18[5] = local_18[2] + 15;
	{
		int i;
		for (i = 0; i < 6; i++) {
			*(int*) ((char*) pObject + 0x14 + i * 4) = local_18[i];
		}
	}
	*(unsigned short*) ((char*) pObject + 0xb4) = *(unsigned short*) ((char*) pObject + 0xb6);
	*(int*) (*(int*) ((char*) g_pActiveManagedEntityOwner + 0x120) + *(int*) ((char*) g_pActiveManagedEntityOwner + 0x118) * 4) = (int) pObject;
	*(int*) ((char*) g_pActiveManagedEntityOwner + 0x118) = *(int*) ((char*) g_pActiveManagedEntityOwner + 0x118) + 1;
}

// MACINTOSH: append_node_chunk_record_from_level_data(int, int)
// FUNCTION: LEMBALL 0x00421440
int __fastcall AppendNodeChunkRecordFromLevelData(void* pObject, int nUnused, int param_2, int param_3)
{
	int iVar1 = *(int*) ((char*) pObject + 4);
	*(int*) ((char*) pObject + 4) = iVar1 + 1;
	((void(__fastcall*)(void*, int, int, int)) 0x402572)((void*) (iVar1 * 0x14 + *(int*) pObject), param_2, param_3, 0);
	return iVar1;
}

// MACINTOSH: set_level_screen_pause_requested_state(int)
// FUNCTION: LEMBALL 0x00437da0
void __fastcall SetLevelScreenPauseRequestedState(void* pObject, int nUnused, int param_1)
{
	if (param_1 == 0) {
		((void(__fastcall*)(void*, int)) 0x4021a8)(pObject, 0);
	} else {
		int iVar1 = *(int*) (*(int*) ((char*) pObject + 0x96c) + 0x108);
		if (iVar1 >= 1 && iVar1 <= 2) {
			((void(__fastcall*)(void*, int)) 0x4013ed)(*(void**) ((char*) pObject + 0x96c), 1);
		}
	}
}

// MACINTOSH: reset_tile_flag_1c_object()
// FUNCTION: LEMBALL 0x004275f0
void __fastcall ResetTileFlag1cObject(void* pObject)
{
	ResetManagedEntityRuntimeStateThunk(pObject);
	*(unsigned short*) ((char*) pObject + 0xbc) = 0;
	*(int*) ((char*) pObject + 0x94) = 0;
	*(int*) ((char*) pObject + 0x13c) = 0;
	*(int*) ((char*) pObject + 0x138) = 0;
	*(int*) ((char*) pObject + 0xb8) = 0x18;
	((void(__fastcall*)(void*, void*)) 0x4023e2)(pObject, (char*) pObject + 0x40);
}

// MACINTOSH: sync_attached_move_chunk_entities_to_current_position()
// FUNCTION: LEMBALL 0x0042eac0
void __fastcall SyncAttachedMoveChunkEntitiesToCurrentPosition(void* pObject)
{
	int i;
	if (*(int*) ((char*) pObject + 0x174) > 0) {
		for (i = 0; i < *(int*) ((char*) pObject + 0x174); i++) {
			void* pElem = *(void**) ((char*) pObject + 0x178 + i * 4);
			((void(__fastcall*)(void*, void*)) 0x401d52)(pElem, (char*) pObject + 0x9c);
			((void(__fastcall*)(void*)) 0x40360c)(pElem);
		}
	}
}

// MACINTOSH: set_selector_manager_action_keyframes(int, int, int)
// FUNCTION: LEMBALL 0x0044da30
void __fastcall SetSelectorManagerActionKeyframes(void* pObject, int nUnused, int param_2, int param_3, int param_4)
{
	*(int*) ((char*) pObject + 0x88) = param_2;
	*(int*) ((char*) pObject + 0x8c) = param_3;
	*(int*) ((char*) pObject + 0x90) = param_3;
	*(int*) ((char*) pObject + 0x94) = param_4;
	*(int*) ((char*) pObject + 0x98) = param_2;
	*(int*) ((char*) pObject + 0x9c) = param_3;
	*(int*) ((char*) pObject + 0xa0) = param_4;
}

// MACINTOSH: release_list_entry_pair(int, int)
// FUNCTION: LEMBALL 0x0045da50
void __fastcall ReleaseListEntryPair(void* pObject, int nUnused, int param_2, int param_3)
{
	ReleaseTypedResourceObjectIfLoaded((void*) (*(int*) ((char*) pObject + 0x80) + param_2 * 0x4c), 0, param_3);
	ReleaseTypedResourceObjectIfLoaded((void*) (*(int*) ((char*) pObject + 0x7c) + param_2 * 0x54), 0, param_3);
}

// MACINTOSH: set_level_mode_profile_range_selector(int)
// FUNCTION: LEMBALL 0x004131b0
void __fastcall SetLevelModeProfileRangeSelector(void* pObject, int nUnused, int param_2)
{
	*(int*) ((char*) pObject + 0xd0) = param_2;
	((void(__fastcall*)(void*, int, int, int, int, int)) 0x401d25)(pObject, param_2, *(int*) ((char*) pObject + 0xc0), *(int*) ((char*) pObject + 0xc4), *(int*) ((char*) pObject + 0xc8), *(int*) ((char*) pObject + 0xcc));
}

// MACINTOSH: select_single_entity_and_dispatch_selection(int*)
// FUNCTION: LEMBALL 0x00438380
void __fastcall SelectSingleEntityAndDispatchSelection(void* pObject, int nUnused, void* param_1)
{
	if (*(int*) ((char*) param_1 + 0xb8) != 8) {
		*(unsigned short*) ((char*) pObject + 0xa4c) = 0;
		((void(__fastcall*)(void*, unsigned short, int)) 0x401474)(pObject, *(unsigned short*) ((char*) param_1 + 0x6a), 0);
		((void(__fastcall*)(void*)) 0x401776)(pObject);
		(*( void(**)(void)) (*(void***) param_1 + 0xd4 / 4))();
	}
}

// MACINTOSH: refresh_level_screen_action_panel()
// FUNCTION: LEMBALL 0x00443250
void __fastcall RefreshLevelScreenActionPanel(void* pObject)
{
	int i;
	for (i = 0; i < 4; i++) {
		((void(__fastcall*)(void*)) 0x4032bf)(*(void**) ((char*) pObject + 0x1c + i * 4));
	}
}

// MACINTOSH: try_apply_tram_chunk_overlap_impulse(int*, int*)
// FUNCTION: LEMBALL 0x004125c0
void __fastcall TryApplyTramChunkOverlapImpulse(void* pObject, int nUnused, void* param_1, void* param_2)
{
	((void(__fastcall*)(void*, void*, void*)) 0x402bd5)(*(void**) ((char*) pObject + 0x1b4), param_1, param_2);
}

// MACINTOSH: find_level_mode_move_chunk_containing_point(int, int, int*)
// FUNCTION: LEMBALL 0x004130d0
void __fastcall FindLevelModeMoveChunkContainingPoint(void* pObject, int nUnused, void* param_1, void* param_2, void* param_3)
{
	((void(__fastcall*)(void*, void*, void*, void*)) 0x402e55)(*(void**) ((char*) pObject + 0x1c0), param_1, param_2, param_3);
}

// MACINTOSH: construct_network_lobby_player_entry_child()
// FUNCTION: LEMBALL 0x00455da0
void* __fastcall ConstructNetworkLobbyPlayerEntryChild(void* pObject)
{
	((void(__fastcall*)(void*)) 0x46a300)(pObject);
	*(void**) pObject = (void**) 0x4986b0;
	((void(__fastcall*)(void*)) 0x4014dd)(pObject);
	return pObject;
}

// MACINTOSH: find_projectile_position_in_rect_from_level_mode(short*, undefined4*)
// FUNCTION: LEMBALL 0x004127e0
void __fastcall FindProjectilePositionInRectFromLevelMode(void* pObject, int nUnused, void* param_1, void* param_2)
{
	((void(__fastcall*)(void*, void*, void*)) 0x402680)(*(void**) ((char*) pObject + 0x168), param_1, param_2);
}

// MACINTOSH: release_level_screen_palette_remap_variants()
// FUNCTION: LEMBALL 0x00436480
void __fastcall ReleaseLevelScreenPaletteRemapVariants(void* pObject)
{
	int i;
	for (i = 0; i < 5; i++) {
		((PaletteRemapPointerTableMemberView*) *(void**) 0x4a2000)->ReleasePaletteRemapVariant((void*) *(int*) ((char*) pObject + 0x64 + i * 4));
	}
}

// MACINTOSH: select_pause_dialog_palette_remap_for_index(int)
// FUNCTION: LEMBALL 0x00444930
int __fastcall SelectPauseDialogPaletteRemapForIndex(void* pObject, int nUnused, int param_1)
{
	if (*(int*) ((char*) pObject + 0x118) == param_1) {
		return *(int*) ((char*) pObject + 0x1e8);
	}
	if (*(int*) ((char*) pObject + 0x124) <= param_1) {
		return *(int*) ((char*) pObject + 0x1e0);
	}
	if (param_1 <= *(int*) ((char*) pObject + 0x11c) && param_1 > 0) {
		return *(int*) ((char*) pObject + 0x1ec);
	}
	return *(int*) ((char*) pObject + 0x1e4);
}

// MACINTOSH: refresh_queued_zrle_variant_range_selection()
// FUNCTION: LEMBALL 0x0044f3d0
void __fastcall RefreshQueuedZrleVariantRangeSelection(void* pObject)
{
	if (*(void**) ((char*) pObject + 0x44) != 0) {
		*(int*) ((char*) pObject + 0x40) = **(int**) ((char*) pObject + 0x44);
	}
	((void(__cdecl*)(void*, int)) 0x4688e0)(*(void**) ((char*) pObject + 0x4c), *(int*) (*(int*) ((char*) pObject + 0x48) + (*(int*) ((char*) pObject + 0x40) - *(int*) ((char*) pObject + 0x3c)) * 4));
}

// MACINTOSH: copy_next_level_title_word_to_line(int, int, int*, int*)
// FUNCTION: LEMBALL 0x0044a2d0
int __cdecl CopyNextLevelTitleWordToLine(int param_1, int param_2, int* param_3, int* param_4)
{
	char cVar1 = *(char*) (*param_3 + param_1);
	while (cVar1 != '\0' && *(char*) (*param_3 + param_1) != ' ') {
		*(char*) (*param_4 + param_2) = *(char*) (*param_3 + param_1);
		*param_3 = *param_3 + 1;
		*param_4 = *param_4 + 1;
		cVar1 = *(char*) (*param_3 + param_1);
	}
	*(char*) (*param_4 + param_2) = 0;
	if (*(char*) (*param_3 + param_1) != '\0') {
		*param_3 = *param_3 + 1;
		return 0;
	}
	return 1;
}

// MACINTOSH: load_list_entry_pair_from_buffers(int, int*, int*)
// FUNCTION: LEMBALL 0x0045d990
int __fastcall LoadListEntryPairFromBuffers(void* pObject, int nUnused, int param_2, void* param_3, void* param_4)
{
	int iVar1 = ((int(__cdecl*)(void*, void*, void*, void*)) 0x45cfb0)((void*) (*(int*) ((char*) pObject + 0x80) + param_2 * 0x4c), param_3, param_4, pObject);
	if (iVar1 == 0) {
		iVar1 = ((int(__cdecl*)(void*, void*, void*, void*)) 0x45cfb0)((void*) (*(int*) ((char*) pObject + 0x7c) + param_2 * 0x54), param_3, param_4, pObject);
		if (iVar1 == 0) {
			return 0;
		}
	}
	return 1;
}

// MACINTOSH: delete_lobby_player_entries(uint)
// FUNCTION: LEMBALL 0x00455e10
void* __fastcall DeleteLobbyPlayerEntries(void* pThis, int nUnused, unsigned int param_1)
{
	if ((param_1 & 2) == 0) {
		*(void**) pThis = (void**) 0x496ea0;
		if ((param_1 & 1) != 0) {
			FreeVSMemBlock(pThis);
		}
		return pThis;
	}
	int iCount = *(int*) ((char*) pThis - 4);
	int i;
	for (i = iCount - 1; i >= 0; i--) {
		((void(__fastcall*)(void*)) 0x40338c)((char*) pThis + i * 0x44);
	}
	FreeVSMemBlock((char*) pThis - 4);
	return pThis;
}

// MACINTOSH: load_list_entry_pair_from_stream(int, int*)
// FUNCTION: LEMBALL 0x0045d9f0
int __fastcall LoadListEntryPairFromStream(void* pObject, int nUnused, int param_2, void* param_3)
{
	int iVar1 = ((int(__cdecl*)(void*, void*, void*)) 0x45cf70)((void*) (*(int*) ((char*) pObject + 0x80) + param_2 * 0x4c), param_3, pObject);
	if (iVar1 == 0) {
		iVar1 = ((int(__cdecl*)(void*, void*, void*)) 0x45cf70)((void*) (*(int*) ((char*) pObject + 0x7c) + param_2 * 0x54), param_3, pObject);
		if (iVar1 == 0) {
			return 0;
		}
	}
	return 1;
}
