#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/GDI/VSGDI.H"

#include <string.h>

extern void* g_GDISUBVT_ListCharEntryTableVtable[4];
extern void* g_GDISUBVT_ListCharEntryTableDeletingVtable[4];
extern void FreeResourceArchiveMemory(void* pMemoryBlock);

__inline static void MergeCompactHelperRect(short* pTarget, const short* pEntry)
{
	short nRight;
	short nBottom;

	if (pEntry[2] < pTarget[2]) {
		pTarget[0] = (short) (pTarget[0] + pTarget[2] - pEntry[2]);
		pTarget[2] = pEntry[2];
	}
	nRight = (short) (pEntry[2] + pEntry[0]);
	if ((short) (pTarget[2] + pTarget[0]) < nRight) {
		pTarget[0] = (short) (nRight - pTarget[2]);
	}
	if (pEntry[3] < pTarget[3]) {
		pTarget[1] = (short) (pTarget[1] + pTarget[3] - pEntry[3]);
		pTarget[3] = pEntry[3];
	}
	nBottom = (short) (pEntry[3] + pEntry[1]);
	if ((short) (pTarget[3] + pTarget[1]) < nBottom) {
		pTarget[1] = (short) (nBottom - pTarget[3]);
	}
}

struct CompactHelperPoint {
	short m_nX;
	short m_nY;
};

struct CompactHelperDirtyPointSink {
	virtual void ReservedSlot0(void) = 0;
	virtual void ReservedSlot1(void) = 0;
	virtual void ReservedSlot2(void) = 0;
	virtual void ReservedSlot3(void) = 0;
	virtual void ReservedSlot4(void) = 0;
	virtual void ReservedSlot5(void) = 0;
	virtual void ReservedSlot6(void) = 0;
	virtual void ReservedSlot7(void) = 0;
	virtual void ReservedSlot8(void) = 0;
	virtual void ReservedSlot9(void) = 0;
	virtual void PropagateDirtyPoint(const CompactHelperPoint* pPoint) = 0;
};

struct CompactHelperGroup0DirtyStateView {
	void SetDirtyFlagAndPropagate(int fDirty);
};

struct CompactHelperLinkNode {
	void* m_pTarget;
	CompactHelperLinkNode* m_pPrev;
};

void LEMBALL_FASTCALL EnsureHelperBackingRowIndexCapacity(VsGdiHelperSurface* pSurface);

__inline static void PlotCompactCirclePixel(int* pRows,
											int x,
											int y,
											int xMin,
											int yMin,
											int xMax,
											int yMax,
											unsigned char nColor)
{
	if (x >= xMin && x <= xMax && y >= yMin && y <= yMax) {
		*(unsigned char*) (pRows[y] + x) = nColor;
	}
}

__inline static void FillCompactCircleSpan(int* pRows,
										   int y,
										   int xCenter,
										   int nRadius,
										   int xMin,
										   int yMin,
										   int xMax,
										   int yMax,
										   unsigned char nColor)
{
	int xLeft;
	int xRight;
	if (y < yMin || y > yMax) {
		return;
	}
	xLeft = xCenter - nRadius;
	xRight = xCenter + nRadius;
	if (xLeft < xMin) {
		xLeft = xMin;
	}
	if (xRight > xMax) {
		xRight = xMax;
	}
	if (xLeft <= xRight) {
		memset((void*) (pRows[y] + xLeft), nColor, xRight - xLeft + 1);
	}
}

// These small entries are compiler-generated adjustor and interface thunks.

