#include "CTCPIPCommonSocket.h"

#include "Visos/Network/CBaseCommonSocket.h"
#include "Platform/Windows/Network/CNetworkWnd.h"

extern int g_socketWindowClassRegistered;
#include "Platform/WinSock/WinSock.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0046fcf0
CTCPIPCommonSocket::CTCPIPCommonSocket() : CNetworkWnd("Socket Window", &g_socketWindowClassRegistered)
{
	enum {
		TCPIP_SOCKET_MESSAGE_RANGE_LAST = 0x45f
	};
	m_asyncBuffer = NULL;
	m_asyncRequest = 0;
	m_firstMessage = TCPIP_MESSAGE_LOCAL_HOST_RESOLVED;
	m_lastMessage = TCPIP_SOCKET_MESSAGE_RANGE_LAST;
}

// FUNCTION: LEMBALL 0x0046fd70
CTCPIPCommonSocket::~CTCPIPCommonSocket()
{
	if (m_asyncBuffer != NULL) {
		operator delete(m_asyncBuffer);
	}
}

// FUNCTION: LEMBALL 0x0046fdb0
int CTCPIPCommonSocket::OnNameResolved(unsigned int p_wParam, unsigned int p_lParam, char** p_buffer)
{
	int error;

	(void) p_wParam;
	error = (unsigned short) (p_lParam >> 16);
	if (error != 0) {
		if (error >= WSAHOST_NOT_FOUND && error <= WSANO_DATA) {
			return NAME_LOOKUP_FAILED;
		}
		SocketError((NetworkErrors) error);
		if (*p_buffer != NULL) {
			operator delete(*p_buffer);
		}
		*p_buffer = NULL;
		return NAME_LOOKUP_ERROR_HANDLED;
	}
	return NAME_RESOLVED;
}

// FUNCTION: LEMBALL 0x00471a60
int CTCPIPCommonSocket::SysCloseSocket()
{
	return closesocket(m_socketHandle);
}

// FUNCTION: LEMBALL 0x00471ad0
void CTCPIPCommonSocket::SocketError()
{
	SocketError((NetworkErrors) WSAGetLastError());
}
