// Cluster reconstruction supplemental TU (avoids append-corruption on the large ClusterScanOps.cpp).
#include "Visos/Generic/Memory.h"

extern void __fastcall AppendType18ChunkObject(void* pStream, int nUnused, unsigned short param_1, void* param_2, int param_3, int param_4);
extern void __fastcall DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void __fastcall ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void __fastcall ReleaseTypedResourceObjectIfLoaded(void* pObject, void* pUnusedEdx, int fReleaseMode);
extern void* g_pActiveManagedEntityOwner;
extern int g_nLevelFrameClockTick;
extern int g_nLevelFrameClockTimeMs;
extern void* g_pLevelDemoPlaybackController;
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

// MACINTOSH: append_move_chunk_object(ushort, ushort, int, int, int)
// FUNCTION: LEMBALL 0x0042f5e0
void __fastcall AppendMoveChunkObject(void* pObject, int nUnused, unsigned short param_1, unsigned short param_2, void* param_3, int param_4, void* param_5)
{
	if (*(int*) ((char*) pObject + 0x34) < *(int*) ((char*) pObject + 0x30)) {
		((void(__fastcall*)(void*, unsigned short, unsigned short, void*, int, void*)) 0x40128a)((void*) (*(int*) ((char*) pObject + 0x38) + *(int*) ((char*) pObject + 0x34) * 0x1a0), param_1, param_2, param_3, param_4, param_5);
		*(int*) ((char*) pObject + 0x34) = *(int*) ((char*) pObject + 0x34) + 1;
	}
}

// MACINTOSH: append_projectile_request_code(int, int)
// FUNCTION: LEMBALL 0x0040f9b0
int __fastcall AppendProjectileRequestCode(void* pObject, int nUnused, int param_1, int param_2)
{
	if (*(int*) ((char*) pObject + 0x220) == 0xc) {
		return 0;
	}
	if (((int(__fastcall*)(void*, int)) (*(void***) pObject)[0xb4 / 4])(pObject, param_1) != 0) {
		return 0;
	}
	*(int*) ((char*) pObject + *(int*) ((char*) pObject + 0x220) * 4 + 0x1c0) = param_1;
	*(int*) ((char*) pObject + *(int*) ((char*) pObject + 0x220) * 4 + 0x1f0) = param_2;
	*(int*) ((char*) pObject + 0x220) = *(int*) ((char*) pObject + 0x220) + 1;
	return 1;
}

// MACINTOSH: release_linked_gmob_chunk_object()
// FUNCTION: LEMBALL 0x0041ccc0
void __fastcall ReleaseLinkedGmobChunkObject(void* pObject)
{
	if (*(int*) ((char*) pObject + 0x14c) != 0xffff) {
		void* pvVar1 = *(void**) ((char*) pObject + 0x148);
		*(int*) ((char*) pObject + 0x9c) = *(int*) ((char*) pvVar1 + 0x9c);
		*(int*) ((char*) pObject + 0xa0) = *(int*) ((char*) pvVar1 + 0xa0);
		*(int*) ((char*) pObject + 0xa4) = *(int*) ((char*) pvVar1 + 0xa4);
		((void(__fastcall*)(void*, int, void*, int)) 0x402cac)(*(void**) 0x4a74c0, -1, pvVar1, 0);
		*(int*) ((char*) pObject + 0x14c) = 0xffff;
	}
}

// MACINTOSH: dispatch_projectile_packet_by_type(short, int)
// FUNCTION: LEMBALL 0x0041aca0
int __fastcall DispatchProjectilePacketByType(void* pObject, int nUnused, short param_1, void* param_2)
{
	if (param_1 != 0x2b) {
		return ((int(__fastcall*)(void*, short, void*)) 0x402342)(pObject, param_1, param_2);
	}
	int iVar2 = ((int(__fastcall*)(void*, void*)) 0x45f280)((void*) ((char*) pObject + 0x138), *(void**) ((char*) param_2 + 0x20));
	if (iVar2 != 0) {
		*(void**) ((char*) param_2 + 0x20) = *(void**) ((char*) pObject + 0x158);
	}
	((void(__fastcall*)(void*, void*)) 0x401299)(*(void**) ((char*) pObject + 0x60), pObject);
	((void(__fastcall*)(void*)) 0x40303a)(pObject);
	return 1;
}

// MACINTOSH: refresh_aligned_zrle_child_and_queue_if_dirty(int)
// FUNCTION: LEMBALL 0x00442a40
void __fastcall RefreshAlignedZrleChildAndQueueIfDirty(void* pObject, int nUnused, void* param_1)
{
	if (((int(__fastcall*)(void*)) (*(void***) pObject)[0xb4 / 4])(pObject) != *(int*) ((char*) pObject + 0x16c)) {
		*(int*) ((char*) pObject + 0xd8) = 1;
		*(int*) ((char*) pObject + 0x16c) = *(int*) ((char*) pObject + 0x16c) == 0;
	}
	if (*(int*) (*(int*) ((char*) pObject + 0x110) + 0x18) != *(int*) ((char*) pObject + 0x170)) {
		*(int*) ((char*) pObject + 0xd8) = 1;
		*(int*) ((char*) pObject + 0x170) = *(int*) (*(int*) ((char*) pObject + 0x110) + 0x18);
	}
	((void(__fastcall*)(void*, void*)) 0x468360)(pObject, param_1);
}

