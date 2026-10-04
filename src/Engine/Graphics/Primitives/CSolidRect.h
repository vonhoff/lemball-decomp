#ifndef LEMBALL_VISOS_GRAPHICS_CSOLIDRECT_H
#define LEMBALL_VISOS_GRAPHICS_CSOLIDRECT_H

#include "Engine/Math/CVSRect.h"
#include "CPrimitive.h"

// SIZE 0x10
// VTABLE: LEMBALL 0x00496d38
class CSolidRect : public CPrimitive {
public:
	// FUNCTION: LEMBALL 0x004394c0
	CSolidRect() : m_bounds() {}
	CVSRect* GetBounds();
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x00439710
	virtual ~CSolidRect() {} // vtable+0x00

	friend class CBaseFrontendDrawer;
	friend class CCdLoadAnimDraw;
	friend class CAboutScreen;
	friend class CSurface;
	friend class CPauseWindow;
	friend class C2D;
	friend class CTrackWindow;

public:
	CVSRect m_bounds;      // 0x04
	unsigned int m_colour; // 0x0c
};

// SYNTHETIC: LEMBALL 0x00469930
// CSolidRect::`vector deleting destructor'

#endif
