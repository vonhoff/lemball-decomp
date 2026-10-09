#ifndef LEMBALL_PLATFORM_WINDOWS_THREAD_H
#define LEMBALL_PLATFORM_WINDOWS_THREAD_H

enum eThreadWaitTimeout {
	THREAD_WAIT_INFINITE = 0xffffffff
};

extern "C" __declspec(dllimport) void* __stdcall CreateThread(void* p_security,
															  unsigned long p_stack,
															  unsigned long(__stdcall* p_start)(void*),
															  void* p_param,
															  unsigned long p_flags,
															  unsigned long* p_id);
extern "C" __declspec(dllimport) int __stdcall SetThreadPriority(void* p_thread, int p_priority);
extern "C" __declspec(dllimport) void* __stdcall CreateEventA(void* p_security,
															  int p_manual,
															  int p_initial,
															  const char* p_name);
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void* p_handle, unsigned long p_ms);
extern "C" __declspec(dllimport) int __stdcall TerminateThread(void* p_thread, unsigned long p_exit);

#endif