// MACINTOSH: sync_level_screen_action_panel_child_values()
// FUNCTION: LEMBALL 0x004431c0
void __fastcall SyncLevelScreenActionPanelChildValues(void* pObject)
{
	int i;
	for (i = 0; i < 4; i++) {
		void* pChild = *(void**) ((char*) pObject + 0x1c + i * 4);
		*(int*) ((char*) pChild + 8) = *(int*) (*(int*) ((char*) pObject + 0x18) + 0x1cc + i * 4 + 4);
	}
}

// MACINTOSH: remove_ball_chunk_entry_from_active_list(int)
// FUNCTION: LEMBALL 0x00421ff0
void __fastcall RemoveBallChunkEntryFromActiveList(void* pObject, int nUnused, int param_1)
{
	int n = *(int*) ((char*) pObject + 8);
	int i = 0;
	if (n > 0) {
		int* pIdx = *(int**) ((char*) pObject + 4);
		while (*pIdx != param_1) {
			pIdx++;
			i++;
			if (n <= i) {
				return;
			}
		}
		*(int*) ((char*) pObject + 8) = n - 1;
		if (i < n - 1) {
			int j;
			for (j = i; j < *(int*) ((char*) pObject + 8); j++) {
				*(int*) (*(int**) ((char*) pObject + 4) + j * 4) = *(int*) (*(int**) ((char*) pObject + 4) + j * 4 + 1);
			}
		}
		*(int*) (*(int**) ((char*) pObject + 4) + *(int*) ((char*) pObject + 8) * 4) = param_1;
		}
		}

// MACINTOSH: release_network_lobby_palette_remaps()
// FUNCTION: LEMBALL 0x00454b10
void __fastcall ReleaseNetworkLobbyPaletteRemaps(void* pObject)
{
	int i;
	for (i = 0; i < 6; i++) {
		((PaletteRemapPointerTableMemberView*) *(void**) 0x4a2000)->ReleasePaletteRemapVariant((void*) *(int*) ((char*) pObject + 0x414 + i * 4));
	}
}

// MACINTOSH: mark_network_lobby_player_entry_dirty(int)
// FUNCTION: LEMBALL 0x00454b40
void __fastcall MarkNetworkLobbyPlayerEntryDirty(void* pObject, int nUnused, int param_1)
{
	if (param_1 != -1 && param_1 < 10) {
		*(int*) (*(int*) ((char*) pObject + 0x3cc) + 0x40 + param_1 * 0x44) = 1;
	}
}

// MACINTOSH: pop_first_managed_entity_move_command_and_reset_motion()
// FUNCTION: LEMBALL 0x00415780
void __fastcall PopFirstManagedEntityMoveCommandAndResetMotion(void* pObject)
{
	((void(__fastcall*)(void*)) 0x402351)(pObject);
	*(int*) ((char*) pObject + 0x88) = 0;
	*(int*) ((char*) pObject + 0xcc) = g_nLevelFrameClockTick;
	*(int*) ((char*) pObject + 0xc8) = g_nLevelFrameClockTick;
}

// MACINTOSH: dispatch_zrle_list_entry(int, int)
// FUNCTION: LEMBALL 0x0045da90
void __fastcall DispatchZrleListEntry(void* pObject, int nUnused, int param_2, int param_3)
{
	(*( void(**)(int)) (*(void***) (*(int*) ((char*) pObject + 0x7c) + param_2 * 0x54) + 0x24 / 4))(param_3);
}

// MACINTOSH: initialize_level_screen_hotspot_entry()
// FUNCTION: LEMBALL 0x00439340
void* __fastcall InitializeLevelScreenHotspotEntry(void* pObject)
{
	*(unsigned short*) ((char*) pObject + 0x2c) = 0xffff;
	*(int*) ((char*) pObject + 0x38) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x3c) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x40) = 0xaa55aa55;
	*(int*) ((char*) pObject + 0x34) = 0;
	*(int*) ((char*) pObject + 0x30) = 0;
	*(int*) ((char*) pObject + 0x18) = 0;
	*(int*) ((char*) pObject + 0x20) = 0;
	return pObject;
}

// MACINTOSH: delete_timed_eff_stream_with_channel_state_wrapper(byte) [scalar-dtor]
// FUNCTION: LEMBALL 0x00462990
void* __fastcall DeleteTimedEffStreamWithChannelStateWrapper(void* pThis, int nUnused, char param_1)
{
	((void(__fastcall*)(void*)) 0x45fd80)(pThis);
	((void(__fastcall*)(void*)) 0x45f6c0)((char*) pThis + 0x78);
	if ((param_1 & 1) != 0) {
		FreeVSMemBlock(pThis);
	}
	return pThis;
}

