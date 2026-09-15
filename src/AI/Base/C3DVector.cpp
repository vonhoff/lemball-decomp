#define LEMBALL_OUTLINE_VIEW_HELPERS
#define LEMBALL_OUTLINE_VECTOR_CONSTRUCTOR
#include "C3DVector.h"

#include "../../Visos/Foundation/Fixed.h"

// FUNCTION: LEMBALL 0x0040c270
C3DVector& C3DVector::operator=(const C3DVector& p_other)
{
	m_xFixed = p_other.m_xFixed;
	m_yFixed = p_other.m_yFixed;
	m_zFixed = p_other.m_zFixed;
	return *this;
}

// FUNCTION: LEMBALL 0x0042b9e0
C3DVector::C3DVector(const Fixed& p_x, const Fixed& p_y, const Fixed& p_z)
{
	m_xFixed = p_x.m_value;
	m_yFixed = p_y.m_value;
	m_zFixed = p_z.m_value;
}
