#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_041)
// FUNCTION: LEMBALL 0x004743e0
void __fastcall SetTextSelectionRangeState(void* pThis, int nUnused, int param_1)
{
	(*(void (**)(void*))(*(void***) pThis + 0 / 4))(pThis);
	int iVar1 = *(int*) ((char*) pThis + 0x50);
	if (iVar1 <= *(int*) ((char*) pThis + 0x54)) {
		int iVar3 = iVar1 * 0xc;
		int iVar2;
		do {
			iVar3 += 0xc;
			iVar2 = iVar1 + 1;
			*(int*) (*(int*) (*(int*) ((char*) pThis + 0x24) + 4) + iVar3 - 4) = param_1;
			((void(__fastcall*)(void*, int, int)) 0x473f80)(pThis, iVar1, 1);
			iVar1 = iVar2;
		} while (iVar2 <= *(int*) ((char*) pThis + 0x54));
	}
	(*(void (**)(void*))(*(void***) pThis + 4 / 4))(pThis);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_119)
// FUNCTION: LEMBALL 0x00473eb0
void __cdecl get_window_client_rect_in_screen_coords(HWND hwnd, DebugTextRect* pRect)
{
	tagPOINT pt;
	pt.x = 0;
	pt.y = 0;
	ClientToScreen(hwnd, &pt);
	GetClientRect(hwnd, pRect);
	pRect->nTop += pt.y;
	pRect->nBottom += pt.y;
	pRect->nLeft += pt.x;
	pRect->nRight += pt.x;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_120)
// FUNCTION: LEMBALL 0x00474000
int __fastcall refresh_debug_text_visible_line_counts(void* pThis, int nEdxSlop)
{
	DebugTextRect rc;
	void** vtbl = *(void***) pThis;
	(*(void(__fastcall**)(void*, int)) vtbl[0])(pThis, 0);
	GetClientRect(*(HWND*) ((char*) pThis + 0x1c), &rc);
	int lineHeight = *(int*) ((char*) pThis + 0x3c);
	*(int*) ((char*) pThis + 0x30) = (rc.nBottom - rc.nTop) / lineHeight;
	*(int*) ((char*) pThis + 0x34) = (lineHeight - 1 + (rc.nBottom - rc.nTop)) / lineHeight;
	(*(void(__fastcall**)(void*, int)) vtbl[1])(pThis, 0);
	return *(int*) ((char*) pThis + 0x34);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_133)
// FUNCTION: LEMBALL 0x00474050
int __fastcall refresh_debug_text_client_width(void* pThis, int nEdxSlop)
{
	DebugTextRect rc;
	void** vtbl = *(void***) pThis;
	(*(void(__fastcall**)(void*, int)) vtbl[0])(pThis, 0);
	GetClientRect(*(HWND*) ((char*) pThis + 0x1c), &rc);
	int nWidth = rc.nRight - rc.nLeft;
	*(int*) ((char*) pThis + 0x40) = nWidth;
	(*(void(__fastcall**)(void*, int)) vtbl[1])(pThis, 0);
	return *(int*) ((char*) pThis + 0x40);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_134)
// FUNCTION: LEMBALL 0x00473f80
void __fastcall invalidate_debug_text_line_range(void* pThis, int nEdxSlop, int nLine, int nCount)
{
	void** vtbl = *(void***) pThis;
	(*(void(__fastcall**)(void*, int)) vtbl[0])(pThis, 0);
	int nVisible = nLine - *(int*) ((char*) pThis + 0x38);
	if (nVisible >= 0 && nVisible < *(int*) ((char*) pThis + 0x34)) {
		DebugTextRect rc;
		int nLineHeight = *(int*) ((char*) pThis + 0x3c);
		rc.nLeft = 0;
		rc.nTop = nVisible * nLineHeight;
		rc.nRight = *(int*) ((char*) pThis + 0x40);
		rc.nBottom = nLineHeight * nCount + rc.nTop;
		if (InvalidateRect(*(HWND*) ((char*) pThis + 0x1c), &rc, 0) == 0) {
			MessageBoxA(0, (const char*) 0x4a2c74, (const char*) 0x4a2c6c, 0);
		}
	}
	(*(void(__fastcall**)(void*, int)) vtbl[1])(pThis, 0);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_135)
