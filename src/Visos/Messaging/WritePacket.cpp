#include "WritePacket.h"

#include <memory.h>
#include <new.h>

#pragma intrinsic(memcpy)

// 68K 0x1020d87c __ct__12CWritePacketFUs
// FUNCTION: LEMBALL 0x00461190
WritePacket::WritePacket(unsigned short p_arg0)
{
	m_data = (unsigned char*) operator new(p_arg0);
	m_available = 1;
	m_retryCount = 0;
}

// 68K 0x1020d8d4 __dt__12CWritePacketFv
// FUNCTION: LEMBALL 0x004611c0
WritePacket::~WritePacket()
{
	operator delete(m_data);
}

// 68K 0x1020d932 Fill__12CWritePacketFPCUcUsP15CNetworkMessage
// FUNCTION: LEMBALL 0x004611e0
void WritePacket::Fill(const unsigned char* p_arg0, unsigned short p_arg1, NetworkMessage* p_arg2)
{
	m_message = p_arg2;
	m_retryCount = 0;
	memcpy(m_data, p_arg0, p_arg1);
}
