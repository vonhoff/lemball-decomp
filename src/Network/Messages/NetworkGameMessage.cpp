#include "NetworkGameMessage.h"

// 68K 0x10a0005e __ct__19CNetworkGameMessageFv
// FUNCTION: LEMBALL 0x004523e0
NetworkGameMessage::NetworkGameMessage()
	: NetworkMessage(5)
{
	m_gameName[0] = '\0';
	m_valid = 0;
	m_payloadCapacity += 0x1e;
	m_headerEnabled = 1;
}

// 68K 0x10a000c2 AddData__19CNetworkGameMessageFv
// STUB: LEMBALL 0x00452420
void NetworkGameMessage::AddData()
{
	Add(m_gameName);
	Add(m_peerName);
}

// 68K 0x10a00184 GetData__19CNetworkGameMessageFv
// FUNCTION: LEMBALL 0x00452490
void NetworkGameMessage::GetData()
{
	GetCopy(m_gameName);
	GetCopy(m_peerName);
	m_valid = 1;
}

// 68K 0x1011c6f0 __dt__19CNetworkGameMessageFv
NetworkGameMessage::~NetworkGameMessage()
{
}

