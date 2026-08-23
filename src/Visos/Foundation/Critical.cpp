#include "Critical.h"

// 68K 0x10100844 EnterCritical__9CCriticalFv
// FUNCTION: LEMBALL 0x00459990
void Critical::EnterCritical()
{
	EnterCriticalSection(m_criticalSection);
}

// 68K 0x1010086a LeaveCritical__9CCriticalFv
// FUNCTION: LEMBALL 0x004599a0
void Critical::LeaveCritical()
{
	LeaveCriticalSection(m_criticalSection);
}
