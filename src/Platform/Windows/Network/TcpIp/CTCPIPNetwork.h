#ifndef LEMBALL_VISOS_NETWORK_CTCPIPNETWORK_H
#define LEMBALL_VISOS_NETWORK_CTCPIPNETWORK_H

#include "Platform/Windows/Network/CNetworkWnd.h"
#include "Multiplayer/Transport/CBaseNetwork.h"

#define TCPIP_MESSAGE_FORCE_PROCESS 0x444
#define TCPIP_TIMER_ID 0x12345678
#define TCPIP_TIMER_INTERVAL_MS 10

struct BasePacketHeader;

// SIZE 0x78
// VTABLE: LEMBALL 0x0049a2dc CNetworkWnd
// VTABLE: LEMBALL 0x0049a2a8 CBaseNetwork
class CTCPIPNetwork : public CNetworkWnd, public CBaseNetwork {
public:
	using CBaseNetwork::Process;

	CTCPIPNetwork();
	virtual void* GetNewBroadcast();                                                   // vtable+0x28
	virtual void* GetNewConnect();                                                     // vtable+0x24
	virtual CNetworkAddress* GetNewNetworkAddress();                                   // vtable+0x2c
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam); // vtable+0x00
	virtual void ForceProcess();                                                       // vtable+0x1c
	virtual void Initialise();                                                         // vtable+0x0c
	virtual void UnInitialise();                                                       // vtable+0x10

private:
	unsigned int m_timerId; // 0x74
};

extern unsigned long g_dwTCPIPNetworkThreadId;
extern void* g_hTCPIPNetworkThread;
extern int g_socketWindowClassRegistered;
extern int g_tcpIpNetworkWindowClassRegistered;

// SYNTHETIC: LEMBALL 0x00471a10
// CTCPIPNetwork::`scalar deleting destructor'

#endif
