#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

#include <stdlib.h>
#include <string.h>

extern void* AllocateVSMemBlock(unsigned int nBytes);

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

struct LargePoolLevelCoord {
	int x;
	int y;
	int z;
};

// FUNCTION: LEMBALL 0x0040ca40
void __fastcall load_network_player_chunk_markers(void* pThis,
												  int nEdxSlop,
												  unsigned short* pData,
												  int nUnused,
												  int fExisting)
{
	typedef unsigned short(__cdecl * FindFreeSlotProc)(void);
	typedef unsigned short(__fastcall * SampleHeightProc)(void*, int, int, int);
	typedef void(__fastcall * AppendTrapDoorProc)(void*, int, unsigned short, LargePoolLevelCoord*, int, int);
	typedef void(__fastcall * SetTripletProc)(void*, int, int, int, int, int);
	typedef void(__fastcall * ConfigureRangesProc)(void*, int, int, int, int, int, int);
	unsigned int nCount;
	unsigned int anRanges[4];
	unsigned short* pEntry;
	LargePoolLevelCoord Position;
	unsigned short nSlot;
	void* pGrid;
	void* pOwner;
	int nTileX;
	int nTileY;
	int nIndex;

	nCount = *pData++;
	anRanges[0] = 0;
	anRanges[1] = 0;
	anRanges[2] = 0;
	anRanges[3] = 0;
	pEntry = pData;
	pOwner = *(void**) 0x004A74B0;

	for (nIndex = 0; nIndex < (int) nCount; ++nIndex) {
		if (fExisting == 0) {
			nSlot = ((FindFreeSlotProc) 0x0040227A)();
		}

		Position.x = (unsigned int) pEntry[0] << 12;
		Position.y = (unsigned int) pEntry[1] << 12;
		Position.z = 0xaa55aa55;
		pGrid = *(void**) 0x004A74B4;
		nTileX = Position.x >> 12;
		nTileY = Position.y >> 12;
		if (nTileX >= 0 && nTileY >= 0 && (nTileX >> 4) < *(int*) ((char*) pGrid + 0x10) &&
			(nTileY >> 4) < *(int*) ((char*) pGrid + 0x14)) {
			Position.z = (unsigned int) ((SampleHeightProc) 0x004029A5)(
							 (char*) *(void**) ((char*) pGrid + 0x0c) +
								 ((nTileY >> 4) * *(int*) ((char*) pGrid + 0x10) + (nTileX >> 4)) * 12,
							 0,
							 nTileX & 0x0f,
							 nTileY & 0x0f)
						 << 12;
		}
		else {
			Position.z = 0;
		}

		anRanges[nIndex] = pEntry[3];
		if (fExisting == 0) {
			((AppendTrapDoorProc) 0x00401393)(pThis, 0, nSlot, &Position, 1, 0);
		}
		((SetTripletProc) 0x0040107D)(pOwner, 0, Position.x >> 12, Position.y >> 12, Position.z >> 12, nIndex);
		pEntry += 4;
	}

	((ConfigureRangesProc) 0x00401D25)(pOwner, 0, nCount, anRanges[0], anRanges[1], anRanges[2], anRanges[3]);
	(void) nEdxSlop;
	(void) nUnused;
}