// FUNCTION: LEMBALL 0x004744a0
void __fastcall update_text_selection_drag(void* pThis, int nEdxSlop, int nUnused, int nScreenX, int nScreenY)
{
	void** vtbl = *(void***) pThis;
	(*(void(__fastcall**)(void*, int)) vtbl[0])(pThis, 0);
	if (*(int*) ((char*) pThis + 0x44) != 0) {
		int nIdx = ((int(__fastcall*)(void*, int, int, int)) 0x473f00)(pThis, 0, nScreenX, nScreenY);
		*(int*) ((char*) pThis + 0x48) = nIdx;
		if (*(int*) ((char*) pThis + 0x2c) <= *(int*) ((char*) pThis + 0x48)) {
			*(int*) ((char*) pThis + 0x48) = *(int*) ((char*) pThis + 0x2c) - 1;
		}
		if (*(int*) ((char*) pThis + 0x48) < 0) {
			(*(void(__fastcall**)(void*, int)) vtbl[1])(pThis, 0);
			return;
		}
		((void(__fastcall*)(void*, int, int)) 0x4743e0)(pThis, 0, 0);
		int nCur = *(int*) ((char*) pThis + 0x48);
		if (nCur <= *(int*) ((char*) pThis + 0x4c)) {
			*(int*) ((char*) pThis + 0x50) = nCur;
		}
		if (*(int*) ((char*) pThis + 0x4c) <= nCur) {
			*(int*) ((char*) pThis + 0x54) = nCur;
		}
		((void(__fastcall*)(void*, int, int)) 0x4743e0)(pThis, 0, 1);
		((void(__fastcall*)(void*, int, int)) 0x474430)(pThis, 0, *(int*) ((char*) pThis + 0x48));
	}
	(*(void(__fastcall**)(void*, int)) vtbl[1])(pThis, 0);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_138)
// FUNCTION: LEMBALL 0x00474290
void __fastcall scroll_debug_text_window(void* pThis, int nEdxSlop, unsigned int nCommand, int param_2)
{
	void** ppVtbl = *(void***) pThis;
	((void(__fastcall*)(void*)) ppVtbl[0])(pThis);
	switch (nCommand) {
	case 0:
		*(int*) ((char*) pThis + 0x38) = *(int*) ((char*) pThis + 0x38) - 1;
		break;
	case 1:
		*(int*) ((char*) pThis + 0x38) = *(int*) ((char*) pThis + 0x38) + 1;
		break;
	case 2:
		*(int*) ((char*) pThis + 0x38) = *(int*) ((char*) pThis + 0x38) - *(int*) ((char*) pThis + 0x34);
		break;
	case 3:
		*(int*) ((char*) pThis + 0x38) = *(int*) ((char*) pThis + 0x38) + *(int*) ((char*) pThis + 0x34);
		break;
	case 4:
	case 5:
		goto lblWriteTopLine;
	case 6:
		*(int*) ((char*) pThis + 0x38) = 0;
		break;
	case 7:
		param_2 = *(int*) ((char*) pThis + 0x2c);
	lblWriteTopLine:
		*(int*) ((char*) pThis + 0x38) = param_2;
		break;
	}
	if (*(int*) ((char*) pThis + 0x2c) <= *(int*) ((char*) pThis + 0x38)) {
		*(int*) ((char*) pThis + 0x38) = *(int*) ((char*) pThis + 0x2c) - 1;
	}
	if (*(int*) ((char*) pThis + 0x38) < 0) {
		*(int*) ((char*) pThis + 0x38) = 0;
	}
	((int(__cdecl*)(HWND, int, int, int)) *
	 (void**) 0x4ac8f0)(*(HWND*) ((char*) pThis + 0x1c), 1, *(int*) ((char*) pThis + 0x38), 1);
	((void(__fastcall*)(void*)) 0x473f60)(pThis);
	((void(__fastcall*)(void*)) ppVtbl[1])(pThis);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_139)
