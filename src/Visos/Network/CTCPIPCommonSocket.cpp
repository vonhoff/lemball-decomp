#include "CTCPIPCommonSocket.h"

#include "Visos/Network/CBaseCommonSocket.h"
#include "Visos/Target/Network/CNetworkWnd.h"

extern int g_socketWindowClassRegistered;
#include "Platform/WinSock/WinSock.h"

// FUNCTION: LEMBALL 0x0046fcf0
CTCPIPCommonSocket::CTCPIPCommonSocket() : CNetworkWnd("Socket Window", &g_socketWindowClassRegistered)
{
	m_asyncBuffer = 0;
	m_asyncRequest = 0;
	m_firstMessage = 0x440;
	m_lastMessage = 0x45f;
}

// FUNCTION: LEMBALL 0x0046fd70
CTCPIPCommonSocket::~CTCPIPCommonSocket()
{
	if (m_asyncBuffer != 0) {
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
		if (*p_buffer != 0) {
			operator delete(*p_buffer);
		}
		*p_buffer = 0;
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