void LEMBALL_FASTCALL CompactHelperSubobjectSlot1(void* pObject, int, void* pEntryObject)
{
	char* pCompactTarget;
	char* pLinkedTarget;
	int nVariableOffset;
	int nLinkedOffset;
	short* pEntry;
	short* pLocalRect;
	short* pLinkedRect;

	pCompactTarget = (char*) pObject - *(int*) ((char*) pObject - 4);
	pEntry = (short*) pEntryObject;
	nVariableOffset = *(int*) (*(int*) (pCompactTarget - 0x51c) + 4);
	pLocalRect = (short*) (pCompactTarget + nVariableOffset - 0x4f8);

	if ((*(unsigned char*) ((char*) pEntryObject + 0x0d) & 0x10) == 0) {
		pLocalRect[0] = pEntry[2];
		pLocalRect[1] = pEntry[3];
		pLocalRect[2] = pEntry[4];
		pLocalRect[3] = pEntry[5];
	}
	else if (pEntry[2] * pEntry[3] != 0) {
		MergeCompactHelperRect(pLocalRect, pEntry + 2);
	}

	pLinkedTarget = *(char**) (pCompactTarget + nVariableOffset - 0x4fc);
	if (pLinkedTarget != (char*) g_pResourceGeometryHelperTarget &&
		(*(unsigned char*) ((char*) pEntryObject + 0x0e) & 1) == 0) {
		nLinkedOffset = *(int*) (*(int*) (pLinkedTarget + 0x40) + 4);
		pLinkedRect = (short*) (pLinkedTarget + nLinkedOffset + 0x64);
		MergeCompactHelperRect(pLocalRect, pLinkedRect);
		if (pLocalRect[0] < 1 || pLocalRect[1] < 1) {
			pLocalRect[0] = 0;
			pLocalRect[1] = 0;
			pLocalRect[2] = 0;
			pLocalRect[3] = 0;
		}
	}
}

void LEMBALL_FASTCALL CompactHelperSubobjectSlot2(void* pObject, int, void* pCommand)
{
	typedef void(LEMBALL_FASTCALL * DirtyRectProc)(void*, int, short*);
	char* pCompact;
	char* pClipView;
	char* pTarget;
	int* pRows;
	short DirtyRect[4];
	int xCenter;
	int yCenter;
	int nRadius;
	int xMin;
	int yMin;
	int xMax;
	int yMax;
	int x;
	int y;
	int nDecision;
	int nDelta;
	int nThreshold;
	int nOldThreshold;
	int nAdjustment;
	unsigned char nColor;

	pCompact = (char*) pObject - *(int*) ((char*) pObject - 4);
	pTarget = pCompact - 0x55c;
	nAdjustment = *(int*) (*(int*) (pCompact - 0x51c) + 4);
	pClipView = pCompact + nAdjustment;
	xMin = *(short*) (pClipView - 0x4f4);
	yMin = *(short*) (pClipView - 0x4f2);
	xMax = xMin + *(short*) (pClipView - 0x4f8) - 1;
	yMax = yMin + *(short*) (pClipView - 0x4f6) - 1;
	if (xMax < xMin || yMax < yMin) {
		return;
	}
	xCenter = *(short*) ((char*) pCommand + 4);
	yCenter = *(short*) ((char*) pCommand + 6);
	nRadius = *(int*) ((char*) pCommand + 8);
	if (nRadius < 0) {
		nRadius = -nRadius;
	}
	nColor = (unsigned char) *(int*) ((char*) pCommand + 0x0c);
	pRows = *(int**) (pCompact - 0x558);

	PlotCompactCirclePixel(pRows, xCenter, yCenter + nRadius, xMin, yMin, xMax, yMax, nColor);
	PlotCompactCirclePixel(pRows, xCenter, yCenter - nRadius, xMin, yMin, xMax, yMax, nColor);
	FillCompactCircleSpan(pRows, yCenter, xCenter, nRadius, xMin, yMin, xMax, yMax, nColor);
	x = 0;
	y = nRadius;
	nDecision = 0;
	nDelta = 1;
	nThreshold = nRadius * 2 - 1;
	while (x < y) {
		nOldThreshold = nThreshold;
		++x;
		nDecision += nDelta;
		nDelta += 2;
		if (nDecision * 2 > nThreshold) {
			--y;
			nDecision -= nThreshold;
			nThreshold -= 2;
		}
		if (x <= y) {
			if (nOldThreshold != nThreshold) {
				FillCompactCircleSpan(pRows, yCenter + x, xCenter, y, xMin, yMin, xMax, yMax, nColor);
				FillCompactCircleSpan(pRows, yCenter - x, xCenter, y, xMin, yMin, xMax, yMax, nColor);
			}
			if (x < y) {
				FillCompactCircleSpan(pRows, yCenter + y, xCenter, x, xMin, yMin, xMax, yMax, nColor);
				FillCompactCircleSpan(pRows, yCenter - y, xCenter, x, xMin, yMin, xMax, yMax, nColor);
			}
		}
	}

	DirtyRect[2] = (short) (xCenter - nRadius < xMin ? xMin : xCenter - nRadius);
	DirtyRect[3] = (short) (yCenter - nRadius < yMin ? yMin : yCenter - nRadius);
	DirtyRect[0] = (short) ((xCenter + nRadius + 1 > xMax + 1 ? xMax + 1 : xCenter + nRadius + 1) - DirtyRect[2]);
	DirtyRect[1] = (short) ((yCenter + nRadius + 1 > yMax + 1 ? yMax + 1 : yCenter + nRadius + 1) - DirtyRect[3]);
	if (DirtyRect[0] > 0 && DirtyRect[1] > 0) {
		((DirtyRectProc) (*(void***) pTarget)[1])(pTarget, 0, DirtyRect);
	}
}

