#include "AI/CPlayerLemmingGroupManager.h"

#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "Visos/Generic/Memory.h"

extern int g_nSelectedNetworkLobbyPeerId;
extern CFormationManager* g_pGenericGroupFormationManager;
extern int g_GAME_ManagedEntityRegistryTable[1000];

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

// FUNCTION: LEMBALL 0x00418650
void CPlayerLemmingGroupManager::Process(void)
{
	typedef int(__fastcall * GetCountProc)(void* pGroup);
	CPlayerLemmingGroupManager* pManager;
	CPlayerLemmingGroup* pGroup;
	CPlayerLemming* pLemming;
	GetCountProc pGetCount;
	int fSelectedGroupRemoved;

	pManager = (CPlayerLemmingGroupManager*) ((char*) this - 0xb0);
	fSelectedGroupRemoved = 0;
	pGroup = (CPlayerLemmingGroup*) pManager->GetFirstGroup();
	while (pGroup != 0) {
		((void(__fastcall*)(void*))((void**) pGroup->m_pVtable00)[0x14 / 4])(pGroup);
		pGroup = (CPlayerLemmingGroup*) pManager->GetNextGroup();
	}

	pGroup = (CPlayerLemmingGroup*) pManager->GetFirstGroup();
	while (pGroup != 0) {
		pLemming = pGroup->GetFirstDeadLemming();
		while (pLemming != 0) {
			pGetCount = (GetCountProc) ((void**) pGroup->m_pVtable00)[0x108 / 4];
			pGroup->RemoveLemmingFromGroup(pLemming);
			*(CPlayerLemming**) ((char*) this + 0x7c + *(int*) ((char*) this + 0x78) * sizeof(CPlayerLemming*)) =
				pLemming;
			++*(int*) ((char*) this + 0x78);
			if (pGetCount(pGroup) == 0) {
				if (pManager->GetPlayerControlledGroup() == pGroup) {
					fSelectedGroupRemoved = 1;
				}
				((void(__fastcall*)(void*, int, void*)) 0x40281a)(pManager, 0, pGroup);
				pLemming = 0;
			}
			else {
				pLemming = pGroup->GetFirstDeadLemming();
			}
		}
		pGroup = (CPlayerLemmingGroup*) pManager->GetNextGroup();
	}
	if (fSelectedGroupRemoved != 0) {
		pManager->MakePreviousGroupPlayerControlled();
	}
	pManager->ProcessDead();
}

// FUNCTION: LEMBALL 0x00418730
void CPlayerLemmingGroupManager::CreateNewGroup(unsigned short cLemmings, unsigned short* pLemmingIds)
{
	CPlayerLemmingGroup* pGroup;
	CPlayerLemming* pLemming;
	int iGroup;
	int cAdded;
	unsigned short i;

	ProcessDead();
	pGroup = 0;
	iGroup = 0;
	if (m_nGroupCountA4 > 0) {
		do {
			CPlayerLemmingGroup* pCandidate = (CPlayerLemmingGroup*) m_apGroups04[iGroup];
			if (((int(__fastcall*)(void*))((void**) pCandidate->m_pVtable00)[0x108 / 4])(pCandidate) == 0) {
				pGroup = (CPlayerLemmingGroup*) ((void*(__fastcall*) (void*, int, int) ) 0x402eb9)(this, 0, iGroup);
				break;
			}
			++iGroup;
		} while (iGroup < m_nGroupCountA4);
	}

	cAdded = 0;
	for (i = 0; i < cLemmings; ++i) {
		pLemming = (CPlayerLemming*) (int) g_GAME_ManagedEntityRegistryTable[pLemmingIds[i]];
		if (((int(__fastcall*)(void*))((void**) ((CGameObject*) pLemming)->m_pVtable00)[0x70 / 4])(pLemming) != 0) {
			((void(__fastcall*)(void*)) 0x402446)(pLemming);
			((void(__fastcall*)(void*, int, void*, void*)) 0x4024b4)(this, 0, pLemming, pGroup);
			((void(__fastcall*)(void*)) 0x4011ef)(pLemming);
			++cAdded;
		}
	}
	if (cAdded > 0) {
		((void(__fastcall*)(void*, int, void*)) 0x4011f9)(this, 0, pGroup);
		((void(__fastcall*)(void*, int, void*)) 0x40184d)(this, 0, pGroup);
	}
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
			((RestartProc) ((void**) ((CGameObject*) pLemming)->m_pVtable00)[0x104 / sizeof(void*)])(pLemming);
			pChunkStream = this == 0 ? 0 : m_abReservedB0;
			((CGameObject*) pLemming)->m_pOwningChunkStream60 = pChunkStream;
		}
		m_fNetworkInitialised14C = 1;
	}
}