// MACINTOSH: parse_gami_resource_descriptor()
// FUNCTION: LEMBALL 0x0045e1e0
void __fastcall ParseGamiResourceDescriptor(void* pObject)
{
	unsigned int* puVar2 = *(unsigned int**) ((char*) pObject + 0x34);
	unsigned short uVar1 = *(unsigned short*) (puVar2 + 1);
	*(short*) ((char*) pObject + 0x48) = (short) *puVar2;
	*(unsigned short*) ((char*) pObject + 0x4a) = uVar1;
	*(char*) ((char*) pObject + 0x4c) = *(char*) (puVar2 + 2);
	*(char*) ((char*) pObject + 0x4d) = *(char*) ((char*) puVar2 + 9);
	*(unsigned int*) ((char*) pObject + 0x50) = puVar2[3];
}

// MACINTOSH: construct_palette_remap_variant_wrapper(int, byte*, int)
// FUNCTION: LEMBALL 0x00473610
void* __fastcall ConstructPaletteRemapVariantWrapper(void* pThis, int nUnused, int param_1, unsigned char* param_2, int param_3)
{
	ConstructPaletteRemapVariant(pThis, nUnused, param_1, param_2, param_3);
	return pThis;
}

// MACINTOSH: refresh_resource_sprite_text_owner_layout()
// FUNCTION: LEMBALL 0x00469690
void __fastcall RefreshResourceSpriteTextOwnerLayout(void* pObject)
{
	if (*(int*) ((char*) pObject + 0x14c) != 0) {
		((void(__cdecl*)(void*, int)) 0x469210)(*(void**) ((char*) pObject + 0x154), 0);
		return;
	}
	((void(__cdecl*)(void*, int)) 0x469210)(*(void**) ((char*) pObject + 0x150), 0);
}

// MACINTOSH: restore_inertial_event_source_sink_vtables()
// FUNCTION: LEMBALL 0x00432650
void __fastcall RestoreInertialEventSourceSinkVtables(void* pObject)
{
	*(void**) ((char*) pObject + 0x60) = (void**) 0x496ca8;
	int i;
	for (i = 0; i < 2; i++) {
		((void(__fastcall*)(void*)) 0x401307)((char*) pObject + 0x34 - i * 0x10);
	}
}

// MACINTOSH: emit_variant_child_slot_render_marker(int, short*)
// FUNCTION: LEMBALL 0x004327e0
void __fastcall EmitVariantChildSlotRenderMarker(void* pObject, int nUnused, void* param_2, void* param_3)
{
	((void(__fastcall*)(void*, short, short, int, int, int, int)) 0x4016a9)(*(void**) ((char*) pObject + 0x10), *(short*) param_3, *(short*) ((char*) param_3 + 2), 0xfd, 0, 0, 0);
}

// MACINTOSH: configure_level_mode_managed_entity_profiles(int, int, int, int, int)
// FUNCTION: LEMBALL 0x00413130
void __fastcall ConfigureLevelModeManagedEntityProfiles(void* pObject, int nUnused, int param_2, int param_3, int param_4, int param_5, int param_6)
{
	((void(__fastcall*)(void*, int, int, int, int, int)) 0x402720)(*(void**) ((char*) pObject + 0x15c), param_2, param_3, param_4, param_5, param_6);
}

// MACINTOSH: dispatch_managed_entity_chunk_contacts_from_current_tile()
// FUNCTION: LEMBALL 0x00416820
void __fastcall DispatchManagedEntityChunkContactsFromCurrentTile(void* param_1)
{
	*(int*) ((char*) param_1 + 0xcc) = g_nLevelFrameClockTick + 8;
	((void(__fastcall*)(void*, void*, void*, unsigned short)) 0x40341d)(g_pActiveManagedEntityOwner, (char*) param_1 + 0x9c, param_1, *(unsigned short*) ((char*) param_1 + 0x68));
}

// MACINTOSH: refresh_level_selection_navigation_locks()
// FUNCTION: LEMBALL 0x0044a3e0
void __fastcall RefreshLevelSelectionNavigationLocks(void* param_1)
{
	int bVar1 = ((int(__fastcall*)(void*)) 0x402793)(*(void**) 0x49cb68);
	int bVar2 = ((int(__fastcall*)(void*)) 0x401f82)(*(void**) 0x49cb68);
	*(int*) ((char*) param_1 + 0x480) = 0;
	if (bVar1 != 1) {
		*(int*) ((char*) param_1 + 0x480) = 1;
	}
	*(int*) ((char*) param_1 + 0x484) = 0;
	if (bVar2 != 1) {
		*(int*) ((char*) param_1 + 0x484) = 1;
	}
	((void(__fastcall*)(void*, int)) 0x40329c)(*(void**) ((char*) param_1 + 0x338), 0xacef000e);
	((void(__fastcall*)(void*, int)) 0x40329c)(*(void**) ((char*) param_1 + 0x338), 0xacef000f);
}

// MACINTOSH: remove_pointer_from_level_mode_list_0x120(int)
// FUNCTION: LEMBALL 0x00410a20
void __fastcall RemovePointerFromLevelModeList0x120(void* param_1)
{
	int* piVar2 = (int*) ((char*) g_pActiveManagedEntityOwner + 0x118);
	int iVar6 = 0;
	int iVar4 = *piVar2;
	if (iVar4 > 0) {
		int* piVar3 = *(int**) ((char*) g_pActiveManagedEntityOwner + 0x120);
		while (*piVar3 != (int) param_1) {
			piVar3++;
			iVar6++;
			if (iVar4 <= iVar6) {
				return;
			}
		}
		*piVar2 = iVar4 - 1;
		if (iVar6 < iVar4 - 1) {
			int j;
			for (j = iVar6; j < *piVar2; j++) {
				*(int*) (*(int**) ((char*) g_pActiveManagedEntityOwner + 0x120) + j * 4) = *(int*) (*(int**) ((char*) g_pActiveManagedEntityOwner + 0x120) + j * 4 + 1);
			}
		}
		*(int*) (*(int**) ((char*) g_pActiveManagedEntityOwner + 0x120) + *piVar2 * 4) = 0;
		}
		}

