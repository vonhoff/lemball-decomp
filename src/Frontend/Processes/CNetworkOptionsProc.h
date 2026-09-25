#ifndef LEMBALL_FRONTEND_PROCESSES_CNETWORKOPTIONSPROC_H
#define LEMBALL_FRONTEND_PROCESSES_CNETWORKOPTIONSPROC_H

#include "../Base/CBaseFrontendProcess.h"
#include "Visos/Network/CBaseCommonSocket.h"

class CConnect;

class CGame;
class CGameAcceptMessage;
class CGameRejectMessage;
// SIZE 0x38
// VTABLE: LEMBALL 0x004986d8 CBaseQueueHandler
// VTABLE: LEMBALL 0x004986e8 CBaseProcess
class CNetworkOptionsProc : public CBaseFrontendProcess {
public:
	CNetworkOptionsProc(CGame* p_game);
	virtual bool ReceiveCritical(unsigned long p_id, CReadPacket* p_packet, CConnect* p_connection); // vtable+0x08
	virtual void Processing();                                                                       // vtable+0x0c
	virtual ~CNetworkOptionsProc();                                                                  // vtable+0x00
	void Accept(CConnect* p_connection, unsigned int p_ready);
	void NetworkEvent(NetworkEvents p_event);
	void Reject(CConnect* p_connection);
	void Start();
	void Stop();
	void StopBroadcast();

	friend class CNetworkOptionsDrawer;

private:
	unsigned int m_startFailed;          // 0x28
	unsigned int m_started;              // 0x2c
	CGameRejectMessage* m_rejectMessage; // 0x30
	CGameAcceptMessage* m_acceptMessage; // 0x34
};

extern CNetworkOptionsProc* g_pNetworkOptionsProc;

// SYNTHETIC: LEMBALL 0x00455eb0
// CNetworkOptionsProc::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00455ef0
// CNetworkOptionsProc::`vector deleting destructor'

#endif
