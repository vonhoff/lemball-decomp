#ifndef LEMBALL_VISOS_TARGET_TARGETDIRECTDRAWSURFACECONTEXT_H
#define LEMBALL_VISOS_TARGET_TARGETDIRECTDRAWSURFACECONTEXT_H

#include "../../Common.h"

// SIZE 0x7c
// VTABLE: LEMBALL 0x00498828
class TargetDirectDrawSurfaceContext {
public:
	// No functions mapped yet.

private:
	unsigned char* m_bits;                // 0x04
	int m_stride;                         // 0x08
	void* m_surface;                      // 0x0c
	undefined m_surfaceDescription[0x6c]; // 0x10
};

#endif
