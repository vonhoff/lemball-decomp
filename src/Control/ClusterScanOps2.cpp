// Cluster reconstruction supplemental TU (avoids append-corruption on the large ClusterScanOps.cpp).
#include "Visos/Generic/Memory.h"

extern void __fastcall AppendType18ChunkObject(void* pStream, int nUnused, unsigned short param_1, void* param_2, int param_3, int param_4);
extern void* g_pActiveManagedEntityOwner;
struct PlasChildStateEntityView;
extern void __cdecl DispatchPlasChildStateTableVariant0(void* pContext, PlasChildStateEntityView* pEntity);
extern void __cdecl DispatchPlasChildStateTableVariant1(void* pContext, PlasChildStateEntityView* pEntity);

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
