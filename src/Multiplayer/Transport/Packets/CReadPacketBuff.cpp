#include "CReadPacketBuff.h"

#include "CBasePacketBuff.h"
#include "CReadPacket.h"
#include "Multiplayer/Transport/CBaseNetwork.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00461290
CReadPacketBuff::CReadPacketBuff(int p_packetCount, unsigned short p_packetSize)
	: CBasePacketBuff(p_packetCount, p_packetSize)
{
	if (m_packets != NULL) {
		int index;

		for (index = 0; index < m_packetCount; index++) {
			m_packets[index] = new CReadPacket(m_packetSize);
		}
	}
}

// FUNCTION: LEMBALL 0x004612f0
void CReadPacketBuff::FillPacket(int p_index)
{
	((CReadPacket*) m_packets[p_index])->Fill((unsigned char*) g_pNetworkPacketScratch, g_receivedPacketSize);
}

// FUNCTION: LEMBALL 0x00461310
void CReadPacketBuff::UnUseAll()
{
	int offset;
	int index = 0;
	if (m_packetCount > 0) {
		offset = 0;
		do {
			offset += sizeof(CReadPacket*);
			index++;
			((CReadPacket*) m_packets[index - 1])->m_used = 0;
		} while (index < m_packetCount);
	}
}
