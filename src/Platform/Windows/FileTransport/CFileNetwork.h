#ifndef LEMBALL_VISOS_NETWORK_CFILENETWORK_H
#define LEMBALL_VISOS_NETWORK_CFILENETWORK_H

#include "Platform/Windows/Network/CNetworkWnd.h"
#include "Multiplayer/Transport/CBaseNetwork.h"

#define FILE_NETWORK_MESSAGE_FORCE_PROCESS 0x444
#define FILE_NETWORK_TIMER_ID 0x12345679
#define FILE_NETWORK_TIMER_INTERVAL_MS 20

// SIZE 0x7c
// VTABLE: LEMBALL 0x0049a02c CNetworkWnd
// VTABLE: LEMBALL 0x00499ff8 CBaseNetwork
class CFileNetwork : public CNetworkWnd, public CBaseNetwork {
public:
	using CBaseNetwork::Process;

	CFileNetwork();
	virtual int Process(unsigned int p_message, unsigned int p_wParam, long p_lParam); // vtable+0x00
	virtual void* GetNewBroadcast();                                                   // vtable+0x28
	virtual void* GetNewConnect();                                                     // vtable+0x24
	virtual void* GetNewNetworkAddress();                                              // vtable+0x2c
	virtual void AfterDestroyConnections();                                            // vtable+0x1c
	virtual void BeforeDestroyConnections();                                           // vtable+0x18
	virtual void ForceProcess();                                                       // vtable+0x20
	virtual void Initialise();                                                         // vtable+0x0c
	virtual void UnInitialise();                                                       // vtable+0x10
	void ResetTimer(unsigned int p_interval);
	void Setup(const char* p_peerName, const char* p_path);

private:
	unsigned int m_timerId;        // 0x74
	unsigned int m_alternateTimer; // 0x78
};

extern unsigned long g_dwFileNetworkThreadId;
extern void* g_hFileNetworkThread;
extern int g_fileNetworkWindowClassRegistered;
extern char* g_pFileBroadcastData;

// SYNTHETIC: LEMBALL 0x0046f970
// CFileNetwork::`scalar deleting destructor'

#endif
