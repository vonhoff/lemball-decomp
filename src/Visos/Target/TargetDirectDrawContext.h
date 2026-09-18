#ifndef LEMBALL_VISOS_TARGET_TARGETDIRECTDRAWCONTEXT_H
#define LEMBALL_VISOS_TARGET_TARGETDIRECTDRAWCONTEXT_H

#include "TargetDrawingContext.h"

class TargetDirectDrawDriver;
// SIZE 0x08
// VTABLE: LEMBALL 0x00498820
class TargetDirectDrawContext : public TargetDrawingContext {
public:
	TargetDirectDrawContext(int p_surfaceIndex) { m_surfaceIndex = p_surfaceIndex; }
	virtual ~TargetDirectDrawContext() {}

	friend class TargetDirectDrawDriver;

private:
	int m_surfaceIndex; // 0x04
};

// SYNTHETIC: LEMBALL 0x00458380
// TargetDirectDrawContext::`scalar deleting destructor'

#endif
