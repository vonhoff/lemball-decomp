#include "Multiplayer/CGameAcceptMessage.h"

#include "Multiplayer/CGameFlaggedMessage.h"

// FUNCTION: LEMBALL 0x00452530
CGameAcceptMessage::CGameAcceptMessage() : CGameFlaggedMessage(GAME_MESSAGE_ACCEPT)
{
}
