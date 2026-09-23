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
	m_object->m_position.m_xFixed = GetDWORD() << 12;
	m_object->m_position.m_yFixed = GetDWORD() << 12;
	m_object->m_position.m_zFixed = GetDWORD() << 12;
	m_object->m_objectActive = GetDWORD();
}
