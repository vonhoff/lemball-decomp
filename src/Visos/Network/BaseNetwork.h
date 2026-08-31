#ifndef LEMBALL_VISOS_NETWORK_BASENETWORK_H
#define LEMBALL_VISOS_NETWORK_BASENETWORK_H

#include "../../Common.h"
#include "../Foundation/BaseQueueHandler.h" // complete type

// SIZE 0x64
// VTABLE: LEMBALL 0x004991e0
class BaseNetwork : public BaseQueueHandler {
public:
	BaseNetwork();
	virtual int ProcessMsg(Message* p_arg0); // vtable+0x08
	virtual void Initialise();               // vtable+0x0c
	virtual void UnInitialise();             // vtable+0x10
	virtual void SendAllMsg();               // vtable+0x14
	virtual void BeforeDestroyConnections(); // vtable+0x18
	virtual void AfterDestroyConnections();  // vtable+0x1c
	virtual void Process();                  // vtable+0x20
	virtual void* GetNewConnect();           // vtable+0x24
	virtual void* GetNewBroadcast();         // vtable+0x28
	virtual void* GetNewNetworkAddress();    // vtable+0x2c
	virtual void WaitProcess();              // vtable+0x30
	virtual ~BaseNetwork();                  // vtable+0x04
	Connect* NewConnect();
	bool DoInitialise();
	bool Exists(Connect* p_arg0);
	bool FindConnection(NetworkAddress* p_arg0);
	bool Initialise(const char* p_arg0, int p_arg1);
	bool KillUnBornConnection(NetworkAddress* p_arg0);
	bool SendAll(NetworkMessage& p_arg0);
	void AttachMessageQueue(BaseQueueHandler* p_arg0);
	void CtoSRequestConnect(NetworkAddress* p_arg0);
	void CtoSRequestNewPort(NetworkAddress* p_arg0);
	void CtoSgoConnect(NetworkAddress* p_arg0);
	void Delete(Connect* p_arg0);
	void DetachMessageQueue();
	void Establish(NetworkAddress* p_arg0, unsigned char* p_arg1);
	void SetCBuffers(int p_arg0, int p_arg1);
	void SetNcBuffers(unsigned long p_arg0, unsigned long p_arg1, int p_arg2);
	void ShutDown();
	void StoCfailedConnect(NetworkAddress* p_arg0);
	void StoCokConnect(NetworkAddress* p_arg0);

	friend bool VsFNetQuit();
	friend bool VsNetQuit();
	friend class NetworkOptionsProc;
	friend class Game;
	friend class BaseFrontendDrawer;

private:
	char* m_networkName;                      // 0x10
	unsigned int m_shutdownRequested;         // 0x14
	unsigned int m_serverMode;                // 0x18
	unsigned int m_initialized;               // 0x1c
	unsigned int m_initialisePending;         // 0x20
	Broadcast* m_broadcast;                   // 0x24
	Connect* m_firstConnect;                  // 0x28
	Connect* m_lastConnect;                   // 0x2c
	undefined4 m_unk0x30;                     // 0x30
	undefined4 m_unk0x34;                     // 0x34
	BaseQueueHandler* m_pendingAttachQueue;   // 0x38
	void* m_activeStatusItem;                 // 0x3c
	BaseQueueHandler* m_pendingDetachQueue;   // 0x40
	BaseQueueHandler* m_messageQueue;         // 0x44
	unsigned int m_queueTransitionPending;    // 0x48
	unsigned int m_nonCriticalPacketCount;    // 0x4c
	unsigned int m_nonCriticalSequenceWindow; // 0x50
	unsigned int m_nonCriticalSubpacketCount; // 0x54
	unsigned int m_criticalPacketCount;       // 0x58
	unsigned int m_criticalSubpacketCount;    // 0x5c
	undefined4 m_unk0x60;                     // 0x60
};

extern BaseNetwork* g_pBaseNetwork;
#endif
