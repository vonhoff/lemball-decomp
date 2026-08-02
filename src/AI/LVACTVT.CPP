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

	LevelManagedEntityChildIterator* FindManagedEntityByPointer(void* pObject);
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
	LevelManagedEntityChildIterator* pEntity = FindManagedEntityByPointer(pObject);
	return pEntity != 0 ? pEntity->GetActiveStateThunk() : 0;
}

// MACINTOSH: CGenericGroupManager::GetGroupElementIsMemberOf(CGameObject*)
// FUNCTION: LEMBALL 0x0041eb90
LevelManagedEntityChildIterator* LevelManagedEntityActiveGroupView::FindManagedEntityByPointer(void* pObject)
{
	LevelManagedEntityChildIterator* pEntity;
	LevelManagedEntityGroupVtable* pVtable = m_pVtable00;
	pEntity = (this->*pVtable->m_pGetFirst04)();
	while (pEntity != 0) {
		if ((pEntity->*pEntity->m_pVtable00->m_pMatchesObject130)(pObject) == 1) {
			return pEntity;
		}
		pEntity = (this->*pVtable->m_pGetNext08)();
	}
	return 0;
}
