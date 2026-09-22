#include "CAckMessage.h"

// FUNCTION: LEMBALL 0x00462b40
void CAckMessage::AddData()
{
	Add(m_packetSequence);
	Add(m_subpacketSequence);
}

// FUNCTION: LEMBALL 0x00462b60
void CAckMessage::GetData()
{
	Get(m_packetSequence);
	Get(m_subpacketSequence);
}

// GLOBAL: LEMBALL 0x004a1e14
CAckMessage* g_pAckMessage = 0;
