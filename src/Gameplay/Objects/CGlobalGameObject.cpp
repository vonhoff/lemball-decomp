#include "CGlobalGameObject.h"

#include "Gameplay/Messages/CTransportObjectMess.h"
#include "Visos/Network/CConnect.h"
#include "Gameplay/Messages/CObjectChangeStateMess.h"
#include "Gameplay/Messages/CObjectDiesMess.h"
#include "Gameplay/Messages/CObjectHitMess.h"
#include "Gameplay/Messages/CObjectPosMess.h"
#include "Gameplay/Messages/CRemoveObjectMess.h"
#include "Gameplay/Messages/CRequestActionMess.h"
#include "Gameplay/Messages/CRequestCancelMess.h"
#include "Gameplay/Messages/CRequestReplyMess.h"
#include "CGameObject.h"
#include "ObjectActions.h"
#include "Gameplay/Messages/CGameObjectMess.h"
#include "Gameplay/Messages/GameMessageIds.h"
#include "ObjectInteractionStates.h"
#include "Visos/Network/Protocol/CNetworkMessage.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0040ce40
void CGlobalGameObject::DoActivate()
{
}

// FUNCTION: LEMBALL 0x00416d20
CGlobalGameObject::CGlobalGameObject(eObjectType p_objectType,
									 unsigned short p_collisionFlags,
									 unsigned short p_destinationCapacity)
	: CGameObject(p_objectType, p_collisionFlags, p_destinationCapacity)
{
	m_manager = NULL;
}

// FUNCTION: LEMBALL 0x00416d50
void CGlobalGameObject::Restart()
{
	CGameObject::Restart();
	m_requestActive = 0;
	m_requestedAction = ACTION_READY;
	m_pendingAction = ACTION_READY;
	m_usableState = GROUP_OBJECT_REQUEST_NONE;
	m_requestEnabled = 1;
}

// FUNCTION: LEMBALL 0x00416d90
int CGlobalGameObject::UsableState()
{
	int state = m_usableState;
	if (state != 0) {
		m_usableState = GROUP_OBJECT_REQUEST_NONE;
	}
	return state;
}

// FUNCTION: LEMBALL 0x00416db0
void CGlobalGameObject::Action(eAction p_action)
{
	m_action = p_action;
	if (g_pActiveConnection != NULL) {
		g_pObjectChangeStateMessage->Send(this);
	}
}

// FUNCTION: LEMBALL 0x00416de0
void CGlobalGameObject::Action(eAction p_action, int p_argument)
{
	m_actionArgument = p_argument;
	Action(p_action);
}

// FUNCTION: LEMBALL 0x00416e00
void CGlobalGameObject::SetActionAndRequest(eAction p_action, int p_argument)
{
	m_actionArgument = p_argument;
	RequestAction(p_action);
}

// FUNCTION: LEMBALL 0x00416e20
void CGlobalGameObject::RequestAction(eAction p_action)
{
	if (g_pActiveConnection != NULL) {
		m_requestedAction = p_action;
		g_pRequestActionMessage->Send(this);
		return;
	}
	m_action = p_action;
	DoActivate();
	m_usableState = GROUP_OBJECT_REQUEST_ACCEPTED;
}

// FUNCTION: LEMBALL 0x00416e70
void CGlobalGameObject::CancelRequest()
{
	if (m_requestActive != 0) {
		m_isRemoteObject = 0;
		m_requestActive = 0;
		m_activationReserved = 0;
	}
}

