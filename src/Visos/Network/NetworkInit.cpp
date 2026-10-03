#include "Visos/Foundation/CBaseQueue.h"
#include "Visos/Foundation/CBaseQueueHandler.h"
#include "Visos/Foundation/CVSOStream.h"
#include "Visos/Foundation/VsInit.h"
#include "Visos/Network/CBaseNetwork.h"
#include "Visos/Network/CFileNetwork.h"
#include "Visos/Network/CTCPIPNetwork.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x0046f210
unsigned int FileNetworkMessageThread()
{
	MSG message;
	unsigned int count;

	g_pBaseNetwork = new CFileNetwork();
	while (g_pBaseNetwork->m_initialisePending == 0 && g_pBaseNetwork->m_serverMode == 0 &&
		   g_pBaseNetwork->m_shutdownRequested == 0) {
		WaitMessage();
		GetMessageA(&message, NULL, 0, 0);
	}
	if (g_pBaseNetwork->m_initialisePending != 0) {
		g_pBaseNetwork->DoInitialise();
	}
	while (g_pBaseNetwork->m_serverMode == 0 && g_pBaseNetwork->m_shutdownRequested == 0) {
		WaitMessage();
		GetMessageA(&message, NULL, 0, 0);
	}
	if (g_pBaseNetwork->m_shutdownRequested == 0) {
		while (g_pBaseNetwork->m_serverMode != 0) {
			WaitMessage();
			if (PeekMessageA(&message, NULL, 0, 0, PM_NOREMOVE) != 0) {
				while (PeekMessageA(&message, NULL, 0, 0, PM_NOREMOVE) != 0) {
					GetMessageA(&message, NULL, 0, 0);
					TranslateMessage(&message);
					DispatchMessageA(&message);
				}
			}
			if (g_pNetworkStatusQueue != NULL) {
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
	g_pBaseNetwork = NULL;
	return 1;
}

// FUNCTION: LEMBALL 0x0046f3b0
bool VSFNET_Init()
{
	unsigned long startTime;

	g_hFileNetworkThread =
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) FileNetworkMessageThread, NULL, 0, &g_dwFileNetworkThreadId);
	if (g_hFileNetworkThread == NULL) {
		MessageBoxA(NULL, "Unable to start 'VSNET Message loop' thread\n", "ERROR", 0);
		ExitProcess(0xbbbb);
	}

	SetThreadPriority(g_hFileNetworkThread, 2);

	startTime = timeGetTime();
	while (timeGetTime() - startTime < 10000 && g_pBaseNetwork == NULL) {
	}
	if (g_pBaseNetwork == NULL) {
		*g_pErrorOutput << "Network initialisation timed out\n";
		return false;
	}

	startTime = timeGetTime();
	while (timeGetTime() - startTime < 10000 && g_pNetworkStatusQueue == NULL) {
	}
	if (g_pNetworkStatusQueue == NULL) {
		*g_pErrorOutput << "Network queue initialisation timed out\n";
		return false;
	}

	return true;
}

// FUNCTION: LEMBALL 0x0046f480
bool VSFNET_Quit()
{
	unsigned long startTime;

	if (g_pBaseNetwork != NULL) {
		g_pBaseNetwork->m_shutdownRequested = 1;
		g_pBaseNetwork->ForceProcess();
		startTime = timeGetTime();
		while (timeGetTime() - startTime < 10000 && g_pBaseNetwork != NULL) {
		}
		if (g_pBaseNetwork != NULL) {
			*g_pErrorOutput << "Network quit timed out\n";
			return false;
		}
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0046fa10
unsigned int TcpIpNetworkMessageThread()
{
	MSG message;
	unsigned int count;

	g_pBaseNetwork = new CTCPIPNetwork();
	while (g_pBaseNetwork->m_initialisePending == 0 && g_pBaseNetwork->m_serverMode == 0 &&
		   g_pBaseNetwork->m_shutdownRequested == 0) {
		WaitMessage();
		GetMessageA(&message, NULL, 0, 0);
	}
	if (g_pBaseNetwork->m_initialisePending != 0) {
		g_pBaseNetwork->DoInitialise();
	}
	while (g_pBaseNetwork->m_serverMode == 0 && g_pBaseNetwork->m_shutdownRequested == 0) {
		WaitMessage();
		GetMessageA(&message, NULL, 0, 0);
	}
	if (g_pBaseNetwork->m_shutdownRequested == 0) {
		while (g_pBaseNetwork->m_serverMode != 0) {
			WaitMessage();
			if (PeekMessageA(&message, NULL, 0, 0, PM_NOREMOVE) != 0) {
				while (PeekMessageA(&message, NULL, 0, 0, PM_NOREMOVE) != 0) {
					GetMessageA(&message, NULL, 0, 0);
					TranslateMessage(&message);
					DispatchMessageA(&message);
				}
			}
			if (g_pNetworkStatusQueue != NULL) {
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
	g_pBaseNetwork = NULL;
	return 1;
}

// FUNCTION: LEMBALL 0x0046fbb0
bool VSNET_Init()
{
	unsigned long startTime;

	g_hTCPIPNetworkThread =
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) TcpIpNetworkMessageThread, NULL, 0, &g_dwTCPIPNetworkThreadId);
	if (g_hTCPIPNetworkThread == NULL) {
		MessageBoxA(NULL, "Unable to start 'VSNET Message loop' thread\n", "ERROR", 0);
		ExitProcess(0xbbbb);
	}

	SetThreadPriority(g_hTCPIPNetworkThread, 2);

	startTime = timeGetTime();
	while (timeGetTime() - startTime < 10000 && g_pBaseNetwork == NULL) {
	}
	if (g_pBaseNetwork == NULL) {
		*g_pErrorOutput << "Network initialisation timed out\n";
		return false;
	}

	startTime = timeGetTime();
	while (timeGetTime() - startTime < 10000 && g_pNetworkStatusQueue == NULL) {
	}
	if (g_pNetworkStatusQueue == NULL) {
		*g_pErrorOutput << "Network queue initialisation timed out\n";
		return false;
	}

	return true;
}

// FUNCTION: LEMBALL 0x0046fc80
bool VSNET_Quit()
{
	unsigned long startTime;

	if (g_pBaseNetwork != NULL) {
		g_pBaseNetwork->m_shutdownRequested = 1;
		g_pBaseNetwork->ForceProcess();
		startTime = timeGetTime();
		while (timeGetTime() - startTime < 10000 && g_pBaseNetwork != NULL) {
		}
		if (g_pBaseNetwork != NULL) {
			*g_pErrorOutput << "Network quit timed out\n";
			return false;
		}
		return true;
	}
	return false;
}
