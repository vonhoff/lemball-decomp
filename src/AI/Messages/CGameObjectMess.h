#ifndef LEMBALL_AI_MESSAGES_CGAMEOBJECTMESS_H
#define LEMBALL_AI_MESSAGES_CGAMEOBJECTMESS_H

#include "../../Visos/Messaging/CNetworkMessage.h" // complete type

class CGlobalGameObject;
// SIZE 0x30
// VTABLE: LEMBALL 0x00493d80
class CGameObjectMess : public CNetworkMessage {
public:
	CGameObjectMess(unsigned long p_messageId);
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08
	void Send(CGlobalGameObject* p_object);
	CGameObjectMess();

	friend class CObjectChangeStateMess;
	friend class CObjectDiesMess;
	friend class CObjectHitMess;
	friend class CObjectPosMess;
	friend class CRemoveObjectMess;
	friend class CRequestActionMess;
	friend class CRequestCancelMess;
	friend class CRequestReplyMess;
	friend class CGlobalGameObject;

protected:
	CGlobalGameObject* m_object; // 0x2c
};

// SYNTHETIC: LEMBALL 0x00417b50
// CGameObjectMess::`scalar deleting destructor'

#endif
