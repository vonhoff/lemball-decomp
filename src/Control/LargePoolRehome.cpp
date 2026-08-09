#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

#include <string.h>
#include <stdlib.h>

extern "C" {
__declspec(dllimport) void* __stdcall GlobalAlloc(unsigned long, unsigned long);
__declspec(dllimport) void* __stdcall GlobalFree(void*);
__declspec(dllimport) void* __stdcall GlobalLock(void*);
__declspec(dllimport) int __stdcall GlobalUnlock(void*);
__declspec(dllimport) int __stdcall OpenClipboard(void*);
__declspec(dllimport) int __stdcall EmptyClipboard(void);
__declspec(dllimport) void* __stdcall SetClipboardData(unsigned int, void*);
__declspec(dllimport) int __stdcall CloseClipboard(void);
__declspec(dllimport) int __stdcall MessageBoxA(void*, const char*, const char*, unsigned int);
__declspec(dllimport) int __stdcall GetClientRect(void*, void*);
__declspec(dllimport) void* __stdcall SelectObject(void*, void*);
__declspec(dllimport) unsigned long __stdcall SetTextColor(void*, unsigned long);
__declspec(dllimport) unsigned long __stdcall SetBkColor(void*, unsigned long);
__declspec(dllimport) int __stdcall ExtTextOutA(
	void*, int, int, unsigned int, const void*, const char*, unsigned int, const int*);
}

struct LargePoolRect
{
	long left;
	long top;
	long right;
	long bottom;
};

struct LargePoolSixDwords
{
	unsigned long m_values[6];
};

// FUNCTION: LEMBALL 0x0040F7E0
int __fastcall poll_managed_entity_stream_dirty_flag(void* pObject)
{
	char* pThis;
	int* pDirty;
	int nState;
	pThis = (char*)pObject;
	pDirty = (int*)(pThis + 0x164);

	if (((*(unsigned long*)(pThis + 0x194) ^ *(unsigned long*)(pThis + 0x9c)) & 0xfffff000UL) == 0 && *pDirty == 0)
		*pDirty = 0;
	else
		*pDirty = 1;
	if (((*(unsigned long*)(pThis + 0x198) ^ *(unsigned long*)(pThis + 0xa0)) & 0xfffff000UL) == 0 && *pDirty == 0)
		*pDirty = 0;
	else
		*pDirty = 1;
	if (((*(unsigned long*)(pThis + 0x19c) ^ *(unsigned long*)(pThis + 0xa4)) & 0xfffff000UL) == 0 && *pDirty == 0)
		*pDirty = 0;
	else
		*pDirty = 1;

	nState = *(int*)(pThis + 0xb8);
	if (nState == 0)
	{
		if (*(int*)(pThis + 0x1a4) == 0 && *pDirty == 0)
			*pDirty = 0;
		else
			*pDirty = 1;
	}
	else if (nState != 1)
	{
		if (*(int*)(pThis + 0x1a4) == nState && *pDirty == 0)
			*pDirty = 0;
		else
			*pDirty = 1;
		if (*(short*)(pThis + 0x1a8) == *(short*)(pThis + 0xbc) && *pDirty == 0)
			*pDirty = 0;
		else
			*pDirty = 1;
		if (*(int*)(pThis + 0x1ac) == *(int*)(pThis + 0x94) && *pDirty == 0)
			*pDirty = 0;
		else
			*pDirty = 1;
	}
	if (nState != 1)
		*(int*)(pThis + 0x1a4) = nState;

	if (*(int*)(pThis + 0x1b0) == *(int*)(pThis + 0x98) && *pDirty == 0)
		*pDirty = 0;
	else
		*pDirty = 1;

	*(unsigned long*)(pThis + 0x194) = *(unsigned long*)(pThis + 0x9c);
	*(short*)(pThis + 0x1a0) = *(short*)(pThis + 0xb4);
	*(unsigned long*)(pThis + 0x198) = *(unsigned long*)(pThis + 0xa0);
	*(unsigned long*)(pThis + 0x19c) = *(unsigned long*)(pThis + 0xa4);
	*(int*)(pThis + 0x1b0) = *(int*)(pThis + 0x98);
	*(short*)(pThis + 0x1a8) = *(short*)(pThis + 0xbc);
	*(int*)(pThis + 0x1ac) = *(int*)(pThis + 0x94);
	return *pDirty;
}

