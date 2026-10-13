#ifndef LEMBALL_VISOS_TARGET_GRAPHICS_WINGDRAW_H
#define LEMBALL_VISOS_TARGET_GRAPHICS_WINGDRAW_H

#include "Platform/Windows/WindowsTypes.h"

long __stdcall WinGDrawDriverProc(unsigned long p_driverId,
								  void* p_driverHandle,
								  unsigned int p_message,
								  long p_param1,
								  long p_param2);

extern RGBQUAD g_dwWinGDrawColourTable[256];

#endif
