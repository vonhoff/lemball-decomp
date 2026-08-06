#include "AI/CGameObject.h"

extern void* g_pSessionRandomState;
extern int g_nLevelFrameClockTick;

// GLOBAL: LEMBALL 0x0049d134
int g_nManagedEntityRandomActionPhase = 0;

// FUNCTION: LEMBALL 0x00416510
void CGameObject::StartSommersault(void)
{
	int nRandom = (*(int*) g_pSessionRandomState * 0x29 + 0x1f) & 0x7fffff;
	*(int*) g_pSessionRandomState = nRandom;
	m_nNextUpdateTickCC = g_nLevelFrameClockTick + (nRandom % 500 + 50) / 50;
	m_nActionPhaseBC = (short) g_nManagedEntityRandomActionPhase;
	g_nManagedEntityRandomActionPhase ^= 1;
}

// FUNCTION: LEMBALL 0x00419e10
void __cdecl ThunkArmRandomManagedEntityActionDelay(void* pUnused, CGameObject* pEntity)
{
	(void) pUnused;
	pEntity->StartSommersault();
}
