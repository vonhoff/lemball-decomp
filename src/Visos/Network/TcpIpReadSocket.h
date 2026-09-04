#ifndef LEMBALL_VISOS_NETWORK_TCPIPREADSOCKET_H
#define LEMBALL_VISOS_NETWORK_TCPIPREADSOCKET_H

#include "../../Common.h"
#include "BaseCommonSocket.h"  // complete type
#include "ReadSocket.h"        // complete type
#include "TcpIpCommonSocket.h" // complete type

// SIZE 0xac
// VTABLE: LEMBALL 0x0049a1e4 TargetNetworkWindow
// VTABLE: LEMBALL 0x0049a1e8 BaseSocket
// VTABLE: LEMBALL 0x0049a210 TcpIpReadSocket
// The original TargetNetworkWindow subobject starts at +0x8c and its table
// points straight to Process. MSVC 4's default vtordisp mode inserts a four-
// byte field and a dynamic thunk instead.
#pragma vtordisp(off)
class TcpIpReadSocket : public virtual BaseCommonSocket, public virtual ReadSocket, public virtual TcpIpCommonSocket {
public:
	bool ReadBuff();
	bool ReadBuffFrom();
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam); // vtable+0x00
	virtual void Closed(unsigned char p_notifyPeer);                                   // vtable+0x0c
	virtual ~TcpIpReadSocket();                                                        // vtable+0x14
};
#pragma vtordisp(on)

// SYNTHETIC: LEMBALL 0x00471dc0 SYMBOL
// ?SysCloseSocket@TcpIpCommonSocket@@WPPPPPPFM@AEHXZ

// SYNTHETIC: LEMBALL 0x00471de0
// TcpIpReadSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00471e20 SYMBOL
// ?SocketError@TcpIpCommonSocket@@WPPPPPPFM@AEXXZ

// SYNTHETIC: LEMBALL 0x00471e50 SYMBOL
// ??_ETcpIpReadSocket@@WCM@AEPAXI@Z

#endif
