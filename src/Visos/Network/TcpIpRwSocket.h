#ifndef LEMBALL_VISOS_NETWORK_TCPIPRWSOCKET_H
#define LEMBALL_VISOS_NETWORK_TCPIPRWSOCKET_H

#include "../../Common.h"
#include "RwSocket.h"         // complete type
#include "TcpIpReadSocket.h"  // complete type
#include "TcpIpWriteSocket.h" // complete type

// SIZE 0x144
// VTABLE: LEMBALL 0x0049a220 TargetNetworkWindow
// VTABLE: LEMBALL 0x0049a228 BaseSocket's `ReadSocket
// VTABLE: LEMBALL 0x0049a250 BaseSocket's `WriteSocket
// VTABLE: LEMBALL 0x0049a280 TcpIpRwSocket
// 68K 0x101063e4 __dt__14CTCPIPRWSocketFv
// x86 emits only the synthetic deleting destructor.
// Original PDB/x86 has these inherited dominance adjustor thunks.
#pragma warning(disable : 4250)
class TcpIpRwSocket : public virtual RwSocket, public virtual TcpIpReadSocket, public virtual TcpIpWriteSocket {
public:
	// The implicit constructor clears inherited vtordisp fields, as in 0x00471830.
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam); // vtable+0x00
	virtual void Closed(int p_notifyPeer);                                             // vtable+0x0c
	virtual NetworkMessage* ReceiveAcknowledgement();                                  // vtable+0x1c
	virtual void SendAcknowledgement();                                                // vtable+0x18
};
#pragma warning(default : 4250)

// FUNCTION: LEMBALL 0x00471830 SYMBOL
// ??0TcpIpRwSocket@@QAE@XZ

// SYNTHETIC: LEMBALL 0x00471af0 SYMBOL
// ?SysCloseSocket@TcpIpCommonSocket@@WPPPPPONI@AEHXZ

// SYNTHETIC: LEMBALL 0x00471b60 SYMBOL
// ?SocketError@TcpIpCommonSocket@@WPPPPPONI@AEXXZ

// SYNTHETIC: LEMBALL 0x00471cd0
// TcpIpRwSocket::`scalar deleting destructor'

// GLOBAL: LEMBALL 0x0049a3f0
// TcpIpRwSocket::`vbtable'{for `TcpIpWriteSocket'}

// GLOBAL: LEMBALL 0x0049a400
// TcpIpRwSocket::`vbtable'{for `TcpIpReadSocket'}

// GLOBAL: LEMBALL 0x0049a410
// TcpIpRwSocket::`vbtable'{for `TcpIpCommonSocket'}

// GLOBAL: LEMBALL 0x0049a418
// TcpIpRwSocket::`vbtable'{for `RwSocket'}

// GLOBAL: LEMBALL 0x0049a428
// TcpIpRwSocket::`vbtable'{for `ReadSocket'}

// GLOBAL: LEMBALL 0x0049a430
// TcpIpRwSocket::`vbtable'{for `WriteSocket'}

// GLOBAL: LEMBALL 0x0049a438
// TcpIpRwSocket::`vbtable'

#endif
