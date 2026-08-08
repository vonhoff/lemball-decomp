// FUNCTION: LEMBALL 0x00414e80
int __fastcall CAICursor_ProcessMsg(void* pObject, int nUnused, short* param_1)
{
	if (*param_1 != 1) {
		*(int*) ((char*) pObject + 0xc) = *(int*) ((char*) pObject + 0xc) + 1;
		return 0;
	}
	((void(__fastcall*)(void*, int, int)) 0x40281f)(pObject, *(int*) ((char*) param_1 + 8), *(int*) ((char*) param_1 + 0xc));
	return 1;
}
