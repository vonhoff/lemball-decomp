#ifndef LEMBALL_SCAFFOLD_AI_MESSAGES_GAMEOBJECTMESS_H
#define LEMBALL_SCAFFOLD_AI_MESSAGES_GAMEOBJECTMESS_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

// SIZE 0x30
// VTABLE: LEMBALL 0x00493d80
class GameObjectMess : public NetworkMessage {
public:
	GameObjectMess(unsigned long p_arg0);
	virtual void AddData();    // vtable+0x10
	virtual void GetData();    // vtable+0x08
	virtual ~GameObjectMess(); // vtable+0x14
	void Send(GlobalGameObject* p_arg0);
	GameObjectMess();

private:
	GlobalGameObject* m_object; // 0x2c
};

#endif
