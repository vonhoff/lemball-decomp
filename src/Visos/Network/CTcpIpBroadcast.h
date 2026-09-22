#ifndef LEMBALL_VISOS_NETWORK_CTCPIPBROADCAST_H
#define LEMBALL_VISOS_NETWORK_CTCPIPBROADCAST_H

#include "CBroadcast.h"     // complete type
#include "CTcpIpRwSocket.h" // complete type

// SIZE 0x16c
// VTABLE: LEMBALL 0x0049a158 CNetworkWnd
// VTABLE: LEMBALL 0x0049a160 CBaseSocket's `CReadSocket
// VTABLE: LEMBALL 0x0049a188 CBaseSocket's `CWriteSocket
// VTABLE: LEMBALL 0x0049a1b8 CRwSocket
// VTABLE: LEMBALL 0x0049a1c8 CBroadcast
// Original PDB/x86 has these inherited dominance adjustor thunks.
#pragma warning(disable : 4250)
class CTcpIpBroadcast : public CBroadcast, public CTcpIpRwSocket {
public:
	CTcpIpBroadcast();
	virtual bool Start(const char* p_name);                                            // vtable+0x0c
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam); // vtable+0x00
	virtual void Closed(int p_notifyPeer);                                             // vtable+0x0c
	virtual void GetSpecificAddr(const char* p_name);                                  // vtable+0x08
	virtual void StartListen();                                                        // vtable+0x14
	virtual void StopListen();                                                         // vtable+0x18
	void GotHost(int p_failed);
	void GotName(int p_failed);
	void HandleServiceLookupResult(bool p_failed);
	~CTcpIpBroadcast();

private:
	char* m_specificNameBuffer;         // 0x24
	unsigned int m_specificNameRequest; // 0x28
};
#pragma warning(default : 4250)

// SYNTHETIC: LEMBALL 0x00471f60
// CTcpIpBroadcast::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00472000 SYMBOL
// ??_ECTcpIpBroadcast@@$4PPPPPPPM@KI@AEPAXI@Z

// SYNTHETIC: LEMBALL 0x00472010 SYMBOL
// ?SendAcknowledgement@CTcpIpRwSocket@@$4PPPPPPPM@IA@AEXXZ

// SYNTHETIC: LEMBALL 0x00472020 SYMBOL
// ?ReceiveAcknowledgement@CTcpIpRwSocket@@$4PPPPPPPM@IA@AEPAVCNetworkMessage@@XZ

// GLOBAL: LEMBALL 0x0049a0d0
// CTcpIpBroadcast::`vbtable'{for `CTcpIpWriteSocket'}

// GLOBAL: LEMBALL 0x0049a0e0
// CTcpIpBroadcast::`vbtable'{for `CTcpIpReadSocket'}

// GLOBAL: LEMBALL 0x0049a0f0
// CTcpIpBroadcast::`vbtable'{for `CTcpIpCommonSocket'}

// GLOBAL: LEMBALL 0x0049a0f8
// CTcpIpBroadcast::`vbtable'{for `CRwSocket'}

// GLOBAL: LEMBALL 0x0049a108
// CTcpIpBroadcast::`vbtable'{for `CReadSocket'}

// GLOBAL: LEMBALL 0x0049a110
// CTcpIpBroadcast::`vbtable'{for `CWriteSocket'}

// GLOBAL: LEMBALL 0x0049a118
// CTcpIpBroadcast::`vbtable'{for `CTcpIpRwSocket'}

// GLOBAL: LEMBALL 0x0049a138
// CTcpIpBroadcast::`vbtable'{for `CBroadcast'}

#endif
