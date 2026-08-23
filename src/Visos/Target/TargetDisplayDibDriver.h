#ifndef LEMBALL_VISOS_TARGET_TARGETDISPLAYDIBDRIVER_H
#define LEMBALL_VISOS_TARGET_TARGETDISPLAYDIBDRIVER_H

#include "../../Common.h"
#include "TargetGraphicsDriver.h" // complete type

// SIZE 0x30
// VTABLE: LEMBALL 0x004987b0
class TargetDisplayDibDriver : public TargetGraphicsDriver {
private:
	unsigned char* m_frameBuffer; // 0x1c
	unsigned int m_reserved20;    // 0x20
	void* m_displayDibStart;      // 0x24
	void* m_displayDibEnd;        // 0x28
	void* m_displayDibGetAddress; // 0x2c
};

#endif