// FUNCTION: LEMBALL 0x0041f2e0
void __fastcall load_shpg_chunk_objects(void* pThis, int, unsigned char* pData, unsigned int cbData, int fExisting)
{
	typedef void(LEMBALL_FASTCALL * NoArgProc)(void*);
	typedef void(LEMBALL_FASTCALL * OneArgProc)(void*, int, int);
	typedef void*(__cdecl * AllocateProc)(unsigned int);
	typedef void*(LEMBALL_FASTCALL * ConstructGroupProc)(void*, int, void*, void*, void*);
	typedef void*(LEMBALL_FASTCALL * ConstructChildProc)(void*, int, void*, int, int, int, int);
	typedef void(LEMBALL_FASTCALL * AddGroupProc)(void*, int, void*);
	typedef void(LEMBALL_FASTCALL * AttachChildProc)(void*, int, void*, void*);
	unsigned int nRecords = cbData / 6;
	if (fExisting == 0) {
		while (nRecords != 0) {
			unsigned int nChildren = pData[0];
			unsigned int nFormation = pData[1];
			unsigned int nX = *(unsigned short*) (pData + 2);
			unsigned int nY = *(unsigned short*) (pData + 4);
			void* pMemory = ((AllocateProc) 0x0045A780)(0x16c);
			void* pGroup = 0;
			if (pMemory != 0) {
				pGroup = ((ConstructGroupProc) 0x00402E2D)(pMemory,
														   0,
														   *(void**) 0x004A782C,
														   *(void**) 0x004A7830,
														   *(void**) 0x004A7834);
			}
			void* pVtable = *(void**) pGroup;
			((NoArgProc) ((void**) pVtable)[0x104 / 4])(pGroup);
			((AddGroupProc) 0x004027E8)(pThis, 0, pGroup);
			((OneArgProc) ((void**) pVtable)[0x110 / 4])(pGroup, 0, nFormation);
			while (nChildren != 0) {
				pMemory = ((AllocateProc) 0x0045A780)(0x124);
				void* pChild = 0;
				if (pMemory != 0) {
					pChild = ((ConstructChildProc) 0x0040234C)(pMemory, 0, *(void**) 0x004A782C, nX, nY, 0, 0);
				}
				nX -= 0x14;
				nY -= 0x14;
				((NoArgProc) (*(void***) pChild)[0x104 / 4])(pChild);
				((AttachChildProc) 0x004017DF)(pVtable, 0, pChild, pGroup);
				--nChildren;
			}
			((OneArgProc) ((void**) pVtable)[0x154 / 4])(pGroup, 0, *(int*) 0x004A7834);
			pData += 6;
			--nRecords;
		}
	}
}

// FUNCTION: LEMBALL 0x00434D40
void __fastcall load_level_screen_variant_state_wrapper_range(void* pObject, int, int nFirst, int nLast, int nMode)
{
	char* pThis;
	void* pSelector;
	void* pAllocator;
	int nFrameCount;
	int i;

	pThis = (char*) pObject;
	if (*(int*) (pThis + 0x94) != 0) {
		*(int*) (pThis + 0x90) += nLast - nFirst;
		return;
	}
	if (nFirst > nLast) {
		return;
	}
	pAllocator = *(void**) (*(char**) (pThis + 0x70));
	for (i = nFirst; i <= nLast; ++i) {
		((void(__fastcall*)(void*, int, int)) 0x467490)(pObject, 0, i);
		pSelector = 0;
		switch (nMode) {
		case 0:
			pSelector = AllocateVSMemBlock(0x10);
			if (pSelector) {
				*(void**) pSelector = (void*) 0x496ce4;
				*(int*) ((char*) pSelector + 4) = 1;
				*(int*) ((char*) pSelector + 0x0c) = 0;
			}
			break;
		case 1:
			nFrameCount = ((int(__fastcall*)(void*, int, int)) 0x467540)(pObject, 0, i);
			pSelector = AllocateVSMemBlock(0x1c);
			if (pSelector) {
				*(void**) pSelector = (void*) 0x496ce8;
				*(int*) ((char*) pSelector + 4) = nFrameCount;
				*(int*) ((char*) pSelector + 0x18) = 1;
			}
			break;
		case 2:
			pSelector = AllocateVSMemBlock(0x10);
			if (pSelector) {
				*(void**) pSelector = (void*) 0x496d04;
				*(int*) ((char*) pSelector + 4) = 1;
				*(int*) ((char*) pSelector + 0x0c) = 0;
			}
			break;
		case 3:
			nFrameCount = ((int(__fastcall*)(void*, int, int)) 0x467540)(pObject, 0, i);
			pSelector = AllocateVSMemBlock(0x1c);
			if (pSelector) {
				*(void**) pSelector = (void*) 0x496d20;
				*(int*) ((char*) pSelector + 4) = nFrameCount;
				*(int*) ((char*) pSelector + 0x18) = 1;
			}
			break;
		}
		*(void**) (*(char**) (pThis + 0x74) + *(short*) (*(char**) (pThis + 0x28) + i * 2) * 4) = pSelector;
		((void(__fastcall*)(void*, int)) pAllocator)(pThis + 0x70, 0);
	}
}