// MACINTOSH: dispatch_level_screen_projectile_request_by_index(int)
// FUNCTION: LEMBALL 0x00438330
void __fastcall DispatchLevelScreenProjectileRequestByIndex(void* pObject, int nUnused, int param_1)
{
	void* pHolder = (void*) (*(int*) ((char*) pObject + 0x96c) + 0x1d0 + param_1 * 4);
	int iVar2 = ((int(__fastcall*)(void*)) 0x4015d2)(*(void**) pHolder);
	if (iVar2 != 0xffff && *(int*) (*(int**) pHolder + 0xb8) != 8) {
		(*( void(**)(int)) (*(void***) *(int**) pHolder + 0x34 / 4))(0x1f);
		SelectSingleEntityAndDispatchSelection(pObject, nUnused, *(void**) pHolder);
	}
}

// MACINTOSH: refresh_viewport_anchor()
// FUNCTION: LEMBALL 0x004381c0
void __fastcall RefreshViewportAnchor(void* pObject, int nUnused, void* param_2)
{
	*(int*) ((char*) pObject + 0x178) = 0;
	if (*(int*) ((char*) pObject + 0xa68) == 0) {
		if ((g_pLevelDemoPlaybackController == 0 || *(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) == 0) && ((int(__fastcall*)(void*)) (*(void***) *(void**) ((char*) pObject + 0x964) + 0x60 / 4))(*(void**) ((char*) pObject + 0x964)) == 0) {
			return;
		}
		((void(__fastcall*)(void*)) 0x4019ec)((char*) pObject - 0x14);
	}
}

// MACINTOSH: load_ball_chunk_entries(ushort*)
// FUNCTION: LEMBALL 0x00422060
void __fastcall LoadBallChunkEntries(void* pObject, int nUnused, unsigned short* param_1)
{
	unsigned short uVar1 = *param_1;
	param_1 = param_1 + 1;
	unsigned int uVar3 = (unsigned int) uVar1;
	((void(__fastcall*)(void*, unsigned int)) 0x4032f6)(pObject, uVar3);
	*(unsigned int*) ((char*) pObject + 8) = uVar3;
	if (uVar3 != 0) {
		unsigned int iVar4 = 0;
		int iVar2 = 0;
		do {
			iVar4 = iVar4 + 4;
			iVar2++;
			((void(__fastcall*)(void*, unsigned short**)) 0x403030)(*(void**) (*(int*) ((char*) pObject + 4) + iVar4 - 4), &param_1);
		} while (iVar2 < *(int*) ((char*) pObject + 8));
	}
}

// MACINTOSH: destroy_managed_entity_pointer_array()
// FUNCTION: LEMBALL 0x00422510
void __fastcall DestroyManagedEntityPointerArray(void* pObject)
{
	int i;
	for (i = 0; i < *(int*) ((char*) pObject + 0x3c); i++) {
		void* pElem = *(void**) (*(int*) ((char*) pObject + 0x34) + i * 4);
		if (pElem != 0) {
			(*( void(**)(int)) **(void***) pElem)(1);
		}
	}
	*(int*) ((char*) pObject + 0x3c) = 0;
}

// MACINTOSH: emit_plas_chunk_manager_render_entries(int)
// FUNCTION: LEMBALL 0x00419490
int __fastcall EmitPlasChunkManagerRenderEntries(void* pObject, int nUnused, int param_1)
{
	int local_4 = 0;
	if (*(int*) 0x4a011c != 0) {
		int iVar3 = 4;
		local_4 = 4;
		int i;
		for (i = 0; i < 4; i++) {
			void* pElem = *(void**) ((char*) pObject + 0x13c + i * 4);
			(*( void(**)(int)) (*(void***) pElem + 0xc / 4))(param_1);
			param_1 += 0x4c;
		}
	}
	int iVar2 = ((int(__fastcall*)(void*, int)) 0x401f64)(pObject, param_1);
	return iVar2 + local_4;
}

// MACINTOSH: start_level_game_mode_session()
// FUNCTION: LEMBALL 0x00411b10
void __fastcall StartLevelGameModeSession(void* param_1)
{
	if (*(int*) ((char*) param_1 + 0x64) != 0) {
		*(int*) ((char*) param_1 + 0x6c) = 1;
		*(int*) ((char*) param_1 + 0x70) = 0;
		void* pRuntime = *(void**) 0x4a0120;
		*(int*) ((char*) pRuntime + 0x48) = 3;
		*(int*) ((char*) pRuntime + 0x4c) = 0;
		return;
	}
	if (g_pLevelDemoPlaybackController != 0 && *(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) != 0) {
		*(int*) ((char*) g_pLevelDemoPlaybackController + 0x38) = (int) ((unsigned(__fastcall*)()) 0x462e80)();
		*(int*) ((char*) g_pLevelDemoPlaybackController + 0x3c) = 0;
	}
	((void(__fastcall*)(void*, int)) 0x4013ed)(param_1, 2);
	*(int*) ((char*) param_1 + 0x68) = 1;
}