void LEMBALL_FASTCALL CompactHelperSubobjectSlot3(void* pObject, int, void* pCommand)
{
	typedef void(LEMBALL_FASTCALL * DirtyRectProc)(void*, int, short*);
	char* pCompact;
	char* pClipView;
	char* pTarget;
	int* pRows;
	short DirtyRect[4];
	int xCenter;
	int yCenter;
	int nRadius;
	int xMin;
	int yMin;
	int xMax;
	int yMax;
	int x;
	int y;
	int nDecision;
	int nDelta;
	int nThreshold;
	int nAdjustment;
	unsigned char nColor;

	pCompact = (char*) pObject - *(int*) ((char*) pObject - 4);
	pTarget = pCompact - 0x55c;
	nAdjustment = *(int*) (*(int*) (pCompact - 0x51c) + 4);
	pClipView = pCompact + nAdjustment;
	xMin = *(short*) (pClipView - 0x4f4);
	yMin = *(short*) (pClipView - 0x4f2);
	xMax = xMin + *(short*) (pClipView - 0x4f8) - 1;
	yMax = yMin + *(short*) (pClipView - 0x4f6) - 1;
	if (xMax < xMin || yMax < yMin) {
		return;
	}
	xCenter = *(short*) ((char*) pCommand + 4);
	yCenter = *(short*) ((char*) pCommand + 6);
	nRadius = *(int*) ((char*) pCommand + 8);
	if (nRadius < 0) {
		nRadius = -nRadius;
	}
	nColor = (unsigned char) *(int*) ((char*) pCommand + 0x0c);
	pRows = *(int**) (pCompact - 0x558);

	PlotCompactCirclePixel(pRows, xCenter, yCenter + nRadius, xMin, yMin, xMax, yMax, nColor);
	PlotCompactCirclePixel(pRows, xCenter, yCenter - nRadius, xMin, yMin, xMax, yMax, nColor);
	PlotCompactCirclePixel(pRows, xCenter + nRadius, yCenter, xMin, yMin, xMax, yMax, nColor);
	PlotCompactCirclePixel(pRows, xCenter - nRadius, yCenter, xMin, yMin, xMax, yMax, nColor);
	x = 0;
	y = nRadius;
	nDecision = 0;
	nDelta = 1;
	nThreshold = nRadius * 2 - 1;
	while (x < y) {
		++x;
		nDecision += nDelta;
		nDelta += 2;
		if (nDecision * 2 > nThreshold) {
			--y;
			nDecision -= nThreshold;
			nThreshold -= 2;
		}
		if (x <= y) {
			PlotCompactCirclePixel(pRows, xCenter + x, yCenter + y, xMin, yMin, xMax, yMax, nColor);
			PlotCompactCirclePixel(pRows, xCenter - x, yCenter + y, xMin, yMin, xMax, yMax, nColor);
			PlotCompactCirclePixel(pRows, xCenter + x, yCenter - y, xMin, yMin, xMax, yMax, nColor);
			PlotCompactCirclePixel(pRows, xCenter - x, yCenter - y, xMin, yMin, xMax, yMax, nColor);
			if (x < y) {
				PlotCompactCirclePixel(pRows, xCenter + y, yCenter + x, xMin, yMin, xMax, yMax, nColor);
				PlotCompactCirclePixel(pRows, xCenter - y, yCenter + x, xMin, yMin, xMax, yMax, nColor);
				PlotCompactCirclePixel(pRows, xCenter + y, yCenter - x, xMin, yMin, xMax, yMax, nColor);
				PlotCompactCirclePixel(pRows, xCenter - y, yCenter - x, xMin, yMin, xMax, yMax, nColor);
			}
		}
	}

	DirtyRect[2] = (short) (xCenter - nRadius < xMin ? xMin : xCenter - nRadius);
	DirtyRect[3] = (short) (yCenter - nRadius < yMin ? yMin : yCenter - nRadius);
	DirtyRect[0] = (short) ((xCenter + nRadius + 1 > xMax + 1 ? xMax + 1 : xCenter + nRadius + 1) - DirtyRect[2]);
	DirtyRect[1] = (short) ((yCenter + nRadius + 1 > yMax + 1 ? yMax + 1 : yCenter + nRadius + 1) - DirtyRect[3]);
	if (DirtyRect[0] > 0 && DirtyRect[1] > 0) {
		((DirtyRectProc) (*(void***) pTarget)[1])(pTarget, 0, DirtyRect);
	}
}

