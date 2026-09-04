#include "Headers.h"

#include "HeaderMessage.h"

#include <new.h>

// 68K 0x10206dc4 __ct__8CHeadersFi
// FUNCTION: LEMBALL 0x00479620
Headers::Headers(int p_arg0)
{
	int index;

	m_count = p_arg0;
	m_headers = new HeaderMessage[p_arg0];
	m_sequences = new unsigned short[m_count];
	for (index = 0; index < m_count; index++) {
		m_sequences[index] = 0;
	}
	m_payloadCapacity += m_headers->m_payloadCapacity * p_arg0;
}

// 68K 0x10206e82 __dt__8CHeadersFv
// FUNCTION: LEMBALL 0x004796e0
Headers::~Headers()
{
	operator delete(m_sequences);
	delete[] m_headers;
}

// 68K 0x10206eec AddData__8CHeadersFv
// FUNCTION: LEMBALL 0x00479790
void Headers::AddData()
{
	int i;
	HeaderMessage* header;

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

// 68K 0x10206f50 GetData__8CHeadersFv
// FUNCTION: LEMBALL 0x004797d0
void Headers::GetData()
{
	int i;
	HeaderMessage* header;

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
