#include "CObjectPosMess.h"

#include "../Base/CGlobalGameObject.h"
#include "AI/Base/AiCoord.h"
#include "AI/Messages/CGameObjectMess.h"
#include "AI/Messages/GameMessageIds.h"

// FUNCTION: LEMBALL 0x00416bb0
CObjectPosMess::CObjectPosMess() : CGameObjectMess(MESSAGE_OBJECT_POS)
{
	m_payloadCapacity += 16;
}

// FUNCTION: LEMBALL 0x00416bd0
void CObjectPosMess::AddData()
{
	CGameObjectMess::AddData();
	Add((unsigned long) (m_object->m_position.m_xFixed >> 12));
	Add((unsigned long) (m_object->m_position.m_yFixed >> 12));
	Add((unsigned long) (m_object->m_position.m_zFixed >> 12));
	Add((unsigned long) m_object->m_objectActive);
}

// FUNCTION: LEMBALL 0x00416c30
void CObjectPosMess::GetData()
{
	const unsigned long& x = GetDWORD() << 12;
	m_object->m_position.m_xFixed = x;
	const unsigned long& y = GetDWORD() << 12;
	m_object->m_position.m_yFixed = y;
	const unsigned long& z = GetDWORD() << 12;
	m_object->m_position.m_zFixed = z;
	m_object->m_objectActive = GetDWORD();
}