void LEMBALL_FASTCALL CompactHelperSubobjectSlot4(void* pObject, int, void* pCommand)
{
	typedef void(LEMBALL_FASTCALL * DirtyRectProc)(void*, int, short*);
	char* pCompact;
	char* pTarget;
	int* pRows;
	short DirtyRect[4];
	int x0;
	int y0;
	int x1;
	int y1;
	int dx;
	int dy;
	int nError;
	int nStepY;
	int i;
	unsigned char nColor;

	pCompact = (char*) pObject - *(int*) ((char*) pObject - 4);
	pTarget = pCompact - 0x55c;
	x0 = *(short*) ((char*) pCommand + 4);
	y0 = *(short*) ((char*) pCommand + 6);
	x1 = *(short*) ((char*) pCommand + 8);
	y1 = *(short*) ((char*) pCommand + 0x0a);
	if (x1 < x0) {
		i = x0;
		x0 = x1;
		x1 = i;
		i = y0;
		y0 = y1;
		y1 = i;
	}
	if (((VsGdiHelperSurface*) pTarget)->ClipLineToActiveRect(&x0, &y0, &x1, &y1) != 0) {
		return;
	}
	DirtyRect[0] = (short) (x1 - x0 + 1);
	DirtyRect[1] = (short) ((y1 < y0 ? y0 - y1 : y1 - y0) + 1);
	DirtyRect[2] = (short) x0;
	DirtyRect[3] = (short) (y0 < y1 ? y0 : y1);

	pRows = *(int**) (pCompact - 0x558);
	nColor = (unsigned char) *(int*) ((char*) pCommand + 0x0c);
	dx = x1 - x0;
	dy = y1 - y0;
	nStepY = 1;
	if (dy < 0) {
		nStepY = -1;
		dy = -dy;
	}
	if (dy < dx) {
		nError = 0;
		for (i = 0; i < dx; ++i) {
			++x0;
			nError += dy * 2;
			*(unsigned char*) (pRows[y0] + x0 - 1) = nColor;
			if (nError > dx) {
				y0 += nStepY;
				nError -= dx * 2;
			}
		}
	}
	else {
		nError = 0;
		for (i = 0; i < dy; ++i) {
			*(unsigned char*) (pRows[y0] + x0) = nColor;
			y0 += nStepY;
			nError += dx * 2;
			if (nError > dy) {
				++x0;
				nError -= dy * 2;
			}
		}
	}
	((DirtyRectProc) (*(void***) pTarget)[1])(pTarget, 0, DirtyRect);
}

