#define LEMBALL_CBALLOON_HARDTAIL_METHODS
#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "AI/CCatapult.h"
#include "AI/CAmmo.h"
#include "AI/CCrate.h"
#include "AI/CBalloon.h"

void LEMBALL_FASTCALL ResetManagedEntityRuntimeStateThunk(void* pObject);
extern int g_nLevelFrameClockTimeMs;
extern void* g_pSessionRandomState;
extern int g_nLevelFrameClockTick;
extern void* g_pActiveManagedEntityOwner;

struct ManagedEntityStateView {
	void RequestManagedEntityStateId(int nState);
};

struct BalloonTargetVtable;
struct BalloonTargetView {
	BalloonTargetVtable* m_pVtable00;
	int TypeTest(int nType);
};
typedef int (BalloonTargetView::*BalloonTargetTypeTestProc)(int);
typedef void (BalloonTargetView::*BalloonTargetActivateProc)(int, CBalloon*);
struct BalloonTargetVtable {
	void* m_apReserved00[45];
	BalloonTargetTypeTestProc m_pTypeTestB4;
	BalloonTargetActivateProc m_pActivateB8;
};
typedef char BalloonTargetMemberPointerMustBeOneWord
	[sizeof(BalloonTargetTypeTestProc) == sizeof(void*) ? 1 : -1];

struct BalloonSelfVtable;
struct BalloonSelfView {
	BalloonSelfVtable* m_pVtable00;
	void Action(int nAction);
};
typedef void (BalloonSelfView::*BalloonSelfActionProc)(int);
struct BalloonSelfVtable {
	void* m_apReserved00[13];
	BalloonSelfActionProc m_pAction34;
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

// FUNCTION: LEMBALL 0x0041d600
void CBalloon::Restart(void)
{
	int nRandom;

	((SavedPositionChunkObjectView*) this)->ResetToSavedPosition();
	nRandom = (*(int*) g_pSessionRandomState * 0x29 + 0x1f) & 0x7fffff;
	*(int*) g_pSessionRandomState = nRandom;
	m_nFrameTime94 = g_nLevelFrameClockTimeMs - nRandom % 0x1000;
}

// FUNCTION: LEMBALL 0x0041d740
int CBalloon::Activate(void* pTarget)
{
	BalloonTargetView* pObject;

	pObject = (BalloonTargetView*) pTarget;
	m_pTarget5C = pTarget;
	if ((pObject->*pObject->m_pVtable00->m_pTypeTestB4)(m_nObjectType64) == 0) {
		((ManagedEntityStateView*) this)->RequestManagedEntityStateId(0x1a);
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041d780
void CBalloon::DoActivate(void)
{
	BalloonTargetView* pObject;
	BalloonSelfView* pSelf;

	pObject = (BalloonTargetView*) m_pTarget5C;
	(pObject->*pObject->m_pVtable00->m_pActivateB8)(m_nObjectType64, this);
	pSelf = (BalloonSelfView*) this;
	(pSelf->*pSelf->m_pVtable00->m_pAction34)(0x2a);
	((LevelVtSmallFunctionView*) g_pActiveManagedEntityOwner)->AddLevelScoreClamped(50);
}

struct BalloonProcessActionView {
	virtual void ReservedSlot0(int nValue);
	virtual void ReservedSlot1(int nValue);
	virtual void SetManagedEntityStateId(int nStateId);
	virtual void ReservedSlot3(int nValue);
	virtual void ReservedSlot4(int nValue);
	virtual void ReservedSlot5(int nValue);
	virtual void ReservedSlot6(int nValue);
	virtual void ReservedSlot7(int nValue);
	virtual void ReservedSlot8(int nValue);
	virtual void ReservedSlot9(int nValue);
	virtual void ReservedSlot10(int nValue);
	virtual void ReservedSlot11(int nValue);
	virtual void ReservedSlot12(int nValue);
	virtual void SetActionState(int nStateId);
};

extern void* g_pLevelTileGrid;

// FUNCTION: LEMBALL 0x0041d650
int CBalloon::Process(void)
{
	char* pObjectBytes;
	char* pGrid;
	int nLocalY;
	int nLocalX;
	int nTileX;
	int nTileY;
	unsigned short nHeightOffset;
	int nStateId;

	pObjectBytes = (char*) this;
	pGrid = (char*) g_pLevelTileGrid;
	nLocalY = *(int*) (pObjectBytes + 0xa0) >> 12;
	nLocalX = *(int*) (pObjectBytes + 0x9c) >> 12;
	nTileX = nLocalX >> 4;
	nTileY = nLocalY >> 4;
	if (nLocalX < 0 || nLocalY < 0 || nTileX >= *(int*) (pGrid + 0x10) || nTileY >= *(int*) (pGrid + 0x14)) {
		nHeightOffset = 0;
	}
	else {
		void* pTile;

		pTile = *(char**) (pGrid + 0x0c) + (nTileY * *(int*) (pGrid + 0x10) + nTileX) * 12;
		nHeightOffset = ((unsigned short(__fastcall*)(void*, int, int, int)) 0x4029a5)(
			pTile, 0, nLocalX & 15, nLocalY & 15);
	}
	*(int*) (pObjectBytes + 0xa4) = nHeightOffset << 12;
	nStateId = *(int*) (pObjectBytes + 0xb8);
	if (*(int*) (pObjectBytes + 0x114) != 0) {
		if (*(int*) (pObjectBytes + 0x128) != nStateId) {
			if (nStateId == 0x1a) {
				((BalloonProcessActionView*) this)->SetActionState(0x2a);
			}
			*(int*) (pObjectBytes + 0x128) = *(int*) (pObjectBytes + 0xb8);
		}
		return 1;
	}
	if (nStateId == 0x1a) {
		((BalloonProcessActionView*) this)->SetManagedEntityStateId(0x18);
		*(int*) (pObjectBytes + 0x38) = 0;
		return 1;
	}
	return 1;
}
