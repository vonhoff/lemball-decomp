#include "CBasePacketBuff.h"

#include "CBasePacket.h"

// FUNCTION: LEMBALL 0x00461210
CBasePacketBuff::CBasePacketBuff(int p_arg0, unsigned short p_arg1)
{
	m_packetSize = p_arg1;
	m_packetCount = p_arg0;
	if (p_arg0 > 0) {
		m_packets = (CBasePacket**) operator new(p_arg0 * sizeof(CBasePacket*));
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
