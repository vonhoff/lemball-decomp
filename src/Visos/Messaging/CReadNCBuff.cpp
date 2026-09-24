#include "CReadNCBuff.h"

#include "../Network/CTCPIPNetwork.h"
#include "BasePacketHeader.h"
#include "CReadPacket.h"
#include "Visos/Messaging/CReadPacketBuff.h"

// FUNCTION: LEMBALL 0x00461560
CReadNCBuff::CReadNCBuff(unsigned long p_lastMessageId, unsigned short p_packetSize)
	: CReadPacketBuff(p_lastMessageId - 2, p_packetSize)
{
	m_messageSlots = p_lastMessageId - 2;
}

// FUNCTION: LEMBALL 0x00461580
CReadPacket* CReadNCBuff::UpdatePacket()
{
	unsigned short messageId = g_pNetworkPacketScratch->m_messageId;
	int isNew = 0;
	unsigned int index = messageId;
	CReadPacket* packet;

	if (messageId >= 3) {
		index -= 3;
	}

	packet = (CReadPacket*) m_packets[index];
	BasePacketHeader* packetHeader = (BasePacketHeader*) packet->m_data;
	if (messageId < 3 || (int) g_pNetworkPacketScratch->m_packetSequence - (int) packetHeader->m_packetSequence > 0) {
		isNew = packet->m_used == 0;
		FillPacket(index);
	}

	if (isNew) {
		return (CReadPacket*) m_packets[index];
	}
	return 0;
}

// FUNCTION: LEMBALL 0x004615f0
CReadPacket* CReadNCBuff::GetPacket(unsigned long p_messageId)
{
	if ((int) p_messageId >= 3) {
		p_messageId -= 3;
	}
	return (CReadPacket*) m_packets[p_messageId];
}
