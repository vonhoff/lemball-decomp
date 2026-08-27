#include "RequestActionMess.h"

#include "../Base/GlobalGameObject.h"
#include "RequestReplyMess.h"

// 68K 0x1060b54c __ct__18CRequestActionMessFv
// FUNCTION: LEMBALL 0x00416a10
RequestActionMess::RequestActionMess()
	: GameObjectMess(MESSAGE_REQUEST_ACTION)
{
	m_payloadCapacity += 4;
}

// 68K 0x1060b596 AddData__18CRequestActionMessFv
// FUNCTION: LEMBALL 0x00416a30
void RequestActionMess::AddData()
{
	GameObjectMess::AddData();
	Add((unsigned long) m_object->m_requestedAction);
}

// 68K 0x1060b5e6 GetData__18CRequestActionMessFv
// FUNCTION: LEMBALL 0x00416a50
void RequestActionMess::GetData()
{
	GetDword();
	int active;
	if (m_object->m_isRemoteObject == 0 && m_object->m_unk0x8c == 0 && m_object->IsUsable(m_object->m_action)) {
		active = 1;
	} else {
		active = 0;
	}
	m_object->m_requestActive = active;
	m_object->m_isRemoteObject = active;
	if (active != 0) {
		m_object->m_unk0x8c = 1;
	}
	g_pRequestReplyMessage->Send(m_object);
}

// 68K 0x1011a1f0 __dt__18CRequestActionMessFv
RequestActionMess::~RequestActionMess()
{
}

