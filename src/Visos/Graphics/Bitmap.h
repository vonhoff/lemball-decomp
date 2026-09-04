#ifndef LEMBALL_VISOS_GRAPHICS_BITMAP_H
#define LEMBALL_VISOS_GRAPHICS_BITMAP_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496e90
class Bitmap : public Primitive {
public:
	Bitmap();
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~Bitmap() {}             // vtable+0x00

	friend class BaseFrontendDrawer;
	friend class Surface;
	friend class CdLoadAnimDraw;

private:
	short m_x;       // 0x04
	short m_y;       // 0x06
	short m_width;   // 0x08
	short m_height;  // 0x0a
	short m_sourceX; // 0x0c
	short m_sourceY; // 0x0e
};

// SYNTHETIC: LEMBALL 0x004396e0
// Bitmap::`scalar deleting destructor'

#endif
