#include "AI/CMover.h"

#include "AI/CGameObject.h"
#include "AI/CPlayerLemming.h"
#include "AI/LevelNodePoint.h"
#include "AI/LevelTileGridCell.h"

#include <string.h>

extern void* g_pActiveManagedEntityOwner;
extern void* g_pLevelTileGrid;
extern void __fastcall ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void __fastcall ResetMoveChunkObjectRuntimeStateThunk(void* pObject);
extern void __fastcall DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void* g_LEVELVT_MoveChunkObjectVtable[16];
extern int Distance2DIntPixels(int x1, int y1, int x2, int y2);
extern unsigned short g_GAME_ManagedEntityRegistryCount;
extern int g_GAME_ManagedEntityRegistryTable[1000];




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
	m_Move14C.m_Start.x = 0;
	m_Move14C.m_Start.y = 0;
	m_Move14C.m_Start.z = 0;
	m_Move14C.m_Velocity.x = (int) 0xaa55aa55;
	m_Move14C.m_Velocity.y = (int) 0xaa55aa55;
	m_Move14C.m_Velocity.z = (int) 0xaa55aa55;
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
	*(int*) ((char*) this + 0x94) = 0;
	m_fActive138 = 0;
	*(int*) ((char*) this + 0x13c) = 0;
	m_nMode144 = 0;
	m_fSwitch148 = 0;
	m_cAttachedEntities174 = 0;
	*(int*) ((char*) this + 0x140) = 1;
	*(int*) ((char*) this + 0xb8) = 0x18;
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

	((CGameObject*) this)->SetId(nSlotId);
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
	m_nNodeStartTickC8 = nTime;
	m_nNodeEndTickCC = nTime + Distance2DIntPixels(start.x, start.y, end.x, end.y);
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
	m_fSwitch148 = 1;
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
				void* pOwnerGroup = ((CPlayerLemming*) pEntity)->GetGroup();
				((OwnerResetSlotProc) (*(void***) pOwnerGroup)[84])(pOwnerGroup);
			}
			else {
				((CGameObject*) pEntity)->ResetInstructions();
			}
			++ppEntity;
			++i;
		} while (i < m_cAttachedEntities174);
	}
}

// FUNCTION: LEMBALL 0x0042e980
void CMover::FindObjectsOnTopOfMe(void)
{
	int minX;
	int maxX;
	int minY;
	int maxY;
	unsigned int count;
	unsigned int i;
	void* pObj;

	minX = (m_nWorldX9C >> 0xc) - 8;
	maxX = minX + 0xf;
	minY = (m_nWorldYA0 >> 0xc) - 8;
	maxY = minY + 0xf;
	count = g_GAME_ManagedEntityRegistryCount;
	for (i = 0; (int) i < (int) count; i++) {
		pObj = (void*) g_GAME_ManagedEntityRegistryTable[i];
		if (pObj != (void*) 0x0) {
			if ((short) ((short(__fastcall*) (void*)) 0x401794)(pObj) != (short) -1) {
				if ((short) ((short(__fastcall*) (void*)) 0x401794)(this) != (short) ((short(__fastcall*) (void*)) 0x401794)(pObj)) {
					if (*(int*) ((char*) pObj + 0x64) != 7) {
						int ex = *(int*) ((char*) pObj + 0x9c) >> 0xc;
						int ey = *(int*) ((char*) pObj + 0xa0) >> 0xc;
						if (minX <= ex && ex <= maxX && minY <= ey && ey <= maxY) {
							((void(__fastcall*) (void*, void*)) 0x4036b1)(this, pObj);
						}
					}
				}
			}
		}
	}
}

