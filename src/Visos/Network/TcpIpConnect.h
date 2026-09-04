#ifndef LEMBALL_VISOS_NETWORK_TCPIPCONNECT_H
#define LEMBALL_VISOS_NETWORK_TCPIPCONNECT_H

#include "../../Common.h"
#include "Connect.h"       // complete type
#include "TcpIpRwSocket.h" // complete type

// SIZE 0x170
// VTABLE: LEMBALL 0x0049a368 TargetNetworkWindow
// VTABLE: LEMBALL 0x0049a370 BaseSocket's `ReadSocket
// VTABLE: LEMBALL 0x0049a398 BaseSocket's `WriteSocket
// VTABLE: LEMBALL 0x0049a3c8 RwSocket
// VTABLE: LEMBALL 0x0049a3d8 CConnect
class TcpIpConnect : public Connect, public TcpIpRwSocket {
public:
	bool Start(const char* p_localName, const char* p_remoteName);
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam);
	virtual void InitSocket();                        // vtable+0x08
	virtual void Closed(unsigned char p_notifyPeer);  // vtable+0x0c
	virtual void Connect();                           // vtable+0x10
	virtual NetworkMessage* ReceiveAcknowledgement(); // vtable+0x1c
	virtual ~TcpIpConnect();                          // vtable+0x14
	void GotHost(int p_failed);
	void HandleServiceLookupResult(bool p_failed);
	void Listen(NetworkAddress* p_address);
};

// SYNTHETIC: LEMBALL 0x00471b10
// TcpIpConnect::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00471c60 SYMBOL
// ??_ETcpIpConnect@@$4PPPPPPPM@KI@AEPAXI@Z

#endif
