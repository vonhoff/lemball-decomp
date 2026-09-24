#include "CBaseCommonSocket.h"

#include "CTCPIPNetwork.h"

// FUNCTION: LEMBALL 0x0045f680
CBaseCommonSocket::CBaseCommonSocket()
{
	CBaseCommonSocket* self;

	m_socketHandle = -1;
	self = this;
	m_readReady = 0;
	m_isOpen = 0;
	self->m_port = -1;
	m_writeReady = 0;
	m_closePending = 0;
	m_eventPending = 0;
	m_socketFlags = 0;
	m_lastError = (NetworkErrors) 0;
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
		m_socketHandle = -1;
		m_writeReady = 0;
	}
}

// FUNCTION: LEMBALL 0x0045f720
void CBaseCommonSocket::CloseSocket()
{
	if (m_isOpen != 0) {
		m_readReady = 0;
		m_isOpen = 0;
		if (SysCloseSocket() == -1) {
			SocketError();
		}
	}
}

// FUNCTION: LEMBALL 0x004628d0
void CBaseCommonSocket::Closed(int p_notifyPeer)
{
	(void) p_notifyPeer;
}
