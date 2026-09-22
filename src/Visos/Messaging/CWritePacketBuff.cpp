#include "CWritePacketBuff.h"

#include "CWritePacket.h"
#include "Visos/Messaging/CBasePacketBuff.h"

// FUNCTION: LEMBALL 0x00461340
CWritePacketBuff::CWritePacketBuff(int p_arg0, unsigned short p_arg1) : CBasePacketBuff(p_arg0, p_arg1)
{
	if (m_packets != 0) {
		int index;

		for (index = 0; index < m_packetCount; index++) {
			m_packets[index] = new CWritePacket(m_packetSize);
		}
	}
}

// FUNCTION: LEMBALL 0x004613a0
void CWritePacketBuff::FillPacket(int p_arg0,
								  const unsigned char* p_arg1,
								  unsigned short p_arg2,
								  CNetworkMessage* p_arg3)
{
	((CWritePacket*) m_packets[p_arg0])->Fill(p_arg1, p_arg2, p_arg3);
}
