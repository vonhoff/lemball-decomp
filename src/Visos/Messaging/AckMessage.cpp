#include "AckMessage.h"

// 68K 0x10107400 AddData__11CAckMessageFv
// FUNCTION: LEMBALL 0x00462b40
void AckMessage::AddData()
{
	Add(m_packetSequence);
	Add(m_subpacketSequence);
}

// 68K 0x10107448 GetData__11CAckMessageFv
// FUNCTION: LEMBALL 0x00462b60
void AckMessage::GetData()
{
	Get(m_packetSequence);
	Get(m_subpacketSequence);
}

// 68K 0x10107490 __dt__11CAckMessageFv
AckMessage::~AckMessage()
{
}

// GLOBAL: LEMBALL 0x004a1e14
AckMessage* g_pAckMessage = 0;

