#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_009)
// FUNCTION: LEMBALL 0x0041b940
void __fastcall DeactivateManagedEntityGroupChildBySlot(void* pObject, int nUnused, short param_2)
{
	int i;
	if (*(unsigned short*) ((char*) pObject + 0x36) != 0) {
		for (i = 0;; i++) {
			int nChildAddress = *(int*) (*(int*) ((char*) pObject + 0x3c) + i * 4);
			if (nChildAddress != 0 && ((char(__fastcall*)(void*)) 0x401794)((void*) nChildAddress) == (char) param_2) {
				((void(__fastcall*)(void*, int)) 0x4024ff)(pObject, i);
				break;
			}
			if ((int) *(unsigned short*) ((char*) pObject + 0x36) <= i + 1) {
				return;
			}
		}
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_011)
// FUNCTION: LEMBALL 0x0041b8a0
void __fastcall DispatchCountedChildSlot0x18ById(void* pObject, int nUnused, unsigned int param_1, int param_2)
{
	int i;
	if (*(unsigned short*) ((char*) pObject + 0x36) != 0) {
		for (i = 0; i < (int) *(unsigned short*) ((char*) pObject + 0x36); i++) {
			void* pChild = *(void**) (*(int*) ((char*) pObject + 0x3c) + i * 4);
			if (*(unsigned short*) ((char*) pChild + 0x6a) == param_1) {
				(*(void (**)(int))(*(void***) pChild + 0x18 / 4))(param_2);
			}
		}
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_015)
// FUNCTION: LEMBALL 0x0041b160
void __fastcall DestroyManagedEntityGroupChildren(void* pObject)
{
	int i;
	if (*(unsigned short*) ((char*) pObject + 0x36) != 0) {
		for (i = 0; i < (int) *(unsigned short*) ((char*) pObject + 0x36); i++) {
			void* pChild = *(void**) (*(int*) ((char*) pObject + 0x3c) + i * 4);
			((void(__fastcall*)(void*, short)) 0x402293)(pChild, 0xffff);
			if (pChild != 0) {
				(*(void (**)(int)) * (void***) pChild)(1);
			}
		}
	}
	*(unsigned short*) ((char*) pObject + 0x36) = 0;
	*(unsigned short*) ((char*) pObject + 0x38) = 0;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_023)
#include "Visos/Generic/Memory.h"

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
#endif
