#ifndef LEMBALL_VISOS_NETWORK_CTCPIPBROADCAST_H
#define LEMBALL_VISOS_NETWORK_CTCPIPBROADCAST_H

#include "CBroadcast.h"
#include "CTCPIPRWSocket.h"

// SIZE 0x16c
// VTABLE: LEMBALL 0x0049a158 CNetworkWnd
// VTABLE: LEMBALL 0x0049a160 CBaseSocket's `CReadSocket
// VTABLE: LEMBALL 0x0049a188 CBaseSocket's `CWriteSocket
// VTABLE: LEMBALL 0x0049a1b8 CRwSocket
// VTABLE: LEMBALL 0x0049a1c8 CBroadcast
#pragma warning(disable : 4250)
class CTCPIPBroadcast : public CBroadcast, public CTCPIPRWSocket {
public:
	CTCPIPBroadcast();
	virtual bool Start(const char* p_name);                                            // vtable+0x0c
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam); // vtable+0x00
	virtual void Closed(int p_notifyPeer);                                             // vtable+0x0c
	virtual void GetSpecificAddr(const char* p_name);                                  // vtable+0x08
	virtual void StartListen();                                                        // vtable+0x14
	virtual void StopListen();                                                         // vtable+0x18
	void GotHost(int p_failed);
	void GotName(int p_failed);
	void HandleServiceLookupResult(bool p_failed);
	~CTCPIPBroadcast();

private:
	char* m_specificNameBuffer;         // 0x24
	unsigned int m_specificNameRequest; // 0x28
};
#pragma warning(default : 4250)

// SYNTHETIC: LEMBALL 0x00471f60
// CTCPIPBroadcast::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00472000 SYMBOL
// ??_ECTCPIPBroadcast@@$4PPPPPPPM@KI@AEPAXI@Z

// SYNTHETIC: LEMBALL 0x00472010 SYMBOL
// ?SendAcknowledgement@CTCPIPRWSocket@@$4PPPPPPPM@IA@AEXXZ

// SYNTHETIC: LEMBALL 0x00472020 SYMBOL
// ?ReceiveAcknowledgement@CTCPIPRWSocket@@$4PPPPPPPM@IA@AEPAVCNetworkMessage@@XZ

// GLOBAL: LEMBALL 0x0049a0d0
// CTCPIPBroadcast::`vbtable'{for `CTCPIPWriteSocket'}

// GLOBAL: LEMBALL 0x0049a0e0
// CTCPIPBroadcast::`vbtable'{for `CTCPIPReadSocket'}

// GLOBAL: LEMBALL 0x0049a0f0
// CTCPIPBroadcast::`vbtable'{for `CTCPIPCommonSocket'}

// GLOBAL: LEMBALL 0x0049a0f8
// CTCPIPBroadcast::`vbtable'{for `CRwSocket'}

// GLOBAL: LEMBALL 0x0049a108
// CTCPIPBroadcast::`vbtable'{for `CReadSocket'}

// GLOBAL: LEMBALL 0x0049a110
// CTCPIPBroadcast::`vbtable'{for `CWriteSocket'}

// GLOBAL: LEMBALL 0x0049a118
// CTCPIPBroadcast::`vbtable'{for `CTCPIPRWSocket'}

// GLOBAL: LEMBALL 0x0049a138
// CTCPIPBroadcast::`vbtable'{for `CBroadcast'}

#endif
