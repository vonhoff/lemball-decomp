#ifndef LEMBALL_SCAFFOLD_VISOS_GRAPHICS_DRAWINGMARK_H
#define LEMBALL_SCAFFOLD_VISOS_GRAPHICS_DRAWINGMARK_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x04
// VTABLE: LEMBALL 0x00496e40
class DrawingMark : public Primitive {
public:
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~DrawingMark();          // vtable+0x00
};

#endif