// MACINTOSH: is_managed_entity_within_followup_activation_range(int, int, int, int, int, int)
// FUNCTION: LEMBALL 0x004140d0
int __cdecl IsManagedEntityWithinFollowupActivationRange(int param_1, int param_2, int param_3, int param_4, int param_5, int param_6)
{
	int iVar3 = (param_1 >> 12) - (param_4 >> 12);
	int iVar2 = (param_2 >> 12) - (param_5 >> 12);
	int iVar1 = (param_3 >> 12) - (param_6 >> 12);
	if (iVar1 < 0) {
		iVar1 = -iVar1;
	}
	if (iVar1 < 0x11 && iVar2 * iVar2 + iVar3 * iVar3 < 100) {
		return 1;
	}
	return 0;
}

// MACINTOSH: pop_move_command_and_copy_next_position(undefined4*)
// FUNCTION: LEMBALL 0x00416050
void* __fastcall PopMoveCommandAndCopyNextPosition(void* param_1, int nUnused, void* param_2)
{
	unsigned short* puVar2 = *(unsigned short**) ((char*) param_1 + 0x70);
	unsigned short uVar1 = *puVar2;
	int iVar4 = 0;
	if (uVar1 != 0) {
		int iVar5 = 0;
		if (uVar1 != 1 && (int) (uVar1 - 1) >= 0) {
			do {
				iVar5++;
				void* puVar3 = (void*) (*(int*) (puVar2 + 2) + iVar4);
				iVar4 += 0x14;
				*(int*) puVar3 = *(int*) ((char*) puVar3 + 0x14);
				*(int*) ((char*) puVar3 + 4) = *(int*) ((char*) puVar3 + 0x18);
				*(int*) ((char*) puVar3 + 8) = *(int*) ((char*) puVar3 + 0x1c);
				*(int*) ((char*) puVar3 + 0xc) = *(int*) ((char*) puVar3 + 0x20);
				*(unsigned short*) ((char*) puVar3 + 0x10) = *(unsigned short*) ((char*) puVar3 + 0x24);
			} while (iVar5 < (int) (*puVar2 - 1));
		}
		*puVar2 = *puVar2 - 1;
	}
	((void(__cdecl*)(void*, void*)) 0x40336e)(param_1, param_2);
	return param_2;
}

// MACINTOSH: set_managed_entity_ice_attachment(int, int, int)
// FUNCTION: LEMBALL 0x00410250
void __fastcall SetManagedEntityIceAttachment(void* pThis, int nUnused, int param_1, void* param_2, int param_3)
{
	if (param_1 == 0 && *(int*) ((char*) pThis + 0x188) != 0 && param_3 != 0) {
		((void(__fastcall*)(void*, void*)) 0x402c16)(*(void**) ((char*) pThis + 0x18c), pThis);
	}
	*(int*) ((char*) pThis + 0x188) = param_1;
	*(void**) ((char*) pThis + 0x18c) = param_2;
	if (param_1 != 0) {
		int iVar1 = ((int(__fastcall*)(void*)) (*(void***) *(void**) ((char*) pThis + 0x224) + 0x108 / 4))(*(void**) ((char*) pThis + 0x224));
		if (iVar1 > 1) {
			(*( void(**)(int, void*)) (*(void***) *(void**) ((char*) g_pActiveManagedEntityOwner + 0x15c) + 0x34 / 4))(1, (char*) pThis + 0x6a);
		}
		(*( void(**)(void*)) (*(void***) *(void**) ((char*) pThis + 0x224) + 0x150 / 4))(*(void**) ((char*) pThis + 0x224));
	}
}

// MACINTOSH: set_text_selection_range_state(int)
// FUNCTION: LEMBALL 0x004743e0
void __fastcall SetTextSelectionRangeState(void* pThis, int nUnused, int param_1)
{
	(*( void(**)(void*)) (*(void***) pThis + 0 / 4))(pThis);
	int iVar1 = *(int*) ((char*) pThis + 0x50);
	if (iVar1 <= *(int*) ((char*) pThis + 0x54)) {
		int iVar3 = iVar1 * 0xc;
		int iVar2;
		do {
			iVar3 += 0xc;
			iVar2 = iVar1 + 1;
			*(int*) (*(int*) (*(int*) ((char*) pThis + 0x24) + 4) + iVar3 - 4) = param_1;
			((void(__fastcall*)(void*, int, int)) 0x473f80)(pThis, iVar1, 1);
			iVar1 = iVar2;
		} while (iVar2 <= *(int*) ((char*) pThis + 0x54));
	}
	(*( void(**)(void*)) (*(void***) pThis + 4 / 4))(pThis);
}

