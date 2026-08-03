#include "AI/CMover.h"

extern void* g_pActiveManagedEntityOwner;
extern void* g_pLevelTileGrid;
extern void __fastcall ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void __fastcall ResetMoveChunkObjectRuntimeStateThunk(void* pObject);
extern void __fastcall DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void* g_LEVELVT_MoveChunkObjectVtable[16];
extern int Distance2DIntPixels(int x1, int y1, int x2, int y2);
extern void* __fastcall get_managed_entity_owner_group(void* pEntity);
extern void __fastcall clear_managed_entity_child_pending_state_if_interruptible(void* pEntity);

struct LevelNodePoint {
	int m_anValues[3];
};

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nVariant, unsigned short nStateId);
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

// FUNCTION: LEMBALL 0x0042e590
CMover::CMover(void)
{
	char* pObject = (char*) this;
	((LevelChunkObjectBaseView*) this)->InitializeLevelChunkObjectBase(0x34, 0, 0);
	*(int*) (pObject + 0x150) = 0;
	*(int*) (pObject + 0x154) = 0;
	*(int*) (pObject + 0x158) = 0;
	*(int*) (pObject + 0x15c) = 0xaa55aa55;
	*(int*) (pObject + 0x160) = 0xaa55aa55;
	*(int*) (pObject + 0x164) = 0xaa55aa55;
	*(void**) this = g_LEVELVT_MoveChunkObjectVtable;
}

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

// FUNCTION: LEMBALL 0x0042e640
CMover::~CMover(void)
{
	*(void**) this = g_LEVELVT_MoveChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(this);
}

// FUNCTION: LEMBALL 0x0042e650
void CMover::SetPos(void)
{
	MoveChunkTileGridView* pGrid = (MoveChunkTileGridView*) g_pLevelTileGrid;
	int nMinX = ((m_nWorldX9C >> 12) - 8) / 16;
	int nMaxX = ((m_nWorldX9C >> 12) + 7) / 16;
	int nMinY = ((m_nWorldYA0 >> 12) - 8) / 16;
	int nMaxY = ((m_nWorldYA0 >> 12) + 7) / 16;
	int y;

	for (y = nMinY; y <= nMaxY; ++y) {
		int x;
		for (x = nMinX; x <= nMaxX; ++x) {
			if (x >= 0 && y >= 0 && x < pGrid->m_cColumns10 && y < pGrid->m_cRows14) {
				pGrid->m_pCells0C[y * pGrid->m_cColumns10 + x].m_nFlags06 |= 0x10;
			}
		}
	}
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

// FUNCTION: LEMBALL 0x0042e850
void CMover::SetUpNextNode(unsigned long nTime)
{
	LevelNodePoint point;
	CPt3 start;
	CPt3 end;
	int iNextNode = m_nState170 + 1;
	MoveChunkTileGridView* pGrid = (MoveChunkTileGridView*) g_pLevelTileGrid;
	short nHeight = 0;

	if (iNextNode >= m_nValue16C) {
		iNextNode = 0;
	}
	((LevelModeNodeView*) g_pActiveManagedEntityOwner)->CopyNodePointByIndex(&point, m_iNode168 + iNextNode);
	if ((point.m_anValues[0] >> 12) >= 0 && (point.m_anValues[1] >> 12) >= 0 &&
		(point.m_anValues[0] >> 16) < pGrid->m_cColumns10 && (point.m_anValues[1] >> 16) < pGrid->m_cRows14) {
		CGround* pGround =
			(CGround*) &pGrid
				->m_pCells0C[(point.m_anValues[1] >> 16) * pGrid->m_cColumns10 + (point.m_anValues[0] >> 16)];
		nHeight = pGround->GetZThunk((point.m_anValues[0] >> 12) & 15, (point.m_anValues[1] >> 12) & 15);
	}
	start.x = m_nWorldX9C >> 12;
	start.y = m_nWorldYA0 >> 12;
	start.z = m_nWorldZA4 >> 12;
	end.x = point.m_anValues[0] >> 12;
	end.y = point.m_anValues[1] >> 12;
	end.z = ((unsigned short) nHeight << 12) >> 12;
	*(int*) ((char*) this + 0xc8) = nTime;
	*(int*) ((char*) this + 0xcc) = nTime + Distance2DIntPixels(start.x, start.y, end.x, end.y);
	m_Move14C.Set(start, end, nTime, 1);
}

// FUNCTION: LEMBALL 0x0042ea40
void CMover::MoveObjects(int nDeltaX, int nDeltaY, int nDeltaZ)
{
	int i;
	for (i = 0; i < m_cAttachedEntities174; ++i) {
		char* pEntity = (char*) m_apAttachedEntities178[i];
		*(int*) (pEntity + 0x9c) += nDeltaX << 12;
		*(int*) (pEntity + 0xa0) += nDeltaY << 12;
		*(int*) (pEntity + 0xa4) += nDeltaZ << 12;
	}
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

// FUNCTION: LEMBALL 0x0042ef40
void CMover::VerifyObjects(void)
{
	int nMinX = (m_nWorldX9C >> 12) - 8;
	int nMinY = (m_nWorldYA0 >> 12) - 8;
	int i = 0;

	while (i < m_cAttachedEntities174) {
		char* pEntity = (char*) m_apAttachedEntities178[i];
		int nX = *(int*) (pEntity + 0x9c) >> 12;
		int nY = *(int*) (pEntity + 0xa0) >> 12;
		if (nX < nMinX || nMinX + 15 < nX || nY < nMinY || nMinY + 15 < nY) {
			int j;
			*(int*) (pEntity + 0x11c) = 0;
			for (j = i + 1; j < m_cAttachedEntities174; ++j) {
				m_apAttachedEntities178[j - 1] = m_apAttachedEntities178[j];
			}
			--i;
			--m_cAttachedEntities174;
		}
		++i;
	}
}

// FUNCTION: LEMBALL 0x0042f140
void CMover::StopObjectsMoving(void)
{
	typedef void(__fastcall * OwnerResetSlotProc)(void*);
	int i = 0;
	if (m_cAttachedEntities174 > 0) {
		void** ppEntity = m_apAttachedEntities178;
		do {
			void* pEntity = *ppEntity;
			if (*(int*) ((char*) pEntity + 0x64) == 2) {
				void* pOwnerGroup = get_managed_entity_owner_group(pEntity);
				((OwnerResetSlotProc) (*(void***) pOwnerGroup)[84])(pOwnerGroup);
			}
			else {
				clear_managed_entity_child_pending_state_if_interruptible(pEntity);
			}
			++ppEntity;
			++i;
		} while (i < m_cAttachedEntities174);
	}
}
