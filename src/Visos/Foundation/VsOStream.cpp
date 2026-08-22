#include "VsOStream.h"

// 68K 0x10213d5e __ct__10CVSOStreamFP12CVSStreambuf
// STUB: LEMBALL 0x00458450
VsOStream::VsOStream(VsStreambuf* p_arg0)
{
}

// 68K 0x10213dc6 __dt__10CVSOStreamFv
// STUB: LEMBALL 0x004584a0
VsOStream::~VsOStream()
{
}

// 68K 0x10213e2a _FormatNum__10CVSOStreamFv
// STUB: LEMBALL 0x004584c0
void VsOStream::FormatNum()
{
}

// 68K 0x10213f2c __ls__10CVSOStreamFPCc
// STUB: LEMBALL 0x004585b0
VsOStream& VsOStream::operator<<(const char* p_arg0)
{
	return *(VsOStream*) 0;
}

// 68K 0x10213f6a __ls__10CVSOStreamFc
// STUB: LEMBALL 0x004585d0
VsOStream& VsOStream::operator<<(char p_arg0)
{
	return *(VsOStream*) 0;
}

// 68K 0x10213fa6 __ls__10CVSOStreamFl
// STUB: LEMBALL 0x004585f0
VsOStream& VsOStream::operator<<(long p_arg0)
{
	return *(VsOStream*) 0;
}

// 68K 0x10213ff6 __ls__10CVSOStreamFi
// STUB: LEMBALL 0x00458630
VsOStream& VsOStream::operator<<(int p_arg0)
{
	return *(VsOStream*) 0;
}

// 68K 0x10214046 __ls__10CVSOStreamFPCv
// STUB: LEMBALL 0x00458670
VsOStream& VsOStream::operator<<(const void* p_arg0)
{
	// STRING: LEMBALL 0x004a0e58 "(null)"
	return *(VsOStream*) 0;
}

// 68K 0x102140ba __ls__10CVSOStreamFUl
// STUB: LEMBALL 0x004586e0
VsOStream& VsOStream::operator<<(unsigned long p_arg0)
{
	return *(VsOStream*) 0;
}

// 68K 0x1021410a __ls__10CVSOStreamF3HEX
// STUB: LEMBALL 0x00458780
VsOStream& VsOStream::operator<<(Hex p_arg0)
{
	return *(VsOStream*) 0;
}

// 68K 0x10214186 __ls__10CVSOStreamF4HEX8
// STUB: LEMBALL 0x004589c0
VsOStream& VsOStream::operator<<(Hex8 p_arg0)
{
	return *(VsOStream*) 0;
}

// 68K 0x10214240 __ls__10CVSOStreamF5CHAR4
// STUB: LEMBALL 0x00458d40
VsOStream& VsOStream::operator<<(Har4 p_arg0)
{
	return *(VsOStream*) 0;
}

// 68K 0x10201618 __ls__FR10CVSOStream5RNAME
// STUB: LEMBALL 0x0045bad0
VsOStream* operator<<(VsOStream& p_arg0, Rname p_arg1)
{
	return 0;
}

// 68K 0x1020123e __ls__FR10CVSOStreamR7CString
// STUB: LEMBALL 0x0046e7f0
VsOStream* operator<<(VsOStream& p_arg0, String& p_arg1)
{
	return 0;
}

// Confirmed class-scoped globals.
// GLOBAL: LEMBALL 0x004a93a8
VsOStream* g_pErrorOutput;

// GLOBAL: LEMBALL 0x004a97b8
VsOStream* g_pSysOutput;
