#ifndef LEMBALL_VISOS_NETWORK_CTCPIPCONNECT_H
#define LEMBALL_VISOS_NETWORK_CTCPIPCONNECT_H

#include "CConnect.h"       // complete type
#include "CTCPIPRWSocket.h" // complete type
#include "Visos/Network/CBaseCommonSocket.h"
#include "Visos/Network/CReadSocket.h"
#include "Visos/Network/CRwSocket.h"
#include "Visos/Network/CTCPIPCommonSocket.h"
#include "Visos/Network/CTCPIPReadSocket.h"
#include "Visos/Network/CTCPIPWriteSocket.h"
#include "Visos/Network/CWriteSocket.h"

// SIZE 0x170
// VTABLE: LEMBALL 0x0049a368 CNetworkWnd
// VTABLE: LEMBALL 0x0049a370 CBaseSocket's `CReadSocket
// VTABLE: LEMBALL 0x0049a398 CBaseSocket's `CWriteSocket
// VTABLE: LEMBALL 0x0049a3c8 CRwSocket
// VTABLE: LEMBALL 0x0049a3d8 CConnect
// x86 emits only the synthetic deleting destructor.
// Original PDB/x86 has these inherited dominance adjustor thunks.
#pragma warning(disable : 4250)
class CTCPIPConnect : public CConnect, public CTCPIPRWSocket {
public:
	bool Start(const char* p_localName, const char* p_remoteName);
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam);
	virtual void InitSocket();                         // vtable+0x08
	virtual void Closed(int p_notifyPeer);             // vtable+0x0c
	virtual void Connect();                            // vtable+0x10
	virtual CNetworkMessage* ReceiveAcknowledgement(); // vtable+0x1c
	void GotHost(int p_failed);
	void HandleServiceLookupResult(bool p_failed);
	void Listen(CNetworkAddress* p_address);
};
#pragma warning(default : 4250)

// SYNTHETIC: LEMBALL 0x00471b10
// CTCPIPConnect::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00471c60 SYMBOL
// ??_ECTCPIPConnect@@$4PPPPPPPM@KI@AEPAXI@Z

// GLOBAL: LEMBALL 0x0049a2e0
// CTCPIPConnect::`vbtable'{for `CTCPIPWriteSocket'}

// GLOBAL: LEMBALL 0x0049a2f0
// CTCPIPConnect::`vbtable'{for `CTCPIPReadSocket'}

// GLOBAL: LEMBALL 0x0049a300
// CTCPIPConnect::`vbtable'{for `CTCPIPCommonSocket'}

// GLOBAL: LEMBALL 0x0049a308
// CTCPIPConnect::`vbtable'{for `CRwSocket'}

// GLOBAL: LEMBALL 0x0049a318
// CTCPIPConnect::`vbtable'{for `CReadSocket'}

// GLOBAL: LEMBALL 0x0049a320
// CTCPIPConnect::`vbtable'{for `CWriteSocket'}

// GLOBAL: LEMBALL 0x0049a328
// CTCPIPConnect::`vbtable'{for `CTCPIPRWSocket'}

// GLOBAL: LEMBALL 0x0049a348
// CTCPIPConnect::`vbtable'{for `CConnect'}

#endif
