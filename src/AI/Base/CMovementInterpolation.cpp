#include "CMovementInterpolation.h"

// FUNCTION: LEMBALL 0x00417b00
void CMovementInterpolation::SetEndpoints(CVector p_start, CVector p_end)
{
	m_deltaX = p_end.m_xFixed - p_start.m_xFixed;
	m_deltaY = p_end.m_yFixed - p_start.m_yFixed;
	m_startX = p_start.m_xFixed;
	m_startY = p_start.m_yFixed;
}

// FUNCTION: LEMBALL 0x004267a0
CMovementInterpolation& CMovementInterpolation::operator=(const CMovementInterpolation& p_other)
{
	m_startX = p_other.m_startX;
	m_startY = p_other.m_startY;
	m_deltaX = p_other.m_deltaX;
	m_deltaY = p_other.m_deltaY;
	return *this;
}
