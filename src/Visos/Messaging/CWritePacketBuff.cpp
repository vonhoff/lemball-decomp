#include "CWritePacketBuff.h"

#include "CWritePacket.h"
#include "Visos/Messaging/CBasePacketBuff.h"

// FUNCTION: LEMBALL 0x00461340
CWritePacketBuff::CWritePacketBuff(int p_packetCount, unsigned short p_packetSize)
	: CBasePacketBuff(p_packetCount, p_packetSize)
{
	if (m_packets != 0) {
		int index;

		for (index = 0; index < m_packetCount; index++) {
			m_packets[index] = new CWritePacket(m_packetSize);
		}
	}
}

// FUNCTION: LEMBALL 0x004613a0
void CWritePacketBuff::FillPacket(int p_index,
								  const unsigned char* p_data,
								  unsigned short p_size,
								  CNetworkMessage* p_message)
{
	((CWritePacket*) m_packets[p_index])->Fill(p_data, p_size, p_message);
}
