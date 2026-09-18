#ifndef LEMBALL_VISOS_TARGET_TARGETGDIDIBCONTEXT_H
#define LEMBALL_VISOS_TARGET_TARGETGDIDIBCONTEXT_H

#include "TargetDibContext.h"

class TargetGDIDriver;
// SIZE 0x14
// VTABLE: LEMBALL 0x00498798
class TargetGdiDibContext : public TargetDibContext {
public:
	TargetGdiDibContext(void* p_bitmap) { m_hBitmap = p_bitmap; }
	virtual ~TargetGdiDibContext() {}

	friend class TargetGDIDriver;

private:
	void* m_previousBitmap; // 0x0c
	void* m_hBitmap;        // 0x10
};

// SYNTHETIC: LEMBALL 0x00458310
// TargetGdiDibContext::`scalar deleting destructor'

#endif
