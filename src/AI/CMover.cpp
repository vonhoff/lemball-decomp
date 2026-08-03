#include "AI/CMover.h"

extern void* g_pActiveManagedEntityOwner;
extern void* g_pLevelTileGrid;
extern void __fastcall ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void __fastcall ResetMoveChunkObjectRuntimeStateThunk(void* pObject);

struct LevelNodePoint {
	int m_anValues[3];
};

struct LevelModeNodeView {
	char m_abReserved00[0x17c];
	void* m_pNodeManager17C;

	LevelNodePoint* CopyNodePointByIndex(LevelNodePoint* pOutput, int nIndex);
};

struct CGround {
	int m_nShapeType;
	int m_nReserved04;
	short m_nHeight;

	short GetZThunk(int nLocalX, int nLocalY);
};

struct LevelTileGridCell {
	int m_nType00;
	short m_nVariant04;
	unsigned short m_nFlags06;
	short m_nReserved08;
	short m_nReserved0A;
};

struct MoveChunkTileGridView {
	char m_abReserved00[0xc];
	LevelTileGridCell* m_pCells0C;
	int m_cColumns10;
	int m_cRows14;
};

// FUNCTION: LEMBALL 0x0042e5e0
void CMover::Restart(void)
{
	ResetManagedEntityRuntimeStateThunk(this);
	ResetMoveChunkObjectRuntimeStateThunk(this);
}

// FUNCTION: LEMBALL 0x0042e600
void CMover::Initialise(void)
{
	char* pObject = (char*) this;

	*(int*) (pObject + 0x94) = 0;
	*(int*) (pObject + 0x138) = 0;
	*(int*) (pObject + 0x13c) = 0;
	*(int*) (pObject + 0x144) = 0;
	*(int*) (pObject + 0x148) = 0;
	*(int*) (pObject + 0x174) = 0;
	*(int*) (pObject + 0x140) = 1;
	*(int*) (pObject + 0xb8) = 0x18;
}

// FUNCTION: LEMBALL 0x0042e700
int CMover::IsAt(int x, int y, int* pHeight)
{
	int nLeft;
	int nRight;
	int nTop;
	int nBottom;

	nLeft = (m_nWorldX9C >> 12) - 8;
	nRight = nLeft + 15;
	nTop = (m_nWorldYA0 >> 12) - 8;
	nBottom = nTop + 15;
	if (nLeft <= x && x <= nRight && nTop <= y && y <= nBottom) {
		*pHeight = (m_nWorldZA4 >> 12) + 8;
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0042e760
void CMover::Set(unsigned short nSlotId, short nHeading, int nMode, int iNode, int nValue)
{
	LevelNodePoint point;
	LevelNodePoint* pPoint;
	MoveChunkTileGridView* pGrid;
	CGround* pCell;
	int nPixelY;
	int nPixelX;
	int nTileY;
	int nTileX;
	unsigned short nHeight;

	SetManagedEntitySlotId(nSlotId);
	pPoint = ((LevelModeNodeView*) g_pActiveManagedEntityOwner)->CopyNodePointByIndex(&point, iNode);
	m_nWorldX9C = pPoint->m_anValues[0];
	nPixelX = m_nWorldX9C >> 12;
	m_nWorldYA0 = pPoint->m_anValues[1];
	nPixelY = m_nWorldYA0 >> 12;
	m_nWorldZA4 = pPoint->m_anValues[2];
	nTileX = nPixelX >> 4;
	nTileY = nPixelY >> 4;
	pGrid = (MoveChunkTileGridView*) g_pLevelTileGrid;
	if (nPixelX < 0 || nPixelY < 0 || nTileX >= pGrid->m_cColumns10 || nTileY >= pGrid->m_cRows14) {
		nHeight = 0;
	}
	else {
		pCell = (CGround*) &pGrid->m_pCells0C[nTileY * pGrid->m_cColumns10 + nTileX];
		nHeight = (unsigned short) pCell->GetZThunk(nPixelX & 0x0f, nPixelY & 0x0f);
	}

	m_fActive138 = 1;
	m_nHeadingBC = nHeading;
	m_nWorldZA4 = (int) nHeight << 12;
	m_iNode168 = iNode;
	m_nState170 = 0;
	m_cAttachedEntities174 = 0;
	m_nMode144 = nMode;
	m_nValue16C = nValue;
}

// FUNCTION: LEMBALL 0x0042eeb0
void CMover::Switch(void)
{
	*(int*) ((char*) this + 0x148) = 1;
}

// FUNCTION: LEMBALL 0x0042eee0
int CMover::IsOn(AICOORD& point)
{
	int nMinX = (m_nWorldX9C >> 12) - 8;
	int nMinY = (m_nWorldYA0 >> 12) - 8;
	int nX = point.x >> 12;
	int nY = point.y >> 12;
	return nMinX <= nX && nX <= nMinX + 15 && nMinY <= nY && nY <= nMinY + 15;
}
