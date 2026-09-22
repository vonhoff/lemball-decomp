#include "CGameObjectMess.h"

#include "../Base/CGlobalGameObject.h"
#include "../Managers/CBaseObjectManager.h"
#include "Visos/Messaging/CNetworkMessage.h"

// FUNCTION: LEMBALL 0x00416850
CGameObjectMess::CGameObjectMess(unsigned long p_messageId) : CNetworkMessage(p_messageId)
{
	m_headerEnabled = 1;
	m_payloadCapacity += 4;
}

// FUNCTION: LEMBALL 0x00416880
void CGameObjectMess::Send(CGlobalGameObject* p_object)
{
	m_object = p_object;
	p_object->m_manager->Add(this);
}

// FUNCTION: LEMBALL 0x004168a0
void CGameObjectMess::AddData()
{
	Add((unsigned short) m_messageId);
	Add((unsigned short) m_object->GetId());
}

// FUNCTION: LEMBALL 0x004168c0
void CGameObjectMess::GetData()
{
}
