#include "C3DVector.h"

#include "Engine/Math/CFixed.h"

// FUNCTION: LEMBALL 0x0042b9e0
C3DVector::C3DVector(const CFixed& p_x, const CFixed& p_y, const CFixed& p_z)
{
	m_xFixed = p_x.m_value;
	m_yFixed = p_y.m_value;
	m_zFixed = p_z.m_value;
}
