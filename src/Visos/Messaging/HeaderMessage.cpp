#include "HeaderMessage.h"

// 68K 0x10206c6c __ct__14CHeaderMessageFv
// STUB: LEMBALL 0x00479540
HeaderMessage::HeaderMessage()
{
}

// 68K 0x10206cd2 AddData__14CHeaderMessageFv
// FUNCTION: LEMBALL 0x00479580
void HeaderMessage::AddData()
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

// 68K 0x10206d46 GetData__14CHeaderMessageFv
// FUNCTION: LEMBALL 0x004795d0
void HeaderMessage::GetData()
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

// 68K 0x1011667c __dt__14CHeaderMessageFv
// SYNTHETIC: LEMBALL 0x0047b7f0
// HeaderMessage::`scalar deleting destructor'
HeaderMessage::~HeaderMessage()
{
}
