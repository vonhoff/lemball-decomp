#ifndef LEMBALL_SCAFFOLD_NETWORK_GAMEREJECTMESSAGE_H
#define LEMBALL_SCAFFOLD_NETWORK_GAMEREJECTMESSAGE_H

#include "../Common.h"
#include "GameFlaggedMessage.h" // complete type

// SIZE 0x30
// VTABLE: LEMBALL 0x004985b8
class GameRejectMessage : public GameFlaggedMessage {
public:
	GameRejectMessage();
	virtual ~GameRejectMessage(); // vtable+0x14
};

#endif
