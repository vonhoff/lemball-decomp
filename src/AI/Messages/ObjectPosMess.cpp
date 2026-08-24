#include "ObjectPosMess.h"

#include "../Base/GlobalGameObject.h"

// 68K 0x1060b864 __ct__14CObjectPosMessFv
// FUNCTION: LEMBALL 0x00416bb0
ObjectPosMess::ObjectPosMess()
	: GameObjectMess(MESSAGE_OBJECT_POS)
{
	m_payloadCapacity += 16;
}

// 68K 0x1060b8ac AddData__14CObjectPosMessFv
// FUNCTION: LEMBALL 0x00416bd0
void ObjectPosMess::AddData()
{
	GameObjectMess::AddData();
	Add((unsigned long) (m_object->m_position.m_xFixed >> 12));
	Add((unsigned long) (m_object->m_position.m_yFixed >> 12));
	Add((unsigned long) (m_object->m_position.m_zFixed >> 12));
	Add((unsigned long) m_object->m_heading);
}

// 68K 0x1060b95e GetData__14CObjectPosMessFv
// FUNCTION: LEMBALL 0x00416c30
void ObjectPosMess::GetData()
{
	unsigned long x, y, z;
	x = GetDword();
	m_object->m_position.m_xFixed = x << 12;
	y = GetDword();
	m_object->m_position.m_yFixed = y << 12;
	z = GetDword();
	m_object->m_position.m_zFixed = z << 12;
	m_object->m_heading = GetDword();
	(void) &x;
	(void) &y;
	(void) &z;
}

// 68K 0x1011a2c0 __dt__14CObjectPosMessFv
// SYNTHETIC: LEMBALL 0x00417c70
// ObjectPosMess::`scalar deleting destructor'
ObjectPosMess::~ObjectPosMess()
{
}
