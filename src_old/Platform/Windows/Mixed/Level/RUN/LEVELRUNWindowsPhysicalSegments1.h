#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_001)
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
			*(unsigned short*) ((char*) pObject + 0x14 + i * 0x18) =
				*(unsigned short*) ((char*) pObject + 0x14 + i * 0x18 + 2);
		}
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_008)
// FUNCTION: LEMBALL 0x0041e9a0
void __fastcall DestroyCachedChunkObjectChildren(void* pObject)
{
	int i;
	if (*(int*) ((char*) pObject + 0xa4) > 0) {
		for (i = 0; i < *(int*) ((char*) pObject + 0xa4); i++) {
			void* pChild = *(void**) ((char*) pObject + 4 + i * 4);
			if (pChild != 0) {
				(*(void (**)(int)) * (void***) pChild)(1);
			}
			*(void**) ((char*) pObject + 4 + i * 4) = 0;
		}
	}
	*(int*) ((char*) pObject + 0xa4) = 0;
	*(int*) ((char*) pObject + 0xa8) = 0;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_013)
// FUNCTION: LEMBALL 0x00421fc0
int __fastcall AdvanceActiveBallChunkEntries(void* pObject)
{
	int i;
	if (*(int*) ((char*) pObject + 8) > 0) {
		for (i = 0; i < *(int*) ((char*) pObject + 8); i++) {
			(*(void (**)(void))(*(void***) *(void**) (*(int*) ((char*) pObject + 4) + i * 4) + 0x14 / 4))();
		}
	}
	return 1;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_025)
#include "Visos/Generic/Memory.h"

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
		}
		else {
			*(int*) ((char*) pObject + 4) = 0;
		}
	}
	for (i = 0; *(int*) ((char*) pObject + 8) > i; i++) {
		(*(void (**)(void))(*(void***) (*(int*) ((char*) pObject + 4) + i * 0x150) + 0x104 / 4))();
	}
}
#endif