// FUNCTION: LEMBALL 0x0042eff0
int CMover::GetOn(CGameObject* pObject)
{
	AICOORD position;
	AICOORD destination;
	int nMoverZ;
	int nObjectZ;
	int i;

	position = pObject->m_WorldPosition9C;
	nMoverZ = m_nWorldZA4 >> 12;
	nObjectZ = position.z >> 12;
	if (nObjectZ < nMoverZ - 0x10 || nMoverZ + 0x10 < nObjectZ) {
		return 0;
	}
	if (!IsOn(position)) {
		return 0;
	}
	if (m_cAttachedEntities174 >= 10) {
		return 0;
	}
	for (i = 0; i < m_cAttachedEntities174; ++i) {
		if (m_apAttachedEntities178[i] == pObject) {
			return 1;
		}
	}
	m_apAttachedEntities178[m_cAttachedEntities174] = pObject;
	pObject->m_fOnMover11C = 1;
	++m_cAttachedEntities174;
	VerifyObjects();
	if (m_nFieldB8 != 2 && pObject->m_nEntityType64 == 2) {
		destination.x = m_nWorldX9C;
		destination.y = m_nWorldYA0;
		destination.z = position.z;
		pObject->AddDestination(destination);
		pObject->StartMoving();
		return 1;
	}
	pObject->m_WorldPosition9C.x = position.x;
	pObject->m_WorldPosition9C.y = position.y;
	pObject->m_WorldPosition9C.z = m_nWorldZA4 + 0x8000;
	return 1;
}

