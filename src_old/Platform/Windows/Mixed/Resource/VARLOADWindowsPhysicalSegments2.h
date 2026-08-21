#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_029)
// FUNCTION: LEMBALL 0x0045e1e0
void __fastcall ParseGamiResourceDescriptor(void* pObject)
{
	unsigned int* puVar2 = *(unsigned int**) ((char*) pObject + 0x34);
	unsigned short uVar1 = *(unsigned short*) (puVar2 + 1);
	*(short*) ((char*) pObject + 0x48) = (short) *puVar2;
	*(unsigned short*) ((char*) pObject + 0x4a) = uVar1;
	*(char*) ((char*) pObject + 0x4c) = *(char*) (puVar2 + 2);
	*(char*) ((char*) pObject + 0x4d) = *(char*) ((char*) puVar2 + 9);
	*(unsigned int*) ((char*) pObject + 0x50) = puVar2[3];
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_096)
// FUNCTION: LEMBALL 0x0045ded0
void* __fastcall ConstructTwoArrayListResource(void* pObject, int nUnused)
{
	unsigned int dwTable = *(unsigned int*) 0x4a1d6c;
	*(void**) pObject = (void*) 0x498980;
	*(unsigned int*) ((char*) pObject + 0x48) = dwTable;
	*(void**) pObject = (void*) 0x4989c0;
	*(int*) ((char*) pObject + 0x18) = 0;
	*(int*) ((char*) pObject + 0x5c) = 0;
	*(int*) ((char*) pObject + 0x58) = 0;
	*(int*) ((char*) pObject + 0x54) = 0;
	*(int*) ((char*) pObject + 0x60) = 0;
	*(void**) pObject = (void*) 0x498c88;
	*(int*) ((char*) pObject + 0x78) = 0;
	*(int*) ((char*) pObject + 0x7c) = 0;
	ResetTypedResourceObjectState(pObject);
	*(int*) ((char*) pObject + 0x18) = 0;
	return pObject;
}
#endif
