#include "CBasePacketBuff.h"

#include "CBasePacket.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00461210
CBasePacketBuff::CBasePacketBuff(int p_packetCount, unsigned short p_packetSize)
{
	m_packetSize = p_packetSize;
	m_packetCount = p_packetCount;
	if (p_packetCount > 0) {
		m_packets = (CBasePacket**) operator new(p_packetCount * sizeof(CBasePacket*));
	}
	else {
		m_packets = NULL;
	}
}

// FUNCTION: LEMBALL 0x00461250
CBasePacketBuff::~CBasePacketBuff()
{
	if (m_packets != NULL) {
		int index;

		for (index = 0; index < m_packetCount; index++) {
			if (m_packets[index] != NULL) {
				delete m_packets[index];
			}
		}
		operator delete(m_packets);
	}
}
