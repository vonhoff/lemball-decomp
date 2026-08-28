#ifndef LEMBALL_VISOS_GRAPHICS_COPYCOLOURTOBACKBUFF_H
#define LEMBALL_VISOS_GRAPHICS_COPYCOLOURTOBACKBUFF_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496d90
class CopyColourToBackBuff : public Primitive {
public:
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~CopyColourToBackBuff(); // vtable+0x00

	friend class Surface;

private:
	int m_colour;   // 0x04
	short m_width;  // 0x08
	short m_height; // 0x0a
	short m_x;      // 0x0c
	short m_y;      // 0x0e
};

// SYNTHETIC: LEMBALL 0x00439680
// CopyColourToBackBuff::`scalar deleting destructor'


#endif

