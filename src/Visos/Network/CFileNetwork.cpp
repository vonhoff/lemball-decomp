#include "CFileNetwork.h"

#include "../Foundation/CBaseQueue.h"
#include "../Foundation/CVsOStream.h"
#include "CFileBroadcast.h"
#include "CFileConnect.h"
#include "CFileNetworkAddress.h"
#include "Visos/Foundation/CBaseQueueHandler.h"
#include "Visos/Network/CBaseNetwork.h"
#include "Visos/Target/Network/CNetworkWnd.h"

#include <new.h>

extern "C" __declspec(dllimport) int __stdcall KillTimer(void* p_window, unsigned int p_id);
extern "C" __declspec(dllimport) int __stdcall PostMessageA(void* p_window,
															unsigned int p_message,
															unsigned int p_wParam,
															long p_lParam);
extern "C" __declspec(dllimport) unsigned int __stdcall SetTimer(void* p_window,
																 unsigned int p_id,
																 unsigned int p_elapse,
																 void* p_callback);

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
	m_timerId = SetTimer(m_windowHandle, 0x12345679, 0x14, 0);
}

// FUNCTION: LEMBALL 0x0046f730
void CFileNetwork::UnInitialise()
{
	KillTimer(m_windowHandle, m_timerId);
}

// FUNCTION: LEMBALL 0x0046f740
void CFileNetwork::ResetTimer(unsigned int p_interval)
{
	*g_pDebugOutput << "Setting next timer event to " << (unsigned long) p_interval << "ms from now\n";

	KillTimer(m_windowHandle, m_timerId);
	m_timerId = SetTimer(m_windowHandle, 0x12345679, p_interval, 0);
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

	if (p_message != 0x113) {
		if (p_message != 0x444) {
			return -1;
		}
		if (m_alternateTimer != 0) {
			ResetTimer(20);
		}
		if (g_pNetworkStatusQueue != 0 && ((CBaseQueue*) g_pNetworkStatusQueue)->GetMessageCount() != 0) {
			((CBaseQueue*) g_pNetworkStatusQueue)
				->ProcessNMsgs(((CBaseQueue*) g_pNetworkStatusQueue)->GetMessageCount());
		}
	}
	CBaseNetwork& network = *(CBaseNetwork*) ((unsigned char*) this + sizeof(CNetworkWnd));
	network.Process();
	return 0;
}

// FUNCTION: LEMBALL 0x0046f840
void CFileNetwork::ForceProcess()
{
	PostMessageA(m_windowHandle, 0x444, 0, 0);
}

// FUNCTION: LEMBALL 0x0046f860
void* CFileNetwork::GetNewNetworkAddress()
{
	void* storage;
	CFileNetworkAddress* address;

	storage = operator new(sizeof(CFileNetworkAddress));
	if (storage != 0) {
		address = new (storage) CFileNetworkAddress();
		address->m_text[0] = '\0';
		return address;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0046f930
void* CFileNetwork::GetNewConnect()
{
	void* storage;

	storage = operator new(sizeof(CFileConnect));
	if (storage != 0) {
		return new (storage) CFileConnect();
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0046f950
void* CFileNetwork::GetNewBroadcast()
{
	void* storage;

	storage = operator new(sizeof(CFileBroadcast));
	if (storage != 0) {
		return new (storage) CFileBroadcast();
	}
	return 0;
}

// GLOBAL: LEMBALL 0x004a2260
unsigned int g_dwFileNetworkThreadId = 0x12345678;

// GLOBAL: LEMBALL 0x004a2264
void* g_hFileNetworkThread = 0;

// GLOBAL: LEMBALL 0x004a2268
int g_fileNetworkWindowClassRegistered = 0;

// GLOBAL: LEMBALL 0x004a2de4
char* g_pFileBroadcastData = 0;
