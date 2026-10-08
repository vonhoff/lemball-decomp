#include "Platform/Windows/Network/FileTransport/CFileNetwork.h"

#include "Engine/Queues/CBaseQueue.h"
#include "Engine/Queues/CBaseQueueHandler.h"
#include "Engine/Streams/CVSOStream.h"
#include "Multiplayer/Transport/FileTransport/CFileBroadcast.h"
#include "Multiplayer/Transport/FileTransport/CFileConnect.h"
#include "Multiplayer/Transport/FileTransport/CFileNetworkAddress.h"
#include "Multiplayer/Transport/NetworkConstants.h"
#include "Platform/Windows/Network/CNetworkWnd.h"
#include "Platform/Windows/ThreadConstants.h"

#include <new.h>
#include <stddef.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// FUNCTION: LEMBALL 0x0046f6b0
CFileNetwork::CFileNetwork()
	: CNetworkWnd("File-based Network", &g_fileNetworkWindowClassRegistered), m_alternateTimer(0)
{
}

// FUNCTION: LEMBALL 0x0046f6f0
void CFileNetwork::Initialise()
{
	*g_pDebugOutput << "Network Initialised:\n";
	*g_pDebugOutput << "Windows file-based networking\n";
	m_timerId = SetTimer((HWND) m_windowHandle, FILE_NETWORK_TIMER_ID, FILE_NETWORK_TIMER_INTERVAL_MS, NULL);
}

// FUNCTION: LEMBALL 0x0046f730
void CFileNetwork::UnInitialise()
{
	KillTimer((HWND) m_windowHandle, m_timerId);
}

// FUNCTION: LEMBALL 0x0046f740
void CFileNetwork::ResetTimer(unsigned int p_interval)
{
	*g_pDebugOutput << "Setting next timer event to " << (unsigned long) p_interval << "ms from now\n";

	KillTimer((HWND) m_windowHandle, m_timerId);
	m_timerId = SetTimer((HWND) m_windowHandle, FILE_NETWORK_TIMER_ID, p_interval, NULL);
	m_alternateTimer = m_alternateTimer == 0;
}

// FUNCTION: LEMBALL 0x0046f7a0
void CFileNetwork::Setup(const char* p_peerName, const char* p_path)
{
	CFileBroadcast::Setup(p_peerName, p_path);
}

// FUNCTION: LEMBALL 0x0046f7c0
void CFileNetwork::BeforeDestroyConnections()
{
	((CFileBroadcast*) m_broadcast)->ReadPortInfo();
}

// FUNCTION: LEMBALL 0x0046f7d0
void CFileNetwork::AfterDestroyConnections()
{
	((CFileBroadcast*) m_broadcast)->WritePortInfo();
}

// FUNCTION: LEMBALL 0x0046f7e0
int CFileNetwork::Process(unsigned int p_message, unsigned int p_wParam, long p_lParam)
{
	(void) p_wParam;
	(void) p_lParam;

	if (p_message != WM_TIMER) {
		if (p_message != FILE_NETWORK_MESSAGE_FORCE_PROCESS) {
			return NETWORK_WINDOW_MESSAGE_UNHANDLED;
		}
		if (m_alternateTimer != 0) {
			ResetTimer(FILE_NETWORK_TIMER_INTERVAL_MS);
		}
		if (g_pNetworkStatusQueue != NULL && ((CBaseQueue*) g_pNetworkStatusQueue)->GetMessageCount() != 0) {
			((CBaseQueue*) g_pNetworkStatusQueue)
				->ProcessNMsgs(((CBaseQueue*) g_pNetworkStatusQueue)->GetMessageCount());
		}
	}
	Process();
	return 0;
}

// FUNCTION: LEMBALL 0x0046f840
void CFileNetwork::ForceProcess()
{
	PostMessageA((HWND) m_windowHandle, FILE_NETWORK_MESSAGE_FORCE_PROCESS, 0, 0);
}

// FUNCTION: LEMBALL 0x0046f860
CNetworkAddress* CFileNetwork::GetNewNetworkAddress()
{
	void* storage;
	CFileNetworkAddress* address;

	storage = operator new(sizeof(CFileNetworkAddress));
	if (storage != NULL) {
		address = new (storage) CFileNetworkAddress();
		address->m_text[0] = '\0';
		return address;
	}
	return NULL;
}

// FUNCTION: LEMBALL 0x0046f930
void* CFileNetwork::GetNewConnect()
{
	void* storage;

	storage = operator new(sizeof(CFileConnect));
	if (storage != NULL) {
		return new (storage) CFileConnect();
	}
	return NULL;
}

// FUNCTION: LEMBALL 0x0046f950
void* CFileNetwork::GetNewBroadcast()
{
	void* storage;

	storage = operator new(sizeof(CFileBroadcast));
	if (storage != NULL) {
		return new (storage) CFileBroadcast();
	}
	return NULL;
}

// GLOBAL: LEMBALL 0x004a2260
unsigned long g_dwFileNetworkThreadId = THREAD_ID_BEFORE_CREATE;

// GLOBAL: LEMBALL 0x004a2264
void* g_hFileNetworkThread = NULL;

// GLOBAL: LEMBALL 0x004a2268
int g_fileNetworkWindowClassRegistered = 0;
