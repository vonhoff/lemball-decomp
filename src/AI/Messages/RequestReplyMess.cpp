#include "RequestReplyMess.h"

#include "../Base/GlobalGameObject.h"

// 68K 0x1060b69a __ct__17CRequestReplyMessFv
// FUNCTION: LEMBALL 0x00416ac0
RequestReplyMess::RequestReplyMess()
	: GameObjectMess(MESSAGE_REQUEST_REPLY)
{
	m_payloadCapacity += 4;
}

// 68K 0x1060b6e2 AddData__17CRequestReplyMessFv
// FUNCTION: LEMBALL 0x00416ae0
void RequestReplyMess::AddData()
{
	GameObjectMess::AddData();
	Add((unsigned long) m_object->m_requestActive);
}

// 68K 0x1060b732 GetData__17CRequestReplyMessFv
// FUNCTION: LEMBALL 0x00416b00
void RequestReplyMess::GetData()
{
	m_object->m_requestActive = GetDword();
	if (m_object->m_requestActive != 0) {
		m_object->m_action = m_object->m_requestedAction;
		m_object->DoActivate();
		m_object->Action(m_object->m_requestedAction);
		m_object->m_usableState = 2;
	}
	else {
		m_object->m_usableState = 1;
	}
	m_object->m_requestedAction = (eAction) 0x18;
}

// 68K 0x1011a18c __dt__17CRequestReplyMessFv
RequestReplyMess::~RequestReplyMess()
{
}

