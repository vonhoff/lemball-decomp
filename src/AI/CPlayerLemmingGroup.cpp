#include "AI/CPlayerLemmingGroup.h"

#include "AI/CPlayerLemming.h"
#include "AI/LevelManagedEntityStateIdView.h"
#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

extern int g_GAME_ManagedEntityRegistryTable[1000];
extern unsigned short g_GAME_ManagedEntityRegistryCount;

struct LevelManagedEntityTargetView {
	unsigned char m_abReserved00[0x8c];
	int m_fActiveTarget8C;
};

;

struct LevelManagedEntityDirtyStateView {
	int PollStreamDirtyFlagThunk(void);
};

// FUNCTION: LEMBALL 0x00414040
void CPlayerLemmingGroup::Restart(void)
{
	CGenericGroup::Restart();
	m_nRuntimeState160 = 0;
	m_nRuntimeState164 = 0;
	m_nRuntimeState168 = 0;
	m_pActiveTarget170 = 0;
}

// FUNCTION: LEMBALL 0x00414130
void CPlayerLemmingGroup::Delete(void)
{
	if (m_pActiveTarget170 != 0) {
		m_pActiveTarget170->m_fActiveTarget8C = 0;
	}
	m_pActiveTarget170 = 0;
}

// FUNCTION: LEMBALL 0x00414600
int CPlayerLemmingGroup::AddLemmingToGroup(CPlayerLemming* pLemming)
{
	AddElementToGroup((CGameObject*) pLemming);
	m_nRuntimeState164 = 1;
	pLemming->SetGroup(this);
	if ((this->*m_pVtable00->m_pGetNumberOfElements108)() == 0 && m_nRuntimeState168 == 1) {
		pLemming->SetGroupLeader(1);
		return 1;
	}
	pLemming->SetGroupLeader(0);
	return 1;
}

// FUNCTION: LEMBALL 0x00414810
void CPlayerLemmingGroup::SetPlayerControlled(int fPlayerControlled, CPlayerLemming* pLeader)
{
	int fControlled = fPlayerControlled;
	CPlayerLemming* pLemming = (CPlayerLemming*) GetFirstElementInGroup();
	CPlayerLemming* pFirst = pLemming;

	while (pLemming != 0) {
		pLemming->SetGroup(fControlled);
		pLemming->SetGroupLeader(0);
		pLemming = (CPlayerLemming*) GetNextElementInGroup();
	}
	if (pLeader == 0) {
		pLeader = pFirst;
	}
	m_nRuntimeState168 = fControlled;
	if (pLeader != 0) {
		pLeader->SetGroupLeader(1);
		if (pLeader != pFirst) {
			SwapElements(pLeader, pFirst);
		}
	}
}

// FUNCTION: LEMBALL 0x00414880
int CPlayerLemmingGroup::CheckPlayerControlled(void)
{
	return m_nRuntimeState168;
}

