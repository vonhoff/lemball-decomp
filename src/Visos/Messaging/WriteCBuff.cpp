#include "WriteCBuff.h"

#include "WritePacket.h"

// 68K 0x1020e50a __ct__11CWriteCBuffFiUs
// FUNCTION: LEMBALL 0x00461970
WriteCBuff::WriteCBuff(int p_arg0, unsigned short p_arg1) : WritePacketBuff(p_arg0, p_arg1)
{
}

// 68K 0x1020e540 StorePacket__11CWriteCBuffFUsPCUciP15CNetworkMessage
// FUNCTION: LEMBALL 0x00461990
WritePacket* WriteCBuff::StorePacket(unsigned short p_arg0,
									 const unsigned char* p_arg1,
									 int p_arg2,
									 NetworkMessage* p_arg3)
{
	int index;
	WritePacket* packet;

	index = p_arg0 % m_packetCount;
	packet = (WritePacket*) m_packets[index];
	if (packet->m_available == 0) {
		return 0;
	}
	FillPacket(index, p_arg1, (unsigned short) p_arg2, p_arg3);
	return packet;
}

// 68K 0x1020e5cc IsPacketAvailable__11CWriteCBuffFUs
// FUNCTION: LEMBALL 0x004619d0
bool WriteCBuff::IsPacketAvailable(unsigned short p_arg0)
{
	return ((WritePacket*) m_packets[(p_arg0 + 1) % m_packetCount])->m_available;
}
