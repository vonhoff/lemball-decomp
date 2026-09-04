#ifndef LEMBALL_VISOS_TARGET_TARGETDRAWINGCONTEXT_H
#define LEMBALL_VISOS_TARGET_TARGETDRAWINGCONTEXT_H

#include "../../Common.h"

// SIZE 0x0c
// VTABLE: LEMBALL 0x00498770
class TargetDrawingContext {
public:
	TargetDrawingContext(void* p_hDC = 0)
	{
		m_hDC = p_hDC;
		m_hBitmap = 0;
	}
	virtual ~TargetDrawingContext(); // vtable+0x00
	virtual void SetDC(void* p_hDC); // vtable+0x04

	friend class TargetGraphicsDriver;
	friend class Surface;

private:
	void* m_hDC;     // 0x04
	void* m_hBitmap; // 0x08
};

#endif
