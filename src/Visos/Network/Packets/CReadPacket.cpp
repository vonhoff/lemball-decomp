#include "CReadPacket.h"

#include "BasePacketHeader.h"

#include <string.h>

// FUNCTION: LEMBALL 0x00461090
CReadPacket::CReadPacket(unsigned short p_capacity)
{
	m_data = (unsigned char*) operator new(p_capacity);
	m_ready = 1;
	m_used = 0;
	((BasePacketHeader*) m_data)->m_packetSequence = 0;
}

// FUNCTION: LEMBALL 0x004610f0
CReadPacket::~CReadPacket()
{
	operator delete(m_data);
}

// FUNCTION: LEMBALL 0x00461140
void CReadPacket::Fill(const unsigned char* p_data, unsigned short p_size)
{
	EnterCritical();
	memcpy(m_data, p_data, p_size);
	m_used = 1;
	LeaveCritical();
}