extern void __cdecl SelectionSortRecordsWithComparator(char*, char*, int, void*);

// FUNCTION: LEMBALL 0x00463960
void __cdecl VSQSort(char* pFirst, unsigned int nRecords, unsigned int nRecordSize, void* pComparator)
{
	char* pLeftStack[30];
	char* pRightStack[30];
	char* pRight;
	char* pScanLeft;
	char* pScanRight;
	unsigned int nPartitionRecords;
	int nStackDepth;

	if (nRecords < 2 || nRecordSize == 0)
		return;
	nStackDepth = 0;
	pRight = pFirst + (nRecords - 1) * nRecordSize;
partition:
	nPartitionRecords = (unsigned int)(pRight - pFirst) / nRecordSize + 1;
	if (nPartitionRecords <= 8)
	{
		SelectionSortRecordsWithComparator(pFirst, pRight, nRecordSize, pComparator);
		goto pop_partition;
	}
	((void(__cdecl*)(char*, char*, int))0x00463B20)(pFirst + (nPartitionRecords >> 1) * nRecordSize, pFirst, nRecordSize);
	pScanRight = pRight + nRecordSize;
	pScanLeft = pFirst;
scan_left:
	pScanLeft += nRecordSize;
	if (pScanLeft <= pRight && ((int(__cdecl*)(void*, void*))pComparator)(pScanLeft, pFirst) <= 0)
		goto scan_left;
scan_right:
	pScanRight -= nRecordSize;
	if (pScanRight > pFirst && ((int(__cdecl*)(void*, void*))pComparator)(pScanRight, pFirst) >= 0)
		goto scan_right;
	if (pScanLeft <= pScanRight)
	{
		((void(__cdecl*)(char*, char*, int))0x00463B20)(pScanLeft, pScanRight, nRecordSize);
		goto scan_left;
	}
	((void(__cdecl*)(char*, char*, int))0x00463B20)(pFirst, pScanRight, nRecordSize);
	if ((pScanRight - pFirst - 1) < (pRight - pScanLeft))
	{
		if (pScanLeft < pRight)
		{
			pLeftStack[nStackDepth] = pScanLeft;
			pRightStack[nStackDepth] = pRight;
			++nStackDepth;
		}
		if (pFirst + nRecordSize < pScanRight)
		{
			pRight = pScanRight - nRecordSize;
			goto partition;
		}
	}
	else
	{
		if (pFirst + nRecordSize < pScanRight)
		{
			pLeftStack[nStackDepth] = pFirst;
			pRightStack[nStackDepth] = pScanRight - nRecordSize;
			++nStackDepth;
		}
		pFirst = pScanLeft;
		if (pScanLeft < pRight)
			goto partition;
	}
pop_partition:
	--nStackDepth;
	if (nStackDepth < 0)
		return;
	pFirst = pLeftStack[nStackDepth];
	pRight = pRightStack[nStackDepth];
	goto partition;
}

// NOTE: 0x004766F0 unmatchable: natural clipped ZRLE decoder structure measured 15.20%.
// NOTE: 0x00476EE0 unmatchable: mirrored clipped decoder register/loop shape measured 7.06%.

