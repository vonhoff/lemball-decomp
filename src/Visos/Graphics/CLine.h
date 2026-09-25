#ifndef LEMBALL_VISOS_GRAPHICS_CLINE_H
#define LEMBALL_VISOS_GRAPHICS_CLINE_H

#include "../Foundation/CVsRect.h"
#include "CPrimitive.h"

// SIZE 0x10
// VTABLE: LEMBALL 0x00496d38
class CLine : public CPrimitive {
public:
	// FUNCTION: LEMBALL 0x004394c0
	CLine() : m_bounds() {}
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
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
	CVsRect m_bounds;     // 0x04
	unsigned int m_color; // 0x0c
};

// SYNTHETIC: LEMBALL 0x00469930
// CLine::`vector deleting destructor'

#endif
