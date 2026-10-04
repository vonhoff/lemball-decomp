#ifndef LEMBALL_FRONTEND_SUPPORT_CUSERACTIONMESSAGE_H
#define LEMBALL_FRONTEND_SUPPORT_CUSERACTIONMESSAGE_H

#include "Engine/Network/Protocol/CNetworkMessage.h"

enum eUserActions {
	USER_ACTION_NEXT_LEVEL = 0,
	USER_ACTION_PREVIOUS_LEVEL = 1,
	USER_ACTION_PREVIEW_GO_REQUEST = 2,
	USER_ACTION_PREVIEW_RETURN_CONFIRM = 2,
	USER_ACTION_SUCC_FAIL_RETURN_REQUEST = 2,
	USER_ACTION_SUCC_FAIL_GO_CONFIRM = 2,
	USER_ACTION_PREVIEW_RETURN_REQUEST = 3,
	USER_ACTION_PREVIEW_GO_CONFIRM = 3,
	USER_ACTION_SUCC_FAIL_GO_REQUEST = 3,
	USER_ACTION_SUCC_FAIL_RETURN_CONFIRM = 3
};

enum eUserActionStages {
	USER_ACTION_STAGE_REQUEST = 0,
	USER_ACTION_STAGE_CONFIRM = 1,
	USER_ACTION_STAGE_REJECT = 2
};

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
