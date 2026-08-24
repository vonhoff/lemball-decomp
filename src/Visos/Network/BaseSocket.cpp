#include "BaseSocket.h"

#include "../Messaging/BasePacketHeader.h"

// 68K 0x1020b3a4 __ct__11CBaseSocketFv
// FUNCTION: LEMBALL 0x0045f750
BaseSocket::BaseSocket()
{
	m_receiveSequence = 0;
	m_payloadCapacity += 0x10;
	m_multiMessageSequence = 0;
	m_criticalSequence = 0;
	m_nonCriticalSequence = 0;
}

// 68K 0x1020b3fe AddData__11CBaseSocketFv
// FUNCTION: LEMBALL 0x0045f790
void BaseSocket::AddData()
{
	Add((unsigned long) m_packetHeader->m_magic);
	Add((unsigned long) m_packetHeader->m_packetSize);
	Add(m_packetHeader->m_messageId);
	Add(m_packetHeader->m_packetSequence);
	Add(m_packetHeader->m_subpacketSequence);
	Add(m_packetHeader->m_critical);
}

// 68K 0x1020b480 GetData__11CBaseSocketFv
// FUNCTION: LEMBALL 0x0045f7f0
void BaseSocket::GetData()
{
	ConvertDword();
	ConvertDword();
	ConvertWord();
	ConvertWord();
	ConvertWord();
	ConvertByte();
}

// 68K 0x1010619c __dt__11CBaseSocketFv
// SYNTHETIC: LEMBALL 0x00462900
// BaseSocket::`scalar deleting destructor'
BaseSocket::~BaseSocket()
{
}
