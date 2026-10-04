#ifndef LEMBALL_VISOS_TARGET_GRAPHICS_CGDIDIBCONTEXT_H
#define LEMBALL_VISOS_TARGET_GRAPHICS_CGDIDIBCONTEXT_H

#include "CDibContext.h"

class CGdiDriver;
// SIZE 0x14
// VTABLE: LEMBALL 0x00498798
class CGdiDibContext : public CDibContext {
public:
	CGdiDibContext(void* p_bitmap) { m_hBitmap = p_bitmap; }
	virtual ~CGdiDibContext() {}

	friend class CGdiDriver;

private:
	void* m_previousBitmap; // 0x0c
	void* m_hBitmap;        // 0x10
};

// SYNTHETIC: LEMBALL 0x00458310
// CGdiDibContext::`scalar deleting destructor'

#endif
