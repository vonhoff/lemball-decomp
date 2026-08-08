// FUNCTION: LEMBALL 0x0041ccc0
void __fastcall CCrate_TriggerContents(void* pObject)
{
	if (*(int*) ((char*) pObject + 0x14c) != 0xffff) {
		void* pvVar1 = *(void**) ((char*) pObject + 0x148);
		*(int*) ((char*) pObject + 0x9c) = *(int*) ((char*) pvVar1 + 0x9c);
		*(int*) ((char*) pObject + 0xa0) = *(int*) ((char*) pvVar1 + 0xa0);
		*(int*) ((char*) pObject + 0xa4) = *(int*) ((char*) pvVar1 + 0xa4);
		((void(__fastcall*)(void*, int, void*, int)) 0x402cac)(*(void**) 0x4a74c0, -1, pvVar1, 0);
		*(int*) ((char*) pObject + 0x14c) = 0xffff;
	}
}
