#ifndef LEMBALL_VISOS_NETWORK_CTCPIPWRITESOCKET_H
#define LEMBALL_VISOS_NETWORK_CTCPIPWRITESOCKET_H

#include "Multiplayer/Transport/CBaseCommonSocket.h"
#include "Platform/Windows/Network/TcpIp/CTCPIPCommonSocket.h"
#include "Multiplayer/Transport/CWriteSocket.h"
#include "Platform/WinSock/WinSock.h"

// SIZE 0xdc
// VTABLE: LEMBALL 0x0049a088 CNetworkWnd
// VTABLE: LEMBALL 0x0049a090 CBaseSocket
// VTABLE: LEMBALL 0x0049a0c0 CTCPIPWriteSocket
#pragma warning(disable : 4250)
class CTCPIPWriteSocket : public virtual CBaseCommonSocket,
						  public virtual CWriteSocket,
						  public virtual CTCPIPCommonSocket {
public:
	CTCPIPWriteSocket();
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam); // vtable+0x00
	virtual bool SendPacket(const unsigned char* p_data, int p_size);                  // vtable+0x24
	virtual void Closed(int p_notifyPeer);                                             // vtable+0x0c
	virtual void SetDestAddr(CNetworkAddress* p_address);                              // vtable+0x20
	virtual void SetPort(short p_port);                                                // vtable+0x28

private:
	TcpIpSocketAddress m_destination; // 0x04
};
#pragma warning(default : 4250)

// SYNTHETIC: LEMBALL 0x00471bf0 SYMBOL
// ?SetDestAddr@CTCPIPWriteSocket@@WPPPPPOMA@AEXPAVCNetworkAddress@@@Z

// SYNTHETIC: LEMBALL 0x00471c00 SYMBOL
// ?SendPacket@CTCPIPWriteSocket@@WPPPPPOMA@AEHPBEH@Z

// SYNTHETIC: LEMBALL 0x00471c10 SYMBOL
// ?SetPort@CTCPIPWriteSocket@@WPPPPPOMA@AEXF@Z

// SYNTHETIC: LEMBALL 0x00471e60 SYMBOL
// ?SysCloseSocket@CTCPIPCommonSocket@@WPPPPPPDI@AEHXZ

// SYNTHETIC: LEMBALL 0x00471e80
// CTCPIPWriteSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00471ec0 SYMBOL
// ?SocketError@CTCPIPCommonSocket@@WPPPPPPDI@AEXXZ

// GLOBAL: LEMBALL 0x0049a068
// CTCPIPWriteSocket::`vbtable'{for `CTCPIPCommonSocket'}

// GLOBAL: LEMBALL 0x0049a070
// CTCPIPWriteSocket::`vbtable'{for `CWriteSocket'}

// GLOBAL: LEMBALL 0x0049a078
// CTCPIPWriteSocket::`vbtable'{for `CTCPIPWriteSocket'}

#endif
