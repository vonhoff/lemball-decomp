#ifndef LEMBALL_SCAFFOLD_VISOS_GRAPHICS_LINE_H
#define LEMBALL_SCAFFOLD_VISOS_GRAPHICS_LINE_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496d38
class Line : public Primitive {
public:
	Line();
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~Line();                 // vtable+0x00

private:
	short m_x1;           // 0x04
	short m_y1;           // 0x06
	short m_x2;           // 0x08
	short m_y2;           // 0x0a
	unsigned int m_color; // 0x0c
};

#endif
