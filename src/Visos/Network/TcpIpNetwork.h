#ifndef LEMBALL_VISOS_NETWORK_TCPIPNETWORK_H
#define LEMBALL_VISOS_NETWORK_TCPIPNETWORK_H

#include "../../Common.h"
#include "BaseNetwork.h" // complete type

// SIZE 0x78
// VTABLE: LEMBALL 0x0049a2dc native window shell
// VTABLE: LEMBALL 0x0049a2a8 BaseNetwork subobject at +0x10
class TcpIpNetwork {
public:
	TcpIpNetwork();
	virtual TcpIpBroadcast* GetNewBroadcast();           // vtable+0x24
	virtual TcpIpConnect* GetNewConnect();               // vtable+0x20
	virtual TcpIpNetworkAddress* GetNewNetworkAddress(); // vtable+0x28
	virtual int Process();                               // vtable+0x00
	virtual void ForceProcess();                         // vtable+0x1c
	virtual void Initialise();                           // vtable+0x0c
	virtual void UnInitialise();                         // vtable+0x10
	virtual ~TcpIpNetwork();                             // vtable+0x04

private:
	void* m_windowHandle;   // 0x04
	undefined m_windowPad[8]; // 0x08
	BaseNetwork m_base;     // 0x10
	unsigned int m_timerId; // 0x74
};

extern unsigned int g_dwTCPIPNetworkThreadId;
extern void* g_hTCPIPNetworkThread;
extern unsigned int g_networkPacketSize;
extern BasePacketHeader* g_pNetworkPacketScratch;
extern unsigned int g_receivedPacketSize;
extern unsigned char* g_pBroadcastPacketTemplate;
extern char* g_szBroadcastPeerName;
extern unsigned int g_broadcastPayloadLength;
extern unsigned short g_broadcastPort;
extern int g_lastNetworkError;

// SYNTHETIC: LEMBALL 0x00471a10
// TcpIpNetwork::`scalar deleting destructor'

#endif

