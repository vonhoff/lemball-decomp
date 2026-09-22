#ifndef LEMBALL_VISOS_GRAPHICS_CCOPYTOBACKBUFF_H
#define LEMBALL_VISOS_GRAPHICS_CCOPYTOBACKBUFF_H

#include "../Foundation/CVsRect.h"
#include "CPrimitive.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496da0
class CCopyToBackBuff : public CPrimitive {
public:
	// FUNCTION: LEMBALL 0x00439550
	CCopyToBackBuff() {}
	virtual void Draw(CGdi* p_gdi);   // vtable+0x04
	virtual void Render(CGdi* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x00439740
	virtual ~CCopyToBackBuff() {} // vtable+0x00

	friend class CSurface;

private:
	short m_field04;       // 0x04
	short m_field06;       // 0x06
	CVsRect m_destination; // 0x08
};

// SYNTHETIC: LEMBALL 0x004396b0
// CCopyToBackBuff::`scalar deleting destructor'

#endif
