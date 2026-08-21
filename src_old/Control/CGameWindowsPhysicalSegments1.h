#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_017)
// FUNCTION: LEMBALL 0x0044bc50
void __fastcall ResizeRegistrationInfoScreenViewport(void* pObject, int nUnused, short* param_1)
{
	*(short*) ((char*) pObject + 0x18) = param_1[0];
	*(short*) ((char*) pObject + 0x1a) = param_1[1];
	if (*(void**) ((char*) pObject + 0x8c) != 0) {
		struct Pt {
			short x;
			short y;
		} pt;
		pt.x = (short) ((param_1[0] - 0x60) / 2);
		pt.y = (short) (param_1[1] - 0x20);
		(*(void (**)(void*))(*(void***) *(void**) ((char*) pObject + 0x8c) + 0x38 / 4))(&pt);
	}
}
#endif
