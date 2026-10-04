#include "CBaseCommonSocket.h"

#include "CTCPIPNetwork.h"
#include "NetworkConstants.h"

// FUNCTION: LEMBALL 0x0045f680
CBaseCommonSocket::CBaseCommonSocket()
{
	CBaseCommonSocket* self;

	m_socketHandle = NETWORK_SOCKET_HANDLE_INVALID;
	self = this;
	m_readReady = 0;
	m_isOpen = 0;
	self->m_port = NETWORK_PORT_UNASSIGNED;
	m_writeReady = 0;
	m_closePending = 0;
	m_eventPending = 0;
	m_socketFlags = 0;
	m_lastError = NETWORK_ERROR_NONE;
	m_platformState = operator new(0x10);
}

// FUNCTION: LEMBALL 0x0045f6c0
CBaseCommonSocket::~CBaseCommonSocket()
{
	operator delete(m_platformState);
}

// FUNCTION: LEMBALL 0x0045f6e0
void CBaseCommonSocket::SocketError(NetworkErrors p_error)
{
	m_lastError = p_error;
	g_lastNetworkError = p_error;
	if (p_error != 0 && m_isOpen != 0) {
		SysCloseSocket();
		m_readReady = 0;
		m_isOpen = 0;
		m_socketHandle = NETWORK_SOCKET_HANDLE_INVALID;
		m_writeReady = 0;
	}
}

// FUNCTION: LEMBALL 0x0045f720
void CBaseCommonSocket::CloseSocket()
{
	if (m_isOpen != 0) {
		m_readReady = 0;
		m_isOpen = 0;
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
