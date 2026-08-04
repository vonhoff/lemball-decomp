#include "AI/CAmmo.h"
#include "AI/CBalloon.h"

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
int* CBalloon::ActivatePosition(int* pPoint)
{
	int nY;
	int nZ;

	nY = m_nPositionYA0;
	nZ = m_nPositionZA4;
	pPoint[0] = m_nPositionX9C;
	pPoint[1] = nY;
	pPoint[2] = nZ;
	return pPoint;
}
