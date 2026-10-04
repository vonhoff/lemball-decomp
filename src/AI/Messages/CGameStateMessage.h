#ifndef LEMBALL_AI_MESSAGES_CGAMESTATEMESSAGE_H
#define LEMBALL_AI_MESSAGES_CGAMESTATEMESSAGE_H

#include "../../Visos/Messaging/CNetworkMessage.h"

enum eGameStates {
	GAME_STATE_PAUSED = 0,
	GAME_STATE_RUNNING = 1,
	GAME_STATE_SUCCESS = 2,
	GAME_STATE_COMPLETING = 3,
	GAME_STATE_FAILURE = 4,
	GAME_STATE_QUIT = 6,
	GAME_STATE_TIME_EXPIRED = 7,
	GAME_STATE_RESTART = 8
};

enum eGameStateStages {
	GAME_STATE_STAGE_REQUEST = 0,
	GAME_STATE_STAGE_CONFIRM = 1,
	GAME_STATE_STAGE_REJECT = 2
};

// SIZE 0x3c
// VTABLE: LEMBALL 0x00493a00
class CGameStateMessage : public CNetworkMessage {
public:
	CGameStateMessage();
	virtual void AddData(); // vtable+0x10
	virtual void GetData(); // vtable+0x08

private:
	friend class CAI;

	eGameStates m_state;       // 0x2c
	eGameStateStages m_stage;  // 0x30
	unsigned long m_levelTime; // 0x34
	unsigned long m_score;     // 0x38
};

// SYNTHETIC: LEMBALL 0x00413df0
// CGameStateMessage::`scalar deleting destructor'

#endif
