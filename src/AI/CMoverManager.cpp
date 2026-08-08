// FUNCTION: LEMBALL 0x0042f5e0
void __fastcall CMoverManager_Add(void* pObject, int nUnused, unsigned short param_1, unsigned short param_2, void* param_3, int param_4, void* param_5)
{
	if (*(int*) ((char*) pObject + 0x34) < *(int*) ((char*) pObject + 0x30)) {
		((void(__fastcall*)(void*, unsigned short, unsigned short, void*, int, void*)) 0x40128a)((void*) (*(int*) ((char*) pObject + 0x38) + *(int*) ((char*) pObject + 0x34) * 0x1a0), param_1, param_2, param_3, param_4, param_5);
		*(int*) ((char*) pObject + 0x34) = *(int*) ((char*) pObject + 0x34) + 1;
	}
}
// FUNCTION: LEMBALL 0x0042f540
int __fastcall CMoverManager_GetViewData(void* pThis, int nUnused, int param_1)
{
	int iVar2 = 0;
	int iVar3 = 0;
	int local_4 = 0;
	if (*(int*) ((char*) pThis + 0x34) > 0) {
		do {
			int* piVar1 = (int*) (*(int*) ((char*) pThis + 0x38) + iVar3);
			iVar2 = iVar2 + 1;
			iVar3 = iVar3 + 0x1a0;
			(*( void(**)(int)) (*(void***) *piVar1 + 0xc / 4))(param_1);
			local_4 = local_4 + 1;
			param_1 = param_1 + 0x4c;
		} while (iVar2 < *(int*) ((char*) pThis + 0x34));
	}
	return local_4;
}
// FUNCTION: LEMBALL 0x0042f680
void __fastcall CMoverManager_LoadLevel(void* pThis, int nUnused, unsigned short* pStream, int param_4, int param_5)
{
	unsigned short nCount;
	unsigned int uSlot;
	unsigned int uFlags;
	unsigned int flagOverride;
	unsigned int routeSel;
	unsigned int routeCount;

	nCount = *pStream;
	pStream++;
	((void(__fastcall*) (void*, int)) 0x401811)(pThis, nCount);
	*(int*) ((char*) pThis + 0x34) = 0;
	if (nCount != 0) {
		do {
			if (*(unsigned short*) (*(int*) ((char*) pThis + 0x3c) + 0x54) < 2) {
				uSlot = (unsigned int) ((int(__fastcall*) ()) 0x40214e)();
			}
			else {
				uSlot = *pStream;
				pStream++;
			}
			uFlags = 0;
			flagOverride = 0;
			if (*(unsigned short*) (*(int*) ((char*) pThis + 0x3c) + 0x54) > 5) {
				uFlags = *pStream;
				pStream++;
				if ((uFlags & 0x8000) != 0) {
					flagOverride = 1;
					uFlags &= 0x7fff;
				}
			}
			routeSel = *pStream;
			pStream++;
			routeCount = *pStream;
			pStream++;
			((void(__fastcall*) (void*, int, int, int, int, int)) 0x401f55)(pThis, uSlot, uFlags, flagOverride, routeSel, routeCount);
			nCount--;
		} while (nCount != 0);
	}
}
