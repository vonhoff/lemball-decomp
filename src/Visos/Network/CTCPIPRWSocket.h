#ifndef LEMBALL_VISOS_NETWORK_CTCPIPRWSOCKET_H
#define LEMBALL_VISOS_NETWORK_CTCPIPRWSOCKET_H

#include "CRwSocket.h"         // complete type
#include "CTCPIPReadSocket.h"  // complete type
#include "CTCPIPWriteSocket.h" // complete type
#include "Visos/Network/CBaseCommonSocket.h"
#include "Visos/Network/CReadSocket.h"
#include "Visos/Network/CTCPIPCommonSocket.h"
#include "Visos/Network/CWriteSocket.h"

// SIZE 0x144
// VTABLE: LEMBALL 0x0049a220 CNetworkWnd
// VTABLE: LEMBALL 0x0049a228 CBaseSocket's `CReadSocket
// VTABLE: LEMBALL 0x0049a250 CBaseSocket's `CWriteSocket
// VTABLE: LEMBALL 0x0049a280 CTCPIPRWSocket
// x86 emits only the synthetic deleting destructor.
// Original PDB/x86 has these inherited dominance adjustor thunks.
#pragma warning(disable : 4250)
class CTCPIPRWSocket : public virtual CRwSocket, public virtual CTCPIPReadSocket, public virtual CTCPIPWriteSocket {
public:
	// The implicit constructor clears inherited vtordisp fields, as in 0x00471830.
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam); // vtable+0x00
	virtual void Closed(int p_notifyPeer);                                             // vtable+0x0c
	virtual CNetworkMessage* ReceiveAcknowledgement();                                 // vtable+0x1c
	virtual void SendAcknowledgement();                                                // vtable+0x18
};
#pragma warning(default : 4250)

// FUNCTION: LEMBALL 0x00471830 SYMBOL
// ??0CTCPIPRWSocket@@QAE@XZ

// SYNTHETIC: LEMBALL 0x00471af0 SYMBOL
// ?SysCloseSocket@CTCPIPCommonSocket@@WPPPPPONI@AEHXZ

// SYNTHETIC: LEMBALL 0x00471b60 SYMBOL
// ?SocketError@CTCPIPCommonSocket@@WPPPPPONI@AEXXZ

// SYNTHETIC: LEMBALL 0x00471cd0
// CTCPIPRWSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00471d90 SYMBOL
// ??_ECTCPIPRWSocket@@$4PPPPPPPM@KI@AEPAXI@Z

// GLOBAL: LEMBALL 0x0049a3f0
// CTCPIPRWSocket::`vbtable'{for `CTCPIPWriteSocket'}

// GLOBAL: LEMBALL 0x0049a400
// CTCPIPRWSocket::`vbtable'{for `CTCPIPReadSocket'}

// GLOBAL: LEMBALL 0x0049a410
// CTCPIPRWSocket::`vbtable'{for `CTCPIPCommonSocket'}

// GLOBAL: LEMBALL 0x0049a418
// CTCPIPRWSocket::`vbtable'{for `CRwSocket'}

// GLOBAL: LEMBALL 0x0049a428
// CTCPIPRWSocket::`vbtable'{for `CReadSocket'}

// GLOBAL: LEMBALL 0x0049a430
// CTCPIPRWSocket::`vbtable'{for `CWriteSocket'}

// GLOBAL: LEMBALL 0x0049a438
// CTCPIPRWSocket::`vbtable'

#endif
