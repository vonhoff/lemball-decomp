#ifndef LEMBALL_AI_BASE_CGLOBALGAMEOBJECT_H
#define LEMBALL_AI_BASE_CGLOBALGAMEOBJECT_H

#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "CGameObject.h" // complete type

class CNetworkMessage;
class CObjectChangeStateMess;
class CObjectDiesMess;
class CObjectHitMess;
class CObjectPosMess;
class CRemoveObjectMess;
class CRequestActionMess;
class CRequestCancelMess;
class CRequestReplyMess;
class CTransportObjectMess;
// SIZE 0x138
// VTABLE: LEMBALL 0x00493ea0
class CGlobalGameObject : public CGameObject {
public:
	CGlobalGameObject(eObjectType p_objectType, unsigned short p_collisionFlags, unsigned short p_destinationCapacity);
	virtual void Action(eAction p_arg0);                                  // vtable+0x08
	virtual void Action(eAction p_arg0, int p_arg1);                      // vtable+0x04
	virtual void SendRemove();                                            // vtable+0xf4
	virtual void SendCancel();                                            // vtable+0xf8
	virtual int UsableState();                                            // vtable+0xfc
	virtual void Restart();                                               // vtable+0x104
	virtual bool Process() = 0;                                           // vtable+0x14
	virtual bool Receive(unsigned short p_arg0, CNetworkMessage* p_arg1); // vtable+0x108
	virtual void DoActivate();                                            // vtable+0x10c
	void CancelRequest();
	static void DeleteMessages();
	void RequestAction(eAction p_arg0);
	static void SetMessages();
	CGlobalGameObject();
	void SetActionAndRequest(eAction p_action, int p_argument);

	friend class CGameObjectMess;
	friend class CObjectChangeStateMess;
	friend class CObjectDiesMess;
	friend class CObjectHitMess;
	friend class CObjectManager;
	friend class CObjectPosMess;
	friend class CRemoveObjectMess;
	friend class CRequestActionMess;
	friend class CRequestCancelMess;
	friend class CRequestReplyMess;
	friend class CTrampolineManager;
	friend class CCollectableManager;
	friend class CRocketManager;
	friend class CDoorManager;
	friend class CLaserManager;
	friend class CLiftManager;
	friend class CMineManager;
	friend class CRocketManager;
	friend class CTrapDoorManager;
	friend class CHandManager;

protected:
	unsigned int m_requestEnabled; // 0x124
	eAction m_pendingAction;       // 0x128
	eAction m_requestedAction;     // 0x12c
	unsigned int m_requestActive;  // 0x130
	unsigned int m_usableState;    // 0x134
};

extern CTransportObjectMess* g_pTransportObjectMessage;
extern CObjectChangeStateMess* g_pObjectChangeStateMessage;
extern CRemoveObjectMess* g_pRemoveObjectMessage;
extern CRequestActionMess* g_pRequestActionMessage;
extern CRequestReplyMess* g_pRequestReplyMessage;
extern CRequestCancelMess* g_pRequestCancelMessage;
extern CObjectPosMess* g_pObjectPosMessage;
extern CObjectHitMess* g_pObjectHitMessage;
extern CObjectDiesMess* g_pObjectDiesMessage;

// SYNTHETIC: LEMBALL 0x00417d00
// CGlobalGameObject::`scalar deleting destructor'

#endif
