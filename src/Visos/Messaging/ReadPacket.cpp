#include "ReadPacket.h"
#include <new.h>
#include <string.h>

// 68K 0x1020d71a __ct__11CReadPacketFUs
// STUB: LEMBALL 0x00461090
ReadPacket::ReadPacket(unsigned short p_arg0)
{
	m_data = (unsigned char*) operator new(p_arg0);
	m_ready = 1;
	m_used = 0;
	*(unsigned short*) (m_data + 10) = 0;
}

// 68K 0x1020d7a0 __dt__11CReadPacketFv
// STUB: LEMBALL 0x004610f0
ReadPacket::~ReadPacket()
{
	operator delete(m_data);
}

// 68K 0x1020d808 Fill__11CReadPacketFPCUcUs
// FUNCTION: LEMBALL 0x00461140
void ReadPacket::Fill(const unsigned char* p_arg0, unsigned short p_arg1)
{
	m_critical.EnterCritical();
	memcpy(m_data, p_arg0, p_arg1);
	m_used = 1;
	m_critical.LeaveCritical();
}