// Original body 0x00474fd0 is reached through vtordisp thunk 0x0046db90.
void LEMBALL_FASTCALL CompactHelperSubobjectSlot5(void* pObject, int, void* pCommand)
{
	typedef void(LEMBALL_FASTCALL * DirtyRectProc)(void*, int, short*);
	char* pCompact;
	char* pClipView;
	char* pTarget;
	short DirtyRect[4];
	short nClipX;
	short nClipY;
	short x;
	short y;
	int nAdjustment;

	pCompact = (char*) pObject - *(int*) ((char*) pObject - 4);
	nAdjustment = *(int*) (*(int*) (pCompact - 0x51c) + 4);
	pClipView = pCompact + nAdjustment;
	x = *(short*) ((char*) pCommand + 4);
	y = *(short*) ((char*) pCommand + 6);
	nClipX = *(short*) (pClipView - 0x4f4);
	nClipY = *(short*) (pClipView - 0x4f2);
	if (x < nClipX || x >= (short) (nClipX + *(short*) (pClipView - 0x4f8)) || y < nClipY ||
		y >= (short) (nClipY + *(short*) (pClipView - 0x4f6))) {
		return;
	}

	*(unsigned char*) (*(int*) (*(int*) (pCompact - 0x558) + y * 4) + x) =
		(unsigned char) *(int*) ((char*) pCommand + 8);
	DirtyRect[0] = 1;
	DirtyRect[1] = 1;
	DirtyRect[2] = x;
	DirtyRect[3] = y;
	pTarget = pCompact - 0x55c;
	((DirtyRectProc) (*(void***) pTarget)[1])(pTarget, 0, DirtyRect);
}

void LEMBALL_FASTCALL CompactHelperSubobjectSlot9(void* pObject, int, void* pEntryObject, void* pFrameObject)
{
	typedef unsigned char*(LEMBALL_FASTCALL * GetDataProc)(void*, int);
	typedef void(LEMBALL_FASTCALL * DirtyRectProc)(void*, int, VsGdiRect*);
	unsigned char* pEntry;
	unsigned char* pFrame;
	unsigned char* pSource;
	unsigned char** ppRows;
	VsGdiHelperSurface* pSurface;
	VsGdiRect Rect;
	VsGdiRect ClippedRect;
	short* pRect;
	short* pClipped;
	int xDestination;
	int yDestination;
	int cxFrame;
	int cyFrame;
	int iRow;
	int xSource;

	pEntry = (unsigned char*) pEntryObject;
	pFrame = (unsigned char*) pFrameObject;
	pSurface = (VsGdiHelperSurface*) ((char*) pObject - 0x55c);
	cxFrame = *(short*) (pFrame + 0x4c);
	cyFrame = *(short*) (pFrame + 0x4e);
	if (cxFrame <= 0 || cyFrame <= 0) {
		return;
	}

	xDestination = *(short*) (pEntry + 4);
	yDestination = *(short*) (pEntry + 6);
	if ((*(unsigned int*) (pEntry + 0x0c) & 0x400) == 0) {
		xDestination += *(short*) (pFrame + 0x48);
		yDestination += *(short*) (pFrame + 0x4a);
	}
	if (cxFrame > 0xff) {
		cxFrame = 0xff;
	}
	if (cyFrame > 0xff) {
		cyFrame = 0xff;
	}

	pRect = (short*) &Rect;
	pRect[0] = (short) cxFrame;
	pRect[1] = (short) cyFrame;
	pRect[2] = (short) xDestination;
	pRect[3] = (short) yDestination;
	memset(&ClippedRect, 0, sizeof(ClippedRect));
	pClipped = (short*) &ClippedRect;
	if (pSurface->ClipRenderRectToSurfaceBounds(&Rect, &ClippedRect) && (pClipped[0] < 1 || pClipped[1] < 1)) {
		return;
	}
	((DirtyRectProc) (*(void***) pSurface)[1])(pSurface, 0, &Rect);

	pSource = ((GetDataProc) (*(void***) pFrame)[10])(pFrame, 0);
	ppRows = *(unsigned char***) ((char*) pSurface + 4);
	for (iRow = 0; iRow < cyFrame; ++iRow) {
		unsigned char* pDestination;
		unsigned char nCommand;
		int xOutput;
		int yOutput;

		xSource = 0;
		yOutput = yDestination + iRow;
		pDestination = yOutput >= Rect.m_cy && yOutput < Rect.m_cy + Rect.m_y ? ppRows[yOutput] : 0;
		do {
			nCommand = *pSource++;
			if (nCommand < 0x80) {
				xSource += nCommand;
			}
			else if (nCommand > 0x80) {
				int cbLiteral;
				int i;

				cbLiteral = nCommand & 0x7f;
				for (i = 0; i < cbLiteral; ++i) {
					xOutput = xDestination + xSource + i;
					if (pDestination != 0 && xOutput >= Rect.m_cx && xOutput < Rect.m_cx + Rect.m_x) {
						pDestination[xOutput] = pSource[i];
					}
				}
				pSource += cbLiteral;
				xSource += cbLiteral;
			}
		} while (nCommand != 0x80);
	}
}

