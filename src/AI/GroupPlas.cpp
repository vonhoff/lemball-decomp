#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

struct LevelPoint3 {
	int m_nX;
	int m_nY;
	int m_nZ;
};

struct LevelManagedEntityChildIterator {
	void* GetFirstChildThunk(void);
};

struct LevelManagedEntityChildView {
	char m_abReserved00[0x9c];
	LevelPoint3 m_position9C;
};

struct LevelManagedEntityActiveGroupView {
	LevelManagedEntityChildIterator* GetFirstActiveEntityThunk(void);
	int GetActiveEntityPosition(LevelPoint3* pPosition);
};

typedef int(__cdecl* PlasChildStatePredicate)(void* pContext, void* pEntity, int* pScratch);
typedef void(__cdecl* PlasChildStateAction)(void* pContext, void* pEntity, int* pScratch);

struct PlasChildStateDispatchEntry {
	PlasChildStatePredicate m_pPredicate;
	PlasChildStateAction m_pAction;
	int m_nNextState;
	int m_nStateVariant;
};

struct PlasChildStateEntityView {
	virtual void Reserved00(void);
	virtual void Reserved01(void);
	virtual void SetState(int nState);
	virtual void Reserved03(void);
	virtual void Reserved04(void);
	virtual void Reserved05(void);
	virtual void Reserved06(void);
	virtual void Reserved07(void);
	virtual void Reserved08(void);
	virtual void Reserved09(void);
	virtual void Reserved10(void);
	virtual void Reserved11(void);
	virtual void Reserved12(void);
	virtual void SetStateVariant(int nVariant);
	virtual int HasStateVariant(void);

	char m_abReserved04[0x90];
	int m_nStateDeadline94;
	char m_abReserved98[0x20];
	int m_nStateB8;
};

extern int g_nLevelFrameClockTick;
void LEMBALL_FASTCALL UpdateManagedEntityBoundsFromPosition(void* pObject);

// Split from LEVELVT.CPP to preserve compiler state in the original translation unit.

// FUNCTION: LEMBALL 0x004185f0
int LevelManagedEntityActiveGroupView::GetActiveEntityPosition(LevelPoint3* pPosition)
{
	LevelManagedEntityChildIterator* pEntity;
	LevelManagedEntityChildView* pChild;

	pEntity = GetFirstActiveEntityThunk();
	if (pEntity == NULL) {
		return 0;
	}
	pChild = (LevelManagedEntityChildView*) pEntity->GetFirstChildThunk();
	if (pChild == NULL) {
		return 0;
	}
	pPosition->m_nX = pChild->m_position9C.m_nX;
	pPosition->m_nY = pChild->m_position9C.m_nY;
	pPosition->m_nZ = pChild->m_position9C.m_nZ;
	return 1;
}

// FUNCTION: LEMBALL 0x00419980
void __cdecl DispatchPlasChildStateTable(PlasChildStateDispatchEntry** ppStateTable,
										 void* pContext,
										 PlasChildStateEntityView* pEntity)
{
	int nOriginalState;
	PlasChildStateDispatchEntry* pEntry;
	int nNextState;
	int nStateVariant;
	int nScratch;

	nOriginalState = pEntity->m_nStateB8;
	pEntry = ppStateTable[nOriginalState];
	while (pEntry->m_pPredicate != NULL && pEntry->m_pPredicate(pContext, pEntity, &nScratch) == 0) {
		++pEntry;
	}
	if (pEntry->m_pAction != NULL) {
		pEntry->m_pAction(pContext, pEntity, &nScratch);
	}
	nNextState = pEntry->m_nNextState;
	nStateVariant = pEntry->m_nStateVariant;
	if (pEntity->HasStateVariant() == 0) {
		pEntity->SetStateVariant(nStateVariant);
	}
	UpdateManagedEntityBoundsFromPosition(pEntity);
	if (nNextState != 0x28 && nNextState != nOriginalState && pEntity->m_nStateB8 == nOriginalState) {
		pEntity->m_nStateDeadline94 = g_nLevelFrameClockTick * 50;
		pEntity->SetState(nNextState);
	}
}
