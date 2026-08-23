#include "ObjectDiesMess.h"

// 68K 0x1060baf0 __ct__15CObjectDiesMessFv
// FUNCTION: LEMBALL 0x00416ce0
ObjectDiesMess::ObjectDiesMess()
	: GameObjectMess(MESSAGE_OBJECT_DIES)
{
	m_payloadCapacity += 4;
}

// 68K 0x1060bb36 AddData__15CObjectDiesMessFv
// FUNCTION: LEMBALL 0x00416d00
void ObjectDiesMess::AddData()
{
	GameObjectMess::AddData();
}

// 68K 0x1060bb66 GetData__15CObjectDiesMessFv
// FUNCTION: LEMBALL 0x00416d10
void ObjectDiesMess::GetData()
{
}

// 68K 0x1011a384 __dt__15CObjectDiesMessFv
// SYNTHETIC: LEMBALL 0x00417cd0
// ObjectDiesMess::`scalar deleting destructor'
ObjectDiesMess::~ObjectDiesMess()
{
}
