#ifndef LEMBALL_AI_MESSAGES_GAMEOBJECTMESS_H
#define LEMBALL_AI_MESSAGES_GAMEOBJECTMESS_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

#define MESSAGE_OBJECT_CHANGE_STATE 0x23
#define MESSAGE_OBJECT_POS          0x24
#define MESSAGE_OBJECT_HIT          0x25
#define MESSAGE_OBJECT_DIES         0x26
#define MESSAGE_REQUEST_ACTION      0x27
#define MESSAGE_REQUEST_REPLY       0x28
#define MESSAGE_REQUEST_CANCEL      0x29
#define MESSAGE_REMOVE_OBJECT       0x2a

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

