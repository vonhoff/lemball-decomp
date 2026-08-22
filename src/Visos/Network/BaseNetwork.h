#ifndef LEMBALL_VISOS_NETWORK_BASENETWORK_H
#define LEMBALL_VISOS_NETWORK_BASENETWORK_H

#include "../../Common.h"
#include "../Foundation/BaseQueueHandler.h" // complete type

// SIZE 0x64
// VTABLE: LEMBALL 0x004991e0
class BaseNetwork : public BaseQueueHandler {
public:
	BaseNetwork();
	Connect* NewConnect();
	bool DoInitialise();
	bool Exists(Connect* p_arg0);
	bool FindConnection(NetworkAddress* p_arg0);
	bool Initialise(const char* p_arg0, int p_arg1);
	bool KillUnBornConnection(NetworkAddress* p_arg0);
	bool SendAll(NetworkMessage& p_arg0);
	virtual int ProcessMsg(Message* p_arg0); // vtable+0x08
	void AfterDestroyConnections();
	void AttachMessageQueue(BaseQueueHandler* p_arg0);
	void BeforeDestroyConnections();
	void CtoSRequestConnect(NetworkAddress* p_arg0);
	void CtoSRequestNewPort(NetworkAddress* p_arg0);
	void CtoSgoConnect(NetworkAddress* p_arg0);
	void Delete(Connect* p_arg0);
	void DetachMessageQueue();
	void Establish(NetworkAddress* p_arg0, unsigned char* p_arg1);
	void Process();
	void SetCBuffers(int p_arg0, int p_arg1);
	void SetNcBuffers(unsigned long p_arg0, unsigned long p_arg1, int p_arg2);
	void ShutDown();
	void StoCfailedConnect(NetworkAddress* p_arg0);
	void StoCokConnect(NetworkAddress* p_arg0);
	void WaitProcess();
	~BaseNetwork();

private:
	char* m_networkName;                      // 0x10
	unsigned int m_shutdownRequested;         // 0x14
	unsigned int m_serverMode;                // 0x18
	unsigned int m_initialized;               // 0x1c
	unsigned int m_initialisePending;         // 0x20
	void* m_broadcast;                        // 0x24
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

// Confirmed class-scoped globals.
extern BaseNetwork* g_pBaseNetwork;
#endif
