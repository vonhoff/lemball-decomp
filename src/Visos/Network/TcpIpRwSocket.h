#ifndef LEMBALL_VISOS_NETWORK_TCPIPRWSOCKET_H
#define LEMBALL_VISOS_NETWORK_TCPIPRWSOCKET_H

#include "../../Common.h"
#include "BaseCommonSocket.h"  // complete type
#include "ReadSocket.h"        // complete type
#include "TcpIpCommonSocket.h" // complete type
#include "WriteSocket.h"       // complete type

// SIZE 0x130
class TcpIpRwSocket : public virtual BaseCommonSocket,
					  public virtual WriteSocket,
					  public virtual ReadSocket,
					  public TcpIpCommonSocket {
public:
	virtual int SysCloseSocket();                    // vtable+0x00
	virtual void Closed(unsigned char p_notifyPeer); // vtable+0x0c
	virtual void ReceiveAcknowledgement();           // vtable+0x1c
	virtual void SendAcknowledgement();              // vtable+0x18
	virtual ~TcpIpRwSocket();                        // vtable+0x14

private:
	undefined4 m_reserved04;           // 0x04
	BaseCommonSocket m_commonBase;     // 0x08
	undefined4 m_reserved34;           // 0x34
	undefined m_writeSocketCore[0x78]; // 0x38
	undefined m_readSocketCore[0x58];  // 0xb0
	undefined m_reserved108[0x08];     // 0x108
	undefined m_nativeShell[0x20];     // 0x110
};

// SYNTHETIC: LEMBALL 0x00471cd0
// TcpIpRwSocket::`scalar deleting destructor'


#endif

