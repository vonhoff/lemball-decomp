// Cluster reconstruction: clean scan/array operations appended to a fresh TU
// (avoids disturbing existing translation units' formatting).
#include "Visos/Generic/Memory.h"

extern int __fastcall FindNetworkLobbyPeerSlotByPeer(void* pObject, int nUnused, int param_1);
extern void __fastcall ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void __fastcall DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void* g_pActiveManagedEntityOwner;
struct PlasChildStateEntityView;
extern void __cdecl DispatchPlasChildStateTableVariant0(void* pContext, PlasChildStateEntityView* pEntity);
extern void __cdecl DispatchPlasChildStateTableVariant1(void* pContext, PlasChildStateEntityView* pEntity);
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
// FUNCTION: LEMBALL 0x00419930
void* __fastcall DestroyManagedEntityGroup(void* pThis, int nUnused, unsigned char param_1)
{
	((void(__fastcall*)(void*)) 0x402441)(pThis);
	if ((param_1 & 1) != 0) {
		FreeVSMemBlock((void*) ((char*) pThis - 0xb0));
	}
	return (void*) ((char*) pThis - 0xb0);
}
// FUNCTION: LEMBALL 0x00452b90
void* __fastcall GetNetworkLobbyPeerEntryStreamByPeer(void* pObject, int nUnused, int param_1)
{
	int iVar1 = FindNetworkLobbyPeerSlotByPeer(pObject, nUnused, param_1);
	if (iVar1 == -1) {
		return 0;
	}
	return (void*) (*(int*) ((char*) pObject + 0x14) + iVar1 * 0x50);
}
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
// FUNCTION: LEMBALL 0x0041b8a0
void __fastcall DispatchCountedChildSlot0x18ById(void* pObject, int nUnused, unsigned int param_1, int param_2)
{
	int i;
	if (*(unsigned short*) ((char*) pObject + 0x36) != 0) {
		for (i = 0; i < (int) *(unsigned short*) ((char*) pObject + 0x36); i++) {
			void* pChild = *(void**) (*(int*) ((char*) pObject + 0x3c) + i * 4);
			if (*(unsigned short*) ((char*) pChild + 0x6a) == param_1) {
				(*( void(**)(int)) (*(void***) pChild + 0x18 / 4))(param_2);
			}
		}
	}
}
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
// FUNCTION: LEMBALL 0x00421fc0
int __fastcall AdvanceActiveBallChunkEntries(void* pObject)
{
	int i;
	if (*(int*) ((char*) pObject + 8) > 0) {
		for (i = 0; i < *(int*) ((char*) pObject + 8); i++) {
			(*( void(**)(void)) (*(void***) *(void**) (*(int*) ((char*) pObject + 4) + i * 4) + 0x14 / 4))();
		}
	}
	return 1;
}
// FUNCTION: LEMBALL 0x00420000
int __fastcall EvaluateManagedEntityProbeConditionCode(void* pObject, int nUnused, int param_1)
{
	switch (param_1) {
	case 0: return 1;
	case 2: return ((int(__fastcall*)(void*)) 0x4025f9)(pObject);
	case 3: return ((int(__fastcall*)(void*)) 0x4025f9)(pObject) == 0;
	case 4: return ((int(__fastcall*)(void*)) 0x4031a2)(pObject);
	case 5: return ((int(__fastcall*)(void*)) 0x4031a2)(pObject) == 0;
	default: return 0;
	}
}
// FUNCTION: LEMBALL 0x0041b160
void __fastcall DestroyManagedEntityGroupChildren(void* pObject)
{
	int i;
	if (*(unsigned short*) ((char*) pObject + 0x36) != 0) {
		for (i = 0; i < (int) *(unsigned short*) ((char*) pObject + 0x36); i++) {
			void* pChild = *(void**) (*(int*) ((char*) pObject + 0x3c) + i * 4);
			((void(__fastcall*)(void*, short)) 0x402293)(pChild, 0xffff);
			if (pChild != 0) {
				(*( void(**)(int)) *(void***) pChild)(1);
			}
		}
	}
	*(unsigned short*) ((char*) pObject + 0x36) = 0;
	*(unsigned short*) ((char*) pObject + 0x38) = 0;
}
// FUNCTION: LEMBALL 0x00473a10
void __fastcall AppendDebugTextLinesSplitNewlines(void* pObject, int nUnused, char* param_1, int param_2)
{
	char* pcVar2 = param_1;
	while (*param_1 != '\0') {
		if (*param_1 == '\n') {
			*param_1 = '\0';
			((void(__fastcall*)(void*, char*, int)) 0x4738e0)(pObject, pcVar2, param_2);
			pcVar2 = param_1 + 1;
		}
		param_1++;
	}
	if (pcVar2 < param_1) {
		((void(__fastcall*)(void*, char*, int)) 0x4738e0)(pObject, pcVar2, param_2);
	}
}
// FUNCTION: LEMBALL 0x0044bc50
void __fastcall ResizeRegistrationInfoScreenViewport(void* pObject, int nUnused, short* param_1)
{
	*(short*) ((char*) pObject + 0x18) = param_1[0];
	*(short*) ((char*) pObject + 0x1a) = param_1[1];
	if (*(void**) ((char*) pObject + 0x8c) != 0) {
		struct Pt { short x; short y; } pt;
		pt.x = (short) ((param_1[0] - 0x60) / 2);
		pt.y = (short) (param_1[1] - 0x20);
		(*( void(**)(void*)) (*(void***) *(void**) ((char*) pObject + 0x8c) + 0x38 / 4))(&pt);
	}
}
// FUNCTION: LEMBALL 0x0044aa20
unsigned int __fastcall GetTimedFrameSequenceIndex(void* pObject)
{
	if (*(int*) ((char*) pObject + 0x1c) != 0) {
		return *(unsigned int*) ((char*) pObject + 8);
	}
	unsigned int uVar2 = *(unsigned int*) ((char*) pObject + 0x10);
	unsigned int uVar1 = *(unsigned int*) ((char*) pObject + 0x14) - *(unsigned int*) ((char*) pObject + 0xc);
	if (uVar2 <= uVar1) {
		uVar2 = *(unsigned int*) ((char*) pObject + 4) - 1;
		*(int*) ((char*) pObject + 0x1c) = 1;
		*(unsigned int*) ((char*) pObject + 8) = uVar2;
		return uVar2;
	}
	uVar2 = ((uVar1 % uVar2) * *(unsigned int*) ((char*) pObject + 4)) / uVar2;
	if (*(int*) ((char*) pObject + 0x18) != 1) {
		uVar2 = (*(unsigned int*) ((char*) pObject + 4) - uVar2) - 1;
	}
	return uVar2;
}
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
// FUNCTION: LEMBALL 0x0040f2c0
void __fastcall SetDoorChunkObjectTargetTile(void* pObject, int nUnused, int param_1, int param_2)
{
	if (*(int*) ((char*) pObject + 0x184) == 0) {
		int iVar1 = *(int*) ((char*) pObject + 0xb8);
		if (iVar1 == 0 || iVar1 == 2 || iVar1 == 6) {
			*(int*) ((char*) pObject + 0x184) = 1;
			*(int*) ((char*) pObject + 0x1b4) = param_1 << 12;
			*(int*) ((char*) pObject + 0x1b8) = param_2 << 12;
		}
	}
}
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
// FUNCTION: LEMBALL 0x00422460
void __fastcall CollectableManagerInitialise(void* pObject, int nUnused, int param_1)
{
	int i;
	int* pArr;
	*(int*) ((char*) pObject + 0x38) = param_1;
	if (param_1 == 0) {
		*(int*) ((char*) pObject + 0x34) = 0;
		return;
	}
	if (*(int*) ((char*) pObject + 0x34) == 0) {
		pArr = (int*) AllocateVSMemBlock(param_1 * 4);
		*(int**) ((char*) pObject + 0x34) = pArr;
		if (*(int*) ((char*) pObject + 0x38) > 0) {
			for (i = 0; i < *(int*) ((char*) pObject + 0x38); i++) {
				pArr[i] = 0;
			}
		}
	}
}
// FUNCTION: LEMBALL 0x00454ad0
void __fastcall RegisterUiPaletteRemapVariantTable(void* pObject)
{
	int i;
	for (i = 0; i < 6; i++) {
		*(int*) ((char*) pObject + 0x414 + i * 4) = ((int(__cdecl*)(int, unsigned char*, int)) 0x46ad70)(
			*(int*) (*(int*) ((char*) pObject + 0x84) + 0x54),
			*(unsigned char**) (0x4a02f0 + i * 4), 0x2);
	}
}
// FUNCTION: LEMBALL 0x0040b930
void __fastcall SlinkyManagerInitialise(void* pObject, int nUnused, int param_1)
{
	int i;
	char* pBase;
	*(int*) ((char*) pObject + 8) = param_1;
	*(int*) ((char*) pObject + 0xc) = 0;
	if (param_1 == 0) {
		*(int*) ((char*) pObject + 4) = 0;
		return;
	}
	if (*(int*) ((char*) pObject + 4) == 0) {
		pBase = (char*) AllocateVSMemBlock(param_1 * 0x150 + 4);
		if (pBase != 0) {
			*(int*) pBase = param_1;
			*(int**) ((char*) pObject + 4) = (int*) (pBase + 4);
			for (i = param_1 - 1; i >= 0; i--) {
				((void(__fastcall*)(void*)) 0x401285)(pBase + 4 + i * 0x150);
			}
		} else {
			*(int*) ((char*) pObject + 4) = 0;
		}
	}
	for (i = 0; *(int*) ((char*) pObject + 8) > i; i++) {
		(*( void(**)(void)) (* (void***) (*(int*) ((char*) pObject + 4) + i * 0x150) + 0x104 / 4))();
	}
}
