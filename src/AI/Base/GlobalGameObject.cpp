#include "GlobalGameObject.h"

#include "../Messages/ObjectChangeStateMess.h"
#include "../Messages/ObjectDiesMess.h"
#include "../Messages/ObjectHitMess.h"
#include "../Messages/ObjectPosMess.h"
#include "../Messages/RemoveObjectMess.h"
#include "../Messages/RequestActionMess.h"
#include "../Messages/RequestCancelMess.h"
#include "../Messages/RequestReplyMess.h"
#include "../../Visos/Messaging/TransportObjectMess.h"

GlobalGameObject::GlobalGameObject()
{
}

// 68K 0x1011a15e DoActivate__17CGlobalGameObjectFv
// FUNCTION: LEMBALL 0x0040ce40
void GlobalGameObject::DoActivate()
{
}

// 68K 0x1060bb8e __ct__17CGlobalGameObjectF11eObjectTypeUsUs
// STUB: LEMBALL 0x00416d20
GlobalGameObject::GlobalGameObject(eObjectType p_arg0, unsigned short p_arg1, unsigned short p_arg2)
{
}

// 68K 0x1060bbf2 Restart__17CGlobalGameObjectFv
// STUB: LEMBALL 0x00416d50
void GlobalGameObject::Restart()
{
}

// 68K 0x1060bc46 UsableState__17CGlobalGameObjectFv
// FUNCTION: LEMBALL 0x00416d90
int GlobalGameObject::UsableState()
{
	int state = m_usableState;
	if (state != 0) {
		m_usableState = 0;
	}
	return state;
}

// 68K 0x1060bc8c Action__17CGlobalGameObjectF7eAction
// STUB: LEMBALL 0x00416db0
void GlobalGameObject::Action(eAction p_arg0)
{
}

// 68K 0x1060bce8 Action__17CGlobalGameObjectF7eActioni
// STUB: LEMBALL 0x00416de0
void GlobalGameObject::Action(eAction p_arg0, int p_arg1)
{
}

// 68K 0x1060bd38 RequestAction__17CGlobalGameObjectF7eAction
// STUB: LEMBALL 0x00416e20
void GlobalGameObject::RequestAction(eAction p_arg0)
{
}

// 68K 0x1060bdb8 CancelRequest__17CGlobalGameObjectFv
// STUB: LEMBALL 0x00416e70
void GlobalGameObject::CancelRequest()
{
}

// 68K 0x1060be00 Receive__17CGlobalGameObjectFUsP15CNetworkMessage
// STUB: LEMBALL 0x00416e90
bool GlobalGameObject::Receive(unsigned short p_arg0, NetworkMessage* p_arg1)
{
	return 0;
}

// 68K 0x1060bf4a SendRemove__17CGlobalGameObjectFv
// FUNCTION: LEMBALL 0x00416fc0
void GlobalGameObject::SendRemove()
{
	g_pRemoveObjectMessage->Send(this);
}

// 68K 0x1060bf8e SendCancel__17CGlobalGameObjectFv
// FUNCTION: LEMBALL 0x00416fd0
void GlobalGameObject::SendCancel()
{
	g_pRequestCancelMessage->Send(this);
}

// 68K 0x1060bfd2 SetMessages__17CGlobalGameObjectFv
// FUNCTION: LEMBALL 0x00416fe0
void GlobalGameObject::SetMessages()
{
	g_pTransportObjectMessage = new TransportObjectMess();
	g_pObjectChangeStateMessage = new ObjectChangeStateMess();
	g_pRemoveObjectMessage = new RemoveObjectMess();
	g_pRequestActionMessage = new RequestActionMess();
	g_pRequestReplyMessage = new RequestReplyMess();
	g_pRequestCancelMessage = new RequestCancelMess();
	g_pObjectPosMessage = new ObjectPosMess();
	g_pObjectHitMessage = new ObjectHitMess();
	g_pObjectDiesMessage = new ObjectDiesMess();
}

// 68K 0x1060c134 DeleteMessages__17CGlobalGameObjectFv
// FUNCTION: LEMBALL 0x00417150
void GlobalGameObject::DeleteMessages()
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

// 68K 0x101180c6 __dt__17CGlobalGameObjectFv
GlobalGameObject::~GlobalGameObject()
{
}

// GLOBAL: LEMBALL 0x0049d110
TransportObjectMess* g_pTransportObjectMessage;

// GLOBAL: LEMBALL 0x0049d114
ObjectChangeStateMess* g_pObjectChangeStateMessage;

// GLOBAL: LEMBALL 0x0049d118
RemoveObjectMess* g_pRemoveObjectMessage;

// GLOBAL: LEMBALL 0x0049d11c
RequestActionMess* g_pRequestActionMessage;

// GLOBAL: LEMBALL 0x0049d120
RequestReplyMess* g_pRequestReplyMessage;

// GLOBAL: LEMBALL 0x0049d124
RequestCancelMess* g_pRequestCancelMessage;

// GLOBAL: LEMBALL 0x0049d128
ObjectPosMess* g_pObjectPosMessage;

// GLOBAL: LEMBALL 0x0049d12c
ObjectHitMess* g_pObjectHitMessage;

// GLOBAL: LEMBALL 0x0049d130
ObjectDiesMess* g_pObjectDiesMessage;

