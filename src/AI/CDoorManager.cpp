// FUNCTION: LEMBALL 0x0040e080
int __fastcall CDoorManager_GetViewData(void* pThis, int nUnused, int param_1)
{
	int iVar2 = 0;
	int iVar3 = 0;
	if (*(int*) ((char*) pThis + 0x34) > 0) {
		do {
			int* piVar1 = (int*) (*(int*) ((char*) pThis + 0x3c) + iVar2);
			iVar2 = iVar2 + 0x14c;
			iVar3 = iVar3 + 1;
			(*( void(**)(int)) (*(void***) *piVar1 + 0xc / 4))(param_1);
			param_1 = param_1 + 0x4c;
		} while (iVar3 < *(int*) ((char*) pThis + 0x34));
	}
	return *(int*) ((char*) pThis + 0x34);
}
// FUNCTION: LEMBALL 0x0040e550
void __fastcall CDoorManager_Process(void* param_1)
{
	int iVar3 = 0;
	if (*(int*) ((char*) param_1 + 0x34) > 0) {
		int iVar2 = 0;
		do {
			*(int*) (*(int*) ((char*) param_1 + 0x3c) + 0x124 + iVar2) = 1;
			void* piVar1 = (void*) (*(int*) ((char*) param_1 + 0x3c) + iVar2);
			if (*(int*) ((char*) piVar1 + 0x51 * 4) != 0 || *(int*) ((char*) piVar1 + 0x45 * 4) != 0) {
				(*( void(**)(void)) (*(void***) piVar1 + 0x14 / 4))();
			}
			iVar2 = iVar2 + 0x14c;
			iVar3 = iVar3 + 1;
		} while (iVar3 < *(int*) ((char*) param_1 + 0x34));
	}
}
