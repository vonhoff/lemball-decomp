// Cluster reconstruction: clean scan/array operations appended to a fresh TU
// (avoids disturbing existing translation units' formatting).
#include "Visos/Generic/Memory.h"

extern int __fastcall FindNetworkLobbyPeerSlotByPeer(void* pObject, int nUnused, int param_1);
extern void __fastcall ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void __fastcall DestroyLevelChunkObjectBaseAutoThunk(void* pObject);

// MACINTOSH: append_static_anim_chunk_record_if_unique(short*)
// FUNCTION: LEMBALL 0x0040d0c0
void __fastcall AppendStaticAnimChunkRecordIfUnique(void* pObject, int nUnused, short* param_1)
{
	int i;
	if (((int(__fastcall*)(void*, short*)) 0x40291e)(pObject, param_1) != 0) {
		return;
	}
	if (*(int*) ((char*) pObject + 4) >= 200) {
		return;
	}
	*(int*) ((char*) pObject + *(int*) ((char*) pObject + 4) * 0x18 + 0x1c) = 0;
	*(unsigned short*) ((char*) pObject + *(int*) ((char*) pObject + 4) * 0x18 + 0x1a) = 0;
	*(int*) ((char*) pObject + *(int*) ((char*) pObject + 4) * 0x18 + 8) = *(int*) param_1;
	*(short*) ((char*) pObject + *(int*) ((char*) pObject + 4) * 0x18 + 0xc) = param_1[2];
	*(int*) ((char*) pObject + 4) = *(int*) ((char*) pObject + 4) + 1;
	if (*(int*) ((char*) pObject + 4) > 0) {
		for (i = 0; i < *(int*) ((char*) pObject + 4); i++) {
			*(unsigned short*) ((char*) pObject + 0x14 + i * 0x18) = *(unsigned short*) ((char*) pObject + 0x14 + i * 0x18 + 2);
		}
	}
}

// MACINTOSH: get_last_queued_projectile_request_code()
// FUNCTION: LEMBALL 0x0040fbe0
int __fastcall GetLastQueuedProjectileRequestCode(void* pObject)
{
	int i;
	int count = *(int*) ((char*) pObject + 0x220);
	if (count == 0) {
		return 0xffff;
	}
	for (i = count - 1; i >= 0; i--) {
		switch (*(int*) ((char*) pObject + 0x1c0 + i * 4)) {
		case 0x27: return 0x27;
		case 0x29: return 0x29;
		case 0x2b: return 0x2b;
		case 0x2d: return 0x2d;
		}
	}
	return 0xffff;
}

// MACINTOSH: remove_queued_projectile_request_code(int)
// FUNCTION: LEMBALL 0x0040fc50
void __fastcall RemoveQueuedProjectileRequestCode(void* pObject, int nUnused, int param_1)
{
	int iVar1 = *(int*) ((char*) pObject + 0x220);
	int iVar4 = 0;
	if (iVar1 > 0) {
		int* piVar3 = (int*) ((char*) pObject + 0x1c0);
		while (piVar3[0] != param_1) {
			piVar3++;
			iVar4++;
			if (iVar1 <= iVar4) {
				return;
			}
		}
		iVar4++;
		if (iVar4 < iVar1) {
			do {
				*(int*) ((char*) pObject + iVar4 * 4 + 0x1bc) = *(int*) ((char*) pObject + (iVar4 + 1) * 4 + 0x1bc);
				iVar4++;
			} while (iVar4 < *(int*) ((char*) pObject + 0x220));
		}
		*(int*) ((char*) pObject + 0x220) = *(int*) ((char*) pObject + 0x220) - 1;
	}
}

// MACINTOSH: destroy_managed_entity_group(byte) [scalar-dtor]
// FUNCTION: LEMBALL 0x00419930
void* __fastcall DestroyManagedEntityGroup(void* pThis, int nUnused, unsigned char param_1)
{
	((void(__fastcall*)(void*)) 0x402441)(pThis);
	if ((param_1 & 1) != 0) {
		FreeVSMemBlock((void*) ((char*) pThis - 0xb0));
	}
	return (void*) ((char*) pThis - 0xb0);
}

// MACINTOSH: get_network_lobby_peer_entry_stream_by_peer(int)
// FUNCTION: LEMBALL 0x00452b90
void* __fastcall GetNetworkLobbyPeerEntryStreamByPeer(void* pObject, int nUnused, int param_1)
{
	int iVar1 = FindNetworkLobbyPeerSlotByPeer(pObject, nUnused, param_1);
	if (iVar1 == -1) {
		return 0;
	}
	return (void*) (*(int*) ((char*) pObject + 0x14) + iVar1 * 0x50);
}

// MACINTOSH: activate_coll_chunk_object()
// FUNCTION: LEMBALL 0x004228b0
void __fastcall ActivateCollChunkObject(void* pObject)
{
	ResetManagedEntityRuntimeStateThunk(pObject);
	*(int*) ((char*) pObject + 0x9c) = *(int*) ((char*) pObject + 0x40);
	*(int*) ((char*) pObject + 0xa0) = *(int*) ((char*) pObject + 0x44);
	*(int*) ((char*) pObject + 0xa4) = *(int*) ((char*) pObject + 0x48);
	*(int*) ((char*) pObject + 0x138) = 1;
	*(int*) ((char*) pObject + 0xb8) = 0x18;
}

