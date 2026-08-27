#ifndef LEMBALL_AI_MESSAGES_OBJECTPOSMESS_H
#define LEMBALL_AI_MESSAGES_OBJECTPOSMESS_H

#include "../../Common.h"
#include "GameObjectMess.h" // complete type

class ObjectPosMess : public GameObjectMess {
public:
	ObjectPosMess();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08
	~ObjectPosMess();
};

// SYNTHETIC: LEMBALL 0x00417c70
// ObjectPosMess::`scalar deleting destructor'


#endif

