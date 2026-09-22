#include "CObjectChangeStateMess.h"

#include "../../Control/Game/GameTime.h"
#include "../Base/CGlobalGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Messages/CGameObjectMess.h"
#include "AI/Messages/GameMessageIds.h"

// FUNCTION: LEMBALL 0x004168d0
CObjectChangeStateMess::CObjectChangeStateMess() : CGameObjectMess(MESSAGE_OBJECT_CHANGE_STATE)
{
	m_payloadCapacity += 14;
}

// FUNCTION: LEMBALL 0x004168f0
void CObjectChangeStateMess::AddData()
{
	CGameObjectMess::AddData();
	Add(g_dwSimulationTimestamp);
	Add((unsigned long) m_object->m_action);
	Add((unsigned long) m_object->m_stateTimer);
	Add((unsigned short) m_object->m_actionArgument);
}

// FUNCTION: LEMBALL 0x00416940
void CObjectChangeStateMess::GetData()
{
	unsigned long time = GetDword();
	SetRemoteGameTimeReal(time);
	if (m_object->m_requestEnabled == 0) {
		m_object->m_requestEnabled = 1;
		CGlobalGameObject* obj = m_object;
		obj->Process();
		m_object = obj;
	}
	m_object->m_action = (eAction) GetDword();
	m_object->m_stateTimer = GetDword();
	m_object->m_actionArgument = (short) GetWord();
	m_object->m_requestEnabled = 0;
	m_object->m_requestActive = 0;
	if (!m_object->IsUsable(m_object->m_action) && m_object->m_action != ACTION_0x27) {
		m_object->m_isRemoteObject = 1;
		return;
	}
	m_object->m_isRemoteObject = 0;
	m_object->m_pendingAction = ACTION_0x18;
	m_object->m_unk0x8c = 0;
}
