#ifndef LEMBALL_VISOS_TARGET_GRAPHICS_CGDIDIBCONTEXT_H
#define LEMBALL_VISOS_TARGET_GRAPHICS_CGDIDIBCONTEXT_H

#include "CDibContext.h"
#include "Platform/Windows/WindowsTypes.h"

// SIZE 0x14
// VTABLE: LEMBALL 0x00498798
class CGdiDibContext : public CDibContext {
public:
	CGdiDibContext(HBITMAP p_bitmap) { m_hBitmap = p_bitmap; }
	virtual ~CGdiDibContext() {}

	friend class CGdiDriver;

private:
	HGDIOBJ m_previousBitmap; // 0x0c
	HBITMAP m_hBitmap;        // 0x10
};

// SYNTHETIC: LEMBALL 0x00458310
// CGdiDibContext::`scalar deleting destructor'

#endif
