#include "CNetworkGameStage.h"

#include "Visos/Messaging/CNetworkMessage.h"

// FUNCTION: LEMBALL 0x004523b0
CNetworkGameStage::CNetworkGameStage() : CNetworkMessage(4)
{
	m_payloadCapacity += 4;
}

// FUNCTION: LEMBALL 0x00452e40
void CNetworkGameStage::AddData()
{
	Add((unsigned long) m_stage);
}

// FUNCTION: LEMBALL 0x00452e50
void CNetworkGameStage::GetData()
{
	m_stage = (unsigned int) GetDWORD();
}
