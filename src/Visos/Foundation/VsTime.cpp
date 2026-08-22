#include "VsTime.h"

extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime(void);

// 68K 0x1011014a CurrentMilliTimer__Fv
// FUNCTION: LEMBALL 0x00462e80
unsigned long CurrentMilliTimer()
{
	return timeGetTime();
}

// 68K 0x101101a6 CurrentQueueTimer__Fv
// FUNCTION: LEMBALL 0x00462e90
unsigned long CurrentQueueTimer()
{
	return timeGetTime();
}
