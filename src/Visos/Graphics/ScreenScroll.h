#ifndef LEMBALL_VISOS_GRAPHICS_SCREENSCROLL_H
#define LEMBALL_VISOS_GRAPHICS_SCREENSCROLL_H

#include "../Foundation/VsPoint.h" // complete type
#include "../Foundation/VsRect.h"  // complete type
#include "Primitive.h"             // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496e40
class ScreenScroll : public Primitive {
public:
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~ScreenScroll() {}       // vtable+0x00

	friend class Surface;

private:
	VsPoint m_destination; // 0x04
	VsRect m_rect;         // 0x08
};

// SYNTHETIC: LEMBALL 0x00439650
// ScreenScroll::`scalar deleting destructor'

#endif
