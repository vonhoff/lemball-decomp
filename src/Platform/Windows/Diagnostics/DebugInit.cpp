#include "DebugInit.h"

#include <setjmp.h>
#include <stddef.h>

#include "Platform/Windows/Diagnostics/VsDebug.h"
#include "Engine/Files/VsFile.h"
#include "Engine/Startup/ProcessExitCodes.h"
#include "Platform/Windows/WindowsTypes.h"

struct _Filet;

unsigned long __cdecl DebugMessageThreadMain();

// GLOBAL: LEMBALL 0x004a82e0
jmp_buf g_vsDebugJumpBuffer;

// FUNCTION: LEMBALL 0x00472be0
bool _DBG_Init()
{
	if (g_nAsyncDebugEnabled == 1) {
		g_pDebugSyncEvent = CreateEventA(NULL, 0, 0, "Sync_Debug");
		g_pDebugThread =
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DebugMessageThreadMain, NULL, 0, &g_nDebugThreadId);
		if (g_pDebugThread == NULL) {
			MessageBoxA(NULL, "Unable to start 'Debug Message loop' thread\n", "ERROR", 0);
			ExitProcess(VISOS_THREAD_START_FAILURE_EXIT_CODE);
		}

		SetThreadPriority(g_pDebugThread, 1);
		WaitForSingleObject(g_pDebugSyncEvent, INFINITE);
	}

	return true;
}

// FUNCTION: LEMBALL 0x00472c70
bool _DBG_Quit(unsigned int p_force)
{
	if (g_nAsyncDebugEnabled == 1) {
		if (p_force == 0) {
			WaitForSingleObject(g_pDebugSyncEvent, INFINITE);
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
