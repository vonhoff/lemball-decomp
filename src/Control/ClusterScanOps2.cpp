// Cluster reconstruction supplemental TU (avoids append-corruption on the large ClusterScanOps.cpp).
#include "Visos/Generic/Memory.h"

extern void __fastcall AppendType18ChunkObject(void* pStream, int nUnused, unsigned short param_1, void* param_2, int param_3, int param_4);
extern void __fastcall DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void __fastcall ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void __fastcall ReleaseTypedResourceObjectIfLoaded(void* pObject, void* pUnusedEdx, int fReleaseMode);
extern void* g_pActiveManagedEntityOwner;
extern void* g_pSharedRenderDispatchQueue;
extern void* g_pVariantResourceEntryManager;
extern void* g_pLevelProgressState;
extern int g_nQueuedVariantChildSlotManagerModeSelectedResourceId;
extern void* g_pActiveNetworkLobbyTransportController;
extern char g_szNetworkLobbyLocalPlayerName[0x10];
extern void SetLevelScreenStatusIndicatorMode(int nMode, int nValue);
extern void __fastcall ResetTypedResourceObjectState(void* pObject);
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

struct GameVariantResourceEntryManager {
	void PlayVariantResourceEffect(int nEffectSlot);
};

struct DebugTextRect {
	long nLeft;
	long nTop;
	long nRight;
	long nBottom;
};

extern "C" BOOL WINAPI GetClientRect(HWND hWnd, DebugTextRect* lpRect);
extern BOOL WINAPI ClientToScreen(HWND hWnd, tagPOINT* lpPoint);
extern "C" int WINAPI GetSystemMetrics(int nIndex);
extern "C" BOOL WINAPI AdjustWindowRect(DebugTextRect* lpRect, long dwStyle, BOOL bMenu);
extern "C" BOOL WINAPI SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, unsigned int uFlags);


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

struct CountedU16Record {
	unsigned int m_nType00;
	unsigned int m_cValues04;
	unsigned int m_nFlags08;
	int m_nSignedValue0C;
	unsigned short* m_pValues10;
};

