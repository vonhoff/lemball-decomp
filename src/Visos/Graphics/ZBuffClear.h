#ifndef LEMBALL_VISOS_GRAPHICS_ZBUFFCLEAR_H
#define LEMBALL_VISOS_GRAPHICS_ZBUFFCLEAR_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x10
class ZBuffClear : public Primitive {
public:
	friend class Surface;

private:
	short m_depth;           // 0x04
	undefined2 m_reserved06; // 0x06
	short m_width;           // 0x08
	short m_height;          // 0x0a
	short m_x;               // 0x0c
	short m_y;               // 0x0e
};

#endif
