#ifndef LEMBALL_VISOS_GRAPHICS_PRIMITIVE_H
#define LEMBALL_VISOS_GRAPHICS_PRIMITIVE_H

#include "../../Common.h"

// SIZE 0x04
// VTABLE: LEMBALL 0x00496ca8
class Primitive {
public:
	inline Primitive() {}

	virtual ~Primitive() {}              // vtable+0x00
	virtual void Draw(Gdi* p_gdi) = 0;   // vtable+0x04
	virtual void Render(Gdi* p_gdi) = 0; // vtable+0x08
};

// SYNTHETIC: LEMBALL 0x00432350
// Primitive::`scalar deleting destructor'

#endif
