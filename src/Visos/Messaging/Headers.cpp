#include "Headers.h"
#include "HeaderMessage.h"

// 68K 0x10206dc4 __ct__8CHeadersFi
// STUB: LEMBALL 0x00479620
Headers::Headers(int p_arg0)
{
}

// 68K 0x10206eec AddData__8CHeadersFv
// STUB: LEMBALL 0x00479790
void Headers::AddData()
{
	int i;
	int offset;
	HeaderMessage* header;

	i = 0;
	if (m_count > 0) {
		offset = 0;
		do {
			header = (HeaderMessage*) ((char*) m_headers + offset);
			header->CopyDataStream(m_writeCursor, 0);
			offset += sizeof(HeaderMessage);
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

// 68K 0x10206e82 __dt__8CHeadersFv
Headers::~Headers()
{
}

