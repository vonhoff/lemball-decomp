#include "CWriteCBuff.h"

#include "CWritePacket.h"
#include "Visos/Messaging/CWritePacketBuff.h"

// FUNCTION: LEMBALL 0x00461970
CWriteCBuff::CWriteCBuff(int p_packetCount, unsigned short p_packetSize) : CWritePacketBuff(p_packetCount, p_packetSize)
{
}

// FUNCTION: LEMBALL 0x00461990
CWritePacket* CWriteCBuff::StorePacket(unsigned short p_sequence,
									   const unsigned char* p_data,
									   int p_size,
									   CNetworkMessage* p_message)
{
	int index;
	CWritePacket* packet;

	index = p_sequence % m_packetCount;
	packet = (CWritePacket*) m_packets[index];
	if (packet->m_available == 0) {
		return 0;
	}
	FillPacket(index, p_data, (unsigned short) p_size, p_message);
	return packet;
}

// FUNCTION: LEMBALL 0x004619d0
bool CWriteCBuff::IsPacketAvailable(unsigned short p_sequence)
{
	return ((CWritePacket*) m_packets[(p_sequence + 1) % m_packetCount])->m_available;
}
