#include "Platform/Windows/VsTime.h"

#include "WindowsTypes.h"

#include <mmsystem.h>

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
