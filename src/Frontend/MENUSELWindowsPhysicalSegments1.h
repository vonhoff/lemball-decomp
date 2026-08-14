#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_019)
// FUNCTION: LEMBALL 0x004676a0
int __fastcall ResolveVariantRenderFramePointer(void* pObject, int nUnused, int param_2, void** param_3)
{
	int nVariantAddress =
		*(int*) (*(int*) ((char*) pObject + 0x24) + *(short*) (*(int*) ((char*) pObject + 0x28) + param_2 * 2) * 4);
	int nFrameIndex;
	if (param_3 == 0) {
		nFrameIndex = 0;
	}
	else {
		nFrameIndex = (*(int (**)(void)) *param_3)();
	}
	if (*(int*) (nVariantAddress + 0x40) == 0x5a524c45) {
		return nVariantAddress;
	}
	return *(int*) (nVariantAddress + 0x78) + nFrameIndex * 0x54;
}
#endif