// FUNCTION: LEMBALL 0x00468C50
void __fastcall update_buffered_geometry_child_fill_and_edges(void* pObject)
{
	char* pThis;
	char* pEntry;
	int nTarget;
	int nOffset;
	int nFill;
	int nEdge;
	int nActive;
	short nWidth;
	short nHeight;

	pThis = (char*)pObject;
	nTarget = *(int*)(*(int*)(pThis + 0x4c) + 0x0c);
	((void(__fastcall*)(void*))(*(void***)(*(int*)(*(int*)(nTarget + 0x40) + 4) + nTarget + 0x40))[0x38 / 4])(
		(void*)(*(int*)(*(int*)(nTarget + 0x40) + 4) + nTarget + 0x40));

	pEntry = *(char**)(pThis + 0x10c);
	*(short*)(pEntry + 4) = *(short*)(pThis + 0xc0);
	*(short*)(pEntry + 6) = *(short*)(pThis + 0xc2);
	*(short*)(pEntry + 8) = 0;
	*(short*)(pEntry + 0x0a) = 0;
	*(int*)(pEntry + 0x0c) = *(int*)(pThis + 0x114);
	((void(__fastcall*)(void*, int, void*))(*(void***)pEntry)[1])(
		pEntry, 0, *(void**)(pThis + 0x4c));

	nActive = (*(int*)(pThis + 0xd0) != 0 && *(int*)(pThis + 0x94) != 0);
	if (nActive)
	{
		nEdge = 0xf8;
		nFill = 0xff;
	}
	else
	{
		nEdge = 0xff;
		nFill = 0xf8;
	}

	pEntry = *(char**)(pThis + 0x110);
	nWidth = *(short*)(pThis + 0xc0);
	nHeight = *(short*)(pThis + 0xc2);
	*(short*)(pEntry + 4) = 0;
	*(short*)(pEntry + 6) = 0;
	*(short*)(pEntry + 8) = nWidth - 1;
	*(short*)(pEntry + 0x0a) = 0;
	*(int*)(pEntry + 0x0c) = nEdge;

	*(short*)(pEntry + 0x14) = 0;
	*(short*)(pEntry + 0x16) = 0;
	*(short*)(pEntry + 0x18) = 0;
	*(short*)(pEntry + 0x1a) = nHeight - 1;
	*(int*)(pEntry + 0x1c) = nEdge;

	*(short*)(pEntry + 0x24) = nWidth - 1;
	*(short*)(pEntry + 0x26) = 0;
	*(short*)(pEntry + 0x28) = nWidth - 1;
	*(short*)(pEntry + 0x2a) = nHeight - 1;
	*(int*)(pEntry + 0x2c) = nFill;

	*(short*)(pEntry + 0x34) = 0;
	*(short*)(pEntry + 0x36) = nHeight - 1;
	*(short*)(pEntry + 0x38) = nWidth - 1;
	*(short*)(pEntry + 0x3a) = nHeight - 1;
	*(int*)(pEntry + 0x3c) = nFill;

	nOffset = 0;
	do
	{
		pEntry = *(char**)(pThis + 0x110) + nOffset;
		nOffset += 0x10;
		((void(__fastcall*)(void*, int, void*))(*(void***)pEntry)[1])(
			pEntry, 0, *(void**)(pThis + 0x4c));
	} while (nOffset < 0x40);
}

// FUNCTION: LEMBALL 0x00469210
void __fastcall layout_resource_sprite_text_owner(void* pObject, int, char* pPrimaryText, char* pSecondaryText)
{
	char* pThis;
	short aPrimaryExtent[4];
	short aSecondaryExtent[4];
	short aRect[4];
	short aPrimaryPosition[2];
	short aSecondaryPosition[2];
	void* pChild;

	pThis = (char*)pObject;
	*(char**)(pThis + 0x11c) = pSecondaryText;
	*(char**)(pThis + 0x118) = pPrimaryText;
	((void(__fastcall*)(void*, int, short*, char*, int))0x0045DB30)(
		*(void**)(pThis + 0x134), 0, aPrimaryExtent, pPrimaryText, 0x20);
	((void(__fastcall*)(void*, int, short*))0x00469120)(pThis, 0, aPrimaryExtent);

	aSecondaryExtent[0] = aPrimaryExtent[0];
	aSecondaryExtent[1] = aPrimaryExtent[1];
	if (pSecondaryText != 0)
	{
		((void(__fastcall*)(void*, int, short*, char*, int))0x0045DB30)(
			*(void**)(pThis + 0x134), 0, aSecondaryExtent, pSecondaryText, 0x20);
		((void(__fastcall*)(void*, int, short*))0x00469120)(pThis, 0, aSecondaryExtent);
	}

	((void(__fastcall*)(void*, int, short*, short*))0x00469180)(
		pThis, 0, (short*)(pThis + 0x12c), aPrimaryPosition);
	if (pSecondaryText == 0)
	{
		*(char**)(pThis + 0x11c) = pPrimaryText;
		*(short*)(pThis + 0x128) = *(short*)(pThis + 0x12c);
		*(short*)(pThis + 0x12a) = *(short*)(pThis + 0x12e);
	}
	else
	{
		((void(__fastcall*)(void*, int, short*, short*))0x00469180)(
			pThis, 0, (short*)(pThis + 0x128), aSecondaryPosition);
	}
	++*(short*)(pThis + 0x128);
	++*(short*)(pThis + 0x12a);

	if (*(int*)(pThis + 0x13c) == 0)
	{
		aRect[0] = *(short*)(pThis + 0xc0);
		aRect[1] = *(short*)(pThis + 0xc2);
		aRect[2] = *(short*)(pThis + 0xdc);
		aRect[3] = *(short*)(pThis + 0xde);
		((void(__fastcall*)(void*, int, short*, void*, const char*))(*(void***)pThis)[1])(
			pThis, 0, aRect, *(void**)(pThis + 0xc8), (const char*)0x0049F02C);
		pChild = pThis != 0 ? pThis + 0x90 : 0;
		*(short*)(pThis + 0xc4) += *(short*)(pThis + 0x18);
		*(short*)(pThis + 0xc6) += *(short*)(pThis + 0x1a);
		((void(__fastcall*)(void*, int, void*))0x0046A9A0)(
			*(void**)(*(int*)(pThis + 0xc8) + 0x1c), 0, pChild);
		((void(__fastcall*)(void*, int, int))0x0046A530)(pThis + 0x90, 0, 1);
		*(int*)(pThis + 0x13c) = 1;
	}
	*(int*)(pThis + 0xd8) = 1;
}

