#include "ObjectChangeStateMess.h"

#include "../../Control/Game/GameTime.h"
#include "../Base/GlobalGameObject.h"

// 68K 0x1060b366 __ct__22CObjectChangeStateMessFv
// FUNCTION: LEMBALL 0x004168d0
ObjectChangeStateMess::ObjectChangeStateMess()
	: GameObjectMess(MESSAGE_OBJECT_CHANGE_STATE)
{
	m_payloadCapacity += 14;
}

// 68K 0x1060b3b6 AddData__22CObjectChangeStateMessFv
// FUNCTION: LEMBALL 0x004168f0
void ObjectChangeStateMess::AddData()
{
	GameObjectMess::AddData();
	Add(g_dwSimulationTimestamp);
	Add((unsigned long) m_object->m_action);
	Add((unsigned long) m_object->m_stateTimer);
	Add((unsigned short) m_object->m_actionArgument);
}

// 68K 0x1060b43a GetData__22CObjectChangeStateMessFv
// FUNCTION: LEMBALL 0x00416940
void ObjectChangeStateMess::GetData()
{
	unsigned long time = GetDword();
	SetRemoteGameTimeReal(time);
	if (m_object->m_requestEnabled == 0) {
		m_object->m_requestEnabled = 1;
		GlobalGameObject* obj = m_object;
		obj->Process();
		m_object = obj;
	}
	m_object->m_action = (eAction) GetDword();
	m_object->m_stateTimer = GetDword();
	m_object->m_actionArgument = (short) GetWord();
	m_object->m_requestEnabled = 0;
	m_object->m_requestActive = 0;
	if (!m_object->IsUsable(m_object->m_action) && m_object->m_action != 0x27) {
		m_object->m_isRemoteObject = 1;
		return;
	}
	m_object->m_isRemoteObject = 0;
	m_object->m_pendingAction = 0x18;
	m_object->m_unk0x8c = 0;
}

// 68K 0x1011a256 __dt__22CObjectChangeStateMessFv
// SYNTHETIC: LEMBALL 0x00417b80
// ObjectChangeStateMess::`scalar deleting destructor'
ObjectChangeStateMess::~ObjectChangeStateMess()
{
}
