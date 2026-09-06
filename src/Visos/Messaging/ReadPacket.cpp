#include "ReadPacket.h"

#include "BasePacketHeader.h"

#include <new.h>
#include <string.h>

// 68K 0x1020d71a __ct__11CReadPacketFUs
// FUNCTION: LEMBALL 0x00461090
ReadPacket::ReadPacket(unsigned short p_capacity)
{
	m_data = (unsigned char*) operator new(p_capacity);
	m_ready = 1;
	m_used = 0;
	((BasePacketHeader*) m_data)->m_packetSequence = 0;
}

// 68K 0x1020d7a0 __dt__11CReadPacketFv
// FUNCTION: LEMBALL 0x004610f0
ReadPacket::~ReadPacket()
{
	operator delete(m_data);
}

// 68K 0x1020d808 Fill__11CReadPacketFPCUcUs
// FUNCTION: LEMBALL 0x00461140
void ReadPacket::Fill(const unsigned char* p_data, unsigned short p_size)
{
	EnterCritical();
	memcpy(m_data, p_data, p_size);
	m_used = 1;
	LeaveCritical();
}
