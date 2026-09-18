#include "GameRejectMessage.h"

#include "Network/Messages/GameFlaggedMessage.h"

// 68K 0x10a002ba __ct__18CGameRejectMessageFv
// FUNCTION: LEMBALL 0x00452510
GameRejectMessage::GameRejectMessage() : GameFlaggedMessage(6)
{
}
