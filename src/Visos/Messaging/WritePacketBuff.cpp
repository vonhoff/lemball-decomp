#include "WritePacketBuff.h"

#include "WritePacket.h"

#include <new.h>

WritePacketBuff::WritePacketBuff()
{
}

// 68K 0x1020dbb0 __ct__16CWritePacketBuffFiUs
// FUNCTION: LEMBALL 0x00461340
WritePacketBuff::WritePacketBuff(int p_arg0, unsigned short p_arg1) : BasePacketBuff(p_arg0, p_arg1)
{
	if (m_packets != 0) {
		int index;

		for (index = 0; index < m_packetCount; index++) {
			m_packets[index] = new WritePacket(m_packetSize);
		}
	}
}

// 68K 0x1020dc2e FillPacket__16CWritePacketBuffFiPCUcUsP15CNetworkMessage
// FUNCTION: LEMBALL 0x004613a0
void WritePacketBuff::FillPacket(int p_arg0, const unsigned char* p_arg1, unsigned short p_arg2, NetworkMessage* p_arg3)
{
	((WritePacket*) m_packets[p_arg0])->Fill(p_arg1, p_arg2, p_arg3);
}
