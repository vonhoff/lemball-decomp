#include "VsTime.h"

extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x00462e80
unsigned long CurrentMilliTimer()
{
	return timeGetTime();
}

// FUNCTION: LEMBALL 0x00462e90
unsigned long CurrentQueueTimer()
{
	return timeGetTime();
}
