#include "CVSPoint.h"

// FUNCTION: LEMBALL 0x00442130
CVSPoint& CVSPoint::operator=(const CVSPoint& p_source)
{
	m_x = p_source.m_x;
	m_y = p_source.m_y;
	return *this;
}

// FUNCTION: LEMBALL 0x004452a0
CVSPoint* CVSPoint::AddInPlace(CVSPoint* p_delta)
{
	m_x = (short) (m_x + p_delta->m_x);
	m_y = (short) (m_y + p_delta->m_y);
	return this;
}

// FUNCTION: LEMBALL 0x004452c0
CVSPoint* CVSPoint::SubtractInPlace(CVSPoint* p_delta)
{
	m_x = (short) (m_x - p_delta->m_x);
	m_y = (short) (m_y - p_delta->m_y);
	return this;
}

// FUNCTION: LEMBALL 0x0046ba50
int CVSPoint::Equals(const CVSPoint& p_other)
{
	return m_x == p_other.m_x && m_y == p_other.m_y;
}
