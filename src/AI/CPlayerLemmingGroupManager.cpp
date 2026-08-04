#include "AI/CPlayerLemmingGroupManager.h"

struct LevelManagedEntityChildIterator;

struct LevelManagedEntityChildIterator {
	void* GetFirstChildThunk(void);
	int GetActiveStateThunk(void);
};

// FUNCTION: LEMBALL 0x004185d0
CPlayerLemming* CPlayerLemmingGroupManager::GetDead(void)
{
	if (m_nDeadCount128 == 0) {
		return 0;
	}
	return m_apDeadLemmings12C[--m_nDeadCount128];
}

// FUNCTION: LEMBALL 0x004185f0
int CPlayerLemmingGroupManager::GetLeaderPos(AICOORD& position)
{
	CPlayerLemmingGroup* pGroup;
	CGameObject* pLemming;

	pGroup = GetPlayerControlledGroup();
	if (pGroup == 0) {
		return 0;
	}
	pLemming = (CGameObject*) ((LevelManagedEntityChildIterator*) pGroup)->GetFirstChildThunk();
	if (pLemming == 0) {
		return 0;
	}
	position.x = pLemming->m_WorldPosition9C.x;
	position.y = pLemming->m_WorldPosition9C.y;
	position.z = pLemming->m_WorldPosition9C.z;
	return 1;
}

// FUNCTION: LEMBALL 0x00418640
void CPlayerLemmingGroupManager::ProcessDead(void)
{
}

// FUNCTION: LEMBALL 0x00418840
int CPlayerLemmingGroupManager::IsLemmingPlayerControlled(CPlayerLemming* pLemming)
{
	CGenericGroup* pGroup = GetGroupElementIsMemberOf((CGameObject*) pLemming);
	return pGroup != 0 ? ((LevelManagedEntityChildIterator*) pGroup)->GetActiveStateThunk() : 0;
}

// FUNCTION: LEMBALL 0x004189f0
CPlayerLemmingGroup* CPlayerLemmingGroupManager::GetPlayerControlledGroup(void)
{
	int iGroup;
	CGenericGroup** ppGroup;
	CPlayerLemmingGroup* pGroup;

	iGroup = 0;
	if (m_nGroupCountA4 > 0) {
		ppGroup = m_apGroups04;
		do {
			pGroup = (CPlayerLemmingGroup*) *ppGroup;
			if (pGroup != 0 && ((LevelManagedEntityChildIterator*) pGroup)->GetActiveStateThunk() == 1) {
				return (CPlayerLemmingGroup*) m_apGroups04[iGroup];
			}
			++ppGroup;
			++iGroup;
		} while (iGroup < m_nGroupCountA4);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x004193f0
int CPlayerLemmingGroupManager::HasSFXChanged(void)
{
	int fDirty;
	int iGroup;
	CGenericGroup** ppGroup;
	CPlayerLemmingGroup* pGroup;

	fDirty = 0;
	iGroup = 0;
	if (m_nGroupCountA4 > 0) {
		ppGroup = m_apGroups04;
		do {
			pGroup = (CPlayerLemmingGroup*) *ppGroup;
			fDirty = pGroup->HasSFXChanged() || fDirty;
			++ppGroup;
			++iGroup;
		} while (iGroup < m_nGroupCountA4);
	}
	return fDirty;
}
