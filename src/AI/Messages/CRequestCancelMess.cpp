#include "CRequestCancelMess.h"

#include "AI/Messages/CGameObjectMess.h"
#include "AI/Messages/GameMessageIds.h"

// FUNCTION: LEMBALL 0x00416b70
CRequestCancelMess::CRequestCancelMess() : CGameObjectMess(MESSAGE_REQUEST_CANCEL)
{
}
