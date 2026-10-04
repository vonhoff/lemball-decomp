#ifndef LEMBALL_VISOS_TARGET_GRAPHICS_WINGDRAW_H
#define LEMBALL_VISOS_TARGET_GRAPHICS_WINGDRAW_H

long __stdcall WinGDrawDriverProc(unsigned int p_driverId,
								  void* p_driverHandle,
								  unsigned int p_message,
								  long p_param1,
								  long p_param2);

#endif
