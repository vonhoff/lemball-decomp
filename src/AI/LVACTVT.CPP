#include "Platform/Windows/Mixed/Engine/CORE/LINKSCF.H"

struct LevelManagedEntityChildIterator;
struct LevelManagedEntityActiveGroupView;
struct LevelManagedEntityGroupVtable;
struct LevelManagedEntityVtable;

struct LevelManagedEntityChildIterator {
	LevelManagedEntityVtable* m_pVtable00;
	int GetActiveStateThunk(void);
};

struct LevelManagedEntityActiveGroupView {
	LevelManagedEntityGroupVtable* m_pVtable00;

	int GetEntityActiveState(void* pObject);
};

typedef LevelManagedEntityChildIterator* (LevelManagedEntityActiveGroupView::*GetManagedEntityProc)(void);
typedef int (LevelManagedEntityChildIterator::*MatchesManagedEntityProc)(void* pObject);

struct LevelManagedEntityGroupVtable {
	void* m_pReserved00;
	GetManagedEntityProc m_pGetFirst04;
	GetManagedEntityProc m_pGetNext08;
};

struct LevelManagedEntityVtable {
	void* m_apReserved00[76];
	MatchesManagedEntityProc m_pMatchesObject130;
};

// MACINTOSH: CPlayerLemmingGroupManager::IsLemmingPlayerControlled(CPlayerLemming*)
// FUNCTION: LEMBALL 0x00418840
int LevelManagedEntityActiveGroupView::GetEntityActiveState(void* pObject)
{
	CGenericGroup* pGroup = ((CGenericGroupManager*) this)->GetGroupElementIsMemberOf((CGameObject*) pObject);
	return pGroup != 0 ? ((LevelManagedEntityChildIterator*) pGroup)->GetActiveStateThunk() : 0;
}
