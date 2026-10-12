#include "CVSOStream.h"
#include "Engine/Diagnostics/CVSDebugStreambuf.h"
#include "Engine/Diagnostics/VsDebug.h"
#include "Engine/Startup/VsInit.h"

#include <stddef.h>

// GLOBAL: LEMBALL 0x004a93b0
char g_szStreamFixedBuffer[0x400];

// FUNCTION: LEMBALL 0x00458f70
bool _STRM_Init()
{
	g_pDebugStreambuf =
		new CVSDebugStreambuf(g_szStreamFixedBuffer, sizeof(g_szStreamFixedBuffer), _RAWOUT_DebugString);
	g_pSysStreambuf = new CVSDebugStreambuf(g_szStreamFixedBuffer, sizeof(g_szStreamFixedBuffer), _RAWOUT_SysString);
	g_pErrorStreambuf =
		new CVSDebugStreambuf(g_szStreamFixedBuffer, sizeof(g_szStreamFixedBuffer), _RAWOUT_ErrorString);

	g_pDebugOutput = new CVSOStream(g_pDebugStreambuf);
	g_pSysOutput = new CVSOStream(g_pSysStreambuf);
	g_pErrorOutput = new CVSOStream(g_pErrorStreambuf);

	return true;
}

// FUNCTION: LEMBALL 0x004590b0
bool _STRM_Quit()
{
	delete g_pErrorOutput;
	delete g_pSysOutput;
	delete g_pDebugOutput;
	delete g_pErrorStreambuf;
	delete g_pSysStreambuf;
	delete g_pDebugStreambuf;
	return true;
}
