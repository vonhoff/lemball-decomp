#include "CBroadcastMessage.h"

#include <string.h>

#pragma intrinsic(strcmp, strlen)

// FUNCTION: LEMBALL 0x0045f360
void CBroadcastMessage::AddHeader()
{
	Add(m_header);
}

// FUNCTION: LEMBALL 0x0045f370
bool CBroadcastMessage::GetHeader()
{
	int different;

	different = strcmp((char*) m_readCursor, m_header) != 0;
	m_readCursor += strlen(m_header) + 1;
	return different == 0;
}
