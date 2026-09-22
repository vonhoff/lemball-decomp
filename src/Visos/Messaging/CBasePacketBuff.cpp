#include "CBasePacketBuff.h"

#include "CBasePacket.h"

// FUNCTION: LEMBALL 0x00461210
CBasePacketBuff::CBasePacketBuff(int p_packetCount, unsigned short p_packetSize)
{
	m_packetSize = p_packetSize;
	m_packetCount = p_packetCount;
	if (p_packetCount > 0) {
		m_packets = (CBasePacket**) operator new(p_packetCount * sizeof(CBasePacket*));
	}
	else {
		m_packets = 0;
	}
}

// FUNCTION: LEMBALL 0x00461250
CBasePacketBuff::~CBasePacketBuff()
{
	if (m_packets != 0) {
		int index;

		for (index = 0; index < m_packetCount; index++) {
			if (m_packets[index] != 0) {
				delete m_packets[index];
			}
		}
		operator delete(m_packets);
	}
}
