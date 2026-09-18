#include "GameAcceptMessage.h"

#include "Network/Messages/GameFlaggedMessage.h"

// 68K 0x10a00300 __ct__18CGameAcceptMessageFv
// FUNCTION: LEMBALL 0x00452530
GameAcceptMessage::GameAcceptMessage() : GameFlaggedMessage(7)
{
}
