#include "RequestCancelMess.h"

#include "AI/Messages/GameMessageIds.h"
#include "AI/Messages/GameObjectMess.h"

// 68K 0x1060b7da __ct__18CRequestCancelMessFv
// FUNCTION: LEMBALL 0x00416b70
RequestCancelMess::RequestCancelMess() : GameObjectMess(MESSAGE_REQUEST_CANCEL)
{
}
