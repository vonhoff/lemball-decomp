#ifndef LEMBALL_VISOS_FOUNDATION_CVECTOR_H
#define LEMBALL_VISOS_FOUNDATION_CVECTOR_H

// SIZE 0x08
class CVector {
public:
	CVector();
	CVector(const int& p_x, const int& p_y) : m_xFixed(p_x), m_yFixed(p_y) {}
	// FUNCTION: LEMBALL 0x00417b30
	CVector(const CVector& p_other) : m_xFixed(p_other.m_xFixed), m_yFixed(p_other.m_yFixed) {}
	CVector(long p_x, long p_y);
	// FUNCTION: LEMBALL 0x0040c290
	CVector& operator=(const CVector& p_other)
	{
		m_xFixed = p_other.m_xFixed;
		m_yFixed = p_other.m_yFixed;
		return *this;
	}

	int m_xFixed; // 0x00
	int m_yFixed; // 0x04
};

CVector operator*(const CVector& p_vector, int p_scale);
CVector operator+(const CVector& p_left, const CVector& p_right);

#endif
