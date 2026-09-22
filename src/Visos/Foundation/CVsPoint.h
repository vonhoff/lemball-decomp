#ifndef LEMBALL_VISOS_FOUNDATION_CVSPOINT_H
#define LEMBALL_VISOS_FOUNDATION_CVSPOINT_H

// SIZE 0x04
struct CVsPoint {
	CVsPoint(const CVsPoint& p_source);

	CVsPoint();

	CVsPoint(short p_x, short p_y) : m_x(p_x), m_y(p_y) {}

	CVsPoint& operator=(const CVsPoint& p_source);
	CVsPoint* AddInPlace(CVsPoint* p_delta);
	CVsPoint* SubtractInPlace(CVsPoint* p_delta);
	int Equals(const CVsPoint& p_other);

	short m_x; // 0x00
	short m_y; // 0x02
};

inline CVsPoint::CVsPoint(const CVsPoint& p_source) : m_x(p_source.m_x), m_y(p_source.m_y)
{
}

// FUNCTION: LEMBALL 0x0044b5d0
inline CVsPoint::CVsPoint()
{
	m_x = m_y = 0;
}

#endif
