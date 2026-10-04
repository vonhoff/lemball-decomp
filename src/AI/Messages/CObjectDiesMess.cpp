#include "CObjectDiesMess.h"

#include "AI/Messages/CGameObjectMess.h"
#include "AI/Messages/GameMessageIds.h"

#define OBJECT_DIES_MESSAGE_EXTRA_CAPACITY_BYTES 4

// FUNCTION: LEMBALL 0x00416ce0
CObjectDiesMess::CObjectDiesMess() : CGameObjectMess(MESSAGE_OBJECT_DIES)
{
	m_payloadCapacity += OBJECT_DIES_MESSAGE_EXTRA_CAPACITY_BYTES;
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
