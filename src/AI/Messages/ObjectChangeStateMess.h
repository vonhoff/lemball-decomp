#ifndef LEMBALL_AI_MESSAGES_OBJECTCHANGESTATEMESS_H
#define LEMBALL_AI_MESSAGES_OBJECTCHANGESTATEMESS_H

#include "../../Common.h"
#include "GameObjectMess.h" // complete type

class ObjectChangeStateMess : public GameObjectMess {
public:
	ObjectChangeStateMess();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08
	~ObjectChangeStateMess();
};

// SYNTHETIC: LEMBALL 0x00417b80
// ObjectChangeStateMess::`scalar deleting destructor'

#endif
