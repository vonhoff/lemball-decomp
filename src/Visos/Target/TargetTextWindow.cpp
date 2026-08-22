#include "TargetTextWindow.h"

// STUB: LEMBALL 0x00473d90
void TargetTextWindow::PostAllocatedTextControlString(const char* p_text, unsigned int p_color)
{
}

// Confirmed class-scoped globals.
// GLOBAL: LEMBALL 0x004a29f0
TargetTextWindow* g_pDebugWindow;

// GLOBAL: LEMBALL 0x004a2b68
int g_nTargetTextWindowClassRegistered;

// GLOBAL: LEMBALL 0x004a2b7c
int g_nTargetTextWindowActive;

// GLOBAL: LEMBALL 0x004a2b8c
int g_nTargetTextWindowCreated;
