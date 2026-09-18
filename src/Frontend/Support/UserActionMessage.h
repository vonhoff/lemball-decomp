#ifndef LEMBALL_FRONTEND_SUPPORT_USERACTIONMESSAGE_H
#define LEMBALL_FRONTEND_SUPPORT_USERACTIONMESSAGE_H

#include "../../Visos/Messaging/NetworkMessage.h" // complete type

enum eUserActions {
	USER_ACTION_0 = 0,
	USER_ACTION_1 = 1,
	USER_ACTION_2 = 2,
	USER_ACTION_3 = 3
};

enum eUserActionStages {
	USER_ACTION_STAGE_REQUEST = 0,
	USER_ACTION_STAGE_CONFIRM = 1,
	USER_ACTION_STAGE_REJECT = 2
};

// SIZE 0x34
// VTABLE: LEMBALL 0x00497878
class UserActionMessage : public NetworkMessage {
public:
	UserActionMessage();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08

	friend class BaseFrontendProcess;

private:
	eUserActions m_action;     // 0x2c
	eUserActionStages m_stage; // 0x30
};

// SYNTHETIC: LEMBALL 0x00446f20
// UserActionMessage::`scalar deleting destructor'

#endif