// MACINTOSH: activate_or_queue_managed_entity_for_level_entity_id(uint)
// FUNCTION: LEMBALL 0x00418ab0
void __fastcall ActivateOrQueueManagedEntityForLevelEntityId(void* pOwner, int nUnused, unsigned int param_1)
{
	void* this_00 = (void*) ((int(__fastcall*)(void*)) 0x4021df)(pOwner);
	if (this_00 != 0) {
		void* pvVar1 = (void*) (*(void**) (0x4a6510 + (param_1 & 0xffff) * 4));
		if (*(int*) ((char*) pvVar1 + 0x64) != 2) {
			((void(__fastcall*)(void*, unsigned int)) 0x401c62)(this_00, param_1);
			return;
		}
		if (*(int*) ((char*) pvVar1 + 0xb8) != 8) {
			void* this_01 = (void*) ((int(__fastcall*)(void*)) 0x4037ba)(pvVar1);
			if (this_01 != this_00) {
				((void(__fastcall*)(void*, int, void*)) 0x4014bf)(this_00, 0, 0);
			}
			((void(__fastcall*)(void*, int, void*)) 0x4014bf)(this_01, 1, pvVar1);
		}
	}
}

// MACINTOSH: set_active_managed_entity_children_door_target_tile(int, int)
// FUNCTION: LEMBALL 0x00418b60
void __fastcall SetActiveManagedEntityChildrenDoorTargetTile(void* pObject, int nUnused, int param_1, int param_2)
{
	void* pEntity = (void*) ((int(__fastcall*)(void*)) 0x4021df)(pObject);
	if (pEntity != 0) {
		void* pChild = (void*) ((int(__fastcall*)(void*)) 0x40241e)(pEntity);
		while (pChild != 0) {
			((void(__fastcall*)(void*, int, int)) 0x401659)(pChild, param_1, param_2);
			pChild = (void*) ((int(__fastcall*)(void*)) 0x401816)(pEntity);
		}
	}
}

// MACINTOSH: reset_then_forward_managed_entity_vslot_0x60()
// FUNCTION: LEMBALL 0x00419ed0
void __cdecl ResetThenForwardManagedEntityVslot0x60(int param_1, int* param_2)
{
	int iVar1 = *param_2;
	int iVar2 = (*( int(**)(void)) (*(void***) iVar1 + 0xe4 / 4))();
	if (iVar2 != 0) {
		(*( void(**)(int, int, int)) (*(void***) iVar1 + 0xe8 / 4))(0, 0, 1);
	}
	(*( void(**)(void)) (*(void***) iVar1 + 0x60 / 4))();
}

// MACINTOSH: ensure_list_glyph_lookup_table()
// FUNCTION: LEMBALL 0x0045daf0
void __fastcall EnsureListGlyphLookupTable(void* pObject)
{
	if (*(int*) ((char*) pObject + 0x78) == 0) {
		void* pThis = (void*) ((void*(__cdecl*)(int)) 0x45baf0)(8);
		if (pThis != 0) {
			*(void**) ((char*) pObject + 0x78) = (void*) ((void*(__fastcall*)(void*, void*)) 0x473650)(pThis, pObject);
			return;
		}
		*(int*) ((char*) pObject + 0x78) = 0;
	}
}

// MACINTOSH: cancel_network_lobby_prompt()
// FUNCTION: LEMBALL 0x00454620
void __fastcall CancelNetworkLobbyPrompt(void* param_1)
{
	void* pController = *(void**) 0x49f140;
	if (*(int*) ((char*) pController + 0x2c) != 0 && *(int*) ((char*) pController + 0x28) == 0) {
		((void(__fastcall*)(void*)) 0x4013a7)(pController);
	}
	((void(__fastcall*)(void*, int)) 0x4012da)(param_1, 0);
	*(int*) ((char*) param_1 + 0x39c) = 0;
}

// MACINTOSH: set_dual_child_overlay_positions(short, short)
// FUNCTION: LEMBALL 0x0044c5b0
void __fastcall SetDualChildOverlayPositions(void* pThis, int nUnused, short param_2, short param_3)
{
	unsigned short pos[2];
	pos[0] = (unsigned short) param_2;
	pos[1] = (unsigned short) param_3;
	if (*(int**) ((char*) pThis + 0x50) != 0) {
		(*( void(**)(void*)) (*(void***) *(void**) ((char*) pThis + 0x50) + 0x38 / 4))(pos);
	}
	if (*(int**) ((char*) pThis + 0x54) != 0) {
		(*( void(**)(void*)) (*(void***) *(void**) ((char*) pThis + 0x54) + 0x38 / 4))(pos);
	}
}

// MACINTOSH: coalesce_free_memory_block_with_trailing_neighbor(int)
// FUNCTION: LEMBALL 0x00459b90
int __fastcall CoalesceFreeMemoryBlockWithTrailingNeighbor(void* pThis, int nUnused, void* param_2)
{
	void* piVar1 = *(void**) ((char*) param_2 + 0x14);
	if (((int(__fastcall*)(void*, void*)) 0x45a0e0)(pThis, piVar1) != 0) {
		if (((int(__fastcall*)(void*, void*)) 0x45a230)(pThis, piVar1) != 0) {
			((void(__fastcall*)(void*, void*, void*)) 0x459b10)(pThis, param_2, piVar1);
		}
	}
	return 0;
}

