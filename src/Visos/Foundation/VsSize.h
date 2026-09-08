#ifndef LEMBALL_VISOS_FOUNDATION_VSSIZE_H
#define LEMBALL_VISOS_FOUNDATION_VSSIZE_H

#include "../../Common.h"

// SIZE 0x04
struct VsSize {
	VsSize() {}
	VsSize(short p_width, short p_height);
	VsSize(const VsSize& p_source);

	short m_width;  // 0x00
	short m_height; // 0x02
};

#endif
