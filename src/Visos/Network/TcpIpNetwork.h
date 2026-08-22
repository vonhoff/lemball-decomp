#ifndef LEMBALL_VISOS_NETWORK_TCPIPNETWORK_H
#define LEMBALL_VISOS_NETWORK_TCPIPNETWORK_H

#include "../../Common.h"
#include "BaseNetwork.h" // complete type

// SIZE 0x78
// VTABLE: LEMBALL 0x0049a2dc native window shell
// VTABLE: LEMBALL 0x0049a2a8 BaseNetwork subobject at +0x10
class TcpIpNetwork {
public:
	TcpIpNetwork();
	virtual TcpIpBroadcast* GetNewBroadcast();           // vtable+0x24
	virtual TcpIpConnect* GetNewConnect();               // vtable+0x20
	virtual TcpIpNetworkAddress* GetNewNetworkAddress(); // vtable+0x28
	virtual int Process();                               // vtable+0x00
	virtual void ForceProcess();                         // vtable+0x1c
	virtual void Initialise();                           // vtable+0x0c
	virtual void UnInitialise();                         // vtable+0x10
	virtual ~TcpIpNetwork();                             // vtable+0x04

private:
	undefined m_nativeWindow[0x10]; // 0x00
	BaseNetwork m_base;             // 0x10
	unsigned int m_timerId;         // 0x74
};

#endif
