#include "CGameFlaggedMessage.h"

#include "Engine/Network/Protocol/CNetworkMessage.h"

// FUNCTION: LEMBALL 0x004524b0
CGameFlaggedMessage::CGameFlaggedMessage(unsigned long p_messageId) : CNetworkMessage(p_messageId)
{
	m_payloadCapacity += sizeof(m_flag);
	m_headerEnabled = 1;
	m_flag = 0;
}

// FUNCTION: LEMBALL 0x004524f0
void CGameFlaggedMessage::AddData()
{
	Add((unsigned long) m_flag);
}

// FUNCTION: LEMBALL 0x00452500
void CGameFlaggedMessage::GetData()
{
	m_flag = GetDWORD();
}
