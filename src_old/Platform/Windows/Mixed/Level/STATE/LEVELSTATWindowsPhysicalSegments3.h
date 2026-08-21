#if defined(LEMBALL_WINDOWS_PHYSICAL_3_SEGMENT_005)
#include <string.h>

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

	pThis = (char*) pObject;
	pGrid = *(char**) (pThis + 0x930);
	*(short*) (pThis + 0x9b8) = 0x10;
	*(short*) (pThis + 0x9ba) = 0x10;
	*(short*) (pThis + 0x9bc) = *(short*) (pThis + 0x938) - *(short*) (pThis + 0x950) - 3;
	*(short*) (pThis + 0x9be) = *(short*) (pThis + 0x93a) - *(short*) (pThis + 0x954) - 3;

	nX = *(short*) (pThis + 0x8d6);
	nY = *(short*) (pThis + 0x8d8);
	nX = (nX + ((nX >> 31) & 0x0f)) >> 4;
	nY = (nY + ((nY >> 31) & 0x0f)) >> 4;
	nRight = *(short*) (pThis + 0x8d6) + *(short*) (pThis + 0x8d2) - 1;
	nBottom = *(short*) (pThis + 0x8d8) + *(short*) (pThis + 0x8d4) - 1;
	nWidth = ((nRight + ((nRight >> 31) & 0x0f)) >> 4) - nX + 1;
	nHeight = ((nBottom + ((nBottom >> 31) & 0x0f)) >> 4) - nY + 1;
	nGridWidth = *(short*) (pGrid + 8);
	nGridHeight = *(short*) (pGrid + 0x0a);
	if (nX < nGridWidth && nY < nGridHeight) {
		if (nX < 0) {
			nWidth += nX;
			nX = 0;
		}
		if (nY < 0) {
			nHeight += nY;
			nY = 0;
		}
		if (nX + nWidth >= nGridWidth) {
			nWidth = nGridWidth - nX;
		}
		if (nY + nHeight >= nGridHeight) {
			nHeight = nGridHeight - nY;
		}
		if (nWidth > 0 && nHeight > 0) {
			pFirst = *(unsigned char**) (pGrid + 0x10) + nX + nY * nGridWidth;
			pSecond = *(unsigned char**) (pGrid + 0x14) + nX + nY * nGridWidth;
			while (nHeight-- != 0) {
				memset(pFirst, 1, nWidth);
				memset(pSecond, 1, nWidth);
				pFirst += nGridWidth;
				pSecond += nGridWidth;
			}
		}
	}
	((void(__fastcall*)(void*, int, short*)) 0x00401D1B)(pGrid, 0, (short*) (pThis + 0x8da));
}
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_3_SEGMENT_011)
extern void* AllocateVSMemBlock(unsigned int nBytes);

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
#endif
#if defined(LEMBALL_WINDOWS_PHYSICAL_3_SEGMENT_012)
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
#endif
