#include "CObjectHitMess.h"

#include "CGameObjectMess.h"
#include "GameMessageIds.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Simulation/AIScoreConstants.h"
#include "Gameplay/Simulation/CAI.h"

// FUNCTION: LEMBALL 0x00416c80
CObjectHitMess::CObjectHitMess() : CGameObjectMess(MESSAGE_OBJECT_HIT)
{
	m_payloadCapacity += sizeof(unsigned long);
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
	GetDWORD();
	g_pAI->Score(AI_SCORE_OBJECT_HIT_MESSAGE_POINTS);
}
