#include "CBaseSocket.h"

#include "../Messaging/BasePacketHeader.h"

// FUNCTION: LEMBALL 0x0045f750
CBaseSocket::CBaseSocket()
{
	m_receiveSequence = 0;
	m_payloadCapacity += sizeof(BasePacketHeader);
	m_multiMessageSequence = 0;
	m_criticalSequence = 0;
	m_nonCriticalSequence = 0;
}

// FUNCTION: LEMBALL 0x0045f790
void CBaseSocket::AddData()
{
	Add((unsigned long) m_packetHeader->m_magic);
	Add((unsigned long) m_packetHeader->m_packetSize);
	Add(m_packetHeader->m_messageId);
	Add(m_packetHeader->m_packetSequence);
	Add(m_packetHeader->m_subpacketSequence);
	Add(m_packetHeader->m_critical);
}

// FUNCTION: LEMBALL 0x0045f7f0
void CBaseSocket::GetData()
{
	ConvertDWORD();
	ConvertDWORD();
	ConvertWORD();
	ConvertWORD();
	ConvertWORD();
	ConvertBYTE();
}
