#include "CReadCmsBuff.h"

#include "../Network/CTcpIpNetwork.h"
#include "BasePacketHeader.h"
#include "Visos/Messaging/CReadMsBuff.h"

// FUNCTION: LEMBALL 0x00461840
CReadCmsBuff::CReadCmsBuff(int p_messageCount, int p_messageCapacity, unsigned short p_packetSize)
	: CReadMsBuff(p_messageCount, p_messageCapacity, p_packetSize)
{
	int index;

	m_messageCount = p_messageCount;
	if (p_messageCapacity > 0) {
		m_messages = (CReadMsBuff**) operator new(p_messageCount * sizeof(CReadMsBuff*));
		for (index = 0; index < p_messageCount; index++) {
			m_messages[index] = new CReadMsBuff(p_messageCount, p_messageCapacity, p_packetSize);
		}
	}
	else {
		m_messages = 0;
	}
}

// FUNCTION: LEMBALL 0x004618e0
CReadCmsBuff::~CReadCmsBuff()
{
	if (m_messages != 0) {
		int index;

		for (index = 0; index < m_messageCount; index++) {
			delete m_messages[index];
		}
		operator delete(m_messages);
	}
}

// FUNCTION: LEMBALL 0x00461930
CReadMsBuff* CReadCmsBuff::StoreSubPacket()
{
	BasePacketHeader* packet = g_pNetworkPacketScratch;

	m_messages[packet->m_packetSequence % m_messageCount]->FillPacket();
	return m_messages[packet->m_packetSequence % m_messageCount];
}