// MACINTOSH: service_composite_children_and_seek_target()
// FUNCTION: LEMBALL 0x0041f820
int __fastcall ServiceCompositeChildrenAndSeekTarget(void* pObject)
{
	int iVar3 = 0;
	(*( void(**)(int)) (*(void***) pObject + 0x138 / 4))(0x18);
	if (*(int*) ((char*) pObject + 0x124) > 0) {
		int i;
		for (i = 0; i < *(int*) ((char*) pObject + 0x124); i++) {
			void* piVar1 = *(void**) ((char*) pObject + 0x128 + i * 4);
			(*( void(**)(void)) (*(void***) piVar1 + 0x14 / 4))();
		}
	}
	((void(__fastcall*)(void*)) 0x403765)(pObject);
	((void(__fastcall*)(void*)) 0x402577)(pObject);
	return 0;
}

// MACINTOSH: activate_embedded_media_and_restore_music()
// FUNCTION: LEMBALL 0x00450d30
void __fastcall ActivateEmbeddedMediaAndRestoreMusic(void* param_1)
{
	if (*(int*) 0x4a6284 == 0) {
		((void(__fastcall*)(void*)) 0x46e300)(param_1);
	}
	if (*(int*) ((char*) param_1 + 0xd4) == 0) {
		((void(__fastcall*)(void*, int)) 0x401762)(*(void**) 0x49eb80, 1);
		*(int*) ((char*) param_1 + 0xd4) = 1;
	}
}

// MACINTOSH: award_150_points_and_emit_object_callback()
// FUNCTION: LEMBALL 0x0041fad0
void __fastcall Award150PointsAndEmitObjectCallback(void* pObject)
{
	((void(__fastcall*)(void*, int)) 0x402f22)(g_pActiveManagedEntityOwner, 0x96);
	int vec[3];
	vec[0] = 0;
	vec[1] = 0;
	vec[2] = 0xa000;
	(*( void(**)(void*, int)) (*(void***) pObject + 0x20 / 4))(vec, 0);
}

