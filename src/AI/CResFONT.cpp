#include "Visos/Generic/Memory.h"
extern void __fastcall ReleaseTypedResourceObjectIfLoaded(void* pObject, void* pUnusedEdx, int fReleaseMode);

// FUNCTION: LEMBALL 0x0045da50
void __fastcall CResFONT_UnLoadResources(void* pObject, int nUnused, int param_2, int param_3)
{
	ReleaseTypedResourceObjectIfLoaded((void*) (*(int*) ((char*) pObject + 0x80) + param_2 * 0x4c), 0, param_3);
	ReleaseTypedResourceObjectIfLoaded((void*) (*(int*) ((char*) pObject + 0x7c) + param_2 * 0x54), 0, param_3);
}
// FUNCTION: LEMBALL 0x0045da90
void __fastcall CResFONT_UnLoadVramData(void* pObject, int nUnused, int param_2, int param_3)
{
	(*( void(**)(int)) (*(void***) (*(int*) ((char*) pObject + 0x7c) + param_2 * 0x54) + 0x24 / 4))(param_3);
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
