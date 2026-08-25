#ifndef LEMBALL_VISOS_FOUNDATION_VSRECT_H
#define LEMBALL_VISOS_FOUNDATION_VSRECT_H

#include "../../Common.h"

// SIZE 0x08
class VsRect {
public:
	VsRect();
	VsRect(short p_x, short p_y, short p_width, short p_height);

	friend class Gdi;

private:
	short m_width;  // 0x00
	short m_height; // 0x02
	short m_x;      // 0x04
	short m_y;      // 0x06
};

#endif
