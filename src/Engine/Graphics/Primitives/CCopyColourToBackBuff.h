#ifndef LEMBALL_VISOS_GRAPHICS_CCOPYCOLOURTOBACKBUFF_H
#define LEMBALL_VISOS_GRAPHICS_CCOPYCOLOURTOBACKBUFF_H

#include "Engine/Math/CVSRect.h"
#include "CPrimitive.h"

// SIZE 0x10
// VTABLE: LEMBALL 0x00496e80
class CCopyColourToBackBuff : public CPrimitive {
public:
	// FUNCTION: LEMBALL 0x004394f0
	CCopyColourToBackBuff() : m_bounds() {}
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	virtual ~CCopyColourToBackBuff(); // vtable+0x00

private:
	friend class CSurface;
	unsigned int m_colour; // 0x04
	CVSRect m_bounds;      // 0x08
};

// SYNTHETIC: LEMBALL 0x00439620
// CCopyColourToBackBuff::`scalar deleting destructor'

#endif