// FUNCTION: LEMBALL 0x00473cf0
void __fastcall destroy_debug_text_window(void* pThis)
{
	unsigned int uVar1;
	int iVar2;
	int iVar3;
	int iVar4;
	*(void**) pThis = (void*) 0x49a4a0;
	if (*(int*) ((char*) pThis + 0x44) != 0) {
		((void (*)(void)) * (void**) 0x4ac88c)();
		*(int*) ((char*) pThis + 0x44) = 0;
	}
	uVar1 = *(unsigned int*) ((char*) pThis + 0x24);
	if (uVar1 != 0) {
		iVar2 = *(int*) (uVar1 + 4);
		if (iVar2 != 0) {
			iVar3 = *(int*) (iVar2 - 4);
			iVar4 = iVar2 + iVar3 * 0xc;
			while (iVar3 = iVar3 - 1, iVar3 > -1) {
				iVar4 = iVar4 - 0xc;
				((void(__fastcall*)(void*)) 0x4564c0)((void*) iVar4);
			}
			((void(__cdecl*)(void*)) 0x45a790)((void*) (iVar2 - 4));
		}
		((void(__cdecl*)(void*)) 0x45a790)((void*) uVar1);
	}
	*(int*) 0x4a2b7c = 0;
	*(int*) ((char*) pThis + 0x18) = 0;
	((void(__cdecl*)(void*)) * (void**) 0x4ac624)(*(void**) ((char*) pThis + 0x1c));
	*(void**) pThis = (void*) 0x4988d0;
	((void(__cdecl*)(void*)) * (void**) 0x4ac7b8)((char*) pThis + 4);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_140)
// FUNCTION: LEMBALL 0x004743b0
void __fastcall release_debug_text_mouse_capture(void* pThis, int nEdxSlop, int param_1, int param_2, int param_3)
{
	void** ppVtbl = *(void***) pThis;
	((void(__fastcall*)(void*)) ppVtbl[0])(pThis);
	if (*(int*) ((char*) pThis + 0x44) != 0) {
		((void (*)(void)) * (void**) 0x4ac88c)();
		*(int*) ((char*) pThis + 0x44) = 0;
	}
	((void(__fastcall*)(void*)) ppVtbl[1])(pThis);
	(void) param_1;
	(void) param_2;
	(void) param_3;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_150)
// FUNCTION: LEMBALL 0x00474090
void __fastcall resize_debug_text_window_to_line_height(void* pThis, int nEdxSlop, int xPos, int nRequestH)
{
	void** vtbl = *(void***) pThis;
	(*(void(__fastcall**)(void*, int)) vtbl[0])(pThis, 0);
	HWND hwnd = *(HWND*) ((char*) pThis + 0x1c);
	int nLineH = *(int*) ((char*) pThis + 0x3c);
	if (nRequestH % nLineH == 0) {
		refresh_debug_text_visible_line_counts(pThis, 0);
		refresh_debug_text_client_width(pThis, 0);
	}
	else {
		DebugTextRect rc;
		rc.nLeft = 0;
		rc.nTop = 0;
		rc.nRight = xPos + GetSystemMetrics(2);
		rc.nBottom = nRequestH - nRequestH % nLineH + nLineH;
		AdjustWindowRect(&rc, GetWindowLongA(hwnd, -16), 0);
		SetWindowPos(hwnd, 0, 0, 0, rc.nRight - rc.nLeft, rc.nBottom - rc.nTop, 6);
	}
	(*(void(__fastcall**)(void*, int)) vtbl[1])(pThis, 0);
	(void) nEdxSlop;
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_2_SEGMENT_170)
// FUNCTION: LEMBALL 0x00473f00
int __fastcall map_screen_y_to_debug_text_line_index(void* pThis, int nEdxSlop, int screenY)
{
	DebugTextRect rc;
	void** vtbl = *(void***) pThis;
	int nLine;
	int nTopLine;
	(*(void(__fastcall**)(void*, int)) vtbl[0])(pThis, 0);
	get_window_client_rect_in_screen_coords(*(HWND*) ((char*) pThis + 0x1c), &rc);
	nLine = screenY - rc.nTop;
	if (nLine < 0) {
		nLine = -1;
	}
	else if (rc.nBottom - rc.nTop < nLine) {
		nLine = *(int*) ((char*) pThis + 0x34) + 1;
	}
	else {
		nLine = nLine / *(int*) ((char*) pThis + 0x3c);
	}
	nTopLine = *(int*) ((char*) pThis + 0x38);
	(*(void(__fastcall**)(void*, int)) vtbl[1])(pThis, 0);
	return nTopLine + nLine;
	(void) nEdxSlop;
}
#endif
