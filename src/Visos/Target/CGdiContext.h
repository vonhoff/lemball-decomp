#ifndef LEMBALL_VISOS_TARGET_CGDICONTEXT_H
#define LEMBALL_VISOS_TARGET_CGDICONTEXT_H

#include "CDrawingContext.h"

// SIZE 0x0c
// VTABLE: LEMBALL 0x00498770
class CGdiContext : public CDrawingContext {
public:
	CGdiContext(void* p_hDC = 0)
	{
		m_hDC = p_hDC;
		m_hBitmap = 0;
	}
	virtual ~CGdiContext() {}        // vtable+0x00
	virtual void SetDc(void* p_hDC); // vtable+0x04

	friend class CGraphicsDriver;
	friend class CGdiDriver;
	friend class CDisplayDibDriver;
	friend class CPlanarDibDriver;
	friend class CSurface;

private:
	void* m_hDC;     // 0x04
	void* m_hBitmap; // 0x08
};

// SYNTHETIC: LEMBALL 0x00458260
// CGdiContext::`scalar deleting destructor'

#endif
