#include "VsDebug.h"

#include "../Target/UI/CTextWnd.h"
#include "CDebugOStream.h"
#include "CString.h"
#include "CVSDebugStreambuf.h"
#include "ProcessExitCodes.h"
#include "Visos/Foundation/CVSOStream.h"
#include "VsFile.h"
#include "VsInit.h"
#include "VsString.h"

#include <setjmp.h>
#include <string.h>

namespace
{
enum {
	DEBUG_WINDOW_DEBUG_TEXT_COLOUR = 0x8000,
	DEBUG_WINDOW_ERROR_TEXT_COLOUR = 0xff,
	DEBUG_WINDOW_SYSTEM_TEXT_COLOUR = 0xff0000,
};
}

struct FILE;
struct _Filet;

extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void* p_hWnd,
														   const char* p_lpText,
														   const char* p_lpCaption,
														   unsigned int p_uType);
extern "C" __declspec(dllimport) unsigned int __stdcall GetLastError();
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int p_code);

#pragma intrinsic(strlen)

// FUNCTION: LEMBALL 0x00459970
void _VSExit(int p_exitCode)
{
	longjmp(g_vsExitJumpBuffer, p_exitCode);
}

// FUNCTION: LEMBALL 0x0045b8e0
void _VSRELassert(const char* p_reason, const char* p_file, unsigned int p_line)
{
	DisplayRelAssert((void*) p_reason, (void*) p_file, p_line);
}

// FUNCTION: LEMBALL 0x004728b0
void WriteDebugString2File(char* p_text)
{
	if (g_pDebugOutputPath != NULL) {
		if (strlen(p_text) != 0) {
			g_pDebugOutputFile = (FILE*) vsOpen(g_pDebugOutputPath, "a");
			vsWrite((_Filet*) g_pDebugOutputFile, (void*) p_text, strlen(p_text));
			vsClose((_Filet*) g_pDebugOutputFile);
		}
	}
}

// FUNCTION: LEMBALL 0x00472910
int _RAWOUT_DebugString(char* p_text)
{
	if (g_nDebugInitialised == 0) {
		MessageBoxA(NULL, p_text, "_RAWOUT_DebugString", 0);
		return 1;
	}
	if (g_pDebugWindow != NULL) {
		g_pDebugWindow->PostText(p_text, DEBUG_WINDOW_DEBUG_TEXT_COLOUR);
	}
	else if (g_nDebugFileOutputEnabled != 0) {
		WriteDebugString2File(p_text);
	}
	return g_pDebugWindow != NULL;
}

// FUNCTION: LEMBALL 0x00472980
int _RAWOUT_ErrorString(char* p_text)
{
	if (g_nDebugInitialised == 0) {
		MessageBoxA(NULL, p_text, "_RAWOUT_ErrorString", 0);
		return 1;
	}
	if (g_pDebugWindow != NULL) {
		g_pDebugWindow->PostText(p_text, DEBUG_WINDOW_ERROR_TEXT_COLOUR);
	}
	else if (g_nDebugFileOutputEnabled != 0) {
		WriteDebugString2File(p_text);
	}
	return g_pDebugWindow != NULL;
}

// FUNCTION: LEMBALL 0x004729f0
int _RAWOUT_SysString(char* p_text)
{
	if (g_nDebugInitialised == 0) {
		MessageBoxA(NULL, p_text, "_RAWOUT_SysString", 0);
		return 1;
	}
	if (g_pDebugWindow != NULL) {
		g_pDebugWindow->PostText(p_text, DEBUG_WINDOW_SYSTEM_TEXT_COLOUR);
	}
	else if (g_nDebugFileOutputEnabled != 0) {
		WriteDebugString2File(p_text);
	}
	return g_pDebugWindow != NULL;
}

// FUNCTION: LEMBALL 0x004734f0
void DisplayRelAssert(void* p_reason, void* p_file, unsigned int p_line)
{
	CString msg;
	msg = "Release Version Assertion Failure\n";
	msg += "Reason: ";
	msg += (char*) p_reason;
	msg += "\n";
	msg += "In File: ";
	msg += (char*) p_file;
	msg += "At Line No.: ";
	char lineBuf[16];
	vsLtoa(p_line, lineBuf, 10);
	msg += lineBuf;
	MessageBoxA(NULL, msg, "Error", 0);
	_VSExit(VISOS_FATAL_EXIT_CODE);
}

// FUNCTION: LEMBALL 0x00473790
void FatalWin32Error(char* p_context)
{
	unsigned long error = GetLastError();
	char buffer[0x80];
	{
		CDebugOStream stream(buffer, sizeof(buffer));
		stream << p_context << '\n' << " GetLastError()=" << (long) error << ", " << Hex8(error);
	}
	MessageBoxA(NULL, buffer, "FATAL ERROR", 0);
	ExitProcess(VISOS_FATAL_EXIT_CODE);
}
