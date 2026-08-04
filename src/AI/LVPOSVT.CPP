#include "AI/CAmmo.h"

struct GmobChunkObjectPositionView {
	char m_abReserved00[0x9c];
	LevelThreeDwordPosition m_CurrentPosition9c;

	LevelThreeDwordPosition* CopyCurrentPositionVariant(LevelThreeDwordPosition* pPoint);
};

// FUNCTION: LEMBALL 0x0041cc70
LevelThreeDwordPosition* CAmmo::ActivatePosition(LevelThreeDwordPosition* pPoint)
{
	int nY;
	int nZ;

	nY = m_CurrentPosition9C.m_nY;
	nZ = m_CurrentPosition9C.m_nZ;
	pPoint->m_nX = m_CurrentPosition9C.m_nX;
	pPoint->m_nY = nY;
	pPoint->m_nZ = nZ;
	return pPoint;
}

// FUNCTION: LEMBALL 0x0041d7b0
LevelThreeDwordPosition* GmobChunkObjectPositionView::CopyCurrentPositionVariant(LevelThreeDwordPosition* pPoint)
{
	int nY;
	int nZ;

	nY = m_CurrentPosition9c.m_nY;
	nZ = m_CurrentPosition9c.m_nZ;
	pPoint->m_nX = m_CurrentPosition9c.m_nX;
	pPoint->m_nY = nY;
	pPoint->m_nZ = nZ;
	return pPoint;
}
