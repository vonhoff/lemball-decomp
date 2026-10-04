#ifndef LEMBALL_VISOS_NETWORK_CTCPIPCOMMONSOCKET_H
#define LEMBALL_VISOS_NETWORK_CTCPIPCOMMONSOCKET_H

#include "Platform/Windows/Network/CNetworkWnd.h"
#include "Engine/Network/CBaseCommonSocket.h"

// SIZE 0x4c
// VTABLE: LEMBALL 0x0049a060 CNetworkWnd
// VTABLE: LEMBALL 0x0049a050 CBaseCommonSocket
class CTCPIPCommonSocket : public CNetworkWnd, public virtual CBaseCommonSocket {
public:
	enum AsyncMessage {
		TCPIP_MESSAGE_LOCAL_HOST_RESOLVED = 0x440,
		TCPIP_MESSAGE_SPECIFIC_HOST_RESOLVED = 0x441,
		TCPIP_MESSAGE_SERVICE_RESOLVED = 0x442,
		TCPIP_MESSAGE_SOCKET_EVENT = 0x443
	};

	enum NameResult {
		NAME_RESOLVED = 0,
		NAME_LOOKUP_FAILED = 2,
		NAME_LOOKUP_ERROR_HANDLED = 14
	};

	using CBaseCommonSocket::SocketError;

	CTCPIPCommonSocket();
	int OnNameResolved(unsigned int p_wParam, unsigned int p_lParam, char** p_buffer);
	virtual int SysCloseSocket();  // vtable+0x00
	virtual void SocketError();    // vtable+0x08
	virtual ~CTCPIPCommonSocket(); // vtable+0x04

	friend class CTCPIPBroadcast;
	friend class CTCPIPConnect;

private:
	char* m_asyncBuffer;         // 0x14
	unsigned int m_asyncRequest; // 0x18
};

// SYNTHETIC: LEMBALL 0x00471a90
// CTCPIPCommonSocket::`scalar deleting destructor'

// GLOBAL: LEMBALL 0x0049a048
// CTCPIPCommonSocket::`vbtable'

#endif
