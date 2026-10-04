#ifndef LEMBALL_VISOS_TARGET_GRAPHICS_CDIRECTDRAWCONTEXT_H
#define LEMBALL_VISOS_TARGET_GRAPHICS_CDIRECTDRAWCONTEXT_H

#include "CDrawingContext.h"

class CDirectDrawDriver;
// SIZE 0x08
// VTABLE: LEMBALL 0x00498820
class CDirectDrawContext : public CDrawingContext {
public:
	CDirectDrawContext(int p_surfaceIndex) { m_surfaceIndex = p_surfaceIndex; }
	virtual ~CDirectDrawContext() {}

	friend class CDirectDrawDriver;

private:
	int m_surfaceIndex; // 0x04
};

// SYNTHETIC: LEMBALL 0x00458380
// CDirectDrawContext::`scalar deleting destructor'

#endif
