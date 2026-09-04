#include "ReadCmsBuff.h"

#include "../Network/TcpIpNetwork.h"
#include "BasePacketHeader.h"

#include <new.h>

// 68K 0x1020e358 __ct__12CReadCMSBuffFiiUs
// FUNCTION: LEMBALL 0x00461840
ReadCmsBuff::ReadCmsBuff(int p_arg0, int p_arg1, unsigned short p_arg2) : ReadMsBuff(p_arg0, p_arg1, p_arg2)
{
	int index;

	m_messageCount = p_arg0;
	if (p_arg1 > 0) {
		m_messages = (ReadMsBuff**) operator new(p_arg0 * sizeof(ReadMsBuff*));
		for (index = 0; index < p_arg0; index++) {
			m_messages[index] = new ReadMsBuff(p_arg0, p_arg1, p_arg2);
		}
	}
	else {
		m_messages = 0;
	}
}

// 68K 0x1020e402 __dt__12CReadCMSBuffFv
// FUNCTION: LEMBALL 0x004618e0
ReadCmsBuff::~ReadCmsBuff()
{
	if (m_messages != 0) {
		int index;

		for (index = 0; index < m_messageCount; index++) {
			if (m_messages[index] != 0) {
				delete m_messages[index];
			}
		}
		operator delete(m_messages);
	}
}

// 68K 0x1020e49e StoreSubPacket__12CReadCMSBuffFv
// FUNCTION: LEMBALL 0x00461930
ReadMsBuff* ReadCmsBuff::StoreSubPacket()
{
	BasePacketHeader* packet = g_pNetworkPacketScratch;

	m_messages[packet->m_packetSequence % m_messageCount]->FillPacket();
	return m_messages[packet->m_packetSequence % m_messageCount];
}
