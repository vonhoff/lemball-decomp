#include "GameFlaggedMessage.h"

GameFlaggedMessage::GameFlaggedMessage()
{
}

// 68K 0x10a001da __ct__19CGameFlaggedMessageFUl
// STUB: LEMBALL 0x004524b0
GameFlaggedMessage::GameFlaggedMessage(unsigned long p_arg0)
	: NetworkMessage(p_arg0)
{
	m_payloadCapacity += 4;
	m_headerEnabled = 1;
	m_flag = 0;
}

// 68K 0x10a0023a AddData__19CGameFlaggedMessageFv
// FUNCTION: LEMBALL 0x004524f0
void GameFlaggedMessage::AddData()
{
	Add((unsigned long) m_flag);
}

// 68K 0x10a0027c GetData__19CGameFlaggedMessageFv
// FUNCTION: LEMBALL 0x00452500
void GameFlaggedMessage::GetData()
{
	m_flag = GetDword();
}

// 68K 0x10118182 __dt__19CGameFlaggedMessageFv
GameFlaggedMessage::~GameFlaggedMessage()
{
}

