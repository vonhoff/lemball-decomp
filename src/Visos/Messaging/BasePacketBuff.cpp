#include "BasePacketBuff.h"

#include "BasePacket.h"

#include <new.h>

BasePacketBuff::BasePacketBuff()
{
}

// 68K 0x1020d9a0 __ct__15CBasePacketBuffFiUs
// FUNCTION: LEMBALL 0x00461210
BasePacketBuff::BasePacketBuff(int p_arg0, unsigned short p_arg1)
{
	*(unsigned int*) &m_packetSize = p_arg1;
	m_packetCount = p_arg0;
	if (p_arg0 > 0) {
		m_packets = (BasePacket**) operator new(p_arg0 * sizeof(BasePacket*));
	}
	else {
		m_packets = 0;
	}
}

// 68K 0x1020da00 __dt__15CBasePacketBuffFv
// FUNCTION: LEMBALL 0x00461250
BasePacketBuff::~BasePacketBuff()
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
