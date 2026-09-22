#ifndef LEMBALL_VISOS_GRAPHICS_CLINE_H
#define LEMBALL_VISOS_GRAPHICS_CLINE_H

#include "CPrimitive.h" // complete type

// SIZE 0x10
// VTABLE: LEMBALL 0x00496d38
class CLine : public CPrimitive {
public:
	// FUNCTION: LEMBALL 0x004394c0
	CLine() : m_y1(0)
	{
		m_x1 = 0;
		m_y2 = 0;
		m_x2 = 0;
	}
	virtual void Draw(CGdi* p_gdi);   // vtable+0x04
	virtual void Render(CGdi* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x00439710
	virtual ~CLine() {} // vtable+0x00

	friend class CBaseFrontendDrawer;
	friend class CCdLoadAnimDraw;
	friend class CAboutScreen;
	friend class CSurface;
	friend class CPauseWindow;
	friend class C2D;
	friend class CTrackWindow;

public:
	short m_x1;           // 0x04
	short m_y1;           // 0x06
	short m_x2;           // 0x08
	short m_y2;           // 0x0a
	unsigned int m_color; // 0x0c
};

// SYNTHETIC: LEMBALL 0x00469930
// CLine::`vector deleting destructor'

#endif
