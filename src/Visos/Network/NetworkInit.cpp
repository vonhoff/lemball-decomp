#include "Platform/Windows/Thread.h"
#include "Visos/Foundation/CBaseQueue.h"
#include "Visos/Foundation/CBaseQueueHandler.h"
#include "Visos/Foundation/CVSOStream.h"
#include "Visos/Foundation/VsInit.h"
#include "Visos/Network/CBaseNetwork.h"
#include "Visos/Network/CFileNetwork.h"
#include "Visos/Network/CTCPIPNetwork.h"

#include <new.h>

extern "C" unsigned long __stdcall timeGetTime(void);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int p_code);
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void* p_window,
														   const char* p_text,
														   const char* p_caption,
														   unsigned int p_type);
extern "C" __declspec(dllimport) int __stdcall WaitMessage();
extern "C" __declspec(dllimport) int __stdcall GetMessageA(void* p_message,
														   void* p_window,
														   unsigned int p_min,
														   unsigned int p_max);
extern "C" __declspec(dllimport) int __stdcall TranslateMessage(const void* p_message);
extern "C" __declspec(dllimport) int __stdcall PeekMessageA(void* p_message,
															void* p_window,
															unsigned int p_min,
															unsigned int p_max,
															unsigned int p_remove);
extern "C" __declspec(dllimport) long __stdcall DispatchMessageA(const void* p_message);


// FUNCTION: LEMBALL 0x0046f210
unsigned int FileNetworkMessageThread()
{
	unsigned int message[7];
	unsigned int count;

	g_pBaseNetwork = new CFileNetwork();
	while (g_pBaseNetwork->m_initialisePending == 0 && g_pBaseNetwork->m_serverMode == 0 &&
		   g_pBaseNetwork->m_shutdownRequested == 0) {
		WaitMessage();
		GetMessageA(message, 0, 0, 0);
	}
	if (g_pBaseNetwork->m_initialisePending != 0) {
		g_pBaseNetwork->DoInitialise();
	}
	while (g_pBaseNetwork->m_serverMode == 0 && g_pBaseNetwork->m_shutdownRequested == 0) {
		WaitMessage();
		GetMessageA(message, 0, 0, 0);
	}
	if (g_pBaseNetwork->m_shutdownRequested == 0) {
		while (g_pBaseNetwork->m_serverMode != 0) {
			int(__stdcall * translateMessage)(const void*) = TranslateMessage;
			int(__stdcall * peekMessage)(void*, void*, unsigned int, unsigned int, unsigned int) = PeekMessageA;
			WaitMessage();
			if (peekMessage(message, 0, 0, 0, 0) != 0) {
				while (peekMessage(message, 0, 0, 0, 0) != 0) {
					GetMessageA(message, 0, 0, 0);
					translateMessage(message);
					DispatchMessageA(message);
				}
			}
			if (g_pNetworkStatusQueue != 0) {
				do {
					count = ((CBaseQueue*) g_pNetworkStatusQueue)->GetMessageCount();
					if (count != 0) {
						((CBaseQueue*) g_pNetworkStatusQueue)->ProcessNMsgs(count);
					}
				} while (count != 0);
			}
		}
	}
	delete g_pBaseNetwork;
	g_pBaseNetwork = 0;
	return 1;
}

// FUNCTION: LEMBALL 0x0046f3b0
bool VSFNET_Init()
{
	unsigned long startTime;

	g_hFileNetworkThread =
		CreateThread(0, 0, (unsigned int(__stdcall*)(void*)) FileNetworkMessageThread, 0, 0, &g_dwFileNetworkThreadId);
	if (g_hFileNetworkThread == 0) {
		MessageBoxA(0, "Unable to start 'VSNET Message loop' thread\n", "ERROR", 0);
		ExitProcess(0xbbbb);
	}

	SetThreadPriority(g_hFileNetworkThread, 2);

	startTime = timeGetTime();
	while (timeGetTime() - startTime < 10000 && g_pBaseNetwork == 0) {
	}
	if (g_pBaseNetwork == 0) {
		*g_pErrorOutput << "Network initialisation timed out\n";
		return 0;
	}

	startTime = timeGetTime();
	while (timeGetTime() - startTime < 10000 && g_pNetworkStatusQueue == 0) {
	}
	if (g_pNetworkStatusQueue == 0) {
		*g_pErrorOutput << "Network queue initialisation timed out\n";
		return 0;
	}

	return 1;
}

