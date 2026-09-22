#include "Visos/Foundation/CVSDebugStreambuf.h"
#include "Visos/Foundation/CVSOStream.h"
#include "Visos/Foundation/VsDebug.h"
#include "Visos/Foundation/VsInit.h"

#include <new.h>

// GLOBAL: LEMBALL 0x004a93b0
char g_szStreamFixedBuffer[0x400];

// FUNCTION: LEMBALL 0x00458f70
bool _STRM_Init()
{
	void* storage;

	storage = operator new(0x1c);
	if (storage != 0) {
		g_pDebugStreambuf =
			new (storage) CVSDebugStreambuf(g_szStreamFixedBuffer, 0x400, (void (*)(char*)) _RAWOUT_DebugString);
	}
	else {
		g_pDebugStreambuf = 0;
	}

	storage = operator new(0x1c);
	if (storage != 0) {
		g_pSysStreambuf =
			new (storage) CVSDebugStreambuf(g_szStreamFixedBuffer, 0x400, (void (*)(char*)) _RAWOUT_SysString);
	}
	else {
		g_pSysStreambuf = 0;
	}

	storage = operator new(0x1c);
	if (storage != 0) {
		g_pErrorStreambuf =
			new (storage) CVSDebugStreambuf(g_szStreamFixedBuffer, 0x400, (void (*)(char*)) _RAWOUT_ErrorString);
	}
	else {
		g_pErrorStreambuf = 0;
	}

	storage = operator new(0x14c);
	if (storage != 0) {
		g_pDebugOutput = new (storage) CVSOStream(g_pDebugStreambuf);
	}
	else {
		g_pDebugOutput = 0;
	}

	storage = operator new(0x14c);
	if (storage != 0) {
		g_pSysOutput = new (storage) CVSOStream(g_pSysStreambuf);
	}
	else {
		g_pSysOutput = 0;
	}

	storage = operator new(0x14c);
	if (storage != 0) {
		g_pErrorOutput = new (storage) CVSOStream(g_pErrorStreambuf);
	}
	else {
		g_pErrorOutput = 0;
	}

	return 1;
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
	return 1;
}