// FUNCTION: LEMBALL 0x0042FC60
CMover& CMover::CopyPrefix(const CMover& source)
{
	char* pDest;
	const char* pSource;

	pDest = (char*) this;
	pSource = (const char*) &source;
	*(unsigned short*) (pDest + 0x4) = *(const unsigned short*) (pSource + 0x4);
	*(int*) (pDest + 0x8) = *(const int*) (pSource + 0x8);
	*(int*) (pDest + 0xc) = *(const int*) (pSource + 0xc);
	*(int*) (pDest + 0x10) = *(const int*) (pSource + 0x10);
	memcpy(pDest + 0x14, pSource + 0x14, 0x18);
	*(int*) (pDest + 0x2c) = *(const int*) (pSource + 0x2c);
	*(int*) (pDest + 0x30) = *(const int*) (pSource + 0x30);
	*(int*) (pDest + 0x34) = *(const int*) (pSource + 0x34);
	*(int*) (pDest + 0x38) = *(const int*) (pSource + 0x38);
	*(int*) (pDest + 0x3c) = *(const int*) (pSource + 0x3c);
	*(int*) (pDest + 0x40) = *(const int*) (pSource + 0x40);
	*(int*) (pDest + 0x44) = *(const int*) (pSource + 0x44);
	*(int*) (pDest + 0x48) = *(const int*) (pSource + 0x48);
	*(int*) (pDest + 0x4c) = *(const int*) (pSource + 0x4c);
	*(int*) (pDest + 0x50) = *(const int*) (pSource + 0x50);
	*(int*) (pDest + 0x54) = *(const int*) (pSource + 0x54);
	*(int*) (pDest + 0x58) = *(const int*) (pSource + 0x58);
	*(int*) (pDest + 0x5c) = *(const int*) (pSource + 0x5c);
	*(int*) (pDest + 0x60) = *(const int*) (pSource + 0x60);
	*(int*) (pDest + 0x64) = *(const int*) (pSource + 0x64);
	*(unsigned short*) (pDest + 0x68) = *(const unsigned short*) (pSource + 0x68);
	*(unsigned short*) (pDest + 0x6a) = *(const unsigned short*) (pSource + 0x6a);
	*(unsigned short*) (pDest + 0x6c) = *(const unsigned short*) (pSource + 0x6c);
	*(unsigned short*) (pDest + 0x6e) = *(const unsigned short*) (pSource + 0x6e);
	*(int*) (pDest + 0x70) = *(const int*) (pSource + 0x70);
	*(int*) (pDest + 0x74) = *(const int*) (pSource + 0x74);
	*(int*) (pDest + 0x78) = *(const int*) (pSource + 0x78);
	*(int*) (pDest + 0x7c) = *(const int*) (pSource + 0x7c);
	*(int*) (pDest + 0x80) = *(const int*) (pSource + 0x80);
	*(int*) (pDest + 0x84) = *(const int*) (pSource + 0x84);
	*(int*) (pDest + 0x88) = *(const int*) (pSource + 0x88);
	*(int*) (pDest + 0x8c) = *(const int*) (pSource + 0x8c);
	*(int*) (pDest + 0x90) = *(const int*) (pSource + 0x90);
	*(int*) (pDest + 0x94) = *(const int*) (pSource + 0x94);
	*(int*) (pDest + 0x98) = *(const int*) (pSource + 0x98);
	*(int*) (pDest + 0x9c) = *(const int*) (pSource + 0x9c);
	*(int*) (pDest + 0xa0) = *(const int*) (pSource + 0xa0);
	*(int*) (pDest + 0xa4) = *(const int*) (pSource + 0xa4);
	*(int*) (pDest + 0xa8) = *(const int*) (pSource + 0xa8);
	*(int*) (pDest + 0xac) = *(const int*) (pSource + 0xac);
	*(int*) (pDest + 0xb0) = *(const int*) (pSource + 0xb0);
	*(unsigned short*) (pDest + 0xb4) = *(const unsigned short*) (pSource + 0xb4);
	*(unsigned short*) (pDest + 0xb6) = *(const unsigned short*) (pSource + 0xb6);
	*(int*) (pDest + 0xb8) = *(const int*) (pSource + 0xb8);
	*(unsigned short*) (pDest + 0xbc) = *(const unsigned short*) (pSource + 0xbc);
	*(int*) (pDest + 0xc0) = *(const int*) (pSource + 0xc0);
	*(unsigned short*) (pDest + 0xc4) = *(const unsigned short*) (pSource + 0xc4);
	*(int*) (pDest + 0xc8) = *(const int*) (pSource + 0xc8);
	*(int*) (pDest + 0xcc) = *(const int*) (pSource + 0xcc);
	*(int*) (pDest + 0xd0) = *(const int*) (pSource + 0xd0);
	*(int*) (pDest + 0xd4) = *(const int*) (pSource + 0xd4);
	*(int*) (pDest + 0xd8) = *(const int*) (pSource + 0xd8);
	*(int*) (pDest + 0xdc) = *(const int*) (pSource + 0xdc);
	*(int*) (pDest + 0xe0) = *(const int*) (pSource + 0xe0);
	*(int*) (pDest + 0xe4) = *(const int*) (pSource + 0xe4);
	*(int*) (pDest + 0xe8) = *(const int*) (pSource + 0xe8);
	*(int*) (pDest + 0xec) = *(const int*) (pSource + 0xec);
	*(int*) (pDest + 0xf0) = *(const int*) (pSource + 0xf0);
	*(int*) (pDest + 0xf4) = *(const int*) (pSource + 0xf4);
	*(int*) (pDest + 0xf8) = *(const int*) (pSource + 0xf8);
	*(int*) (pDest + 0xfc) = *(const int*) (pSource + 0xfc);
	*(int*) (pDest + 0x100) = *(const int*) (pSource + 0x100);
	*(int*) (pDest + 0x104) = *(const int*) (pSource + 0x104);
	*(int*) (pDest + 0x108) = *(const int*) (pSource + 0x108);
	*(int*) (pDest + 0x10c) = *(const int*) (pSource + 0x10c);
	*(int*) (pDest + 0x110) = *(const int*) (pSource + 0x110);
	*(int*) (pDest + 0x114) = *(const int*) (pSource + 0x114);
	*(int*) (pDest + 0x118) = *(const int*) (pSource + 0x118);
	*(int*) (pDest + 0x11c) = *(const int*) (pSource + 0x11c);
	*(unsigned short*) (pDest + 0x120) = *(const unsigned short*) (pSource + 0x120);
	return *this;
}
