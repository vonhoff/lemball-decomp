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
class TcpIpReadSocket : public virtual BaseCommonSocket, public virtual ReadSocket, public virtual TcpIpCommonSocket {
public:
	bool ReadBuff();
	bool ReadBuffFrom();
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam); // vtable+0x00
	virtual void Closed(unsigned char p_notifyPeer);                                   // vtable+0x0c
	virtual ~TcpIpReadSocket();                                                        // vtable+0x14
};

// SYNTHETIC: LEMBALL 0x00471de0
// TcpIpReadSocket::`scalar deleting destructor'

#endif