// MACINTOSH: initialize_enmy_chunk_object_behavior_fields(int, int, int, int, int, int)
// FUNCTION: LEMBALL 0x0041fe30
void __fastcall InitializeEnmyChunkObjectBehaviorFields(void* pObject, int nUnused, int param_1, int param_2, int param_3, int param_4, int param_5, int param_6)
{
	*(int*) ((char*) pObject + 300) = param_1;
	*(int*) ((char*) pObject + 0x130) = param_2;
	*(int*) ((char*) pObject + 0x138) = param_3;
	*(int*) ((char*) pObject + 0x13c) = param_4;
	*(int*) ((char*) pObject + 0x144) = param_5;
	*(int*) ((char*) pObject + 0x148) = param_6;
}

// MACINTOSH: destroy_cached_chunk_object_children()
// FUNCTION: LEMBALL 0x0041e9a0
void __fastcall DestroyCachedChunkObjectChildren(void* pObject)
{
	int i;
	if (*(int*) ((char*) pObject + 0xa4) > 0) {
		for (i = 0; i < *(int*) ((char*) pObject + 0xa4); i++) {
			void* pChild = *(void**) ((char*) pObject + 4 + i * 4);
			if (pChild != 0) {
				(*( void(**)(int)) *(void***) pChild)(1);
			}
			*(void**) ((char*) pObject + 4 + i * 4) = 0;
		}
	}
	*(int*) ((char*) pObject + 0xa4) = 0;
	*(int*) ((char*) pObject + 0xa8) = 0;
}

// MACINTOSH: deactivate_managed_entity_group_child_by_slot(short)
// FUNCTION: LEMBALL 0x0041b940
void __fastcall DeactivateManagedEntityGroupChildBySlot(void* pObject, int nUnused, short param_2)
{
	int i;
	if (*(unsigned short*) ((char*) pObject + 0x36) != 0) {
		for (i = 0; ; i++) {
			int iVar1 = *(int*) (*(int*) ((char*) pObject + 0x3c) + i * 4);
			if (iVar1 != 0 && ((char(__fastcall*)(void*)) 0x401794)((void*) iVar1) == (char) param_2) {
				((void(__fastcall*)(void*, int)) 0x4024ff)(pObject, i);
				break;
			}
			if ((int) *(unsigned short*) ((char*) pObject + 0x36) <= i + 1) {
				return;
			}
		}
	}
}

// MACINTOSH: get_managed_entity_geometry_fields(undefined4*, undefined4*, undefined4*, undefined4*, undefined4*, undefined4*)
// FUNCTION: LEMBALL 0x0041fe70
void __fastcall GetManagedEntityGeometryFields(void* pObject, int nUnused, int* param_2, int* param_3, int* param_4, int* param_5, int* param_6, int* param_7)
{
	*param_2 = *(int*) ((char*) pObject + 300);
	*param_3 = *(int*) ((char*) pObject + 0x130);
	*param_4 = *(int*) ((char*) pObject + 0x138);
	*param_5 = *(int*) ((char*) pObject + 0x13c);
	*param_6 = *(int*) ((char*) pObject + 0x144);
	*param_7 = *(int*) ((char*) pObject + 0x148);
}

// MACINTOSH: delete_linked_type_0x11_chunk_object(byte) [scalar-dtor]
// FUNCTION: LEMBALL 0x0041c540
void* __fastcall DeleteLinkedType11ChunkObject(void* pObject, int nUnused, unsigned char param_1)
{
	*(void**) pObject = (void**) 0x4948d0;
	void* pLinked = *(void**) ((char*) pObject + 0x148);
	if (pLinked != 0 && *(int*) ((char*) pObject + 0x14c) != 0xffff && pLinked != 0) {
		(*( void(**)(int)) *(void***) pLinked)(1);
	}
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
	if ((param_1 & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// MACINTOSH: resolve_variant_render_frame_pointer(int, undefined4*)
// FUNCTION: LEMBALL 0x004676a0
int __fastcall ResolveVariantRenderFramePointer(void* pObject, int nUnused, int param_2, void** param_3)
{
	int iVar1 = *(int*) (*(int*) ((char*) pObject + 0x24) + *(short*) (*(int*) ((char*) pObject + 0x28) + param_2 * 2) * 4);
	int iVar2;
	if (param_3 == 0) {
		iVar2 = 0;
	} else {
		iVar2 = (*( int(**)(void)) *param_3)();
	}
	if (*(int*) (iVar1 + 0x40) == 0x5a524c45) {
		return iVar1;
	}
	return *(int*) (iVar1 + 0x78) + iVar2 * 0x54;
}

// MACINTOSH: activate_saved_position_chunk_object()
// FUNCTION: LEMBALL 0x0041c670
void __fastcall ActivateSavedPositionChunkObject(void* pObject)
{
	*(int*) ((char*) pObject + 0x9c) = *(int*) ((char*) pObject + 0x138);
	*(int*) ((char*) pObject + 0xa0) = *(int*) ((char*) pObject + 0x13c);
	*(int*) ((char*) pObject + 0x38) = 1;
	*(int*) ((char*) pObject + 0xa4) = *(int*) ((char*) pObject + 0x140);
	if (*(int*) 0x4a011c != 0) {
		((void(__fastcall*)(void*, void*)) 0x4032dd)(*(void**) 0x49d128, pObject);
	}
}
