#ifndef LEMBALL_VISOS_NETWORK_TCPIPCOMMONSOCKET_H
#define LEMBALL_VISOS_NETWORK_TCPIPCOMMONSOCKET_H

#include "../../Common.h"
#include "BaseCommonSocket.h" // complete type

// SIZE 0x4c
// VTABLE: LEMBALL 0x0049a060 native window shell
// VTABLE: LEMBALL 0x0049a050 BaseCommonSocket virtual base at +0x20
class TcpIpCommonSocket : public virtual BaseCommonSocket {
public:
	TcpIpCommonSocket();
	virtual int SysCloseSocket(); // vtable+0x00
	virtual void SocketError();   // vtable+0x08
	virtual ~TcpIpCommonSocket(); // vtable+0x04

private:
	undefined m_nativeWindow[0x10];  // 0x00
	undefined m_platformState[0x0c]; // 0x14
	BaseCommonSocket m_base;         // 0x20
};

// SYNTHETIC: LEMBALL 0x00471a90
// TcpIpCommonSocket::`scalar deleting destructor'

#endif
