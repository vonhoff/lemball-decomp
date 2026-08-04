#include "AI/CPlayerLemming.h"

// FUNCTION: LEMBALL 0x0040f0d0
void CPlayerLemming::SetGroup(CPlayerLemmingGroup* pGroup)
{
	m_pGroup224 = pGroup;
}

// FUNCTION: LEMBALL 0x0040f0e0
CPlayerLemmingGroup* CPlayerLemming::GetGroup(void)
{
	return m_pGroup224;
}

// FUNCTION: LEMBALL 0x0040f0f0
void CPlayerLemming::SetGroupLeader(int fLeader)
{
	m_fGroupLeader17C = fLeader;
}

// FUNCTION: LEMBALL 0x0040f100
void CPlayerLemming::SetGroup(int fGrouped)
{
	m_fGrouped180 = fGrouped;
}
