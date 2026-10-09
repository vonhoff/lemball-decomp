#include "CVMovePos.h"

// FUNCTION: LEMBALL 0x00417b00
void CVMovePos::SetEndpoints(CVector p_start, CVector p_end)
{
	m_delta.m_xFixed = p_end.m_xFixed - p_start.m_xFixed;
	m_delta.m_yFixed = p_end.m_yFixed - p_start.m_yFixed;
	m_start.m_xFixed = p_start.m_xFixed;
	m_start.m_yFixed = p_start.m_yFixed;
}
