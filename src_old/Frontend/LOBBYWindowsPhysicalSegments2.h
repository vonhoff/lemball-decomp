#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_020)
// FUNCTION: LEMBALL 0x00455da0
void* __fastcall ConstructNetworkLobbyPlayerEntryChild(void* pObject)
{
	((void(__fastcall*)(void*)) 0x46a300)(pObject);
	*(void**) pObject = (void**) 0x4986b0;
	((void(__fastcall*)(void*)) 0x4014dd)(pObject);
	return pObject;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_026)
// FUNCTION: LEMBALL 0x00454b40
void __fastcall MarkNetworkLobbyPlayerEntryDirty(void* pObject, int nUnused, int param_1)
{
	if (param_1 != -1 && param_1 < 10) {
		*(int*) (*(int*) ((char*) pObject + 0x3cc) + 0x40 + param_1 * 0x44) = 1;
	}
}
#endif