extern void* g_pSharedRenderDispatchQueue;

// FUNCTION: LEMBALL 0x00443AF0
void __fastcall configure_level_screen_pause_dialog_options(void* pObject)
{
	char* pThis;
	const char** ppText;
	short* pRects;
	short* pRect;
	int nMode;
	int nIndex;
	int nRemaining;
	int nRectCount;
	int nChild;

	pThis = (char*)pObject;
	nIndex = 0;
	nMode = *(int*)(pThis + 0x104);
	if (nMode >= 0)
	{
		nRemaining = nMode + 1;
		do
		{
			*(int*)(pThis + 0x120) = 0;
			ppText = (const char**)0x0049F068 + nIndex;
			do
			{
				++ppText;
				++*(int*)(pThis + 0x120);
				++nIndex;
			} while (*ppText != 0);
			++nIndex;
			--nRemaining;
		} while (nRemaining != 0);
	}
	*(int*)(pThis + 0x124) = 0;
	*(int*)(pThis + 0x11c) = 0;
	*(const char***)(pThis + 0x100) = (const char**)0x0049F064 +
		(nIndex - *(int*)(pThis + 0x120));
	switch (nMode)
	{
	case 0:
		*(int*)(pThis + 0x124) = 1;
		if (((int(__fastcall*)(void*))(*(void***)*(void**)(pThis + 0x110))[1])(
			*(void**)(pThis + 0x110)) == 0)
		{
			++*(int*)(pThis + 0x124);
			++*(int*)(pThis + 0x11c);
		}
		*(int*)(pThis + 0x118) = *(int*)(pThis + 0x124);
		break;
	case 1:
	case 2:
	case 4:
		*(int*)(pThis + 0x124) = 1;
		*(int*)(pThis + 0x118) = 1;
		break;
	case 3:
		*(int*)(pThis + 0x124) = 1;
		*(int*)(pThis + 0x118) = 2;
		break;
	default:
		*(int*)(pThis + 0x118) = 0;
		break;
	}
	*(int*)(pThis + 0x128) = *(int*)(pThis + 0x118);
	nChild = pThis != 0 ? (int)(pThis + 0x90) : 0;
	((void(__fastcall*)(void*, int, int, int))0x004632A0)(
		g_pSharedRenderDispatchQueue, 0, nChild, 0);

	nRectCount = *(int*)(pThis + 0x120);
	*(int*)(pThis + 500) = 0;
	pRects = (short*)((void*(__cdecl*)(unsigned int))0x0045A780)(nRectCount * 8);
	if (pRects == 0)
		*(short**)(pThis + 0x1f8) = 0;
	else
	{
		pRect = pRects;
		while (--nRectCount >= 0)
		{
			((void(__fastcall*)(void*))0x004017C6)(pRect);
			pRect += 4;
		}
		*(short**)(pThis + 0x1f8) = pRects;
	}
	((void(__fastcall*)(void*))0x00402F54)(pThis);
	*(int*)(pThis + 0x208) = 0;
	*(int*)(pThis + 0x1dc) = 0;
}

