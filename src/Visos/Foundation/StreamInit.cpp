#include "Visos/Foundation/CVSDebugStreambuf.h"
#include "Visos/Foundation/CVSOStream.h"
#include "Visos/Foundation/VsDebug.h"
#include "Visos/Foundation/VsInit.h"

#include <new.h>
#include <stddef.h>

// GLOBAL: LEMBALL 0x004a93b0
char g_szStreamFixedBuffer[0x400];

// FUNCTION: LEMBALL 0x00458f70
bool _STRM_Init()
{
	void* storage;

	storage = operator new(sizeof(CVSDebugStreambuf));
	if (storage != NULL) {
		g_pDebugStreambuf =
			new (storage) CVSDebugStreambuf(g_szStreamFixedBuffer, sizeof(g_szStreamFixedBuffer), _RAWOUT_DebugString);
	}
	else {
		g_pDebugStreambuf = NULL;
	}

	storage = operator new(sizeof(CVSDebugStreambuf));
	if (storage != NULL) {
		g_pSysStreambuf =
			new (storage) CVSDebugStreambuf(g_szStreamFixedBuffer, sizeof(g_szStreamFixedBuffer), _RAWOUT_SysString);
	}
	else {
		g_pSysStreambuf = NULL;
	}

	storage = operator new(sizeof(CVSDebugStreambuf));
	if (storage != NULL) {
		g_pErrorStreambuf =
			new (storage) CVSDebugStreambuf(g_szStreamFixedBuffer, sizeof(g_szStreamFixedBuffer), _RAWOUT_ErrorString);
	}
	else {
		g_pErrorStreambuf = NULL;
	}

	storage = operator new(sizeof(CVSOStream));
	if (storage != NULL) {
		g_pDebugOutput = new (storage) CVSOStream(g_pDebugStreambuf);
	}
	else {
		g_pDebugOutput = NULL;
	}

	storage = operator new(sizeof(CVSOStream));
	if (storage != NULL) {
		g_pSysOutput = new (storage) CVSOStream(g_pSysStreambuf);
	}
	else {
		g_pSysOutput = NULL;
	}

	storage = operator new(sizeof(CVSOStream));
	if (storage != NULL) {
		g_pErrorOutput = new (storage) CVSOStream(g_pErrorStreambuf);
	}
	else {
		g_pErrorOutput = NULL;
	}

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
