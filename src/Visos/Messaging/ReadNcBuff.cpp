#include "ReadNcBuff.h"

#include "../Network/TcpIpNetwork.h"
#include "BasePacketHeader.h"
#include "ReadPacket.h"

// 68K 0x1020de9c __ct__11CReadNCBuffFUlUs
// FUNCTION: LEMBALL 0x00461560
ReadNcBuff::ReadNcBuff(unsigned long p_arg0, unsigned short p_arg1) : ReadPacketBuff(p_arg0 - 2, p_arg1)
{
	m_messageSlots = p_arg0 - 2;
}

// 68K 0x1020deea UpdatePacket__11CReadNCBuffFv
// FUNCTION: LEMBALL 0x00461580
ReadPacket* ReadNcBuff::UpdatePacket()
{
	unsigned short messageId = g_pNetworkPacketScratch->m_messageId;
	int isNew = 0;
	unsigned int index = messageId;
	ReadPacket* packet;

	if (messageId >= 3) {
		index -= 3;
	}

	packet = (ReadPacket*) m_packets[index];
	if (messageId < 3 ||
		(int) g_pNetworkPacketScratch->m_packetSequence - (int) ((BasePacketHeader*) packet->m_data)->m_packetSequence >
			0) {
		isNew = packet->m_used == 0;
		FillPacket(index);
	}

	if (isNew) {
		return (ReadPacket*) m_packets[index];
	}
	return 0;
}

// 68K 0x1020df94 GetPacket__11CReadNCBuffFUl
// FUNCTION: LEMBALL 0x004615f0
ReadPacket* ReadNcBuff::GetPacket(unsigned long p_arg0)
{
	if ((int) p_arg0 >= 3) {
		p_arg0 -= 3;
	}
	return (ReadPacket*) m_packets[p_arg0];
}
