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

// FUNCTION: LEMBALL 0x00419490
int __fastcall CPlayerLemmingGroupManager_GetViewData(void* pObject, int nUnused, int param_1)
{
	int local_4 = 0;
	if (*(int*) 0x4a011c != 0) {
		int iVar3 = 4;
		local_4 = 4;
		int i;
		for (i = 0; i < 4; i++) {
			void* pElem = *(void**) ((char*) pObject + 0x13c + i * 4);
			(*( void(**)(int)) (*(void***) pElem + 0xc / 4))(param_1);
			param_1 += 0x4c;
		}
	}
	int iVar2 = ((int(__fastcall*)(void*, int)) 0x401f64)(pObject, param_1);
	return iVar2 + local_4;
}
// FUNCTION: LEMBALL 0x00418ab0
void __fastcall CPlayerLemmingGroupManager_UseObject(void* pOwner, int nUnused, unsigned int param_1)
{
	void* this_00 = (void*) ((int(__fastcall*)(void*)) 0x4021df)(pOwner);
	if (this_00 != 0) {
		void* pvVar1 = (void*) (*(void**) (0x4a6510 + (param_1 & 0xffff) * 4));
		if (*(int*) ((char*) pvVar1 + 0x64) != 2) {
			((void(__fastcall*)(void*, unsigned int)) 0x401c62)(this_00, param_1);
			return;
		}
		if (*(int*) ((char*) pvVar1 + 0xb8) != 8) {
			void* this_01 = (void*) ((int(__fastcall*)(void*)) 0x4037ba)(pvVar1);
			if (this_01 != this_00) {
				((void(__fastcall*)(void*, int, void*)) 0x4014bf)(this_00, 0, 0);
			}
			((void(__fastcall*)(void*, int, void*)) 0x4014bf)(this_01, 1, pvVar1);
		}
	}
}
// FUNCTION: LEMBALL 0x00418b60
void __fastcall CPlayerLemmingGroupManager_PlayerGroupRequestFire(void* pObject, int nUnused, int param_1, int param_2)
{
	void* pEntity = (void*) ((int(__fastcall*)(void*)) 0x4021df)(pObject);
	if (pEntity != 0) {
		void* pChild = (void*) ((int(__fastcall*)(void*)) 0x40241e)(pEntity);
		while (pChild != 0) {
			((void(__fastcall*)(void*, int, int)) 0x401659)(pChild, param_1, param_2);
			pChild = (void*) ((int(__fastcall*)(void*)) 0x401816)(pEntity);
		}
	}
}
// FUNCTION: LEMBALL 0x00418b20
void __fastcall CPlayerLemmingGroupManager_ReformAlteredGroups(void* pThis, int nUnused, int* param_1)
{
	int* pi;
	pi = (int*) ((int(__fastcall*)(void*)) 0x401078)(pThis);
	while (pi != 0) {
		if (pi != param_1) {
			((void(__fastcall*)(void*, int)) (*(int*) *pi + 0x154))(pi, *(int*) 0x4a7834);
		}
		pi = (int*) ((int(__fastcall*)(void*)) 0x403549)(pThis);
	}
}
