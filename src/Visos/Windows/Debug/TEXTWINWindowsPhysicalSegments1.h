#if defined(LEMBALL_WINDOWS_PHYSICAL_1_SEGMENT_016)
// FUNCTION: LEMBALL 0x00473a10
void __fastcall AppendDebugTextLinesSplitNewlines(void* pObject, int nUnused, char* param_1, int param_2)
{
	char* pLineStart = param_1;
	while (*param_1 != '\0') {
		if (*param_1 == '\n') {
			*param_1 = '\0';
			((void(__fastcall*)(void*, char*, int)) 0x4738e0)(pObject, pLineStart, param_2);
			pLineStart = param_1 + 1;
		}
		param_1++;
	}
	if (pLineStart < param_1) {
		((void(__fastcall*)(void*, char*, int)) 0x4738e0)(pObject, pLineStart, param_2);
	}
}
#endif
