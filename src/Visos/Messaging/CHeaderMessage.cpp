#include "CHeaderMessage.h"

// FUNCTION: LEMBALL 0x00479540
CHeaderMessage::CHeaderMessage()
{
	m_mirroredSequence = 1;
	m_payloadCapacity += 0x30;
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
	Add((unsigned short) m_sequence);
	sequence = m_sequence;
	value = m_headerValue;
	m_mirroredSequence = sequence;
	Add(value);
	Add((const unsigned char*) m_text1, 0x15);
	Add((const unsigned char*) m_text0, 0x15);
}

// FUNCTION: LEMBALL 0x004795d0
void CHeaderMessage::GetData()
{
	Get(m_sequence);
	if (m_mirroredSequence != m_sequence) {
		Get(m_headerValue);
		GetCopy((unsigned char*) m_text1, 0x15);
		GetCopy((unsigned char*) m_text0, 0x15);
		return;
	}
	m_readCursor += 0x2e;
}
