#include "AI/CGameObject.h"
#include "AI/CLift.h"

struct CGameObjectLiftVtableView {};

typedef void (CGameObjectLiftVtableView::*CGameObjectOffLiftOneProc)(tCoord3d& point);
typedef void (CGameObjectLiftVtableView::*CGameObjectOffLiftTwoProc)(tCoord3d& start, tCoord3d& end);

struct CGameObjectLiftVtable {
	void* m_apReserved00[0x28];
	CGameObjectOffLiftTwoProc m_pOffLiftTwoA0;
	CGameObjectOffLiftOneProc m_pOffLiftOneA4;
};

// FUNCTION: LEMBALL 0x00416410
void CGameObject::OffLift(tCoord3d& point)
{
	(((CGameObjectLiftVtableView*) this)->*((CGameObjectLiftVtable*) m_pVtable00)->m_pOffLiftOneA4)(point);
}

// FUNCTION: LEMBALL 0x004164f0
void CGameObject::OffLift(tCoord3d& start, tCoord3d& end)
{
	(((CGameObjectLiftVtableView*) this)->*((CGameObjectLiftVtable*) m_pVtable00)->m_pOffLiftTwoA0)(start, end);
}
