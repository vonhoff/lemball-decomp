#ifndef LEMBALL_VISOS_GRAPHICS_CZBUFFCLEAR_H
#define LEMBALL_VISOS_GRAPHICS_CZBUFFCLEAR_H

#include "Engine/Math/CVSRect.h"
#include "CPrimitive.h"

// SIZE 0x10
// VTABLE: LEMBALL 0x00496da0
class CZBuffClear : public CPrimitive {
public:
	// FUNCTION: LEMBALL 0x00439550
	CZBuffClear() {}
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	// FUNCTION: LEMBALL 0x00439740
	virtual ~CZBuffClear() {} // vtable+0x00

	friend class CSurface;

public:
	unsigned int m_depth; // 0x04
	CVSRect m_bounds;     // 0x08
};

// SYNTHETIC: LEMBALL 0x004396b0
// CZBuffClear::`scalar deleting destructor'

#endif
