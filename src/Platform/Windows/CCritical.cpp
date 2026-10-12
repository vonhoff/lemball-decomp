#include "CCritical.h"

// FUNCTION: LEMBALL 0x00459990
void CCritical::EnterCritical()
{
	EnterCriticalSection(&m_criticalSection);
}

// FUNCTION: LEMBALL 0x004599a0
void CCritical::LeaveCritical()
{
	LeaveCriticalSection(&m_criticalSection);
}
