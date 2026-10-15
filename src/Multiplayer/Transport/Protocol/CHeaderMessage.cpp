#include "CHeaderMessage.h"

// FUNCTION: LEMBALL 0x00479540
CHeaderMessage::CHeaderMessage()
{
	m_sequenceState.m_mirroredSequence = 1;
	m_payloadCapacity += sizeof(m_sequenceState.m_sequence) + sizeof(m_packetSize) + sizeof(m_sourceAddressText) +
						 sizeof(m_destinationAddressText);
	m_sequenceState.m_sequence = 0;
	m_packetSize = 0;
	m_destinationAddressText[0] = 0;
	m_sourceAddressText[0] = 0;
}

// FUNCTION: LEMBALL 0x00479580
void CHeaderMessage::AddData()
{
	unsigned short sequence;
	unsigned long packetSize;

	++m_sequenceState.m_sequence;
	Add(m_sequenceState.m_sequence);
	sequence = m_sequenceState.m_sequence;
	packetSize = m_packetSize;
	m_sequenceState.m_mirroredSequence = sequence;
	Add(packetSize);
	Add((const unsigned char*) m_destinationAddressText, sizeof(m_destinationAddressText));
	Add((const unsigned char*) m_sourceAddressText, sizeof(m_sourceAddressText));
}

// FUNCTION: LEMBALL 0x004795d0
void CHeaderMessage::GetData()
{
	Get(m_sequenceState.m_sequence);
	if (m_sequenceState.m_mirroredSequence != m_sequenceState.m_sequence) {
		Get(m_packetSize);
		GetCopy((unsigned char*) m_destinationAddressText, sizeof(m_destinationAddressText));
		GetCopy((unsigned char*) m_sourceAddressText, sizeof(m_sourceAddressText));
		return;
	}
	m_readCursor += sizeof(m_packetSize) + sizeof(m_destinationAddressText) + sizeof(m_sourceAddressText);
}
