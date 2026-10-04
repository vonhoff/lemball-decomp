#include "CPVAnimWnd.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// FUNCTION: LEMBALL 0x0046e3f0
unsigned int CPVAnimWnd::GetStyle()
{
	return WS_CHILD;
}
