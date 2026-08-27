#ifndef LEMBALL_AI_MESSAGES_OBJECTHITMESS_H
#define LEMBALL_AI_MESSAGES_OBJECTHITMESS_H

#include "../../Common.h"
#include "GameObjectMess.h" // complete type

class ObjectHitMess : public GameObjectMess {
public:
	ObjectHitMess();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08
	~ObjectHitMess();
};

// SYNTHETIC: LEMBALL 0x00417ca0
// ObjectHitMess::`scalar deleting destructor'


#endif

