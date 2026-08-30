#ifndef LEMBALL_NETWORK_MESSAGES_GAMEACCEPTMESSAGE_H
#define LEMBALL_NETWORK_MESSAGES_GAMEACCEPTMESSAGE_H

#include "../../Common.h"
#include "GameFlaggedMessage.h" // complete type

// SIZE 0x30
// VTABLE: LEMBALL 0x004985d8
class GameAcceptMessage : public GameFlaggedMessage {
public:
	GameAcceptMessage();
	virtual ~GameAcceptMessage(); // vtable+0x14
};

// SYNTHETIC: LEMBALL 0x00452ef0
// GameAcceptMessage::`scalar deleting destructor'

#endif