// FUNCTION: LEMBALL 0x00416e90
bool CGlobalGameObject::Receive(unsigned short p_messageId, CNetworkMessage* p_message)
{
	CGameObjectMess* msg;

	switch (p_messageId) {
	case MESSAGE_OBJECT_CHANGE_STATE:
		msg = g_pObjectChangeStateMessage;
		msg->m_object = this;
		if (msg->Set(p_message->m_readCursor)) {
			p_message->m_readCursor = msg->m_readCursor;
		}
		return true;
	case MESSAGE_OBJECT_POS:
		msg = g_pObjectPosMessage;
		msg->m_object = this;
		if (msg->Set(p_message->m_readCursor)) {
			p_message->m_readCursor = msg->m_readCursor;
		}
		return true;
	case MESSAGE_OBJECT_HIT:
		msg = g_pObjectHitMessage;
		msg->m_object = this;
		if (msg->Set(p_message->m_readCursor)) {
			p_message->m_readCursor = msg->m_readCursor;
		}
		return true;
	case MESSAGE_REQUEST_ACTION:
		msg = g_pRequestActionMessage;
		msg->m_object = this;
		if (msg->Set(p_message->m_readCursor)) {
			p_message->m_readCursor = msg->m_readCursor;
		}
		return true;
	case MESSAGE_REQUEST_REPLY:
		msg = g_pRequestReplyMessage;
		msg->m_object = this;
		if (msg->Set(p_message->m_readCursor)) {
			p_message->m_readCursor = msg->m_readCursor;
		}
		return true;
	case MESSAGE_REQUEST_CANCEL:
		CancelRequest();
		return true;
	default:
		return false;
	}
}

// FUNCTION: LEMBALL 0x00416fc0
void CGlobalGameObject::SendRemove()
{
	g_pRemoveObjectMessage->Send(this);
}

// FUNCTION: LEMBALL 0x00416fd0
void CGlobalGameObject::SendCancel()
{
	g_pRequestCancelMessage->Send(this);
}

// FUNCTION: LEMBALL 0x00416fe0
void CGlobalGameObject::SetMessages()
{
	g_pTransportObjectMessage = new CTransportObjectMess();
	g_pObjectChangeStateMessage = new CObjectChangeStateMess();
	g_pRemoveObjectMessage = new CRemoveObjectMess();
	g_pRequestActionMessage = new CRequestActionMess();
	g_pRequestReplyMessage = new CRequestReplyMess();
	g_pRequestCancelMessage = new CRequestCancelMess();
	g_pObjectPosMessage = new CObjectPosMess();
	g_pObjectHitMessage = new CObjectHitMess();
	g_pObjectDiesMessage = new CObjectDiesMess();
}

// FUNCTION: LEMBALL 0x00417150
void CGlobalGameObject::DeleteMessages()
{
	delete g_pObjectDiesMessage;
	delete g_pObjectHitMessage;
	delete g_pObjectPosMessage;
	delete g_pTransportObjectMessage;
	delete g_pObjectChangeStateMessage;
	delete g_pRemoveObjectMessage;
	delete g_pRequestActionMessage;
	delete g_pRequestReplyMessage;
	delete g_pRequestCancelMessage;
}

// GLOBAL: LEMBALL 0x0049d110
CTransportObjectMess* g_pTransportObjectMessage;

// GLOBAL: LEMBALL 0x0049d114
CObjectChangeStateMess* g_pObjectChangeStateMessage;

// GLOBAL: LEMBALL 0x0049d118
CRemoveObjectMess* g_pRemoveObjectMessage;

// GLOBAL: LEMBALL 0x0049d11c
CRequestActionMess* g_pRequestActionMessage;

// GLOBAL: LEMBALL 0x0049d120
CRequestReplyMess* g_pRequestReplyMessage;

// GLOBAL: LEMBALL 0x0049d124
CRequestCancelMess* g_pRequestCancelMessage;

// GLOBAL: LEMBALL 0x0049d128
CObjectPosMess* g_pObjectPosMessage;

// GLOBAL: LEMBALL 0x0049d12c
CObjectHitMess* g_pObjectHitMessage;

// GLOBAL: LEMBALL 0x0049d130
CObjectDiesMess* g_pObjectDiesMessage;

// GLOBAL: LEMBALL 0x0049d134
int g_dwSommersaultDirection = SOMMERSAULT_DIRECTION_NORMAL;
