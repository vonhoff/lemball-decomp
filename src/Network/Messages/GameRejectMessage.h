#ifndef LEMBALL_NETWORK_MESSAGES_GAMEREJECTMESSAGE_H
#define LEMBALL_NETWORK_MESSAGES_GAMEREJECTMESSAGE_H

#include "../../Common.h"
#include "GameFlaggedMessage.h" // complete type

// SIZE 0x30
// VTABLE: LEMBALL 0x004985b8
class GameRejectMessage : public GameFlaggedMessage {
public:
	GameRejectMessage();
	virtual ~GameRejectMessage(); // vtable+0x14
};

// SYNTHETIC: LEMBALL 0x00452ec0
// GameRejectMessage::`scalar deleting destructor'

#endif
