#include "ObjectDiesMess.h"

#include "AI/Messages/GameMessageIds.h"
#include "AI/Messages/GameObjectMess.h"

// 68K 0x1060baf0 __ct__15CObjectDiesMessFv
// FUNCTION: LEMBALL 0x00416ce0
ObjectDiesMess::ObjectDiesMess() : GameObjectMess(MESSAGE_OBJECT_DIES)
{
	m_payloadCapacity += 4;
}

// 68K 0x1060bb36 AddData__15CObjectDiesMessFv
// FUNCTION: LEMBALL 0x00416d00 FOLDED
void ObjectDiesMess::AddData()
{
	GameObjectMess::AddData();
}

// 68K 0x1060bb66 GetData__15CObjectDiesMessFv
// FUNCTION: LEMBALL 0x00416d10
void ObjectDiesMess::GetData()
{
}
