#include "TargetDirectDrawError.h"

#include "../Foundation/VsString.h"

#include <string.h>

// GLOBAL: LEMBALL 0x004a0aa8
static const char* g_directDrawErrorPrefix = "UNKNOWN DIRECT DRAW ERROR: ";

// The next original graphics global is at 0x004a8290; this reserves the
// intervening scratch region. Its declared array bound is not recovered.
// GLOBAL: LEMBALL 0x004a8190
static char g_directDrawErrorText[0x100];

// FUNCTION: LEMBALL 0x00456720
char* FormatUnknownDirectDrawError(long p_result)
{
	strcpy(g_directDrawErrorText, g_directDrawErrorPrefix);
	VsLtoa(p_result, g_directDrawErrorText + strlen(g_directDrawErrorPrefix), 10);
	return g_directDrawErrorText;
}
