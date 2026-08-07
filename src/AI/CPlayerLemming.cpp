#include "AI/CPlayerLemming.h"

extern void* g_pSessionRandomState;
extern int g_nLevelFrameClockTick;

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

// MACINTOSH: CPlayerLemming::RandomAction()
// FUNCTION: LEMBALL 0x0040fa10
int CPlayerLemming::RandomAction(void)
{
	int nRandom;
	int nVariant;

	nRandom = *(int*) g_pSessionRandomState * 0x29 + 0x1f & 0x7fffff;
	*(int*) g_pSessionRandomState = nRandom;
	nVariant = nRandom % 3;
	*(short*) ((char*) this + 0xbc) = (short) nVariant;
	if (nVariant == 0) {
		nRandom = g_nLevelFrameClockTick + 0x38;
		*(int*) ((char*) this + 0xcc) = nRandom;
		return nRandom;
	}
	if (nVariant != 1) {
		if (nVariant != 2) {
			return nRandom / 3;
		}
		nRandom = g_nLevelFrameClockTick + 0x26;
		*(int*) ((char*) this + 0xcc) = nRandom;
		return nRandom;
	}
	nRandom = g_nLevelFrameClockTick + 0x21;
	*(int*) ((char*) this + 0xcc) = nRandom;
	return nRandom;
}


