#include "CBaseCommonSocket.h"

#include "CBaseNetwork.h"
#include "NetworkConstants.h"

// FUNCTION: LEMBALL 0x0045f680
CBaseCommonSocket::CBaseCommonSocket()
{
	m_socketHandle = NETWORK_SOCKET_HANDLE_INVALID;
	m_readReady = false;
	m_isOpen = false;
	m_port = NETWORK_PORT_UNASSIGNED;
	m_writeReady = false;
	m_closePending = false;
	m_eventPending = false;
	m_socketFlags = false;
	m_lastError = NETWORK_ERROR_NONE;
	m_platformState = new unsigned char[0x10];
}

// FUNCTION: LEMBALL 0x0045f6c0
CBaseCommonSocket::~CBaseCommonSocket()
{
	delete[] m_platformState;
}

// FUNCTION: LEMBALL 0x0045f6e0
void CBaseCommonSocket::SocketError(NetworkErrors p_error)
{
	m_lastError = p_error;
	g_lastNetworkError = p_error;
	if (p_error != 0 && m_isOpen != 0) {
		SysCloseSocket();
		m_readReady = false;
		m_isOpen = false;
		m_socketHandle = NETWORK_SOCKET_HANDLE_INVALID;
		m_writeReady = false;
	}
}

// FUNCTION: LEMBALL 0x0045f720
void CBaseCommonSocket::CloseSocket()
{
	if (m_isOpen != 0) {
		m_readReady = false;
		m_isOpen = false;
		if (SysCloseSocket() == NETWORK_SOCKET_ERROR) {
			SocketError();
		}
	}
}

// FUNCTION: LEMBALL 0x004628d0
void CBaseCommonSocket::Closed(int p_notifyPeer)
{
	(void) p_notifyPeer;
}
