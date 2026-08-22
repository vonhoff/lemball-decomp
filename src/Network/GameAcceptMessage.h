#ifndef LEMBALL_SCAFFOLD_NETWORK_GAMEACCEPTMESSAGE_H
#define LEMBALL_SCAFFOLD_NETWORK_GAMEACCEPTMESSAGE_H

#include "../Common.h"
#include "GameFlaggedMessage.h" // complete type

// SIZE 0x30
// VTABLE: LEMBALL 0x004985d8
class GameAcceptMessage : public GameFlaggedMessage {
public:
	GameAcceptMessage();
	virtual ~GameAcceptMessage(); // vtable+0x14
};

#endif
