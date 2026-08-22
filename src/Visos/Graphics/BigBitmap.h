#ifndef LEMBALL_SCAFFOLD_VISOS_GRAPHICS_BIGBITMAP_H
#define LEMBALL_SCAFFOLD_VISOS_GRAPHICS_BIGBITMAP_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496e80
class BigBitmap : public Primitive {
public:
	BigBitmap();
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~BigBitmap();            // vtable+0x00

private:
	undefined4 m_reserved04; // 0x04
	short m_x;               // 0x08
	short m_y;               // 0x0a
	short m_width;           // 0x0c
	short m_height;          // 0x0e
};

#endif
