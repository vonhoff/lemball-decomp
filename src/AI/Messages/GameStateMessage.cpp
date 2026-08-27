#include "GameStateMessage.h"

#define MESSAGE_GAME_STATE 10

// 68K 0x1060000c __ct__17CGameStateMessageFv
// FUNCTION: LEMBALL 0x00410b80
GameStateMessage::GameStateMessage()
	: NetworkMessage(MESSAGE_GAME_STATE)
{
	m_payloadCapacity += 16;
	m_headerEnabled = 1;
}

// 68K 0x10600066 AddData__17CGameStateMessageFv
// FUNCTION: LEMBALL 0x00410bb0
void GameStateMessage::AddData()
{
	Add((unsigned long) m_state);
	Add((unsigned long) m_stage);
	Add((unsigned long) m_levelTime);
	Add((unsigned long) m_score);
}

// 68K 0x106000d4 GetData__17CGameStateMessageFv
// FUNCTION: LEMBALL 0x00410be0
void GameStateMessage::GetData()
{
	m_state = (eGameStates) GetDword();
	m_stage = (eGameStateStages) GetDword();
	Get(m_levelTime);
	Get(m_score);
}

// 68K 0x101181dc __dt__17CGameStateMessageFv
GameStateMessage::~GameStateMessage()
{
}

