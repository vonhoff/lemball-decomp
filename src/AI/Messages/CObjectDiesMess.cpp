#include "CObjectDiesMess.h"

#include "AI/Messages/CGameObjectMess.h"
#include "AI/Messages/GameMessageIds.h"

// FUNCTION: LEMBALL 0x00416ce0
CObjectDiesMess::CObjectDiesMess() : CGameObjectMess(MESSAGE_OBJECT_DIES)
{
	m_payloadCapacity += 4;
}

// FUNCTION: LEMBALL 0x00416d00 FOLDED
void CObjectDiesMess::AddData()
{
	CGameObjectMess::AddData();
}

// FUNCTION: LEMBALL 0x00416d10
void CObjectDiesMess::GetData()
{
}
