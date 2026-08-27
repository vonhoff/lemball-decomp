#ifndef LEMBALL_VISOS_GRAPHICS_PUSHACTIVE_H
#define LEMBALL_VISOS_GRAPHICS_PUSHACTIVE_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x08
// VTABLE: LEMBALL 0x00496e70
class PushActive : public Primitive {
public:
	PushActive();
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~PushActive();           // vtable+0x00

private:
	unsigned int m_activeMarker; // 0x04
};

// SYNTHETIC: LEMBALL 0x004397a0
// PushActive::`scalar deleting destructor'


#endif

