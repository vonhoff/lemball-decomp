#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_053)
// FUNCTION: LEMBALL 0x00463ac0
void __cdecl SelectionSortRecordsWithComparator(char* param_1, char* param_2, int param_3, void* pComparator)
{
	char* end = param_2;
	while (param_1 < end) {
		char* pMax = param_1;
		char* pCur = param_1 + param_3;
		while (pCur <= end) {
			if (((int(__cdecl*)(void*, void*)) pComparator)(pCur, pMax) > 0) {
				pMax = pCur;
			}
			pCur += param_3;
		}
		((void(__cdecl*)(void*, void*, int)) 0x463b20)(pMax, end, param_3);
		end -= param_3;
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_073)
// Minimal view of the LEVELVT runtime-state resetter so Restart can emit
// a direct `call LevelChunkObjectRuntimeStateView::ResetRuntimeStateThunk` (orig ILT 0x40209f).
struct LevelChunkObjectRuntimeStateView {
	void ResetRuntimeStateThunk(void);
}; // FUNCTION: LEMBALL 0x00458d40
int __fastcall CVSOStream___ls(void* pThis, int nUnused, unsigned int value)
{
	int i;
	unsigned int uShift;
	void* pInner;
	void* pTarget;

	(void) nUnused;
	uShift = 0x18;
	pInner = *(void**) (*(char**) pThis + 4);
	for (i = 0; i < 3; i++) {
		pTarget = *(void**) ((char*) (int) pInner + (int) pThis + 0x1c);
		((void(__fastcall*)(void*, int, unsigned int))(*(void***) pTarget + 2))(pTarget, 0, value >> uShift);
		uShift -= 8;
	}
	return (int) pThis;
}
#endif
