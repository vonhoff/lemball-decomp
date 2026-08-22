#ifndef LEMBALL_FRONTEND_PROCESSES_NETWORKOPTIONSPROC_H
#define LEMBALL_FRONTEND_PROCESSES_NETWORKOPTIONSPROC_H

#include "../../Common.h"
#include "../Base/BaseFrontendProcess.h" // complete type

// SIZE 0x38
// VTABLE: LEMBALL 0x004986e8
class NetworkOptionsProc : public BaseFrontendProcess {
public:
	NetworkOptionsProc(Game* p_arg0);
	virtual bool ReceiveCritical(unsigned long p_id, ReadPacket* p_packet, Connect* p_connection); // vtable+0x08
	virtual void Processing();                                                                     // vtable+0x0c
	virtual ~NetworkOptionsProc();                                                                 // vtable+0x00
	void Accept(Connect* p_connection, unsigned char p_ready);
	void NetworkEvent(int p_event);
	void Reject(Connect* p_connection);
	void Start();
	void Stop();
	void StopBroadcast();

private:
	unsigned int m_startFailed;         // 0x28
	unsigned int m_started;             // 0x2c
	GameRejectMessage* m_rejectMessage; // 0x30
	GameAcceptMessage* m_acceptMessage; // 0x34
};

#endif
