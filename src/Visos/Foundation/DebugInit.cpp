#include "Platform/Windows/Thread.h"
#include "Visos/Foundation/CVSDebugStreambuf.h"
#include "Visos/Foundation/ProcessExitCodes.h"
#include "Visos/Foundation/VsFile.h"
#include "Visos/Foundation/VsInit.h"

#include <stddef.h>

extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int p_code);
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void* p_window,
														   const char* p_text,
														   const char* p_caption,
														   unsigned int p_type);

unsigned int __cdecl DebugMessageThreadMain();

// GLOBAL: LEMBALL 0x004a82e0
jmp_buf g_vsDebugJumpBuffer;

// FUNCTION: LEMBALL 0x00472be0
bool _DBG_Init()
{
	if (g_nAsyncDebugEnabled == 1) {
		g_pDebugSyncEvent = CreateEventA(NULL, 0, 0, "Sync_Debug");
		g_pDebugThread = CreateThread(NULL,
									  0,
									  (unsigned int(__stdcall*)(void*)) DebugMessageThreadMain,
									  NULL,
									  0,
									  (unsigned int*) &g_nDebugThreadId);
		if (g_pDebugThread == NULL) {
			MessageBoxA(NULL, "Unable to start 'Debug Message loop' thread\n", "ERROR", 0);
			ExitProcess(VISOS_THREAD_START_FAILURE_EXIT_CODE);
		}

		SetThreadPriority(g_pDebugThread, 1);
		WaitForSingleObject(g_pDebugSyncEvent, THREAD_WAIT_INFINITE);
	}

	return true;
}

// FUNCTION: LEMBALL 0x00472c70
bool _DBG_Quit(unsigned int p_force)
{
	if (g_nAsyncDebugEnabled == 1) {
		if (p_force == 0) {
			WaitForSingleObject(g_pDebugSyncEvent, THREAD_WAIT_INFINITE);
		}
		else {
			TerminateThread(g_pDebugThread, VISOS_FATAL_EXIT_CODE);
		}
		g_nAsyncDebugEnabled = 0;
		return true;
	}
	if (g_pDebugOutputFile != NULL) {
		vsClose((_Filet*) g_pDebugOutputFile);
		g_pDebugOutputFile = NULL;
	}
	return true;
}
