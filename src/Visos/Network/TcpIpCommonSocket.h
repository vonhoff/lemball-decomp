#ifndef LEMBALL_VISOS_NETWORK_TCPIPCOMMONSOCKET_H
#define LEMBALL_VISOS_NETWORK_TCPIPCOMMONSOCKET_H

#include "../../Common.h"
#include "../Target/TargetNetworkWindow.h" // complete type
#include "BaseCommonSocket.h"              // complete type

// SIZE 0x4c
// VTABLE: LEMBALL 0x0049a060 TargetNetworkWindow
// VTABLE: LEMBALL 0x0049a050 BaseCommonSocket
class TcpIpCommonSocket : public TargetNetworkWindow, public virtual BaseCommonSocket {
public:
	TcpIpCommonSocket();
	int HandleAsyncNameResolutionResult(unsigned int p_wParam, unsigned int p_lParam, char** p_buffer);
	virtual int SysCloseSocket(); // vtable+0x00
	virtual void SocketError();   // vtable+0x08
	virtual ~TcpIpCommonSocket(); // vtable+0x04

	friend class TcpIpBroadcast;
	friend class TcpIpConnect;

private:
	char* m_asyncBuffer;         // 0x14
	unsigned int m_asyncRequest; // 0x18
};

// SYNTHETIC: LEMBALL 0x00471a90
// TcpIpCommonSocket::`scalar deleting destructor'

#endif
