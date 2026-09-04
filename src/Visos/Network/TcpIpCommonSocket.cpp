#include "TcpIpCommonSocket.h"

#include <new.h>

extern int g_unk0x4a23b8;
extern "C" int __stdcall closesocket(int p_socket);
extern "C" int __stdcall WSAGetLastError();

// 68K 0x1010c74e __ct__18CTCPIPCommonSocketFv
// FUNCTION: LEMBALL 0x0046fcf0
TcpIpCommonSocket::TcpIpCommonSocket()
	: TargetNetworkWindow("Socket Window", &g_unk0x4a23b8), m_asyncBuffer(0), m_asyncRequest(0)
{
	m_firstMessage = 0x440;
	m_lastMessage = 0x45f;
}

// FUNCTION: LEMBALL 0x0046fdb0
int TcpIpCommonSocket::HandleAsyncNameResolutionResult(unsigned int p_wParam, unsigned int p_lParam, char** p_buffer)
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
	BaseCommonSocket::SocketError((NetworkErrors) error);
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

// 68K 0x1010c820 SysCloseSocket__18CTCPIPCommonSocketFv
// FUNCTION: LEMBALL 0x00471a60
int TcpIpCommonSocket::SysCloseSocket()
{
	return closesocket(m_socketHandle);
}

// 68K 0x1010c7b0 __dt__18CTCPIPCommonSocketFv
TcpIpCommonSocket::~TcpIpCommonSocket()
{
}

// 68K 0x101075e2 SocketError__18CTCPIPCommonSocketFv
// FUNCTION: LEMBALL 0x00471ad0
void TcpIpCommonSocket::SocketError()
{
	BaseCommonSocket::SocketError((NetworkErrors) WSAGetLastError());
}
