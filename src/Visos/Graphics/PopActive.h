#ifndef LEMBALL_VISOS_GRAPHICS_POPACTIVE_H
#define LEMBALL_VISOS_GRAPHICS_POPACTIVE_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x04
// VTABLE: LEMBALL 0x00496e50
class PopActive : public Primitive {
public:
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~PopActive();            // vtable+0x00
};

#endif
