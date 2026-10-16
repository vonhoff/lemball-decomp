#ifndef LEMBALL_FRONTEND_SUPPORT_CUSERACTIONMESSAGE_H
#define LEMBALL_FRONTEND_SUPPORT_CUSERACTIONMESSAGE_H

#include "Multiplayer/Transport/Protocol/CNetworkMessage.h"
#include "UserActions.h"

// SIZE 0x34
// VTABLE: LEMBALL 0x00497878
class CUserActionMessage : public CNetworkMessage {
public:
	CUserActionMessage();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08

	friend class CBaseFrontendProcess;

private:
	eUserActions m_action;     // 0x2c
	eUserActionStages m_stage; // 0x30
};

// SYNTHETIC: LEMBALL 0x00446f20
// CUserActionMessage::`scalar deleting destructor'

#endif
