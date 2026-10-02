#ifndef LEMBALL_VISOS_GRAPHICS_CCOPYTOBACKBUFF_H
#define LEMBALL_VISOS_GRAPHICS_CCOPYTOBACKBUFF_H

#include "../Foundation/CVSRect.h"
#include "CPrimitive.h"

// SIZE 0x10
// VTABLE: LEMBALL 0x00496e90
class CCopyToBackBuff : public CPrimitive, public CVSPoint {
public:
	// FUNCTION: LEMBALL 0x00439580
	CCopyToBackBuff() {}
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x00439750
	virtual ~CCopyToBackBuff() {} // vtable+0x00

	friend class CBaseFrontendDrawer;
	friend class CSurface;
	friend class CCDLoadAnim;
	friend class C2D;

public:
	CVSRect m_destination; // 0x08
};

// SYNTHETIC: LEMBALL 0x004396e0
// CCopyToBackBuff::`scalar deleting destructor'

#endif
