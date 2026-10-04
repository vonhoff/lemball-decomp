#include "CHeaderMessage.h"

// FUNCTION: LEMBALL 0x00479540
CHeaderMessage::CHeaderMessage()
{
	m_mirroredSequence = 1;
	m_payloadCapacity += sizeof(m_sequence) + sizeof(m_headerValue) + sizeof(m_text0) + sizeof(m_text1);
	m_sequence = 0;
	m_headerValue = 0;
	m_text1[0] = 0;
	m_text0[0] = 0;
}

// FUNCTION: LEMBALL 0x00479580
void CHeaderMessage::AddData()
{
	unsigned short sequence;
	unsigned long value;

	++m_sequence;
	Add(m_sequence);
	sequence = m_sequence;
	value = m_headerValue;
	m_mirroredSequence = sequence;
	Add(value);
	Add((const unsigned char*) m_text1, sizeof(m_text1));
	Add((const unsigned char*) m_text0, sizeof(m_text0));
}

// FUNCTION: LEMBALL 0x004795d0
void CHeaderMessage::GetData()
{
	Get(m_sequence);
	if (m_mirroredSequence != m_sequence) {
		Get(m_headerValue);
		GetCopy((unsigned char*) m_text1, sizeof(m_text1));
		GetCopy((unsigned char*) m_text0, sizeof(m_text0));
		return;
	}
	m_readCursor += sizeof(m_headerValue) + sizeof(m_text1) + sizeof(m_text0);
}