// FUNCTION: LEMBALL 0x0046f480
bool VSFNET_Quit()
{
	unsigned long startTime;

	if (g_pBaseNetwork != 0) {
		g_pBaseNetwork->m_shutdownRequested = 1;
		g_pBaseNetwork->ForceProcess();
		startTime = timeGetTime();
		while (timeGetTime() - startTime < 10000 && g_pBaseNetwork != 0) {
		}
		if (g_pBaseNetwork != 0) {
			*g_pErrorOutput << "Network quit timed out\n";
			return 0;
		}
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0046fa10
unsigned int TcpIpNetworkMessageThread()
{
	unsigned int message[7];
	unsigned int count;

	g_pBaseNetwork = new CTCPIPNetwork();
	while (g_pBaseNetwork->m_initialisePending == 0 && g_pBaseNetwork->m_serverMode == 0 &&
		   g_pBaseNetwork->m_shutdownRequested == 0) {
		WaitMessage();
		GetMessageA(message, 0, 0, 0);
	}
	if (g_pBaseNetwork->m_initialisePending != 0) {
		g_pBaseNetwork->DoInitialise();
	}
	while (g_pBaseNetwork->m_serverMode == 0 && g_pBaseNetwork->m_shutdownRequested == 0) {
		WaitMessage();
		GetMessageA(message, 0, 0, 0);
	}
	if (g_pBaseNetwork->m_shutdownRequested == 0) {
		while (g_pBaseNetwork->m_serverMode != 0) {
			int(__stdcall * translateMessage)(const void*) = TranslateMessage;
			int(__stdcall * peekMessage)(void*, void*, unsigned int, unsigned int, unsigned int) = PeekMessageA;
			WaitMessage();
			if (peekMessage(message, 0, 0, 0, 0) != 0) {
				while (peekMessage(message, 0, 0, 0, 0) != 0) {
					GetMessageA(message, 0, 0, 0);
					translateMessage(message);
					DispatchMessageA(message);
				}
			}
			if (g_pNetworkStatusQueue != 0) {
				do {
					count = ((CBaseQueue*) g_pNetworkStatusQueue)->GetMessageCount();
					if (count != 0) {
						((CBaseQueue*) g_pNetworkStatusQueue)->ProcessNMsgs(count);
					}
				} while (count != 0);
			}
		}
	}
	delete g_pBaseNetwork;
	g_pBaseNetwork = 0;
	return 1;
}

// FUNCTION: LEMBALL 0x0046fbb0
bool VSNET_Init()
{
	unsigned long startTime;

	g_hTCPIPNetworkThread = CreateThread(0,
										 0,
										 (unsigned int(__stdcall*)(void*)) TcpIpNetworkMessageThread,
										 0,
										 0,
										 &g_dwTCPIPNetworkThreadId);
	if (g_hTCPIPNetworkThread == 0) {
		MessageBoxA(0, "Unable to start 'VSNET Message loop' thread\n", "ERROR", 0);
		ExitProcess(0xbbbb);
	}

	SetThreadPriority(g_hTCPIPNetworkThread, 2);

	startTime = timeGetTime();
	while (timeGetTime() - startTime < 10000 && g_pBaseNetwork == 0) {
	}
	if (g_pBaseNetwork == 0) {
		*g_pErrorOutput << "Network initialisation timed out\n";
		return 0;
	}

	startTime = timeGetTime();
	while (timeGetTime() - startTime < 10000 && g_pNetworkStatusQueue == 0) {
	}
	if (g_pNetworkStatusQueue == 0) {
		*g_pErrorOutput << "Network queue initialisation timed out\n";
		return 0;
	}

	return 1;
}

// FUNCTION: LEMBALL 0x0046fc80
bool VSNET_Quit()
{
	unsigned long startTime;

	if (g_pBaseNetwork != 0) {
		g_pBaseNetwork->m_shutdownRequested = 1;
		g_pBaseNetwork->ForceProcess();
		startTime = timeGetTime();
		while (timeGetTime() - startTime < 10000 && g_pBaseNetwork != 0) {
		}
		if (g_pBaseNetwork != 0) {
			*g_pErrorOutput << "Network quit timed out\n";
			return 0;
		}
		return 1;
	}
	return 0;
}