// FUNCTION: LEMBALL 0x00420f90
unsigned char* __fastcall ParseCountedU16RecordFromStream(void*,
	int,
	unsigned char* pStream,
	CountedU16Record** ppRecord)
{
	unsigned int cValues;
	unsigned char bSigned;
	unsigned char* pSource;
	int nOffset;
	int cRemaining;

	((void(__cdecl*)(const unsigned char*)) 0x40168b)(pStream);
	pStream += 4;
	*ppRecord = (CountedU16Record*) AllocateVSMemBlock(sizeof(CountedU16Record));
	cValues = pStream[1];
	(*ppRecord)->m_nType00 = pStream[0];
	(*ppRecord)->m_cValues04 = cValues;
	(*ppRecord)->m_nFlags08 = pStream[2];
	bSigned = pStream[3];
	if ((bSigned & 0x80) != 0) {
		(*ppRecord)->m_nSignedValue0C = (int) bSigned | ~0xff;
	}
	else {
		(*ppRecord)->m_nSignedValue0C = bSigned;
	}
	(*ppRecord)->m_pValues10 =
		(unsigned short*) AllocateVSMemBlock(cValues * sizeof(unsigned short));
	if (cValues > 0) {
		pSource = pStream + 4;
		nOffset = 0;
		cRemaining = cValues;
		do {
			unsigned short nValue = *(unsigned short*) pSource;
			pSource += 2;
			nOffset += 2;
			--cRemaining;
			*(unsigned short*) ((char*) (*ppRecord)->m_pValues10 + nOffset - 2) = nValue;
		} while (cRemaining != 0);
	}
	return pStream + cValues * 2 + 4;
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


// FUNCTION: LEMBALL 0x0045ded0
void* __fastcall ConstructTwoArrayListResource(void* pObject, int nUnused)
{
	unsigned int dwTable = *(unsigned int*) 0x4a1d6c;
	*(void**) pObject = (void*) 0x498980;
	*(unsigned int*) ((char*) pObject + 0x48) = dwTable;
	*(void**) pObject = (void*) 0x4989c0;
	*(int*) ((char*) pObject + 0x18) = 0;
	*(int*) ((char*) pObject + 0x5c) = 0;
	*(int*) ((char*) pObject + 0x58) = 0;
	*(int*) ((char*) pObject + 0x54) = 0;
	*(int*) ((char*) pObject + 0x60) = 0;
	*(void**) pObject = (void*) 0x498c88;
	*(int*) ((char*) pObject + 0x78) = 0;
	*(int*) ((char*) pObject + 0x7c) = 0;
	ResetTypedResourceObjectState(pObject);
	*(int*) ((char*) pObject + 0x18) = 0;
	return pObject;
}

// FUNCTION: LEMBALL 0x0043a130
void* __fastcall ConstructResourceWindowOwnerBuffer(void* pThis, void* pUnusedEdx,
	unsigned int param_2, unsigned int param_3, unsigned int param_4, unsigned int param_5)
{
	((void(__cdecl*)(unsigned int, unsigned int, int, int)) 0x468f90)(param_2, param_3, 0x30, 0xc);
	*(void**) pThis = (void*) 0x497108;
	*(void**) ((char*) pThis + 0x90) = (void*) 0x4970e0;
	*(int*) ((char*) pThis + 0x14c) = 0;
	((void(__fastcall*)(void*, int, unsigned int)) 0x403017)(pThis, 0, param_2);
	*(void**) ((char*) pThis + 0xf4) = g_pSharedRenderDispatchQueue;
	*(void**) ((char*) pThis + 0xcc) = (void*) param_2;
	return pThis;
}

// FUNCTION: LEMBALL 0x0043a250
void* __fastcall ConstructRegisteredRenderSlotArray(void* pThis, int nUnused, int param_1)
{
	void* pAlloc;
	int i;
	((void*(__fastcall*)(void*, int)) 0x462ea0)(pThis, 0);
	*(void**) pThis = (void*) 0x497208;
	pAlloc = AllocateVSMemBlock((unsigned int) param_1 * 8);
	*(void**) ((char*) pThis + 0x10) = pAlloc;
	*(int*) ((char*) pThis + 0x18) = param_1;
	*(int*) ((char*) pThis + 0x14) = 0;
	for (i = 0; i < param_1; i++) {
		*(int*) ((char*) pAlloc + i * 8) = 0;
		*(int*) ((char*) pAlloc + i * 8 + 4) = 0;
	}
	((void(__fastcall*)(void*, int, void*, int)) 0x4632a0)(g_pSharedRenderDispatchQueue, 0, pThis, -0x19);
	return pThis;
}

// FUNCTION: LEMBALL 0x00462cb0
void* __fastcall DeleteEffChannelStreamStackWrapper(void* pThis, int nUnused, char param_1)
{
	((void(__fastcall*)(void*)) 0x45f8a0)((char*) pThis + 0xa8);
	((void(__fastcall*)(void*)) 0x45fd80)((char*) pThis + 0x30);
	((void(__fastcall*)(void*)) 0x45f6c0)(pThis);
	if ((param_1 & 1) != 0) {
		FreeVSMemBlock((char*) pThis - 8);
	}
	return (char*) pThis - 8;
}

// FUNCTION: LEMBALL 0x00457d00
int __fastcall set_directdraw_palette_entries(void* pObject, int param_2)
{
	unsigned int uResult;
	void* pStream;
	char* pError;
	uResult = ((unsigned int(__stdcall*)(void*, int, int, unsigned short)) (*(void***) pObject)[0x18 / 4])(pObject, 0, 0, *(unsigned short*) ((char*) param_2 + 2));
	if (uResult != 0) {
		pError = ((char* (__cdecl*)(unsigned int)) 0x456720)(uResult & 0xfff);
		pStream = *(void**) 0x4a93a8;
		pStream = ((void* (__fastcall*)(void*, int, const char*)) 0x4585b0)(pStream, 0, "Direct Draw Set Palette Entries failed ");
		pStream = ((void* (__fastcall*)(void*, int, const char*)) 0x4585b0)(pStream, 0, pError);
		((void* (__fastcall*)(void*, int, const char*)) 0x4585b0)(pStream, 0, "\n");
		return 0;
	}
	return 1;
}
// FUNCTION: LEMBALL 0x00468b20
void* __fastcall ConstructBufferedResourceGeometryChild(void* pThis, int nUnused, void* param_2, void* param_3)
{
	((void(__fastcall*)(void*, int, void*))0x467cd0)(pThis, 0, param_2);
	*(void**)pThis = (void*)0x497630;
	*(int*)((char*)pThis + 0x108) = 0;
	*(int*)((char*)pThis + 0x104) = 0;
	*(void**)pThis = (void*)0x499838;
	*(void**)((char*)pThis + 0x90) = (void*)0x497608;
	*(void**)((char*)pThis + 0x90) = (void*)0x499818;
	*(void**)((char*)pThis + 0x114) = param_3;
	((void(__fastcall*)(void*))0x468b80)(pThis);
	return pThis;
}

// FUNCTION: LEMBALL 0x00468e50
void* __fastcall ConstructSingleChildOverlayOwnerFromRect(void* pThis, int nUnused, void* param_2, void* param_3, void* param_4, void* param_5, void* param_6)
{
	((void(__fastcall*)(void*, int, void*, void*, void*))0x468a40)(pThis, 0, param_2, param_3, param_5);
	*(void**)pThis = (void*)0x499928;
	*(void**)((char*)pThis + 0x90) = (void*)0x499908;
	void* pBlock = ((void*(__cdecl*)(int))0x45a780)(0x130);
	if (pBlock != 0) {
		*(void**)((char*)pThis + 0x118) = ((void*(__fastcall*)(void*, int, void*, void*, void*, void*))0x468410)(pBlock, 0, param_2, param_3, param_4, param_6);
	} else {
		*(void**)((char*)pThis + 0x118) = 0;
	}
	return pThis;
}

// FUNCTION: LEMBALL 0x00468f90
void* __fastcall ConstructResourceSpriteGeometryChildFromRect(void* pThis, int nUnused, void* param_2, void* param_3, void* param_4, void* param_5)
{
	ConstructBufferedResourceGeometryChild(pThis, 0, (void*)0xf7, param_3);
	*(unsigned short*)((char*)pThis + 0x12a) = 0;
	*(unsigned short*)((char*)pThis + 0x128) = 0;
	*(void**)pThis = (void*)0x499a18;
	*(unsigned short*)((char*)pThis + 0x12e) = 0;
	*(unsigned short*)((char*)pThis + 0x12c) = 0;
	*(unsigned short*)((char*)pThis + 0x142) = 0;
	*(void**)((char*)pThis + 0x90) = (void*)0x4999f8;
	*(unsigned short*)((char*)pThis + 0x140) = 0;
	unsigned short* pRect = (unsigned short*)param_2;
	unsigned short* pRectWH = (unsigned short*)(param_2 != 0 ? (char*)param_2 + 4 : 0);
	*(unsigned short*)((char*)pThis + 0xdc) = (unsigned short)(param_2 != 0 ? pRectWH[0] : 0);
	*(unsigned short*)((char*)pThis + 0xde) = (unsigned short)(param_2 != 0 ? pRectWH[1] : 0);
	*(unsigned short*)((char*)pThis + 0xc0) = (unsigned short)(param_2 != 0 ? pRect[0] : 0);
	*(unsigned short*)((char*)pThis + 0xc2) = (unsigned short)(param_2 != 0 ? pRect[1] : 0);
	*(unsigned short*)((char*)pThis + 0xc4) = 0;
	*(unsigned short*)((char*)pThis + 0xc6) = 0;
	((void(__fastcall*)(void*, int, int))0x46a530)((char*)pThis + 0x90, 0, 1);
	*(void**)((char*)pThis + 0x130) = param_4;
	*(void**)((char*)pThis + 0x138) = param_5;
	((void(__fastcall*)(void*))0x4693b0)(pThis);
	return pThis;
}

// FUNCTION: LEMBALL 0x0044f970
void __fastcall record_queued_variant_child_slot_state(void* pObject, int nUnused, int param_1, int param_2, int param_3)
{
	int iSlot = *(int*) ((char*) pObject + 0x80);
	*(int*) ((char*) pObject + 0xbc + iSlot * 0x10) = 1;
	*(int*) ((char*) pObject + 0xc0 + iSlot * 0x10) = param_1;
	*(int*) ((char*) pObject + 0xc4 + iSlot * 0x10) = param_2;
	*(int*) ((char*) pObject + 0xc8 + iSlot * 0x10) = param_3;
}

// FUNCTION: LEMBALL 0x0044d080
void __fastcall append_non_zrle_selector_child(void* pObject, int nUnused, int param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8, int param_9, unsigned short* param_10, int param_11, int param_12)
{
	int iCounter = *(int*) ((char*) pObject + 0x218) + 1;
	int nChild = *(int*) ((char*) pObject + 0x80);
	*(int*) ((char*) pObject + 0x218) = iCounter;
	void* pEntry = ((void* (__cdecl*) (unsigned int)) 0x45a780)(0x60);
	if (pEntry == 0) {
		*(int*) ((char*) pObject + nChild * 4 + 0x1e0) = 0;
	} else {
		*(int*) ((char*) pObject + nChild * 4 + 0x1e0) = (int) ((int* (__fastcall*) (void*, unsigned short*, int, int, int, int, int, int, int, int, int, int)) 0x402617)
			(pEntry, param_10, *(int*) ((char*) pObject + 0x214), *(int*) ((char*) pObject + 0x210), param_2, param_3, param_4, param_5, param_8, iCounter, param_9, param_11);
	}
	((void (__fastcall*) (void*, int, int, int, int)) 0x40165e)
		(pObject, param_2, param_3, 1, *(int*) (*(int*) ((char*) pObject + nChild * 4 + 0x1e0) + 0x30));
	*(int*) (*(int*) (*(int*) (*(int*) ((char*) pObject + nChild * 4 + 0x1e0) + 0x54) + 0x134) + 0x130) = param_12;
	*(int*) ((char*) pObject + 0x80) = nChild + 1;
}

// FUNCTION: LEMBALL 0x0044e700
void __fastcall emit_variant_render_entry_with_temporary_context(void* pObject, int nUnused, unsigned short* param_1, int param_2, unsigned short* param_3, int param_4, int param_5, int* param_6, int param_7)
{
	int nPrevContext = *(int*) ((char*) pObject + 0x1c);
	*(int*) ((char*) pObject + 0x1c) = param_2;
	unsigned short localRect[4];
	((void (__fastcall*) (void*, unsigned short*, unsigned short*, int, int, int*, int)) 0x467730)
		(pObject, &localRect[0], param_3, param_4, param_5, param_6, param_7);
	*(int*) ((char*) pObject + 0x1c) = nPrevContext;
	param_1[0] = localRect[0];
	param_1[1] = localRect[1];
	param_1[2] = localRect[2];
	param_1[3] = localRect[3];
}

// FUNCTION: LEMBALL 0x004316c0
void __fastcall DestroyPrimaryContext(void* pObject)
{
	void* pRenderQueueBase = (char*) pObject - 0x90;
	*(void**) pObject = (void*) 0x496ba8;
	*(void**) pRenderQueueBase = (void*) 0x496bb8;
	((void(__fastcall*)(void*)) 0x45d180)(*(void**) ((char*) pObject + 0x20));
	((void(__fastcall*)(void*)) 0x45d180)(*(void**) ((char*) pObject + 0x1c));
	((void(__fastcall*)(void*)) 0x45d180)(*(void**) ((char*) pObject + 0x18));
	((void(__fastcall*)(void*, int, void*, int)) 0x4633b0)(
		g_pSharedRenderDispatchQueue, 0,
		pRenderQueueBase != 0 ? pObject : 0, -0x19);
	*(void**) ((char*) pObject + 0x40) = (void*) 0x496ca8;
	*(void**) (pRenderQueueBase != 0 ? pObject : 0) = (void*) 0x493110;
	((void(__fastcall*)(void*)) 0x463bd0)(pRenderQueueBase);
}

// FUNCTION: LEMBALL 0x0043a1d0
void __fastcall SetTextOwnerStringAndLayout(void* pThis, int nUnused, char* pszText)
{
	if (*(void**) ((char*) pThis + 0x14c) != 0) {
		FreeVSMemBlock(*(void**) ((char*) pThis + 0x14c));
	}
	*(void**) ((char*) pThis + 0x14c) = AllocateVSMemBlock((unsigned int) strlen(pszText) + 1);
	memcpy(*(void**) ((char*) pThis + 0x14c), pszText, strlen(pszText) + 1);
	((void(__fastcall*)(void*, int, void*, int)) 0x469210)(pThis, 0, *(void**) ((char*) pThis + 0x14c), 0);
}
// FUNCTION: LEMBALL 0x00432590
void* __fastcall ConstructQueuedVariantChildSlotRenderPointSink(void* pThis, int nUnused, int param_2, int param_3, int param_4)
{
	int i;
	for (i = 0; i < 2; i++) {
		((void(__fastcall*)(void*)) 0x401c7b)((char*) pThis + 0x14 + i * 0x10);
	}
	*(int*) ((char*) pThis + 0x34) = 0xaa55aa55;
	*(int*) ((char*) pThis + 0x38) = 0xaa55aa55;
	*(int*) ((char*) pThis + 0x3c) = 0xaa55aa55;
	*(int*) ((char*) pThis + 0x40) = 0xaa55aa55;
	*(int*) ((char*) pThis + 0x44) = 0xaa55aa55;
	*(int*) ((char*) pThis + 0x48) = 0xaa55aa55;
	*(void**) ((char*) pThis + 0x60) = (void*) 0x496ca8;
	*(void**) ((char*) pThis + 0x60) = (void*) 0x496cb8;
	*(short*) ((char*) pThis + 0x66) = 0;
	*(int*) ((char*) pThis + 8) = param_3;
	*(short*) ((char*) pThis + 100) = 0;
	*(short*) ((char*) pThis + 0x6a) = 0;
	*(short*) ((char*) pThis + 0x68) = 0;
	*(int*) ((char*) pThis + 4) = param_4;
	*(int*) ((char*) pThis + 0x10) = param_2;
	*(int*) ((char*) pThis + 0xc) = *(int*) ((char*) param_3 + 0x164);
	*(int*) ((char*) pThis + 0x3c) = 0;
	*(int*) ((char*) pThis + 0x40) = 0;
	*(int*) ((char*) pThis + 0x44) = 0;
	*(int*) ((char*) pThis + 0x48) = 0;
	*(int*) ((char*) pThis + 0x54) = 0;
	*(int*) ((char*) pThis + 0x58) = 0;
	*(int*) ((char*) pThis + 0x34) = 0xa0000;
	*(int*) ((char*) pThis + 0x38) = 0x64000;
	((void(__fastcall*)(void*)) 0x401a78)(pThis);
	return pThis;
}

// FUNCTION: LEMBALL 0x004349e0
void __fastcall EmitLevelScreenVariantEntryWithContext(void* pThis, int nUnused, int param_2,
	unsigned short param_3, unsigned short param_4, int param_5, int param_6, int param_7)
{
	unsigned short uStack_c = param_3;
	unsigned short uStack_a = param_4;
	char auStack_8[8];
	void* puVar1;
	switch (param_5) {
	case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
	case 0xb5: case 0xb6: case 0xb7: case 0xb8: case 0xb9: case 0xba:
	case 0xbb: case 0xbc: case 0xbd: case 0xbe: case 0xbf: case 0xc0:
	case 0xc1: case 0xc2: case 0xc3: case 0xc4: case 0xc5: case 0xc6:
	case 0xc7: case 0xc8: case 0xc9: case 0xca: case 0xcb: case 0xcc:
	case 0xcd: case 0xce: case 0xcf: case 0xd0: case 0xd1: case 0xd2:
	case 0xd3: case 0xd4: case 0xd5: case 0xd6: case 0xd7: case 0xd8:
	case 0xd9: case 0xda: case 0xdb: case 0xdc: case 0xdd: case 0xde:
	case 0xdf: case 0xe0: case 0xe1: case 0xe4: case 0xe5: case 0x103: case 0x105:
		puVar1 = *(void**) (*(int*) ((char*) pThis + 0x74) +
			(short) *(short*) (*(int*) ((char*) pThis + 0x28) + param_5 * 2) * 4);
		*(int*) ((char*) puVar1 + 0xc) = param_6;
		uStack_c = param_3;
		uStack_a = param_4;
		{
			int uSaved = *(int*) ((char*) pThis + 0x1c);
			*(int*) ((char*) pThis + 0x1c) = param_2;
			((int(__fastcall*)(void*, void*, void*, int, int, void*, int)) 0x467730)
				(pThis, auStack_8, &uStack_c, param_5, 0, puVar1, param_7);
			*(int*) ((char*) pThis + 0x1c) = uSaved;
		}
		return;
	case 0xe7: case 0x104: case 0x106:
		puVar1 = *(void**) (*(int*) ((char*) pThis + 0x74) +
			(short) *(short*) (*(int*) ((char*) pThis + 0x28) + param_5 * 2) * 4);
		uStack_c = param_3;
		uStack_a = param_4;
		{
			int uSaved = *(int*) ((char*) pThis + 0x1c);
			*(int*) ((char*) pThis + 0x1c) = param_2;
			((int(__fastcall*)(void*, void*, void*, int, int, void*, int)) 0x467730)
				(pThis, auStack_8, &uStack_c, param_5, 0, puVar1, param_7);
			*(int*) ((char*) pThis + 0x1c) = uSaved;
		}
	}
}

// FUNCTION: LEMBALL 0x00432880
void __fastcall StartDirectionalOffsetAnimationIfIdle(void* pObject, int nUnusedEdx, int direction)
{
	if (*(int*) ((char*) pObject + 0x54) == 0) {
		*(int*) ((char*) pObject + 0x44) = (direction == 0) ? -0xcc : 0xcc;
		*(unsigned long*) ((char*) pObject + 0x4c) = timeGetTime();
		*(int*) ((char*) pObject + 0x54) = 1;
	}
}

// FUNCTION: LEMBALL 0x004328d0
void __fastcall StartSignedOffsetAnimationIfIdle(void* pObject, int nUnusedEdx, int direction)
{
	if (*(int*) ((char*) pObject + 0x58) == 0) {
		*(int*) ((char*) pObject + 0x48) = (direction == 0) ? -0xcc : 0xcc;
		*(unsigned long*) ((char*) pObject + 0x50) = timeGetTime();
		*(int*) ((char*) pObject + 0x58) = 1;
	}
}

// FUNCTION: LEMBALL 0x004326e0
void __fastcall IntegrateInertialCoordinateEventSource(void* pObject, int nUnusedEdx)
{
	unsigned long uNow = timeGetTime();
	if (*(int*) ((char*) pObject + 0x54) != 0) {
		int nStamp = *(int*) ((char*) pObject + 0x4c);
		*(unsigned long*) ((char*) pObject + 0x4c) = uNow;
		int v = *(int*) ((char*) pObject + 0x44) * (uNow - nStamp) / 0x14 + *(int*) ((char*) pObject + 0x3c);
		*(int*) ((char*) pObject + 0x3c) = v;
		if (v > 0x5000) *(int*) ((char*) pObject + 0x3c) = 0x5000;
		if (v < -0x5000) *(int*) ((char*) pObject + 0x3c) = -0x5000;
	}
	int nSignedBusy = *(int*) ((char*) pObject + 0x58);
	if (nSignedBusy != 0) {
		int nStamp = *(int*) ((char*) pObject + 0x50);
		*(unsigned long*) ((char*) pObject + 0x50) = uNow;
		int v = *(int*) ((char*) pObject + 0x48) * (uNow - nStamp) / 0x14 + *(int*) ((char*) pObject + 0x40);
		*(int*) ((char*) pObject + 0x40) = v;
		if (v > 0x5000) *(int*) ((char*) pObject + 0x40) = 0x5000;
		if (v < -0x5000) *(int*) ((char*) pObject + 0x40) = -0x5000;
	}
	if (*(int*) ((char*) pObject + 0x54) == 0 && nSignedBusy == 0 && *(int*) ((char*) pObject + 0x5c) == 0)
		return;
	if (*(int*) ((char*) pObject + 0x54) != 0 || nSignedBusy != 0) {
		*(int*) ((char*) pObject + 0x34) += *(int*) ((char*) pObject + 0x3c);
		*(int*) ((char*) pObject + 0x38) += *(int*) ((char*) pObject + 0x40);
	}
	((void(__fastcall*)(void*)) 0x432680)(pObject);
	*(int*) ((char*) pObject + 0x5c) = 0;
}
// FUNCTION: LEMBALL 0x00468b80
void __fastcall initialize_buffered_geometry_child_storage(void* self)
{
	int* block;
	int* entry;
	block = (int*)((void* (__cdecl*)(int))0x45a780)(0x14);
	if (block == 0) {
		*(int*)((char*)self + 0x10c) = 0;
	} else {
		*block = 1;
		entry = block + 1;
		((void (__fastcall*)(void*))0x40272f)(entry);
		*(int**)((char*)self + 0x10c) = entry;
	}
	*(int*)((char*)self + 0x50) += 1;
	block = (int*)((void* (__cdecl*)(int))0x45a780)(0x44);
	if (block != 0) {
		*block = 4;
		entry = block + 1;
		((void (__fastcall*)(void*))0x401c7b)(entry);
		((void (__fastcall*)(void*))0x401c7b)(entry + 0x10);
		((void (__fastcall*)(void*))0x401c7b)(entry + 0x20);
		((void (__fastcall*)(void*))0x401c7b)(entry + 0x30);
		*(int**)((char*)self + 0x110) = entry;
	} else {
		*(int*)((char*)self + 0x110) = 0;
	}
	*(int*)((char*)self + 0x50) += 4;
}
// FUNCTION: LEMBALL 0x00469480
void __fastcall refresh_resource_sprite_window_owner_frame_draw_entry(void* pObject)
{
	short uStack_4;
	short uStack_2;
	int iVar1;
	int iVar3;
	bool bVar2 = (*(int*) ((char*) pObject + 0xd0) != 0) && (*(int*) ((char*) pObject + 0x94) != 0);
	if (bVar2) {
		uStack_4 = *(unsigned short*) ((char*) pObject + 0x128);
		iVar3 = *(int*) ((char*) pObject + 0x11c);
		uStack_2 = *(unsigned short*) ((char*) pObject + 0x12a);
	} else {
		uStack_4 = *(unsigned short*) ((char*) pObject + 0x12c);
		iVar3 = *(int*) ((char*) pObject + 0x118);
		uStack_2 = *(unsigned short*) ((char*) pObject + 0x12e);
	}
	if (iVar3 != 0) {
		iVar1 = *(int*) (*(int*) ((char*) pObject + 0x4c) + 0xc);
		(*(void(**)(void)) (*(void***) (*(int*) (*(int*) (iVar1 + 0x40) + 4) + iVar1 + 0x40) + 0x38 / 4))();
		(*(void(**)(short*, int, int, int, int)) (*(void***) *(void**) ((char*) pObject + 0x124) + 0x14 / 4))
			(&uStack_4, *(int*) ((char*) pObject + 0x134), iVar3, 0x20, *(int*) ((char*) pObject + 0x144));
		(*(void(**)(void*)) (*(void***) *(void**) ((char*) pObject + 0x124) + 4 / 4))(*(void**) ((char*) pObject + 0x4c));
	}
}

// FUNCTION: LEMBALL 0x004429b0
void __fastcall sync_action_button_state(void* pObject, int nUnused, void* param_1)
{
	void* iVar1 = *(void**) ((char*) pObject + 0x110);
	if ((unsigned int) *(unsigned short*) (*(int*) ((char*) iVar1 + 8) + 0x228) != *(int*) ((char*) pObject + 0x164)) {
		*(int*) ((char*) pObject + 0xd8) = 1;
		*(int*) ((char*) pObject + 0x164) = (unsigned int) *(unsigned short*) (*(int*) ((char*) iVar1 + 8) + 0x228);
	}
	if (*(int*) ((char*) iVar1 + 0x1c) != *(int*) ((char*) pObject + 0x174)) {
		*(int*) ((char*) pObject + 0xd8) = 1;
		*(int*) ((char*) pObject + 0x174) = *(int*) ((char*) iVar1 + 0x1c);
	}
	if ((unsigned int) (*(int*) (*(int*) ((char*) iVar1 + 8) + 0xb8) == 8) != *(int*) ((char*) pObject + 0x168)) {
		*(int*) ((char*) pObject + 0xd8) = 1;
		*(int*) ((char*) pObject + 0x168) = (unsigned int) (*(int*) ((char*) pObject + 0x168) == 0);
	}
	{
		int iVar2 = (*(int(**)(void*, int)) (*(void***) *(void**) ((char*) iVar1 + 8) + 0xb4 / 4))(*(void**) ((char*) iVar1 + 8), 0xc);
		if (iVar2 != *(int*) ((char*) pObject + 0x16c)) {
			*(int*) ((char*) pObject + 0xd8) = 1;
			*(int*) ((char*) pObject + 0x16c) = (unsigned int) (*(int*) ((char*) pObject + 0x16c) == 0);
		}
	}
	if (*(int*) ((char*) *(void**) ((char*) pObject + 0x110) + 0x18) != *(int*) ((char*) pObject + 0x170)) {
		*(int*) ((char*) pObject + 0xd8) = 1;
		*(int*) ((char*) pObject + 0x170) = *(int*) ((char*) *(void**) ((char*) pObject + 0x110) + 0x18);
	}
	((void(__fastcall*)(void*, void*)) 0x468360)(pObject, param_1);
}
// FUNCTION: LEMBALL 0x00444b20
void __fastcall select_pause_option_at_point(void* pThis, int nUnused, short* param_1, int param_2)
{
	int iVar4 = *(int*) ((char*) pThis + 0x80);
	if (iVar4 < *(int*) ((char*) pThis + 0x7c)) {
		short sVar1 = param_1[0] - *(short*) ((char*) pThis - 0x8c);
		short sVar2 = param_1[1] - *(short*) ((char*) pThis - 0x8a);
		short* psVar3 = (short*) (*(int*) ((char*) pThis + 0x154) + 4 + iVar4 * 8);
		while (sVar1 < *psVar3
			|| (short) (psVar3[-2] + *psVar3) <= sVar1
			|| sVar2 < psVar3[1]
			|| (short) (psVar3[-1] + psVar3[1]) <= sVar2) {
			psVar3 = psVar3 + 4;
			iVar4 = iVar4 + 1;
			if (*(int*) ((char*) pThis + 0x7c) <= iVar4) {
				return;
			}
		}
		(*(void(**)(int)) (*(void***) *(void**) ((char*) pThis + 0x6c)))(*(int*) ((char*) pThis + 0x74) + 1);
		*(int*) ((char*) pThis + 0x74) = iVar4;
		*(int*) ((char*) pThis + 0x64) = 1;
		SetLevelScreenStatusIndicatorMode(1, 1);
		((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)->PlayVariantResourceEffect(3);
	}
}
// FUNCTION: LEMBALL 0x00468f00
void* __fastcall construct_single_child_overlay_owner_from_point(void* self, int nEdxSlop, void* pPoint, void* argColor, int argOriginX, void* argRes, int argOriginY)
{
	void* block;
	int* child;
	((void (__fastcall*)(void*, void*, void*))0x468b20)(self, argColor, argRes);
	*(void**)self = (void*)0x499928;
	*(void**)((char*)self + 0x90) = (void*)0x499908;
	block = (void*)((void* (__cdecl*)(int))0x45a780)(0x130);
	if (block == 0) {
		*(int*)((char*)self + 0x118) = 0;
	} else {
		child = ((int* (__fastcall*)(void*, void*, void*, int, int))0x468530)(
			block, pPoint, argColor, argOriginX, argOriginY);
		*(int**)((char*)self + 0x118) = child;
	}
	*(unsigned short*)((char*)self + 0xdc) = *(unsigned short*)pPoint;
	*(unsigned short*)((char*)self + 0xde) = *(unsigned short*)((char*)pPoint + 2);
	return self;
}
// FUNCTION: LEMBALL 0x00473eb0
void __cdecl get_window_client_rect_in_screen_coords(HWND hwnd, DebugTextRect* pRect)
{
	tagPOINT pt;
	pt.x = 0;
	pt.y = 0;
	ClientToScreen(hwnd, &pt);
	GetClientRect(hwnd, pRect);
	pRect->nTop += pt.y;
	pRect->nBottom += pt.y;
	pRect->nLeft += pt.x;
	pRect->nRight += pt.x;
}

// FUNCTION: LEMBALL 0x00474000
int __fastcall refresh_debug_text_visible_line_counts(void* pThis, int nEdxSlop)
{
	DebugTextRect rc;
	void** vtbl = *(void***)pThis;
	(*(void (__fastcall**)(void*, int))vtbl[0])(pThis, 0);
	GetClientRect(*(HWND*)((char*)pThis + 0x1c), &rc);
	int lineHeight = *(int*)((char*)pThis + 0x3c);
	*(int*)((char*)pThis + 0x30) = (rc.nBottom - rc.nTop) / lineHeight;
	*(int*)((char*)pThis + 0x34) = (lineHeight - 1 + (rc.nBottom - rc.nTop)) / lineHeight;
	(*(void (__fastcall**)(void*, int))vtbl[1])(pThis, 0);
	return *(int*)((char*)pThis + 0x34);
}
// FUNCTION: LEMBALL 0x004695d0
void* __fastcall construct_resource_sprite_window_owner_variant_a(void* pThis, void* pUnusedEdx,
	void* param_2, void* param_3, void* param_4, void* param_5, void* param_6)
{
	((void*(__fastcall*)(void*, int, void*, void*, void*, void*)) 0x468f90)(pThis, 0, param_3, param_4, param_5, param_6);
	*(void**) pThis = (void*) 0x499b28;
	*(void**) ((char*) pThis + 0x90) = (void*) 0x499b08;
	((void(__fastcall*)(void*)) 0x469670)(pThis);
	*(void**) ((char*) pThis + 0xcc) = param_2;
	return pThis;
}

// FUNCTION: LEMBALL 0x00469620
void* __fastcall construct_resource_sprite_window_owner_variant_b(void* pThis, void* pUnusedEdx,
	void* param_2, void* param_3, void* param_4, void* param_5, void* param_6)
{
	((void*(__fastcall*)(void*, int, void*, void*, void*, void*)) 0x469070)(pThis, 0, param_3, param_4, param_5, param_6);
	*(void**) pThis = (void*) 0x499b28;
	*(void**) ((char*) pThis + 0x90) = (void*) 0x499b08;
	((void(__fastcall*)(void*)) 0x469670)(pThis);
	*(void**) ((char*) pThis + 0xcc) = param_2;
	return pThis;
}

// FUNCTION: LEMBALL 0x00469810
void* __fastcall construct_resource_sprite_window_owner_variant_c(void* pThis, void* pUnusedEdx,
	void* param_2, void* param_3, void* param_4, void* param_5, void* param_6)
{
	((void*(__fastcall*)(void*, int, void*, void*, void*, void*)) 0x469070)(pThis, 0, param_3, param_4, param_5, param_6);
	*(void**) pThis = (void*) 0x499c18;
	*(void**) ((char*) pThis + 0x90) = (void*) 0x499bf8;
	((void(__fastcall*)(void*)) 0x469860)(pThis);
	*(void**) ((char*) pThis + 0xcc) = param_2;
	return pThis;
}

// FUNCTION: LEMBALL 0x00475bc0
int __fastcall classify_circle_against_helper_clip_rect(
	void* pThis, int nEdxSlop, int nCX, int nCY, int nRad)
{
	int iRect = *(int*)(*(int*)((char*)pThis + 0x40) + 4);
	char* pRect = (char*)pThis + iRect;
	short w = *(short*)(pRect + 0x64);
	short h = *(short*)(pRect + 0x66);
	int nLeft  = *(short*)(pRect + 0x68);
	int nTop   = *(short*)(pRect + 0x6a);
	if (h < 1 || w < 1)
		return 1;
	int nRight  = nLeft + w - 1;
	int nBottom = nTop + h - 1;
	int nMinX = nCX - nRad;
	int nMaxX = nCX + nRad;
	int nMinY = nCY - nRad;
	int nMaxY = nCY + nRad;
	if (nMaxX < nLeft || nRight < nMinX || nMaxY < nTop || nBottom < nMinY)
		return 1;
	if (nLeft <= nMinX && nMaxX <= nRight && nTop <= nMinY && nMaxY <= nBottom)
		return 2;
	return 3;
}

// FUNCTION: LEMBALL 0x00451630
void __fastcall rotate_password_keypad_selection(void* pThis, int nEdxSlop, int nUnusedArg)
{
	if (*(int*)((char*)pThis + 0x488) == 1)
		return;
	int nSel = *(int*)((char*)pThis + 0x480);
	void* pWidget = *(void**)((char*)pThis + 0x424 + *(int*)(0x4a0028 + nSel * 4) * 4);
	void* pSub = (char*)pWidget + 0x90;
	void** pVt = *(void***)pSub;
	unsigned short auPoint[2] = { 0, 0 };
	((void(__fastcall*)(void*, unsigned short*, int))pVt[2])(pSub, auPoint, 0);
	int nNew = nSel + auPoint[0];
	if (nNew < 0)
		nNew += 12;
	if (nNew > 11)
		nNew -= 12;
	*(int*)((char*)pThis + 0x480) = nNew;
	*(int*)((char*)pThis + 0x478) = *(int*)((char*)pThis + 0x3c0 + nNew * 8);
	*(int*)((char*)pThis + 0x47c) = *(int*)((char*)pThis + 0x3c4 + nNew * 8);
	((void(__fastcall*)(void*, int)) 0x401479)(g_pVariantResourceEntryManager, 0x1b);
}

// FUNCTION: LEMBALL 0x00469120
void __fastcall expand_text_owner_bounds_for_extent(void* pThis, int nEdxSlop, short* pExtent)
{
	short sW, sH;
	if ((int)*(short*)((char*)pThis + 0x140) * (int)*(short*)((char*)pThis + 0x142) != 0) {
		sW = *(short*)((char*)pThis + 0x140) * 2 + pExtent[0];
		sH = pExtent[1] + *(short*)((char*)pThis + 0x142) * 2;
		if (*(short*)((char*)pThis + 0xc0) < sW)
			*(short*)((char*)pThis + 0xc0) = sW;
		if (*(short*)((char*)pThis + 0xc2) < sH)
			*(short*)((char*)pThis + 0xc2) = sH;
	}
}

// FUNCTION: LEMBALL 0x00469180
void __fastcall align_text_owner_extent_within_bounds(void* pThis, int nEdxSlop, short* pPos, short* pExtent)
{
	if ((*(unsigned int*)((char*)pThis + 0x138) & 0x10) == 0) {
		if ((*(unsigned int*)((char*)pThis + 0x138) & 4) != 0) {
			pPos[0] = (short)(((int)*(short*)((char*)pThis + 0xc0) - (int)pExtent[0]) / 2);
		}
	} else {
		pPos[0] = *(short*)((char*)pThis + 0xc0) - pExtent[0];
	}
	if ((*(unsigned int*)((char*)pThis + 0x138) & 0x20) == 0) {
		if ((*(unsigned int*)((char*)pThis + 0x138) & 8) != 0) {
			pPos[1] = (short)(((int)*(short*)((char*)pThis + 0xc2) - (int)pExtent[1]) / 2);
		}
	} else {
		pPos[1] = *(short*)((char*)pThis + 0xc2) - pExtent[1];
	}
	*(unsigned int*)((char*)pThis + 0xd8) = 1;
}

// FUNCTION: LEMBALL 0x0044a330
void __fastcall initialize_level_selection_palette_remap(void* pThis, int nEdxSlop)
{
	unsigned char* pMap;
	int hPal;
	int i;
	hPal = ((int(__cdecl*)(int)) 0x45dd90)(0x10a);
	pMap = (unsigned char*) ((void*(__cdecl*)(int)) 0x45a780)(0x100);
	*(unsigned int*)((char*)pThis + 0x3cc) = (unsigned int)pMap;
	for (i = 0; i < 0x100; i++)
		pMap[i] = (char) i;
	for (i = 0; i < 0x28; i += 4) {
		if (*(int*)(0x49f880 + i) != 0)
			pMap[*(int*)(0x49f858 + i)] = (char) *(int*)(0x49f880 + i);
	}
	*(unsigned int*)((char*)pThis + 0x3d0) =
		((unsigned int(__fastcall*)(void*, int, int, void*, int)) 0x46ad70)((void*) 0x4a2000, 0, 0x10a, pMap, 0);
	((void(__fastcall*)(void*)) 0x45d180)((void*) hPal);
}
// FUNCTION: LEMBALL 0x0044f290
void __fastcall load_queued_zrle_variant_range_resources(void* pThis, int nUnused, int* param_1)
{
	void* pArray;
	void* pOverlay;
	int* pInner;
	short pt[2];
	int i;
	*(int*) ((char*) pThis + 0x48) = (int) param_1;
	pArray = ((void*(__cdecl*)(unsigned int)) 0x45a780)((unsigned int) *(int*) ((char*) pThis + 0x34) << 2);
	*(int*) ((char*) pThis + 0x50) = (int) pArray;
	for (i = 0; i < *(int*) ((char*) pThis + 0x34); i++) {
		*(int*) ((char*) pArray + i * 4) =
			(int) ((void*(__cdecl*)(int)) 0x45d610)(param_1[i]);
	}
	pOverlay = ((void*(__cdecl*)(unsigned int)) 0x45a780)(0x130);
	if (pOverlay == 0) {
		*(int*) ((char*) pThis + 0x4c) = 0;
	} else {
		pt[0] = (short) *(int*) ((char*) pThis + 0x18);
		pt[1] = (short) *(int*) ((char*) pThis + 0x1c);
		*(int*) ((char*) pThis + 0x4c) = (int) ((void*(__fastcall*)(void*, int, short*, int, int, int)) 0x468530)(
			pOverlay, 0, pt, *(int*) ((char*) pThis + 0x24),
			param_1[*(int*) ((char*) pThis + 0x40) - *(int*) ((char*) pThis + 0x3c)], 3);
	}
	pOverlay = (void*) *(int*) ((char*) pThis + 0x4c);
	pInner = (int*) *(int*) (*(int*) ((char*) pOverlay + 0x4c) + 0xc);
	((void(__fastcall*)(void*, int, int)) 0x467e40)(pOverlay, 0, 0);
	*(int*) (*(int*) (*(int*) ((char*) pInner + 0x40) + 4) + 0x70 + (int) pInner) = 0;
	*(int*) ((char*) pOverlay + 0xf4) = (int) g_pSharedRenderDispatchQueue;
	*(int*) ((char*) pOverlay + 0xcc) = *(int*) ((char*) pThis + 0x2c);
}

// FUNCTION: LEMBALL 0x0044fa00
void __fastcall sample_helper_point_into_owner_and_emit_variant_entry(void* param_1)
{
	void* pRenderContext;
	char* pGeometryOwner;
	char* pGeometryDispatch;
	int nPointBias;
	int nSavedOwner;
	short pt[2];
	char auStack_c[12];
	if (*(int*) ((char*) param_1 + 0x144) != 0) {
		pRenderContext = *(void**) ((char*) param_1 + 0x12c);
		nPointBias = *(int*) ((char*) param_1 + 0x130) == 1 ? -1 : -2;
		pGeometryOwner = *(char**) ((char*) pRenderContext + 0x0c);
		pGeometryDispatch = pGeometryOwner + 0x40 +
			*(int*) ((char*) *(void**) (pGeometryOwner + 0x40) + 4);
		*(short*) ((char*) param_1 + 0x100) = *(short*) (pGeometryDispatch + 0x14);
		*(short*) ((char*) param_1 + 0x102) = *(short*) (pGeometryDispatch + 0x16);
		*(short*) ((char*) param_1 + 0x104) = 0;
		*(short*) ((char*) param_1 + 0x106) = 0;
		*(int*) ((char*) param_1 + 0x108) = 0x10000;
		(*(void(__fastcall**)(void*, int, void*)) (*(void***) ((char*) param_1 + 0xfc))[0x4 / 4])
			((char*) param_1 + 0xfc, 0, pRenderContext);
		*(int*) ((char*) param_1 + 0x124) = 0;
		pt[0] = (short) (*(int*) ((char*) param_1 + 0x98) + nPointBias);
		pt[1] = (short) (*(int*) ((char*) param_1 + 0x9c) + nPointBias);
		nSavedOwner = *(int*) ((char*) param_1 + 0x2c);
		*(int*) ((char*) param_1 + 0x2c) = (int) pRenderContext;
		((int(__fastcall*)(void*, void*, void*, int, int, void*, int)) 0x467730)(
			(char*) param_1 + 0x10, auStack_c, pt,
			g_nQueuedVariantChildSlotManagerModeSelectedResourceId, 0,
			(char*) param_1 + 0x118, 0);
		*(int*) ((char*) param_1 + 0x2c) = nSavedOwner;
		((void(__fastcall*)(void*)) 0x4678c0)((char*) param_1 + 0x10);
	}
}

// FUNCTION: LEMBALL 0x00418be0
void __fastcall configure_managed_entity_profile_ranges(void* pThis, int nEdxSlop,
	int nCount, int nData0, int nData1, int nData2, int nData3)
{
	*(int*)((char*)pThis + 0x124) = nCount;
	if (nData0 == -1) {
		const int* pData = ((const int*) 0x0049d138) + nCount * 4;
		*(int*)((char*)pThis + 0x110) = pData[0];
		*(int*)((char*)pThis + 0x114) = pData[1];
		*(int*)((char*)pThis + 0x118) = pData[2];
		*(int*)((char*)pThis + 0x11c) = pData[3];
	} else {
		*(int*)((char*)pThis + 0x110) = nData0;
		*(int*)((char*)pThis + 0x114) = nData1;
		*(int*)((char*)pThis + 0x118) = nData2;
		*(int*)((char*)pThis + 0x11c) = nData3;
	}
	if (nCount > 0) {
		int nIndex = 0;
		int* pLo = (int*)((char*)pThis + 0xe0);
		int* pHi = pLo + 4;
		do {
			if (*pLo > 0x400 || *pLo < 0)
				*pLo = nIndex;
			if (*pHi > 0x400 || *pHi < 0)
				*pHi = nIndex;
			pLo++;
			pHi++;
			nIndex += 0x10;
			nCount--;
		} while (nCount != 0);
	}
}

// FUNCTION: LEMBALL 0x00419f30
void __cdecl ClearPlasChildRuntimeFlag_0x30WithOptionalVirtualCleanup(void* pUnused, int* pObject)
{
	int (__fastcall* pfnCheck)(void*, int);
	void* pVtbl;
	(void) pUnused;
	pVtbl = *(void**) pObject;
	pfnCheck = (int (__fastcall*) (void*, int)) *(void**) ((char*) pVtbl + 0xe4);
	if (pfnCheck(pObject, 0) != 0) {
		((void (__fastcall*) (void*, int, int, int, int)) *(void**) ((char*) pVtbl + 0xe8))(pObject, 0, 0, 0, 1);
	}
	((void (__fastcall*) (void*, int)) *(void**) ((char*) pVtbl + 0xd8))(pObject, 0);
}
// FUNCTION: LEMBALL 0x00474050
int __fastcall refresh_debug_text_client_width(void* pThis, int nEdxSlop)
{
	DebugTextRect rc;
	void** vtbl = *(void***)pThis;
	(*(void (__fastcall**)(void*, int))vtbl[0])(pThis, 0);
	GetClientRect(*(HWND*)((char*)pThis + 0x1c), &rc);
	int nWidth = rc.nRight - rc.nLeft;
	*(int*)((char*)pThis + 0x40) = nWidth;
	(*(void (__fastcall**)(void*, int))vtbl[1])(pThis, 0);
	return *(int*)((char*)pThis + 0x40);
}

// FUNCTION: LEMBALL 0x00473f80
void __fastcall invalidate_debug_text_line_range(void* pThis, int nEdxSlop, int nLine, int nCount)
{
	void** vtbl = *(void***)pThis;
	(*(void (__fastcall**)(void*, int))vtbl[0])(pThis, 0);
	int nVisible = nLine - *(int*)((char*)pThis + 0x38);
	if (nVisible >= 0 && nVisible < *(int*)((char*)pThis + 0x34)) {
		DebugTextRect rc;
		int nLineHeight = *(int*)((char*)pThis + 0x3c);
		rc.nLeft  = 0;
		rc.nTop   = nVisible * nLineHeight;
		rc.nRight = *(int*)((char*)pThis + 0x40);
		rc.nBottom = nLineHeight * nCount + rc.nTop;
		if (InvalidateRect(*(HWND*)((char*)pThis + 0x1c), &rc, 0) == 0) {
			MessageBoxA(0, (const char*)0x4a2c74, (const char*)0x4a2c6c, 0);
		}
	}
	(*(void (__fastcall**)(void*, int))vtbl[1])(pThis, 0);
}

// FUNCTION: LEMBALL 0x004744a0
void __fastcall update_text_selection_drag(void* pThis, int nEdxSlop, int nUnused, int nScreenX, int nScreenY)
{
	void** vtbl = *(void***)pThis;
	(*(void (__fastcall**)(void*, int))vtbl[0])(pThis, 0);
	if (*(int*)((char*)pThis + 0x44) != 0) {
		int nIdx = ((int(__fastcall*)(void*, int, int, int)) 0x473f00)(pThis, 0, nScreenX, nScreenY);
		*(int*)((char*)pThis + 0x48) = nIdx;
		if (*(int*)((char*)pThis + 0x2c) <= *(int*)((char*)pThis + 0x48)) {
			*(int*)((char*)pThis + 0x48) = *(int*)((char*)pThis + 0x2c) - 1;
		}
		if (*(int*)((char*)pThis + 0x48) < 0) {
			(*(void (__fastcall**)(void*, int))vtbl[1])(pThis, 0);
			return;
		}
		((void(__fastcall*)(void*, int, int)) 0x4743e0)(pThis, 0, 0);
		int nCur = *(int*)((char*)pThis + 0x48);
		if (nCur <= *(int*)((char*)pThis + 0x4c)) {
			*(int*)((char*)pThis + 0x50) = nCur;
		}
		if (*(int*)((char*)pThis + 0x4c) <= nCur) {
			*(int*)((char*)pThis + 0x54) = nCur;
		}
		((void(__fastcall*)(void*, int, int)) 0x4743e0)(pThis, 0, 1);
		((void(__fastcall*)(void*, int, int)) 0x474430)(pThis, 0, *(int*)((char*)pThis + 0x48));
	}
	(*(void (__fastcall**)(void*, int))vtbl[1])(pThis, 0);
}

// FUNCTION: LEMBALL 0x004421d0
void* __fastcall construct_text_aligned_zrle_child_overlay(
	void* pThis, unsigned long ulTextId,
	unsigned short* pPoint, unsigned long ulA, int nB, int nC)
{
	((void* (__fastcall*)(void*, unsigned short*, unsigned long, int, int))0x00468530)(
		pThis, pPoint, ulA, nB, nC);
	*(unsigned long*)pThis                = 0x004973e0;
	*(unsigned long*)((char*)pThis+0x130) = 0;
	*(unsigned long*)pThis                = 0x004972b8;
	*(unsigned long*)((char*)pThis+0x134) = ulTextId;
	*(unsigned long*)((char*)pThis+0x138) = 0;
	*(unsigned long*)((char*)pThis+0x90)  = 0x004973b8;
	*(unsigned long*)((char*)pThis+0x90)  = 0x00497290;
	*(int*)((char*)pThis+0xb8)            = 1;
	return pThis;
}

// FUNCTION: LEMBALL 0x00469530
void __fastcall queue_resource_sprite_window_owner_if_dirty(int* pThis)
{
	int* pBuffer;
	void* pQueue;
	int nRet;
	int nHelper;
	if (pThis[0x52] != pThis[0x51]) {
		pThis[0x36] = 1;
	}
	pBuffer = (int*) pThis[0x13];
	if ((*(int*)((char*)pBuffer + 4) == 0) &&
	    ((pThis[0x3e] != 0) || (pThis[0x36] != 0) || (pThis[0x34] != pThis[0x35]))) {
		nRet = (**(int(__fastcall**)(void*))(*pThis + 0x68))(pThis);
		if (nRet != 0) {
			(**(void(__fastcall**)(void*))(*pThis + 0xb8))(pThis);
			((void(__fastcall*)(int))0x468c50)((int)pThis);
			(**(void(__fastcall**)(void*))(*pThis + 0xbc))(pThis);
		}
		pQueue = *(void**)((char*)pBuffer + 0xc);
		nHelper = (**(int(__fastcall**)(void*))(*(int*)pQueue + 8))(pQueue);
		((void(__fastcall*)(void*, int, int))0x4670f0)(pBuffer, 0, pThis[0x38]);
		((void(__fastcall*)(int))0x466b60)(nHelper);
		pThis[0x3f] = 1;
	}
}
// FUNCTION: LEMBALL 0x00474290
void __fastcall scroll_debug_text_window(void* pThis, int nEdxSlop, unsigned int nCommand, int param_2)
{
	void** ppVtbl = *(void***)pThis;
	((void(__fastcall*)(void*))ppVtbl[0])(pThis);
	switch (nCommand) {
	case 0:
		*(int*) ((char*) pThis + 0x38) = *(int*) ((char*) pThis + 0x38) - 1;
		break;
	case 1:
		*(int*) ((char*) pThis + 0x38) = *(int*) ((char*) pThis + 0x38) + 1;
		break;
	case 2:
		*(int*) ((char*) pThis + 0x38) = *(int*) ((char*) pThis + 0x38) - *(int*) ((char*) pThis + 0x34);
		break;
	case 3:
		*(int*) ((char*) pThis + 0x38) = *(int*) ((char*) pThis + 0x38) + *(int*) ((char*) pThis + 0x34);
		break;
	case 4:
	case 5:
		goto lblWriteTopLine;
	case 6:
		*(int*) ((char*) pThis + 0x38) = 0;
		break;
	case 7:
		param_2 = *(int*) ((char*) pThis + 0x2c);
	lblWriteTopLine:
		*(int*) ((char*) pThis + 0x38) = param_2;
		break;
	}
	if (*(int*) ((char*) pThis + 0x2c) <= *(int*) ((char*) pThis + 0x38)) {
		*(int*) ((char*) pThis + 0x38) = *(int*) ((char*) pThis + 0x2c) - 1;
	}
	if (*(int*) ((char*) pThis + 0x38) < 0) {
		*(int*) ((char*) pThis + 0x38) = 0;
	}
	((int(__cdecl*)(HWND, int, int, int))*(void**)0x4ac8f0)(*(HWND*) ((char*) pThis + 0x1c), 1, *(int*) ((char*) pThis + 0x38), 1);
	((void(__fastcall*)(void*))0x473f60)(pThis);
	((void(__fastcall*)(void*))ppVtbl[1])(pThis);
}

// FUNCTION: LEMBALL 0x00473cf0
void __fastcall destroy_debug_text_window(void* pThis)
{
	unsigned int uVar1;
	int iVar2;
	int iVar3;
	int iVar4;
	*(void**) pThis = (void*) 0x49a4a0;
	if (*(int*) ((char*) pThis + 0x44) != 0) {
		((void(*)(void))*(void**)0x4ac88c)();
		*(int*) ((char*) pThis + 0x44) = 0;
	}
	uVar1 = *(unsigned int*) ((char*) pThis + 0x24);
	if (uVar1 != 0) {
		iVar2 = *(int*) (uVar1 + 4);
		if (iVar2 != 0) {
			iVar3 = *(int*) (iVar2 - 4);
			iVar4 = iVar2 + iVar3 * 0xc;
			while (iVar3 = iVar3 - 1, iVar3 > -1) {
				iVar4 = iVar4 - 0xc;
				((void(__fastcall*)(void*))0x4564c0)((void*) iVar4);
			}
			((void(__cdecl*)(void*))0x45a790)((void*)(iVar2 - 4));
		}
		((void(__cdecl*)(void*))0x45a790)((void*) uVar1);
	}
	*(int*) 0x4a2b7c = 0;
	*(int*) ((char*) pThis + 0x18) = 0;
	((void(__cdecl*)(void*))*(void**)0x4ac624)(*(void**) ((char*) pThis + 0x1c));
	*(void**) pThis = (void*) 0x4988d0;
	((void(__cdecl*)(void*))*(void**)0x4ac7b8)((char*) pThis + 4);
}

// FUNCTION: LEMBALL 0x004743b0
void __fastcall release_debug_text_mouse_capture(void* pThis, int nEdxSlop, int param_1, int param_2, int param_3)
{
	void** ppVtbl = *(void***)pThis;
	((void(__fastcall*)(void*))ppVtbl[0])(pThis);
	if (*(int*) ((char*) pThis + 0x44) != 0) {
		((void(*)(void))*(void**)0x4ac88c)();
		*(int*) ((char*) pThis + 0x44) = 0;
	}
	((void(__fastcall*)(void*))ppVtbl[1])(pThis);
	(void) param_1; (void) param_2; (void) param_3;
}

// FUNCTION: LEMBALL 0x00444a90
void __fastcall update_pause_dialog_hovered_option_at_point(void* p, short* param_1)
{
	int i;
	*(int*)((char*)p + 0x64) = 0;
	i = *(int*)((char*)p + 0x80);
	if (i < *(int*)((char*)p + 0x7c)) {
		short sx = (short)(param_1[0] - *(short*)((char*)p - 0x8c));
		short sy = (short)(param_1[1] - *(short*)((char*)p - 0x8a));
		short* r = (short*)(*(int*)((char*)p + 0x154) + 4 + i * 8);
		do {
			if ((r[-2] <= sx && sx < (short)(r[-2] + r[0])) &&
			    (r[-1] <= sy && sy < (short)(r[-1] + r[1]))) {
				*(int*)((char*)p + 0x64) = 4;
				*(int*)((char*)p + 0x74) = i;
				break;
			}
			r += 4;
			++i;
		} while (i < *(int*)((char*)p + 0x7c));
	}
	((void(__cdecl*)(int, int))0x401b72)(1, *(int*)((char*)p + 0x64));
}

// FUNCTION: LEMBALL 0x00451f10
void __fastcall initialize_password_entry_render_context(void* pThis)
{
	void* pLayout;
	int* pCtxSlot;
	void* pCtx;
	short vSum;
	short region[4];
	pLayout = *(void**)((char*)pThis + 0x3a8);
	vSum = (short)(*(int*)((char*)pLayout + 0x60)) + (short)(*(int*)((char*)pLayout + 0x10));
	pCtxSlot = (int*)((char*)pThis + 0x3a4);
	pCtx = ((void*(__cdecl*)(int))0x45a780)(0x90);
	if (pCtx != 0)
		pCtx = ((void*(__fastcall*)(void*))0x403571)(pCtx);
	*pCtxSlot = (int)pCtx;
	region[0] = vSum * 3;
	region[1] = vSum * 4;
	region[2] = (short)(*(int*)((char*)pLayout + 8)) - 1;
	region[3] = (short)(*(int*)((char*)pLayout + 0xc)) - 1;
	((void(__fastcall*)(void*, int, short*, int, int))(*(int*)(*(int*)pCtx + 4)))
		(pCtx, 0, region, *(int*)((char*)pThis + 0x84), 0);
	*(int*)((char*)pThis + 0x474) = *(int*)((char*)pCtx + 0x4c);
}// FUNCTION: LEMBALL 0x00412800
void* __fastcall find_counted_level_child_containing_point(void* this_, int nUnused, int x, int y, int z)
{
	int pt[3];
	((void(__fastcall*)(int*, int*))0x0040161d)(pt, &x);
	return ((void*(__fastcall*)(void*, int, int))0x00403206)(
		*(void**)((char*)this_ + 0x158), pt[0], pt[1]);
	(void) y; (void) z;
}

// FUNCTION: LEMBALL 0x00412830
void* __fastcall find_level_mode_managed_entity_at_point_by_type(void* this_, int nUnused, int x, int y, int z, int type)
{
	int pt[3];
	((void(__fastcall*)(int*, int*))0x0040161d)(pt, &x);
	return ((void*(__fastcall*)(void*, int, int, int, int))0x00402504)(
		*(void**)((char*)this_ + 0x158), pt[0], pt[1], pt[2], type);
	(void) y;
}

// FUNCTION: LEMBALL 0x00471e80
void* __fastcall delete_timed_socket_window_channel_stack_wrapper(void* pThis, void* nUnused, char param_1)
{
	((void(__fastcall*)(void*))0x46fd70)((char*)pThis + 0xc8);
	((void(__fastcall*)(void*))0x45fd80)((char*)pThis + 0x30);
	((void(__fastcall*)(void*))0x45f6c0)(pThis);
	if (param_1 & 1) {
		((void(__cdecl*)(void*))0x45a790)((char*)pThis - 0x18);
	}
	return (char*)pThis - 0x18;
	(void) nUnused;
}

// FUNCTION: LEMBALL 0x004327b0
void __fastcall emit_inertial_source_marker_render_entry(void* this_, void* nUnused, int arg)
{
	((void(__fastcall*)(void*, int, int, int, int, int, int))0x004016a9)(
		*(void**)((char*)this_ + 0x10),
		(*(int*)((char*)this_ + 0x34) >> 12) - *(int*)((char*)this_ + 0x70),
		(*(int*)((char*)this_ + 0x38) >> 12) - *(int*)((char*)this_ + 0x74),
		0xfd, 0, 0, 0);
	(void) arg;
}

// FUNCTION: LEMBALL 0x00454520
void __fastcall begin_network_lobby_text_prompt(void* this_, void* nUnused, int param_1)
{
	if (*(int*)((char*)this_ + 0x39c) != 0 &&
	    ((*(int*)((char*)this_ + 0x3bc) != 0 && param_1 != 0) ||
	     (*(int*)((char*)this_ + 0x3bc) == 0 && param_1 == 0))) {
		((void(__fastcall*)(void*))0x0040118b)(this_);
		return;
	}
	*(int*)((char*)this_ + 0x3bc) = param_1;
	*(int*)((char*)this_ + 0x3b8) = 0;
	*(int*)((char*)this_ + 0x39c) = 0;
	*(int*)((char*)this_ + 0x430) = 0;
	((void(__fastcall*)(void*))0x00402c11)(g_pActiveNetworkLobbyTransportController);
	if (g_szNetworkLobbyLocalPlayerName[0] == '\0') {
		((void(__fastcall*)(void*, int, int))0x00403085)(this_, 1, 1);
		return;
	}
	((void(__fastcall*)(void*, char*))0x00402243)(
		*(void**)((char*)this_ + 0x398), g_szNetworkLobbyLocalPlayerName);
	((void(__fastcall*)(void*, int, int))0x00403085)(this_, 1, 0);
	(void) nUnused;
}// FUNCTION: LEMBALL 0x0043a190
void __fastcall destroy_resource_window_owner_buffer_0043a190(void* pThis)
{
	*(void**)((char*)pThis + 0x000) = (void*)0x004970e0;
	*(void**)((char*)pThis - 0x090) = (void*)0x00497108;
	if (*(void**)((char*)pThis + 0x0bc) != 0) {
		((void(__cdecl*)(void*))0x45a790)(*(void**)((char*)pThis + 0x0bc));
	}
	((void(__fastcall*)(void*))0x47fccc)(pThis);
}

// FUNCTION: LEMBALL 0x00469070
void* __fastcall construct_resource_sprite_geometry_child_from_point_size(
	void* pThis, int nEdxSlop, const short* pPoint, const short* pSize,
	int nBufferSize, int nColor)
{
	((void(__fastcall*)(void*, int, int))0x468b20)(pThis, nBufferSize, 0xf7);
	*(unsigned short*)((char*)pThis + 0x128) = 0;
	*(unsigned short*)((char*)pThis + 0x12a) = 0;
	*(unsigned short*)((char*)pThis + 0x12c) = 0;
	*(unsigned short*)((char*)pThis + 0x12e) = 0;
	*(unsigned short*)((char*)pThis + 0x140) = 0;
	*(unsigned short*)((char*)pThis + 0x142) = 0;
	*(void**)((char*)pThis + 0x000) = (void*)0x00499a18;
	*(void**)((char*)pThis + 0x090) = (void*)0x004999f8;
	*(unsigned short*)((char*)pThis + 0x0dc) = pPoint[0];
	*(unsigned short*)((char*)pThis + 0x0de) = pPoint[1];
	*(int*)((char*)pThis + 0x130) = nColor;
	*(int*)((char*)pThis + 0x138) = 0x0c;
	*(unsigned short*)((char*)pThis + 0x140) = pSize[0];
	*(unsigned short*)((char*)pThis + 0x142) = pSize[1];
	((void(__fastcall*)(void*))0x4693b0)(pThis);
	return pThis;
}
// FUNCTION: LEMBALL 0x00441fe0
void __fastcall mark_level_screen_update_grid_rect(void* pThis, int nEdxSlop, short* pRect)
{
	int nX0 = pRect[2] / 16;
	int nY0 = pRect[3] / 16;
	int nWidth = (pRect[0] + pRect[2] - 1) / 16 - nX0 + 1;
	int nHeight = (pRect[1] + pRect[3] - 1) / 16 - nY0 + 1;
	int nGridW = *(short*) ((char*) pThis + 0x8);
	int nGridH = *(short*) ((char*) pThis + 0xa);
	if (nX0 < nGridW && nY0 < nGridH) {
		if (nX0 < 0) { nWidth += nX0; nX0 = 0; }
		if (nY0 < 0) { nHeight += nY0; nY0 = 0; }
		if (nGridW <= nX0 + nWidth) nWidth = nGridW - nX0;
		if (nGridH <= nY0 + nHeight) nHeight = nGridH - nY0;
		if (nWidth > 0 && nHeight > 0) {
			int nBase = nX0 + nY0 * nGridW;
			char* pBuf1 = *(char**) ((char*) pThis + 0x10) + nBase;
			char* pBuf2 = *(char**) ((char*) pThis + 0x14) + nBase;
			do {
				memset(pBuf1, 1, nWidth);
				memset(pBuf2, 1, nWidth);
				pBuf1 += nGridW;
				pBuf2 += nGridW;
			} while (--nHeight != 0);
		}
	}
	(void) nEdxSlop;
}

// FUNCTION: LEMBALL 0x00474090
void __fastcall resize_debug_text_window_to_line_height(void* pThis, int nEdxSlop, int xPos, int nRequestH)
{
	void** vtbl = *(void***) pThis;
	(*(void (__fastcall**)(void*, int)) vtbl[0])(pThis, 0);
	HWND hwnd = *(HWND*) ((char*) pThis + 0x1c);
	int nLineH = *(int*) ((char*) pThis + 0x3c);
	if (nRequestH % nLineH == 0) {
		refresh_debug_text_visible_line_counts(pThis, 0);
		refresh_debug_text_client_width(pThis, 0);
	} else {
		DebugTextRect rc;
		rc.nLeft   = 0;
		rc.nTop    = 0;
		rc.nRight  = xPos + GetSystemMetrics(2);
		rc.nBottom = nRequestH - nRequestH % nLineH + nLineH;
		AdjustWindowRect(&rc, GetWindowLongA(hwnd, -16), 0);
		SetWindowPos(hwnd, 0, 0, 0, rc.nRight - rc.nLeft, rc.nBottom - rc.nTop, 6);
	}
	(*(void (__fastcall**)(void*, int)) vtbl[1])(pThis, 0);
	(void) nEdxSlop;
}

// FUNCTION: LEMBALL 0x004432c0
int __fastcall handle_level_screen_pause_action_button_event(void* pThis, int nUnused, short* pActionButtonEvent)
{
	void* pBase = *(void**)((char*)pThis + 0x10);
	if (*(int*)((char*)pBase + 0xa7c) == 0 &&
	    *(int*)(*(char**)((char*)pBase + 0x96c) + 0x108) != 1) {
		if (*(short*)pActionButtonEvent == 4) {
			if (((int(__fastcall*)(void*, int, int))0x00402a81)(pThis, 0, *(int*)((char*)pActionButtonEvent + 8)) == 8) {
				*(int*)((char*)g_pVariantResourceEntryManager + 0x10) = 3;
				((void(__fastcall*)(void*, int, int))0x00402ed7)(pBase, 0, (unsigned int)(*(int*)((char*)pBase + 0xa7c) == 0));
				void* pCur = *(void**)((char*)pThis + 0x2c);
				int nPause = *(int*)((char*)pBase + 0xa7c);
				*(int*)((char*)pCur + 0x130) = nPause;
				*(int*)((char*)pCur + 0x104) = nPause;
				return 1;
			}
		}
	}
	return 0;
}// FUNCTION: LEMBALL 0x004422b0
void __fastcall toggle_level_pause_state(void* pThis, int nUnused, int bEnablePause)
{
	if (bEnablePause == 0) {
		unsigned int uVar3;
		int uVar1;
		int doSet;
		void* pObj;
		void* vSub;
		uVar3 = *(unsigned int*)((char*)pThis + 0x130) ^ 1;
		*(unsigned int*)((char*)pThis + 0x130) = uVar3;
		*(unsigned int*)((char*)pThis + 0x104) = uVar3;
		pObj = *(void**)((char*)pThis + 0x134);
		vSub = *(void**)((char*)pObj + 0x10);
		doSet = 0;
		if (uVar3 == 0) {
			void** pVt = *(void***)((char*)vSub + 0x4c);
			if (((int(__fastcall*)(void*))pVt[1])((char*)vSub + 0x4c) != 0)
				doSet = 1;
		} else if (*(int*)(*(char**)((char*)vSub + 0x96c) + 0x108) != 1) {
			doSet = 1;
		}
		if (doSet)
			((void(__fastcall*)(void*, int, int))0x00402ed7)(vSub, 0, uVar3);
		uVar1 = *(int*)((char*)vSub + 0xa7c);
		*(unsigned int*)((char*)pThis + 0x130) = uVar1;
		*(unsigned int*)((char*)pThis + 0x104) = uVar1;
		*(unsigned int*)((char*)pThis + 0x138) = 0;
		((void(__fastcall*)(void*, int, int, int))0x00401b72)(vSub, 0, 1, 0);
	}
	(void) nUnused;
}// FUNCTION: LEMBALL 0x00477440
void __fastcall decode_zrle_rows_gated_by_mask(void* pThis, int nUnused, int param_1, int* param_2, unsigned short param_3)
{
	short sRowBits = *(short*)(param_1 + 4);
	short sRowOff = *(short*)(param_1 + 6);
	unsigned char* pSrc = (unsigned char*) (*( unsigned char* (**)(void*)) ((void***) param_2 + 0x28 / 4))(param_2);
	int rowIndex = 0;
	if (0 < *(short*)(param_1 + 2)) {
		int rowOffset = (int)sRowOff << 2;
		do {
			unsigned char* pDst = (unsigned char*) (*(int*) (*(int*) ((char*) pThis + 4) + rowOffset) + (int)sRowBits);
			unsigned short* pMask = (unsigned short*) (*(int*) (*(int*) ((char*) pThis + 0x50) + rowOffset) + (int)sRowBits * 2);
			unsigned char b;
			do {
				b = *pSrc++;
				if (b < 0x80) {
					pDst = pDst + (unsigned char)b;
					pMask = pMask + (unsigned char)b;
				} else if (b > 0x80) {
					b = b & 0x7f;
					unsigned char* p = pSrc;
					unsigned char* d = pDst;
					unsigned short* m = pMask;
					for (unsigned char n = b; n != 0; n = n - 1) {
						if (*m <= param_3) {
							*d = *p;
						}
						d = d + 1;
						m = m + 1;
						p = p + 1;
					}
					pSrc = pSrc + (unsigned char)b;
					pDst = pDst + (unsigned char)b;
					pMask = pMask + (unsigned char)b;
				}
			} while (b != 0x80);
			rowIndex = rowIndex + 1;
			rowOffset = rowOffset + 4;
		} while (rowIndex < *(short*)(param_1 + 2));
	}
	(void) nUnused;
}

// Minimal view of GameLevelProgressState (links to CGame.cpp def; member mangling via name+signature).
struct GameLevelProgressState {
	int ValidateAndApplyPassword(char* pszPassword);
	void Snapshot(void);
};

// FUNCTION: LEMBALL 0x0040d130
void __fastcall AppendAnimChunkRecordIfUnique(void* pObject, int nUnusedEdx, short* pTile, unsigned short nFirstFrame, unsigned short nLastFrame)
{
	int nCount;
	int nIndex;
	int nRow;
	int nColumn;
	unsigned short* pWord;

	// ILT has_anim_chunk_record_at_tile (0x40291e) forwards to HasAnimChunkRecordAtTile (0x40d080).
	if (((int (__fastcall*) (void*, int, short*)) 0x0040291e)(pObject, 0, pTile) == 0) {
		nCount = *(int*) ((char*) pObject + 4);
		if (nCount < 200) {
			*(int*) ((char*) pObject + nCount * 0x18 + 0x1c) = 1;
			nIndex = *(int*) ((char*) pObject + 4);
			*(int*) ((char*) pObject + nIndex * 0x18 + 8) = *(int*) pTile;
			*(short*) ((char*) pObject + nIndex * 0x18 + 0x0c) = pTile[2];
			*(unsigned short*) ((char*) pObject + nCount * 0x18 + 0x14) = nFirstFrame;
			*(unsigned short*) ((char*) pObject + nCount * 0x18 + 0x16) = nFirstFrame;
			*(unsigned short*) ((char*) pObject + nCount * 0x18 + 0x18) = nLastFrame;
			*(short*) ((char*) pObject + nCount * 0x18 + 0x1a) = (nLastFrame < nFirstFrame) ? -1 : 1;
			nRow = ((int) (short) pTile[1] + ((int) (short) pTile[1] >> 31 & 0x0f)) >> 4;
			nColumn = ((int) (short) pTile[0] + ((int) (short) pTile[0] >> 31 & 0x0f)) >> 4;
			*(int*) ((char*) pObject + nCount * 0x18 + 0x10) =
				(nRow * *(int*) ((char*) g_pAnimChunkTileGrid + 0x10) + nColumn) * 0x0c
				+ *(int*) ((char*) g_pAnimChunkTileGrid + 0x0c);
			nCount = *(int*) ((char*) pObject + 4) + 1;
			*(int*) ((char*) pObject + 4) = nCount;
			if (nCount > 0) {
				pWord = (unsigned short*) ((char*) pObject + 0x14);
				nIndex = 0;
				do {
					*pWord = pWord[1];
					++nIndex;
					pWord = (unsigned short*) ((char*) pWord + 0x18);
				} while (nIndex < *(int*) ((char*) pObject + 4));
			}
		}
	}
	(void) nUnusedEdx;
}

// FUNCTION: LEMBALL 0x00451d20
void __fastcall HandlePasswordEntryAction(void* pObject, int nUnused, int nAction)
{
	int nResult;
	unsigned long nTime;
	int nLength;

	if (*(int*) ((char*) pObject + 0x488) == 1) {
		return;
	}
	switch (nAction) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		nLength = *(int*) ((char*) pObject + 0x420);
		if (nLength > 9) {
			((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)->PlayVariantResourceEffect(0x19);
			return;
		}
		*(char*) ((char*) pObject + nLength + 0x3b0) = (char) nAction + '0';
		*(int*) ((char*) pObject + 0x420) = nLength + 1;
		return;
	case 10:
		nLength = *(int*) ((char*) pObject + 0x420);
		if (nLength < 1) {
			((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)->PlayVariantResourceEffect(0x19);
		} else {
			*(int*) ((char*) pObject + 0x420) = nLength - 1;
		}
		nLength = *(int*) ((char*) pObject + 0x420);
		if (nLength >= 0 && nLength < 10) {
			*(char*) ((char*) pObject + nLength + 0x3b0) = 0x2d;
			return;
		}
		break;
	case 11:
		nResult = ((GameLevelProgressState*) g_pLevelProgressState)->ValidateAndApplyPassword((char*) ((char*) pObject + 0x3b0));
		*(int*) ((char*) pObject + 0x484) = nResult;
		((void (__fastcall*) (void*)) (*(void***) *(void**) pObject + 0x4c / 4))(pObject);
		((GameLevelProgressState*) g_pLevelProgressState)->Snapshot();
		((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)->PlayVariantResourceEffect((nResult == 0) ? 0x22 : 0x13);
		nTime = ((unsigned (*) (void)) 0x00462e80)();
		*(int*) ((char*) pObject + 0x488) = 1;
		*(unsigned long*) ((char*) pObject + 0x48c) = nTime;
		*(unsigned long*) ((char*) pObject + 0x490) = nTime + 1000;
		break;
	}
	(void) nUnused;
}

// FUNCTION: LEMBALL 0x00477130
void __fastcall decode_zrle_rows(void* pThis, int nUnused, int param_1, int* param_2, int param_3)
{
	short sRowBits = *(short*)(param_1 + 4);
	short sRowOff = *(short*)(param_1 + 6);
	int step = 1;
	int rowOffset = (int)sRowOff;
	int rowIndex = 0;
	unsigned char* pSrc = (unsigned char*) (*( unsigned char* (**)(void*)) ((void***) param_2 + 0x28 / 4))(param_2);
	if (param_3 != 0) {
		step = -1;
		rowOffset = rowOffset + *(short*)(param_1 + 2) - 1;
	}
	if (0 < *(short*)(param_1 + 2)) {
		rowOffset = rowOffset << 2;
		do {
			unsigned char* pDst;
			unsigned char b;
			pDst = (unsigned char*) (*(int*) (*(int*) ((char*) pThis + 4) + rowOffset) + (int)sRowBits);
			do {
				b = *pSrc++;
				if (b < 0x80) {
					pDst = pDst + (unsigned char)b;
				} else if (b > 0x80) {
					unsigned char* p;
					unsigned char* d;
					unsigned int nDwords;
					b = b & 0x7f;
					p = pSrc;
					d = pDst;
					for (nDwords = (unsigned int)b >> 2; nDwords != 0; nDwords = nDwords - 1) {
						*(unsigned int*)d = *(unsigned int*)p;
						p = p + 4;
						d = d + 4;
					}
					for (nDwords = (unsigned int)b & 3; nDwords != 0; nDwords = nDwords - 1) {
						*d = *p;
						p = p + 1;
						d = d + 1;
					}
					pSrc = pSrc + (unsigned char)b;
					pDst = pDst + (unsigned char)b;
				}
			} while (b != 0x80);
			rowIndex = rowIndex + 1;
			rowOffset = rowOffset + step * 4;
		} while (rowIndex < *(short*)(param_1 + 2));
	}
	(void) nUnused;
}

// FUNCTION: LEMBALL 0x00477660
void __fastcall decode_zrle_rows_mirrored(void* pThis, int nUnused, int param_1, int* param_2, int param_3)
{
	int baseOffset;
	int step;
	int rowOffset;
	int rowIndex;
	unsigned char* pSrc;

	baseOffset = (int) *(short*) (param_1 + 4) + (int) *(short*) param_1 - 1;
	step = 1;
	rowOffset = (int) *(short*) (param_1 + 6);
	if (param_3 != 0) {
		step = -1;
		rowOffset += *(short*) (param_1 + 2) - 1;
	}
	pSrc = (unsigned char*) (*(unsigned char* (**)(void*)) ((void***) param_2 + 0x28 / 4))(param_2);
	rowIndex = 0;
	if (0 < *(short*) (param_1 + 2)) {
		rowOffset <<= 2;
		step <<= 2;
		do {
			unsigned char* pDst;
			unsigned char b;
			unsigned char* p;
			unsigned char* d;
			int n;

			pDst = (unsigned char*) (*(int*) (*(int*) ((char*) pThis + 4) + rowOffset) + baseOffset);
			do {
				b = *pSrc++;
				if (b < 0x80) {
					pDst -= (unsigned char) b;
				} else if (b > 0x80) {
					b &= 0x7f;
					p = pSrc;
					d = pDst;
					for (n = (int) b; n > 0; --n) {
						*d-- = *p++;
					}
					pSrc += (unsigned char) b;
					pDst -= (unsigned char) b;
				}
			} while (b != 0x80);
			rowOffset += step;
			++rowIndex;
		} while (rowIndex < *(short*) (param_1 + 2));
	}
	(void) nUnused;
}

// FUNCTION: LEMBALL 0x004781e0
void __fastcall decode_zrle_rows_palette(
	void* pThis, int nUnused, int param_1, int* param_2, int param_3, unsigned char* pPalette)
{
	short sRowBits;
	int step;
	int rowOffset;
	int rowIndex;
	unsigned char* pSrc;

	sRowBits = *(short*) (param_1 + 4);
	rowOffset = (int) *(short*) (param_1 + 6);
	step = 1;
	if (param_3 != 0) {
		step = -1;
		rowOffset += *(short*) (param_1 + 2) - 1;
	}
	pSrc = (unsigned char*) (*(unsigned char* (**)(void*)) ((void***) param_2 + 0x28 / 4))(param_2);
	rowIndex = 0;
	if (0 < *(short*) (param_1 + 2)) {
		rowOffset <<= 2;
		do {
			unsigned char* pDst;
			unsigned char b;
			unsigned char* p;
			unsigned char* d;
			unsigned int n;

			pDst = (unsigned char*) (*(int*) (*(int*) ((char*) pThis + 4) + rowOffset) + (int) sRowBits);
			do {
				b = *pSrc++;
				if (b < 0x80) {
					pDst += (unsigned char) b;
				} else if (b > 0x80) {
					b &= 0x7f;
					p = pSrc;
					d = pDst;
					for (n = (unsigned int) b; n != 0; --n) {
						*d++ = pPalette[*p++];
					}
					pSrc += (unsigned char) b;
					pDst += (unsigned char) b;
				}
			} while (b != 0x80);
			rowOffset += step * 4;
			++rowIndex;
		} while (rowIndex < *(short*) (param_1 + 2));
	}
	(void) nUnused;
}

// FUNCTION: LEMBALL 0x004782d0
void __fastcall decode_zrle_rows_palette_mirrored(
	void* pThis, int nUnused, int param_1, int* param_2, int param_3, unsigned char* pPalette)
{
	short sRowBits;
	short sBaseX;
	int step;
	int rowOffset;
	int rowIndex;
	unsigned char* pSrc;

	sRowBits = *(short*) (param_1 + 4);
	sBaseX = *(short*) param_1;
	rowOffset = (int) *(short*) (param_1 + 6);
	step = 1;
	if (param_3 != 0) {
		step = -1;
		rowOffset += *(short*) (param_1 + 2) - 1;
	}
	pSrc = (unsigned char*) (*(unsigned char* (**)(void*)) ((void***) param_2 + 0x28 / 4))(param_2);
	rowIndex = 0;
	if (0 < *(short*) (param_1 + 2)) {
		rowOffset <<= 2;
		do {
			unsigned char* pDst;
			unsigned char b;
			unsigned char* p;
			unsigned char* d;
			unsigned int n;

			pDst = (unsigned char*)
				(*(int*) (*(int*) ((char*) pThis + 4) + rowOffset) + (int) sBaseX + (int) sRowBits - 1);
			do {
				b = *pSrc++;
				if (b < 0x80) {
					pDst -= (unsigned char) b;
				} else if (b > 0x80) {
					b &= 0x7f;
					p = pSrc;
					d = pDst;
					for (n = (unsigned int) b; n != 0; --n) {
						*d-- = pPalette[*p++];
					}
					pSrc += (unsigned char) b;
					pDst -= (unsigned char) b;
				}
			} while (b != 0x80);
			rowOffset += step * 4;
			++rowIndex;
		} while (rowIndex < *(short*) (param_1 + 2));
	}
	(void) nUnused;
}

// FUNCTION: LEMBALL 0x00477200
void __fastcall decode_zrle_rows_write_mask(
	void* pThis, int nUnused, int param_1, int* param_2, unsigned short nMaskValue)
{
	short sRowBits;
	short sRowOff;
	unsigned char* pSrc;
	int rowIndex;
	int rowOffset;
	unsigned int nMaskPattern;

	sRowBits = *(short*) (param_1 + 4);
	sRowOff = *(short*) (param_1 + 6);
	pSrc = (unsigned char*) (*(unsigned char* (**)(void*)) ((void***) param_2 + 0x28 / 4))(param_2);
	rowIndex = 0;
	nMaskPattern = (unsigned int) nMaskValue | ((unsigned int) nMaskValue << 16);
	if (0 < *(short*) (param_1 + 2)) {
		rowOffset = (int) sRowOff << 2;
		do {
			unsigned char* pDst;
			unsigned short* pMask;
			unsigned char b;
			unsigned char* p;
			unsigned char* d;
			unsigned int* m;
			unsigned int n;
			unsigned int nCount;

			pDst = (unsigned char*) (*(int*) (*(int*) ((char*) pThis + 4) + rowOffset) + (int) sRowBits);
			pMask = (unsigned short*)
				(*(int*) (*(int*) ((char*) pThis + 0x50) + rowOffset) + (int) sRowBits * 2);
			do {
				b = *pSrc++;
				if (b < 0x80) {
					pDst += (unsigned char) b;
					pMask += (unsigned char) b;
				} else if (b > 0x80) {
					b &= 0x7f;
					nCount = (unsigned int) b;
					p = pSrc;
					d = pDst;
					for (n = nCount >> 2; n != 0; --n) {
						*(unsigned int*) d = *(unsigned int*) p;
						p += 4;
						d += 4;
					}
					for (n = nCount & 3; n != 0; --n) {
						*d++ = *p++;
					}
					if (b != 0) {
						m = (unsigned int*) pMask;
						for (n = nCount >> 1; n != 0; --n) {
							*m++ = nMaskPattern;
						}
						for (n = nCount & 1; n != 0; --n) {
							*(unsigned short*) m = nMaskValue;
							m = (unsigned int*) ((char*) m + 2);
						}
					}
					pDst += nCount;
					pMask += nCount;
					pSrc += nCount;
				}
			} while (b != 0x80);
			++rowIndex;
			rowOffset += 4;
		} while (rowIndex < *(short*) (param_1 + 2));
	}
	(void) nUnused;
}

// FUNCTION: LEMBALL 0x00477540
void __fastcall decode_zrle_rows_palette_gated_by_mask(
	void* pThis,
	int nUnused,
	int param_1,
	int* param_2,
	unsigned short nMaskLimit,
	unsigned char* pPalette)
{
	short sRowBits;
	short sRowOff;
	unsigned char* pSrc;
	int rowIndex;
	int rowOffset;

	sRowBits = *(short*) (param_1 + 4);
	sRowOff = *(short*) (param_1 + 6);
	pSrc = (unsigned char*) (*(unsigned char* (**)(void*)) ((void***) param_2 + 0x28 / 4))(param_2);
	rowIndex = 0;
	if (0 < *(short*) (param_1 + 2)) {
		rowOffset = (int) sRowOff << 2;
		do {
			unsigned char* pDst;
			unsigned short* pMask;
			unsigned char b;
			unsigned char* p;
			unsigned char* d;
			unsigned short* m;
			unsigned int n;
			unsigned int nCount;

			pDst = (unsigned char*) (*(int*) (*(int*) ((char*) pThis + 4) + rowOffset) + (int) sRowBits);
			pMask = (unsigned short*)
				(*(int*) (*(int*) ((char*) pThis + 0x50) + rowOffset) + (int) sRowBits * 2);
			do {
				b = *pSrc++;
				if (b < 0x80) {
					nCount = (unsigned int) b;
					pDst += nCount;
					pMask += nCount;
				} else if (b > 0x80) {
					b &= 0x7f;
					nCount = (unsigned int) b;
					p = pSrc;
					d = pDst;
					m = pMask;
					for (n = nCount; n != 0; --n) {
						if (*m <= nMaskLimit) {
							*d = pPalette[*p];
						}
						++m;
						++d;
						++p;
					}
					pSrc += nCount;
					pDst += nCount;
					pMask += nCount;
				}
			} while (b != 0x80);
			++rowIndex;
			rowOffset += 4;
		} while (rowIndex < *(short*) (param_1 + 2));
	}
	(void) nUnused;
}

// FUNCTION: LEMBALL 0x00477310
void __fastcall decode_zrle_rows_palette_write_mask(
	void* pThis,
	int nUnused,
	int param_1,
	int* param_2,
	unsigned short nMaskValue,
	unsigned char* pPalette)
{
	short sRowBits;
	short sRowOff;
	unsigned char* pSrc;
	int rowIndex;
	int rowOffset;
	unsigned int nMaskPattern;

	sRowBits = *(short*) (param_1 + 4);
	sRowOff = *(short*) (param_1 + 6);
	pSrc = (unsigned char*) (*(unsigned char* (**)(void*)) ((void***) param_2 + 0x28 / 4))(param_2);
	rowIndex = 0;
	nMaskPattern = (unsigned int) nMaskValue | ((unsigned int) nMaskValue << 16);
	if (0 < *(short*) (param_1 + 2)) {
		rowOffset = (int) sRowOff << 2;
		do {
			unsigned char* pDst;
			unsigned short* pMask;
			unsigned char b;
			unsigned char* p;
			unsigned char* d;
			unsigned int* m;
			unsigned int n;
			unsigned int nCount;

			pDst = (unsigned char*) (*(int*) (*(int*) ((char*) pThis + 4) + rowOffset) + (int) sRowBits);
			pMask = (unsigned short*)
				(*(int*) (*(int*) ((char*) pThis + 0x50) + rowOffset) + (int) sRowBits * 2);
			do {
				b = *pSrc++;
				if (b < 0x80) {
					pDst += (unsigned char) b;
					pMask += (unsigned char) b;
				} else if (b > 0x80) {
					b &= 0x7f;
					nCount = (unsigned int) b;
					p = pSrc;
					d = pDst;
					for (n = nCount; n != 0; --n) {
						*d++ = pPalette[*p++];
					}
					if (b != 0) {
						m = (unsigned int*) pMask;
						for (n = nCount >> 1; n != 0; --n) {
							*m++ = nMaskPattern;
						}
						for (n = nCount & 1; n != 0; --n) {
							*(unsigned short*) m = nMaskValue;
							m = (unsigned int*) ((char*) m + 2);
						}
					}
					pDst += nCount;
					pMask += nCount;
					pSrc += nCount;
				}
			} while (b != 0x80);
			++rowIndex;
			rowOffset += 4;
		} while (rowIndex < *(short*) (param_1 + 2));
	}
	(void) nUnused;
}
extern void* g_pCachedChunkManagerEntityLookup;
class CFormationManager;
extern CFormationManager* g_pGenericGroupFormationManager;
extern void __fastcall AppendGeometryOwnerChild(void* pOwner, int nUnusedEdx, void* pChildOwner);

// FUNCTION: LEMBALL 0x0041f170
void __fastcall CreateShpgGroupWithFourChildren(
	void* pManager, int nUnusedEdx, int nX, int nY, int nZ)
{
	typedef void(__fastcall* NoArgVirtualProc)(void*);
	typedef void(__fastcall* OneArgVirtualProc)(void*, int, int);
	void* pGroup;
	void* pChild;
	void* pMemory;
	int nRemaining;

	pGroup = 0;
	pMemory = AllocateVSMemBlock(0x16c);
	if (pMemory != 0) {
		pGroup = ((void*(__fastcall*)(void*, int, void*, void*, void*)) 0x402e2d)(
			pMemory,
			0,
			g_pCachedChunkManagerLevelMode,
			g_pCachedChunkManagerEntityLookup,
			g_pGenericGroupFormationManager);
	}
	((NoArgVirtualProc) (*(void***) pGroup)[0x104 / 4])(pGroup);
	((OneArgVirtualProc) (*(void***) pGroup)[0x110 / 4])(pGroup, 0, 1);
	((void(__fastcall*)(void*, int, void*)) 0x4027e8)(pManager, 0, pGroup);
	nRemaining = 4;
	do {
		pChild = 0;
		pMemory = AllocateVSMemBlock(0x124);
		if (pMemory != 0) {
			pChild = ((void*(__fastcall*)(void*, int, void*, int, int, int, int)) 0x40234c)(
				pMemory, 0, g_pCachedChunkManagerLevelMode, nX, nY, nZ, 0);
		}
		nX -= 12;
		nY -= 12;
		((NoArgVirtualProc) (*(void***) pChild)[0x104 / 4])(pChild);
		((void(__fastcall*)(void*, int, void*, void*)) 0x4017df)(pManager, 0, pChild, pGroup);
		--nRemaining;
	} while (nRemaining != 0);
	((OneArgVirtualProc) (*(void***) pGroup)[0x154 / 4])(
		pGroup, 0, (int) g_pGenericGroupFormationManager);
	(void) nUnusedEdx;
}

struct BufferedGeometryChildWindowView {
	virtual void ReservedSlot0(void);
	virtual void ConfigureBufferedGeometryChild(short* pRect, void* pOwner, const char* pszName);
};

// FUNCTION: LEMBALL 0x00468a40
void* __fastcall ConstructBufferedGeometryChildOverlay(
	void* pThis, int nUnusedEdx, void* pRect, void* pRenderOwner, void* pOverlayOwner)
{
	short aRect[4];
	char* pChildSubobject;
	char* pBytes;
	void* pAdjustedChild;

	pChildSubobject = (char*) pThis + 0x90;
	pBytes = (char*) pThis;
	((void(__fastcall*)(void*, int, void*, void*)) 0x467c10)(pThis, 0, pRect, pRenderOwner);
	*(void**) pBytes = (void*) 0x497630;
	*(void**) pChildSubobject = (void*) 0x497608;
	*(int*) (pBytes + 0x108) = 0;
	*(int*) (pBytes + 0x104) = 0;
	*(void**) pBytes = (void*) 0x499838;
	*(void**) pChildSubobject = (void*) 0x499818;
	*(void**) (pBytes + 0x114) = pOverlayOwner;
	initialize_buffered_geometry_child_storage(pThis);
	aRect[0] = *(short*) (pBytes + 0xc0);
	aRect[1] = *(short*) (pBytes + 0xc2);
	aRect[2] = *(short*) (pBytes + 0xdc);
	aRect[3] = *(short*) (pBytes + 0xde);
	((BufferedGeometryChildWindowView*) pThis)
		->ConfigureBufferedGeometryChild(aRect, *(void**) (pBytes + 0xc8), (const char*) 0x49f02c);
	*(short*) (pBytes + 0xc4) += *(short*) (pBytes + 0x18);
	*(short*) (pBytes + 0xc6) += *(short*) (pBytes + 0x1a);
	pAdjustedChild = pThis != 0 ? pChildSubobject : 0;
	AppendGeometryOwnerChild(*(void**) (*(int*) (pBytes + 0xc8) + 0x1c), 0, pAdjustedChild);
	(void) nUnusedEdx;
	return pThis;
}
// FUNCTION: LEMBALL 0x00413380
void __fastcall refresh_level_mode_lift_boundary_columns(void* pObject)
{
	((void(__fastcall*)(void*)) 0x40282e)(*(void**) ((char*) pObject + 0x18c));
}

// FUNCTION: LEMBALL 0x00412870
void* __fastcall find_level_mode_entity_by_type_in_rect(void* pObject, int nUnused, short* param_2, int param_3)
{
	return ((void*(__fastcall*)(void*, short*, int)) 0x401e51)(*(void**) ((char*) pObject + 0x158), param_2, param_3);
	(void) nUnused;
}

// FUNCTION: LEMBALL 0x0044f400
void* __fastcall construct_queued_variant_child_context_object(void* pObject)
{
	((void(__fastcall*)(void*)) 0x463b50)(pObject);
	*(void**) pObject = (void*) 0x498160;
	*(int*) ((char*) pObject + 0x50) = *(int*) ((char*) pObject + 0x50) + 1;
	return pObject;
}

// FUNCTION: LEMBALL 0x00439840
void __fastcall delete_large_render_adjusted(void* pObject)
{
	((void(__fastcall*)(void*)) 0x403521)((char*)pObject - 4);
}

// FUNCTION: LEMBALL 0x004511f0
void __fastcall delete_paintball_screen_adjusted(void* pObject)
{
	((void(__fastcall*)(void*)) 0x403035)((char*)pObject - 0xc);
}
