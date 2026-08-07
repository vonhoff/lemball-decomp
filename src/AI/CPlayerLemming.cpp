#include "AI/CPlayerLemming.h"

extern void* g_pSessionRandomState;

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

// MACINTOSH: CPlayerLemming::SetBored(int)
// FUNCTION: LEMBALL 0x00410090
void CPlayerLemming::SetBored(int nBaseTime)
{
	unsigned int uRandom;

	uRandom = *(int*) g_pSessionRandomState * 0x29 + 0x1fU & 0x7fffff;
	*(unsigned int*) g_pSessionRandomState = uRandom;
	uRandom = nBaseTime + uRandom % 5000;
	*(unsigned int*) ((char*) this + 0x190) = uRandom;
	uRandom = uRandom - uRandom % 0x42;
	*(unsigned int*) ((char*) this + 0x190) = uRandom;
	uRandom = uRandom / 0x32;
	*(unsigned int*) ((char*) this + 0x190) = uRandom;
	*(unsigned int*) ((char*) this + 0x190) = *(int*) ((char*) this + 0xcc) + uRandom;
}

