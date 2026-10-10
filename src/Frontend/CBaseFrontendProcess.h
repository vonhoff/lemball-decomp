#ifndef LEMBALL_FRONTEND_BASE_CBASEFRONTENDPROCESS_H
#define LEMBALL_FRONTEND_BASE_CBASEFRONTENDPROCESS_H

#include "Application/CProcess.h"
#include "CUserActionMessage.h"
#include "Engine/Queues/CBaseQueueHandler.h"
class CConnect;

class CGame;
class CReadPacket;
struct tagMESSAGE;
// SIZE 0x28
// VTABLE: LEMBALL 0x00497938 CBaseQueueHandler
// VTABLE: LEMBALL 0x00497948 CProcess
class CBaseFrontendProcess : public CProcess, public CBaseQueueHandler {
public:
	CBaseFrontendProcess(CGame* p_game);
	virtual ~CBaseFrontendProcess();                                                                 // vtable+0x00
	virtual void Process();                                                                          // vtable+0x04
	virtual bool ReceiveCritical(unsigned long p_id, CReadPacket* p_packet, CConnect* p_connection); // vtable+0x08
	virtual void Processing();                                                                       // vtable+0x0c
	virtual bool ProcessMessages(tagMESSAGE* p_message);                                             // vtable+0x10
	int ProcessMsg(tagMESSAGE* p_message);
	void Action(eUserActions p_action, eUserActionStages p_stage);
	CBaseFrontendProcess();

	friend class CNetworkOptionsProc;

private:
	bool m_networkWasActive;                 // 0x1c
	CUserActionMessage* m_userActionMessage; // 0x20
	CGame* m_game;                           // 0x24
};

extern int g_nFrontendAutoFlowToggle;

extern CBaseFrontendProcess* g_pCurrentFrontendProcess;

// SYNTHETIC: LEMBALL 0x004472b0
// CBaseFrontendProcess::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x004472e0
// CBaseFrontendProcess::`vector deleting destructor'

#endif
