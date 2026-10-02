#ifndef LEMBALL_VISOS_FOUNDATION_CVSPOINT_H
#define LEMBALL_VISOS_FOUNDATION_CVSPOINT_H

// SIZE 0x04
struct CVSPoint {
	CVSPoint(const CVSPoint& p_source);

	CVSPoint();

	CVSPoint(short p_x, short p_y) : m_x(p_x), m_y(p_y) {}

	CVSPoint& operator=(const CVSPoint& p_source);
	CVSPoint* AddInPlace(CVSPoint* p_delta);
	CVSPoint* SubtractInPlace(CVSPoint* p_delta);
	int Equals(const CVSPoint& p_other);

	short m_x; // 0x00
	short m_y; // 0x02
};

// FUNCTION: LEMBALL 0x00465a50
inline CVSPoint::CVSPoint(const CVSPoint& p_source) : m_x(p_source.m_x), m_y(p_source.m_y)
{
}

// FUNCTION: LEMBALL 0x0044b5d0
inline CVSPoint::CVSPoint()
{
	m_x = m_y = 0;
}

#endif
