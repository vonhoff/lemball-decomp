#include "VsPoint.h"

#include "VsRect.h"
#include "VsSize.h"

// FUNCTION: LEMBALL 0x00442130
VsPoint& VsPoint::operator=(const VsPoint& p_source)
{
	m_x = p_source.m_x;
	m_y = p_source.m_y;
	return *this;
}

// FUNCTION: LEMBALL 0x004452a0
VsPoint* VsPoint::AddInPlace(VsPoint* p_delta)
{
	m_x = (short) (m_x + p_delta->m_x);
	m_y = (short) (m_y + p_delta->m_y);
	return this;
}

// FUNCTION: LEMBALL 0x004452c0
VsPoint* VsPoint::SubtractInPlace(VsPoint* p_delta)
{
	m_x = (short) (m_x - p_delta->m_x);
	m_y = (short) (m_y - p_delta->m_y);
	return this;
}