// MACINTOSH: award_300_points_mark_flag_and_emit_callback()
// FUNCTION: LEMBALL 0x004206a0
void __fastcall Award300PointsMarkFlagAndEmitCallback(void* pObject)
{
	*(int*) ((char*) pObject + 0x10c) = 1;
	((void(__fastcall*)(void*, int)) 0x402f22)(g_pActiveManagedEntityOwner, 300);
	int vec[3];
	vec[0] = 0;
	vec[1] = 0;
	vec[2] = 0xa000;
	(*( void(**)(void*, int)) (*(void***) pObject + 0x20 / 4))(vec, 0);
	*(int*) ((char*) pObject + 0x2c) = 1;
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

// MACINTOSH: append_type_0x18_object_with_free_slot(undefined4*, int)
// FUNCTION: LEMBALL 0x00412e80
void __fastcall AppendType18ObjectWithFreeSlot(void* pObject, int nUnused, void* param_2, int param_3)
{
	int nSlot = ((int(__fastcall*)()) 0x40227a)();
	AppendType18ChunkObject(*(void**) ((char*) pObject + 0x1c4), nUnused, (unsigned short) nSlot, param_2, 0, param_3);
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

// MACINTOSH: route_level_chunk_stream_payload(int)
// FUNCTION: LEMBALL 0x0040b290
int __fastcall RouteLevelChunkStreamPayload(void* pObject, int nUnused, void* param_1)
{
	unsigned short uVar1 = *(unsigned short*) (*(int*) ((char*) param_1 + 4) + 8);
	if (uVar1 < 0xb) {
		return 0;
	}
	((void(__cdecl*)(void*, void*)) 0x45f280)(*(void**) (*(int*) ((char*) pObject + 0x10) + *(int*) (*(int*) ((char*) pObject + 0x1c) + (unsigned int) uVar1 * 4 - 0x2c) * 4), (void*) (*(int*) ((char*) param_1 + 4) + 0x10));
	*(int*) ((char*) param_1 + 0x24) = 0;
	return 1;
}

// MACINTOSH: set_type18_chunk_object_position_by_index(int, int, int, int)
// FUNCTION: LEMBALL 0x0040ca10
void __fastcall SetType18ChunkObjectPositionByIndex(void* pObject, int nUnused, int param_2, int param_3, int param_4, int param_5)
{
	if (param_5 < *(int*) ((char*) pObject + 0x50)) {
		((void(__cdecl*)(void*, int, int, int)) 0x401cdf)(*(void**) ((char*) pObject + 0x30 + param_5 * 4), param_2, param_3, param_4);
	}
}

// MACINTOSH: activate_door_chunk_object_trigger()
// FUNCTION: LEMBALL 0x0040dec0
void __fastcall ActivateDoorChunkObjectTrigger(void* pObject)
{
	*(int*) ((char*) pObject + 0x144) = 1;
	*(int*) ((char*) pObject + 0x94) = g_nLevelFrameClockTimeMs;
	*(int*) ((char*) pObject + 0xcc) = *(int*) ((char*) pObject + 0xcc) + g_nLevelFrameClockTick;
	if (*(int*) ((char*) pObject + 0xb8) != 0x1c) {
		unsigned short uVar1 = *(unsigned short*) ((char*) pObject + 0xbc);
		int iVar2;
		if (uVar1 == 0x14) {
			iVar2 = 0x19;
		} else if (uVar1 < 0x15 || uVar1 > 0x17) {
			iVar2 = 0x19;
		} else {
			iVar2 = 0x4b;
		}
		((void(__fastcall*)(void*, int)) 0x402f22)(g_pActiveManagedEntityOwner, iVar2);
	}
}

// MACINTOSH: append_rock_chunk_object(ushort, int, int, int)
// FUNCTION: LEMBALL 0x004270b0
void __fastcall AppendRockChunkObject(void* pObject, int nUnused, unsigned short param_1, int param_2, int param_3, int param_4)
{
	if (*(int*) ((char*) pObject + 0x34) < *(int*) ((char*) pObject + 0x30)) {
		int vec[3];
		vec[0] = param_2 << 12;
		vec[1] = param_3 << 12;
		vec[2] = param_4 << 12;
		((void(__cdecl*)(void*, unsigned short, void*)) 0x402707)((void*) (*(int*) ((char*) pObject + 0x38) + *(int*) ((char*) pObject + 0x34) * 0x144), param_1, vec);
		*(int*) ((char*) pObject + 0x34) = *(int*) ((char*) pObject + 0x34) + 1;
	}
}

// MACINTOSH: set_managed_entity_state_id_with_timestamp(int)
// FUNCTION: LEMBALL 0x00410220
void __fastcall SetManagedEntityStateIdWithTimestamp(void* pObject, int nUnused, int param_1)
{
	*(int*) ((char*) pObject + 0x94) = g_nLevelFrameClockTimeMs;
	if (param_1 == 8) {
		((void(__fastcall*)(void*, int)) 0x402cfc)(pObject, 8);
		return;
	}
	*(int*) ((char*) pObject + 0xb8) = param_1;
}

// MACINTOSH: activate_first_pending_tram_chunk_object_at_nearby_point(int*, int)
// FUNCTION: LEMBALL 0x0042b440
int __fastcall ActivateFirstPendingTramChunkObjectAtNearbyPoint(void* pObject, int nUnused, void* param_2, int param_3)
{
	int i;
	for (i = 0; i < *(int*) ((char*) pObject + 0x34); i++) {
		void* pElem = (void*) (*(int*) ((char*) pObject + 0x38) + i * 0x144);
		if (*(int*) ((char*) pElem + 0x13c) != 0 && *(int*) ((char*) pElem + 0x138) == 0) {
			if (((int(__cdecl*)(void*, void*, int)) 0x403116)(pElem, param_2, param_3) != 0) {
				return 1;
			}
		}
	}
	return 0;
}

// MACINTOSH: deactivate_boon_chunk_object_in_manager(int)
// FUNCTION: LEMBALL 0x0042a460
void __fastcall DeactivateBoonChunkObjectInManager(void* param_1, int nUnused, int param_2)
{
	unsigned short uVar1;
	int iVar2;
	switch (*(int*) ((char*) param_2 + 0x64)) {
	case 0x28: uVar1 = 0xfffe; iVar2 = 0; break;
	case 0x2a: uVar1 = 0xfffd; iVar2 = 1; break;
	case 0x2c: uVar1 = 0xfffb; iVar2 = 2; break;
	case 0x2e: uVar1 = 0xfff7; iVar2 = 3; break;
	default: return;
	}
	*(unsigned short*) param_1 = *(unsigned short*) param_1 & uVar1;
	*(int*) (*(int*) ((char*) param_1 + iVar2 * 2 + 0x1a) + 0x124) = 0;
}

// MACINTOSH: serialize_lift_chunk_start_points(undefined4*)
// FUNCTION: LEMBALL 0x004257e0
int __fastcall SerializeLiftChunkStartPoints(void* pObject, int nUnused, void* param_1)
{
	int i;
	if (*(int*) ((char*) pObject + 0x34) > 0) {
		for (i = 0; i < *(int*) ((char*) pObject + 0x34); i++) {
			void* pElem = (void*) (*(int*) ((char*) pObject + 0x3c) + i * 0x190);
			*(int*) param_1 = *(int*) ((char*) pElem + 0x13a);
			*(unsigned short*) ((char*) param_1 + 4) = *(unsigned short*) ((char*) pElem + 0x13e);
			param_1 = (char*) param_1 + 6;
		}
	}
	return *(int*) ((char*) pObject + 0x34);
}

// MACINTOSH: dispatch_rock_chunk_contacts_for_entity(int*, int)
// FUNCTION: LEMBALL 0x00426fb0
int __fastcall DispatchRockChunkContactsForEntity(void* pObject, int nUnused, void* param_1, int param_2)
{
	int i;
	for (i = 0; i < *(int*) ((char*) pObject + 0x34); i++) {
		void* pElem = (void*) (*(int*) ((char*) pObject + 0x38) + i * 0x144);
		if (*(int*) ((char*) pElem + 0x13c) != 0 && *(int*) ((char*) pElem + 0xb8) == 0x18 && *(int*) ((char*) pElem + 0x12c) == 0x18) {
			if (((int(__cdecl*)(void*, void*, int)) 0x401375)(pElem, param_1, param_2) != 0) {
				return 1;
			}
		}
	}
	return 0;
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