__inline static void ApplyPointDeltaToLinkedHelper(void* pChild, const short* pDelta)
{
	typedef void(LEMBALL_FASTCALL * SetPointProc)(void*, int, short*);
	char* pTarget;
	char* pCompact;
	short* pPoint;
	int nCompactDelta;

	pTarget = (char*) pChild;
	nCompactDelta = *(int*) (*(int*) (pTarget + 0x40) + 4);
	pPoint = (short*) (pTarget + nCompactDelta + 0x50);
	pPoint[0] += pDelta[0];
	pPoint[1] += pDelta[1];
	pCompact = pTarget + 0x40 + nCompactDelta;
	((SetPointProc) (*(void***) pCompact)[11])(pCompact, 0, pPoint);
}

void LEMBALL_FASTCALL CompactHelperSubobjectSlot11(void* pObject, int, short* pPoint)
{
	typedef void(LEMBALL_FASTCALL * SetExtentProc)(void*, int, short*);
	char* pCompact;
	char* pView;
	char* pLinkedTarget;
	short* pRequestedRect;
	short* pWorkingRect;
	short* pBackingRect;
	short* pLinkedExtent;
	short Delta[2];
	short nOldWidth;
	short nOldHeight;
	CompactHelperLinkNode* pNode;
	int nCompactDelta;
	int nLinkedDelta;
	void* pBackingBitmap;

	pCompact = (char*) pObject - *(int*) ((char*) pObject - 4);
	nCompactDelta = *(int*) (*(int*) (pCompact - 0x51c) + 4);
	pView = pCompact + nCompactDelta;
	pRequestedRect = (short*) (pView - 0x510);
	Delta[0] = pPoint[0] - pRequestedRect[2];
	Delta[1] = pPoint[1] - pRequestedRect[3];
	pLinkedTarget = *(char**) (pView - 0x4fc);
	if (pLinkedTarget == (char*) g_pResourceGeometryHelperTarget) {
		return;
	}

	EnterCriticalSection(pCompact - 0x28);
	pRequestedRect[2] = pPoint[0];
	pRequestedRect[3] = pPoint[1];
	pWorkingRect = (short*) (pView - 0x508);
	nOldWidth = pWorkingRect[0];
	nOldHeight = pWorkingRect[1];
	pWorkingRect[0] = pRequestedRect[0];
	pWorkingRect[1] = pRequestedRect[1];
	pWorkingRect[2] = pRequestedRect[2];
	pWorkingRect[3] = pRequestedRect[3];

	nLinkedDelta = *(int*) (*(int*) (pLinkedTarget + 0x40) + 4);
	pLinkedExtent = (short*) (pLinkedTarget + nLinkedDelta + 0x54);
	if (pWorkingRect[2] < 0) {
		pWorkingRect[0] += pWorkingRect[2];
		pWorkingRect[2] = 0;
	}
	if (pWorkingRect[0] + pWorkingRect[2] > pLinkedExtent[0]) {
		pWorkingRect[0] = pLinkedExtent[0] - pWorkingRect[2];
	}
	if (pWorkingRect[3] < 0) {
		pWorkingRect[1] += pWorkingRect[3];
		pWorkingRect[3] = 0;
	}
	if (pWorkingRect[1] + pWorkingRect[3] > pLinkedExtent[1]) {
		pWorkingRect[1] = pLinkedExtent[1] - pWorkingRect[3];
	}
	if (pWorkingRect[0] < 1 || pWorkingRect[1] < 1) {
		pWorkingRect[0] = 0;
		pWorkingRect[1] = 0;
		pWorkingRect[2] = 0;
		pWorkingRect[3] = 0;
	}
	pBackingRect = (short*) (pView - 0x4f8);
	pBackingRect[0] = pWorkingRect[0];
	pBackingRect[1] = pWorkingRect[1];
	if (pWorkingRect[0] != nOldWidth || pWorkingRect[1] != nOldHeight) {
		((SetExtentProc) (*(void***) pCompact)[10])(pCompact, 0, pWorkingRect);
	}

	pBackingBitmap = *(void**) (pCompact - 0x46c);
	if (pBackingBitmap != 0) {
		((VsGdiDisplayStateDispatch*) g_pDisplayState)->ReleaseDisplayBitmap(pBackingBitmap);
		*(void**) (pCompact - 0x46c) = 0;
	}
	*(int*) (pCompact - 0x38) = 0;
	EnsureHelperBackingRowIndexCapacity((VsGdiHelperSurface*) (pCompact - 0x55c));
	LeaveCriticalSection(pCompact - 0x28);

	pNode = *(CompactHelperLinkNode**) (pCompact - 0x34);
	while (pNode != 0) {
		ApplyPointDeltaToLinkedHelper(pNode->m_pTarget, Delta);
		pNode = pNode->m_pPrev;
	}
}

