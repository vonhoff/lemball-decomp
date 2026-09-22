#include "C3DVector.h"

#include "../../Visos/Foundation/CFixed.h"

// FUNCTION: LEMBALL 0x0040c270
C3DVector& C3DVector::operator=(const C3DVector& p_other)
{
	m_xFixed = p_other.m_xFixed;
	m_yFixed = p_other.m_yFixed;
	m_zFixed = p_other.m_zFixed;
	return *this;
}

// FUNCTION: LEMBALL 0x0042b9e0
C3DVector::C3DVector(const CFixed& p_x, const CFixed& p_y, const CFixed& p_z)
{
	m_xFixed = p_x.m_value;
	m_yFixed = p_y.m_value;
	m_zFixed = p_z.m_value;
}
