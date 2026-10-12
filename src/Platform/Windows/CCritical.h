#ifndef LEMBALL_VISOS_FOUNDATION_CCRITICAL_H
#define LEMBALL_VISOS_FOUNDATION_CCRITICAL_H

#include "WindowsTypes.h"

// SIZE 0x1c
// VTABLE: LEMBALL 0x004988d0
class CCritical {
public:
	CCritical() { InitializeCriticalSection(&m_criticalSection); }
	~CCritical() { DeleteCriticalSection(&m_criticalSection); }
	virtual void EnterCritical(); // vtable+0x00
	virtual void LeaveCritical(); // vtable+0x04

protected:
	CRITICAL_SECTION m_criticalSection; // 0x04
};

#endif
