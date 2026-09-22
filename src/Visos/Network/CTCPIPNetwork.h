#ifndef LEMBALL_VISOS_NETWORK_CTCPIPNETWORK_H
#define LEMBALL_VISOS_NETWORK_CTCPIPNETWORK_H

#include "../Target/Network/CNetworkWnd.h" // complete type
#include "CBaseNetwork.h"                  // complete type

struct BasePacketHeader;

// SIZE 0x78
// VTABLE: LEMBALL 0x0049a2dc CNetworkWnd
// VTABLE: LEMBALL 0x0049a2a8 CBaseNetwork
// x86 emits only the synthetic deleting destructor.
class CTCPIPNetwork : public CNetworkWnd, public CBaseNetwork {
public:
	CTCPIPNetwork();
	virtual void* GetNewBroadcast();                                                   // vtable+0x28
	virtual void* GetNewConnect();                                                     // vtable+0x24
	virtual void* GetNewNetworkAddress();                                              // vtable+0x2c
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam); // vtable+0x00
	virtual void ForceProcess();                                                       // vtable+0x1c
	virtual void Initialise();                                                         // vtable+0x0c
	virtual void UnInitialise();                                                       // vtable+0x10

private:
	unsigned int m_timerId; // 0x74
};

extern unsigned int g_dwTCPIPNetworkThreadId;
extern void* g_hTCPIPNetworkThread;
extern int g_socketWindowClassRegistered;
extern int g_tcpIpNetworkWindowClassRegistered;
extern unsigned int g_networkPacketSize;
extern BasePacketHeader* g_pNetworkPacketScratch;
extern unsigned int g_receivedPacketSize;
extern BasePacketHeader* g_pBroadcastPacketTemplate;
extern char* g_szBroadcastPeerName;
extern unsigned int g_broadcastPayloadLength;
extern unsigned short g_broadcastPort;
extern unsigned int g_localHostLookupComplete;
extern int g_lastNetworkError;

// SYNTHETIC: LEMBALL 0x00471a10
// CTCPIPNetwork::`scalar deleting destructor'

#endif
