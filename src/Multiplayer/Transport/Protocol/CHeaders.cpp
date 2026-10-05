#include "CHeaders.h"

#include "CHeaderMessage.h"

#include <stddef.h>

enum {
	HEADER_SEQUENCE_ITERATION_EXHAUSTED = -1
};

// FUNCTION: LEMBALL 0x00479620
CHeaders::CHeaders(int p_headerCount)
{
	m_count = p_headerCount;
	CHeaderMessage* headers = new CHeaderMessage[p_headerCount];
	m_headers = headers;
	m_sequences = new unsigned short[m_count];
	for (int index = 0; index < m_count; index++) {
		m_sequences[index] = 0;
	}
	m_payloadCapacity += m_headers->m_payloadCapacity * p_headerCount;
}

// FUNCTION: LEMBALL 0x004796e0
CHeaders::~CHeaders()
{
	operator delete(m_sequences);
	delete[] m_headers;
}

// FUNCTION: LEMBALL 0x00479720
CHeaderMessage* CHeaders::GetNextHeaderBySequence()
{
	int index = m_currentIndex;
	int count = m_count;
	for (; index < count; index++) {
		if (m_sequences[index] < m_headers[index].m_sequenceState.m_sequence) {
			break;
		}
	}
	if (index == count) {
		m_currentIndex = HEADER_SEQUENCE_ITERATION_EXHAUSTED;
		return NULL;
	}
	m_currentIndex = index;
	m_sequences[index] = m_headers[index].m_sequenceState.m_sequence;
	return m_headers + m_currentIndex;
}

// FUNCTION: LEMBALL 0x00479790
void CHeaders::AddData()
{
	int i;
	CHeaderMessage* header;

	i = 0;
	if (m_count > 0) {
		do {
			header = &m_headers[i];
			header->CopyDataStream(m_writeCursor, 0);
			i++;
			m_writeCursor += header->m_writeCursor - header->m_buffer;
		} while (i < m_count);
	}
}

// FUNCTION: LEMBALL 0x004797d0
void CHeaders::GetData()
{
	int i;
	CHeaderMessage* header;

	i = 0;
	if (m_count > 0) {
		do {
			header = &m_headers[i];
			if (header->Set(m_readCursor)) {
				m_readCursor = header->m_readCursor;
			}
			i++;
		} while (i < m_count);
	}
	m_currentIndex = 0;
}
