#ifndef LEMBALL_VISOS_NETWORK_TCPIPREADSOCKET_H
#define LEMBALL_VISOS_NETWORK_TCPIPREADSOCKET_H

#include "../../Common.h"
#include "BaseCommonSocket.h"  // complete type
#include "ReadSocket.h"        // complete type
#include "TcpIpCommonSocket.h" // complete type

// SIZE 0xac
// VTABLE: LEMBALL 0x0049a1e4 native receive callback
// VTABLE: LEMBALL 0x0049a1e8 ReadSocket virtual base
// VTABLE: LEMBALL 0x0049a210 BaseCommonSocket virtual base
class TcpIpReadSocket : public virtual BaseCommonSocket, public virtual ReadSocket, public TcpIpCommonSocket {
public:
	bool ReadBuff();
	bool ReadBuffFrom();
	virtual int Process();                           // vtable+0x00
	virtual void Closed(unsigned char p_notifyPeer); // vtable+0x0c
	virtual ~TcpIpReadSocket();                      // vtable+0x14

private:
	undefined4 m_reserved04;          // 0x04
	BaseCommonSocket m_commonBase;    // 0x08
	undefined m_readSocketCore[0x58]; // 0x34
	undefined m_nativeShell[0x20];    // 0x8c
};

// SYNTHETIC: LEMBALL 0x00471de0
// TcpIpReadSocket::`scalar deleting destructor'


#endif

