#include "AI/CHand.h"

#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

extern void LEMBALL_FASTCALL DestroyLevelChunkObjectBaseAutoThunk(void* pObject);
extern void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);
extern void* g_pLevelTileGrid;
extern int g_nLevelFrameClockTick;
extern int g_nLevelFrameClockTimeMs;

struct LevelChunkObjectBaseView {
	void* InitializeLevelChunkObjectBase(int nType, unsigned short nChildType, unsigned short nFlags);
};

struct ManagedEntityStateView {
	void RequestManagedEntityStateId(int nStateId);
};

struct LevelTileCell {
	unsigned char m_abReserved00[7];
	unsigned char m_nFlags07;
	unsigned char m_abReserved08[4];
};

struct LevelTileGrid {
	unsigned char m_abReserved00[0xc];
	LevelTileCell* m_pCells0C;
	int m_nWidth10;
	int m_nHeight14;
};

typedef void(LEMBALL_FASTCALL* HandStateProc)(void* pObject, void* pUnused, int nState);

// FUNCTION: LEMBALL 0x00427ad0
CHand::CHand(void)
{
	((LevelChunkObjectBaseView*) this)->InitializeLevelChunkObjectBase(0x1f, 0, 0);
	m_pVtable00 = &g_LINKSCF_CaptureChunkObjectVtable;
}

// FUNCTION: LEMBALL 0x00427af0
void CHand::Initialise(void)
{
	m_nFrameTime94 = 0;
	m_fActivated138 = 0;
	m_fConfigured13C = 0;
}

// FUNCTION: LEMBALL 0x00427b10
void CHand::Restart(void)
{
	ResetManagedEntityRuntimeStateThunk(this);
	Initialise();
	m_WorldPosition9C.x = m_InitialPosition40.x;
	m_WorldPosition9C.y = m_InitialPosition40.y;
	m_WorldPosition9C.z = m_InitialPosition40.z;
}

// FUNCTION: LEMBALL 0x00427b40
CHand::~CHand(void)
{
	m_pVtable00 = &g_LINKSCF_CaptureChunkObjectVtable;
	DestroyLevelChunkObjectBaseAutoThunk(this);
}

// FUNCTION: LEMBALL 0x00427b50
void CHand::Set(unsigned short nSlotId, const AICOORD& position)
{
	LevelTileGrid* pGrid;
	int nTileX;
	int nTileY;
	int y;

	SetId(nSlotId);
	m_InitialPosition40.x = position.x;
	m_InitialPosition40.y = position.y;
	m_InitialPosition40.z = position.z;
	m_WorldPosition9C.x = position.x;
	m_WorldPosition9C.y = position.y;
	m_WorldPosition9C.z = position.z;
	m_fConfigured13C = 1;
	m_nStateB8 = 0x18;
	m_nActionPhaseBC = 0;
	m_fActivated138 = 0;

	nTileX = (position.x >> 12) / 16;
	nTileY = (position.y >> 12) / 16;
	pGrid = (LevelTileGrid*) g_pLevelTileGrid;
	for (y = nTileY + 1; y <= nTileY + 2; ++y) {
		if (nTileX >= 0 && y >= 0 && nTileX < pGrid->m_nWidth10 && y < pGrid->m_nHeight14) {
			pGrid->m_pCells0C[y * pGrid->m_nWidth10 + nTileX].m_nFlags07 |= 0x80;
		}
	}
}

// FUNCTION: LEMBALL 0x00427c40
int CHand::Process(void)
{
	int nState;

	if (m_nPendingState114 != 0) {
		m_nActionPhaseBC = 1;
		if (m_nLastProcessedState128 != m_nStateB8) {
			if (m_nStateB8 == 0x1a) {
				((HandStateProc) ((void**) m_pVtable00)[13])(this, 0, 0x1e);
			}
			m_nLastProcessedState128 = m_nStateB8;
		}
		return 1;
	}

	m_nActionPhaseBC = 0;
	if (m_fActivated138 == 0) {
		return 1;
	}
	nState = m_nStateB8;
	if (nState == 0x17) {
		if ((unsigned int) m_nNextUpdateTickCC < (unsigned int) g_nLevelFrameClockTick) {
			m_fConfigured13C = 1;
			m_fActivated138 = 0;
			((HandStateProc) ((void**) m_pVtable00)[2])(this, 0, 0x18);
		}
	}
	else if (nState == 0x19) {
		if ((unsigned int) m_nSecondaryUpdateTickD0 < (unsigned int) g_nLevelFrameClockTick) {
			((HandStateProc) ((void**) m_pActivatedObject140->m_pVtable00)[2])(m_pActivatedObject140, 0, 0x15);
			m_pActivatedObject140->m_nNextUpdateTickCC = g_nLevelFrameClockTick + 0x28;
			((HandStateProc) ((void**) m_pVtable00)[2])(this, 0, 0x1a);
			((HandStateProc) ((void**) m_pVtable00)[13])(this, 0, 0x1e);
		}
	}
	else if (nState == 0x1a && (unsigned int) m_nNextUpdateTickCC < (unsigned int) g_nLevelFrameClockTick) {
		m_fConfigured13C = 1;
		m_nNextUpdateTickCC = g_nLevelFrameClockTick + 0x14;
		((HandStateProc) ((void**) m_pVtable00)[2])(this, 0, 0x17);
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00427d70
int CHand::StepOn(const AICOORD& position, CGameObject* pObject)
{
	int nDeltaX;
	int nDeltaY;

	if (pObject->m_nEntityType64 != 2) {
		return 0;
	}
	nDeltaX = (position.x >> 12) - (m_WorldPosition9C.x >> 12);
	if (nDeltaX < 0) {
		nDeltaX = -nDeltaX;
	}
	nDeltaY = (position.y >> 12) - (m_WorldPosition9C.y >> 12);
	if (nDeltaX >= 0x10 || nDeltaY < 0 || nDeltaY >= 0x30) {
		return 0;
	}

	m_nSecondaryUpdateTickD0 = 6;
	m_nNextUpdateTickCC = 0x10;
	m_pTarget5C = pObject;
	pObject->ResetInstructions();
	((HandStateProc) ((void**) pObject->m_pVtable00)[2])(pObject, 0, 0);
	pObject->m_nNextUpdateTickCC = g_nLevelFrameClockTick + 1000;
	((ManagedEntityStateView*) this)->RequestManagedEntityStateId(0x19);
	return 1;
}

// FUNCTION: LEMBALL 0x00427e10
void CHand::DoActivate(void)
{
	m_fActivated138 = 1;
	m_pActivatedObject140 = m_pTarget5C;
	m_nMotionStartTickC8 = g_nLevelFrameClockTick;
	m_nSecondaryUpdateTickD0 += g_nLevelFrameClockTick;
	m_nNextUpdateTickCC += g_nLevelFrameClockTick;
	m_nFrameTime94 = g_nLevelFrameClockTimeMs;
}
