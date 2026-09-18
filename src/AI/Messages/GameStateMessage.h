#ifndef LEMBALL_AI_MESSAGES_GAMESTATEMESSAGE_H
#define LEMBALL_AI_MESSAGES_GAMESTATEMESSAGE_H

#include "../../Visos/Messaging/NetworkMessage.h" // complete type

enum eGameStates {
	GAME_STATE_0 = 0,
	GAME_STATE_1 = 1,
	GAME_STATE_2 = 2,
	GAME_STATE_3 = 3,
	GAME_STATE_4 = 4,
	GAME_STATE_6 = 6,
	GAME_STATE_7 = 7,
	GAME_STATE_8 = 8
};

enum eGameStateStages {
	GAME_STATE_STAGE_REQUEST = 0,
	GAME_STATE_STAGE_CONFIRM = 1,
	GAME_STATE_STAGE_REJECT = 2
};

// SIZE 0x3c
// VTABLE: LEMBALL 0x00493a00
class GameStateMessage : public NetworkMessage {
public:
	GameStateMessage();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08

private:
	friend class Ai;

	eGameStates m_state;       // 0x2c
	eGameStateStages m_stage;  // 0x30
	unsigned long m_levelTime; // 0x34
	unsigned long m_score;     // 0x38
};

// SYNTHETIC: LEMBALL 0x00413df0
// GameStateMessage::`scalar deleting destructor'

#endif
