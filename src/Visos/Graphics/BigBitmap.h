#ifndef LEMBALL_VISOS_GRAPHICS_BIGBITMAP_H
#define LEMBALL_VISOS_GRAPHICS_BIGBITMAP_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496e80
class BigBitmap : public Primitive {
public:
	// 68K 0x1011bcf2 __ct__10CBigBitmapFv
	// FUNCTION: LEMBALL 0x004394f0
	BigBitmap() : m_y(0)
	{
		m_x = 0;
		m_height = 0;
		m_width = 0;
	}
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

// SYNTHETIC: LEMBALL 0x00439620
// BigBitmap::`scalar deleting destructor'

#endif
