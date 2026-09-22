#include "CReadPacketBuff.h"

#include "../Network/CTcpIpNetwork.h"
#include "CReadPacket.h"
#include "Visos/Messaging/CBasePacketBuff.h"

// FUNCTION: LEMBALL 0x00461290
CReadPacketBuff::CReadPacketBuff(int p_arg0, unsigned short p_arg1) : CBasePacketBuff(p_arg0, p_arg1)
{
	if (m_packets != 0) {
		int index;

		for (index = 0; index < m_packetCount; index++) {
			m_packets[index] = new CReadPacket(m_packetSize);
		}
	}
}

// FUNCTION: LEMBALL 0x004612f0
void CReadPacketBuff::FillPacket(int p_arg0)
{
	((CReadPacket*) m_packets[p_arg0])->Fill((unsigned char*) g_pNetworkPacketScratch, g_receivedPacketSize);
}

// FUNCTION: LEMBALL 0x00461310
void CReadPacketBuff::UnUseAll()
{
	int index;

	for (index = 0; index < m_packetCount; index++) {
		((CReadPacket*) m_packets[index])->m_used = 0;
	}
}
