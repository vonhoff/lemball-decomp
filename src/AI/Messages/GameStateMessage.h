#ifndef LEMBALL_AI_MESSAGES_GAMESTATEMESSAGE_H
#define LEMBALL_AI_MESSAGES_GAMESTATEMESSAGE_H

#include "../../Common.h"
#include "../../Visos/Messaging/NetworkMessage.h" // complete type

// SIZE 0x3c
// VTABLE: LEMBALL 0x00493a00
class GameStateMessage : public NetworkMessage {
public:
	GameStateMessage();
	virtual void AddData();      // vtable+0x10
	virtual void GetData();      // vtable+0x08
	virtual ~GameStateMessage(); // vtable+0x14

private:
	eGameStates m_state;       // 0x2c
	eGameStateStages m_stage;  // 0x30
	unsigned long m_levelTime; // 0x34
	unsigned long m_score;     // 0x38
};

// SYNTHETIC: LEMBALL 0x00413df0
// GameStateMessage::`scalar deleting destructor'

#endif
