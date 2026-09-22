#include "CGameStateMessage.h"

#include "Visos/Messaging/CNetworkMessage.h"

#define MESSAGE_GAME_STATE 10

// FUNCTION: LEMBALL 0x00410b80
CGameStateMessage::CGameStateMessage() : CNetworkMessage(MESSAGE_GAME_STATE)
{
	m_payloadCapacity += 16;
	m_headerEnabled = 1;
}

// FUNCTION: LEMBALL 0x00410bb0
void CGameStateMessage::AddData()
{
	Add((unsigned long) m_state);
	Add((unsigned long) m_stage);
	Add((unsigned long) m_levelTime);
	Add((unsigned long) m_score);
}

// FUNCTION: LEMBALL 0x00410be0
void CGameStateMessage::GetData()
{
	m_state = (eGameStates) GetDword();
	m_stage = (eGameStateStages) GetDword();
	Get(m_levelTime);
	Get(m_score);
}
