#include "CGameAcceptMessage.h"

#include "Network/Messages/CGameFlaggedMessage.h"

// FUNCTION: LEMBALL 0x00452530
CGameAcceptMessage::CGameAcceptMessage() : CGameFlaggedMessage(GAME_MESSAGE_ACCEPT)
{
}
