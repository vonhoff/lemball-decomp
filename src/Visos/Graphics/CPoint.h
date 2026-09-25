#ifndef LEMBALL_VISOS_GRAPHICS_CPOINT_H
#define LEMBALL_VISOS_GRAPHICS_CPOINT_H

#include "CPrimitive.h"

// SIZE 0x0c
class CPoint : public CPrimitive {
public:
	friend class CSurface;

private:
	short m_x;            // 0x04
	short m_y;            // 0x06
	unsigned int m_color; // 0x08
};

#endif
