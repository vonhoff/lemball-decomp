#ifndef LEMBALL_PLATFORM_WINDOWS_DIAGNOSTICS_DEBUGINIT_H
#define LEMBALL_PLATFORM_WINDOWS_DIAGNOSTICS_DEBUGINIT_H

#include <setjmp.h>

extern jmp_buf g_vsDebugJumpBuffer;

bool _DBG_Init();
bool _DBG_Quit(unsigned int p_force);

#endif
