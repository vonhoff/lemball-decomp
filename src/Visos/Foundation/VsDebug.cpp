#include "VsDebug.h"

#include "../Target/TargetTextWindow.h"
#include "String.h"
#include "VsDebugStreambuf.h"
#include "VsFile.h"
#include "VsInit.h"
#include "VsString.h"

#include <setjmp.h>
#include <string.h>

extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void* hWnd,
														   const char* lpText,
														   const char* lpCaption,
														   unsigned int uType);
extern "C" __declspec(dllimport) unsigned int __stdcall GetLastError();
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int p_code);

#pragma intrinsic(strlen)

// 68K 0x10213c3a _VSExit__Fi
// FUNCTION: LEMBALL 0x00459970
void VsExit(int p_exitCode)
{
	longjmp(g_vsExitJumpBuffer, p_exitCode);
}

// 68K 0x10205a70 _VSRELassert
// FUNCTION: LEMBALL 0x0045b8e0
void VsRelAssert(const char* p_reason, const char* p_file, unsigned int p_line)
{
	DisplayRelAssert((void*) p_reason, (void*) p_file, p_line);
}

// 68K 0x1010ff88 WriteDebugString2File__FPc
// FUNCTION: LEMBALL 0x004728b0
void WriteDebugString2File(char* p_text)
{
	if (g_pDebugOutputPath != NULL) {
		if (strlen(p_text) != 0) {
			g_pDebugOutputFile = (FILE*) VsOpen(g_pDebugOutputPath, "a");
			VsWrite((_Filet*) g_pDebugOutputFile, (void*) p_text, strlen(p_text));
			VsClose((_Filet*) g_pDebugOutputFile);
		}
	}
}

// 68K 0x1011006c _RAWOUT_DebugString__FPc
// FUNCTION: LEMBALL 0x00472910
int RawOutDebugString(char* p_text)
{
	if (g_nDebugInitialized == 0) {
		MessageBoxA(NULL, p_text, "_RAWOUT_DebugString", 0);
		return 1;
	}
	if (g_pDebugWindow != NULL) {
		g_pDebugWindow->PostAllocatedTextControlString(p_text, 0x8000);
	}
	else if (g_nDebugFileOutputEnabled != 0) {
		WriteDebugString2File(p_text);
	}
	return g_pDebugWindow != NULL;
}

// 68K 0x101100a4 _RAWOUT_ErrorString__FPc
// FUNCTION: LEMBALL 0x00472980
int RawOutErrorString(char* p_text)
{
	if (g_nDebugInitialized == 0) {
		MessageBoxA(NULL, p_text, "_RAWOUT_ErrorString", 0);
		return 1;
	}
	if (g_pDebugWindow != NULL) {
		g_pDebugWindow->PostAllocatedTextControlString(p_text, 0xff);
	}
	else if (g_nDebugFileOutputEnabled != 0) {
		WriteDebugString2File(p_text);
	}
	return g_pDebugWindow != NULL;
}

// 68K 0x101100dc _RAWOUT_SysString__FPc
// FUNCTION: LEMBALL 0x004729f0
int RawOutSysString(char* p_text)
{
	if (g_nDebugInitialized == 0) {
		MessageBoxA(NULL, p_text, "_RAWOUT_SysString", 0);
		return 1;
	}
	if (g_pDebugWindow != NULL) {
		g_pDebugWindow->PostAllocatedTextControlString(p_text, 0xff0000);
	}
	else if (g_nDebugFileOutputEnabled != 0) {
		WriteDebugString2File(p_text);
	}
	return g_pDebugWindow != NULL;
}

// 68K 0x10100d4a DisplayRelAssert__FPvPvUi
// FUNCTION: LEMBALL 0x004734f0
void DisplayRelAssert(void* p_reason, void* p_file, unsigned int p_line)
{
	String msg;
	msg = "Release Version Assertion Failure\n";
	msg += "Reason: ";
	msg += (char*) p_reason;
	msg += "\n";
	msg += "In File: ";
	msg += (char*) p_file;
	msg += "At Line No.: ";
	char lineBuf[16];
	VsLtoa(p_line, lineBuf, 10);
	msg += lineBuf;
	MessageBoxA(NULL, msg.GetText(), "Error", 0);
	VsExit(0xaaaa);
}

#include "VsOStream.h"

class LocalDebugOStream : public VsDebugStreambuf, public VsOStream {
public:
	LocalDebugOStream(char* p_buffer, int p_size) : VsDebugStreambuf(p_buffer, p_size, 0), VsOStream(this) {}
};

// FUNCTION: LEMBALL 0x00473790
void FatalWin32Error(char* p_context)
{
	unsigned long error = GetLastError();
	char buffer[0x80];
	{
		LocalDebugOStream stream(buffer, sizeof(buffer));
		stream << p_context << '\n' << " GetLastError()=" << (long) error << ", " << Hex8(error);
	}
	MessageBoxA(0, buffer, "FATAL ERROR", 0);
	ExitProcess(0xaaaa);
}
