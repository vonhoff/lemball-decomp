#ifndef LEMBALL_VISOS_FOUNDATION_VSPOINT_H
#define LEMBALL_VISOS_FOUNDATION_VSPOINT_H

#include "../../Common.h"

// SIZE 0x04
struct VsPoint {
	VsPoint& operator=(const VsPoint& p_source);
	VsPoint* AddInPlace(VsPoint* p_delta);

	short m_x; // 0x00
	short m_y; // 0x02
};

#endif
