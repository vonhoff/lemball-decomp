#ifndef LEMBALL_PLATFORM_WINDOWS_THREAD_H
#define LEMBALL_PLATFORM_WINDOWS_THREAD_H

extern "C" __declspec(dllimport) void* __stdcall CreateThread(void* p_security,
															  unsigned int p_stack,
															  unsigned int(__stdcall* p_start)(void*),
															  void* p_param,
															  unsigned int p_flags,
															  unsigned int* p_id);
extern "C" __declspec(dllimport) int __stdcall SetThreadPriority(void* p_thread, int p_priority);
extern "C" __declspec(dllimport) void* __stdcall CreateEventA(void* p_security,
															  int p_manual,
															  int p_initial,
															  const char* p_name);
extern "C" __declspec(dllimport) unsigned int __stdcall WaitForSingleObject(void* p_handle, unsigned int p_ms);
extern "C" __declspec(dllimport) int __stdcall TerminateThread(void* p_thread, unsigned int p_exit);

#endif
