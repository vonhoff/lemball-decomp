#ifndef LEMBALL_VISOS_GRAPHICS_CFILLEDCIRCLE_H
#define LEMBALL_VISOS_GRAPHICS_CFILLEDCIRCLE_H

#include "CPrimitive.h" // complete type

// SIZE 0x10
class CFilledCircle : public CPrimitive {
public:
	friend class CSurface;

private:
	short m_x;             // 0x04
	short m_y;             // 0x06
	unsigned int m_radius; // 0x08
	unsigned int m_color;  // 0x0c
};

#endif
