#ifndef LEMBALL_AI_MESSAGES_REQUESTACTIONMESS_H
#define LEMBALL_AI_MESSAGES_REQUESTACTIONMESS_H

#include "../../Common.h"
#include "GameObjectMess.h" // complete type

class RequestActionMess : public GameObjectMess {
public:
	RequestActionMess();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08
	~RequestActionMess();
};

// SYNTHETIC: LEMBALL 0x00417bb0
// RequestActionMess::`scalar deleting destructor'


#endif

