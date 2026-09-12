#ifndef LEMBALL_VISOS_FOUNDATION_VSPOINT_H
#define LEMBALL_VISOS_FOUNDATION_VSPOINT_H

#include "../../Common.h"

// SIZE 0x04
struct VsPoint {
	VsPoint(const VsPoint& p_source) : m_x(p_source.m_x), m_y(p_source.m_y) {}

	// FUNCTION: LEMBALL 0x0044b5d0
	VsPoint() { m_x = m_y = 0; }

	VsPoint(short p_x, short p_y) : m_x(p_x), m_y(p_y) {}

	VsPoint& operator=(const VsPoint& p_source);
	VsPoint* AddInPlace(VsPoint* p_delta);
	VsPoint* SubtractInPlace(VsPoint* p_delta);

	short m_x; // 0x00
	short m_y; // 0x02
};

#endif
