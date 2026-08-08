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
extern void* g_pAnimChunkTileGrid;
extern void* g_pLevelTileGrid;
extern int g_nSelectedNetworkLobbyPeerId;
extern void* g_pActiveNetworkRuntimeWindow;
extern "C" unsigned long __stdcall timeGetTime();
#include <string.h>
struct PlasChildStateEntityView;
extern void __cdecl DispatchPlasChildStateTableVariant0(void* pContext, PlasChildStateEntityView* pEntity);
extern void __cdecl DispatchPlasChildStateTableVariant1(void* pContext, PlasChildStateEntityView* pEntity);
// Palette remap pointer table view (mirrors VSINIT.CPP's PaletteRemapPointerTableMemberView for member-symbol linking).
struct PaletteRemapPointerTableMemberView {
	void** m_ppItems;
	int m_nCursor;
	void ReleasePaletteRemapVariant(void* pVariant);
};

// Minimal view-of the LEVELSTAT LevelTileGridOwnerView height sampler so Jump can emit a
// direct `call LevelTileGridOwnerView::GetZ` (matches orig ILT 0x401460). Method mangling
// depends only on name+signature, so this links to the LEVELSTAT def (same as CGameObjectMove.cpp).
struct LevelTileGridOwnerView {
	unsigned short GetZ(int x, int y, void** ppMoveChunk);
};
// FUNCTION: LEMBALL 0x00412eb0
void __fastcall CAI_AddNewTrapDoor(void* pObject, int nUnused, int param_1, int param_2, int param_3, int param_4)
{
	int nSlot = ((int(__fastcall*)()) 0x40227a)();
	int local_c = param_1 << 12;
	int local_8 = param_2 << 12;
	int local_4 = param_3 << 12;
	AppendType18ChunkObject(*(void**) ((char*) pObject + 0x1c4), nUnused, (unsigned short) nSlot, &local_c, 0, param_4);
}
// FUNCTION: LEMBALL 0x0040f600
void __fastcall CPlayerLemming_HitMine(void* pObject)
{
	int vec[3];
	vec[0] = 0;
	vec[1] = 0;
	*(int*) ((char*) pObject + 0x10c) = 1;
	vec[2] = 0xa000;
	(*( void(**)(void*, int)) (*(void***) pObject + 0x20 / 4))(vec, 0);
	*(int*) ((char*) pObject + 0x2c) = 1;
}
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
// FUNCTION: LEMBALL 0x0040f960
int __fastcall CPlayerLemming_HasObject(void* pObject, int nUnused, int param_1)
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
// FUNCTION: LEMBALL 0x00450820
void __fastcall DeactivateEmbeddedMediaIfActive(void* pObject)
{
	if (*(int*) ((char*) pObject + 0x5fc) != 0 && *(int*) ((char*) pObject + 0x524) == 1) {
		(*( void(**)(void)) (*(void***) *(void**) ((char*) pObject + 0x520) + 0x74 / 4))();
		*(int*) ((char*) pObject + 0x5fc) = 0;
		*(int*) ((char*) pObject + 0x5f8) = ((unsigned(*)(void)) 0x462e80)() + 0x28;
	}
}
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
// FUNCTION: LEMBALL 0x00414e80
int __fastcall CAICursor_ProcessMsg(void* pObject, int nUnused, short* param_1)
{
	if (*param_1 != 1) {
		*(int*) ((char*) pObject + 0xc) = *(int*) ((char*) pObject + 0xc) + 1;
		return 0;
	}
	((void(__fastcall*)(void*, int, int)) 0x40281f)(pObject, *(int*) ((char*) param_1 + 8), *(int*) ((char*) param_1 + 0xc));
	return 1;
}
// FUNCTION: LEMBALL 0x0040be20
void __fastcall CSlinkyManager_Process(void* pObject)
{
	int i;
	if (*(int*) ((char*) pObject + 0xc) > 0) {
		for (i = 0; i < *(int*) ((char*) pObject + 0xc); i++) {
			(*( void(**)(void)) (*(void***) (*(int*) ((char*) pObject + 4) + i * 0x150) + 0x14 / 4))();
		}
	}
}
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
// FUNCTION: LEMBALL 0x00466fd0
int __fastcall GetHelperUploadRectEntryAndState(void* pObject, int nUnused, int param_2, int* param_3)
{
	if (*(int*) ((char*) pObject + 4) == -1) {
		((void(__fastcall*)(void*)) 0x466ef0)(pObject);
	}
	*param_3 = *(int*) (*(int*) ((char*) pObject + 0xc) + 8 + param_2 * 0xc);
	return param_2 * 0xc + *(int*) ((char*) pObject + 0xc);
}
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
// FUNCTION: LEMBALL 0x00421440
int __fastcall AppendNodeChunkRecordFromLevelData(void* pObject, int nUnused, int param_2, int param_3)
{
	int iVar1 = *(int*) ((char*) pObject + 4);
	*(int*) ((char*) pObject + 4) = iVar1 + 1;
	((void(__fastcall*)(void*, int, int, int)) 0x402572)((void*) (iVar1 * 0x14 + *(int*) pObject), param_2, param_3, 0);
	return iVar1;
}
// FUNCTION: LEMBALL 0x00437da0
void __fastcall C2D_TriggerPause(void* pObject, int nUnused, int param_1)
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
// FUNCTION: LEMBALL 0x004275f0
void __fastcall CDuplicator_Restart(void* pObject)
{
	ResetManagedEntityRuntimeStateThunk(pObject);
	*(unsigned short*) ((char*) pObject + 0xbc) = 0;
	*(int*) ((char*) pObject + 0x94) = 0;
	*(int*) ((char*) pObject + 0x13c) = 0;
	*(int*) ((char*) pObject + 0x138) = 0;
	*(int*) ((char*) pObject + 0xb8) = 0x18;
	((void(__fastcall*)(void*, void*)) 0x4023e2)(pObject, (char*) pObject + 0x40);
}
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
// FUNCTION: LEMBALL 0x0045da50
void __fastcall CResFONT_UnLoadResources(void* pObject, int nUnused, int param_2, int param_3)
{
	ReleaseTypedResourceObjectIfLoaded((void*) (*(int*) ((char*) pObject + 0x80) + param_2 * 0x4c), 0, param_3);
	ReleaseTypedResourceObjectIfLoaded((void*) (*(int*) ((char*) pObject + 0x7c) + param_2 * 0x54), 0, param_3);
}
// FUNCTION: LEMBALL 0x004131b0
void __fastcall SetLevelModeProfileRangeSelector(void* pObject, int nUnused, int param_2)
{
	*(int*) ((char*) pObject + 0xd0) = param_2;
	((void(__fastcall*)(void*, int, int, int, int, int)) 0x401d25)(pObject, param_2, *(int*) ((char*) pObject + 0xc0), *(int*) ((char*) pObject + 0xc4), *(int*) ((char*) pObject + 0xc8), *(int*) ((char*) pObject + 0xcc));
}
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
// FUNCTION: LEMBALL 0x00443250
void __fastcall RefreshLevelScreenActionPanel(void* pObject)
{
	int i;
	for (i = 0; i < 4; i++) {
		((void(__fastcall*)(void*)) 0x4032bf)(*(void**) ((char*) pObject + 0x1c + i * 4));
	}
}
// FUNCTION: LEMBALL 0x004125c0
void __fastcall CAI_HitTrampoline(void* pObject, int nUnused, void* param_1, void* param_2)
{
	((void(__fastcall*)(void*, void*, void*)) 0x402bd5)(*(void**) ((char*) pObject + 0x1b4), param_1, param_2);
}
// FUNCTION: LEMBALL 0x004130d0
void __fastcall CAI_FindMoverHeight(void* pObject, int nUnused, void* param_1, void* param_2, void* param_3)
{
	((void(__fastcall*)(void*, void*, void*, void*)) 0x402e55)(*(void**) ((char*) pObject + 0x1c0), param_1, param_2, param_3);
}
// FUNCTION: LEMBALL 0x00455da0
void* __fastcall ConstructNetworkLobbyPlayerEntryChild(void* pObject)
{
	((void(__fastcall*)(void*)) 0x46a300)(pObject);
	*(void**) pObject = (void**) 0x4986b0;
	((void(__fastcall*)(void*)) 0x4014dd)(pObject);
	return pObject;
}
// FUNCTION: LEMBALL 0x004127e0
void __fastcall CAI_BulletCheckGroupIntersection(void* pObject, int nUnused, void* param_1, void* param_2)
{
	((void(__fastcall*)(void*, void*, void*)) 0x402680)(*(void**) ((char*) pObject + 0x168), param_1, param_2);
}
// FUNCTION: LEMBALL 0x00436480
void __fastcall C2D_UnRegisterRemaps(void* pObject)
{
	int i;
	for (i = 0; i < 5; i++) {
		((PaletteRemapPointerTableMemberView*) *(void**) 0x4a2000)->ReleasePaletteRemapVariant((void*) *(int*) ((char*) pObject + 0x64 + i * 4));
	}
}
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
// FUNCTION: LEMBALL 0x0044f3d0
void __fastcall RefreshQueuedZrleVariantRangeSelection(void* pObject)
{
	if (*(void**) ((char*) pObject + 0x44) != 0) {
		*(int*) ((char*) pObject + 0x40) = **(int**) ((char*) pObject + 0x44);
	}
	((void(__cdecl*)(void*, int)) 0x4688e0)(*(void**) ((char*) pObject + 0x4c), *(int*) (*(int*) ((char*) pObject + 0x48) + (*(int*) ((char*) pObject + 0x40) - *(int*) ((char*) pObject + 0x3c)) * 4));
}
// FUNCTION: LEMBALL 0x0042f5e0
void __fastcall CMoverManager_Add(void* pObject, int nUnused, unsigned short param_1, unsigned short param_2, void* param_3, int param_4, void* param_5)
{
	if (*(int*) ((char*) pObject + 0x34) < *(int*) ((char*) pObject + 0x30)) {
		((void(__fastcall*)(void*, unsigned short, unsigned short, void*, int, void*)) 0x40128a)((void*) (*(int*) ((char*) pObject + 0x38) + *(int*) ((char*) pObject + 0x34) * 0x1a0), param_1, param_2, param_3, param_4, param_5);
		*(int*) ((char*) pObject + 0x34) = *(int*) ((char*) pObject + 0x34) + 1;
	}
}
// FUNCTION: LEMBALL 0x0040f9b0
int __fastcall CPlayerLemming_AddObject(void* pObject, int nUnused, int param_1, int param_2)
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
// FUNCTION: LEMBALL 0x0041ccc0
void __fastcall CCrate_TriggerContents(void* pObject)
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
// FUNCTION: LEMBALL 0x0041aca0
int __fastcall CBullet_Receive(void* pObject, int nUnused, short param_1, void* param_2)
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
// FUNCTION: LEMBALL 0x004431c0
void __fastcall SyncLevelScreenActionPanelChildValues(void* pObject)
{
	int i;
	for (i = 0; i < 4; i++) {
		void* pChild = *(void**) ((char*) pObject + 0x1c + i * 4);
		*(int*) ((char*) pChild + 8) = *(int*) (*(int*) ((char*) pObject + 0x18) + 0x1cc + i * 4 + 4);
	}
}
// FUNCTION: LEMBALL 0x00421ff0
void __fastcall CBallManager_Delete(void* pObject, int nUnused, int param_1)
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
// FUNCTION: LEMBALL 0x00454b10
void __fastcall ReleaseNetworkLobbyPaletteRemaps(void* pObject)
{
	int i;
	for (i = 0; i < 6; i++) {
		((PaletteRemapPointerTableMemberView*) *(void**) 0x4a2000)->ReleasePaletteRemapVariant((void*) *(int*) ((char*) pObject + 0x414 + i * 4));
	}
}
// FUNCTION: LEMBALL 0x00454b40
void __fastcall MarkNetworkLobbyPlayerEntryDirty(void* pObject, int nUnused, int param_1)
{
	if (param_1 != -1 && param_1 < 10) {
		*(int*) (*(int*) ((char*) pObject + 0x3cc) + 0x40 + param_1 * 0x44) = 1;
	}
}
// FUNCTION: LEMBALL 0x00415780
void __fastcall CGameObject_StopMoving(void* pObject)
{
	((void(__fastcall*)(void*)) 0x402351)(pObject);
	*(int*) ((char*) pObject + 0x88) = 0;
	*(int*) ((char*) pObject + 0xcc) = g_nLevelFrameClockTick;
	*(int*) ((char*) pObject + 0xc8) = g_nLevelFrameClockTick;
}
// FUNCTION: LEMBALL 0x0045da90
void __fastcall CResFONT_UnLoadVramData(void* pObject, int nUnused, int param_2, int param_3)
{
	(*( void(**)(int)) (*(void***) (*(int*) ((char*) pObject + 0x7c) + param_2 * 0x54) + 0x24 / 4))(param_3);
}
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
// FUNCTION: LEMBALL 0x00473610
void* __fastcall ConstructPaletteRemapVariantWrapper(void* pThis, int nUnused, int param_1, unsigned char* param_2, int param_3)
{
	ConstructPaletteRemapVariant(pThis, nUnused, param_1, param_2, param_3);
	return pThis;
}
// FUNCTION: LEMBALL 0x00469690
void __fastcall RefreshResourceSpriteTextOwnerLayout(void* pObject)
{
	if (*(int*) ((char*) pObject + 0x14c) != 0) {
		((void(__cdecl*)(void*, int)) 0x469210)(*(void**) ((char*) pObject + 0x154), 0);
		return;
	}
	((void(__cdecl*)(void*, int)) 0x469210)(*(void**) ((char*) pObject + 0x150), 0);
}
// FUNCTION: LEMBALL 0x00432650
void __fastcall RestoreInertialEventSourceSinkVtables(void* pObject)
{
	*(void**) ((char*) pObject + 0x60) = (void**) 0x496ca8;
	int i;
	for (i = 0; i < 2; i++) {
		((void(__fastcall*)(void*)) 0x401307)((char*) pObject + 0x34 - i * 0x10);
	}
}
// FUNCTION: LEMBALL 0x004327e0
void __fastcall EmitVariantChildSlotRenderMarker(void* pObject, int nUnused, void* param_2, void* param_3)
{
	((void(__fastcall*)(void*, short, short, int, int, int, int)) 0x4016a9)(*(void**) ((char*) pObject + 0x10), *(short*) param_3, *(short*) ((char*) param_3 + 2), 0xfd, 0, 0, 0);
}
// FUNCTION: LEMBALL 0x00413130
void __fastcall ConfigureLevelModeManagedEntityProfiles(void* pObject, int nUnused, int param_2, int param_3, int param_4, int param_5, int param_6)
{
	((void(__fastcall*)(void*, int, int, int, int, int)) 0x402720)(*(void**) ((char*) pObject + 0x15c), param_2, param_3, param_4, param_5, param_6);
}
// FUNCTION: LEMBALL 0x00416820
void __fastcall CGameObject_StartLand(void* param_1)
{
	*(int*) ((char*) param_1 + 0xcc) = g_nLevelFrameClockTick + 8;
	((void(__fastcall*)(void*, void*, void*, unsigned short)) 0x40341d)(g_pActiveManagedEntityOwner, (char*) param_1 + 0x9c, param_1, *(unsigned short*) ((char*) param_1 + 0x68));
}
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
// FUNCTION: LEMBALL 0x004381c0
void __fastcall C2D_OnButtonUp(void* pObject, int nUnused, void* param_2)
{
	*(int*) ((char*) pObject + 0x178) = 0;
	if (*(int*) ((char*) pObject + 0xa68) == 0) {
		if ((g_pLevelDemoPlaybackController == 0 || *(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) == 0) && ((int(__fastcall*)(void*)) (*(void***) *(void**) ((char*) pObject + 0x964) + 0x60 / 4))(*(void**) ((char*) pObject + 0x964)) == 0) {
			return;
		}
		((void(__fastcall*)(void*)) 0x4019ec)((char*) pObject - 0x14);
	}
}
// FUNCTION: LEMBALL 0x00422060
void __fastcall CBallManager_LoadLevel(void* pObject, int nUnused, unsigned short* param_1)
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
// FUNCTION: LEMBALL 0x00419490
int __fastcall CPlayerLemmingGroupManager_GetViewData(void* pObject, int nUnused, int param_1)
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
// FUNCTION: LEMBALL 0x00411b10
void __fastcall CAI_Start(void* param_1)
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
// FUNCTION: LEMBALL 0x00410250
void __fastcall CPlayerLemming_OnConveyor(void* pThis, int nUnused, int param_1, void* param_2, int param_3)
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
// FUNCTION: LEMBALL 0x00418ab0
void __fastcall CPlayerLemmingGroupManager_UseObject(void* pOwner, int nUnused, unsigned int param_1)
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
// FUNCTION: LEMBALL 0x00418b60
void __fastcall CPlayerLemmingGroupManager_PlayerGroupRequestFire(void* pObject, int nUnused, int param_1, int param_2)
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
// FUNCTION: LEMBALL 0x0045daf0
void __fastcall CResFONT_OnLoad(void* pObject)
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
// FUNCTION: LEMBALL 0x0041f820
int __fastcall CSheepGroup_Process(void* pObject)
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
// FUNCTION: LEMBALL 0x0041fad0
void __fastcall CSheep_HitMine(void* pObject)
{
	((void(__fastcall*)(void*, int)) 0x402f22)(g_pActiveManagedEntityOwner, 0x96);
	int vec[3];
	vec[0] = 0;
	vec[1] = 0;
	vec[2] = 0xa000;
	(*( void(**)(void*, int)) (*(void***) pObject + 0x20 / 4))(vec, 0);
}
// FUNCTION: LEMBALL 0x00409d20
void __fastcall SetInvsChunkObjectPointBounds(void* pObject, int nUnused, unsigned short param_2, unsigned short param_3, unsigned short param_4)
{
	unsigned short p1[3];
	unsigned short p2[3];
	p1[0] = param_2;
	p1[1] = param_3;
	p1[2] = param_4;
	p2[0] = param_2;
	p2[1] = param_3;
	p2[2] = param_4;
	((void(__fastcall*)(void*, void*, void*)) 0x40237e)(pObject, p1, p2);
}
// FUNCTION: LEMBALL 0x00443d80
void __fastcall RebuildLevelScreenPauseDialogLayout(void* param_1)
{
	short layout[4];
	((void(__fastcall*)(void*)) 0x403198)(param_1);
	((void(__fastcall*)(void*)) 0x402603)(param_1);
	((void(__fastcall*)(void*, short*)) 0x4030fd)(param_1, layout);
	((void(__fastcall*)(void*, short*)) 0x401c85)(param_1, layout);
}
// FUNCTION: LEMBALL 0x00420600
void __fastcall CEnemy_HitBullet(void* pThis, int nUnused, void* param_1)
{
	if (*(int*) ((char*) param_1 + 0x16c) != 1) {
		*(int*) ((char*) pThis + 0x124) = 1;
		*(int*) ((char*) pThis + 0xcc) = g_nLevelFrameClockTick + 0x3c;
		short sVar1 = *(short*) ((char*) param_1 + 0xb4);
		*(int*) ((char*) pThis + 0x2c) = 1;
		*(unsigned short*) ((char*) pThis + 0xb4) = sVar1 + 4U & 7;
	}
}
// FUNCTION: LEMBALL 0x00438170
void __fastcall C2D_OnInside(void* pThis, int nUnused, unsigned short* param_1)
{
	if ((g_pLevelDemoPlaybackController == 0 || *(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) == 0) && ((int(__fastcall*)(void*)) (*(void***) *(void**) ((char*) pThis + 0x964) + 0x60 / 4))(*(void**) ((char*) pThis + 0x964)) == 0) {
		return;
	}
	*(unsigned short*) ((char*) pThis + 0x928) = param_1[0];
	*(unsigned short*) ((char*) pThis + 0x92a) = param_1[1];
	((void(__fastcall*)(void*)) 0x4019ec)((char*) pThis - 0x14);
}
// FUNCTION: LEMBALL 0x004079e0
void __fastcall EmitMainGameStatusStreamSeparators(void* param_1)
{
	void** pStatus = *(void***) 0x4a97bc;
	void* pvVar1 = (void*) (*( void*(**)(void*, int)) **(void***) *(void**) ((char*) param_1 + 0x50))(pStatus, 10);
	((void(__fastcall*)(void*, char)) 0x4585d0)(pvVar1, (char) pStatus);
	void** pStatus2 = *(void***) 0x4a97bc;
	void* pvVar2 = (void*) (*( void*(**)(void*, int)) **(void***) *(void**) ((char*) param_1 + 0x54))(pStatus2, 10);
	((void(__fastcall*)(void*, char)) 0x4585d0)(pvVar2, (char) pStatus2);
}
// FUNCTION: LEMBALL 0x00420090
unsigned int __fastcall IsSavedTargetWithinProbeBoxesAndDirectionalBand(void* param_1)
{
	if (*(int*) ((char*) g_pActiveManagedEntityOwner + 0x58) == 0) {
		return 0;
	}
	unsigned int bVar1 = ((int(__fastcall*)(void*, int)) 0x40358f)(param_1, 0x32);
	if (bVar1 == 0) {
		return 0;
	}
	int copy[3];
	((void(__fastcall*)(void*, void*)) 0x40161d)(copy, (char*) param_1 + 0x150);
	unsigned int uVar2 = ((unsigned int(__fastcall*)(void*, int, int, int)) 0x403346)(param_1, copy[0], copy[1], copy[2]);
	return bVar1 & uVar2;
}
// FUNCTION: LEMBALL 0x0041a760
void __fastcall CBullet_FireBullet(void* param_1)
{
	*(int*) ((char*) param_1 + 0xc8) = g_nLevelFrameClockTick;
	((void(__fastcall*)(void*)) 0x40303a)(param_1);
	if (*(int*) 0x4a011c != 0) {
		((void(__fastcall*)(void*, void*)) 0x40126c)(*(void**) ((char*) param_1 + 0x60), param_1 != 0 ? (char*) param_1 + 0x138 : 0);
	}
}
// FUNCTION: LEMBALL 0x00419d90
void __cdecl ResetScheduleAndAdvanceManagedEntity(int param_1, int* param_2)
{
	int iVar1 = *param_2;
	(*( void(**)(void*)) (*(void***) iVar1 + 0x94 / 4))(param_2);
	(*( void(**)(void*, int)) (*(void***) iVar1 + 0x84 / 4))(param_2, 4000);
	((void(__cdecl*)(int, int*, int)) 0x40385a)(param_1, param_2, 0);
}
// FUNCTION: LEMBALL 0x00454830
void __fastcall FlushNetworkLobbyDeferredStatus(void* param_1)
{
	if (*(int*) ((char*) param_1 + 0x430) == 7 && (((void(__fastcall*)(void*)) 0x402c11)(*(void**) 0x49f140), *(int*) ((char*) param_1 + 0x3b8) == 0)) {
		return;
	}
	((void(__fastcall*)(void*, int, int)) 0x401d61)(param_1, *(int*) ((char*) param_1 + 0x430), 6000);
	*(int*) ((char*) param_1 + 0x430) = 0;
}
// FUNCTION: LEMBALL 0x00463ac0
void __cdecl SelectionSortRecordsWithComparator(char* param_1, char* param_2, int param_3, void* pComparator)
{
	char* end = param_2;
	while (param_1 < end) {
		char* pMax = param_1;
		char* pCur = param_1 + param_3;
		while (pCur <= end) {
			if (((int(__cdecl*)(void*, void*)) pComparator)(pCur, pMax) > 0) {
				pMax = pCur;
			}
			pCur += param_3;
		}
		((void(__cdecl*)(void*, void*, int)) 0x463b20)(pMax, end, param_3);
		end -= param_3;
	}
}
// FUNCTION: LEMBALL 0x00427050
int __fastcall CRocketManager_GetViewData(void* pThis, int nUnused, int param_1)
{
	int iVar2 = 0;
	int local_4 = 0;
	if (*(int*) ((char*) pThis + 0x34) > 0) {
		int iVar3 = 0;
		do {
			int* piVar1 = (int*) (*(int*) ((char*) pThis + 0x38) + iVar3);
			int iVar4 = param_1;
			if (piVar1[0x2e] != 0x18) {
				iVar4 = param_1 + 0x4c;
				(*( void(**)(int)) (*(void***) *piVar1 + 0xc / 4))(param_1);
				local_4 = local_4 + 1;
			}
			iVar3 = iVar3 + 0x144;
			iVar2 = iVar2 + 1;
			param_1 = iVar4;
		} while (iVar2 < *(int*) ((char*) pThis + 0x34));
	}
	return local_4;
}
// FUNCTION: LEMBALL 0x00422ef0
int __fastcall EmitCollChunkManagerRenderEntries(void* pThis, int nUnused, int param_1)
{
	int local_4 = 0;
	int iVar2 = 0;
	if (*(int*) ((char*) pThis + 0x3c) > 0) {
		int iVar4 = 0;
		do {
			int* iVar1 = *(int**) ((char*) pThis + 0x34);
			int iVar3 = param_1;
			if (*(int*) (*(int*) (iVar4 + (int) iVar1) + 0x138) != 0 || *(int*) (*(int*) (iVar4 + (int) iVar1) + 0x98) != 0) {
				iVar3 = param_1 + 0x4c;
				local_4 = local_4 + 1;
				(*( void(**)(int)) (*(void***) **(int**) (iVar4 + (int) *(int**) ((char*) pThis + 0x34)) + 0xc / 4))(param_1);
			}
			iVar4 = iVar4 + 4;
			iVar2 = iVar2 + 1;
			param_1 = iVar3;
		} while (iVar2 < *(int*) ((char*) pThis + 0x3c));
	}
	return local_4;
}
// FUNCTION: LEMBALL 0x0042b4f0
int __fastcall CTrampolineManager_GetViewData(void* pThis, int nUnused, int param_1)
{
	int local_4 = 0;
	int iVar2 = 0;
	if (*(int*) ((char*) pThis + 0x34) > 0) {
		int iVar3 = 0;
		do {
			int* piVar1 = (int*) (*(int*) ((char*) pThis + 0x38) + iVar3);
			int iVar4 = param_1;
			if (piVar1[0x4e] != 0) {
				iVar4 = param_1 + 0x4c;
				(*( void(**)(int)) (*(void***) *piVar1 + 0xc / 4))(param_1);
				local_4 = local_4 + 1;
			}
			iVar3 = iVar3 + 0x144;
			iVar2 = iVar2 + 1;
			param_1 = iVar4;
		} while (iVar2 < *(int*) ((char*) pThis + 0x34));
	}
	return local_4;
}
// FUNCTION: LEMBALL 0x0042c520
int __fastcall CPaintGunManager_GetViewData(void* pThis, int nUnused, int param_1)
{
	int local_4 = 0;
	int iVar2 = 0;
	if (*(int*) ((char*) pThis + 0x34) > 0) {
		int iVar3 = 0;
		do {
			int* piVar1 = (int*) (*(int*) ((char*) pThis + 0x38) + iVar3);
			int iVar4 = param_1;
			if (piVar1[0x4e] != 0) {
				iVar4 = param_1 + 0x4c;
				(*( void(**)(int)) (*(void***) *piVar1 + 0xc / 4))(param_1);
				local_4 = local_4 + 1;
			}
			iVar3 = iVar3 + 0x144;
			iVar2 = iVar2 + 1;
			param_1 = iVar4;
		} while (iVar2 < *(int*) ((char*) pThis + 0x34));
	}
	return local_4;
}
// FUNCTION: LEMBALL 0x0042f540
int __fastcall CMoverManager_GetViewData(void* pThis, int nUnused, int param_1)
{
	int iVar2 = 0;
	int iVar3 = 0;
	int local_4 = 0;
	if (*(int*) ((char*) pThis + 0x34) > 0) {
		do {
			int* piVar1 = (int*) (*(int*) ((char*) pThis + 0x38) + iVar3);
			iVar2 = iVar2 + 1;
			iVar3 = iVar3 + 0x1a0;
			(*( void(**)(int)) (*(void***) *piVar1 + 0xc / 4))(param_1);
			local_4 = local_4 + 1;
			param_1 = param_1 + 0x4c;
		} while (iVar2 < *(int*) ((char*) pThis + 0x34));
	}
	return local_4;
}
// FUNCTION: LEMBALL 0x0040e080
int __fastcall CDoorManager_GetViewData(void* pThis, int nUnused, int param_1)
{
	int iVar2 = 0;
	int iVar3 = 0;
	if (*(int*) ((char*) pThis + 0x34) > 0) {
		do {
			int* piVar1 = (int*) (*(int*) ((char*) pThis + 0x3c) + iVar2);
			iVar2 = iVar2 + 0x14c;
			iVar3 = iVar3 + 1;
			(*( void(**)(int)) (*(void***) *piVar1 + 0xc / 4))(param_1);
			param_1 = param_1 + 0x4c;
		} while (iVar3 < *(int*) ((char*) pThis + 0x34));
	}
	return *(int*) ((char*) pThis + 0x34);
}
// FUNCTION: LEMBALL 0x0040e550
void __fastcall CDoorManager_Process(void* param_1)
{
	int iVar3 = 0;
	if (*(int*) ((char*) param_1 + 0x34) > 0) {
		int iVar2 = 0;
		do {
			*(int*) (*(int*) ((char*) param_1 + 0x3c) + 0x124 + iVar2) = 1;
			void* piVar1 = (void*) (*(int*) ((char*) param_1 + 0x3c) + iVar2);
			if (*(int*) ((char*) piVar1 + 0x51 * 4) != 0 || *(int*) ((char*) piVar1 + 0x45 * 4) != 0) {
				(*( void(**)(void)) (*(void***) piVar1 + 0x14 / 4))();
			}
			iVar2 = iVar2 + 0x14c;
			iVar3 = iVar3 + 1;
		} while (iVar3 < *(int*) ((char*) param_1 + 0x34));
	}
}
// FUNCTION: LEMBALL 0x0042c4d0
void __fastcall CPaintGunManager_Process(void* param_1)
{
	int iVar3 = 0;
	if (*(int*) ((char*) param_1 + 0x34) > 0) {
		int iVar2 = 0;
		do {
			*(int*) (*(int*) ((char*) param_1 + 0x38) + 0x124 + iVar2) = 1;
			void* piVar1 = (void*) (*(int*) ((char*) param_1 + 0x38) + iVar2);
			if (*(int*) ((char*) piVar1 + 0x4e * 4) != 0) {
				(*( void(**)(void)) (*(void***) piVar1 + 0x14 / 4))();
			}
			iVar2 = iVar2 + 0x144;
			iVar3 = iVar3 + 1;
		} while (iVar3 < *(int*) ((char*) param_1 + 0x34));
	}
}
// FUNCTION: LEMBALL 0x00427010
void __fastcall CRocketManager_Process(void* param_1)
{
	int iVar3 = 0;
	if (*(int*) ((char*) param_1 + 0x34) > 0) {
		int iVar2 = 0;
		do {
			*(int*) (*(int*) ((char*) param_1 + 0x38) + 0x124 + iVar2) = 1;
			void* piVar1 = (void*) (*(int*) ((char*) param_1 + 0x38) + iVar2);
			(*( void(**)(void)) (*(void***) piVar1 + 0x14 / 4))();
			iVar2 = iVar2 + 0x144;
			iVar3 = iVar3 + 1;
		} while (iVar3 < *(int*) ((char*) param_1 + 0x34));
	}
}
// FUNCTION: LEMBALL 0x00422420
void __fastcall CCollectableManager_Restart(void* param_1)
{
	if (*(int*) ((char*) param_1 + 0x34) != 0 && *(int*) ((char*) param_1 + 0x38) > 0) {
		int i;
		for (i = 0; i < *(int*) ((char*) param_1 + 0x38); i++) {
			void* pElem = *(void**) (*(int*) ((char*) param_1 + 0x34) + i * 4);
			if (pElem != 0) {
				(*( void(**)(void)) (*(void***) pElem + 0x104 / 4))();
			}
		}
	}
}
// FUNCTION: LEMBALL 0x004206a0
void __fastcall CEnemy_HitMine(void* pObject)
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
// FUNCTION: LEMBALL 0x0045d990
int __fastcall CResFONT_DirectResources(void* pObject, int nUnused, int param_2, void* param_3, void* param_4)
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
// FUNCTION: LEMBALL 0x00412e80
void __fastcall AppendType18ObjectWithFreeSlot(void* pObject, int nUnused, void* param_2, int param_3)
{
	int nSlot = ((int(__fastcall*)()) 0x40227a)();
	AppendType18ChunkObject(*(void**) ((char*) pObject + 0x1c4), nUnused, (unsigned short) nSlot, param_2, 0, param_3);
}
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
// FUNCTION: LEMBALL 0x0040ca10
void __fastcall SetType18ChunkObjectPositionByIndex(void* pObject, int nUnused, int param_2, int param_3, int param_4, int param_5)
{
	if (param_5 < *(int*) ((char*) pObject + 0x50)) {
		((void(__cdecl*)(void*, int, int, int)) 0x401cdf)(*(void**) ((char*) pObject + 0x30 + param_5 * 4), param_2, param_3, param_4);
	}
}
// FUNCTION: LEMBALL 0x0040dec0
void __fastcall CDoor_DoActivate(void* pObject)
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
// FUNCTION: LEMBALL 0x004270b0
void __fastcall CRocketManager_Add(void* pObject, int nUnused, unsigned short param_1, int param_2, int param_3, int param_4)
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
// FUNCTION: LEMBALL 0x00410220
void __fastcall CPlayerLemming_Action(void* pObject, int nUnused, int param_1)
{
	*(int*) ((char*) pObject + 0x94) = g_nLevelFrameClockTimeMs;
	if (param_1 == 8) {
		((void(__fastcall*)(void*, int)) 0x402cfc)(pObject, 8);
		return;
	}
	*(int*) ((char*) pObject + 0xb8) = param_1;
}
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
// FUNCTION: LEMBALL 0x00426fb0
int __fastcall CRocketManager_StepOn(void* pObject, int nUnused, void* param_1, int param_2)
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
// FUNCTION: LEMBALL 0x0045d9f0
int __fastcall CResFONT_DirectResources(void* pObject, int nUnused, int param_2, void* param_3)
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
// FUNCTION: LEMBALL 0x0041ebe0
void __fastcall CGenericGroupManager_CreateNewGroup(void* pObject, int nUnused, unsigned short nIdCount, unsigned short* pIdList)
{
	void* pMsg;
	void* pGroup;
	if (*(int*) ((char*) pObject + 0xa4) < 0x28) {
		pMsg = 0;
		pGroup = (void*) ((void* (__cdecl*)(int)) 0x45a780)(0x168);
		if (pGroup != 0) {
			pMsg = ((void* (__fastcall*)(void*, void*, void*, void*, void*)) 0x402103)(pGroup, 0,
				(void*) (*(int*) 0x4a782c), (void*) (*(int*) 0x4a7830), (void*) (*(int*) 0x4a7834));
		}
		*(void**) ((char*) pObject + *(int*) ((char*) pObject + 0xa4) * 4 + 4) = pMsg;
		*(int*) ((char*) pObject + 0xa4) += 1;
		if (nIdCount != 0) {
			unsigned int nIdx;
			typedef void (__fastcall * GroupAddProc)(void*, void*, void*, void*);
			GroupAddProc pAdd = *(GroupAddProc*) ((char*) *(void**) pObject + 0x3c);
			for (nIdx = 0; nIdx < nIdCount; nIdx++) {
				unsigned short uId = pIdList[nIdx];
				pAdd(pObject, 0, (void*) (0x4a6510 + (unsigned int) uId * 4), pMsg);
			}
		}
	}
}
// FUNCTION: LEMBALL 0x004496d0
void __fastcall DestroyLevelSelectionScreen(void* param_1)
{
	*(void**) param_1 = (void*) 0x497bf0;
	*(void**) ((char*) param_1 + 0x4) = (void*) 0x497be0;
	*(void**) ((char*) param_1 + 0x14) = (void*) 0x497bdc;
	FreeVSMemBlock(*(void**) ((char*) param_1 + 0x3c4));
	FreeVSMemBlock(*(void**) ((char*) param_1 + 0x3c8));
	FreeVSMemBlock(*(void**) ((char*) param_1 + 0x3c0));
	((void(__fastcall*)(void*)) 0x401587)(param_1);
	if (*(int*) ((char*) param_1 + 0x9c) != 0) {
		((void(__fastcall*)(void*)) 0x402be4)(param_1);
	}
	char* pChild = (char*) param_1 + 0x3bc;
	int i;
	for (i = 0; i < 2; i++) {
		pChild -= 0x24;
		((void(__fastcall*)(void*)) 0x403558)(pChild);
	}
	((void(__fastcall*)(void*)) 0x402081)(param_1);
}
// FUNCTION: LEMBALL 0x004515c0
void __fastcall DestroyPasswordEntryScreen(void* param_1)
{
	*(void**) param_1 = (void*) 0x498408;
	*(void**) ((char*) param_1 + 0x4) = (void*) 0x4983f8;
	*(void**) ((char*) param_1 + 0x14) = (void*) 0x4983f0;
	if (*(int*) ((char*) param_1 + 0x9c) != 0) {
		((void(__fastcall*)(void*)) 0x40244b)(param_1);
	}
	*(void**) ((char*) param_1 + 0x454) = (void*) 0x496ca8;
	((void(__fastcall*)(void*)) 0x402081)(param_1);
}
// FUNCTION: LEMBALL 0x004550c0
void __fastcall DestroyNetworkLobbyTransportController(void* param_1)
{
	*(void**) param_1 = (void*) 0x4986e8;
	*(void**) ((char*) param_1 + 0xc) = (void*) 0x4986d8;
	*(int*) 0x4a0128 = 0;
	if (*(void**) 0x49f144 != 0) {
		int iVar1 = (*( int(**)(void)) (*(void***) *(void**) 0x49f144 + 0x28 / 4))();
		if (iVar1 == 0) {
			((void(__fastcall*)(void*)) 0x4013a7)(param_1);
		} else {
			((void(__fastcall*)(void*)) 0x402c11)(param_1);
		}
	}
	if (*(void**) ((char*) param_1 + 0x30) != 0) {
		(*( void(**)(int)) (*(void***) *(void**) ((char*) param_1 + 0x30) + 0x14 / 4))(1);
	}
	if (*(void**) ((char*) param_1 + 0x34) != 0) {
		(*( void(**)(int)) (*(void***) *(void**) ((char*) param_1 + 0x34) + 0x14 / 4))(1);
	}
	((void(__fastcall*)(void*)) 0x403062)(param_1);
}
// FUNCTION: LEMBALL 0x004535c0
void __fastcall DestroyNetworkLobbyScreen(void* param_1)
{
	*(void**) param_1 = (void*) 0x498640;
	*(void**) ((char*) param_1 + 0x4) = (void*) 0x498630;
	*(void**) ((char*) param_1 + 0x14) = (void*) 0x49862c;
	if (*(int*) ((char*) param_1 + 0x374) == 0) {
		if (*(void**) 0x49f140 != 0) {
			((void(__fastcall*)(void*)) 0x402cca)(param_1);
		}
	} else if (*(void**) 0x49f140 != 0) {
		((void(__fastcall*)(void*)) 0x402c11)(*(void**) 0x49f140);
	}
	int iVar3;
	for (iVar3 = 0; iVar3 < 0x2a8; iVar3 += 0x44) {
		((void(__fastcall*)(void*, void*)) 0x46aa00)(*(void**) (*(int*) ((char*) param_1 + 0x84) + 0x1c), (void*) (*(int*) ((char*) param_1 + 0x3cc) + iVar3));
	}
	void* pPos = *(void**) ((char*) param_1 + 0x3cc);
	if (pPos != 0) {
		(*( void(**)(int)) (*(void***) pPos))(3);
	}
	void* uVar1 = *(void**) ((char*) param_1 + 0x398);
	if (uVar1 != 0) {
		FreeVSMemBlock(*(void**) ((char*) uVar1 + 0xc));
		FreeVSMemBlock(uVar1);
	}
	((void(__fastcall*)(void*)) 0x401bd6)(param_1);
	if (*(int*) ((char*) param_1 + 0x9c) != 0) {
		((void(__fastcall*)(void*)) 0x401221)(param_1);
	}
	*(void**) ((char*) param_1 + 0x3ec) = (void*) 0x496ca8;
	((void(__fastcall*)(void*)) 0x402081)(param_1);
}
// FUNCTION: LEMBALL 0x0043a2e0
int __fastcall DispatchIndexedEntryByMode(void* pThis, int nUnused, unsigned short* param_1)
{
	unsigned short uVar1 = *param_1;
	if (uVar1 > 2 && uVar1 < 5 && *(int*) ((char*) pThis + 0x14) > 0) {
		int* piVar3 = (int*) (*(int*) ((char*) pThis + 0x10) + 4);
		int iVar4 = 0;
		while (*piVar3 != *(int*) (param_1 + 4)) {
			piVar3 += 2;
			iVar4++;
			if (*(int*) ((char*) pThis + 0x14) <= iVar4) {
				return 0;
			}
		}
		int* pEntry = (int*) (*(int*) (*(int*) ((char*) pThis + 0x10) + iVar4 * 8));
		short pos[2];
		pos[0] = 0;
		pos[1] = 0;
		if (uVar1 == 4) {
			(*( void(**)(void*, int)) (*(void***) ((char*) pEntry + 0x90) + 4 / 4))(pos, 0);
		} else {
			(*( void(**)(void*, int)) (*(void***) ((char*) pEntry + 0x90) + 8 / 4))(pos, 0);
		}
		return 1;
	}
	return 0;
}
// FUNCTION: LEMBALL 0x0040fcd0
void __fastcall CPlayerLemming_ExternalControlEnd(void* pThis)
{
	if (*(unsigned short*) ((char*) pThis + 0xbc) != 0 && *(unsigned short*) ((char*) pThis + 0xbc) < 3) {
		(*( void(**)(void)) (*(void***) pThis + 0x68 / 4))();
		(*( void(**)(int)) (*(void***) pThis + 8 / 4))(8);
	} else {
		(*( void(**)(int)) (*(void***) pThis + 8 / 4))(0);
	}
}
// FUNCTION: LEMBALL 0x0040f4b0
int __fastcall CPlayerLemming_FacingTarget(void* pThis)
{
	unsigned int uDir = ((unsigned int(__cdecl*)(int, int, int, int)) 0x401532)(
		(*(int*) ((char*) pThis + 0x9c)) >> 12, (*(int*) ((char*) pThis + 0xa0)) >> 12,
		(*(int*) ((char*) pThis + 0x1b4)) >> 12, (*(int*) ((char*) pThis + 0x1b8)) >> 12);
	return (int) (short) (*(short*) ((char*) pThis + 0xb4)) - (int) uDir == 1;
}
// FUNCTION: LEMBALL 0x0040f430
int __fastcall CPlayerLemming_FacingCursor(void* pThis)
{
	if (*(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) != 0) {
		return 1;
	}
	int local_4;
	int local_8;
	((void(__fastcall*)(void*, void*, void*)) 0x401e65)(*(void**) ((char*) g_pActiveManagedEntityOwner + 0x160), &local_4, &local_8);
	unsigned int uDir = ((unsigned int(__cdecl*)(int, int, int, int)) 0x401532)(
		(*(int*) ((char*) pThis + 0x9c)) >> 12, (*(int*) ((char*) pThis + 0xa0)) >> 12,
		local_4, local_8);
	return (int) (short) (*(short*) ((char*) pThis + 0xb4)) - (int) uDir == 1;
}
// FUNCTION: LEMBALL 0x0040c950
int __fastcall GetType18ChunkObjectTilePosition(void* pThis, int nUnused, int* pOut, int nIndex)
{
	if (*(int*) ((char*) pThis + 0x50) == 0) {
		return 0;
	}
	int pObj = *(int*) ((char*) pThis + 0x30 + nIndex * 4);
	int x = *(int*) (pObj + 0x9c);
	pOut[0] = x;
	int y = *(int*) (pObj + 0xa0);
	pOut[1] = y;
	pOut[2] = *(int*) (pObj + 0xa4);
	unsigned int z = 0;
	if (!((x >> 12) < 0 || (y >> 12) < 0 ||
	      *(int*) ((char*) g_pLevelTileGrid + 0x10) <= (x >> 16) ||
	      *(int*) ((char*) g_pLevelTileGrid + 0x14) <= (y >> 16))) {
		z = ((unsigned int(__fastcall*)(void*, int, int)) 0x4029a5)(
			(void*) ((((y >> 16) * *(int*) ((char*) g_pLevelTileGrid + 0x10) + (x >> 16)) * 0xc) + *(int*) ((char*) g_pLevelTileGrid + 0xc)),
			x >> 12 & 0xf, y >> 12 & 0xf);
	}
	pOut[2] = (z & 0xffff) << 12;
	return 1;
}
// FUNCTION: LEMBALL 0x0040d230
void __fastcall RemoveAnimChunkRecordsAtTile(void* pThis, int nUnused, short* pTile)
{
	int i = 0;
	while (i < *(int*) ((char*) pThis + 4)) {
		if (*(short*) ((char*) pThis + 8 + i * 0x18) == pTile[0] &&
		    *(short*) ((char*) pThis + 8 + i * 0x18 + 2) == pTile[1]) {
			int k;
			for (k = i + 1; k < *(int*) ((char*) pThis + 4); k++) {
				memmove((char*) pThis + (k - 1) * 0x18, (char*) pThis + k * 0x18, 0x18);
			}
			*(int*) ((char*) pThis + 4) = *(int*) ((char*) pThis + 4) - 1;
		}
		i++;
	}
}
// FUNCTION: LEMBALL 0x0040e060
void __fastcall CDoorManager_dtor(void* pThis, int nUnused)
{
	*(int*) pThis = 0x493840;
	if (*(void**) ((char*) pThis + 0x3c) != 0) {
		((void(__fastcall*)(void*, int)) (*(void***) ((char*) pThis + 0x3c))[0])(
			*(void**) ((char*) pThis + 0x3c), 3);
	}
	((void(__fastcall*)(void*)) 0x45eea0)(pThis);
}
// FUNCTION: LEMBALL 0x00417ec0
void __fastcall CBulletManager_dtor(void* pThis, int nUnused)
{
	*(int*) pThis = 0x494008;
	if (*(void**) ((char*) pThis + 0x30) != 0) {
		((void(__fastcall*)(void*, int)) (*(void***) ((char*) pThis + 0x30))[0])(
			*(void**) ((char*) pThis + 0x30), 3);
	}
	((void(__fastcall*)(void*)) 0x45eea0)(pThis);
}
// FUNCTION: LEMBALL 0x00426c00
void __fastcall CRocketManager_dtor(void* pThis, int nUnused)
{
	*(int*) pThis = 0x496020;
	if (*(void**) ((char*) pThis + 0x38) != 0) {
		((void(__fastcall*)(void*, int)) (*(void***) ((char*) pThis + 0x38))[0])(
			*(void**) ((char*) pThis + 0x38), 3);
	}
	((void(__fastcall*)(void*)) 0x45eea0)(pThis);
}
// FUNCTION: LEMBALL 0x0042b090
void __fastcall CTrampolineManager_dtor(void* pThis, int nUnused)
{
	*(int*) pThis = 0x496710;
	if (*(void**) ((char*) pThis + 0x38) != 0) {
		((void(__fastcall*)(void*, int)) (*(void***) ((char*) pThis + 0x38))[0])(
			*(void**) ((char*) pThis + 0x38), 3);
	}
	((void(__fastcall*)(void*)) 0x45eea0)(pThis);
}
// FUNCTION: LEMBALL 0x0042c120
void __fastcall CPaintGunManager_dtor(void* pThis, int nUnused)
{
	*(int*) pThis = 0x496888;
	if (*(void**) ((char*) pThis + 0x38) != 0) {
		((void(__fastcall*)(void*, int)) (*(void***) ((char*) pThis + 0x38))[0])(
			*(void**) ((char*) pThis + 0x38), 3);
	}
	((void(__fastcall*)(void*)) 0x45eea0)(pThis);
}
// FUNCTION: LEMBALL 0x00418b20
void __fastcall CPlayerLemmingGroupManager_ReformAlteredGroups(void* pThis, int nUnused, int* param_1)
{
	int* pi;
	pi = (int*) ((int(__fastcall*)(void*)) 0x401078)(pThis);
	while (pi != 0) {
		if (pi != param_1) {
			((void(__fastcall*)(void*, int)) (*(int*) *pi + 0x154))(pi, *(int*) 0x4a7834);
		}
		pi = (int*) ((int(__fastcall*)(void*)) 0x403549)(pThis);
	}
}
// FUNCTION: LEMBALL 0x0045dab0
int __fastcall CResFONT_ForceLoadVram(void* pThis, int nUnused, int param_2)
{
	void* pSlot = *(void**) (*(int*) ((char*) pThis + 0x7c) + param_2 * 0x54);
	int r = ((int(__fastcall*)(void*)) (*(int*) pSlot + 0xc))(pSlot);
	if (r != 0) {
		return 1;
	}
	return ((int(__fastcall*)(void*)) (*(int*) pSlot + 0xc))(pSlot);
}
// FUNCTION: LEMBALL 0x004147d0
int __fastcall CPlayerLemmingGroup_RemoveLemmingFromGroup(void* pThis, int nUnused, int param_1)
{
	void* pChild;
	((void(__fastcall*)(void*, int)) 0x402879)(pThis, param_1);
	pChild = (void*) ((int(__fastcall*)(void*)) 0x40241e)(pThis);
	if (*(int*) ((char*) pThis + 0x168) == 1 && pChild != 0) {
		((void(__fastcall*)(void*, int)) 0x402667)(pChild, 1);
	}
	*(int*) ((char*) pThis + 0x164) = 1;
	return 1;
}
// FUNCTION: LEMBALL 0x00414080
int __fastcall CPlayerLemmingGroup_GetViewData(void* pThis, int nUnused, int param_1)
{
	int nVtbl = *(int*) pThis;
	int nCount = 0;
	int* pi = (int*) ((int(__fastcall*)(void*)) (*(int*) (nVtbl + 0x114)))(pThis);
	if (pi != 0) {
		void* pGetNext = (void*) *(int*) (nVtbl + 0x118);
		do {
			int nOut = param_1;
			if (pi[0x2e] != 0xc) {
				nOut = param_1 + 0x4c;
				nCount++;
				((void(__fastcall*)(void*, int)) (*(int*) *pi + 0xc))(pi, param_1);
			}
			pi = (int*) ((int(__fastcall*)(void*)) pGetNext)(pThis);
			param_1 = nOut;
		} while (pi != 0);
	}
	return nCount;
}
// FUNCTION: LEMBALL 0x00420650
int __fastcall CEnemy_FacingTarget(void* pThis)
{
	int nOct = ((int(__cdecl*)(int, int, int, int)) 0x401532)(
		*(int*) ((char*) pThis + 0x9c) >> 12,
		*(int*) ((char*) pThis + 0xa0) >> 12,
		*(int*) ((char*) pThis + 0x15c) >> 12,
		*(int*) ((char*) pThis + 0x160) >> 12);
	return (int) *(short*) ((char*) pThis + 0xb4) == nOct;
}
// FUNCTION: LEMBALL 0x0040b9e0
void __fastcall CSlinkyManager_Add(void* pThis, int nUnused, unsigned short param_1, int param_2, int param_3, int param_4, int param_5)
{
	if (*(int*) ((char*) pThis + 0xc) < *(int*) ((char*) pThis + 8)) {
		((void(__fastcall*)(void*, unsigned short)) 0x402293)(
			(void*) (*(int*) ((char*) pThis + 0xc) * 0x150 + *(int*) ((char*) pThis + 4)), param_1);
		((void(__fastcall*)(void*, int, int, int, int)) 0x402892)(
			(void*) (*(int*) ((char*) pThis + 0xc) * 0x150 + *(int*) ((char*) pThis + 4)),
			param_2, param_4, param_3, param_5);
		*(int*) ((char*) pThis + 0xc) = *(int*) ((char*) pThis + 0xc) + 1;
	}
}
// FUNCTION: LEMBALL 0x004148f0
void __fastcall ClearExistingWaypoints(void* pThis)
{
	int nVtbl = *(int*) pThis;
	((void(__fastcall*)(void*)) 0x402ab8)(pThis);
	if (*(int*) ((char*) pThis + 0x170) != 0) {
		if (((int(__fastcall*)(void*)) (*(int*) (nVtbl + 0x13c)))(pThis) == 3) {
			int* pGroup = *(int**) ((char*) pThis + 0x170);
			if (*(int*) ((char*) pGroup + 0x38) != 0 && *(int*) ((char*) pGroup + 0x8c) != 0) {
				if (*(int*) 0x4a011c != 0) {
					((void(__fastcall*)(void*)) (*(int*) *pGroup + 0xf8))(pGroup);
				}
				*(int*) ((char*) pGroup + 0x8c) = 0;
			}
		}
		*(int*) ((char*) pThis + 0x16c) =
			((int(__fastcall*)(void*)) (*(int*) (nVtbl + 0x108)))(pThis);
	}
}
// FUNCTION: LEMBALL 0x0045a180
void* __fastcall ArenaShrink(void* pThis, int nUnused, int param_1)
{
	void* pNode = *(void**) ((char*) pThis + 0x34);
	while (pNode != 0) {
		if (((int(__fastcall*)(void*, void*)) 0x45a200)(pThis, pNode) != 0) {
			if (((int(__fastcall*)(void*, int)) 0x45a580)(pNode, param_1) != 0) {
				return pNode;
			}
		}
		pNode = *(void**) ((char*) pNode + 0x14);
	}
	return 0;
}
// FUNCTION: LEMBALL 0x00420350
void __fastcall CEnemy_TurnToFaceTarget(void* pThis)
{
	int nOct = ((int(__cdecl*)(int, int, int, int)) 0x401532)(
		*(int*) ((char*) pThis + 0x9c) >> 12,
		*(int*) ((char*) pThis + 0xa0) >> 12,
		*(int*) ((char*) pThis + 0x15c) >> 12,
		*(int*) ((char*) pThis + 0x160) >> 12);
	if (nOct != (int) *(short*) ((char*) pThis + 0xb4)) {
		if (*(int*) ((char*) 0x49d020 + (nOct - (int) *(short*) ((char*) pThis + 0xb4) & 7) * 4) < 0) {
			((void(__fastcall*)(void*)) 0x4023e7)(pThis);
		} else {
			((void(__fastcall*)(void*)) 0x402068)(pThis);
		}
	}
	*(int*) ((char*) pThis + 0xcc) = g_nLevelFrameClockTick +
		*(int*) ((char*) 0x49d0b0 + *(int*) ((char*) pThis + 0x64) * 4) / 0x32;
}
// FUNCTION: LEMBALL 0x00421b40
void __fastcall CBall_SetHeightCorrect(void* pThis)
{
	int xq = *(int*) ((char*) pThis + 0x9c) >> 12;
	int yq = *(int*) ((char*) pThis + 0xa0) >> 12;
	int nTileX = *(int*) ((char*) pThis + 0x9c) >> 16;
	int nTileY = *(int*) ((char*) pThis + 0xa0) >> 16;
	unsigned int z = 0;
	if (!(xq < 0 || yq < 0 ||
	      *(int*) ((char*) g_pLevelTileGrid + 0x10) <= nTileX ||
	      *(int*) ((char*) g_pLevelTileGrid + 0x14) <= nTileY)) {
		z = ((unsigned int(__fastcall*)(void*, int, int)) 0x4029a5)(
			(void*) (((nTileY * *(int*) ((char*) g_pLevelTileGrid + 0x10) + nTileX) * 0xc) + *(int*) ((char*) g_pLevelTileGrid + 0xc)),
			xq & 0xf, yq & 0xf);
	}
	*(unsigned int*) ((char*) pThis + 0xa4) = (z & 0xffff) << 12;
}
// FUNCTION: LEMBALL 0x0041cf70
int __fastcall CTower_Process(void* pThis)
{
	int xq = *(int*) ((char*) pThis + 0x9c) >> 12;
	int yq = *(int*) ((char*) pThis + 0xa0) >> 12;
	int nTileX = xq >> 4;
	int nTileY = yq >> 4;
	unsigned int z = 0;
	if (!(xq < 0 || yq < 0 ||
	      *(int*) ((char*) g_pLevelTileGrid + 0x10) <= nTileX ||
	      *(int*) ((char*) g_pLevelTileGrid + 0x14) <= nTileY)) {
		z = ((unsigned int(__fastcall*)(void*, int, int)) 0x4029a5)(
			(void*) (((nTileY * *(int*) ((char*) g_pLevelTileGrid + 0x10) + nTileX) * 0xc) + *(int*) ((char*) g_pLevelTileGrid + 0xc)),
			xq & 0xf, yq & 0xf);
	}
	*(unsigned int*) ((char*) pThis + 0xa4) = (z & 0xffff) << 12;
	return 1;
}
// FUNCTION: LEMBALL 0x0040b4d0
void __fastcall CSlinky_Set(void* pThis, int nUnused, int param_1, int param_2, int param_3, int param_4)
{
	*(int*) ((char*) pThis + 0x124) = param_1;
	*(int*) ((char*) pThis + 0x128) = param_3;
	*(int*) ((char*) pThis + 0x12c) = param_2;
	*(int*) ((char*) pThis + 0x130) = param_4;
	unsigned int z = 0;
	if (param_1 >= 0 && param_3 >= 0 &&
	    (param_1 >> 4) < *(int*) ((char*) g_pLevelTileGrid + 0x10) &&
	    (param_3 >> 4) < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
		z = ((unsigned int(__fastcall*)(void*, int, int)) 0x4029a5)(
			(void*) ((((param_3 >> 4) * *(int*) ((char*) g_pLevelTileGrid + 0x10) + (param_1 >> 4)) * 0xc) + *(int*) ((char*) g_pLevelTileGrid + 0xc)),
			param_1 & 0xf, param_3 & 0xf);
	}
	*(unsigned int*) ((char*) pThis + 0xa4) = (z & 0xffff) << 12;
	*(int*) ((char*) pThis + 0x9c) = *(int*) ((char*) pThis + 0x124) << 12;
	*(int*) ((char*) pThis + 0xa0) = *(int*) ((char*) pThis + 0x128) << 12;
	*(int*) ((char*) pThis + 0xcc) = g_nLevelFrameClockTick;
	*(int*) ((char*) pThis + 0x94) = g_nLevelFrameClockTimeMs;
	((void(__fastcall*)(void*, int)) (*(void***) pThis)[2])(pThis, 0x18);
}
// FUNCTION: LEMBALL 0x00417150
void __cdecl CGlobalGameObjectDeleteMessages(void)
{
	void* p;
	p = *(void**) 0x49d130;
	if (p != 0) {
		((void(__fastcall*)(void*, int)) (*(void***) p)[5])(p, 1);
	}
	p = *(void**) 0x49d12c;
	if (p != 0) {
		((void(__fastcall*)(void*, int)) (*(void***) p)[5])(p, 1);
	}
	p = *(void**) 0x49d128;
	if (p != 0) {
		((void(__fastcall*)(void*, int)) (*(void***) p)[5])(p, 1);
	}
	p = *(void**) 0x49d110;
	if (p != 0) {
		((void(__fastcall*)(void*, int)) (*(void***) p)[5])(p, 1);
	}
	p = *(void**) 0x49d114;
	if (p != 0) {
		((void(__fastcall*)(void*, int)) (*(void***) p)[5])(p, 1);
	}
	p = *(void**) 0x49d118;
	if (p != 0) {
		((void(__fastcall*)(void*, int)) (*(void***) p)[5])(p, 1);
	}
	p = *(void**) 0x49d11c;
	if (p != 0) {
		((void(__fastcall*)(void*, int)) (*(void***) p)[5])(p, 1);
	}
	p = *(void**) 0x49d120;
	if (p != 0) {
		((void(__fastcall*)(void*, int)) (*(void***) p)[5])(p, 1);
	}
	p = *(void**) 0x49d124;
	if (p != 0) {
		((void(__fastcall*)(void*, int)) (*(void***) p)[5])(p, 1);
	}
}
// FUNCTION: LEMBALL 0x00418540
void __fastcall CPlayerLemmingGroupManager_dtor(void* pThis, int nUnused)
{
	*(int*) pThis = 0x494038;
	void* pSub = ((char*) pThis) - 0xb0;
	*(int*) pSub = 0x494068;
	for (int i = 0; i < *(int*) ((char*) pThis + 0x78); i++) {
		void* p = *(void**) ((char*) pThis + 0x7c + i * 4);
		if (p != 0) {
			((void(__fastcall*)(void*, int)) *(int*) p)(p, 1);
		}
	}
	if (*(int*) ((char*) pThis + 0x9c) != 0) {
		for (int i = 0; i < 4; i++) {
			void* p = *(void**) ((char*) pThis + 0x8c + i * 4);
			if (p != 0) {
				((void(__fastcall*)(void*, int)) *(int*) p)(p, 1);
			}
		}
	}
	((void(__fastcall*)(void*)) 0x45eea0)(((unsigned int) pSub >= 1) ? pThis : 0);
	((void(__fastcall*)(void*)) 0x402de7)(pSub);
}
// FUNCTION: LEMBALL 0x004131e0
void __fastcall CAI_SetNetworkTrapDoors(void* pThis, int nUnused, int param_1, int param_2, int param_3, int param_4, int param_5)
{
	*(int*) ((char*) pThis + 0xd0) = param_1;
	if (param_2 == -1) {
		*(int*) ((char*) pThis + 0xc0) = *(int*) (0x49cf50 + (param_1 << 4));
		*(int*) ((char*) pThis + 0xc4) = *(int*) (0x49cf54 + (param_1 << 4));
		*(int*) ((char*) pThis + 0xc8) = *(int*) (0x49cf58 + (param_1 << 4));
		*(int*) ((char*) pThis + 0xcc) = *(int*) (0x49cf5c + (param_1 << 4));
	} else {
		*(int*) ((char*) pThis + 0xc0) = param_2;
		*(int*) ((char*) pThis + 0xc4) = param_3;
		*(int*) ((char*) pThis + 0xc8) = param_4;
		*(int*) ((char*) pThis + 0xcc) = param_5;
	}
	if (param_1 > 0) {
		int* pi = (int*) ((char*) pThis + 0x90);
		int v = 0;
		int count = param_1;
		do {
			if (pi[0] > 0x400 || pi[0] < 0) {
				pi[0] = v;
			}
			if (pi[4] > 0x400 || pi[4] < 0) {
				pi[4] = v;
			}
			pi++;
			v += 0x10;
		} while (--count != 0);
	}
}
// FUNCTION: LEMBALL 0x0040f220
void __fastcall CPlayerLemming_TurnToFaceTarget(void* pThis, int nUnused)
{
	int nOct = ((int(__cdecl*)(int, int, int, int)) 0x401532)(
		*(int*) ((char*) pThis + 0x9c) >> 12,
		*(int*) ((char*) pThis + 0xa0) >> 12,
		*(int*) ((char*) pThis + 0x1b4) >> 12,
		*(int*) ((char*) pThis + 0x1b8) >> 12);
	if (nOct != (int) *(short*) ((char*) pThis + 0xb4)) {
		if (*(int*) ((char*) 0x49d020 + (nOct - (int) *(short*) ((char*) pThis + 0xb4) & 7) * 4) < 0) {
			((void(__fastcall*)(void*)) 0x4023e7)(pThis);
		} else {
			((void(__fastcall*)(void*)) 0x402068)(pThis);
		}
		(*( void(**)(void*, int)) (*(void***) pThis + 0x84 / 4))(pThis, 0xfa0);
	}
	*(int*) ((char*) pThis + 0xcc) = g_nLevelFrameClockTick +
		*(int*) ((char*) 0x49d0b0 + *(int*) ((char*) pThis + 0x64) * 4) / 0x32;
}
// FUNCTION: LEMBALL 0x0040f160
void __fastcall CPlayerLemming_TurnToFaceCursor(void* pThis, int nUnused)
{
	if (*(int*) ((char*) g_pLevelDemoPlaybackController + 0x4c) == 0) {
		int local_4 = 0;
		int local_8 = 0;
		((void(__fastcall*)(void*, int*, int*)) 0x401e65)(
			*(void**) ((char*) g_pActiveManagedEntityOwner + 0x160), &local_4, &local_8);
		int nOct = ((int(__cdecl*)(int, int, int, int)) 0x401532)(
			*(int*) ((char*) pThis + 0x9c) >> 12,
			*(int*) ((char*) pThis + 0xa0) >> 12,
			local_4, local_8);
		if (nOct != (int) *(short*) ((char*) pThis + 0xb4)) {
			if (*(int*) ((char*) 0x49d020 + (nOct - (int) *(short*) ((char*) pThis + 0xb4) & 7) * 4) < 0) {
				((void(__fastcall*)(void*)) 0x4023e7)(pThis);
			} else {
				((void(__fastcall*)(void*)) 0x402068)(pThis);
			}
			(*( void(**)(void*, int)) (*(void***) pThis + 0x84 / 4))(pThis, 0xfa0);
		}
		*(int*) ((char*) pThis + 0xcc) = g_nLevelFrameClockTick +
			*(int*) ((char*) 0x49d070 + *(int*) ((char*) pThis + 0x64) * 4) / 0x32;
	}
}
// FUNCTION: LEMBALL 0x0040b670
void __fastcall CSlinky_Move(void* pThis, int nUnused)
{
	int* pState = *(int**) 0x4a1bcc;
	int v = (*pState * 45 + 31) & 0x7fffff;
	*pState = v;
	int heading = v % 4;
	*(short*) ((char*) pThis + 0xbc) = (short) heading;
	int dx, dy;
	switch (heading) {
	case 0:
		dx = 0x10; dy = 0;
		break;
	case 1:
		dx = -0x10; dy = 0;
		break;
	case 2:
		dx = 0; dy = 0x10;
		break;
	default:
		dx = 0; dy = -0x10;
		break;
	}
	int i = 0;
	do {
		i++;
		*(int*) ((char*) pThis + 0xa8) = ((*(int*) ((char*) pThis + 0x9c) >> 12) + dx) << 12;
		*(int*) ((char*) pThis + 0xac) = ((*(int*) ((char*) pThis + 0xa0) >> 12) + dy) << 12;
		*(int*) ((char*) pThis + 0xb0) = (*(int*) ((char*) pThis + 0xa4) >> 12) << 12;
	} while (i < 8 && ((int(__fastcall*)(void*, int*)) 0x403229)(pThis, (int*) ((char*) pThis + 0xa8)) == 0);
}
// FUNCTION: LEMBALL 0x00412660
int __fastcall CAI_ProcessMsg(void* pThis, int nUnused, short* param_1)
{
	if (param_1[0] == 4) {
		((void(__fastcall*)(void*, int, int)) 0x402d74)(
			*(void**) ((char*) pThis + 0x15c), *(int*) (param_1 + 4), *(int*) (param_1 + 6));
		return 0;
	}
	if (*(int*) ((char*) pThis + 0x58) == 0) {
		return 1;
	}
	switch (param_1[0]) {
	case 2:
		((void(__fastcall*)(void*, int, int)) 0x40201d)(
			*(void**) ((char*) pThis + 0x15c), *(int*) (param_1 + 4), *(int*) (param_1 + 6));
		return 0;
	case 3:
		((void(__fastcall*)(void*)) 0x40121c)(*(void**) ((char*) pThis + 0x15c));
		return 0;
	case 6:
		((void(__fastcall*)(void*)) 0x402d47)(*(void**) ((char*) pThis + 0x15c));
		return 0;
	case 7:
		((void(__fastcall*)(void*)) 0x402509)(*(void**) ((char*) pThis + 0x15c));
		return 0;
	case 8:
		((void(__fastcall*)(void*, int)) 0x40378d)(
			*(void**) ((char*) pThis + 0x15c), *(int*) (param_1 + 4));
		return 0;
	case 5:
		(*( void(**)(void*, int, int)) (*(void***) *(void**) ((char*) pThis + 0x15c) + 0x34 / 4))(
			*(void**) ((char*) pThis + 0x15c), *(int*) (param_1 + 4), *(int*) (param_1 + 6));
		return 0;
	default:
		*(int*) ((char*) pThis + 0xc) = *(int*) ((char*) pThis + 0xc) + 1;
		return 0;
	}
}
// FUNCTION: LEMBALL 0x0040f640
void __fastcall CPlayerLemming_GetData(void* pThis, int nUnused)
{
	unsigned short local_e[2];
	*(unsigned int*) ((char*) pThis - 0x9c) = (unsigned int) (unsigned short) ((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis) << 12;
	*(unsigned int*) ((char*) pThis - 0x98) = (unsigned int) (unsigned short) ((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis) << 12;
	*(unsigned int*) ((char*) pThis - 0x94) = (unsigned int) (unsigned short) ((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis) << 12;
	((void(__fastcall*) (void*, unsigned short*)) 0x45f090)(pThis, local_e);
	*(unsigned short*) ((char*) pThis - 0x84) = (unsigned short) (local_e[0] & 7);
	*(unsigned short*) ((char*) pThis - 0x7c) = (unsigned short) ((local_e[0] & 0x38) >> 3);
	((void(__fastcall*) (void*, unsigned short*)) 0x45f090)(pThis, local_e);
	*(unsigned int*) ((char*) pThis - 0x80) = (unsigned int) (local_e[0] & 0xff);
	*(unsigned int*) ((char*) pThis - 0xa0) = (unsigned int) (local_e[0] >> 8);
	*(unsigned int*) ((char*) pThis - 0xa4) = ((unsigned int(__fastcall*) (void*)) 0x45eff0)(pThis);
}
// FUNCTION: LEMBALL 0x0040f6f0
void __fastcall CPlayerLemming_AddData(void* pThis, int nUnused)
{
	((void(__fastcall*) (void*, unsigned short)) 0x45ef40)(pThis, 0x2c);
	((void(__fastcall*) (void*, unsigned char)) 0x45ef60)(pThis, *(unsigned short*) ((char*) pThis + 0x30));
	((void(__fastcall*) (void*, unsigned short)) 0x45ef40)(pThis, *(int*) ((char*) pThis - 0x9c) >> 12);
	((void(__fastcall*) (void*, unsigned short)) 0x45ef40)(pThis, *(int*) ((char*) pThis - 0x98) >> 12);
	((void(__fastcall*) (void*, unsigned short)) 0x45ef40)(pThis, *(int*) ((char*) pThis - 0x94) >> 12);
	((void(__fastcall*) (void*, unsigned short)) 0x45ef40)(pThis, (unsigned short) (((*(unsigned short*) ((char*) pThis - 0x7c) & 7) << 3) | (*(unsigned short*) ((char*) pThis - 0x84) & 7)));
	((void(__fastcall*) (void*, unsigned short)) 0x45ef40)(pThis, (unsigned short) ((*(unsigned char*) ((char*) pThis - 0xa0) << 8) | *(unsigned char*) ((char*) pThis - 0x80)));
	if (*(unsigned int*) ((char*) pThis - 0x5c) > (unsigned int) g_nLevelFrameClockTimeMs) {
		*(unsigned int*) ((char*) pThis - 0x5c) = g_nLevelFrameClockTimeMs;
	}
	((void(__fastcall*) (void*, unsigned int)) 0x45ef10)(pThis, *(unsigned int*) ((char*) pThis - 0x5c));
	*(unsigned int*) ((char*) pThis + 0x2c) = 0;
}
// FUNCTION: LEMBALL 0x0041a6d0
void __fastcall CBullet_TriggerBullet(void* pThis, int nUnused)
{
	int aPos[3] = {
		*(int*) ((char*) pThis + 0x9c) >> 12,
		*(int*) ((char*) pThis + 0xa0) >> 12,
		*(int*) ((char*) pThis + 0xa4) >> 12
	};
	int aDest[3] = {
		*(int*) ((char*) pThis + 0xa8) >> 12,
		*(int*) ((char*) pThis + 0xac) >> 12,
		*(int*) ((char*) pThis + 0xb0) >> 12
	};
	((void(__fastcall*) (void*, int*, int*, int, int)) 0x4027de)((char*) pThis + 0x184, aPos, aDest, *(int*) ((char*) pThis + 0xc8), 0xc);
	*(int*) ((char*) pThis + 0xb8) = 0x1b;
	*(int*) ((char*) pThis + 0xcc) = *(int*) ((char*) pThis + 0xc8) + 10;
}
// FUNCTION: LEMBALL 0x00416130
void __fastcall CGameObject_Jump(void* pThis, int nUnused)
{
	if (*(unsigned short*) ((char*) pThis + 0xbc) != 0) {
		return;
	}
	void* pMoveChunk = 0;
	int nTick = g_nLevelFrameClockTick - *(int*) ((char*) pThis + 0xc8);
	unsigned int nHeight = ((LevelTileGridOwnerView*) g_pLevelTileGrid)->GetZ(
		*(int*) ((char*) pThis + 0xf4) >> 12, *(int*) ((char*) pThis + 0xf8) >> 12, &pMoveChunk);
	nHeight &= 0xffff;
	int nFall = (nTick * 3 + *(int*) ((char*) pThis + 0x100)) << 12;
	*(int*) ((char*) pThis + 0xa4) = nFall;
	if ((int) (nHeight << 12) <= nFall) {
		*(int*) ((char*) pThis + 0xa4) = nHeight << 12;
		*(int*) ((char*) pThis + 0x9c) = *(int*) ((char*) pThis + 0xf4);
		*(int*) ((char*) pThis + 0xa0) = *(int*) ((char*) pThis + 0xf8);
		*(int*) ((char*) pThis + 0x104) = 0;
		if (*(int*) ((char*) pThis + 0x11c) == 0 && pMoveChunk != 0) {
			if (((int(__fastcall*) (void*, void*)) 0x4036b1)(pMoveChunk, pThis) == 0) {
				((void(__fastcall*) (void*, void*, void*, unsigned short)) 0x40341d)(g_pActiveManagedEntityOwner, (char*) pThis + 0x9c, pThis, *(unsigned short*) ((char*) pThis + 0x68));
				return;
			}
		}
		((void(__fastcall*) (void*, void*, void*, unsigned short)) 0x40341d)(g_pActiveManagedEntityOwner, (char*) pThis + 0x9c, pThis, *(unsigned short*) ((char*) pThis + 0x68));
	}
}
// FUNCTION: LEMBALL 0x00411b70
void __fastcall CAI_SendGameState(void* pThis, int nUnused, int param_1, int param_2)
{
	if (g_nSelectedNetworkLobbyPeerId == 0) {
		return;
	}
	if (*(int*) (*(int*) ((char*) pThis + 0x74) + 0x28) != 0) {
		unsigned long start = timeGetTime();
		while (*(int*) (*(int*) ((char*) pThis + 0x74) + 0x28) != 0 &&
		       timeGetTime() - start < 2000) {
			(*(void(**)(void*)) (*(int*) *(int*) g_pActiveNetworkRuntimeWindow + 0x30))(*(void**) g_pActiveNetworkRuntimeWindow);
		}
	}
	if (*(int*) (*(int*) ((char*) pThis + 0x74) + 0x28) == 0) {
		*(int*) ((char*) pThis + 0x6c) = 1;
		*(int*) (*(int*) ((char*) pThis + 0x74) + 0x2c) = param_1;
		*(int*) (*(int*) ((char*) pThis + 0x74) + 0x30) = param_2;
		*(int*) (*(int*) ((char*) pThis + 0x74) + 0x34) = *(int*) ((char*) pThis + 0xe8);
		*(int*) (*(int*) ((char*) pThis + 0x74) + 0x38) = *(int*) ((char*) pThis + 0xf0);
		((void(__fastcall*) (void*, int)) 0x45f2b0)(*(void**) ((char*) pThis + 0x74), g_nSelectedNetworkLobbyPeerId);
	}
}
// FUNCTION: LEMBALL 0x00412ad0
int __fastcall CAI_OpenDoor(void* pThis, int nUnused, int* param_1, int* param_2, unsigned char param_3)
{
	int tileX = ((param_1[0] >> 12) + ((param_1[0] >> 12) >> 31 & 0xf)) >> 4;
	int tileY = ((param_1[1] >> 12) + ((param_1[1] >> 12) >> 31 & 0xf)) >> 4;
	unsigned short tile = 0x3;
	if (tileX >= 0 && tileY >= 0) {
		void* grid = *(void**) ((char*) pThis + 0x110);
		if (tileX < *(int*) ((char*) grid + 0x10) && tileY < *(int*) ((char*) grid + 0x14)) {
			tile = *(unsigned short*) (*(int*) ((char*) grid + 0xc) + 6 + (*(int*) ((char*) grid + 0x10) * tileY + tileX) * 0xc);
		}
	}
	if ((tile & 0x8000) && (param_3 & 0x20)) {
		return ((int(__fastcall*) (void*, int, void*, void*)) 0x401fa5)(*(void**) ((char*) pThis + 0x190), 0, param_1, param_2);
	}
	return 0;
}
// Minimal view of the LEVELVT runtime-state resetter so Restart can emit
// a direct `call LevelChunkObjectRuntimeStateView::ResetRuntimeStateThunk` (orig ILT 0x40209f).
struct LevelChunkObjectRuntimeStateView {
	void ResetRuntimeStateThunk(void);
};
// FUNCTION: LEMBALL 0x0041fcd0
void __fastcall CEnemy_Restart(void* pThis, int nUnused)
{
	((LevelChunkObjectRuntimeStateView*) pThis)->ResetRuntimeStateThunk();
	*(int*) ((char*) pThis + 0x9c) = *(int*) ((char*) pThis + 0x40);
	*(int*) ((char*) pThis + 0xa0) = *(int*) ((char*) pThis + 0x44);
	*(int*) ((char*) pThis + 0xa4) = *(int*) ((char*) pThis + 0x48);
	*(unsigned short*) ((char*) pThis + 0xb4) = *(unsigned short*) ((char*) pThis + 0xb6);
	*(int*) ((char*) pThis + 0x128) = 0;
	*(int*) ((char*) pThis + 0x168) = 0;
	*(int*) ((char*) pThis + 0x124) = 0;
	*(int*) ((char*) pThis + 0x2c) = 0;
	void** ppReg = *(void***) ((char*) g_pActiveManagedEntityOwner + 0x120);
	ppReg[*(int*) ((char*) g_pActiveManagedEntityOwner + 0x118)] = pThis;
	*(int*) ((char*) g_pActiveManagedEntityOwner + 0x118) += 1;
	if (*(int*) ((char*) pThis + 0x134) != 0) {
		*(int*) (*(int*) ((char*) pThis + 0x134) + 0x8) = 0;
		*(int*) (*(int*) ((char*) pThis + 0x134) + 0xc) = 1;
	}
	if (*(int*) ((char*) pThis + 0x140) != 0) {
		*(int*) (*(int*) ((char*) pThis + 0x140) + 0x8) = 0;
		*(int*) (*(int*) ((char*) pThis + 0x140) + 0xc) = 1;
	}
	if (*(int*) ((char*) pThis + 0x14c) != 0) {
		*(int*) (*(int*) ((char*) pThis + 0x14c) + 0x8) = 0;
		*(int*) (*(int*) ((char*) pThis + 0x14c) + 0xc) = 1;
	}
}
// FUNCTION: LEMBALL 0x0041aaa0
void __fastcall CBullet_AddData(void* pThis, int nUnused)
{
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, 0x2b);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(unsigned short*) ((char*) pThis - 0xcc));
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)(pThis, nUnused, g_nLevelFrameClockTimeMs);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(int*) ((char*) pThis - 0x9c) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(int*) ((char*) pThis - 0x98) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(int*) ((char*) pThis - 0x94) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(int*) ((char*) pThis - 0x90) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(int*) ((char*) pThis - 0x8c) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(int*) ((char*) pThis - 0x88) >> 12);
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(unsigned short*) ((char*) pThis - 0x84));
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)(pThis, nUnused, *(unsigned int*) ((char*) pThis - 0xa0));
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)(pThis, nUnused, *(unsigned int*) ((char*) pThis - 0x70));
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)(pThis, nUnused, *(unsigned int*) ((char*) pThis + 0x30));
	((void(__fastcall*) (void*, int, unsigned int)) 0x45ef10)(pThis, nUnused, *(unsigned int*) ((char*) pThis + 0x34));
	((void(__fastcall*) (void*, int, unsigned short)) 0x45ef40)(pThis, nUnused, *(unsigned short*) ((char*) pThis + 0x38));
}
// FUNCTION: LEMBALL 0x0041ab80
void __fastcall CBullet_GetData(void* pThis, int nUnused)
{
	((void(__cdecl*) (unsigned int)) 0x403107)(((unsigned int(__fastcall*) (void*)) 0x45eff0)(pThis));
	*(unsigned int*) ((char*) pThis - 0x9c) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis)) << 12;
	*(unsigned int*) ((char*) pThis - 0x98) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis)) << 12;
	*(unsigned int*) ((char*) pThis - 0x94) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis)) << 12;
	*(unsigned int*) ((char*) pThis - 0x90) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis)) << 12;
	*(unsigned int*) ((char*) pThis - 0x8c) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis)) << 12;
	*(unsigned int*) ((char*) pThis - 0x88) = (unsigned int) (unsigned short) (((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis)) << 12;
	*(unsigned short*) ((char*) pThis - 0x84) = ((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis);
	*(unsigned int*) ((char*) pThis - 0xa0) = ((unsigned int(__fastcall*) (void*)) 0x45eff0)(pThis);
	*(unsigned int*) ((char*) pThis - 0x70) = ((unsigned int(__fastcall*) (void*)) 0x45eff0)(pThis);
	*(unsigned int*) ((char*) pThis + 0x30) = ((unsigned int(__fastcall*) (void*)) 0x45eff0)(pThis);
	unsigned int nData = ((unsigned int(__fastcall*) (void*)) 0x45eff0)(pThis);
	*(unsigned int*) ((char*) pThis + 0x34) = nData;
	if (nData == 0) {
		*(unsigned int*) ((char*) pThis + 0x34) = 2;
	}
	*(unsigned short*) ((char*) pThis + 0x38) = ((unsigned short(__fastcall*) (void*)) 0x45f070)(pThis);
	*(int*) ((char*) pThis + 0x2c) = 1;
	*(int*) ((char*) pThis - 0x24) = 1;
}
// FUNCTION: LEMBALL 0x004216c0
void __fastcall CBall_Set(void* pObject, int nUnused, int param_1, int param_2, int param_3, int param_4, int param_5, int param_6, int param_7)
{
	*(int*) ((char*) pObject + 0x9c) = param_1;
	*(int*) ((char*) pObject + 0xa0) = param_2;
	*(int*) ((char*) pObject + 0xa4) = param_3;
	*(int*) ((char*) pObject + 0x40) = param_1;
	*(int*) ((char*) pObject + 0x44) = param_2;
	*(int*) ((char*) pObject + 0x48) = param_3;
	*(int*) ((char*) pObject + 0x124) = param_4;
	*(int*) ((char*) pObject + 0x128) = param_5;
	*(int*) ((char*) pObject + 0x12c) = param_6;
	*(int*) ((char*) pObject + 0xb8) = 0x25;
	*(short*) ((char*) pObject + 0xbc) = 0;
	if (*(unsigned short*) ((char*) g_pActiveManagedEntityOwner + 0x54) < 7) {
		*(short*) ((char*) pObject + 0x130) = (short) *(int*) (*(int*) ((char*) pObject + 0x64) * 4 + 0x49d070);
	} else {
		*(short*) ((char*) pObject + 0x130) = (short) param_7;
	}
	if (*(unsigned short*) ((char*) pObject + 0x130) > 1) {
		*(int*) ((char*) pObject + 0x134) = 1;
	} else {
		*(short*) ((char*) pObject + 0x130) = (short) *(int*) (*(int*) ((char*) pObject + 0x64) * 4 + 0x49d070);
		*(int*) ((char*) pObject + 0x134) = 1;
	}
	*(int*) ((char*) pObject + 0x134) = 1;
}
// FUNCTION: LEMBALL 0x00426840
void __fastcall CRocket_Set(void* pObject, int nUnused, unsigned short slotId, int* pVec)
{
	((void(__fastcall*) (void*, unsigned short)) 0x402293)(pObject, slotId);
	*(int*) ((char*) pObject + 0x9c) = pVec[0];
	*(int*) ((char*) pObject + 0xa0) = pVec[1];
	*(int*) ((char*) pObject + 0x13c) = 1;
	*(int*) ((char*) pObject + 0xb8) = 0x18;
	*(int*) ((char*) pObject + 0xa4) = pVec[2];
	int xtile = (pVec[0] >> 12) / 16;
	if (xtile < 0) return;
	int ytile = (pVec[1] >> 12) / 16;
	if (ytile < 0) return;
	void* pGrid = *(void**) 0x4a74b4;
	if (*(int*) ((char*) pGrid + 0x10) <= xtile) return;
	if (*(int*) ((char*) pGrid + 0x14) <= ytile) return;
	int nIndex = *(int*) ((char*) pGrid + 0x10) * ytile + xtile;
	*(char*) (*(int*) ((char*) pGrid + 0xc) + nIndex * 12 + 7) |= (char) 0x80;
}
// FUNCTION: LEMBALL 0x004269d0
int __fastcall CRocket_StepOn(void* pObject, int nUnused, int* pOther, void* param_2)
{
	int nDist = ((int(__cdecl*) (int, int, int, int)) 0x40254a)(*(int*) ((char*) pObject + 0x9c) >> 12, *(int*) ((char*) pObject + 0xa0) >> 12, pOther[0] >> 12, pOther[1] >> 12);
	if (nDist < 0x20) {
		*(int*) ((char*) pObject + 0x9c) = pOther[0] + 0x4000;
		*(int*) ((char*) pObject + 0xa0) = pOther[1] + 0x4000;
		*(int*) ((char*) pObject + 0xa4) = pOther[2];
		*(int*) ((char*) pObject + 0x5c) = (int) param_2;
		*(int*) ((char*) pObject + 0x140) = pOther[2] >> 12;
		*(int*) ((char*) pObject + 0xc8) = 0x30;
		((void(__fastcall*) (void*, int)) 0x401f3c)(pObject, 0x1b);
		return 1;
	}
	return 0;
}
// FUNCTION: LEMBALL 0x00426a60
void __fastcall CRocket_DoActivate(void* pObject, int nUnused)
{
	*(int*) ((char*) pObject + 0xc8) += g_nLevelFrameClockTick;
	*(int*) ((char*) pObject + 0x94) = g_nLevelFrameClockTimeMs;
	void* pTarget = *(void**) ((char*) pObject + 0x5c);
	((void(__fastcall*) (void*, int)) (*(void***) pTarget + 0x8 / 4))(pTarget, 0x15);
	*(int*) ((char*) pTarget + 0xcc) = g_nLevelFrameClockTick + 0x3c;
	((void(__fastcall*) (void*, int)) (*(void***) pObject + 0x34 / 4))(pObject, 0x12);
	if (*(int*) 0x4a011c != 0) {
		((void(__fastcall*) (void*, void*)) 0x4032dd)(*(void**) 0x49d128, pObject);
	}
}
// FUNCTION: LEMBALL 0x00427630
void __fastcall CDuplicator_Set(void* pObject, int nUnused, int* pPos)
{
	int iVar3 = *pPos;
	*(int*) ((char*) pObject + 0x9c) = iVar3;
	int iVar4 = pPos[1];
	*(int*) ((char*) pObject + 0xa0) = iVar4;
	int iVar2 = pPos[2];
	*(int*) ((char*) pObject + 0x138) = 1;
	*(int*) ((char*) pObject + 0x13c) = 1;
	*(int*) ((char*) pObject + 0xa4) = iVar2;
	iVar3 = (iVar3 >> 12) / 16;
	iVar4 = (iVar4 >> 12) / 16;
	if (iVar3 > -1) {
		if (iVar4 > -1 && iVar3 < *(int*) ((char*) g_pLevelTileGrid + 0x10) && iVar4 < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
			*(unsigned char*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (iVar4 * *(int*) ((char*) g_pLevelTileGrid + 0x10) + iVar3) * 0xc) |= 1;
		}
		iVar4--;
		if (iVar4 > -1) {
			if (iVar3 < *(int*) ((char*) g_pLevelTileGrid + 0x10) && iVar4 < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
				*(unsigned char*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (*(int*) ((char*) g_pLevelTileGrid + 0x10) * iVar4 + iVar3) * 0xc) |= 1;
			}
		}
	}
}
// FUNCTION: LEMBALL 0x004276f0
void __fastcall CDuplicator_Delete(void* pObject, int nUnused)
{
	int iVar2 = (*(int*) ((char*) pObject + 0x9c) >> 12) / 16;
	int iVar3 = (*(int*) ((char*) pObject + 0xa0) >> 12) / 16;
	if (iVar2 > -1) {
		if (iVar3 > -1 && iVar2 < *(int*) ((char*) g_pLevelTileGrid + 0x10) && iVar3 < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
			*(unsigned short*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (*(int*) ((char*) g_pLevelTileGrid + 0x10) * iVar3 + iVar2) * 0xc) &= 0xfffe;
		}
		iVar3--;
		if (iVar3 > -1) {
			if (iVar2 < *(int*) ((char*) g_pLevelTileGrid + 0x10) && iVar3 < *(int*) ((char*) g_pLevelTileGrid + 0x14)) {
				*(unsigned short*) (*(int*) ((char*) g_pLevelTileGrid + 0xc) + 6 + (*(int*) ((char*) g_pLevelTileGrid + 0x10) * iVar3 + iVar2) * 0xc) &= 0xfffe;
			}
		}
	}
}
// FUNCTION: LEMBALL 0x004202a0
int __fastcall CEnemy_LineOfSight(void* pObject, int nUnused, int param_1, int param_2)
{
	unsigned int local_8 = param_1 - *(int*) ((char*) pObject + 0x9c);
	unsigned int uVar4 = param_2 - *(int*) ((char*) pObject + 0xa0);
	unsigned int local_4;
	unsigned int* puVar1;
	unsigned int* puVar2;
	if ((int) local_8 < 0) { local_4 = -(int) local_8; puVar1 = &local_4; }
	else { puVar1 = &local_8; }
	if ((int) uVar4 < 0) { puVar2 = &local_4; local_4 = -(int) uVar4; }
	else { puVar2 = &local_8; local_8 = uVar4; }
	int iVar3 = (int) *puVar2 >> 12;
	unsigned int uVar5 = *puVar2 & 0xfff;
	int iVar5 = (int) ((uVar5 * 0x6a0u) >> 12);
	if (iVar3 * 0x6a0 + iVar5 < (int) *puVar1 && (int) *puVar1 < iVar5 + (iVar3 * 0x1350 + (int) uVar5) * 2) {
		return 1;
	}
	return 0;
}
// FUNCTION: LEMBALL 0x0040be50
void __fastcall CSlinkyManager_LoadLevel(void* pThis, int nUnused, unsigned short* pStream)
{
	unsigned short nCount = *pStream;
	pStream++;
	((void(__fastcall*) (void*, int, int)) 0x403396)(pThis, 0, nCount);
	if (nCount != 0) {
		do {
			unsigned short uSlot;
			if (*(unsigned short*) (*(int*) ((char*) pThis) + 0x54) < 2) {
				uSlot = (unsigned short) ((int(__fastcall*) ()) 0x40214e)();
			}
			else {
				uSlot = *pStream;
				pStream++;
			}
			unsigned int word1 = *(pStream);
			pStream++;
			unsigned int word2 = *(pStream);
			pStream++;
			unsigned int word3 = *(pStream);
			pStream++;
			unsigned int word4 = *(pStream);
			pStream++;
			((void(__fastcall*) (void*, int, int, int, int, int, int)) 0x402c6b)(pThis, 0, uSlot, word1, word2, word3, word4);
			nCount--;
		} while (nCount != 0);
	}
}
// FUNCTION: LEMBALL 0x00422790
void __fastcall CCollectableManager_LoadLevel(void* pThis, int nUnused, unsigned short* pStream, int param_3)
{
	unsigned short nCount = *pStream;
	pStream++;
	if (param_3 == 0) {
		((void(__fastcall*) (void*, int, int)) 0x40187f)(pThis, 0, nCount);
	}
	if (nCount != 0) {
		do {
			unsigned short uSlot;
			if (*(unsigned short*) (*(int*) ((char*) pThis + 0x30) + 0x54) < 2) {
				uSlot = (unsigned short) ((int(__fastcall*) ()) 0x40214e)();
			}
			else {
				uSlot = *pStream;
				pStream++;
			}
			unsigned int subtype = *(pStream);
			pStream++;
			unsigned int coord2 = *(pStream);
			pStream++;
			unsigned int coord3 = *(pStream);
			pStream++;
			unsigned int coord4 = *(pStream);
			pStream++;
			int appendType = (int) subtype;
			if (param_3 == 0) {
				void* pBase = *(void**) ((char*) pThis + 0x30);
				if ((subtype == 0xc || subtype == 0xb) && *(int*) ((char*) pBase + 0x64) == 1) {
					if (*(int*) ((char*) pBase + 0x7c) == 1) {
						if (subtype == 0xc) appendType = 0xc; else appendType = 0xb;
					}
					else {
						if (subtype == 0xc) appendType = 0xb; else appendType = 0xc;
					}
				}
			}
			((void(__fastcall*) (void*, int, int, int, int, int, int)) 0x401afa)(*(void**) ((char*) pThis + 0x30), 0, uSlot, coord2, coord3, coord4, appendType);
			nCount--;
		} while (nCount != 0);
	}
}
// FUNCTION: LEMBALL 0x0042a0b0
unsigned int __fastcall CBalloonPostFindPost(void* pThis, int nUnused, unsigned int subtype, void** pOut)
{
	switch (subtype) {
	case 0x28:
		pOut[0] = *(void**) ((char*) pThis + 4);
		pOut[1] = *(void**) ((char*) pThis + 8);
		pOut[2] = *(void**) ((char*) pThis + 0xc);
		return *(unsigned short*) pThis & 1;
	case 0x2a:
		pOut[0] = *(void**) ((char*) pThis + 0x10);
		pOut[1] = *(void**) ((char*) pThis + 0x14);
		pOut[2] = *(void**) ((char*) pThis + 0x18);
		return *(unsigned short*) pThis & 2;
	case 0x2c:
		pOut[0] = *(void**) ((char*) pThis + 0x1c);
		pOut[1] = *(void**) ((char*) pThis + 0x20);
		pOut[2] = *(void**) ((char*) pThis + 0x24);
		return *(unsigned short*) pThis & 4;
	case 0x2e:
		pOut[0] = *(void**) ((char*) pThis + 0x28);
		pOut[1] = *(void**) ((char*) pThis + 0x2c);
		pOut[2] = *(void**) ((char*) pThis + 0x30);
		return *(unsigned short*) pThis & 8;
	}
	return 0;
}
// FUNCTION: LEMBALL 0x0042a4e0
void __fastcall CBalloonPost_LoadLevel(void* pThis, int nUnused, unsigned short* pStream, int param_4, int param_5)
{
	unsigned short* pData;
	unsigned int* pDst;
	void** pObj;
	int i;

	*(unsigned short*) pThis = *pStream;
	pData = pStream + 1;
	pDst = (unsigned int*) ((char*) pThis + 4);
	pObj = (void**) ((char*) pThis + 0x34);
	for (i = 0; i < 4; i++) {
		unsigned int x = (unsigned int) pData[0] << 0xc;
		unsigned int y = (unsigned int) pData[1] << 0xc;
		unsigned int z = (unsigned int) pData[2] << 0xc;
		pData += 3;
		pDst[0] = x;
		pDst[1] = y;
		pDst[2] = z;
		*(unsigned int*) ((char*) *pObj + 0x9c) = x;
		*(unsigned int*) ((char*) *pObj + 0xa0) = y;
		*(unsigned int*) ((char*) *pObj + 0xa4) = z;
		*(int*) ((char*) *pObj + 0x124) = 0;
		pDst += 3;
		pObj++;
	}
	if (*(unsigned char*) pThis & 1) *(int*) ((char*) *(void**) ((char*) pThis + 0x34) + 0x124) = 1;
	if (*(unsigned char*) pThis & 2) *(int*) ((char*) *(void**) ((char*) pThis + 0x38) + 0x124) = 1;
	if (*(unsigned char*) pThis & 4) *(int*) ((char*) *(void**) ((char*) pThis + 0x3c) + 0x124) = 1;
	if (*(unsigned char*) pThis & 8) *(int*) ((char*) *(void**) ((char*) pThis + 0x40) + 0x124) = 1;
}
// FUNCTION: LEMBALL 0x0042f680
void __fastcall CMoverManager_LoadLevel(void* pThis, int nUnused, unsigned short* pStream, int param_4, int param_5)
{
	unsigned short nCount;
	unsigned int uSlot;
	unsigned int uFlags;
	unsigned int flagOverride;
	unsigned int routeSel;
	unsigned int routeCount;

	nCount = *pStream;
	pStream++;
	((void(__fastcall*) (void*, int)) 0x401811)(pThis, nCount);
	*(int*) ((char*) pThis + 0x34) = 0;
	if (nCount != 0) {
		do {
			if (*(unsigned short*) (*(int*) ((char*) pThis + 0x3c) + 0x54) < 2) {
				uSlot = (unsigned int) ((int(__fastcall*) ()) 0x40214e)();
			}
			else {
				uSlot = *pStream;
				pStream++;
			}
			uFlags = 0;
			flagOverride = 0;
			if (*(unsigned short*) (*(int*) ((char*) pThis + 0x3c) + 0x54) > 5) {
				uFlags = *pStream;
				pStream++;
				if ((uFlags & 0x8000) != 0) {
					flagOverride = 1;
					uFlags &= 0x7fff;
				}
			}
			routeSel = *pStream;
			pStream++;
			routeCount = *pStream;
			pStream++;
			((void(__fastcall*) (void*, int, int, int, int, int)) 0x401f55)(pThis, uSlot, uFlags, flagOverride, routeSel, routeCount);
			nCount--;
		} while (nCount != 0);
	}
}
// FUNCTION: LEMBALL 0x00436850
void __fastcall C2D_Restart(void* pThis, int nUnused)
{
	*(int*) ((char*) pThis + 0x918) = 0;
	*(int*) ((char*) pThis + 0x91c) = 0;
	*(int*) ((char*) pThis + 0x2214) = 1;
	((void(__fastcall*) (void*, int, int)) 0x402f2c)(*(void**) ((char*) pThis + 0x97c), 0, 0);
	((void(__fastcall*) (void*)) 0x4021ee)(*(void**) ((char*) pThis + 0x96c));
	((void(__fastcall*) (void*, int, int)) 0x465aa0)(*(void**) ((char*) pThis + 0x978), 0, 0);
	*(int*) ((char*) pThis + 0x8f8) = *(int*) 0x49ce04;
	*(int*) (*(int*) 0x49cb68 + 4) = *(int*) ((char*) pThis + 0x8f4);
	*(int*) (*(int*) ((char*) pThis + 0x96c) + 0xf0) = *(int*) ((char*) pThis + 0x8f4);
	((void(__fastcall*) (void*)) 0x4029aa)(*(void**) ((char*) pThis + 0x96c));
	*(int*) ((char*) pThis + 0x8f0) = *(int*) (*(int*) ((char*) pThis + 0x96c) + 0xf0);
}
// FUNCTION: LEMBALL 0x004200f0
void __fastcall CEnemy_EnemyAction_PATROL(void* pThis, int nUnused, void* pDesc)
{
	unsigned int local_18 = 0xaa55aa55;
	unsigned int local_14 = 0xaa55aa55;
	unsigned int local_10 = 0xaa55aa55;
	unsigned int local_c[3];
	unsigned int* pPoint;

	if (((int(__fastcall*) ()) 0x401f37)() != 1) {
		unsigned int idx = (unsigned int) *(unsigned short*) (*(int*) (*(int**) pDesc + 0x10) + *(int*) (*(int**) pDesc + 8) * 2);
		pPoint = (unsigned int*) ((unsigned int*(__fastcall*) (void*, unsigned int*, unsigned int)) 0x401410)(*(void**) 0x4a74b0, local_c, idx);
		local_18 = pPoint[0];
		local_14 = pPoint[1];
		local_10 = pPoint[2];
		*(int*) (*(int**) pDesc + 8) += *(int*) (*(int**) pDesc + 0xc);
		{
			int* pBase = *(int**) pDesc;
			int cur = *(int*) ((char*) pBase + 8);
			if (*(int*) ((char*) pBase + 4) <= cur || cur < 0) {
				if (*(int*) pBase == 0) {
					*(int*) ((char*) pBase + 0xc) = -*(int*) ((char*) pBase + 0xc);
					*(int*) ((char*) pBase + 8) += *(int*) ((char*) pBase + 0xc);
				}
				else if (*(int*) pBase == 1) {
					*(int*) ((char*) pBase + 8) = 0;
				}
			}
		}
		((void(__fastcall*) (void*, unsigned int*)) 0x401d52)(pThis, &local_18);
	}
}
// FUNCTION: LEMBALL 0x0042e980
void __fastcall CMover_FindObjectsOnTopOfMe(void* pThis, int nUnused)
{
	int minX;
	int maxX;
	int minY;
	int maxY;
	unsigned int count;
	unsigned int i;
	void* pObj;

	minX = (*(int*) ((char*) pThis + 0x9c) >> 0xc) - 8;
	maxX = minX + 0xf;
	minY = (*(int*) ((char*) pThis + 0xa0) >> 0xc) - 8;
	maxY = minY + 0xf;
	count = (unsigned int) *(unsigned short*) 0x4a74bc;
	for (i = 0; (int) i < (int) count; i++) {
		pObj = *(void**) (0x4a6510 + i * 4);
		if (pObj != (void*) 0x0) {
			if ((short) ((short(__fastcall*) (void*)) 0x401794)(pObj) != (short) -1) {
				if ((short) ((short(__fastcall*) (void*)) 0x401794)(pThis) != (short) ((short(__fastcall*) (void*)) 0x401794)(pObj)) {
					if (*(int*) ((char*) pObj + 0x64) != 7) {
						int ex = *(int*) ((char*) pObj + 0x9c) >> 0xc;
						int ey = *(int*) ((char*) pObj + 0xa0) >> 0xc;
						if (minX <= ex && ex <= maxX && minY <= ey && ey <= maxY) {
							((void(__fastcall*) (void*, void*)) 0x4036b1)(pThis, pObj);
						}
					}
				}
			}
		}
	}
}
// FUNCTION: LEMBALL 0x0045d8b0
void __fastcall CResFONT_AllocateResources(void* pThis, int nUnused, unsigned int cEntries)
{
	unsigned int* pGlyphBase;
	unsigned int* pIntCodeBase;
	unsigned int i;

	(void) nUnused;
	pGlyphBase = (unsigned int*) AllocateVSMemBlock(cEntries * 76u + 4u);
	if (pGlyphBase != (void*) 0x0) {
		pGlyphBase[0] = cEntries;
		for (i = 0; (int) i < (int) cEntries; i++) {
			((void* (__fastcall*) (void*)) 0x45e8f0)((char*) (pGlyphBase + 1) + i * 76);
		}
		*(void**) ((char*) pThis + 0x80) = pGlyphBase + 1;
	}
	else {
		*(void**) ((char*) pThis + 0x80) = (void*) 0x0;
	}
	pIntCodeBase = (unsigned int*) AllocateVSMemBlock(cEntries * 84u + 4u);
	if (pIntCodeBase != (void*) 0x0) {
		pIntCodeBase[0] = cEntries;
		for (i = 0; (int) i < (int) cEntries; i++) {
			((void* (__fastcall*) (void*)) 0x45e7e0)((char*) (pIntCodeBase + 1) + i * 84);
		}
		*(void**) ((char*) pThis + 0x7c) = pIntCodeBase + 1;
	}
	else {
		*(void**) ((char*) pThis + 0x7c) = (void*) 0x0;
	}
}
// FUNCTION: LEMBALL 0x00421ef0
void __fastcall CBallManager_Initialise(void* pThis, int nUnused, int nCapacity)
{
	void* pBalls;
	int i;
	void* pBall;

	(void) nUnused;
	*(int*) ((char*) pThis + 0xc) = nCapacity;
	*(int*) ((char*) pThis + 0x8) = 0;
	if (nCapacity == 0) {
		*(void**) ((char*) pThis + 0x4) = (void*) 0x0;
		return;
	}
	pBalls = *(void**) ((char*) pThis + 0x4);
	if (pBalls == (void*) 0x0) {
		pBalls = AllocateVSMemBlock((unsigned int) nCapacity * 4u);
		*(void**) ((char*) pThis + 0x4) = pBalls;
	}
	if (*(int*) ((char*) pThis + 0xc) > 0) {
		for (i = 0; i < *(int*) ((char*) pThis + 0xc); i++) {
			pBall = (void*) ((void* (__cdecl*) (int)) 0x45a780)(0x13c);
			if (pBall != (void*) 0x0) {
				pBall = ((void* (__fastcall*) (void*)) 0x4015c8)(pBall);
				*((void**) pBalls + i) = pBall;
			}
			else {
				*((void**) pBalls + i) = (void*) 0x0;
			}
			pBall = *((void**) pBalls + i);
			((void (__fastcall*) (void*)) (*(void***) pBall + 0x104 / 4))(pBall);
		}
	}
}
// FUNCTION: LEMBALL 0x00458d40
int __fastcall CVSOStream___ls(void* pThis, int nUnused, unsigned int value)
{
	int i;
	unsigned int uShift;
	void* pInner;
	void* pTarget;

	(void) nUnused;
	uShift = 0x18;
	pInner = *(void**) (*(char**) pThis + 4);
	for (i = 0; i < 3; i++) {
		pTarget = *(void**) ((char*) (int) pInner + (int) pThis + 0x1c);
		((void (__fastcall*) (void*, int, unsigned int)) (*(void***) pTarget + 2))(pTarget, 0, value >> uShift);
		uShift -= 8;
	}
	return (int) pThis;
}

// Minimal views so the level-mode stream loaders emit direct `call` to the
// already-reconstructed members (matches orig direct calls to 0x45f250/0x45f280).
struct CNetworkMessage {
	int SaveEffStreamToMemoryRange(int nTargetBuffer, int cbRange);
};
struct GameEffStream {
	int LoadEffStreamFromMemory(int nSourceBuffer);
};
// FUNCTION: LEMBALL 0x00412b80
void __fastcall LoadLevelModeManagerStreamRanges0x1b4(void* pThis)
{
	int* pMgr = (int*)((char*) pThis + 0x1b4);
	unsigned int uMask;
	unsigned int uBase;
	int count = 4;
	{
		unsigned int v = (unsigned int) ((char*) pThis - 0x1c);
		uMask = v >= 1 ? 0xffffffffu : 0u;
		uBase = uMask & (unsigned int) pThis;
	}
	do {
		int mgr = *pMgr++;
		((CNetworkMessage*) (mgr + 0x138))->SaveEffStreamToMemoryRange(*(int*) (uBase + 0x1c), 0);
		*(int*) (uBase + 0x1c) += *(int*) (mgr + 0x154) - *(int*) (mgr + 0x140);
	} while (--count != 0);
}
// FUNCTION: LEMBALL 0x00412be0
void __fastcall LoadLevelModeManagerStreams0x1c4(void* pThis)
{
	int* pMgr = (int*)((char*) pThis + 0x1c4);
	unsigned int uMask;
	unsigned int uBase;
	int count = 4;
	{
		unsigned int v = (unsigned int) ((char*) pThis - 0x1c);
		uMask = v >= 1 ? 0xffffffffu : 0u;
		uBase = uMask & (unsigned int) pThis;
	}
	do {
		int mgr = *pMgr++;
		int r = ((GameEffStream*) (mgr + 0x138))->LoadEffStreamFromMemory(*(int*) (uBase + 0x20));
		if (r != 0) {
			*(int*) (uBase + 0x20) = *(int*) (mgr + 0x158);
		}
	} while (--count != 0);
}
// FUNCTION: LEMBALL 0x00427110
void __fastcall CRocketManager_LoadLevel(void* pThis, int nUnused, int uArg1, int uArg2, unsigned short* pStream)
{
	unsigned short nCount = *pStream;
	unsigned int nRemain = (unsigned int) nCount;
	pStream++;
	((void(__fastcall*) (void*, int)) 0x403760)(pThis, nCount);
	if (nCount != 0) {
		do {
			unsigned short uSlot;
			if (*(unsigned short*) (*(int*) ((char*) pThis + 0x3c) + 0x54) < 2) {
				uSlot = (unsigned short) ((int(__fastcall*) ()) 0x40214e)();
			}
			else {
				uSlot = *pStream;
				pStream++;
			}
			unsigned int x = *(pStream);
			pStream++;
			unsigned int y = *(pStream);
			pStream++;
			unsigned int z = *(pStream);
			pStream++;
			((void(__fastcall*) (void*, unsigned short, int, int, int)) 0x4017b7)(pThis, uSlot, x, y, z);
			nRemain--;
		} while (nRemain != 0);
	}
}
// FUNCTION: LEMBALL 0x0046aaf0
void __fastcall CBaseRemap_MapRemap(void* pThis, int nUnused, unsigned char* param_1)
{
	int* pi = *(int**) ((char*) pThis + 4);
	if (pi[4] == 0) {
		((void(__fastcall*) (void*)) *(int*) (pi + 0x1c / 4))((char*) pi);
	}
	else {
		pi[9] = 0;
	}
	pi[2] = pi[2] + 1;
	unsigned int nSize = *(unsigned int*) (*(int*) ((char*) pThis + 4) + 0x48);
	void* pBuf = ((void* (__cdecl*) (unsigned int)) 0x45a780)(nSize);
	*(void**) pThis = pBuf;
	int i = 0;
	unsigned int nPairs = (unsigned int) param_1[0];
	if (0 < (int) nSize) {
		do {
			*(char*) (*(int*) pThis + i) = (char) i;
			i = i + 1;
		} while (i < (int) nSize);
	}
	i = 0;
	if (nPairs != 0) {
		do {
			if (param_1[nPairs + i + 1] != 0) {
				*(unsigned char*) ((unsigned int) param_1[i + 1] + *(int*) pThis) = param_1[nPairs + i + 1];
			}
			i = i + 1;
		} while (i < (int) nPairs);
	}
	*(int*) (*(int*) ((char*) pThis + 4) + 8) = *(int*) (*(int*) ((char*) pThis + 4) + 8) - 1;
}
// FUNCTION: LEMBALL 0x00419dd0
void __fastcall ResetManagedEntityMotionWith4000TickDelay(void* pThis, int nUnused, void* pObject)
{
	int* pVtbl;
	((void (__fastcall*) (void*)) 0x40207c)(pObject);
	pVtbl = *(int**) pObject;
	((void (__fastcall*) (void*, int)) (pVtbl[0x84 / 4]))(pObject, 4000);
	((void (__fastcall*) (void*)) (pVtbl[0x30 / 4]))(pObject);
}
// FUNCTION: LEMBALL 0x0041b9f0
void* __fastcall FindCountedChildContainingFixedPoint(void* pThis, int nUnused, int x, int y)
{
	unsigned int nCount = *(unsigned short*) ((char*) pThis + 0x36);
	int* pChild = *(int**) ((char*) pThis + 0x3c);
	for (unsigned int i = 0; i < nCount; i++) {
		int tx = *(int*) (*pChild + 0x9c) >> 12;
		int ty = *(int*) (*pChild + 0xa0) >> 12;
		int rx = x >> 12;
		int ry = y >> 12;
		if ((tx - 8 < rx) && (rx < tx) && (ty - 8 < ry) && (ry < ty)) {
			return *(void**) (*(int*) ((char*) pThis + 0x3c) + i * 4);
		}
		pChild++;
	}
	return 0;
}
// FUNCTION: LEMBALL 0x0041ba80
void* __fastcall FindManagedEntityAtTileByType(void* pThis, int nUnused, int x, int y, int pUnused, int nType)
{
	int rx = x >> 12;
	int ry = y >> 12;
	unsigned int i = 0;
	while (true) {
		unsigned int nCount = *(unsigned short*) ((char*) pThis + 0x36);
		if (nCount <= i) {
			return 0;
		}
		void* pChild = *(void**) (*(int*) ((char*) pThis + 0x3c) + i * 4);
		if (*(int*) ((char*) pChild + 0x64) == nType) {
			int tx = *(int*) ((char*) pChild + 0x9c) >> 12;
			int ty = *(int*) ((char*) pChild + 0xa0) >> 12;
			if ((tx - 8 < rx) && (rx < tx) && (ty - 8 < ry) && (ry < ty)) {
				return *(void**) (*(int*) ((char*) pThis + 0x3c) + i * 4);
			}
		}
		i++;
	}
}
// FUNCTION: LEMBALL 0x0041d430
unsigned short* __fastcall LoadGmobChunkType0x14ActionList(void* pThis, int nUnused, unsigned short** ppStream)
{
	unsigned short nCount = *(unsigned short*) *ppStream;
	unsigned int nRemain = (unsigned int) nCount;
	*ppStream = *ppStream + 1;
	if (nCount != 0) {
		do {
			unsigned short* pEntry = *ppStream;
			unsigned int second = pEntry[1];
			unsigned int first = *pEntry;
			*ppStream = pEntry + 2;
			((void (__fastcall*) (void*, int, unsigned int, unsigned short)) 0x403161)(pThis, nUnused, second, first);
			nRemain--;
		} while (nRemain != 0);
	}
	return *ppStream;
}
