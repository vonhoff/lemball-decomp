#include "AI/CGameObject.h"
#include "AI/CLift.h"

extern void* g_pLevelTileGrid;

struct CGround {
	short GetZ(int nLocalX, int nLocalY);
};

struct CGameObjectTileGrid {
	char m_abReserved00[0xc];
	CGround* m_pGround0C;
	int m_nWidth10;
	int m_nHeight14;
};

struct CGameObjectLiftVtableView {};

typedef void (CGameObjectLiftVtableView::*CGameObjectOffLiftOneProc)(tCoord3d& point);
typedef void (CGameObjectLiftVtableView::*CGameObjectOffLiftTwoProc)(tCoord3d& start, tCoord3d& end);

struct CGameObjectLiftVtable {
	void* m_apReserved00[0x28];
	CGameObjectOffLiftTwoProc m_pOffLiftTwoA0;
	CGameObjectOffLiftOneProc m_pOffLiftOneA4;
};

// FUNCTION: LEMBALL 0x00416340
int CGameObject::OnLift(tCoord3d& point)
{
	CGameObjectTileGrid* pGrid;
	short height;
	int minX;
	int minY;
	int x;
	int y;

	if (m_nStateB8 == 8) {
		return 0;
	}
	x = m_WorldPosition9C.x >> 12;
	y = m_WorldPosition9C.y >> 12;
	minX = point.x - 8;
	minY = point.y - 8;
	if (minX <= x && x <= point.x + 7 && minY <= y && y <= point.y + 7) {
		pGrid = (CGameObjectTileGrid*) g_pLevelTileGrid;
		if (minX < 0 || minY < 0 || pGrid->m_nWidth10 <= minX >> 4 || pGrid->m_nHeight14 <= minY >> 4) {
			height = 0;
		}
		else {
			height = pGrid->m_pGround0C[(minY >> 4) * pGrid->m_nWidth10 + (minX >> 4)].GetZ(minX & 0xf, minY & 0xf);
		}
		m_WorldPosition9C.z = (unsigned short) height << 12;
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00416410
void CGameObject::OffLift(tCoord3d& point)
{
	(((CGameObjectLiftVtableView*) this)->*((CGameObjectLiftVtable*) m_pVtable00)->m_pOffLiftOneA4)(point);
}

// FUNCTION: LEMBALL 0x00416420
int CGameObject::OnLift(tCoord3d& start, tCoord3d& end)
{
	CGameObjectTileGrid* pGrid;
	short height;
	int minX;
	int minY;
	int x;
	int y;

	if (m_nStateB8 == 8) {
		return 0;
	}
	minX = start.x - 8;
	minY = start.y - 8;
	x = m_WorldPosition9C.x >> 12;
	y = m_WorldPosition9C.y >> 12;
	if (minX <= x && x <= end.x + 7 && minY <= y && y <= end.y + 7) {
		pGrid = (CGameObjectTileGrid*) g_pLevelTileGrid;
		if (minX < 0 || minY < 0 || pGrid->m_nWidth10 <= minX >> 4 || pGrid->m_nHeight14 <= minY >> 4) {
			height = 0;
		}
		else {
			height = pGrid->m_pGround0C[(minY >> 4) * pGrid->m_nWidth10 + (minX >> 4)].GetZ(minX & 0xf, minY & 0xf);
		}
		m_WorldPosition9C.z = (unsigned short) height << 12;
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x004164f0
void CGameObject::OffLift(tCoord3d& start, tCoord3d& end)
{
	(((CGameObjectLiftVtableView*) this)->*((CGameObjectLiftVtable*) m_pVtable00)->m_pOffLiftTwoA0)(start, end);
}
