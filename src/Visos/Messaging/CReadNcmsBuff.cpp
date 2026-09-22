#include "CReadNcmsBuff.h"

#include "../Network/CTcpIpNetwork.h"
#include "BasePacketHeader.h"
#include "Visos/Messaging/CReadMsBuff.h"

// FUNCTION: LEMBALL 0x00461610
CReadNcmsBuff::CReadNcmsBuff(unsigned long p_firstMessageId,
							 unsigned long p_lastMessageId,
							 int p_messageCapacity,
							 unsigned short p_packetSize)
	: CReadMsBuff(p_lastMessageId - p_firstMessageId + 1, p_messageCapacity, p_packetSize)
{
	int index;

	m_firstMessageId = p_firstMessageId;
	m_messageCount = p_lastMessageId - p_firstMessageId + 1;
	m_nextExpectedSequence = 0;
	if (p_messageCapacity > 0) {
		m_messages = (CReadMsBuff**) operator new(m_messageCount * sizeof(CReadMsBuff*));
		for (index = 0; index < m_messageCount; index++) {
			m_messages[index] = new CReadMsBuff(m_messageCount, p_messageCapacity, p_packetSize);
		}
	}
	else {
		m_messages = 0;
	}
}

// FUNCTION: LEMBALL 0x004616b0
CReadNcmsBuff::~CReadNcmsBuff()
{
	if (m_messages != 0) {
		int index;

		for (index = 0; index < m_messageCount; index++) {
			delete m_messages[index];
		}
		operator delete(m_messages);
	}
}

// FUNCTION: LEMBALL 0x00461700
CReadMsBuff* CReadNcmsBuff::UpdateSubPacket()
{
	unsigned short messageId = g_pNetworkPacketScratch->m_messageId;
	unsigned int index = messageId - m_firstMessageId;
	CReadMsBuff* message = m_messages[index];
	BasePacketHeader* header = (BasePacketHeader*) message->m_data;
	unsigned short packetSequence;

	if (messageId >= 3 && (int) g_pNetworkPacketScratch->m_packetSequence - (int) header->m_packetSequence < 0) {
		return 0;
	}

	packetSequence = g_pNetworkPacketScratch->m_packetSequence;
	if ((int) m_nextExpectedSequence > (int) (unsigned int) packetSequence) {
		return 0;
	}

	if (header->m_packetSequence != packetSequence && (int) message->m_receivedSubpacketCount > 0) {
		m_nextExpectedSequence = (unsigned int) packetSequence + 1;
		return 0;
	}

	message->FillPacket();
	return m_messages[index];
}
