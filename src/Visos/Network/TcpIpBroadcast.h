#ifndef LEMBALL_VISOS_NETWORK_TCPIPBROADCAST_H
#define LEMBALL_VISOS_NETWORK_TCPIPBROADCAST_H

#include "../../Common.h"
#include "Broadcast.h"     // complete type
#include "TcpIpRwSocket.h" // complete type

// SIZE 0x16c
// VTABLE: LEMBALL 0x0049a158 TargetNetworkWindow
// VTABLE: LEMBALL 0x0049a160 BaseSocket's `ReadSocket
// VTABLE: LEMBALL 0x0049a188 BaseSocket's `WriteSocket
// VTABLE: LEMBALL 0x0049a1b8 RwSocket
// VTABLE: LEMBALL 0x0049a1c8 Broadcast
class TcpIpBroadcast : public Broadcast, public TcpIpRwSocket {
public:
	TcpIpBroadcast();
	virtual bool Start(const char* p_name);                                            // vtable+0x0c
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam); // vtable+0x00
	virtual void Closed(unsigned char p_notifyPeer);                                   // vtable+0x0c
	virtual void GetSpecificAddr(const char* p_name);                                  // vtable+0x08
	virtual void StartListen();                                                        // vtable+0x14
	virtual void StopListen();                                                         // vtable+0x18
	void GotHost(int p_failed);
	void GotName(int p_failed);
	void HandleServiceLookupResult(bool p_failed);
	~TcpIpBroadcast();

private:
	char* m_specificNameBuffer;         // 0x24
	unsigned int m_specificNameRequest; // 0x28
};

// SYNTHETIC: LEMBALL 0x00471f60
// TcpIpBroadcast::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00472000 SYMBOL
// ??_ETcpIpBroadcast@@$4PPPPPPPM@KI@AEPAXI@Z

// SYNTHETIC: LEMBALL 0x00472010 SYMBOL
// ?SendAcknowledgement@TcpIpRwSocket@@$4PPPPPPPM@IA@AEXXZ

// SYNTHETIC: LEMBALL 0x00472020 SYMBOL
// ?ReceiveAcknowledgement@TcpIpRwSocket@@$4PPPPPPPM@IA@AEPAVNetworkMessage@@XZ

#endif
