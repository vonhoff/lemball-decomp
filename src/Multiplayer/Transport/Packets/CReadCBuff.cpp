#include "CReadCBuff.h"

#include "BasePacketHeader.h"
#include "CReadPacket.h"
#include "CReadPacketBuff.h"
#include "Multiplayer/Transport/CBaseNetwork.h"

#include <stddef.h>

enum {
	PACKET_SEQUENCE_NEXT_EXPECTED_DELTA = -1
};

// FUNCTION: LEMBALL 0x00461780
CReadCBuff::CReadCBuff(int p_packetCount, unsigned short p_packetSize) : CReadPacketBuff(p_packetCount, p_packetSize)
{
	m_nextSequence = 0;
	m_outOfOrder = 0;
}

// FUNCTION: LEMBALL 0x004617a0
CReadPacket* CReadCBuff::StorePacket()
{
	unsigned short* packetSequence = &g_pNetworkPacketScratch->m_packetSequence;
	unsigned short index = *packetSequence % m_packetCount;
	CReadPacket* packet = (CReadPacket*) m_packets[index];

	if (packet->m_used == 0 && packet->m_ready != 0) {
		FillPacket(index);
		if (m_nextSequence - *packetSequence == PACKET_SEQUENCE_NEXT_EXPECTED_DELTA) {
			m_nextSequence++;
			packet->m_ready = 1;
			return packet;
		}
		packet->m_ready = 0;
		return NULL;
	}

	m_outOfOrder = 1;
	return NULL;
}

// FUNCTION: LEMBALL 0x00461810
CReadPacket* CReadCBuff::NextPacketReady()
{
	unsigned short index = m_nextSequence % m_packetCount;
	CReadPacket* packet = (CReadPacket*) m_packets[index];

	if (packet->m_used != 0 && packet->m_ready == 0) {
		m_nextSequence++;
		packet->m_ready = 1;
		return packet;
	}
	return NULL;
}
