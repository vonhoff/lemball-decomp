#include "ReadCBuff.h"

#include "../Network/TcpIpNetwork.h"
#include "BasePacketHeader.h"
#include "ReadPacket.h"

// 68K 0x1020e212 __ct__10CReadCBuffFiUs
// FUNCTION: LEMBALL 0x00461780
ReadCBuff::ReadCBuff(int p_arg0, unsigned short p_arg1) : ReadPacketBuff(p_arg0, p_arg1)
{
	m_nextSequence = 0;
	m_outOfOrder = 0;
}

// 68K 0x1020e258 StorePacket__10CReadCBuffFv
// FUNCTION: LEMBALL 0x004617a0
ReadPacket* ReadCBuff::StorePacket()
{
	unsigned short* packetSequence = &g_pNetworkPacketScratch->m_packetSequence;
	unsigned short index = *packetSequence % m_packetCount;
	ReadPacket* packet = (ReadPacket*) m_packets[index];

	if (packet->m_used == 0 && packet->m_ready != 0) {
		FillPacket(index);
		if (m_nextSequence - *packetSequence == -1) {
			m_nextSequence++;
			packet->m_ready = 1;
			return packet;
		}
		packet->m_ready = 0;
		return 0;
	}

	m_outOfOrder = 1;
	return 0;
}

// 68K 0x1020e2f0 NextPacketReady__10CReadCBuffFv
// FUNCTION: LEMBALL 0x00461810
ReadPacket* ReadCBuff::NextPacketReady()
{
	unsigned short index = m_nextSequence % m_packetCount;
	ReadPacket* packet = (ReadPacket*) m_packets[index];

	if (packet->m_used != 0 && packet->m_ready == 0) {
		m_nextSequence++;
		packet->m_ready = 1;
		return packet;
	}
	return 0;
}
