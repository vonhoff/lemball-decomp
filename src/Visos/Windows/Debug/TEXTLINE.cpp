#include <stdlib.h>

class DebugTextLineRecord {
public:
	int m_nReserved;
	char* m_pszText;

	~DebugTextLineRecord();
};

// FUNCTION: LEMBALL 0x004564c0
DebugTextLineRecord::~DebugTextLineRecord()
{
	if (m_pszText != 0) {
		free(m_pszText);
	}
}
