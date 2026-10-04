#include "CDibContext.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// FUNCTION: LEMBALL 0x004582b0
bool CDibContext::Lock()
{
	return true;
}

// FUNCTION: LEMBALL 0x004582c0
bool CDibContext::Unlock()
{
	return true;
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
