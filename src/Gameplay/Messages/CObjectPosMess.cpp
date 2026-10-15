#include "CObjectPosMess.h"

#include "CGameObjectMess.h"
#include "Engine/Math/FixedPoint.h"
#include "Engine/Math/FixedPointConversion.h"
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
	m_object->m_position.m_xFixed = IntegerToFixed(GetDWORD());
	m_object->m_position.m_yFixed = IntegerToFixed(GetDWORD());
	m_object->m_position.m_zFixed = IntegerToFixed(GetDWORD());
	m_object->m_objectActive = GetDWORD();
}
