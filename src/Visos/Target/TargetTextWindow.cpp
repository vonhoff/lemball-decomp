#include "TargetTextWindow.h"

#include "../Foundation/VsDebug.h"

#include <stdlib.h>
#include <string.h>

extern "C" __declspec(dllimport) int __stdcall PostMessageA(void*, unsigned int, unsigned int, long);

// GLOBAL: LEMBALL 0x004a2c40
static char g_unableToAllocateTextCopy[] = "Unable to allocate memory for string copy";

// FUNCTION: LEMBALL 0x00473d90
void TargetTextWindow::PostAllocatedTextControlString(const char* p_text, unsigned int p_color)
{
	EnterCritical();
	if (g_nTargetTextWindowActive != 0) {
		char* copy = (char*) malloc(strlen(p_text) + 1);
		if (copy == 0) {
			FatalWin32Error(g_unableToAllocateTextCopy);
		}
		strcpy(copy, p_text);
		if (m_windowHandle != 0) {
			PostMessageA(m_windowHandle, 0x420, (unsigned int) copy, p_color);
		}
		LeaveCritical();
	}
}

// GLOBAL: LEMBALL 0x004a44d8 SYMBOL
// __locktable

// GLOBAL: LEMBALL 0x004a5038 SYMBOL
// __newmode

// GLOBAL: LEMBALL 0x004aa494 SYMBOL
// ?_pnhHeap@@3P6AHI@ZA

// GLOBAL: LEMBALL 0x004ab7e0 SYMBOL
// __crtheap

// GLOBAL: LEMBALL 0x004a29f0
TargetTextWindow* g_pDebugWindow = 0;

// GLOBAL: LEMBALL 0x004a2b68
int g_nTargetTextWindowClassRegistered = 0;

// GLOBAL: LEMBALL 0x004a2b7c
int g_nTargetTextWindowActive = 0;

// GLOBAL: LEMBALL 0x004a2b8c
int g_nTargetTextWindowCreated = 0;
