#ifndef LEMBALL_FRONTEND_PROCESSES_NETWORKOPTIONSPROC_H
#define LEMBALL_FRONTEND_PROCESSES_NETWORKOPTIONSPROC_H

#include "../../Visos/Network/Connect.h"
#include "../Base/BaseFrontendProcess.h" // complete type

class BaseProcess;
class BaseQueueHandler;
class Game;
class GameAcceptMessage;
class GameRejectMessage;
class NetworkOptionsDrawer;
class ReadPacket;
// SIZE 0x38
// VTABLE: LEMBALL 0x004986d8 BaseQueueHandler
// VTABLE: LEMBALL 0x004986e8 BaseProcess
class NetworkOptionsProc : public BaseFrontendProcess {
public:
	NetworkOptionsProc(Game* p_arg0);
	virtual bool ReceiveCritical(unsigned long p_id, ReadPacket* p_packet, CConnect* p_connection); // vtable+0x08
	virtual void Processing();                                                                      // vtable+0x0c
	// 68K 0x10808a36 __dt__19CNetworkOptionsProcFv
	virtual ~NetworkOptionsProc(); // vtable+0x00
	void Accept(CConnect* p_connection, unsigned int p_ready);
	void NetworkEvent(NetworkEvents p_event);
	void Reject(CConnect* p_connection);
	void Start();
	void Stop();
	void StopBroadcast();

	friend class NetworkOptionsDrawer;

private:
	unsigned int m_startFailed;         // 0x28
	unsigned int m_started;             // 0x2c
	GameRejectMessage* m_rejectMessage; // 0x30
	GameAcceptMessage* m_acceptMessage; // 0x34
};

extern NetworkOptionsProc* g_pNetworkOptionsProc;

// SYNTHETIC: LEMBALL 0x00455eb0
// NetworkOptionsProc::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00455ef0
// NetworkOptionsProc::`vector deleting destructor'

#endif
