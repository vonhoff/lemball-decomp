#ifndef LEMBALL_VISOS_GRAPHICS_CBITMAP_H
#define LEMBALL_VISOS_GRAPHICS_CBITMAP_H

#include "../Foundation/CVsRect.h"
#include "CPrimitive.h"

// SIZE 0x10
// VTABLE: LEMBALL 0x00496e90
class CBitmap : public CPrimitive {
public:
	// FUNCTION: LEMBALL 0x00439580
	CBitmap() : m_x(m_y = 0) {}
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x00439750
	virtual ~CBitmap() {} // vtable+0x00

	friend class CBaseFrontendDrawer;
	friend class CSurface;
	friend class CCDLoadAnim;
	friend class C2D;

public:
	short m_x;            // 0x04
	short m_y;            // 0x06
	CVsRect m_sourceRect; // 0x08
};

// SYNTHETIC: LEMBALL 0x004396e0
// CBitmap::`scalar deleting destructor'

#endif
