#ifndef LEMBALL_FRONTEND_SUPPORT_USERACTIONMESSAGE_H
#define LEMBALL_FRONTEND_SUPPORT_USERACTIONMESSAGE_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

// SIZE 0x34
// VTABLE: LEMBALL 0x00497878
class UserActionMessage : public NetworkMessage {
public:
	UserActionMessage();
	virtual void AddData();       // vtable+0x10
	virtual void GetData();       // vtable+0x08
	virtual ~UserActionMessage(); // vtable+0x14

	friend class BaseFrontendProcess;

private:
	eUserActions m_action;     // 0x2c
	eUserActionStages m_stage; // 0x30
};

// SYNTHETIC: LEMBALL 0x00446f20
// UserActionMessage::`scalar deleting destructor'

#endif
