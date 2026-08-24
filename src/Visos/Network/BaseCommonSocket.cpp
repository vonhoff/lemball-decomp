#include "BaseCommonSocket.h"

#include "TcpIpNetwork.h"

#include <new.h>

// 68K 0x1020b1ec __ct__17CBaseCommonSocketFv
// FUNCTION: LEMBALL 0x0045f680
BaseCommonSocket::BaseCommonSocket()
{
	m_socketHandle = -1;
	m_port = 0xffff;
	m_readReady = 0;
	m_isOpen = 0;
	m_writeReady = 0;
	m_closePending = 0;
	m_eventPending = 0;
	m_socketFlags = 0;
	m_lastError = 0;
	m_platformState = operator new(0x10);
}

// 68K 0x1020b25a __dt__17CBaseCommonSocketFv
// FUNCTION: LEMBALL 0x0045f6c0
BaseCommonSocket::~BaseCommonSocket()
{
	operator delete(m_platformState);
}

// 68K 0x1020b2b2 SocketError__17CBaseCommonSocketF13NetworkErrors
// FUNCTION: LEMBALL 0x0045f6e0
void BaseCommonSocket::SocketError(NetworkErrors p_arg0)
{
	m_lastError = p_arg0;
	g_lastNetworkError = p_arg0;
	if (p_arg0 != 0 && m_isOpen != 0) {
		m_readReady = 0;
		m_isOpen = 0;
		m_socketHandle = -1;
		m_writeReady = 0;
	}
}

// 68K 0x1020b332 CloseSocket__17CBaseCommonSocketFv
// STUB: LEMBALL 0x0045f720
void BaseCommonSocket::CloseSocket()
{
}

// 68K 0x1010e52c Closed__17CBaseCommonSocketFUc
// STUB: LEMBALL 0x004628d0
void BaseCommonSocket::Closed(unsigned char p_arg0)
{
}