// FUNCTION: LEMBALL 0x004668d0
void CompactHelperGroup0DirtyStateView::SetDirtyFlagAndPropagate(int fDirty)
{
	char* pGroup;
	int* pOffsetEntry;
	int* pfCurrentDirty;
	CompactHelperPoint* pSourcePoint;
	CompactHelperPoint Point;
	CompactHelperDirtyPointSink* pSink;

	pGroup = (char*) this;
	pfCurrentDirty = (int*) (pGroup + (*(int**) (pGroup - sizeof(void*)))[1] + 0x3c);
	if (*pfCurrentDirty != fDirty) {
		*pfCurrentDirty = fDirty;
		pOffsetEntry = *(int**) (pGroup - sizeof(void*)) + 1;
		pSourcePoint = (CompactHelperPoint*) (pGroup + *pOffsetEntry + 8);
		Point.m_nX = pSourcePoint->m_nX;
		Point.m_nY = pSourcePoint->m_nY;
		pSink = (CompactHelperDirtyPointSink*) (pGroup + *pOffsetEntry - sizeof(void*));
		pSink->PropagateDirtyPoint(&Point);
	}
}

// Compiler-state shim retained for manual table slot 17. Original 0x0046dcb0 is a
// generated -0x518 adjustor to CompactHelperGroup0DirtyStateView::SetDirtyFlagAndPropagate.
void LEMBALL_FASTCALL CompactHelperSubobjectSlot17(void*)
{
}

int LEMBALL_FASTCALL get_helper_group_0_dirty_flag_local(void* pHelperGroup);

// FUNCTION: LEMBALL 0x0046dcc0
int LEMBALL_FASTCALL CompactHelperSubobjectSlot18(void* pSubobject)
{
	return get_helper_group_0_dirty_flag_local((char*) pSubobject - 0x518);
}

class ListCharEntryIndexResolver {
public:
	virtual void Slot0(void) = 0;
	virtual void Slot1(void) = 0;
	virtual char ResolveEntryIndex(void* pEntry) = 0;
};

struct ListCharEntrySourceHeader {
	void* m_pReserved00;
	unsigned int m_cbEntry;
};

struct ListCharEntrySourceView {
	unsigned char m_abReserved00[0x48];
	ListCharEntrySourceHeader* m_pHeader;
	unsigned char m_abReserved4c[0x20];
	unsigned int m_cbEntries;
	unsigned char m_abReserved70[8];
	ListCharEntryIndexResolver* m_pIndexResolver;
	char* m_pEntries;
	char* m_pMetadata;
};

class ListCharEntryTable {
public:
	void* m_pVtable;
	void** m_apEntries;

	ListCharEntryTable* Construct(ListCharEntrySourceView* pSource);
};

