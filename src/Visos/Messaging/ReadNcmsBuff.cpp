#include "ReadNcmsBuff.h"

#include "../Network/TcpIpNetwork.h"
#include "BasePacketHeader.h"

#include <new.h>

// 68K 0x1020dfd8 __ct__13CReadNCMSBuffFUlUliUs
// FUNCTION: LEMBALL 0x00461610
ReadNcmsBuff::ReadNcmsBuff(unsigned long p_arg0, unsigned long p_arg1, int p_arg2, unsigned short p_arg3)
	: ReadMsBuff(p_arg1 - p_arg0 + 1, p_arg2, p_arg3)
{
	int index;

	m_firstMessageId = p_arg0;
	m_messageCount = p_arg1 - p_arg0 + 1;
	m_nextExpectedSequence = 0;
	if (p_arg2 > 0) {
		m_messages = (ReadMsBuff**) operator new(m_messageCount * sizeof(ReadMsBuff*));
		for (index = 0; index < m_messageCount; index++) {
			m_messages[index] = new ReadMsBuff(m_messageCount, p_arg2, p_arg3);
		}
	}
	else {
		m_messages = 0;
	}
}

// 68K 0x1020e0a4 __dt__13CReadNCMSBuffFv
// FUNCTION: LEMBALL 0x004616b0
ReadNcmsBuff::~ReadNcmsBuff()
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

// 68K 0x1020e140 UpdateSubPacket__13CReadNCMSBuffFv
// FUNCTION: LEMBALL 0x00461700
ReadMsBuff* ReadNcmsBuff::UpdateSubPacket()
{
	unsigned short messageId = g_pNetworkPacketScratch->m_messageId;
	unsigned int index = messageId - m_firstMessageId;
	ReadMsBuff* message = m_messages[index];
	unsigned short packetSequence;

	if (messageId >= 3 && (int) g_pNetworkPacketScratch->m_packetSequence -
								  (int) ((BasePacketHeader*) message->m_data)->m_packetSequence <
							  0) {
		return 0;
	}

	packetSequence = g_pNetworkPacketScratch->m_packetSequence;
	if ((int) m_nextExpectedSequence > (int) (unsigned int) packetSequence) {
		return 0;
	}

	if (((BasePacketHeader*) message->m_data)->m_packetSequence != packetSequence &&
		(int) message->m_receivedSubpacketCount > 0) {
		m_nextExpectedSequence = (unsigned int) packetSequence + 1;
		return 0;
	}

	message->FillPacket();
	return m_messages[index];
}
