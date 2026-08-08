#include "Visos/Generic/Memory.h"
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