// FUNCTION: LEMBALL 0x00419060
void CPlayerLemmingGroupManager::LoadAdditionalPlayerStartPositions(unsigned short* pData,
																	int nUnused,
																	int fReuseNetworkObjects)
{
	typedef void(LEMBALL_FASTCALL * RestartProc)(void* pLemming);
	char* pLevel;
	char* pMode;
	unsigned short* pSource;
	int* pStart;
	CPlayerLemming** ppReuse;
	CPlayerLemming* pLemming;
	void* pGroup;
	void* pStorage;
	int nTotal;
	int nRemaining;
	int nDelay;
	int nCount;
	int nListIndex;
	int nListCount;
	int* pList;
	int nHeight;
	int i;
	int j;

	(void) nUnused;
	pLevel = *(char**) 0x004a74b4;
	pMode = *(char**) 0x004a782c;
	m_nReserved124 = *pData++;
	nTotal = 0;
	i = 0;
	if (m_nReserved124 > 0) {
		pStart = (int*) ((char*) this + 0xe0);
		pSource = pData;
		do {
			pStart[0] = pSource[0];
			pStart[4] = pSource[1];
			pStart[8] = pSource[2];
			if (pStart[0] < 0 || pStart[4] < 0 || *(int*) (pLevel + 0x10) <= pStart[0] >> 4 ||
				*(int*) (pLevel + 0x14) <= pStart[4] >> 4) {
				nHeight = 0;
			}
			else {
				nHeight = ((unsigned short(LEMBALL_FASTCALL*)(void*, int, int, int)) 0x004029a5)(
					*(char**) (pLevel + 0x0c) + ((pStart[4] >> 4) * *(int*) (pLevel + 0x10) + (pStart[0] >> 4)) * 0x0c,
					0,
					pStart[0] & 0x0f,
					pStart[4] & 0x0f);
			}
			pStart[8] = (unsigned short) nHeight;
			pStart[12] = pSource[3];
			nTotal += pSource[3];
			++i;
			++pStart;
			pSource += 4;
		} while (i < m_nReserved124);
	}

	((void(LEMBALL_FASTCALL*)(void*, int, int)) 0x004011fe)(pMode, 0, nTotal);
	nRemaining = 4 - nTotal;
	ppReuse = fReuseNetworkObjects != 0 ? (CPlayerLemming**) (pMode + 0x1d0) : 0;
	m_nDeadCount128 = 0;
	pStart = (int*) ((char*) this + 0x100);
	while (nRemaining > 0) {
		if (ppReuse == 0) {
			pStorage = AllocateVSMemBlock(0x22c);
			pLemming =
				pStorage == 0
					? 0
					: (CPlayerLemming*)
						  ConstructPlasChunkObjectForLevelThunk(pStorage, 0, pStart[-8], pStart[-4], *pStart, 0, 0, 0);
		}
		else {
			pLemming = *ppReuse++;
		}
		((RestartProc) ((void**) ((CGameObject*) pLemming)->m_pVtable00)[0x104 / 4])(pLemming);
		*(int*) ((char*) pLemming + 0xb8) = 8;
		nListCount = *(int*) (pMode + 0x118);
		nListIndex = 0;
		if (nListCount > 0) {
			pList = *(int**) (pMode + 0x120);
			while (nListIndex < nListCount && pList[nListIndex] != (int) pLemming) {
				++nListIndex;
			}
			if (nListIndex < nListCount) {
				--*(int*) (pMode + 0x118);
				while (nListIndex < *(int*) (pMode + 0x118)) {
					pList[nListIndex] = pList[nListIndex + 1];
					++nListIndex;
				}
				pList[*(int*) (pMode + 0x118)] = 0;
			}
		}
		--*(int*) 0x004a640c;
		((CGameObject*) pLemming)->m_pOwningChunkStream60 = this == 0 ? 0 : m_abReservedB0;
		m_apDeadLemmings12C[m_nDeadCount128++] = pLemming;
		++pStart;
		--nRemaining;
	}

	i = 0;
	pStart = (int*) ((char*) this + 0x110);
	while (i < m_nReserved124) {
		nDelay = 0xf3c;
		j = 0;
		while (j < *pStart) {
			if (ppReuse == 0) {
				pStorage = AllocateVSMemBlock(0x22c);
				pLemming = pStorage == 0 ? 0
										 : (CPlayerLemming*) ConstructPlasChunkObjectForLevelThunk(pStorage,
																								   0,
																								   pStart[-12],
																								   pStart[-8],
																								   pStart[-4],
																								   0,
																								   0,
																								   nDelay / 0x32);
			}
			else {
				pLemming = *ppReuse++;
			}
			((RestartProc) ((void**) ((CGameObject*) pLemming)->m_pVtable00)[0x104 / 4])(pLemming);
			((CGameObject*) pLemming)->m_pOwningChunkStream60 = this == 0 ? 0 : m_abReservedB0;
			pGroup = ((void*(LEMBALL_FASTCALL*) (void*, int, int) ) 0x00402eb9)(this, 0, i);
			((void(LEMBALL_FASTCALL*)(void*, int, void*, void*)) 0x004024b4)(this, 0, pLemming, pGroup);
			nDelay += 800;
			++j;
		}
		nCount = (*pStart * 800 + 0xf3c) / 0x32;
		if (*(int*) (pMode + 0x5c) < nCount) {
			*(int*) (pMode + 0x5c) = nCount;
		}
		if (fReuseNetworkObjects == 0) {
			((void(LEMBALL_FASTCALL*)(void*, int, int, int, int, int)) 0x004033b9)(pMode,
																				   0,
																				   pStart[-12],
																				   pStart[-8],
																				   pStart[-4],
																				   (*pStart * 800 + 0x1004) / 0x32);
		}
		++pStart;
		++i;
	}
	pLemming = (CPlayerLemming*) ((void*(LEMBALL_FASTCALL*) (void*, int) ) 0x00401078)(this, 0);
	((void(LEMBALL_FASTCALL*)(void*, int, void*)) 0x004011f9)(this, 0, pLemming);
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
int CPlayerLemmingGroupManager::GetViewData(int pViewData)
{
	int nViewDataCount = 0;
	if (g_nSelectedNetworkLobbyPeerId != 0) {
		int nNetworkLemmingCount = 4;
		nViewDataCount = 4;
		int i;
		for (i = 0; i < 4; i++) {
			CPlayerLemming* pElem = m_apNetworkLemmings13C[i];
			((void (**)(int))(void**) ((CGameObject*) pElem)->m_pVtable00)[0xc / 4](pViewData);
			pViewData += 0x4c;
		}
	}
	int nGroupViewDataCount = ((int(__fastcall*)(void*, int)) 0x401f64)(this, pViewData);
	return nGroupViewDataCount + nViewDataCount;
}
// FUNCTION: LEMBALL 0x00418ab0
void CPlayerLemmingGroupManager::UseObject(unsigned int nObject)
{
	CPlayerLemmingGroup* pCurrentGroup = GetPlayerControlledGroup();
	if (pCurrentGroup != 0) {
		void* pObject = (void*) (*(void**) (0x4a6510 + (nObject & 0xffff) * 4));
		if (*(int*) ((char*) pObject + 0x64) != 2) {
			((void(__fastcall*)(void*, unsigned int)) 0x401c62)(pCurrentGroup, nObject);
			return;
		}
		if (*(int*) ((char*) pObject + 0xb8) != 8) {
			CPlayerLemmingGroup* pObjectGroup = (CPlayerLemmingGroup*) ((int(__fastcall*)(void*)) 0x4037ba)(pObject);
			if (pObjectGroup != pCurrentGroup) {
				pCurrentGroup->SetPlayerControlled(0, 0);
			}
			pObjectGroup->SetPlayerControlled(1, (CPlayerLemming*) pObject);
		}
	}
}
// FUNCTION: LEMBALL 0x00418b60
void CPlayerLemmingGroupManager::PlayerGroupRequestFire(int nX, int nY)
{
	CPlayerLemmingGroup* pGroup = GetPlayerControlledGroup();
	if (pGroup != 0) {
		CGameObject* pChild = pGroup->GetFirstElementInGroup();
		while (pChild != 0) {
			((void(__fastcall*)(void*, int, int)) 0x401659)(pChild, nX, nY);
			pChild = pGroup->GetNextElementInGroup();
		}
	}
}
// FUNCTION: LEMBALL 0x00418b20
void CPlayerLemmingGroupManager::ReformAlteredGroups(CPlayerLemmingGroup* pGroup)
{
	int* pi;
	pi = (int*) GetFirstGroup();
	while (pi != 0) {
		if (pi != (int*) pGroup) {
			((void(__fastcall*)(void*, int))(*(int*) *pi + 0x154))(pi, *(int*) 0x4a7834);
		}
		pi = (int*) GetNextGroup();
	}
}

// FUNCTION: LEMBALL 0x00418540
CPlayerLemmingGroupManager::~CPlayerLemmingGroupManager(void)
{
	void* pThis = this;
	void* pSub;
	void* p;
	int i;
	*(int*) pThis = 0x494038;
	pSub = ((char*) pThis) - 0xb0;
	*(int*) pSub = 0x494068;
	for (i = 0; i < *(int*) ((char*) pThis + 0x78); i++) {
		p = *(void**) ((char*) pThis + 0x7c + i * 4);
		if (p != 0) {
			((void(__fastcall*)(void*, int)) * (int*) p)(p, 1);
		}
	}
	if (*(int*) ((char*) pThis + 0x9c) != 0) {
		for (i = 0; i < 4; i++) {
			p = *(void**) ((char*) pThis + 0x8c + i * 4);
			if (p != 0) {
				((void(__fastcall*)(void*, int)) * (int*) p)(p, 1);
			}
		}
	}
	((void(__fastcall*)(void*)) 0x45eea0)(((unsigned int) pSub >= 1) ? pThis : 0);
	((void(__fastcall*)(void*)) 0x402de7)(pSub);
}