// FUNCTION: LEMBALL 0x0043F480
void __fastcall mark_level_screen_occupancy_rect(void* pObject)
{
	char* pThis;
	char* pGrid;
	unsigned char* pFirst;
	unsigned char* pSecond;

	int nX;
	int nY;
	int nWidth;
	int nHeight;
	int nGridWidth;
	int nGridHeight;
	int nRight;
	int nBottom;


	pThis = (char*)pObject;
	pGrid = *(char**)(pThis + 0x930);
	*(short*)(pThis + 0x9b8) = 0x10;
	*(short*)(pThis + 0x9ba) = 0x10;
	*(short*)(pThis + 0x9bc) = *(short*)(pThis + 0x938) - *(short*)(pThis + 0x950) - 3;
	*(short*)(pThis + 0x9be) = *(short*)(pThis + 0x93a) - *(short*)(pThis + 0x954) - 3;

	nX = *(short*)(pThis + 0x8d6);
	nY = *(short*)(pThis + 0x8d8);
	nX = (nX + ((nX >> 31) & 0x0f)) >> 4;
	nY = (nY + ((nY >> 31) & 0x0f)) >> 4;
	nRight = *(short*)(pThis + 0x8d6) + *(short*)(pThis + 0x8d2) - 1;
	nBottom = *(short*)(pThis + 0x8d8) + *(short*)(pThis + 0x8d4) - 1;
	nWidth = ((nRight + ((nRight >> 31) & 0x0f)) >> 4) - nX + 1;
	nHeight = ((nBottom + ((nBottom >> 31) & 0x0f)) >> 4) - nY + 1;
	nGridWidth = *(short*)(pGrid + 8);
	nGridHeight = *(short*)(pGrid + 0x0a);
	if (nX < nGridWidth && nY < nGridHeight)
	{
		if (nX < 0)
		{
			nWidth += nX;
			nX = 0;
		}
		if (nY < 0)
		{
			nHeight += nY;
			nY = 0;
		}
		if (nX + nWidth >= nGridWidth)
			nWidth = nGridWidth - nX;
		if (nY + nHeight >= nGridHeight)
			nHeight = nGridHeight - nY;
		if (nWidth > 0 && nHeight > 0)
		{
			pFirst = *(unsigned char**)(pGrid + 0x10) + nX + nY * nGridWidth;
			pSecond = *(unsigned char**)(pGrid + 0x14) + nX + nY * nGridWidth;
			while (nHeight-- != 0)
			{
				memset(pFirst, 1, nWidth);
				memset(pSecond, 1, nWidth);
				pFirst += nGridWidth;
				pSecond += nGridWidth;
			}
		}
	}
	((void(__fastcall*)(void*, int, short*))0x00401D1B)(
		pGrid, 0, (short*)(pThis + 0x8da));
}

// FUNCTION: LEMBALL 0x004738E0
void __fastcall append_debug_text_line_record(void* pObject, int, char* pText, int nStyle)
{
	char* pThis;
	int* pRecord;
	int nIndex;
	int nOffset;
	char* pSource;

	pThis = (char*)pObject;
	if (*(int*)(pThis + 8) == *(int*)pThis && *(int*)pThis > 1)
	{
		nIndex = 0;
		nOffset = 0;
		do
		{
			pRecord = (int*)(*(char**)(pThis + 4) + nOffset);
			nStyle = pRecord[3];
			pSource = (char*)pRecord[4];
			if (pRecord[1] != 0)
				free((void*)pRecord[1]);
			pRecord[1] = (int)malloc(strlen(pSource) + 1);
			strcpy((char*)pRecord[1], pSource);
			++nIndex;
			nOffset += 0x0c;
			pRecord[0] = nStyle;
			pRecord[2] = 0;
		} while (nIndex < *(int*)pThis - 1);
	}
	pRecord = (int*)(*(char**)(pThis + 4) + *(int*)(pThis + 8) * 0x0c);
	if (pRecord[1] != 0)
		free((void*)pRecord[1]);
	pRecord[1] = (int)malloc(strlen(pText) + 1);
	strcpy((char*)pRecord[1], pText);
	pRecord[0] = nStyle;
	pRecord[2] = 0;
	if (*(int*)pThis - *(int*)(pThis + 8) != 1)
		++*(int*)(pThis + 8);
}

