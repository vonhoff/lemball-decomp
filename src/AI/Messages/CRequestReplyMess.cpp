#include "CRequestReplyMess.h"

#include "../Base/CGlobalGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Messages/CGameObjectMess.h"
#include "AI/Messages/GameMessageIds.h"

// FUNCTION: LEMBALL 0x00416ac0
CRequestReplyMess::CRequestReplyMess() : CGameObjectMess(MESSAGE_REQUEST_REPLY)
{
	m_payloadCapacity += 4;
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
		m_object->m_usableState = 2;
	}
	else {
		m_object->m_usableState = 1;
	}
	m_object->m_requestedAction = ACTION_0x18;
}
