#ifndef LEMBALL_VISOS_NETWORK_CTCPIPRWSOCKET_H
#define LEMBALL_VISOS_NETWORK_CTCPIPRWSOCKET_H

#include "CRwSocket.h"         // complete type
#include "CTcpIpReadSocket.h"  // complete type
#include "CTcpIpWriteSocket.h" // complete type
#include "Visos/Network/CBaseCommonSocket.h"
#include "Visos/Network/CReadSocket.h"
#include "Visos/Network/CTcpIpCommonSocket.h"
#include "Visos/Network/CWriteSocket.h"

// SIZE 0x144
// VTABLE: LEMBALL 0x0049a220 TargetNetworkWindow
// VTABLE: LEMBALL 0x0049a228 CBaseSocket's `CReadSocket
// VTABLE: LEMBALL 0x0049a250 CBaseSocket's `CWriteSocket
// VTABLE: LEMBALL 0x0049a280 CTcpIpRwSocket
// x86 emits only the synthetic deleting destructor.
// Original PDB/x86 has these inherited dominance adjustor thunks.
#pragma warning(disable : 4250)
class CTcpIpRwSocket : public virtual CRwSocket, public virtual CTcpIpReadSocket, public virtual CTcpIpWriteSocket {
public:
	// The implicit constructor clears inherited vtordisp fields, as in 0x00471830.
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam); // vtable+0x00
	virtual void Closed(int p_notifyPeer);                                             // vtable+0x0c
	virtual CNetworkMessage* ReceiveAcknowledgement();                                 // vtable+0x1c
	virtual void SendAcknowledgement();                                                // vtable+0x18
};
#pragma warning(default : 4250)

// FUNCTION: LEMBALL 0x00471830 SYMBOL
// ??0CTcpIpRwSocket@@QAE@XZ

// SYNTHETIC: LEMBALL 0x00471af0 SYMBOL
// ?SysCloseSocket@CTcpIpCommonSocket@@WPPPPPONI@AEHXZ

// SYNTHETIC: LEMBALL 0x00471b60 SYMBOL
// ?SocketError@CTcpIpCommonSocket@@WPPPPPONI@AEXXZ

// SYNTHETIC: LEMBALL 0x00471cd0
// CTcpIpRwSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00471d90 SYMBOL
// ??_ECTcpIpRwSocket@@$4PPPPPPPM@KI@AEPAXI@Z

// GLOBAL: LEMBALL 0x0049a3f0
// CTcpIpRwSocket::`vbtable'{for `CTcpIpWriteSocket'}

// GLOBAL: LEMBALL 0x0049a400
// CTcpIpRwSocket::`vbtable'{for `CTcpIpReadSocket'}

// GLOBAL: LEMBALL 0x0049a410
// CTcpIpRwSocket::`vbtable'{for `CTcpIpCommonSocket'}

// GLOBAL: LEMBALL 0x0049a418
// CTcpIpRwSocket::`vbtable'{for `CRwSocket'}

// GLOBAL: LEMBALL 0x0049a428
// CTcpIpRwSocket::`vbtable'{for `CReadSocket'}

// GLOBAL: LEMBALL 0x0049a430
// CTcpIpRwSocket::`vbtable'{for `CWriteSocket'}

// GLOBAL: LEMBALL 0x0049a438
// CTcpIpRwSocket::`vbtable'

#endif