// FUNCTION: LEMBALL 0x00474620
void __fastcall copy_selected_text_to_clipboard(int* pWindow)
{
	char* pText;
	char* pTarget;
	void* hMemory;

	((void(__fastcall*)(int*))(*(void***)pWindow)[0])(pWindow);
	if (pWindow[0x14] == -1 || pWindow[0x15] == -1)
	{
		((void(__fastcall*)(int*))(*(void***)pWindow)[1])(pWindow);
		return;
	}
	pText = ((char*(__fastcall*)(int*))0x00474520)(pWindow);
	hMemory = GlobalAlloc(0x2002, strlen(pText) + 1);
	if (hMemory == 0)
	{
		free(pText);
		MessageBoxA(0, (const char*)0x004A2CE0, (const char*)0x004A2CD8, 0);
		((void(__fastcall*)(int*))(*(void***)pWindow)[1])(pWindow);
		return;
	}
	pTarget = (char*)GlobalLock(hMemory);
	strcpy(pTarget, pText);
	GlobalUnlock(hMemory);
	free(pText);
	if (!OpenClipboard((void*)pWindow[7]))
	{
		MessageBoxA(0, (const char*)0x004A2D08, (const char*)0x004A2D00, 0);
		GlobalFree(hMemory);
	}
	else
	{
		EmptyClipboard();
		SetClipboardData(1, hMemory);
		CloseClipboard();
	}
	((void(__fastcall*)(int*, int, int))0x004743E0)(pWindow, 0, 0);
	pWindow[0x15] = -1;
	pWindow[0x14] = -1;
	((void(__fastcall*)(int*))(*(void***)pWindow)[1])(pWindow);
}

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

	pThis = (char*)pObject;
	((void(__fastcall*)(void*))(*(void***)pThis)[0])(pThis);
	if (pPaint == 0)
		GetClientRect(*(void**)(pThis + 0x1c), &ClientRect);
	else
	{
		ClientRect.left = *(long*)(pPaint + 8);
		ClientRect.top = *(long*)(pPaint + 0x0c);
		ClientRect.right = *(long*)(pPaint + 0x10);
		ClientRect.bottom = *(long*)(pPaint + 0x14);
	}
	SelectObject(hdc, *(void**)(pThis + 0x20));
	nVisible = (ClientRect.bottom - ClientRect.top - 1 + *(int*)(pThis + 0x3c)) /
		*(int*)(pThis + 0x3c);
	nFirst = *(int*)(pThis + 0x38) + ClientRect.top / *(int*)(pThis + 0x3c);
	nLine = nFirst;
	nOffset = nFirst * 0x0c;
	while (nLine < nFirst + nVisible)
	{
		nTextColor = 0;
		nBackgroundColor = 0x00ffffff;
		if (nLine < *(int*)(pThis + 0x2c))
		{
			pRecord = (int*)(nOffset + *(int*)(*(int*)(pThis + 0x24) + 4));
			pText = (const char*)pRecord[1];
			nTextColor = pRecord[0];
			if (pRecord[2] != 0)
			{
				nBackgroundColor = 0;
				nTextColor = 0x00ffffff;
			}
		}
		else
			pText = (const char*)0x004A2C98;
		LineRect.top = (nLine - *(int*)(pThis + 0x38)) * *(int*)(pThis + 0x3c);
		LineRect.left = 0;
		LineRect.right = *(int*)(pThis + 0x40);
		LineRect.bottom = LineRect.top + *(int*)(pThis + 0x3c);
		SetTextColor(hdc, nTextColor);
		SetBkColor(hdc, nBackgroundColor);
		ExtTextOutA(hdc, LineRect.left, LineRect.top, 2, &LineRect,
			pText, strlen(pText), 0);
		++nLine;
		nOffset += 0x0c;
	}
	((void(__fastcall*)(void*))(*(void***)pThis)[1])(pThis);
}
