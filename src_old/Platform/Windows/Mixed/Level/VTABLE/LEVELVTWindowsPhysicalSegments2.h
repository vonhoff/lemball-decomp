#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_001)
// Shared declarations required by the preserved mixed-owner compilation sequence.
#include "Visos/Generic/Memory.h"
#include "views/2d/C2DRender.h"

extern void __fastcall AppendType18ChunkObject(void* pStream,
											   int nUnused,
											   unsigned short param_1,
											   void* param_2,
											   int param_3,
											   int param_4);
extern void __fastcall DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void __fastcall ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void __fastcall ResetRockChunkObjectRuntimeStateThunk(void* pObject);
extern void __fastcall DestroyResourceSpriteGeometryChild(void* pObject);
extern void __fastcall ReleaseTypedResourceObjectIfLoaded(void* pObject, void* pUnusedEdx, int fReleaseMode);
extern void* g_pActiveManagedEntityOwner;
extern void* g_pSharedRenderDispatchQueue;
extern void* g_pVariantResourceEntryManager;
extern void* g_pLevelProgressState;
extern int g_nQueuedVariantChildSlotManagerModeSelectedResourceId;
extern void* g_pActiveNetworkLobbyTransportController;
extern char g_szNetworkLobbyLocalPlayerName[0x10];
extern char g_szNetworkLobbyJoinAddress[0x10];
extern void* g_pAudioManager;
extern int g_fEffectsOptionAvailable;
extern int g_fVariantResourceEffectsEnabled;
extern void SetLevelScreenStatusIndicatorMode(int nMode, int nValue);
extern void SetLevelScreenStatusIndicatorModeThunk(int nMode, int nValue);
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
	int nCount = 0;
	int nIndex = 0;
	int* pPeerStream = (int*) ((char*) pObject + 0x20);
	for (; nIndex < 10; pPeerStream++, nIndex++) {
		if (*pPeerStream != 0 && *(int*) (*(int*) ((char*) pObject + 0x14) + 0x4c + nIndex * 0x50) != 0) {
			nCount++;
		}
	}
	return nCount;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_009)
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
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_013)
// Compiler-state preservation body for the duplicate CSheep::Restart reconstruction.
void __fastcall PreserveActivateShpgChunkObjectCodegen(void* pObject)
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
	*(int*) (*(int*) ((char*) g_pActiveManagedEntityOwner + 0x120) +
			 *(int*) ((char*) g_pActiveManagedEntityOwner + 0x118) * 4) = (int) pObject;
	*(int*) ((char*) g_pActiveManagedEntityOwner + 0x118) = *(int*) ((char*) g_pActiveManagedEntityOwner + 0x118) + 1;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_015)
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
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_036)
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
				*(int*) (*(int**) ((char*) g_pActiveManagedEntityOwner + 0x120) + j * 4) =
					*(int*) (*(int**) ((char*) g_pActiveManagedEntityOwner + 0x120) + j * 4 + 1);
			}
		}
		*(int*) (*(int**) ((char*) g_pActiveManagedEntityOwner + 0x120) + *piVar2 * 4) = 0;
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_039)
// FUNCTION: LEMBALL 0x004140d0
int __cdecl IsManagedEntityWithinFollowupActivationRange(int param_1,
														 int param_2,
														 int param_3,
														 int param_4,
														 int param_5,
														 int param_6)
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
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_040)
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
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_042)
// FUNCTION: LEMBALL 0x00419ed0
void __cdecl ResetThenForwardManagedEntityVslot0x60(int param_1, int* param_2)
{
	int iVar1 = *param_2;
	int iVar2 = (*(int (**)(void))(*(void***) iVar1 + 0xe4 / 4))();
	if (iVar2 != 0) {
		(*(void (**)(int, int, int))(*(void***) iVar1 + 0xe8 / 4))(0, 0, 1);
	}
	(*(void (**)(void))(*(void***) iVar1 + 0x60 / 4))();
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_050)
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
	unsigned int uVar2 =
		((unsigned int(__fastcall*)(void*, int, int, int)) 0x403346)(param_1, copy[0], copy[1], copy[2]);
	return bVar1 & uVar2;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_051)
// FUNCTION: LEMBALL 0x00419d90
void __cdecl ResetScheduleAndAdvanceManagedEntity(int param_1, int* param_2)
{
	int iVar1 = *param_2;
	(*(void (**)(void*))(*(void***) iVar1 + 0x94 / 4))(param_2);
	(*(void (**)(void*, int))(*(void***) iVar1 + 0x84 / 4))(param_2, 4000);
	((void(__cdecl*)(int, int*, int)) 0x40385a)(param_1, param_2, 0);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_054)
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
			if (*(int*) (*(int*) (iVar4 + (int) iVar1) + 0x138) != 0 ||
				*(int*) (*(int*) (iVar4 + (int) iVar1) + 0x98) != 0) {
				iVar3 = param_1 + 0x4c;
				local_4 = local_4 + 1;
				(*(void (**)(int))(*(void***) **(int**) (iVar4 + (int) *(int**) ((char*) pThis + 0x34)) + 0xc / 4))(
					param_1);
			}
			iVar4 = iVar4 + 4;
			iVar2 = iVar2 + 1;
			param_1 = iVar3;
		} while (iVar2 < *(int*) ((char*) pThis + 0x3c));
	}
	return local_4;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_070)
