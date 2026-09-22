#include "CReadNcBuff.h"

#include "../Network/CTcpIpNetwork.h"
#include "BasePacketHeader.h"
#include "CReadPacket.h"
#include "Visos/Messaging/CReadPacketBuff.h"

// FUNCTION: LEMBALL 0x00461560
CReadNcBuff::CReadNcBuff(unsigned long p_arg0, unsigned short p_arg1) : CReadPacketBuff(p_arg0 - 2, p_arg1)
{
	m_messageSlots = p_arg0 - 2;
}

// FUNCTION: LEMBALL 0x00461580
CReadPacket* CReadNcBuff::UpdatePacket()
{
	unsigned short messageId = g_pNetworkPacketScratch->m_messageId;
	int isNew = 0;
	unsigned int index = messageId;
	CReadPacket* packet;

	if (messageId >= 3) {
		index -= 3;
	}

	packet = (CReadPacket*) m_packets[index];
	if (messageId < 3 ||
		(int) g_pNetworkPacketScratch->m_packetSequence - (int) ((BasePacketHeader*) packet->m_data)->m_packetSequence >
			0) {
		isNew = packet->m_used == 0;
		FillPacket(index);
	}

	if (isNew) {
		return (CReadPacket*) m_packets[index];
	}
	return 0;
}

// FUNCTION: LEMBALL 0x004615f0
CReadPacket* CReadNcBuff::GetPacket(unsigned long p_arg0)
{
	if ((int) p_arg0 >= 3) {
		p_arg0 -= 3;
	}
	return (CReadPacket*) m_packets[p_arg0];
}
