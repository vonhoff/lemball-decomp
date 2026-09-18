#ifndef LEMBALL_AI_MESSAGES_REQUESTREPLYMESS_H
#define LEMBALL_AI_MESSAGES_REQUESTREPLYMESS_H

#include "GameObjectMess.h" // complete type

class RequestReplyMess : public GameObjectMess {
public:
	RequestReplyMess();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08
};

// SYNTHETIC: LEMBALL 0x00417be0
// RequestReplyMess::`scalar deleting destructor'

#endif
