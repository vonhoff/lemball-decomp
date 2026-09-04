#ifndef LEMBALL_VISOS_GRAPHICS_POINT_H
#define LEMBALL_VISOS_GRAPHICS_POINT_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x0c
class Point : public Primitive {
public:
	friend class Surface;

private:
	short m_x;            // 0x04
	short m_y;            // 0x06
	unsigned int m_color; // 0x08
};

#endif
