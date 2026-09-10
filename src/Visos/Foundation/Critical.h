#ifndef LEMBALL_VISOS_FOUNDATION_CRITICAL_H
#define LEMBALL_VISOS_FOUNDATION_CRITICAL_H

#include "../../Common.h"

struct _RTL_CRITICAL_SECTION;
typedef struct _RTL_CRITICAL_SECTION CRITICAL_SECTION;

extern "C"
{
	__declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
	__declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION*);
	__declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
	__declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);
}

// SIZE 0x1c
// VTABLE: LEMBALL 0x004988d0
class Critical {
public:
	inline Critical() { InitializeCriticalSection((_RTL_CRITICAL_SECTION*) m_criticalSection); }
	inline ~Critical() { DeleteCriticalSection((_RTL_CRITICAL_SECTION*) m_criticalSection); }
	virtual void EnterCritical(); // vtable+0x00
	virtual void LeaveCritical(); // vtable+0x04

protected:
	unsigned char m_criticalSection[0x18]; // 0x04
};

#endif
