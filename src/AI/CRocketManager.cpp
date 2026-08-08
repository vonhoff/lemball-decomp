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
