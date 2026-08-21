#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_099)
// FUNCTION: LEMBALL 0x00462cb0
void* __fastcall DeleteEffChannelStreamStackWrapper(void* pThis, int nUnused, char param_1)
{
	((void(__fastcall*)(void*)) 0x45f8a0)((char*) pThis + 0xa8);
	((void(__fastcall*)(void*)) 0x45fd80)((char*) pThis + 0x30);
	((void(__fastcall*)(void*)) 0x45f6c0)(pThis);
	if ((param_1 & 1) != 0) {
		FreeVSMemBlock((char*) pThis - 8);
	}
	return (char*) pThis - 8;
}
#endif
