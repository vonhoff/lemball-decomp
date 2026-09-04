#include "ReadMsBuff.h"

#include "../Foundation/VsMem.h"
#include "../Network/TcpIpNetwork.h"
#include "BasePacketHeader.h"

#include <memory.h>

#pragma intrinsic(memcpy)

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
	payloadSize = (p_arg2 & 0xffff) - sizeof(BasePacketHeader);
	m_subpacketPayloadSize = payloadSize;
	if (p_arg0 != 0 && payloadSize != 0) {
		m_data = (unsigned char*) operator new(p_arg1 + sizeof(BasePacketHeader));
		((BasePacketHeader*) m_data)->m_packetSequence = 0;
	}
	else {
		m_data = 0;
	}
}

// 68K 0x1020dd1a __dt__11CReadMSBuffFv
// FUNCTION: LEMBALL 0x00461440
ReadMsBuff::~ReadMsBuff()
{
	if (m_data != 0) {
		operator delete(m_data);
	}
}

// 68K 0x1020dd7a FillPacket__11CReadMSBuffFv
// FUNCTION: LEMBALL 0x00461470
void ReadMsBuff::FillPacket()
{
	unsigned short* subpacketSequence = &g_pNetworkPacketScratch->m_subpacketSequence;

	if (*subpacketSequence == 0) {
		if (m_receivedSubpacketCount == 0) {
			memcpy(m_data, g_pNetworkPacketScratch, g_receivedPacketSize);
			m_assembledSize += g_receivedPacketSize;
		}
		else {
			memcpy(m_data + sizeof(BasePacketHeader),
				   g_pNetworkPacketScratch + 1,
				   g_receivedPacketSize - sizeof(BasePacketHeader));
			m_assembledSize += g_receivedPacketSize - sizeof(BasePacketHeader);
		}
	}
	else {
		if (m_receivedSubpacketCount == 0) {
			*(BasePacketHeader*) m_data = *g_pNetworkPacketScratch;
			m_assembledSize += sizeof(BasePacketHeader);
		}

		memcpy(m_data + sizeof(BasePacketHeader) + m_subpacketPayloadSize * *subpacketSequence,
			   g_pNetworkPacketScratch + 1,
			   g_receivedPacketSize - sizeof(BasePacketHeader));
		m_assembledSize += g_receivedPacketSize - sizeof(BasePacketHeader);
	}

	m_receivedSubpacketCount++;
}
