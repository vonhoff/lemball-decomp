#ifndef LEMBALL_VISOS_GRAPHICS_CLIPRECT_H
#define LEMBALL_VISOS_GRAPHICS_CLIPRECT_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496cc8
class ClipRect : public Primitive {
public:
	ClipRect();
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~ClipRect();             // vtable+0x00

	friend class PvButton;
	friend class Surface;
	friend class CdLoadAnimDraw;

private:
	short m_left;            // 0x04
	short m_top;             // 0x06
	short m_right;           // 0x08
	short m_bottom;          // 0x0a
	undefined4 m_reserved0c; // 0x0c
};

// SYNTHETIC: LEMBALL 0x00467bb0
// ClipRect::`scalar deleting destructor'


#endif

