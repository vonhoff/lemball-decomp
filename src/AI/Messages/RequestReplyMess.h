#ifndef LEMBALL_AI_MESSAGES_REQUESTREPLYMESS_H
#define LEMBALL_AI_MESSAGES_REQUESTREPLYMESS_H

#include "../../Common.h"
#include "GameObjectMess.h" // complete type

class RequestReplyMess : public GameObjectMess {
public:
	RequestReplyMess();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08
	~RequestReplyMess();
};

#endif
