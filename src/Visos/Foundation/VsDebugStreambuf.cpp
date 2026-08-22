#include "VsDebugStreambuf.h"

// 68K 0x102143c6 __ct__17CVSDebugStreambufFPciPFPc_Uc
// STUB: LEMBALL 0x0045ad70
VsDebugStreambuf::VsDebugStreambuf(char* p_arg0,
								   int p_arg1,
								   undefined4* p_arg2,
								   char* p_arg3,
								   undefined4 p_arg4,
								   unsigned char p_arg5)
{
}

// 68K 0x1021449e flush__17CVSDebugStreambufFv
// STUB: LEMBALL 0x0045add0
void VsDebugStreambuf::Flush()
{
}

// 68K 0x10214504 sputc__17CVSDebugStreambufFc
// STUB: LEMBALL 0x0045ae10
int VsDebugStreambuf::Sputc(char p_arg0)
{
	return 0;
}

// 68K 0x1021466a sputs__17CVSDebugStreambufFPc
// STUB: LEMBALL 0x0045af20
void VsDebugStreambuf::Sputs(char* p_arg0)
{
}

// 68K 0x10214448 __dt__17CVSDebugStreambufFv
// SYNTHETIC: LEMBALL 0x0045af60
// VsDebugStreambuf::`scalar deleting destructor'
VsDebugStreambuf::~VsDebugStreambuf()
{
}

// Confirmed class-scoped globals.
// GLOBAL: LEMBALL 0x004a97b4
VsDebugStreambuf* g_pDebugStreambuf;

// GLOBAL: LEMBALL 0x004a97b0
VsDebugStreambuf* g_pSysStreambuf;

// GLOBAL: LEMBALL 0x004a97c0
VsDebugStreambuf* g_pErrorStreambuf;
