#ifndef LEMBALL_VISOS_GRAPHICS_FILLEDCIRCLE_H
#define LEMBALL_VISOS_GRAPHICS_FILLEDCIRCLE_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x10
class FilledCircle : public Primitive {
public:
	friend class Surface;

private:
	short m_x;              // 0x04
	short m_y;              // 0x06
	unsigned int m_radius;  // 0x08
	unsigned int m_color;   // 0x0c
};

#endif
