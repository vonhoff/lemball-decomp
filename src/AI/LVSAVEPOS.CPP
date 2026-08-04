#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "AI/CCatapult.h"
#include "AI/CAmmo.h"
#include "AI/CCrate.h"

void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);
extern int g_nLevelFrameClockTimeMs;
extern int g_nLevelFrameClockTick;
extern void* g_pActiveManagedEntityOwner;

struct ManagedEntityStateView {
	void RequestManagedEntityStateId(int nState);
};

struct SavedPositionTransitionView {
	char m_abReserved00[0x5c];
	void* m_pTarget5C;
	char m_abReserved60[0x34];
	int m_nFrameTime94;
	char m_abReserved98[0x20];
	int m_nStateB8;
	char m_abReservedBC[0x10];
	int m_nPhaseDurationCC;
	int m_nPhaseStepD0;
	int m_nDeadlineD4;

};

struct SavedPositionChunkObjectView {
	char m_abReserved00[0x9c];
	int m_nPositionX9C;
	int m_nPositionYA0;
	int m_nPositionZA4;
	char m_abReservedA8[0x10];
	int m_nStateB8;
	short m_nVariantBC;
	char m_abReservedBE[0x6a];
	int m_nGmobState128;
	char m_abReserved12C[0x0c];
	int m_nSavedPositionX138;
	int m_nSavedPositionY13C;
	int m_nSavedPositionZ140;
	short m_nLinkedVariant144;

	void ResetToSavedPosition(void);
};




struct GmobType14SavedPositionView {
	char m_abReserved00[0xbc];
	short m_nVariantBC;
	char m_abReservedBE[0x86];
	int m_nLinkedVariant144;
	char m_abReserved148[0x14];
	short m_nType14Variant15C;

	void ResetType14State(void);
};

// FUNCTION: LEMBALL 0x0041c6c0
void SavedPositionChunkObjectView::ResetToSavedPosition(void)
{
	ResetManagedEntityRuntimeStateThunk(this);
	m_nPositionX9C = m_nSavedPositionX138;
	m_nPositionYA0 = m_nSavedPositionY13C;
	m_nPositionZA4 = m_nSavedPositionZ140;
	m_nStateB8 = 0x18;
}

// FUNCTION: LEMBALL 0x0041c700
void CCatapult::Restart(void)
{
	((SavedPositionChunkObjectView*) this)->ResetToSavedPosition();
	m_nVariantBC = 0;
}

// FUNCTION: LEMBALL 0x0041c9b0
int CCatapult::Activate(void* pTarget)
{
	int nFrameTime;

	if (m_nStateB8 == 0x18) {
		m_pTarget5C = pTarget;
		nFrameTime = g_nLevelFrameClockTimeMs;
		m_nPhaseStepD0 = 0x20;
		m_nDeadlineD4 = 0x2e;
		m_nPhaseDurationCC = 0x5e;
		m_nFrameTime94 = nFrameTime;
		((ManagedEntityStateView*) this)->RequestManagedEntityStateId(0x19);
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041ca90
void CAmmo::Restart(void)
{
	((SavedPositionChunkObjectView*) this)->ResetToSavedPosition();
	m_nLinkedVariant144 = 0;
}

// FUNCTION: LEMBALL 0x0041cbe0
int CAmmo::Activate(void* pTarget)
{
	typedef int(LEMBALL_FASTCALL * EntityTypeTestProc)(void*, void*, int);

	if (m_nStateB8 == 0x18 &&
		((EntityTypeTestProc) (*(void***) pTarget)[45])(pTarget, 0, m_nObjectType64) == 0) {
		m_nDeadlineD4 = 8;
		m_pTarget5C = pTarget;
		((ManagedEntityStateView*) this)->RequestManagedEntityStateId(0x1a);
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041cca0
void CCrate::Restart(void)
{
	((SavedPositionChunkObjectView*) this)->ResetToSavedPosition();
	m_nState128 = 0x18;
}

// FUNCTION: LEMBALL 0x0041ce50
int CCrate::Activate(void* pTarget)
{
	(void) pTarget;
	if (m_nStateB8 == 0x18) {
		m_nPhaseStepD0 = 0x10;
		m_nDeadlineD4 = 0x1e;
		((ManagedEntityStateView*) this)->RequestManagedEntityStateId(0x19);
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041d100
void GmobType14SavedPositionView::ResetType14State(void)
{
	((SavedPositionChunkObjectView*) this)->ResetToSavedPosition();
	m_nType14Variant15C = 0;
	m_nLinkedVariant144 = 0;
	m_nVariantBC = 0;
}

struct LevelVtSmallFunctionView {
	void AddLevelScoreClamped(int nValue);
};

struct GmobType14ScoreView {
	char m_abReserved00[0x94];
	int m_nFrameTime94;
	char m_abReserved98[0x3c];
	int m_nDeadlineD4;
	char m_abReservedD8[0x6c];
	int m_fScoreAwarded144;

	void AwardScoreOnce(void);
};

// FUNCTION: LEMBALL 0x0041d2e0
void GmobType14ScoreView::AwardScoreOnce(void)
{
	m_nFrameTime94 = g_nLevelFrameClockTimeMs;
	m_nDeadlineD4 += g_nLevelFrameClockTick;
	if (m_fScoreAwarded144 == 0) {
		((LevelVtSmallFunctionView*) g_pActiveManagedEntityOwner)->AddLevelScoreClamped(0x19);
		m_fScoreAwarded144 = 1;
	}
}

struct LevelChunkObjectRuntimeStateView {
	void ResetRuntimeStateThunk(void);
};

struct CGenericGroup {
	char m_abReserved00[0x124];
	int m_nChildCount124;
	void* m_apChildren128[10];

	void ClearChildren(void);
};

// MACINTOSH: CGenericGroup::Restart()
// FUNCTION: LEMBALL 0x0041de80
void CGenericGroup::ClearChildren(void)
{
	int i;

	((LevelChunkObjectRuntimeStateView*) this)->ResetRuntimeStateThunk();
	for (i = 0; i < 10; ++i) {
		m_apChildren128[i] = 0;
	}
	m_nChildCount124 = 0;
}
