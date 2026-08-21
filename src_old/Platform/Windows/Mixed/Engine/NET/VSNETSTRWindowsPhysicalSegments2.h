#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_028)
// FUNCTION: LEMBALL 0x00462990
void* __fastcall DeleteTimedEffStreamWithChannelStateWrapper(void* pThis, int nUnused, char param_1)
{
	((void(__fastcall*)(void*)) 0x45fd80)(pThis);
	((void(__fastcall*)(void*)) 0x45f6c0)((char*) pThis + 0x78);
	if ((param_1 & 1) != 0) {
		FreeVSMemBlock(pThis);
	}
	return pThis;
}
#endif
