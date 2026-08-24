#ifndef LEMBALL_AI_BASE_GLOBALGAMEOBJECT_H
#define LEMBALL_AI_BASE_GLOBALGAMEOBJECT_H

#include "../../Common.h"
#include "GameObject.h" // complete type

// SIZE 0x138
// VTABLE: LEMBALL 0x00493ea0
class GlobalGameObject : public GameObject {
public:
	GlobalGameObject(eObjectType p_arg0, unsigned short p_arg1, unsigned short p_arg2);
	virtual void Action(eAction p_arg0, int p_arg1);                     // vtable+0x04
	virtual void Action(eAction p_arg0);                                 // vtable+0x08
	virtual void SendRemove();                                           // vtable+0xf4
	virtual void SendCancel();                                           // vtable+0xf8
	virtual int UsableState();                                           // vtable+0xfc
	virtual void Restart();                                              // vtable+0x104
	virtual bool Receive(unsigned short p_arg0, NetworkMessage* p_arg1); // vtable+0x108
	virtual void DoActivate();                                           // vtable+0x10c
	void CancelRequest();
	void DeleteMessages();
	void RequestAction(eAction p_arg0);
	void SetMessages();
	~GlobalGameObject();
	GlobalGameObject();

	friend class GameObjectMess;
	friend class ObjectChangeStateMess;
	friend class ObjectDiesMess;
	friend class ObjectHitMess;
	friend class ObjectPosMess;
	friend class RemoveObjectMess;
	friend class RequestActionMess;
	friend class RequestCancelMess;
	friend class RequestReplyMess;

private:
	unsigned int m_requestEnabled; // 0x124
	eAction m_pendingAction;       // 0x128
	eAction m_requestedAction;     // 0x12c
	unsigned int m_requestActive;  // 0x130
	unsigned int m_usableState;    // 0x134
};

extern TransportObjectMess* g_pTransportObjectMessage;
extern ObjectChangeStateMess* g_pObjectChangeStateMessage;
extern RemoveObjectMess* g_pRemoveObjectMessage;
extern RequestActionMess* g_pRequestActionMessage;
extern RequestReplyMess* g_pRequestReplyMessage;
extern RequestCancelMess* g_pRequestCancelMessage;
extern ObjectPosMess* g_pObjectPosMessage;
extern ObjectHitMess* g_pObjectHitMessage;
extern ObjectDiesMess* g_pObjectDiesMessage;

#endif
