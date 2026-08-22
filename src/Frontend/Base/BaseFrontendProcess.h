#ifndef LEMBALL_SCAFFOLD_FRONTEND_BASE_BASEFRONTENDPROCESS_H
#define LEMBALL_SCAFFOLD_FRONTEND_BASE_BASEFRONTENDPROCESS_H

#include "../../Common.h"
#include "../../Visos/Foundation/BaseQueueHandler.h" // complete type
#include "../../Visos/Foundation/Process.h"          // complete type

// SIZE 0x28
// VTABLE: LEMBALL 0x00497948
class BaseFrontendProcess : public Process {
public:
	BaseFrontendProcess(Game* p_arg0);
	bool ProcessMsg(Message* p_message);
	virtual bool ProcessMessages(Message* p_message);                                              // vtable+0x10
	virtual bool ReceiveCritical(unsigned long p_id, ReadPacket* p_packet, Connect* p_connection); // vtable+0x08
	virtual void Process();                                                                        // vtable+0x04
	virtual void Processing();                                                                     // vtable+0x0c
	virtual ~BaseFrontendProcess();                                                                // vtable+0x00
	void Action(int p_action, int p_stage);
	BaseFrontendProcess();

private:
	BaseQueueHandler m_queueHandler; // 0x0c
	unsigned int m_networkWasActive; // 0x1c
	void* m_userActionMessage;       // 0x20
	Game* m_game;                    // 0x24
};

#endif
