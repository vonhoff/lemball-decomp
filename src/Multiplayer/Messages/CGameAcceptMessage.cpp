#include "CGameAcceptMessage.h"

#include "Multiplayer/Messages/CGameFlaggedMessage.h"

// FUNCTION: LEMBALL 0x00452530
CGameAcceptMessage::CGameAcceptMessage() : CGameFlaggedMessage(GAME_MESSAGE_ACCEPT)
{
}
