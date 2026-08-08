// Cluster reconstruction supplemental TU (avoids append-corruption on the large ClusterScanOps.cpp).
#include "Visos/Generic/Memory.h"

extern void __fastcall AppendType18ChunkObject(void* pStream, int nUnused, unsigned short param_1, void* param_2, int param_3, int param_4);
extern void __fastcall DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void __fastcall ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void __fastcall ReleaseTypedResourceObjectIfLoaded(void* pObject, void* pUnusedEdx, int fReleaseMode);
extern void* g_pActiveManagedEntityOwner;
extern void* g_pCachedChunkManagerLevelMode;
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

struct CGameObject {
	void SetId(unsigned short nSlotId);
};


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


// FUNCTION: LEMBALL 0x00455da0
void* __fastcall ConstructNetworkLobbyPlayerEntryChild(void* pObject)
{
	((void(__fastcall*)(void*)) 0x46a300)(pObject);
	*(void**) pObject = (void**) 0x4986b0;
	((void(__fastcall*)(void*)) 0x4014dd)(pObject);
	return pObject;
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











// Minimal view of the LEVELVT runtime-state resetter so Restart can emit
// a direct `call LevelChunkObjectRuntimeStateView::ResetRuntimeStateThunk` (orig ILT 0x40209f).
struct LevelChunkObjectRuntimeStateView {
	void ResetRuntimeStateThunk(void);
};












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

// FUNCTION: LEMBALL 0x0040acf0
void __cdecl invoke_callback_grid(int nBase, int nStride, int nCount, void (__fastcall* pCallback)(int))
{
	int value = nBase + nCount * nStride;
	while (nCount > 0) {
		value = value - nStride;
		pCallback(value);
		nCount = nCount - 1;
	}
}

// FUNCTION: LEMBALL 0x0041f250
void __fastcall remove_cached_chunk_object_from_level_list_and_destroy(void* pThis, int nUnusedEdx, void* pObject)
{
	void* pOwner;
	void** ppItems;
	void** ppVtbl;
	int nCount;
	int nIndex;
	int nByte;

	(void) nUnusedEdx;
	((void (__fastcall*) (void*, int, void*)) (*(void***) pThis)[0x44 / 4])(pThis, 0, pObject);
	pOwner = g_pCachedChunkManagerLevelMode;
	nCount = *(int*) ((char*) pOwner + 0x118);
	nIndex = 0;
	if (nCount > 0) {
		ppItems = *(void***) ((char*) pOwner + 0x120);
		do {
			if (ppItems[nIndex] == pObject) {
				nCount = nCount - 1;
				*(int*) ((char*) pOwner + 0x118) = nCount;
				if (nIndex < nCount) {
					nByte = nIndex * 4;
					while (nIndex < *(int*) ((char*) pOwner + 0x118)) {
						nIndex = nIndex + 1;
						*(int*) ((char*) ppItems + nByte) = *(int*) ((char*) ppItems + nByte + 4);
						nByte = nByte + 4;
					}
				}
				*(void**) ((char*) ppItems + nCount * 4) = 0;
				break;
			}
			nIndex = nIndex + 1;
		} while (nIndex < nCount);
	}
	ppVtbl = *(void***) pObject;
	((void (__fastcall*) (void*, int)) ppVtbl[0xc8 / 4])(pObject, 0);
	if (pObject != 0) {
		((void (__fastcall*) (void*, int, unsigned char)) ppVtbl[0])(pObject, 0, 1);
	}
}

// FUNCTION: LEMBALL 0x00420d30
void __fastcall remove_cached_enmy_chunk_object_and_destroy(void* pThis, int nUnusedEdx, void* pObject)
{
	void* pOwner;
	void** ppItems;
	void** ppVtbl;
	int nCount;
	int nIndex;
	int nByte;

	(void) nUnusedEdx;
	pOwner = g_pCachedChunkManagerLevelMode;
	nCount = *(int*) ((char*) pOwner + 0x118);
	nIndex = 0;
	if (nCount > 0) {
		ppItems = *(void***) ((char*) pOwner + 0x120);
		do {
			if (ppItems[nIndex] == pObject) {
				nCount = nCount - 1;
				*(int*) ((char*) pOwner + 0x118) = nCount;
				if (nIndex < nCount) {
					nByte = nIndex * 4;
					while (nIndex < *(int*) ((char*) pOwner + 0x118)) {
						nIndex = nIndex + 1;
						*(int*) ((char*) ppItems + nByte) = *(int*) ((char*) ppItems + nByte + 4);
						nByte = nByte + 4;
					}
				}
				*(void**) ((char*) ppItems + nCount * 4) = 0;
				break;
			}
			nIndex = nIndex + 1;
		} while (nIndex < nCount);
	}
	((void (__fastcall*) (void*, int, void*)) (*(void***) pThis)[0x44 / 4])(pThis, 0, pObject);
	ppVtbl = *(void***) pObject;
	((void (__fastcall*) (void*, int)) ppVtbl[0xc8 / 4])(pObject, 0);
	if (pObject != 0) {
		((void (__fastcall*) (void*, int, unsigned char)) ppVtbl[0])(pObject, 0, 1);
	}
}

// FUNCTION: LEMBALL 0x0041d390
void __fastcall append_gmob_type_0x14_lift_or_door_actions(void* pThis, int nUnusedEdx)
{
	int nMode;
	int nIndex;
	unsigned short uLiftId;
	short sDoorId;

	(void) nUnusedEdx;
	nMode = *(int*) ((char*) pThis + 0x14c);
	if (nMode == 1) {
		uLiftId = ((unsigned short (__fastcall*) (void*, int, int)) 0x402126)(g_pActiveManagedEntityOwner, 0, *(int*) ((char*) pThis + 0x150));
		((void (__fastcall*) (void*, int, int, unsigned short)) 0x403161)(pThis, 0, 1, uLiftId);
		return;
	}
	if (nMode == 2) {
		nIndex = *(int*) ((char*) pThis + 0x150);
		if (nIndex < *(int*) ((char*) pThis + 0x154)) {
			do {
				uLiftId = ((unsigned short (__fastcall*) (void*, int, int)) 0x402126)(g_pActiveManagedEntityOwner, 0, nIndex);
				((void (__fastcall*) (void*, int, int, unsigned short)) 0x403161)(pThis, 0, 1, uLiftId);
				nIndex = nIndex + 1;
			} while (nIndex < *(int*) ((char*) pThis + 0x154));
		}
		return;
	}
	if (nMode != 3) {
		return;
	}
	sDoorId = ((short (__fastcall*) (void*, int, int)) 0x40344a)(g_pActiveManagedEntityOwner, 0, *(int*) ((char*) pThis + 0x150));
	if (sDoorId != -1) {
		((void (__fastcall*) (void*, int, int, short)) 0x403161)(pThis, 0, 3, sDoorId);
	}
}

// FUNCTION: LEMBALL 0x0042a3a0
void __fastcall activate_boon_chunk_object_by_subtype_at_position(void* pThis, int nUnusedEdx, int posX, int posY, int posZ, int subtype)
{
	unsigned short nFlag;
	int nIndex;
	void* pBoonObject;

	(void) nUnusedEdx;
	switch (subtype) {
	case 0x28: nFlag = 1; nIndex = 0; break;
	case 0x2a: nFlag = 2; nIndex = 1; break;
	case 0x2c: nFlag = 4; nIndex = 2; break;
	case 0x2e: nFlag = 8; nIndex = 3; break;
	default: return;
	}
	*(unsigned short*) pThis = *(unsigned short*) pThis | nFlag;
	*(int*) ((char*) pThis + nIndex * 12 + 4) = posX << 12;
	*(int*) ((char*) pThis + nIndex * 12 + 8) = posY << 12;
	*(int*) ((char*) pThis + nIndex * 12 + 12) = posZ << 12;
	pBoonObject = *(void**) ((char*) pThis + nIndex * 4 + 0x34);
	*(int*) ((char*) pBoonObject + 0x9c) = posX << 12;
	*(int*) ((char*) pBoonObject + 0xa0) = posY << 12;
	*(int*) ((char*) pBoonObject + 0xa4) = posZ << 12;
	*(int*) ((char*) pBoonObject + 0x124) = 1;
}



// FUNCTION: LEMBALL 0x004166d0
unsigned short collect_free_managed_entity_slot_ids(unsigned short* pOut, int nLimit)
{
	extern unsigned char g_GAME_ManagedEntitySlotBitMasks[8];
	extern unsigned char g_GAME_ManagedEntitySlotClaimBitset[0x100];
	int nCount = 0;
	for (int nSlot = 0; nSlot < 0x100; ++nSlot) {
		if (g_GAME_ManagedEntitySlotClaimBitset[nSlot] == 0xff) {
			continue;
		}
		for (int nMask = 0; nMask < 8; ++nMask) {
			if ((g_GAME_ManagedEntitySlotClaimBitset[nSlot] & g_GAME_ManagedEntitySlotBitMasks[nMask]) != 0) {
				continue;
			}
			pOut[nCount++] = (unsigned short) ((nSlot << 3) | nMask);
			if (nCount == nLimit) {
				return (unsigned short) nLimit;
			}
		}
	}
	return (unsigned short) nCount;
}

// FUNCTION: LEMBALL 0x00422710
int __fastcall remove_managed_entity_array_entry_and_release(void* pArrayOwner, int nUnusedEdx, void* pElement)
{
	typedef void (__fastcall * RefreshProc)(void* pObject);
	typedef void (__fastcall * DeleteProc)(void* pObject, int nUnusedEdx, unsigned int fDelete);
	void** pArray;
	int nCount;
	int nIndex;
	int j;
	void** pVtable;

	nCount = *(int*) ((char*) pArrayOwner + 0x3c);
	pArray = *(void***) ((char*) pArrayOwner + 0x34);
	for (nIndex = 0; nIndex < nCount; ++nIndex) {
		if (pArray[nIndex] == pElement) {
			pVtable = *(void***) pElement;
			((RefreshProc) pVtable[0xc8 / sizeof(void*)])(pElement);
			((CGameObject*) pElement)->SetId(0xffff);
			if (pElement != 0) {
				((DeleteProc) pVtable[0])(pElement, 0, 1);
			}
			nCount = *(int*) ((char*) pArrayOwner + 0x3c) - 1;
			*(int*) ((char*) pArrayOwner + 0x3c) = nCount;
			if (nIndex < nCount) {
				for (j = nIndex; j < nCount; ++j) {
					pArray[j] = pArray[j + 1];
				}
			}
			return nIndex;
		}
	}
	return nIndex;
}

// FUNCTION: LEMBALL 0x00457080
void __stdcall copy_low_bytes_from_dword_stride(unsigned char* pDst, const unsigned char* pSrc, int nCount)
{
	while (nCount > 0) {
		*pDst++ = *pSrc;
		pSrc += 4;
		nCount--;
	}
}

// FUNCTION: LEMBALL 0x0044b660
void __cdecl add_fixed12_2d_vectors(int* pOut, const int* pA, const int* pB)
{
	pOut[0] = pA[0] + pB[0];
	pOut[1] = pA[1] + pB[1];
}

extern void* RegisterPaletteRemapVariant(int nPaletteResourceId, unsigned char* pRemapTable, int nBuildMode);

// FUNCTION: LEMBALL 0x004448c0
void __fastcall RegisterPauseDialogPaletteRemaps(void* pObject)
{
	unsigned char** ppRemapTables = (unsigned char**) 0x49f058;
	int i;
	for (i = 0; i < 4; i++) {
		*(void**) ((char*) pObject + 0x1e0 + i * 4) = RegisterPaletteRemapVariant(
			*(int*) (*(char**) ((char*) pObject + 0x114) + 0x54),
			ppRemapTables[i],
			2);
	}
}

struct LobbyActionButtonFrameRect {
	short m_nX;
	short m_nY;
	short m_nCX;
	short m_nCY;
};

// FUNCTION: LEMBALL 0x004536b0
void __fastcall DrawNetworkLobbyActionButtonFrame(void* pObject, int nUnused, int param_1)
{
	unsigned int* pEntry = (unsigned int*) (*(unsigned int*) ((char*) pObject + 0x42c) + param_1 * 8);
	LobbyActionButtonFrameRect Rect;
	((LobbyActionButtonFrameRect* (__fastcall*)(LobbyActionButtonFrameRect*, int, short, short, short, short)) 0x403594)(
		&Rect, 0, (short) pEntry[0], (short) pEntry[1], (short) pEntry[2], (short) pEntry[3]);
	((void (__fastcall*)(void*, int, LobbyActionButtonFrameRect)) 0x4020e0)(pObject, 0, Rect);
}

// FUNCTION: LEMBALL 0x00412740
void __fastcall CollectLevelModeManagerEntries(void* pLevelModeManager, int nUnused, int* pOutCount)
{
	int nFirstCount;
	*pOutCount = 0;
	nFirstCount = (*(int (__fastcall**)(void*, int, int*)) (*(void***) ((char*) pLevelModeManager + 0x170) + 0x12))((void*) ((char*) pLevelModeManager + 0x170), 0, pOutCount + 1);
	*pOutCount = nFirstCount;
	*(int*) pOutCount += (*(int (__fastcall**)(void*, int, int*)) (*(void***) ((char*) pLevelModeManager + 0x15c) + 0x12))((void*) ((char*) pLevelModeManager + 0x15c), 0, pOutCount + nFirstCount + 1);
}

// FUNCTION: LEMBALL 0x0042de40
int __fastcall copy_ice_chunk_object_aux_records(void* pObject, int nUnused, void* pDstAux)
{
	int iVar2 = 0;
	int iVar3 = 0;
	while (iVar2 < *(int*) ((char*) pObject + 0x34)) {
		int iVar1 = *(int*) ((char*) pObject + 0x38) + iVar3;
		iVar2++;
		iVar3 += 0x188;
		*(int*) ((char*) pDstAux + iVar3 - 0x188) = *(int*) (iVar1 + 0x17c);
		*(unsigned short*) ((char*) pDstAux + (iVar3 - 0x188) + 4) = *(unsigned short*) (iVar1 + 0x180);
		*(int*) ((char*) pDstAux + (iVar3 - 0x188) + 6) = *(int*) (iVar1 + 0x182);
		*(unsigned short*) ((char*) pDstAux + (iVar3 - 0x188) + 10) = *(unsigned short*) (iVar1 + 0x186);
	}
	return *(int*) ((char*) pObject + 0x34);
}

// FUNCTION: LEMBALL 0x0046d560
void __fastcall apply_point_delta_to_child_helper(void* pThis, int nUnused, short* pDelta)
{
	int iBase = *(int*) (*(int*) ((char*) pThis + 0x40) + 4);
	short* pPt = (short*) (iBase + 0x50 + (int) pThis);
	char* pTmp = (char*) (iBase + 0x4c + (int) pThis);
	*pPt += pDelta[0];
	*(short*) (pTmp + 6) += pDelta[1];
	void* pOwner = (char*) pThis + *(int*) (*(int*) ((char*) pThis + 0x40) + 4) + 0x40;
	void* pArg = (iBase + 0x4c + (int) pThis == 1) ? 0 : pPt;
	(*( void(**)(void*)) (*(void***) *(void**) pOwner + 0x2c / 4))(pArg);
}

// FUNCTION: LEMBALL 0x0044c100
void __fastcall expand_rect_to_cover_rect(void* pThis, int nUnused, short* pRect)
{
	int nW = pRect[0];
	int nH = pRect[1];
	if (nW * nH != 0) {
		if (pRect[2] < *(short*) ((char*) pThis + 4)) {
			*(short*) pThis = *(short*) pThis + (*(short*) ((char*) pThis + 4) - pRect[2]);
			*(short*) ((char*) pThis + 4) = pRect[2];
		}
		if ((short) (*(short*) pThis + *(short*) ((char*) pThis + 4)) < (short) (pRect[0] + pRect[2])) {
			*(short*) pThis = (pRect[2] - *(short*) ((char*) pThis + 4)) + pRect[0];
		}
		if (pRect[3] < *(short*) ((char*) pThis + 6)) {
			*(short*) ((char*) pThis + 2) = *(short*) ((char*) pThis + 2) + (*(short*) ((char*) pThis + 6) - pRect[3]);
			*(short*) ((char*) pThis + 6) = pRect[3];
		}
		if ((short) (*(short*) ((char*) pThis + 2) + *(short*) ((char*) pThis + 6)) < (short) (pRect[3] + pRect[1])) {
			*(short*) ((char*) pThis + 2) = (pRect[1] - *(short*) ((char*) pThis + 6)) + pRect[3];
		}
	}
}
