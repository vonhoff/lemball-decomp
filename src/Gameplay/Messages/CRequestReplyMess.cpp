#include "CRequestReplyMess.h"

#include "CGameObjectMess.h"
#include "GameMessageIds.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectInteractionStates.h"

// FUNCTION: LEMBALL 0x00416ac0
CRequestReplyMess::CRequestReplyMess() : CGameObjectMess(MESSAGE_REQUEST_REPLY)
{
	m_payloadCapacity += sizeof(unsigned long);
}

// FUNCTION: LEMBALL 0x00416ae0
void CRequestReplyMess::AddData()
{
	CGameObjectMess::AddData();
	Add((unsigned long) m_object->m_requestActive);
}

// FUNCTION: LEMBALL 0x00416b00
void CRequestReplyMess::GetData()
{
	m_object->m_requestActive = GetDWORD();
	if (m_object->m_requestActive != 0) {
		m_object->m_action = m_object->m_requestedAction;
		m_object->DoActivate();
		m_object->Action(m_object->m_requestedAction);
		m_object->m_usableState = GROUP_OBJECT_REQUEST_ACCEPTED;
	}
	else {
		m_object->m_usableState = GROUP_OBJECT_REQUEST_REJECTED;
	}
	m_object->m_requestedAction = ACTION_READY;
}