// FUNCTION: LEMBALL 0x00473650
ListCharEntryTable* ListCharEntryTable::Construct(ListCharEntrySourceView* pSource)
{
	unsigned int cbOffset;
	unsigned int i;
	int cbEntryOffset;
	int cbMetadataOffset;
	int iEntry;

	m_pVtable = g_GDISUBVT_ListCharEntryTableVtable;
	m_apEntries = (void**) AllocateVSMemBlock(0x400);
	cbOffset = 0;
	do {
		cbOffset += sizeof(void*);
		*(void**) ((char*) m_apEntries + cbOffset - sizeof(void*)) = 0;
	} while (cbOffset < 0x400);

	cbEntryOffset = 0;
	i = 0;
	if (pSource->m_cbEntries / pSource->m_pHeader->m_cbEntry != 0) {
		cbMetadataOffset = 0;
		do {
			char* pEntry;

			if (pSource->m_pMetadata == 0) {
				iEntry = pSource->m_pIndexResolver->ResolveEntryIndex(pSource->m_pEntries + cbEntryOffset);
			}
			else {
				iEntry = *(int*) (pSource->m_pMetadata + cbMetadataOffset + 0x48);
			}
			pEntry = pSource->m_pEntries + cbEntryOffset;
			cbMetadataOffset += 0x4c;
			cbEntryOffset += 0x54;
			m_apEntries[iEntry] = pEntry;
			++i;
		} while (i < pSource->m_cbEntries / pSource->m_pHeader->m_cbEntry);
	}
	return this;
}

// FUNCTION: LEMBALL 0x00473700
void LEMBALL_FASTCALL ResourceGeometryRowBufferSlot5(void*)
{
}

// FUNCTION: LEMBALL 0x00473710
void LEMBALL_FASTCALL ResourceGeometryRowBufferSlot6(void*)
{
}

// FUNCTION: LEMBALL 0x00473730
void LEMBALL_FASTCALL DestroyListCharEntryTable(void* pObject)
{
	*(void**) pObject = g_GDISUBVT_ListCharEntryTableVtable;
	FreeVSMemBlock(*(void**) ((char*) pObject + 4));
	*(void**) pObject = g_GDISUBVT_ListCharEntryTableDeletingVtable;
}

// FUNCTION: LEMBALL 0x00473750
void* LEMBALL_FASTCALL ResourceGeometryRowBufferSlot4(void* pObject, int, BYTE fDelete)
{
	DestroyListCharEntryTable(pObject);
	if ((fDelete & 1) != 0) {
		FreeResourceArchiveMemory(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x00473770
void* LEMBALL_FASTCALL ResourceGeometryRowBufferSlot8(void* pObject, int, BYTE fDelete)
{
	*(void**) pObject = g_GDISUBVT_ListCharEntryTableDeletingVtable;
	if ((fDelete & 1) != 0) {
		FreeResourceArchiveMemory(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x00474c20
void LEMBALL_FASTCALL CompactHelperSubobjectSlot0(void* pObject, int, short* pEntry)
{
	typedef int(LEMBALL_FASTCALL * QueryProc)(void*, int);
	typedef void(LEMBALL_FASTCALL * DirtyRectProc)(void*, int, short*);
	char* pCompact;
	char* pPrimary;
	char* pTarget;
	short DestinationPoint[2];
	short DestinationRect[4];
	short ScaledPoint[2];
	short ScaledRect[4];

	pCompact = (char*) pObject;
	pTarget = pCompact - 0x55c;
	DestinationPoint[0] = pEntry[2];
	DestinationPoint[1] = pEntry[3];
	DestinationRect[0] = pEntry[4];
	DestinationRect[1] = pEntry[5];
	DestinationRect[2] = pEntry[6];
	DestinationRect[3] = pEntry[7];
	pPrimary = pTarget + *(int*) (*(int*) (pTarget + 0x40) + 4);

	if (((QueryProc) (*(void***) pPrimary)[15])(pPrimary, 0) != 0) {
		((VsGdiHelperSurface*) (pTarget + 0xa0))->CopyOrScrollRows(DestinationRect, DestinationPoint);
	}
	if (((QueryProc) (*(void***) pPrimary)[16])(pPrimary, 0) != 0) {
		ScaledRect[0] = DestinationRect[0] * 2;
		ScaledRect[1] = DestinationRect[1];
		ScaledRect[2] = DestinationRect[2] * 2;
		ScaledRect[3] = DestinationRect[3];
		ScaledPoint[0] = DestinationPoint[0] * 2;
		ScaledPoint[1] = DestinationPoint[1];
		((VsGdiHelperSurface*) (pTarget + 0x4c))->CopyOrScrollRows(ScaledRect, ScaledPoint);
	}
	((VsGdiHelperSurface*) pTarget)->CopyOrScrollRows(DestinationRect, DestinationPoint);
	((DirtyRectProc) (*(void***) pTarget)[1])(pTarget, 0, DestinationRect);
}
