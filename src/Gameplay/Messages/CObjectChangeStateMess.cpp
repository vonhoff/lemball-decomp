#include "CObjectChangeStateMess.h"

#include "Gameplay/Simulation/GameTime.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "CGameObjectMess.h"
#include "GameMessageIds.h"

// FUNCTION: LEMBALL 0x004168d0
CObjectChangeStateMess::CObjectChangeStateMess() : CGameObjectMess(MESSAGE_OBJECT_CHANGE_STATE)
{
	m_payloadCapacity += 3 * sizeof(unsigned long) + sizeof(unsigned short);
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
	unsigned long time = GetDWORD();
	SetRemoteGameTimeReal(time);
	if (m_object->m_requestEnabled == 0) {
		m_object->m_requestEnabled = 1;
		CGlobalGameObject* obj = m_object;
		obj->Process();
		m_object = obj;
	}
	m_object->m_action = (eAction) GetDWORD();
	m_object->m_stateTimer = GetDWORD();
	m_object->m_actionArgument = (short) GetWORD();
	m_object->m_requestEnabled = 0;
	m_object->m_requestActive = 0;
	if (!m_object->IsUsable(m_object->m_action) && m_object->m_action != ACTION_LOCAL_CONTROL) {
		m_object->m_isRemoteObject = 1;
		return;
	}
	m_object->m_isRemoteObject = 0;
	m_object->m_pendingAction = ACTION_READY;
	m_object->m_activationReserved = 0;
}
