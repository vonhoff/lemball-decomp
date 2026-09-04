#ifndef LEMBALL_VISOS_NETWORK_TCPIPWRITESOCKET_H
#define LEMBALL_VISOS_NETWORK_TCPIPWRITESOCKET_H

#include "../../Common.h"
#include "BaseCommonSocket.h"  // complete type
#include "TcpIpCommonSocket.h" // complete type
#include "WriteSocket.h"       // complete type

// SIZE 0xe0
// VTABLE: LEMBALL 0x0049a088 TargetNetworkWindow
// VTABLE: LEMBALL 0x0049a090 BaseSocket
// VTABLE: LEMBALL 0x0049a0c0 TcpIpWriteSocket
// 68K 0x10106290 __dt__17CTCPIPWriteSocketFv; x86 emits only the synthetic deleting destructor.
class TcpIpWriteSocket : public virtual BaseCommonSocket, public virtual WriteSocket, public virtual TcpIpCommonSocket {
public:
	TcpIpWriteSocket();
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam); // vtable+0x00
	virtual bool SendPacket(const unsigned char* p_data, int p_size);                  // vtable+0x24
	virtual void Closed(unsigned char p_notifyPeer);                                   // vtable+0x0c
	virtual void SetDestAddr(NetworkAddress* p_address);                               // vtable+0x20
	virtual void SetPort(short p_port);                                                // vtable+0x28

private:
	unsigned short m_addressFamily; // 0x04
	unsigned short m_networkPort;   // 0x06
	unsigned int m_destinationIPv4; // 0x08
	undefined m_sockaddrPadding[8]; // 0x0c
};

// SYNTHETIC: LEMBALL 0x00471bf0 SYMBOL
// ?SetDestAddr@TcpIpWriteSocket@@WPPPPPOMA@AEXPAVNetworkAddress@@@Z

// SYNTHETIC: LEMBALL 0x00471c00 SYMBOL
// ?SendPacket@TcpIpWriteSocket@@WPPPPPOMA@AEHPBEH@Z

// SYNTHETIC: LEMBALL 0x00471c10 SYMBOL
// ?SetPort@TcpIpWriteSocket@@WPPPPPOMA@AEXF@Z

// SYNTHETIC: LEMBALL 0x00471e60 SYMBOL
// ?SysCloseSocket@TcpIpCommonSocket@@WPPPPPPDI@AEHXZ

// SYNTHETIC: LEMBALL 0x00471e80
// TcpIpWriteSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00471ec0 SYMBOL
// ?SocketError@TcpIpCommonSocket@@WPPPPPPDI@AEXXZ

#endif
