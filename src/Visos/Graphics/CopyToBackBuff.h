#ifndef LEMBALL_VISOS_GRAPHICS_COPYTOBACKBUFF_H
#define LEMBALL_VISOS_GRAPHICS_COPYTOBACKBUFF_H

#include "../../Common.h"
#include "Primitive.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496da0
class CopyToBackBuff : public Primitive {
public:
	CopyToBackBuff();
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	virtual ~CopyToBackBuff();       // vtable+0x00

	friend class Surface;

private:
	short m_field04; // 0x04
	short m_field06; // 0x06
	short m_field08; // 0x08
	short m_field0a; // 0x0a
	short m_field0c; // 0x0c
	short m_field0e; // 0x0e
};

// SYNTHETIC: LEMBALL 0x004396b0
// CopyToBackBuff::`scalar deleting destructor'


#endif

