#include "CRequestActionMess.h"

#include "../Base/CGlobalGameObject.h"
#include "AI/Messages/CGameObjectMess.h"
#include "AI/Messages/GameMessageIds.h"
#include "CRequestReplyMess.h"

// FUNCTION: LEMBALL 0x00416a10
CRequestActionMess::CRequestActionMess() : CGameObjectMess(MESSAGE_REQUEST_ACTION)
{
	m_payloadCapacity += 4;
}

// FUNCTION: LEMBALL 0x00416a30
void CRequestActionMess::AddData()
{
	CGameObjectMess::AddData();
	Add((unsigned long) m_object->m_requestedAction);
}

// FUNCTION: LEMBALL 0x00416a50
void CRequestActionMess::GetData()
{
	GetDword();
	int active;
	if (m_object->m_isRemoteObject == 0 && m_object->m_unk0x8c == 0 && m_object->IsUsable(m_object->m_action)) {
		active = 1;
	}
	else {
		active = 0;
	}
	m_object->m_requestActive = active;
	m_object->m_isRemoteObject = active;
	if (active != 0) {
		m_object->m_unk0x8c = 1;
	}
	g_pRequestReplyMessage->Send(m_object);
}
