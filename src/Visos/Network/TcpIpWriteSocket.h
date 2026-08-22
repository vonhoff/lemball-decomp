#ifndef LEMBALL_SCAFFOLD_VISOS_NETWORK_TCPIPWRITESOCKET_H
#define LEMBALL_SCAFFOLD_VISOS_NETWORK_TCPIPWRITESOCKET_H

#include "../../Common.h"
#include "BaseCommonSocket.h"  // complete type
#include "TcpIpCommonSocket.h" // complete type
#include "WriteSocket.h"       // complete type

// SIZE 0xe0
// VTABLE: LEMBALL 0x0049a088 native window shell at +0xc0
// VTABLE: LEMBALL 0x0049a090 WriteSocket virtual base at +0x48
// VTABLE: LEMBALL 0x0049a0c0 BaseCommonSocket virtual base at +0x18
class TcpIpWriteSocket : public virtual BaseCommonSocket, public virtual WriteSocket, public TcpIpCommonSocket {
public:
	TcpIpWriteSocket();
	virtual bool SendPacket(const unsigned char* p_data, int p_size); // vtable+0x24
	virtual void Closed(unsigned char p_notifyPeer);                  // vtable+0x0c
	virtual void SetDestAddr(NetworkAddress* p_address);              // vtable+0x20
	virtual void SetPort(short p_port);                               // vtable+0x28
	virtual ~TcpIpWriteSocket();                                      // vtable+0x14

private:
	unsigned short m_addressFamily;    // 0x04
	unsigned short m_networkPort;      // 0x06
	unsigned int m_destinationIPv4;    // 0x08
	undefined m_sockaddrPadding[0x0c]; // 0x0c
	BaseCommonSocket m_commonBase;     // 0x18
	undefined m_writeSocketCore[0x78]; // 0x48
	undefined m_nativeShell[0x20];     // 0xc0
};

#endif
