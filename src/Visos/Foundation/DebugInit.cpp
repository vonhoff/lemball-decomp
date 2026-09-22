#include "Platform/Windows/Thread.h"
#include "Visos/Foundation/CVsDebugStreambuf.h"
#include "Visos/Foundation/VsFile.h"
#include "Visos/Foundation/VsInit.h"

extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int p_code);
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void* p_window,
														   const char* p_text,
														   const char* p_caption,
														   unsigned int p_type);

unsigned int __cdecl DebugMessageThreadMain();

// GLOBAL: LEMBALL 0x004a82e0
jmp_buf g_vsDebugJumpBuffer;

// FUNCTION: LEMBALL 0x00472be0
bool InternalDbgInit()
{
	if (g_nAsyncDebugEnabled == 1) {
		g_pDebugSyncEvent = CreateEventA(0, 0, 0, "Sync_Debug");
		g_pDebugThread = CreateThread(0,
									  0,
									  (unsigned int(__stdcall*)(void*)) DebugMessageThreadMain,
									  0,
									  0,
									  (unsigned int*) &g_nDebugThreadId);
		if (g_pDebugThread == 0) {
			MessageBoxA(0, "Unable to start 'Debug Message loop' thread\n", "ERROR", 0);
			ExitProcess(0xbbbb);
		}

		SetThreadPriority(g_pDebugThread, 1);
		WaitForSingleObject(g_pDebugSyncEvent, 0xffffffff);
	}

	return 1;
}

// FUNCTION: LEMBALL 0x00472c70
bool InternalDbgQuit(unsigned int p_force)
{
	if (g_nAsyncDebugEnabled == 1) {
		if (p_force == 0) {
			WaitForSingleObject(g_pDebugSyncEvent, 0xffffffff);
		}
		else {
			TerminateThread(g_pDebugThread, 0xaaaa);
		}
		g_nAsyncDebugEnabled = 0;
		return 1;
	}
	if (g_pDebugOutputFile != 0) {
		VsClose((_Filet*) g_pDebugOutputFile);
		g_pDebugOutputFile = 0;
	}
	return 1;
}