// FUNCTION: LEMBALL 0x00414890
CPlayerLemming* CPlayerLemmingGroup::GetFirstDeadLemming(void)
{
	CGameObject* pObject = (this->*m_pVtable00->m_pGetFirstElement114)();
	while (pObject != 0) {
		if (((LevelManagedEntityStateIdView*) pObject)->m_nStateIdB8 == 8) {
			return (CPlayerLemming*) pObject;
		}
		pObject = (this->*m_pVtable00->m_pGetNextElement118)();
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00414960
int CPlayerLemmingGroup::HasSFXChanged(void)
{
	int fDirty = 0;
	CGameObject* pObject = GetFirstElementInGroup();
	while (pObject != 0) {
		fDirty = ((LevelManagedEntityDirtyStateView*) pObject)->PollStreamDirtyFlagThunk() || fDirty;
		pObject = GetNextElementInGroup();
	}
	return fDirty;
}

// FUNCTION: LEMBALL 0x004147d0
int CPlayerLemmingGroup::RemoveLemmingFromGroup(CPlayerLemming* pLemming)
{
	void* pChild;
	((void(__fastcall*)(void*, int)) 0x402879)(this, (int) pLemming);
	pChild = (void*) ((int(__fastcall*)(void*)) 0x40241e)(this);
	if (m_nRuntimeState168 == 1 && pChild != 0) {
		((void(__fastcall*)(void*, int)) 0x402667)(pChild, 1);
	}
	m_nRuntimeState164 = 1;
	return 1;
}
// FUNCTION: LEMBALL 0x00414080
int CPlayerLemmingGroup::GetViewData(int pViewData)
{
	int nVtbl = (int) m_pVtable00;
	int nCount = 0;
	int* pi = (int*) ((int(__fastcall*)(void*))(*(int*) (nVtbl + 0x114)))(this);
	if (pi != 0) {
		void* pGetNext = (void*) *(int*) (nVtbl + 0x118);
		do {
			int nOut = pViewData;
			if (pi[0x2e] != 0xc) {
				nOut = pViewData + 0x4c;
				nCount++;
				((void(__fastcall*)(void*, int))(*(int*) *pi + 0xc))(pi, pViewData);
			}
			pi = (int*) ((int(__fastcall*)(void*)) pGetNext)(this);
			pViewData = nOut;
		} while (pi != 0);
	}
	return nCount;
}

// FUNCTION: LEMBALL 0x00414660
void CPlayerLemmingGroup::AddUseObject(int nId)
{
	AICOORD destination;
	CGameObject* pObject;
	unsigned short i;

	pObject = 0;
	i = 0;
	while (i < g_GAME_ManagedEntityRegistryCount) {
		pObject = (CGameObject*) g_GAME_ManagedEntityRegistryTable[i];
		if (pObject != 0 && (unsigned int) pObject->m_nRegistryIndex6A == (unsigned int) nId) {
			break;
		}
		++i;
	}

	((AICOORD * (__fastcall*) (void*, int, AICOORD*) )((void**) pObject->m_pVtable00)[0x2c / 4])(
		pObject, 0, &destination);
	if (m_pCommandQueue70->m_cEntries < m_pCommandQueue70->m_cCapacity) {
		CGameObjectCommand* pCommand =
			&m_pCommandQueue70->m_pEntries[m_pCommandQueue70->m_cEntries++];
		pCommand->m_nType = 1;
		pCommand->m_Position = destination;
	}
	if (m_pCommandQueue70->m_cEntries < m_pCommandQueue70->m_cCapacity) {
		CGameObjectCommand* pCommand =
			&m_pCommandQueue70->m_pEntries[m_pCommandQueue70->m_cEntries++];
		pCommand->m_nType = 2;
		pCommand->m_Position = destination;
		pCommand->m_nFlags = (unsigned short) nId;
	}
}

// FUNCTION: LEMBALL 0x00414730
void CPlayerLemmingGroup::AddUseObject(CGameObject* pObject, int nId)
{
	AICOORD dest;
	((AICOORD * (__fastcall*) (void*, int, AICOORD*) )((void**) pObject->m_pVtable00)[0x2c / 4])(pObject, 0, &dest);
	if (m_pCommandQueue70->m_cEntries < m_pCommandQueue70->m_cCapacity) {
		CGameObjectCommand* pCmd1 = &m_pCommandQueue70->m_pEntries[m_pCommandQueue70->m_cEntries++];
		pCmd1->m_nType = 1;
		pCmd1->m_Position = dest;
	}
	if (m_pCommandQueue70->m_cEntries < m_pCommandQueue70->m_cCapacity) {
		CGameObjectCommand* pCmd2 = &m_pCommandQueue70->m_pEntries[m_pCommandQueue70->m_cEntries++];
		pCmd2->m_nType = 2;
		pCmd2->m_Position = dest;
		pCmd2->m_nFlags = (unsigned short) nId;
	}
}
