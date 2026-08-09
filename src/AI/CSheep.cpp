#include "AI/CSheep.h"
#include <string.h>

extern void* g_pActiveManagedEntityOwner;

struct LevelChunkObjectRuntimeStateView {
	void ResetRuntimeStateThunk(void);
};

struct LevelVtSmallFunctionView {
	void AddLevelScoreClamped(int nValue);
};

// FUNCTION: LEMBALL 0x0041f9f0
void CSheep::Restart(void)
{
	int aBounds[6];
	int nX;
	int nY;
	int nZ;
	void* pOwner;

	((LevelChunkObjectRuntimeStateView*) this)->ResetRuntimeStateThunk();
	nX = m_nSpawnX40;
	nY = m_nSpawnY44;
	nZ = m_nSpawnZ48;
	m_nWorldX9C = nX;
	m_nWorldYA0 = nY;
	m_nWorldZA4 = nZ;
	nX >>= 12;
	nY >>= 12;
	nZ >>= 12;
	aBounds[0] = nX - 8;
	aBounds[1] = nY - 15;
	aBounds[2] = nZ;
	aBounds[3] = nX + 7;
	aBounds[4] = nY + 7;
	aBounds[5] = nZ + 15;
	memcpy(m_anBounds14, aBounds, sizeof(aBounds));
	m_sHeadingB4 = m_sSavedHeadingB6;
	pOwner = g_pActiveManagedEntityOwner;
	(*(void***) ((char*) pOwner + 0x120))[*(int*) ((char*) pOwner + 0x118)] = this;
	++*(int*) ((char*) pOwner + 0x118);
}

// FUNCTION: LEMBALL 0x0041fad0
void CSheep::HitMine(void)
{
	((LevelVtSmallFunctionView*) g_pActiveManagedEntityOwner)->AddLevelScoreClamped(0x96);
	int vec[3];
	vec[0] = 0;
	vec[1] = 0;
	vec[2] = 0xa000;
	(*( void(**)(void*, int)) (*(void***) ((void**) m_pVtable00) + 0x20 / 4))(vec, 0);
}
