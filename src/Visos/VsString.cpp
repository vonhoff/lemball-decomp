#include "VsString.h"

// 68K 0x107001ea StrCmpI__FPCcPCci
// STUB: LEMBALL 0x00406790
int StrCmpI(const char* p_left, const char* p_right, int p_maxLength)
{
	return 0;
}

// 68K 0x1021429c vsLtoa__FlPci
// STUB: LEMBALL 0x00458d80
void VsLtoa(long p_arg0, char* p_arg1, int p_arg2)
{
}

// 68K 0x102142e8 vsULtoa__FUlPci
// STUB: LEMBALL 0x00458db0
char* VsULtoa(unsigned long p_arg0, char* p_arg1, int p_arg2)
{
	// STRING: LEMBALL 0x004a0e40 "0123456789abcdef"
	return 0;
}

// 68K 0x10212fae OkFailed__FUc
// STUB: LEMBALL 0x00458f10
char* OkFailed(unsigned char p_arg0)
{
	// STRING: LEMBALL 0x004a0fac "Failed"
	return 0;
}

// 68K 0x10212fda strtol__FPcPPci
// STUB: LEMBALL 0x00458f30
int Strtol(char* p_arg0, char** p_arg1, int p_arg2)
{
	return 0;
}
