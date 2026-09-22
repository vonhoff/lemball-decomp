#ifndef LEMBALL_FRONTEND_BASE_CBASEFRONTENDPROCESS_H
#define LEMBALL_FRONTEND_BASE_CBASEFRONTENDPROCESS_H

#include "../../Visos/Foundation/CBaseProcess.h"      // complete type
#include "../../Visos/Foundation/CBaseQueueHandler.h" // complete type
#include "../Support/CUserActionMessage.h"
class CConnect;

class CGame;
class CReadPacket;
struct Message;
// SIZE 0x28
// VTABLE: LEMBALL 0x00497938 CBaseQueueHandler
// VTABLE: LEMBALL 0x00497948 CBaseProcess
class CBaseFrontendProcess : public CBaseProcess, public CBaseQueueHandler {
public:
	CBaseFrontendProcess(CGame* p_game);
	virtual ~CBaseFrontendProcess();                                                                 // vtable+0x00
	virtual void Process();                                                                          // vtable+0x04
	virtual bool ReceiveCritical(unsigned long p_id, CReadPacket* p_packet, CConnect* p_connection); // vtable+0x08
	virtual void Processing();                                                                       // vtable+0x0c
	virtual bool ProcessMessages(Message* p_message);                                                // vtable+0x10
	int ProcessMsg(Message* p_message);
	void Action(eUserActions p_action, eUserActionStages p_stage);
	CBaseFrontendProcess();

	friend class CNetworkOptionsProc;

private:
	unsigned int m_networkWasActive; // 0x1c
	void* m_userActionMessage;       // 0x20
	CGame* m_game;                   // 0x24
};

extern int g_nTestAllLevels;
extern int g_nFrontendAutoFlowToggle;
extern int g_nAnimationsDisabled;
extern int g_nZoomEnabled;
extern int g_nMusicAvailable;
extern int g_nEffectsAvailable;
extern int g_nAnimationsAvailable;
extern int g_nZoomAvailable;
extern int g_nDisplayMode;

extern CBaseFrontendProcess* g_pCurrentFrontendProcess;

// SYNTHETIC: LEMBALL 0x004472b0
// CBaseFrontendProcess::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x004472e0
// CBaseFrontendProcess::`vector deleting destructor'

#endif