// FUNCTION: LEMBALL 0x0043ED20
void __fastcall keep_level_screen_follow_target_in_view(char* pThis)
{
	int aPosition[3];
	int nExtra;
	int nTileX;
	int nTileY;
	int nHeight;
	int nMode;
	int nMarginX;
	int nMarginY;
	int nViewX;
	int nViewY;
	int nOldX;
	int nOldY;
	int nDx;
	int nDy;
	int fChanged;
	char* pGrid;
	aPosition[0] = aPosition[1] = aPosition[2] = 0xaa55aa55;
	fChanged = 0;
	if (*(int*) (pThis + 0xa48) == 1 && *(unsigned short*) (pThis + 0xa4e) != 0) {
		((void(__fastcall*)(void*, int, unsigned int, int*)) 0x403148)(
			*(void**) (pThis + 0x96c),
			0,
			*(unsigned short*) (pThis + 0xa4e + *(unsigned short*) (pThis + 0xa4e) * 2),
			aPosition);
	}
	else if (!((int(__fastcall*)(void*, int, int*, int*)) 0x4028d8)(*(void**) (pThis + 0x96c), 0, aPosition, &nExtra)) {
		return;
	}
	pGrid = *(char**) (pThis + 0x914);
	nTileX = aPosition[0] >> 12;
	nTileY = aPosition[1] >> 12;
	if (nTileX < 0 || nTileY < 0 || nTileX >> 4 >= *(int*) (pGrid + 0x10) || nTileY >> 4 >= *(int*) (pGrid + 0x14)) {
		nHeight = 0;
	}
	else {
		nHeight = ((unsigned short(__fastcall*)(void*, int, int, int)) 0x4029a5)(
			*(char**) (pGrid + 0x0c) + ((nTileY >> 4) * *(int*) (pGrid + 0x10) + (nTileX >> 4)) * 12,
			0,
			nTileX & 0x0f,
			nTileY & 0x0f);
	}
	aPosition[2] = nHeight << 12;
	nMode = *(int*) (*(char**) (pThis + 0x96c) + 0x108);
	if (nMode != 0 && nMode != 2) {
		return;
	}
	nMarginX = *(short*) (pThis + 0x958) * 2 / 5;
	nMarginY = *(short*) (pThis + 0x95a) * 2 / 5;
	*(int*) (pThis + 0x944) = aPosition[0];
	*(int*) (pThis + 0x948) = aPosition[1];
	*(int*) (pThis + 0x94c) = aPosition[2];
	nTileX = aPosition[0] >> 12;
	nTileY = aPosition[1] >> 12;
	nHeight = aPosition[2] >> 12;
	((void(__fastcall*)(void*, int, int*, int*)) 0x40199c)(pGrid, 0, &nTileX, &nTileY);
	nViewX = nTileX;
	nViewY = nTileY - nHeight;
	nOldX = *(int*) (pThis + 0x918);
	nOldY = *(int*) (pThis + 0x91c);
	nDx = nViewX - nOldX;
	nDy = nViewY - nOldY;
	if (nDx < nMarginX) {
		fChanged = 1;
		*(int*) (pThis + 0x918) = nViewX - nMarginX;
	}
	if (nDy < nMarginY) {
		fChanged = 1;
		*(int*) (pThis + 0x91c) = nViewY - nMarginY;
	}
	if (*(short*) (pThis + 0x958) - nMarginX < nDx) {
		fChanged = 1;
		*(int*) (pThis + 0x918) = nViewX - *(short*) (pThis + 0x958) + nMarginX;
	}
	if (*(short*) (pThis + 0x95a) - nMarginY < nDy) {
		fChanged = 1;
		*(int*) (pThis + 0x91c) = nViewY - *(short*) (pThis + 0x95a) + nMarginY;
	}
	if (fChanged) {
		((void(__fastcall*)(void*, int)) 0x4019ec)(pThis, 0);
		*(int*) (pThis + 0x2218) = 1;
		*(short*) (pThis + 0x221c) = (short) nOldX - *(short*) (pThis + 0x918);
		*(short*) (pThis + 0x221e) = (short) nOldY - *(short*) (pThis + 0x91c);
	}
}

