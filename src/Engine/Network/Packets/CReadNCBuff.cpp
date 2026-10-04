#include "CReadNCBuff.h"

#include "Platform/Windows/TcpIp/CTCPIPNetwork.h"
#include "BasePacketHeader.h"
#include "Engine/Network/Protocol/CNetworkMessage.h"
#include "CReadPacket.h"
#include "CReadPacketBuff.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00461560
CReadNCBuff::CReadNCBuff(unsigned long p_lastMessageId, unsigned short p_packetSize)
	: CReadPacketBuff(p_lastMessageId - NETWORK_MESSAGE_SEQUENCE_TRACKING_START_ID + 1, p_packetSize)
{
	m_messageSlots = p_lastMessageId - NETWORK_MESSAGE_SEQUENCE_TRACKING_START_ID + 1;
}

// FUNCTION: LEMBALL 0x00461580
CReadPacket* CReadNCBuff::UpdatePacket()
{
	unsigned short messageId = g_pNetworkPacketScratch->m_messageId;
	int isNew = 0;
	unsigned int index = messageId;
	CReadPacket* packet;

	if (messageId >= NETWORK_MESSAGE_SEQUENCE_TRACKING_START_ID) {
		index -= NETWORK_MESSAGE_SEQUENCE_TRACKING_START_ID;
	}

	packet = (CReadPacket*) m_packets[index];
	BasePacketHeader* packetHeader = (BasePacketHeader*) packet->m_data;
	if (messageId < NETWORK_MESSAGE_SEQUENCE_TRACKING_START_ID ||
		(int) g_pNetworkPacketScratch->m_packetSequence - (int) packetHeader->m_packetSequence > 0) {
		isNew = packet->m_used == 0;
		FillPacket(index);
	}

	if (isNew) {
		return (CReadPacket*) m_packets[index];
	}
	return NULL;
}

// FUNCTION: LEMBALL 0x004615f0
CReadPacket* CReadNCBuff::GetPacket(unsigned long p_messageId)
{
	if ((int) p_messageId >= NETWORK_MESSAGE_SEQUENCE_TRACKING_START_ID) {
		p_messageId -= NETWORK_MESSAGE_SEQUENCE_TRACKING_START_ID;
	}
	return (CReadPacket*) m_packets[p_messageId];
}
