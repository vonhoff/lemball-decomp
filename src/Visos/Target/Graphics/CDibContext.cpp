#include "CDibContext.h"

#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// FUNCTION: LEMBALL 0x004582b0
bool CDibContext::Lock()
{
	return 1;
}

// FUNCTION: LEMBALL 0x004582c0
bool CDibContext::Unlock()
{
	return 1;
}

// FUNCTION: LEMBALL 0x004582d0
unsigned char* CDibContext::GetBits()
{
	return m_bits;
}

// FUNCTION: LEMBALL 0x004582e0
int CDibContext::GetStride()
{
	return m_width;
}
