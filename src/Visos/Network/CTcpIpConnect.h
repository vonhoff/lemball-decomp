#ifndef LEMBALL_VISOS_NETWORK_CTCPIPCONNECT_H
#define LEMBALL_VISOS_NETWORK_CTCPIPCONNECT_H

#include "CConnect.h"       // complete type
#include "CTcpIpRwSocket.h" // complete type
#include "Visos/Network/CBaseCommonSocket.h"
#include "Visos/Network/CReadSocket.h"
#include "Visos/Network/CRwSocket.h"
#include "Visos/Network/CTcpIpCommonSocket.h"
#include "Visos/Network/CTcpIpReadSocket.h"
#include "Visos/Network/CTcpIpWriteSocket.h"
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
class CTcpIpConnect : public CConnect, public CTcpIpRwSocket {
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
// CTcpIpConnect::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00471c60 SYMBOL
// ??_ECTcpIpConnect@@$4PPPPPPPM@KI@AEPAXI@Z

#endif
