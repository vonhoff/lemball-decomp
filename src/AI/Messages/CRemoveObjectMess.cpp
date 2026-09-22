#include "CRemoveObjectMess.h"

#include "AI/Messages/CGameObjectMess.h"
#include "AI/Messages/GameMessageIds.h"

// FUNCTION: LEMBALL 0x00416b90
CRemoveObjectMess::CRemoveObjectMess() : CGameObjectMess(MESSAGE_REMOVE_OBJECT)
{
}
