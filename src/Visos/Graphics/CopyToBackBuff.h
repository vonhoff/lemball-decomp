#ifndef LEMBALL_VISOS_GRAPHICS_COPYTOBACKBUFF_H
#define LEMBALL_VISOS_GRAPHICS_COPYTOBACKBUFF_H

#include "../Foundation/VsRect.h"
#include "Primitive.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496da0
class CopyToBackBuff : public Primitive {
public:
	// 68K 0x1010a668 __ct__15CCopyToBackBuffFv
	// FUNCTION: LEMBALL 0x00439550
	CopyToBackBuff() {}
	virtual void Draw(Gdi* p_gdi);   // vtable+0x04
	virtual void Render(Gdi* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x00439740
	virtual ~CopyToBackBuff() {} // vtable+0x00

	friend class Surface;

private:
	short m_field04;      // 0x04
	short m_field06;      // 0x06
	VsRect m_destination; // 0x08
};

// SYNTHETIC: LEMBALL 0x004396b0
// CopyToBackBuff::`scalar deleting destructor'

#endif
