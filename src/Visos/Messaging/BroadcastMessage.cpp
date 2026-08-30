#include "BroadcastMessage.h"

#include <string.h>

#pragma intrinsic(strcmp, strlen)

// 68K 0x1020ac3e AddHeader__17CBroadcastMessageFv
// FUNCTION: LEMBALL 0x0045f360
void BroadcastMessage::AddHeader()
{
	Add(m_header);
}

// 68K 0x1020ac7a GetHeader__17CBroadcastMessageFv
// FUNCTION: LEMBALL 0x0045f370
bool BroadcastMessage::GetHeader()
{
	int different;

	different = strcmp((char*) m_readCursor, m_header) != 0;
	m_readCursor += strlen(m_header) + 1;
	return different == 0;
}

// 68K 0x1010609c __dt__17CBroadcastMessageFv
BroadcastMessage::~BroadcastMessage()
{
}
