#include "CGameRejectMessage.h"

#include "Network/Messages/CGameFlaggedMessage.h"

// FUNCTION: LEMBALL 0x00452510
CGameRejectMessage::CGameRejectMessage() : CGameFlaggedMessage(GAME_MESSAGE_REJECT)
{
}
