#include "CObjectPosMess.h"

#include "CGameObjectMess.h"
#include "Engine/Math/FixedPoint.h"
#include "Engine/Math/FixedPointConversion.h"
#include "GameMessageIds.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Multiplayer/Transport/Protocol/CNetworkMessage.h"

// FUNCTION: LEMBALL 0x00416bb0
CObjectPosMess::CObjectPosMess() : CGameObjectMess(MESSAGE_OBJECT_POS)
{
	m_payloadCapacity += 4 * sizeof(Dword);
}

// FUNCTION: LEMBALL 0x00416bd0
void CObjectPosMess::AddData()
{
	CGameObjectMess::AddData();
	Add((Dword) (m_object->m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS));
	Add((Dword) (m_object->m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS));
	Add((Dword) (m_object->m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS));
	Add((Dword) m_object->m_objectActive);
}

// FUNCTION: LEMBALL 0x00416c30
void CObjectPosMess::GetData()
{
	m_object->m_position.m_xFixed = IntegerToFixed(GetDWORD());
	m_object->m_position.m_yFixed = IntegerToFixed(GetDWORD());
	m_object->m_position.m_zFixed = IntegerToFixed(GetDWORD());
	m_object->m_objectActive = GetDWORD();
}
