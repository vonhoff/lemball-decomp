#ifndef LEMBALL_VISOS_GRAPHICS_SCREENSCROLL_H
#define LEMBALL_VISOS_GRAPHICS_SCREENSCROLL_H

#include "Primitive.h" // complete type

// SIZE 0x10
class ScreenScroll : public Primitive {
public:
	friend class Surface;

private:
	VsPoint m_destination; // 0x04
	VsRect m_rect;         // 0x08
};

#endif
