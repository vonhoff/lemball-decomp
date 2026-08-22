#include "VsIOs.h"

VsIOs::VsIOs()
{
}

// 68K 0x10213d20 __dt__6CVSIOsFv
// SYNTHETIC: LEMBALL 0x00407e10
// VsIOs::`scalar deleting destructor'
VsIOs::~VsIOs()
{
}

// 68K 0x10213cca __ct__6CVSIOsFP12CVSStreambuf
// FUNCTION: LEMBALL 0x00458410
VsIOs::VsIOs(VsStreambuf* p_arg0)
{
	m_fill = ' ';
	m_flags = 0x14;
	m_width = 0;
	m_radix = 10;
	m_streamBuffer = p_arg0;
}

// Confirmed class-scoped globals.
// GLOBAL: LEMBALL 0x004a8088
char g_szCurrentDirectory[256];

// GLOBAL: LEMBALL 0x004a9ff8
char g_szSourceDiskPath[256];