// FUNCTION: LEMBALL 0x004148f0
void __fastcall ClearExistingWaypoints(void* pThis)
{
	int nVtbl = *(int*) pThis;
	((void(__fastcall*)(void*)) 0x402ab8)(pThis);
	if (*(int*) ((char*) pThis + 0x170) != 0) {
		if (((int(__fastcall*)(void*))(*(int*) (nVtbl + 0x13c)))(pThis) == 3) {
			int* pGroup = *(int**) ((char*) pThis + 0x170);
			if (*(int*) ((char*) pGroup + 0x38) != 0 && *(int*) ((char*) pGroup + 0x8c) != 0) {
				if (*(int*) 0x4a011c != 0) {
					((void(__fastcall*)(void*))(*(int*) *pGroup + 0xf8))(pGroup);
				}
				*(int*) ((char*) pGroup + 0x8c) = 0;
			}
		}
		*(int*) ((char*) pThis + 0x16c) = ((int(__fastcall*)(void*))(*(int*) (nVtbl + 0x108)))(pThis);
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_076)
// FUNCTION: LEMBALL 0x00419dd0
void __fastcall ResetManagedEntityMotionWith4000TickDelay(void* pThis, int nUnused, void* pObject)
{
	int* pVtbl;
	((void(__fastcall*)(void*)) 0x40207c)(pObject);
	pVtbl = *(int**) pObject;
	((void(__fastcall*)(void*, int))(pVtbl[0x84 / 4]))(pObject, 4000);
	((void(__fastcall*)(void*))(pVtbl[0x30 / 4]))(pObject);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_079)
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
			((void(__fastcall*)(void*, int, unsigned int, unsigned short)) 0x403161)(pThis, nUnused, second, first);
			nRemain--;
		} while (nRemain != 0);
	}
	return *ppStream;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_080)
// FUNCTION: LEMBALL 0x0040acf0
void __cdecl invoke_callback_grid(int nBase, int nStride, int nCount, void(__fastcall* pCallback)(int))
{
	int value = nBase + nCount * nStride;
	while (nCount > 0) {
		value = value - nStride;
		pCallback(value);
		nCount = nCount - 1;
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_081)
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
	((void(__fastcall*)(void*, int, void*))(*(void***) pThis)[0x44 / 4])(pThis, 0, pObject);
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
	((void(__fastcall*)(void*, int)) ppVtbl[0xc8 / 4])(pObject, 0);
	if (pObject != 0) {
		((void(__fastcall*)(void*, int, unsigned char)) ppVtbl[0])(pObject, 0, 1);
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_084)
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
		uLiftId = ((unsigned short(__fastcall*)(void*, int, int)) 0x402126)(g_pActiveManagedEntityOwner,
																			0,
																			*(int*) ((char*) pThis + 0x150));
		((void(__fastcall*)(void*, int, int, unsigned short)) 0x403161)(pThis, 0, 1, uLiftId);
		return;
	}
	if (nMode == 2) {
		nIndex = *(int*) ((char*) pThis + 0x150);
		if (nIndex < *(int*) ((char*) pThis + 0x154)) {
			do {
				uLiftId =
					((unsigned short(__fastcall*)(void*, int, int)) 0x402126)(g_pActiveManagedEntityOwner, 0, nIndex);
				((void(__fastcall*)(void*, int, int, unsigned short)) 0x403161)(pThis, 0, 1, uLiftId);
				nIndex = nIndex + 1;
			} while (nIndex < *(int*) ((char*) pThis + 0x154));
		}
		return;
	}
	if (nMode != 3) {
		return;
	}
	sDoorId = ((
		short(__fastcall*)(void*, int, int)) 0x40344a)(g_pActiveManagedEntityOwner, 0, *(int*) ((char*) pThis + 0x150));
	if (sDoorId != -1) {
		((void(__fastcall*)(void*, int, int, short)) 0x403161)(pThis, 0, 3, sDoorId);
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_087)
// FUNCTION: LEMBALL 0x00422710
int __fastcall remove_managed_entity_array_entry_and_release(void* pArrayOwner, int nUnusedEdx, void* pElement)
{
	typedef void(__fastcall * RefreshProc)(void* pObject);
	typedef void(__fastcall * DeleteProc)(void* pObject, int nUnusedEdx, unsigned int fDelete);
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
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_093)
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
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_132)
// FUNCTION: LEMBALL 0x00419f30
void __cdecl ClearPlasChildRuntimeFlag_0x30WithOptionalVirtualCleanup(void* pUnused, int* pObject)
{
	int(__fastcall * pfnCheck)(void*, int);
	void* pVtbl;
	(void) pUnused;
	pVtbl = *(void**) pObject;
	pfnCheck = (int(__fastcall*)(void*, int)) * (void**) ((char*) pVtbl + 0xe4);
	if (pfnCheck(pObject, 0) != 0) {
		((void(__fastcall*)(void*, int, int, int, int)) * (void**) ((char*) pVtbl + 0xe8))(pObject, 0, 0, 0, 1);
	}
	((void(__fastcall*)(void*, int)) * (void**) ((char*) pVtbl + 0xd8))(pObject, 0);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_165)
// FUNCTION: LEMBALL 0x00413380
void __fastcall refresh_level_mode_lift_boundary_columns(void* pObject)
{
	((void(__fastcall*)(void*)) 0x40282e)(*(void**) ((char*) pObject + 0x18c));
}
#endif
