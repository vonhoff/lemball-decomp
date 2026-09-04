#ifndef LEMBALL_VISOS_NETWORK_TCPIPRWSOCKET_H
#define LEMBALL_VISOS_NETWORK_TCPIPRWSOCKET_H

#include "../../Common.h"
#include "RwSocket.h"         // complete type
#include "TcpIpReadSocket.h"  // complete type
#include "TcpIpWriteSocket.h" // complete type

// SIZE 0x144
class TcpIpRwSocket : public virtual RwSocket, public virtual TcpIpReadSocket, public virtual TcpIpWriteSocket {
public:
	TcpIpRwSocket();
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam); // vtable+0x00
	virtual void Closed(unsigned char p_notifyPeer);                                   // vtable+0x0c
	virtual NetworkMessage* ReceiveAcknowledgement();                                  // vtable+0x1c
	virtual void SendAcknowledgement();                                                // vtable+0x18
	virtual ~TcpIpRwSocket();                                                          // vtable+0x14
};

// SYNTHETIC: LEMBALL 0x00471af0 SYMBOL
// ?SysCloseSocket@TcpIpCommonSocket@@WPPPPPONI@AEHXZ

// SYNTHETIC: LEMBALL 0x00471b60 SYMBOL
// ?SocketError@TcpIpCommonSocket@@WPPPPPONI@AEXXZ

// SYNTHETIC: LEMBALL 0x00471cd0
// TcpIpRwSocket::`scalar deleting destructor'

#endif
