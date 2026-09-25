#ifndef LEMBALL_VISOS_GRAPHICS_CPOPACTIVE_H
#define LEMBALL_VISOS_GRAPHICS_CPOPACTIVE_H

#include "CPrimitive.h"

// SIZE 0x04
// VTABLE: LEMBALL 0x00496e50
class CPopActive : public CPrimitive {
public:
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
};

// SYNTHETIC: LEMBALL 0x004395c0
// CPopActive::`scalar deleting destructor'

#endif
