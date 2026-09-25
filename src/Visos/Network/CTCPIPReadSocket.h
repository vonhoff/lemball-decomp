#ifndef LEMBALL_VISOS_NETWORK_CTCPIPREADSOCKET_H
#define LEMBALL_VISOS_NETWORK_CTCPIPREADSOCKET_H

#include "CBaseCommonSocket.h"
#include "CReadSocket.h"
#include "CTCPIPCommonSocket.h"

// SIZE 0xac
// VTABLE: LEMBALL 0x0049a1e4 CNetworkWnd
// VTABLE: LEMBALL 0x0049a1e8 CBaseSocket
// VTABLE: LEMBALL 0x0049a210 CTCPIPReadSocket
#pragma warning(disable : 4250)
#pragma vtordisp(off)
class CTCPIPReadSocket : public virtual CBaseCommonSocket,
						 public virtual CReadSocket,
						 public virtual CTCPIPCommonSocket {
public:
	bool ReadBuff();
	bool ReadBuffFrom();
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam); // vtable+0x00
	virtual void Closed(int p_notifyPeer);                                             // vtable+0x0c
};
#pragma vtordisp(on)
#pragma warning(default : 4250)

// SYNTHETIC: LEMBALL 0x00471dc0 SYMBOL
// ?SysCloseSocket@CTCPIPCommonSocket@@WPPPPPPFM@AEHXZ

// SYNTHETIC: LEMBALL 0x00471de0
// CTCPIPReadSocket::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00471e20 SYMBOL
// ?SocketError@CTCPIPCommonSocket@@WPPPPPPFM@AEXXZ

// SYNTHETIC: LEMBALL 0x00471e50 SYMBOL
// ??_ECTCPIPReadSocket@@WCM@AEPAXI@Z

#endif
