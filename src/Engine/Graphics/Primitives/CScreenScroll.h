#ifndef LEMBALL_VISOS_GRAPHICS_CSCREENSCROLL_H
#define LEMBALL_VISOS_GRAPHICS_CSCREENSCROLL_H

#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "CPrimitive.h"

// SIZE 0x10
// VTABLE: LEMBALL 0x00496e40
class CScreenScroll : public CPrimitive {
public:
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	virtual ~CScreenScroll() {}       // vtable+0x00

	friend class CSurface;

public:
	CVSPoint m_destination; // 0x04
	CVSRect m_rect;         // 0x08
};

// SYNTHETIC: LEMBALL 0x00439650
// CScreenScroll::`scalar deleting destructor'

#endif
