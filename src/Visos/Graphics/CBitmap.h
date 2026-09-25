#ifndef LEMBALL_VISOS_GRAPHICS_CBITMAP_H
#define LEMBALL_VISOS_GRAPHICS_CBITMAP_H

#include "CPrimitive.h"

// SIZE 0x10
// VTABLE: LEMBALL 0x00496e90
class CBitmap : public CPrimitive {
public:
	// FUNCTION: LEMBALL 0x00439580
	CBitmap() : m_x(m_y = 0)
	{
		m_height = 0;
		m_width = 0;
		m_sourceY = 0;
		m_sourceX = 0;
	}
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x00439750
	virtual ~CBitmap() {} // vtable+0x00

	friend class CBaseFrontendDrawer;
	friend class CSurface;
	friend class CCDLoadAnim;
	friend class C2D;

public:
	short m_x;       // 0x04
	short m_y;       // 0x06
	short m_width;   // 0x08
	short m_height;  // 0x0a
	short m_sourceX; // 0x0c
	short m_sourceY; // 0x0e
};

// SYNTHETIC: LEMBALL 0x004396e0
// CBitmap::`scalar deleting destructor'

#endif
