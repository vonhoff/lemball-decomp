#include "CReadNCMSBuff.h"

#include "BasePacketHeader.h"
#include "CReadMSBuff.h"
#include "Multiplayer/Transport/CBaseNetwork.h"
#include "Multiplayer/Transport/Protocol/CNetworkMessage.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00461610
CReadNCMSBuff::CReadNCMSBuff(unsigned long p_firstMessageId,
							 unsigned long p_lastMessageId,
							 int p_messageCapacity,
							 unsigned short p_packetSize)
	: CReadMSBuff(p_lastMessageId - p_firstMessageId + 1, p_messageCapacity, p_packetSize)
{
	int index;

	m_firstMessageId = p_firstMessageId;
	m_messageCount = p_lastMessageId - p_firstMessageId + 1;
	m_nextExpectedSequence = 0;
	if (p_messageCapacity > 0) {
		m_messages = new CReadMSBuff*[m_messageCount];
		for (index = 0; index < m_messageCount; index++) {
			m_messages[index] = new CReadMSBuff(m_messageCount, p_messageCapacity, p_packetSize);
		}
	}
	else {
		m_messages = NULL;
	}
}

// FUNCTION: LEMBALL 0x004616b0
CReadNCMSBuff::~CReadNCMSBuff()
{
	if (m_messages != NULL) {
		int index;

		for (index = 0; index < m_messageCount; index++) {
			delete m_messages[index];
		}
		delete[] m_messages;
	}
}

// FUNCTION: LEMBALL 0x00461700
CReadMSBuff* CReadNCMSBuff::UpdateSubPacket()
{
	unsigned short messageId = g_pNetworkPacketScratch->m_messageId;
	unsigned int index = messageId - m_firstMessageId;
	CReadMSBuff* message = m_messages[index];
	BasePacketHeader* header = (BasePacketHeader*) message->m_data;
	unsigned short packetSequence;

	if (messageId >= NETWORK_MESSAGE_SEQUENCE_TRACKING_START_ID &&
		g_pNetworkPacketScratch->m_packetSequence - header->m_packetSequence < 0) {
		return NULL;
	}

	packetSequence = g_pNetworkPacketScratch->m_packetSequence;
	if (m_nextExpectedSequence > packetSequence) {
		return NULL;
	}

	if (header->m_packetSequence != packetSequence && message->m_receivedSubpacketCount > 0) {
		m_nextExpectedSequence = packetSequence + 1;
		return NULL;
	}

	message->FillPacket();
	return m_messages[index];
}
