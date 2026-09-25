#include "DirectDrawError.h"

#include "../../Foundation/VsString.h"

#include <string.h>

// GLOBAL: LEMBALL 0x004a0aa8
static const char* g_directDrawErrorPrefix = "UNKNOWN DIRECT DRAW ERROR: ";

// GLOBAL: LEMBALL 0x004a818c
unsigned int g_dwWindowQuitRequested = 0;

// GLOBAL: LEMBALL 0x004a8190
static char g_directDrawErrorText[0x100] = {0};

// FUNCTION: LEMBALL 0x00456720
char* FormatUnknownDirectDrawError(long p_result)
{
	strcpy(g_directDrawErrorText, g_directDrawErrorPrefix);
	vsLtoa(p_result, g_directDrawErrorText + strlen(g_directDrawErrorPrefix), 10);
	return g_directDrawErrorText;
}

// FUNCTION: LEMBALL 0x00456780
void OnDirectDrawCreateFailure(int p_context, long p_result)
{
}
