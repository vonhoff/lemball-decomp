#include "ReadMsBuff.h"

#include "../Foundation/VsMem.h"

ReadMsBuff::ReadMsBuff()
{
}

// 68K 0x1020dc92 __ct__11CReadMSBuffFiiUs
// FUNCTION: LEMBALL 0x004613d0
ReadMsBuff::ReadMsBuff(int p_arg0, int p_arg1, unsigned short p_arg2)
{
	unsigned int payloadSize;

	m_expectedSubpacketCount = p_arg0;
	m_assembledSize = 0;
	m_receivedSubpacketCount = 0;
	payloadSize = (p_arg2 & 0xffff) - 0x10;
	m_subpacketPayloadSize = payloadSize;
	if (p_arg0 != 0 && payloadSize != 0) {
		m_buffer = (unsigned char*) operator new(p_arg1 + 0x10);
		*(unsigned short*) (m_buffer + 0x0a) = 0;
	}
	else {
		m_buffer = 0;
	}
}

// 68K 0x1020dd1a __dt__11CReadMSBuffFv
// STUB: LEMBALL 0x00461440
ReadMsBuff::~ReadMsBuff()
{
}

// 68K 0x1020dd7a FillPacket__11CReadMSBuffFv
// STUB: LEMBALL 0x00461470
void ReadMsBuff::FillPacket()
{
}
