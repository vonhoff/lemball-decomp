#include "ObjectHitMess.h"

#include "../Base/GlobalGameObject.h"
#include "../Navigation/Ai.h"

// 68K 0x1060ba1e __ct__14CObjectHitMessFv
// FUNCTION: LEMBALL 0x00416c80
ObjectHitMess::ObjectHitMess() : GameObjectMess(MESSAGE_OBJECT_HIT)
{
	m_payloadCapacity += 4;
}

// 68K 0x1060ba64 AddData__14CObjectHitMessFv
// FUNCTION: LEMBALL 0x00416ca0
void ObjectHitMess::AddData()
{
	GameObjectMess::AddData();
	Add((unsigned long) m_object->m_objectType);
}

// 68K 0x1060bab0 GetData__14CObjectHitMessFv
// FUNCTION: LEMBALL 0x00416cc0
void ObjectHitMess::GetData()
{
	GetDword();
	g_pAI->Score(0x9c4);
}

// 68K 0x1011a322 __dt__14CObjectHitMessFv
ObjectHitMess::~ObjectHitMess()
{
}
