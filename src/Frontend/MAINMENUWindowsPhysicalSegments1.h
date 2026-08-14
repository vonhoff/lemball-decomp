#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_024)
// FUNCTION: LEMBALL 0x00454ad0
void __fastcall RegisterUiPaletteRemapVariantTable(void* pObject)
{
	int i;
	for (i = 0; i < 6; i++) {
		*(int*) ((char*) pObject + 0x414 + i * 4) =
			((int(__cdecl*)(int, unsigned char*, int)) 0x46ad70)(*(int*) (*(int*) ((char*) pObject + 0x84) + 0x54),
																 *(unsigned char**) (0x4a02f0 + i * 4),
																 0x2);
	}
}
#endif
