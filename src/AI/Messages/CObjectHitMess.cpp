#include "CObjectHitMess.h"

#include "../Base/CGlobalGameObject.h"
#include "../Navigation/CAi.h"
#include "AI/Messages/CGameObjectMess.h"
#include "AI/Messages/GameMessageIds.h"

// FUNCTION: LEMBALL 0x00416c80
CObjectHitMess::CObjectHitMess() : CGameObjectMess(MESSAGE_OBJECT_HIT)
{
	m_payloadCapacity += 4;
}

// FUNCTION: LEMBALL 0x00416ca0
void CObjectHitMess::AddData()
{
	CGameObjectMess::AddData();
	Add((unsigned long) m_object->m_objectType);
}

// FUNCTION: LEMBALL 0x00416cc0
void CObjectHitMess::GetData()
{
	GetDword();
	g_pAI->Score(0x9c4);
}
