#if defined(LEMBALL_WINDOWS_PHYSICAL_3_SEGMENT_006)
#include <stdlib.h>
#include <string.h>

// FUNCTION: LEMBALL 0x004738E0
void __fastcall append_debug_text_line_record(void* pObject, int, char* pText, int nStyle)
{
	char* pThis;
	int* pRecord;
	int nIndex;
	int nOffset;
	char* pSource;

	pThis = (char*) pObject;
	if (*(int*) (pThis + 8) == *(int*) pThis && *(int*) pThis > 1) {
		nIndex = 0;
		nOffset = 0;
		do {
			pRecord = (int*) (*(char**) (pThis + 4) + nOffset);
			nStyle = pRecord[3];
			pSource = (char*) pRecord[4];
			if (pRecord[1] != 0) {
				free((void*) pRecord[1]);
			}
			pRecord[1] = (int) malloc(strlen(pSource) + 1);
			strcpy((char*) pRecord[1], pSource);
			++nIndex;
			nOffset += 0x0c;
			pRecord[0] = nStyle;
			pRecord[2] = 0;
		} while (nIndex < *(int*) pThis - 1);
	}
	pRecord = (int*) (*(char**) (pThis + 4) + *(int*) (pThis + 8) * 0x0c);
	if (pRecord[1] != 0) {
		free((void*) pRecord[1]);
	}
	pRecord[1] = (int) malloc(strlen(pText) + 1);
	strcpy((char*) pRecord[1], pText);
	pRecord[0] = nStyle;
	pRecord[2] = 0;
	if (*(int*) pThis - *(int*) (pThis + 8) != 1) {
		++*(int*) (pThis + 8);
	}
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_3_SEGMENT_007)
#include <stdlib.h>
#include <string.h>

extern "C"
{
	__declspec(dllimport) void* __stdcall GlobalAlloc(unsigned long, unsigned long);
	__declspec(dllimport) void* __stdcall GlobalFree(void*);
	__declspec(dllimport) void* __stdcall GlobalLock(void*);
	__declspec(dllimport) int __stdcall GlobalUnlock(void*);
	__declspec(dllimport) int __stdcall OpenClipboard(void*);
	__declspec(dllimport) int __stdcall EmptyClipboard(void);
	__declspec(dllimport) void* __stdcall SetClipboardData(unsigned int, void*);
	__declspec(dllimport) int __stdcall CloseClipboard(void);
	__declspec(dllimport) int __stdcall MessageBoxA(void*, const char*, const char*, unsigned int);
}

// FUNCTION: LEMBALL 0x00474620
void __fastcall copy_selected_text_to_clipboard(int* pWindow)
{
	char* pText;
	char* pTarget;
	void* hMemory;

	((void(__fastcall*)(int*))(*(void***) pWindow)[0])(pWindow);
	if (pWindow[0x14] == -1 || pWindow[0x15] == -1) {
		((void(__fastcall*)(int*))(*(void***) pWindow)[1])(pWindow);
		return;
	}
	pText = ((char*(__fastcall*) (int*) ) 0x00474520)(pWindow);
	hMemory = GlobalAlloc(0x2002, strlen(pText) + 1);
	if (hMemory == 0) {
		free(pText);
		MessageBoxA(0, (const char*) 0x004A2CE0, (const char*) 0x004A2CD8, 0);
		((void(__fastcall*)(int*))(*(void***) pWindow)[1])(pWindow);
		return;
	}
	pTarget = (char*) GlobalLock(hMemory);
	strcpy(pTarget, pText);
	GlobalUnlock(hMemory);
	free(pText);
	if (!OpenClipboard((void*) pWindow[7])) {
		MessageBoxA(0, (const char*) 0x004A2D08, (const char*) 0x004A2D00, 0);
		GlobalFree(hMemory);
	}
	else {
		EmptyClipboard();
		SetClipboardData(1, hMemory);
		CloseClipboard();
	}
	((void(__fastcall*)(int*, int, int)) 0x004743E0)(pWindow, 0, 0);
	pWindow[0x15] = -1;
	pWindow[0x14] = -1;
	((void(__fastcall*)(int*))(*(void***) pWindow)[1])(pWindow);
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_3_SEGMENT_008)
#include <string.h>

extern "C"
{
	__declspec(dllimport) int __stdcall GetClientRect(void*, void*);
	__declspec(dllimport) void* __stdcall SelectObject(void*, void*);
	__declspec(dllimport) unsigned long __stdcall SetTextColor(void*, unsigned long);
	__declspec(dllimport) unsigned long __stdcall SetBkColor(void*, unsigned long);
	__declspec(dllimport) int __stdcall ExtTextOutA(void*,
													int,
													int,
													unsigned int,
													const void*,
													const char*,
													unsigned int,
													const int*);
}

struct LargePoolRect {
	long left;
	long top;
	long right;
	long bottom;
};

// NOTE: 0x004750C0 unmatchable: natural clipped line rasterizer measured 17.81%.
// NOTE: 0x00475290 unmatchable: natural circle-outline command structure measured 8.40%.

// FUNCTION: LEMBALL 0x00474130
void __fastcall paint_debug_text_window_lines(void* pObject, int, void* hdc, char* pPaint)
{
	char* pThis;
	LargePoolRect ClientRect;
	LargePoolRect LineRect;
	int nVisible;
	int nFirst;
	int nLine;
	int nOffset;
	int* pRecord;
	unsigned long nTextColor;
	unsigned long nBackgroundColor;
	const char* pText;

	pThis = (char*) pObject;
	((void(__fastcall*)(void*))(*(void***) pThis)[0])(pThis);
	if (pPaint == 0) {
		GetClientRect(*(void**) (pThis + 0x1c), &ClientRect);
	}
	else {
		ClientRect.left = *(long*) (pPaint + 8);
		ClientRect.top = *(long*) (pPaint + 0x0c);
		ClientRect.right = *(long*) (pPaint + 0x10);
		ClientRect.bottom = *(long*) (pPaint + 0x14);
	}
	SelectObject(hdc, *(void**) (pThis + 0x20));
	nVisible = (ClientRect.bottom - ClientRect.top - 1 + *(int*) (pThis + 0x3c)) / *(int*) (pThis + 0x3c);
	nFirst = *(int*) (pThis + 0x38) + ClientRect.top / *(int*) (pThis + 0x3c);
	nLine = nFirst;
	nOffset = nFirst * 0x0c;
	while (nLine < nFirst + nVisible) {
		nTextColor = 0;
		nBackgroundColor = 0x00ffffff;
		if (nLine < *(int*) (pThis + 0x2c)) {
			pRecord = (int*) (nOffset + *(int*) (*(int*) (pThis + 0x24) + 4));
			pText = (const char*) pRecord[1];
			nTextColor = pRecord[0];
			if (pRecord[2] != 0) {
				nBackgroundColor = 0;
				nTextColor = 0x00ffffff;
			}
		}
		else {
			pText = (const char*) 0x004A2C98;
		}
		LineRect.top = (nLine - *(int*) (pThis + 0x38)) * *(int*) (pThis + 0x3c);
		LineRect.left = 0;
		LineRect.right = *(int*) (pThis + 0x40);
		LineRect.bottom = LineRect.top + *(int*) (pThis + 0x3c);
		SetTextColor(hdc, nTextColor);
		SetBkColor(hdc, nBackgroundColor);
		ExtTextOutA(hdc, LineRect.left, LineRect.top, 2, &LineRect, pText, strlen(pText), 0);
		++nLine;
		nOffset += 0x0c;
	}
	((void(__fastcall*)(void*))(*(void***) pThis)[1])(pThis);
}
#endif
