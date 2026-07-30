#include "ENGINE/CORE/COMMON.H"

extern int g_nLevelFrameClockTimeMs;
extern int g_nLevelFrameClockTick;

extern void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);

struct Type18ChunkObjectRuntimeState {
	char m_abReserved00[0x40];
	int m_nPositionX40;
	int m_nPositionY44;
	int m_nPositionZ48;
	char m_abReserved4C[0x48];
	int m_nFrameTime94;
	char m_abReserved98[4];
	int m_nSavedPositionX9C;
	int m_nSavedPositionYA0;
	int m_nSavedPositionZA4;
	char m_abReservedA8[0x10];
	int m_nStateIdB8;
	char m_abReservedBC[0x10];
	int m_nFrameTickCC;
	char m_abReservedD0[0x78];
	int m_fTransitionActive148;
	int m_nTransitionDuration14C;
};

// FUNCTION: LEMBALL 0x0040c350
void LEMBALL_FASTCALL ResetType18ChunkObjectRuntimeState(void* pObject)
{
	Type18ChunkObjectRuntimeState* pState;
	int y;
	int z;
	int x;
	int nTick;

	pState = (Type18ChunkObjectRuntimeState*) pObject;
	ResetManagedEntityRuntimeStateThunk(pObject);
	x = pState->m_nPositionX40;
	y = pState->m_nPositionY44;
	z = pState->m_nPositionZ48;
	pState->m_nSavedPositionX9C = x;
	pState->m_nSavedPositionYA0 = y;
	pState->m_nSavedPositionZA4 = z;
	pState->m_nStateIdB8 = 0x18;
	pState->m_nFrameTime94 = g_nLevelFrameClockTimeMs;
	nTick = g_nLevelFrameClockTick;
	pState->m_fTransitionActive148 = 1;
	pState->m_nTransitionDuration14C = 0x50;
	pState->m_nFrameTickCC = nTick;
}
