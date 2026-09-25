#ifndef LEMBALL_VISOS_NETWORK_CBASENETWORK_H
#define LEMBALL_VISOS_NETWORK_CBASENETWORK_H

#include "../Foundation/CBaseQueueHandler.h"
#include "CConnect.h"

class CBroadcast;
class CNetworkAddress;
class CNetworkMessage;

// SIZE 0x64
// VTABLE: LEMBALL 0x004991e0
class CBaseNetwork : public CBaseQueueHandler {
public:
	CBaseNetwork();
	virtual int ProcessMsg(Message* p_message); // vtable+0x08
	virtual void Initialise() = 0;              // vtable+0x0c
	virtual void UnInitialise() = 0;            // vtable+0x10
	virtual void Process();                     // vtable+0x14
	virtual void BeforeDestroyConnections();    // vtable+0x18
	virtual void AfterDestroyConnections();     // vtable+0x1c
	virtual void ForceProcess() = 0;            // vtable+0x20
	virtual void* GetNewConnect() = 0;          // vtable+0x24
	virtual void* GetNewBroadcast() = 0;        // vtable+0x28
	virtual void* GetNewNetworkAddress() = 0;   // vtable+0x2c
	virtual void WaitProcess();                 // vtable+0x30
	virtual ~CBaseNetwork();                    // vtable+0x04
	CConnect* NewConnect();
	bool DoInitialise();
	bool Exists(CConnect* p_connection);
	CConnect* FindConnection(CNetworkAddress* p_address);
	bool Initialise(const char* p_networkName, int p_packetSize);
	void KillUnBornConnection(CNetworkAddress* p_address);
	bool SendAll(CNetworkMessage& p_message);
	void AttachMessageQueue(CBaseQueueHandler* p_queueHandler);
	void CtoSRequestConnect(CNetworkAddress* p_address);
	void CtoSRequestNewPort(CNetworkAddress* p_address);
	void CtoSGOConnect(CNetworkAddress* p_address);
	void Delete(CConnect* p_connection);
	void DetachMessageQueue();
	void Establish(CNetworkAddress* p_address, unsigned char* p_data);
	void SetCBuffers(int p_packetCount, int p_messageCapacity);
	void SetNCBuffers(unsigned long p_lastSinglePacketMessageId, unsigned long p_lastMessageId, int p_messageCapacity);
	void ShutDown();
	void StoCFAILEDConnect(CNetworkAddress* p_address);
	void StoCOKConnect(CNetworkAddress* p_address);
	void HandleNewConnectionEvent(const char* p_localName, const char* p_remoteName);
	CConnect* FindEventConnection(CNetworkAddress* p_address);
	void HandleConnectionMessage(CNetworkAddress* p_address);

	friend bool VSFNET_Quit();
	friend bool VSNET_Quit();
	friend class CNetworkOptionsProc;
	friend class CNetworkManager;
	friend class CGame;
	friend class CBaseFrontendDrawer;
	friend class CFileNetwork;
	friend class CConnect;
	friend class CWriteSocket;
	friend class C2D;

private:
	char* m_networkName; // 0x10

public:
	unsigned int m_shutdownRequested; // 0x14
	unsigned int m_serverMode;        // 0x18
	unsigned int m_initialized;       // 0x1c
	unsigned int m_initialisePending; // 0x20

private:
	CBroadcast* m_broadcast;                   // 0x24
	CConnect* m_firstConnect;                  // 0x28
	CConnect* m_lastConnect;                   // 0x2c
	unsigned int m_suspendBroadcastOnConnect;  // 0x30
	unsigned int m_broadcastMode;              // 0x34
	CBaseQueueHandler* m_pendingAttachQueue;   // 0x38
	void* m_activeStatusItem;                  // 0x3c
	CBaseQueueHandler* m_pendingDetachQueue;   // 0x40
	CBaseQueueHandler* m_messageQueue;         // 0x44
	unsigned int m_queueTransitionPending;     // 0x48
	unsigned int m_lastSinglePacketMessageId;  // 0x4c
	unsigned int m_lastNonCriticalMessageId;   // 0x50
	unsigned int m_nonCriticalMessageCapacity; // 0x54
	unsigned int m_criticalPacketCount;        // 0x58
	unsigned int m_criticalMessageCapacity;    // 0x5c
	unsigned int m_criticalRetryLimit;         // 0x60
};

extern CBaseNetwork* g_pBaseNetwork;
// SYNTHETIC: LEMBALL 0x00462ad0
// CBaseNetwork::`scalar deleting destructor'

#endif
