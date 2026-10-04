#include "CCritical.h"

// FUNCTION: LEMBALL 0x00459990
void CCritical::EnterCritical()
{
	EnterCriticalSection((_RTL_CRITICAL_SECTION*) m_criticalSection);
}

// FUNCTION: LEMBALL 0x004599a0
void CCritical::LeaveCritical()
{
	LeaveCriticalSection((_RTL_CRITICAL_SECTION*) m_criticalSection);
}
