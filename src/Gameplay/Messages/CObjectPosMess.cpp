#include "CObjectPosMess.h"

#include "CGameObjectMess.h"
#include "Engine/Math/FixedPoint.h"
#include "GameMessageIds.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGlobalGameObject.h"

// FUNCTION: LEMBALL 0x00416bb0
CObjectPosMess::CObjectPosMess() : CGameObjectMess(MESSAGE_OBJECT_POS)
{
	m_payloadCapacity += 4 * sizeof(unsigned long);
}

// FUNCTION: LEMBALL 0x00416bd0
void CObjectPosMess::AddData()
{
	CGameObjectMess::AddData();
	Add((unsigned long) (m_object->m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS));
	Add((unsigned long) (m_object->m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS));
	Add((unsigned long) (m_object->m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS));
	Add((unsigned long) m_object->m_objectActive);
}

// FUNCTION: LEMBALL 0x00416c30
void CObjectPosMess::GetData()
{
	const unsigned long& x = GetDWORD() << FIXED_POINT_FRACTION_BITS;
	m_object->m_position.m_xFixed = x;
	const unsigned long& y = GetDWORD() << FIXED_POINT_FRACTION_BITS;
	m_object->m_position.m_yFixed = y;
	const unsigned long& z = GetDWORD() << FIXED_POINT_FRACTION_BITS;
	m_object->m_position.m_zFixed = z;
	m_object->m_objectActive = GetDWORD();
}
