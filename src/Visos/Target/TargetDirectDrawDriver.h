#ifndef LEMBALL_VISOS_TARGET_TARGETDIRECTDRAWDRIVER_H
#define LEMBALL_VISOS_TARGET_TARGETDIRECTDRAWDRIVER_H

#include "../../Common.h"
#include "TargetGraphicsDriver.h" // complete type

// SIZE 0x4a8
// VTABLE: LEMBALL 0x004987e8
class TargetDirectDrawDriver : public TargetGraphicsDriver {
private:
	void* m_directDraw;            // 0x1c
	void* m_primarySurface;        // 0x20
	void* m_surface24;             // 0x24
	void* m_surface28;             // 0x28
	void* m_surface2c;             // 0x2c
	void* m_paletteInterface;      // 0x30
	undefined m_driverState[0x6c]; // 0x34
	void* m_contextSurfaces[257];  // 0xa0
	int m_nextContextIndex;        // 0x4a4
};

#endif
