#include "CReadMSBuff.h"

#include "../Network/CTCPIPNetwork.h"
#include "BasePacketHeader.h"

#include <memory.h>

#pragma intrinsic(memcpy)

// FUNCTION: LEMBALL 0x004613d0
CReadMSBuff::CReadMSBuff(int p_messageSlots, int p_messageCapacity, unsigned short p_packetSize)
{
	unsigned int payloadSize;

	m_messageSlotCount = p_messageSlots;
	m_assembledSize = 0;
	m_receivedSubpacketCount = 0;
	payloadSize = (p_packetSize & 0xffff) - sizeof(BasePacketHeader);
	m_subpacketPayloadSize = payloadSize;
	if (p_messageSlots != 0 && payloadSize != 0) {
		m_data = (unsigned char*) operator new(p_messageCapacity + sizeof(BasePacketHeader));
		((BasePacketHeader*) m_data)->m_packetSequence = 0;
	}
	else {
		m_data = 0;
	}
}

// FUNCTION: LEMBALL 0x00461440
CReadMSBuff::~CReadMSBuff()
{
	if (m_data != 0) {
		operator delete(m_data);
	}
}

// FUNCTION: LEMBALL 0x00461470
void CReadMSBuff::FillPacket()
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
