#ifndef LEMBALL_VISOS_GRAPHICS_CPUSHACTIVE_H
#define LEMBALL_VISOS_GRAPHICS_CPUSHACTIVE_H

#include "CPrimitive.h" // complete type

// SIZE 0x08
// VTABLE: LEMBALL 0x00496e70
class CPushActive : public CPrimitive {
public:
	// FUNCTION: LEMBALL 0x004698b0
	CPushActive() { m_activeMarker = 0; }
	virtual void Draw(CGDI* p_gdi);   // vtable+0x04
	virtual void Render(CGDI* p_gdi); // vtable+0x08
	virtual ~CPushActive() {}         // vtable+0x00

	friend class CBaseCursor;

private:
	unsigned int m_activeMarker; // 0x04
};

// SYNTHETIC: LEMBALL 0x004397a0
// CPushActive::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x004698f0
// CPushActive::~CPushActive

#endif
