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

private:
	short m_field04; // 0x04
	short m_field06; // 0x06
	short m_field08; // 0x08
	short m_field0a; // 0x0a
	short m_field0c; // 0x0c
	short m_field0e; // 0x0e
};

#endif
