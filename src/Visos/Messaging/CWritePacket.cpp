#include "CWritePacket.h"

#include <memory.h>

#pragma intrinsic(memcpy)

// FUNCTION: LEMBALL 0x00461190
CWritePacket::CWritePacket(unsigned short p_capacity)
{
	m_data = (unsigned char*) operator new(p_capacity);
	m_available = 1;
	m_retryCount = 0;
}

// FUNCTION: LEMBALL 0x004611c0
CWritePacket::~CWritePacket()
{
	operator delete(m_data);
}

// FUNCTION: LEMBALL 0x004611e0
void CWritePacket::Fill(const unsigned char* p_data, unsigned short p_size, CNetworkMessage* p_message)
{
	m_message = p_message;
	m_retryCount = 0;
	memcpy(m_data, p_data, p_size);
}
