#include "AI/CPlayerLemmingGroup.h"

#include "AI/CPlayerLemming.h"
#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

struct LevelManagedEntityTargetView {
	unsigned char m_abReserved00[0x8c];
	int m_fActiveTarget8C;
};

struct LevelManagedEntityStateIdView {
	char m_abReserved00[0xb8];
	int m_nStateIdB8;
};

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
	CGameObject* pObject = (this->*m_pVtable00->m_pGetFirstElement114)();
	while (pObject != 0) {
		fDirty = ((LevelManagedEntityDirtyStateView*) pObject)->PollStreamDirtyFlagThunk() || fDirty;
		pObject = (this->*m_pVtable00->m_pGetNextElement118)();
	}
	return fDirty;
}
