#include "CUserActionMessage.h"

#include "Visos/Messaging/CNetworkMessage.h"

// FUNCTION: LEMBALL 0x004453b0
CUserActionMessage::CUserActionMessage() : CNetworkMessage(8)
{
	m_payloadCapacity += 8;
	m_headerEnabled = 1;
}

// FUNCTION: LEMBALL 0x004453e0
void CUserActionMessage::AddData()
{
	Add((unsigned long) m_action);
	Add((unsigned long) m_stage);
}

// FUNCTION: LEMBALL 0x00445400
void CUserActionMessage::GetData()
{
	m_action = (eUserActions) GetDWORD();
	m_stage = (eUserActionStages) GetDWORD();
}