// FUNCTION: LEMBALL 0x0044B750
void* __fastcall construct_registration_info_screen(char* pThis,
													int,
													int* pRenderOwner,
													void* pRenderTarget,
													short* pSize)
{
	int nIndex;
	char* pEntry;
	void* pResource;
	int* pText;
	unsigned int nTime;
	*(void**) pThis = (void*) 0x496d48;
	((void(__fastcall*)(void*, int)) 0x462ea0)(pThis + 4, 0);
	*(void**) (pThis + 0x24) = (void*) 0x496ca8;
	*(int*) (pThis + 0x18) = 0;
	*(void**) (pThis + 0x24) = (void*) 0x496d38;
	*(void**) (pThis + 0x34) = (void*) 0x496c98;
	*(void**) (pThis + 0x3c) = (void*) 0x496ca8;
	*(int*) (pThis + 0x28) = 0;
	*(int*) (pThis + 0x2c) = 0;
	*(int*) (pThis + 0x40) = 0;
	*(void**) (pThis + 0x3c) = (void*) 0x497928;
	*(int*) (pThis + 0x44) = 0;
	*(void**) (pThis + 0x3c) = (void*) 0x497918;
	*(int*) (pThis + 0x48) = 0;
	*(int*) (pThis + 0x5c) = 0;
	pEntry = pThis + 0x64;
	for (nIndex = 0; nIndex < 2; ++nIndex, pEntry += 0x10) {
		((void(__fastcall*)(void*, int)) 0x403139)(pEntry, 0);
	}
	*(void**) pThis = (void*) 0x497cb0;
	*(void**) (pThis + 4) = (void*) 0x497ca0;
	*(int*) (pThis + 0x14) = 0;
	((void(__fastcall*)(void*, int, int)) 0x46b370)(*(void**) 0x4a9bf4, 0, 0);
	*(int*) (pThis + 0x38) = 0;
	((void(__fastcall*)(void*, int, void*, int)) 0x4632a0)(*(void**) 0x4a9360, 0, pThis + 4, 0);
	*(int**) (pThis + 0x1c) = pRenderOwner;
	*(void**) (pThis + 0x20) = pRenderTarget;
	*(short*) (pThis + 0x18) = pSize[0];
	*(short*) (pThis + 0x1a) = pSize[1];
	((void(__fastcall*)(int*, int, int))(*(void***) pRenderOwner)[0x2b])(pRenderOwner, 0, 0x102);
	*(void**) (pThis + 0x60) = ((void*(__cdecl*) (int) ) 0x45e210)(0x101);
	*(int*) (pThis + 0x8c) = 0;
	pResource = ((void*(__cdecl*) (int) ) 0x45a780)(0x24);
	if (pResource) {
		*(void**) (pThis + 0x90) =
			((void*(__fastcall*) (void*, int, int, int, int, int) ) 0x469c60)(pResource, 0, 0x2b6, 1, 10, 0);
	}
	else {
		*(void**) (pThis + 0x90) = 0;
	}
	((void(__fastcall*)(void*, int, int)) 0x469eb0)(*(void**) (pThis + 0x90), 0, 0x32);
	pText = ((int*(__cdecl*) (int) ) 0x45de00)(0x100);
	*(int**) (pThis + 0x84) = pText;
	if (pText[4]) {
		pText[9] = 0;
	}
	else {
		((void(__fastcall*)(int*, int))(*(void***) pText)[7])(pText, 0);
	}
	++pText[2];
	*(int*) (pThis + 0x88) = pText[14];
	nTime = ((unsigned int(__cdecl*)(void)) 0x462e80)();
	*(unsigned int*) (pThis + 0x94) = nTime;
	*(unsigned int*) (pThis + 0x98) = nTime + 5000;
	return pThis;
}

