#include "CReadCmsBuff.h"

#include "../Network/CTcpIpNetwork.h"
#include "BasePacketHeader.h"
#include "Visos/Messaging/CReadMsBuff.h"

// FUNCTION: LEMBALL 0x00461840
CReadCmsBuff::CReadCmsBuff(int p_arg0, int p_arg1, unsigned short p_arg2) : CReadMsBuff(p_arg0, p_arg1, p_arg2)
{
	int index;

	m_messageCount = p_arg0;
	if (p_arg1 > 0) {
		m_messages = (CReadMsBuff**) operator new(p_arg0 * sizeof(CReadMsBuff*));
		for (index = 0; index < p_arg0; index++) {
			m_messages[index] = new CReadMsBuff(p_arg0, p_arg1, p_arg2);
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
