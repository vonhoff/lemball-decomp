#include "RemoveObjectMess.h"

#include "AI/Messages/GameMessageIds.h"
#include "AI/Messages/GameObjectMess.h"

// 68K 0x1060b820 __ct__17CRemoveObjectMessFv
// FUNCTION: LEMBALL 0x00416b90
RemoveObjectMess::RemoveObjectMess() : GameObjectMess(MESSAGE_REMOVE_OBJECT)
{
}
