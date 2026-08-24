#include "NetworkGameStage.h"

// 68K 0x10a0000c __ct__17CNetworkGameStageFv
// FUNCTION: LEMBALL 0x004523b0
NetworkGameStage::NetworkGameStage()
	: NetworkMessage(4)
{
	m_payloadCapacity += 4;
}

// 68K 0x10119150 AddData__17CNetworkGameStageFv
// FUNCTION: LEMBALL 0x00452e40
void NetworkGameStage::AddData()
{
	Add((unsigned long) m_stage);
}

// 68K 0x10119190 GetData__17CNetworkGameStageFv
// FUNCTION: LEMBALL 0x00452e50
void NetworkGameStage::GetData()
{
	m_stage = (unsigned int) GetDword();
}

// 68K 0x101191cc __dt__17CNetworkGameStageFv
// SYNTHETIC: LEMBALL 0x00452e60
// NetworkGameStage::`scalar deleting destructor'
NetworkGameStage::~NetworkGameStage()
{
}
