#ifndef LEMBALL_AI_MESSAGES_OBJECTDIESMESS_H
#define LEMBALL_AI_MESSAGES_OBJECTDIESMESS_H

#include "../../Common.h"
#include "GameObjectMess.h" // complete type

class ObjectDiesMess : public GameObjectMess {
public:
	ObjectDiesMess();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08
	~ObjectDiesMess();
};

// SYNTHETIC: LEMBALL 0x00417cd0
// ObjectDiesMess::`scalar deleting destructor'


#endif

