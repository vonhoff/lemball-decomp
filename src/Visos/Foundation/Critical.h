#ifndef LEMBALL_VISOS_FOUNDATION_CRITICAL_H
#define LEMBALL_VISOS_FOUNDATION_CRITICAL_H

#include "../../Common.h"

extern "C" {
__declspec(dllimport) void __stdcall InitializeCriticalSection(void*);
__declspec(dllimport) void __stdcall DeleteCriticalSection(void*);
__declspec(dllimport) void __stdcall EnterCriticalSection(void*);
__declspec(dllimport) void __stdcall LeaveCriticalSection(void*);
}

// SIZE 0x1c
// VTABLE: LEMBALL 0x004988d0
class Critical {
public:
	inline Critical()
	{
		InitializeCriticalSection(m_criticalSection);
	}
	inline ~Critical()
	{
		DeleteCriticalSection(m_criticalSection);
	}
	virtual void EnterCritical(); // vtable+0x00
	virtual void LeaveCritical(); // vtable+0x04

private:
	unsigned char m_criticalSection[0x18]; // 0x04
};

#endif
