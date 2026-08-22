#ifndef LEMBALL_SCAFFOLD_VISOS_FOUNDATION_VSRECT_H
#define LEMBALL_SCAFFOLD_VISOS_FOUNDATION_VSRECT_H

#include "../../Common.h"

// SIZE 0x08
class VsRect {
public:
	VsRect();

private:
	short m_width;  // 0x00
	short m_height; // 0x02
	short m_x;      // 0x04
	short m_y;      // 0x06
};

#endif
