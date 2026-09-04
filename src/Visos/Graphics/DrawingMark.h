#ifndef LEMBALL_VISOS_GRAPHICS_DRAWINGMARK_H
#define LEMBALL_VISOS_GRAPHICS_DRAWINGMARK_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x04
// VTABLE: LEMBALL 0x00496c98
// A second original construction table exists at 0x00496e40.
class DrawingMark : public Primitive {
public:
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~DrawingMark() {}        // vtable+0x00
};

// SYNTHETIC: LEMBALL 0x00432320
// DrawingMark::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00439650
// DrawingMark::`scalar deleting destructor'

#endif
