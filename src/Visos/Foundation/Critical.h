#ifndef LEMBALL_SCAFFOLD_VISOS_FOUNDATION_CRITICAL_H
#define LEMBALL_SCAFFOLD_VISOS_FOUNDATION_CRITICAL_H

#include "../../Common.h"

// SIZE 0x1c
class Critical {
public:
	virtual void EnterCritical(); // vtable+0x00
	virtual void LeaveCritical(); // vtable+0x04

private:
	undefined m_criticalSection[0x18]; // 0x04
};

#endif
