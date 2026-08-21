#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_002)
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
		case 0x27:
			return 0x27;
		case 0x29:
			return 0x29;
		case 0x2b:
			return 0x2b;
		case 0x2d:
			return 0x2d;
		}
	}
	return 0xffff;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_003)
// FUNCTION: LEMBALL 0x0040fc50
void __fastcall RemoveQueuedProjectileRequestCode(void* pObject, int nUnused, int param_1)
{
	int nRequestCount = *(int*) ((char*) pObject + 0x220);
	int iRequest = 0;
	if (nRequestCount > 0) {
		int* pRequestCode = (int*) ((char*) pObject + 0x1c0);
		while (pRequestCode[0] != param_1) {
			pRequestCode++;
			iRequest++;
			if (nRequestCount <= iRequest) {
				return;
			}
		}
		iRequest++;
		if (iRequest < nRequestCount) {
			do {
				*(int*) ((char*) pObject + iRequest * 4 + 0x1bc) =
					*(int*) ((char*) pObject + (iRequest + 1) * 4 + 0x1bc);
				iRequest++;
			} while (iRequest < *(int*) ((char*) pObject + 0x220));
		}
		*(int*) ((char*) pObject + 0x220) = *(int*) ((char*) pObject + 0x220) - 1;
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_004)
#include "Visos/Generic/Memory.h"

// FUNCTION: LEMBALL 0x00419930
void* __fastcall DestroyManagedEntityGroup(void* pThis, int nUnused, unsigned char param_1)
{
	((void(__fastcall*)(void*)) 0x402441)(pThis);
	if ((param_1 & 1) != 0) {
		FreeVSMemBlock((void*) ((char*) pThis - 0xb0));
	}
	return (void*) ((char*) pThis - 0xb0);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_005)
extern int __fastcall FindNetworkLobbyPeerSlotByPeer(void* pObject, int nUnused, int param_1);

// FUNCTION: LEMBALL 0x00452b90
void* __fastcall GetNetworkLobbyPeerEntryStreamByPeer(void* pObject, int nUnused, int param_1)
{
	int iPeerSlot = FindNetworkLobbyPeerSlotByPeer(pObject, nUnused, param_1);
	if (iPeerSlot == -1) {
		return 0;
	}
	return (void*) (*(int*) ((char*) pObject + 0x14) + iPeerSlot * 0x50);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_006)
extern void __fastcall ResetManagedEntityRuntimeStateThunk(void* pObject);

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
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_007)
// FUNCTION: LEMBALL 0x0041fe30
void __fastcall InitializeEnmyChunkObjectBehaviorFields(void* pObject,
														int nUnused,
														int param_1,
														int param_2,
														int param_3,
														int param_4,
														int param_5,
														int param_6)
{
	*(int*) ((char*) pObject + 300) = param_1;
	*(int*) ((char*) pObject + 0x130) = param_2;
	*(int*) ((char*) pObject + 0x138) = param_3;
	*(int*) ((char*) pObject + 0x13c) = param_4;
	*(int*) ((char*) pObject + 0x144) = param_5;
	*(int*) ((char*) pObject + 0x148) = param_6;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_010)
// FUNCTION: LEMBALL 0x0041fe70
void __fastcall GetManagedEntityGeometryFields(void* pObject,
											   int nUnused,
											   int* param_2,
											   int* param_3,
											   int* param_4,
											   int* param_5,
											   int* param_6,
											   int* param_7)
{
	*param_2 = *(int*) ((char*) pObject + 300);
	*param_3 = *(int*) ((char*) pObject + 0x130);
	*param_4 = *(int*) ((char*) pObject + 0x138);
	*param_5 = *(int*) ((char*) pObject + 0x13c);
	*param_6 = *(int*) ((char*) pObject + 0x144);
	*param_7 = *(int*) ((char*) pObject + 0x148);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_012)
#include "Visos/Generic/Memory.h"

extern void __fastcall DestroyLevelChunkObjectBaseAutoThunk(void* pObject);

// FUNCTION: LEMBALL 0x0041c540
void* __fastcall DeleteLinkedType11ChunkObject(void* pObject, int nUnused, unsigned char param_1)
{
	*(void**) pObject = (void**) 0x4948d0;
	void* pLinked = *(void**) ((char*) pObject + 0x148);
	if (pLinked != 0 && *(int*) ((char*) pObject + 0x14c) != 0xffff && pLinked != 0) {
		(*(void (**)(int)) * (void***) pLinked)(1);
	}
	DestroyLevelChunkObjectBaseAutoThunk(pObject);
	if ((param_1 & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_014)
// FUNCTION: LEMBALL 0x00420000
int __fastcall EvaluateManagedEntityProbeConditionCode(void* pObject, int nUnused, int param_1)
{
	switch (param_1) {
	case 0:
		return 1;
	case 2:
		return ((int(__fastcall*)(void*)) 0x4025f9)(pObject);
	case 3:
		return ((int(__fastcall*)(void*)) 0x4025f9)(pObject) == 0;
	case 4:
		return ((int(__fastcall*)(void*)) 0x4031a2)(pObject);
	case 5:
		return ((int(__fastcall*)(void*)) 0x4031a2)(pObject) == 0;
	default:
		return 0;
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_020)
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
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_021)
// FUNCTION: LEMBALL 0x0040f2c0
void __fastcall SetDoorChunkObjectTargetTile(void* pObject, int nUnused, int param_1, int param_2)
{
	if (*(int*) ((char*) pObject + 0x184) == 0) {
		int nState = *(int*) ((char*) pObject + 0xb8);
		if (nState == 0 || nState == 2 || nState == 6) {
			*(int*) ((char*) pObject + 0x184) = 1;
			*(int*) ((char*) pObject + 0x1b4) = param_1 << 12;
			*(int*) ((char*) pObject + 0x1b8) = param_2 << 12;
		}
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_022)
extern void* g_pActiveManagedEntityOwner;
struct PlasChildStateEntityView;
extern void __cdecl DispatchPlasChildStateTableVariant0(void* pContext, PlasChildStateEntityView* pEntity);
extern void __cdecl DispatchPlasChildStateTableVariant1(void* pContext, PlasChildStateEntityView* pEntity);

// FUNCTION: LEMBALL 0x0040f120
int __fastcall DispatchDoorChunkObjectPlasStateTableVariant0or1(void* pObject)
{
	if (*(int*) ((char*) pObject + 0x114) == 0) {
		if (((int(__fastcall*)(void*, void*)) 0x402bb7)(g_pActiveManagedEntityOwner, pObject) != 0) {
			DispatchPlasChildStateTableVariant0(g_pActiveManagedEntityOwner, (PlasChildStateEntityView*) pObject);
			return 0;
		}
		DispatchPlasChildStateTableVariant1(g_pActiveManagedEntityOwner, (PlasChildStateEntityView*) pObject);
	}
	return 0;
}
#endif
