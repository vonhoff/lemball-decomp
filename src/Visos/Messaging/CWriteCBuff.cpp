#include "CWriteCBuff.h"

#include "CWritePacket.h"
#include "Visos/Messaging/CWritePacketBuff.h"

// FUNCTION: LEMBALL 0x00461970
CWriteCBuff::CWriteCBuff(int p_arg0, unsigned short p_arg1) : CWritePacketBuff(p_arg0, p_arg1)
{
}

// FUNCTION: LEMBALL 0x00461990
CWritePacket* CWriteCBuff::StorePacket(unsigned short p_arg0,
									   const unsigned char* p_arg1,
									   int p_arg2,
									   CNetworkMessage* p_arg3)
{
	int index;
	CWritePacket* packet;

	index = p_arg0 % m_packetCount;
	packet = (CWritePacket*) m_packets[index];
	if (packet->m_available == 0) {
		return 0;
	}
	FillPacket(index, p_arg1, (unsigned short) p_arg2, p_arg3);
	return packet;
}

// FUNCTION: LEMBALL 0x004619d0
bool CWriteCBuff::IsPacketAvailable(unsigned short p_arg0)
{
	return ((CWritePacket*) m_packets[(p_arg0 + 1) % m_packetCount])->m_available;
}
