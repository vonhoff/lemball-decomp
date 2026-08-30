#ifndef LEMBALL_FRONTEND_BASE_BASEFRONTENDPROCESS_H
#define LEMBALL_FRONTEND_BASE_BASEFRONTENDPROCESS_H

#include "../../Common.h"
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/BaseProcess.h"          // complete type

// SIZE 0x28
// VTABLE: LEMBALL 0x00497938 BaseQueueHandler
// VTABLE: LEMBALL 0x00497948
class BaseFrontendProcess : public BaseProcess, public BaseQueueHandler {
public:
	BaseFrontendProcess(Game* p_arg0);
	virtual ~BaseFrontendProcess();                                                                // vtable+0x00
	virtual void Process();                                                                        // vtable+0x04
	virtual bool ReceiveCritical(unsigned long p_id, ReadPacket* p_packet, Connect* p_connection); // vtable+0x08
	virtual void Processing();                                                                     // vtable+0x0c
	virtual bool ProcessMessages(Message* p_message);                                              // vtable+0x10
	int ProcessMsg(Message* p_message);
	void Action(int p_action, int p_stage);
	BaseFrontendProcess();

	friend class NetworkOptionsProc;

private:
	unsigned int m_networkWasActive; // 0x1c
	void* m_userActionMessage;       // 0x20
	Game* m_game;                    // 0x24
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

extern BaseFrontendProcess* g_pCurrentFrontendProcess;

// SYNTHETIC: LEMBALL 0x004472b0
// BaseFrontendProcess::`scalar deleting destructor'

#endif

