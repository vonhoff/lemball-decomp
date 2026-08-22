#ifndef LEMBALL_SCAFFOLD_AI_MESSAGES_OBJECTCHANGESTATEMESS_H
#define LEMBALL_SCAFFOLD_AI_MESSAGES_OBJECTCHANGESTATEMESS_H

#include "../../Common.h"
#include "GameObjectMess.h" // complete type

class ObjectChangeStateMess : public GameObjectMess {
public:
	ObjectChangeStateMess();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08
	~ObjectChangeStateMess();
};

#endif
