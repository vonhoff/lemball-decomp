#include "ReadPacketBuff.h"

#include "../Network/TcpIpNetwork.h"
#include "ReadPacket.h"

#include <new.h>

ReadPacketBuff::ReadPacketBuff()
{
}

// 68K 0x1020da8a __ct__15CReadPacketBuffFiUs
// FUNCTION: LEMBALL 0x00461290
ReadPacketBuff::ReadPacketBuff(int p_arg0, unsigned short p_arg1) : BasePacketBuff(p_arg0, p_arg1)
{
	if (m_packets != 0) {
		int index;

		for (index = 0; index < m_packetCount; index++) {
			m_packets[index] = new ReadPacket(m_packetSize);
		}
	}
}

// 68K 0x1020db06 FillPacket__15CReadPacketBuffFi
// FUNCTION: LEMBALL 0x004612f0
void ReadPacketBuff::FillPacket(int p_arg0)
{
	((ReadPacket*) m_packets[p_arg0])->Fill((unsigned char*) g_pNetworkPacketScratch, g_receivedPacketSize);
}

// 68K 0x1020db6a UnUseAll__15CReadPacketBuffFv
// FUNCTION: LEMBALL 0x00461310
void ReadPacketBuff::UnUseAll()
{
	int index;

	for (index = 0; index < m_packetCount; index++) {
		((ReadPacket*) m_packets[index])->m_used = 0;
	}
}