// FUNCTION: LEMBALL 0x0044BCA0
void __fastcall prepare_registration_info_screen_active_upload(char* pThis)
{
	void* pUpload;
	void* pRender;
	int nCount;
	int nIndex;
	short* pRect;
	short aRect[4];
	int nBitmap;
	short aZero[2];
	pRender = *(void**) (*(char**) (pThis + 0x20) + 0x0c);
	pUpload = ((void*(__fastcall*) (void*, int) )(*(void***) pRender)[2])(pRender, 0);
	nCount = ((int(__fastcall*)(void*, int)) 0x466ef0)(pUpload, 0);
	nIndex = ((int(__fastcall*)(void*, int)) 0x467020)(pUpload, 0);
	if (nIndex < nCount) {
		pRect = ((short*(__fastcall*) (void*, int, int) ) 0x467000)(pUpload, 0, nIndex);
		aRect[0] = pRect[0];
		aRect[1] = pRect[1];
		aRect[2] = pRect[2];
		aRect[3] = pRect[3];
		while (++nIndex < nCount) {
			pRect = ((short*(__fastcall*) (void*, int, int) ) 0x467000)(pUpload, 0, nIndex);
			((void(__fastcall*)(short*, int, short*)) 0x402798)(aRect, 0, pRect);
		}
		if ((int) aRect[0] * aRect[1] > 0) {
			if (*(short*) (pThis + 0x18) < aRect[0]) {
				aRect[0] = *(short*) (pThis + 0x18);
			}
			if (*(short*) (pThis + 0x1a) < aRect[1]) {
				aRect[1] = *(short*) (pThis + 0x1a);
			}
			*(short*) (pThis + 0x68) = aRect[0];
			*(short*) (pThis + 0x6a) = aRect[1];
			*(short*) (pThis + 0x6c) = aRect[2];
			*(short*) (pThis + 0x6e) = aRect[3];
			*(int*) (pThis + 0x70) = 0;
			((void(__fastcall*)(void*, int, void*))(
				*(void***) (pThis + 0x64))[1])(pThis + 0x64, 0, *(void**) (pThis + 0x20));
			nBitmap = *(int*) (pThis + 0x60);
			*(short*) (pThis + 0x28) = *(short*) (pThis + 0x18);
			*(short*) (pThis + 0x2a) = *(short*) (pThis + 0x1a);
			*(int*) (pThis + 0x2c) = 0;
			*(int*) (pThis + 0x30) = 0;
			((void(__fastcall*)(void*, int, void*))(
				*(void***) (pThis + 0x24))[1])(pThis + 0x24, 0, *(void**) (pThis + 0x20));
			*(short*) (pThis + 0x40) = (*(short*) (pThis + 0x18) - *(short*) (nBitmap + 0x48)) / 2;
			*(short*) (pThis + 0x42) = (*(short*) (pThis + 0x1a) - *(short*) (nBitmap + 0x4a)) / 2;
			*(int*) (pThis + 0x4c) = nBitmap;
			*(int*) (pThis + 0x50) = 0x800;
			*(int*) (pThis + 0x54) = 0;
			((void(__fastcall*)(void*, int, void*))(
				*(void***) (pThis + 0x3c))[1])(pThis + 0x3c, 0, *(void**) (pThis + 0x20));
			((void(__fastcall*)(void*, int)) 0x403733)(pThis, 0);
			aZero[0] = aZero[1] = 0;
			*(int*) (pThis + 0x80) = 0;
			((void(__fastcall*)(void*, int, void*))(
				*(void***) (pThis + 0x74))[1])(pThis + 0x74, 0, *(void**) (pThis + 0x20));
		}
	}
	((void(__fastcall*)(void*, int)) 0x466b60)(pUpload, 0);
	((void(__fastcall*)(void*, int, void*)) 0x4670f0)(*(void**) (pThis + 0x20), 0, pThis + 0x34);
}
