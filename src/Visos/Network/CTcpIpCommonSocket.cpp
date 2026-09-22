#include "CTcpIpCommonSocket.h"

#include "Visos/Network/CBaseCommonSocket.h"
#include "Visos/Target/CNetworkWnd.h"

extern int g_unk0x4a23b8;
extern "C" int __stdcall closesocket(int p_socket);
extern "C" int __stdcall WSAGetLastError();

// FUNCTION: LEMBALL 0x0046fcf0
CTcpIpCommonSocket::CTcpIpCommonSocket() : CNetworkWnd("Socket Window", &g_unk0x4a23b8)
{
	m_asyncBuffer = 0;
	m_asyncRequest = 0;
	m_firstMessage = 0x440;
	m_lastMessage = 0x45f;
}

// FUNCTION: LEMBALL 0x0046fd70
CTcpIpCommonSocket::~CTcpIpCommonSocket()
{
	if (m_asyncBuffer != 0) {
		operator delete(m_asyncBuffer);
	}
}

// FUNCTION: LEMBALL 0x0046fdb0
int CTcpIpCommonSocket::OnNameResolved(unsigned int p_wParam, unsigned int p_lParam, char** p_buffer)
{
	int error;

	(void) p_wParam;
	error = (unsigned short) (p_lParam >> 16);
	if (error == 0) {
		goto success;
	}
	if (error < 0x2af9) {
		goto failure;
	}
	if (error <= 0x2afc) {
		goto retry;
	}

failure:
	SocketError((NetworkErrors) error);
	if (*p_buffer != 0) {
		operator delete(*p_buffer);
	}
	*p_buffer = 0;
	return 0xe;

success:
	return 0;

retry:
	return 2;
}

// FUNCTION: LEMBALL 0x00471a60
int CTcpIpCommonSocket::SysCloseSocket()
{
	return closesocket(m_socketHandle);
}

// FUNCTION: LEMBALL 0x00471ad0
void CTcpIpCommonSocket::SocketError()
{
	SocketError((NetworkErrors) WSAGetLastError());
}
