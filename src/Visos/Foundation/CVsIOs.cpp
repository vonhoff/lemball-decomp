#include "CVsIOs.h"

// FUNCTION: LEMBALL 0x00458410
CVsIOs::CVsIOs(CVsStreambuf* p_streamBuffer)
	: m_streamBuffer(p_streamBuffer), m_flags(0x14), m_fill(' '), m_width(0), m_radix(10)
{
}

// FUNCTION: LEMBALL 0x00458440
CVsIOs::~CVsIOs()
{
}

// GLOBAL: LEMBALL 0x004a8088
char g_szCurrentDirectory[256];

// GLOBAL: LEMBALL 0x004a9ff8
char g_szSourceDiskPath[256];
