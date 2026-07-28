#include "ENGINE/CORE/VSINIT.H"

typedef void*(LEMBALL_FASTCALL* LevelChildIteratorProc)(void* pIterator);
typedef void(LEMBALL_FASTCALL* ActivateLevelChildProc)(void* pChild);

struct LevelChildIteratorVtable {
	void* m_pReservedSlot0;
	LevelChildIteratorProc m_pGetFirstChild;
	LevelChildIteratorProc m_pGetNextChild;
};

struct LevelChildIteratorView {
	LevelChildIteratorVtable* m_pVtable;
};

struct LevelIteratedChildVtable {
	void* m_apReservedSlots[65];
	ActivateLevelChildProc m_pActivateChild;
};

struct LevelIteratedChildView {
	LevelIteratedChildVtable* m_pVtable;
};

// Split from the original LEVELRUN source group to preserve MSVC 4.20 code generation in LEVELRUN.CPP.

// FUNCTION: LEMBALL 0x0041e970
void LEMBALL_FASTCALL ActivateIteratedLevelChunkChildren(void* pIterator)
{
	LevelChildIteratorVtable* pVtable;
	LevelIteratedChildView* pChild;
	LevelChildIteratorProc pGetNextChild;

	pVtable = ((LevelChildIteratorView*) pIterator)->m_pVtable;
	pChild = (LevelIteratedChildView*) pVtable->m_pGetFirstChild(pIterator);
	if (pChild != NULL) {
		pGetNextChild = pVtable->m_pGetNextChild;
		do {
			pChild->m_pVtable->m_pActivateChild(pChild);
			pChild = (LevelIteratedChildView*) pGetNextChild(pIterator);
		} while (pChild != NULL);
	}
}
