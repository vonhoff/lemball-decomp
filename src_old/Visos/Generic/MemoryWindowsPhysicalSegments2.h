#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_045)
// FUNCTION: LEMBALL 0x00459b90
int __fastcall CoalesceFreeMemoryBlockWithTrailingNeighbor(void* pThis, int nUnused, void* param_2)
{
	void* piVar1 = *(void**) ((char*) param_2 + 0x14);
	if (((int(__fastcall*)(void*, void*)) 0x45a0e0)(pThis, piVar1) != 0) {
		if (((int(__fastcall*)(void*, void*)) 0x45a230)(pThis, piVar1) != 0) {
			((void(__fastcall*)(void*, void*, void*)) 0x459b10)(pThis, param_2, piVar1);
		}
	}
	return 0;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_071)
// FUNCTION: LEMBALL 0x0045a180
void* __fastcall ArenaShrink(void* pThis, int nUnused, int param_1)
{
	void* pNode = *(void**) ((char*) pThis + 0x34);
	while (pNode != 0) {
		if (((int(__fastcall*)(void*, void*)) 0x45a200)(pThis, pNode) != 0) {
			if (((int(__fastcall*)(void*, int)) 0x45a580)(pNode, param_1) != 0) {
				return pNode;
			}
		}
		pNode = *(void**) ((char*) pNode + 0x14);
	}
	return 0;
}
#endif
