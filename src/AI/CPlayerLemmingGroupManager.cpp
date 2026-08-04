#include "AI/CPlayerLemmingGroupManager.h"

#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "Visos/Generic/Memory.h"

extern int g_nSelectedNetworkLobbyPeerId;
extern CFormationManager* g_pGenericGroupFormationManager;

extern void* LEMBALL_FASTCALL ConstructPlasChunkObjectForLevelThunk(void* pObject,
																	int nUnusedEdx,
																	int nX,
																	int nY,
																	int nZ,
																	unsigned short nVariant,
																	int nNetworkPlayer,
																	int nStartDelay);

struct LevelManagedEntityChildIterator;

struct LevelManagedEntityChildIterator {
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
	pLemming = pGroup->GetFirstElementInGroup();
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

// FUNCTION: LEMBALL 0x00418820
void CPlayerLemmingGroupManager::AddPlayerLemmingToGroup(CPlayerLemming* pLemming, CPlayerLemmingGroup* pGroup)
{
	FindElementInGroupAndRemoveIt((CGameObject*) pLemming);
	pGroup->AddLemmingToGroup(pLemming);
}

// FUNCTION: LEMBALL 0x00418840
int CPlayerLemmingGroupManager::IsLemmingPlayerControlled(CPlayerLemming* pLemming)
{
	CGenericGroup* pGroup = GetGroupElementIsMemberOf((CGameObject*) pLemming);
	return pGroup != 0 ? ((LevelManagedEntityChildIterator*) pGroup)->GetActiveStateThunk() : 0;
}

// FUNCTION: LEMBALL 0x00418860
int CPlayerLemmingGroupManager::MakeNextGroupPlayerControlled(void)
{
	MakeNoGroupsPlayerControlled();
	for (int i = 0; i < m_nGroupCountA4; ++i) {
		++m_nCurrentPlayerGroup120;
		if (m_nCurrentPlayerGroup120 == m_nGroupCountA4) {
			m_nCurrentPlayerGroup120 = 0;
		}
		if (m_apGroups04[m_nCurrentPlayerGroup120] != 0 &&
			((CPlayerLemmingGroup*) m_apGroups04[m_nCurrentPlayerGroup120]
				 ->*((CPlayerLemmingGroup*) m_apGroups04[m_nCurrentPlayerGroup120])
				 ->m_pVtable00->m_pGetNumberOfElements108)() > 0) {
			((CPlayerLemmingGroup*) m_apGroups04[m_nCurrentPlayerGroup120])->SetPlayerControlled(1, 0);
			return 1;
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x004188e0
int CPlayerLemmingGroupManager::MakePreviousGroupPlayerControlled(void)
{
	MakeNoGroupsPlayerControlled();
	MakeNoGroupsPlayerControlled();
	for (int i = 0; i < m_nGroupCountA4; ++i) {
		--m_nCurrentPlayerGroup120;
		if (m_nCurrentPlayerGroup120 < 0) {
			m_nCurrentPlayerGroup120 += m_nGroupCountA4;
		}
		if (m_apGroups04[m_nCurrentPlayerGroup120] != 0 &&
			((CPlayerLemmingGroup*) m_apGroups04[m_nCurrentPlayerGroup120]
				 ->*((CPlayerLemmingGroup*) m_apGroups04[m_nCurrentPlayerGroup120])
				 ->m_pVtable00->m_pGetNumberOfElements108)() > 0) {
			((CPlayerLemmingGroup*) m_apGroups04[m_nCurrentPlayerGroup120])->SetPlayerControlled(1, 0);
			return 1;
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00418960
int CPlayerLemmingGroupManager::MakeParticularGroupPlayerControlled(CPlayerLemmingGroup* pGroup)
{
	MakeNoGroupsPlayerControlled();
	for (int i = 0; i < 8; ++i) {
		if (m_apGroups04[i] == pGroup) {
			m_nCurrentPlayerGroup120 = i;
			if ((pGroup->*pGroup->m_pVtable00->m_pGetNumberOfElements108)() == 0) {
				return MakeNextGroupPlayerControlled();
			}
			pGroup->SetPlayerControlled(1, 0);
			return 1;
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x004189c0
int CPlayerLemmingGroupManager::MakeNoGroupsPlayerControlled(void)
{
	for (int i = 0; i < 8; ++i) {
		CPlayerLemmingGroup* pGroup = (CPlayerLemmingGroup*) m_apGroups04[i];
		if (pGroup != 0) {
			pGroup->SetPlayerControlled(0, 0);
		}
	}
	return 1;
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

// FUNCTION: LEMBALL 0x00418a30
void CPlayerLemmingGroupManager::AddNewWaypointToCurrentGroup(int nX, int nY)
{
	AICOORD position;
	CPlayerLemmingGroup* pGroup;

	position.x = nX << 12;
	position.y = nY << 12;
	position.z = 0;
	pGroup = GetPlayerControlledGroup();
	if (pGroup != 0) {
		(pGroup->*(pGroup->m_pVtable00->m_pAddNewWaypoint144))(position, g_pGenericGroupFormationManager);
	}
}

// FUNCTION: LEMBALL 0x00418a90
void CPlayerLemmingGroupManager::RemoveWaypointsFromCurrentGroup(void)
{
	CPlayerLemmingGroup* pGroup = GetPlayerControlledGroup();

	if (pGroup != 0) {
		(pGroup->*(pGroup->m_pVtable00->m_pClearExistingWaypoints150))();
	}
}

// FUNCTION: LEMBALL 0x00418ca0
void CPlayerLemmingGroupManager::InitialiseNetwork(void)
{
	typedef void(LEMBALL_FASTCALL * RestartProc)(void* pLemming);
	CPlayerLemming* pLemming;
	void* pStorage;
	void* pChunkStream;
	int i;

	if (g_nSelectedNetworkLobbyPeerId != 0) {
		for (i = 0; i < 4; ++i) {
			if (m_fNetworkInitialised14C == 0) {
				pStorage = AllocateVSMemBlock(0x22c);
				if (pStorage == 0) {
					m_apNetworkLemmings13C[i] = 0;
				}
				else {
					m_apNetworkLemmings13C[i] =
						(CPlayerLemming*) ConstructPlasChunkObjectForLevelThunk(pStorage, 0, 0, 0, 0, 0, 1, 0);
				}
			}

			pLemming = m_apNetworkLemmings13C[i];
			((RestartProc) (*(void***) pLemming)[0x104 / sizeof(void*)])(pLemming);
			pChunkStream = this == 0 ? 0 : (char*) this + 0xb0;
			((CGameObject*) pLemming)->m_pOwningChunkStream60 = pChunkStream;
		}
		m_fNetworkInitialised14C = 1;
	}
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
