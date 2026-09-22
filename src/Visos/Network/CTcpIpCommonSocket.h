#ifndef LEMBALL_VISOS_NETWORK_CTCPIPCOMMONSOCKET_H
#define LEMBALL_VISOS_NETWORK_CTCPIPCOMMONSOCKET_H

#include "../Target/TargetNetworkWindow.h" // complete type
#include "CBaseCommonSocket.h"             // complete type

// SIZE 0x4c
// VTABLE: LEMBALL 0x0049a060 TargetNetworkWindow
// VTABLE: LEMBALL 0x0049a050 CBaseCommonSocket
class CTcpIpCommonSocket : public TargetNetworkWindow, public virtual CBaseCommonSocket {
public:
	using CBaseCommonSocket::SocketError;

	CTcpIpCommonSocket();
	int HandleAsyncNameResolutionResult(unsigned int p_wParam, unsigned int p_lParam, char** p_buffer);
	virtual int SysCloseSocket();  // vtable+0x00
	virtual void SocketError();    // vtable+0x08
	virtual ~CTcpIpCommonSocket(); // vtable+0x04

	friend class CTcpIpBroadcast;
	friend class CTcpIpConnect;

private:
	char* m_asyncBuffer;         // 0x14
	unsigned int m_asyncRequest; // 0x18
};

// SYNTHETIC: LEMBALL 0x00471a90
// CTcpIpCommonSocket::`scalar deleting destructor'

#endif
