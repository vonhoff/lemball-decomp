#ifndef LEMBALL_VISOS_GRAPHICS_CDRAWINGMARK_H
#define LEMBALL_VISOS_GRAPHICS_CDRAWINGMARK_H

#include "CPrimitive.h"

// SIZE 0x04
// VTABLE: LEMBALL 0x00496c98
class CDrawingMark : public CPrimitive {
public:
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	virtual ~CDrawingMark() {}        // vtable+0x00
};

// SYNTHETIC: LEMBALL 0x00432320
// CDrawingMark::`scalar deleting destructor'

#endif
