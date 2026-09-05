#ifndef LEMBALL_AI_MESSAGES_GAMEOBJECTMESS_H
#define LEMBALL_AI_MESSAGES_GAMEOBJECTMESS_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

#include "GameMessageIds.h"

// SIZE 0x30
// VTABLE: LEMBALL 0x00493d80
class GameObjectMess : public NetworkMessage {
public:
	GameObjectMess(unsigned long p_messageId);
	virtual void AddData();    // vtable+0x10
	virtual void GetData();    // vtable+0x08
	virtual ~GameObjectMess(); // vtable+0x14
	void Send(GlobalGameObject* p_object);
	GameObjectMess();

	friend class ObjectChangeStateMess;
	friend class ObjectDiesMess;
	friend class ObjectHitMess;
	friend class ObjectPosMess;
	friend class RemoveObjectMess;
	friend class RequestActionMess;
	friend class RequestCancelMess;
	friend class RequestReplyMess;

protected:
	GlobalGameObject* m_object; // 0x2c
};

// SYNTHETIC: LEMBALL 0x00417b50
// GameObjectMess::`scalar deleting destructor'

#endif
