#include "AI/CHandManager.h"

#include "AI/AICoord.h"
#include "AI/CGameObject.h"
#include "AI/CHand.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"

extern void* g_pActiveNetworkRuntimeWindow;
extern int g_cbEffTransportMaxPacketBytes;

struct VsNetEffStreamCommon {
	virtual ~VsNetEffStreamCommon(void);
};

struct HandManagerDeletableChild {
	virtual void Delete(unsigned char fDelete);
};

typedef void(LEMBALL_FASTCALL* HandObjectProc)(void* pObject);
typedef void(LEMBALL_FASTCALL* HandViewDataProc)(CGameObject* pObject, void* pUnused, CViewData* pViewData);

struct HandLevelFormatView {
	unsigned char m_abReserved00[0x54];
	unsigned short m_nVersion54;
};

// FUNCTION: LEMBALL 0x00427e60
CHandManager::CHandManager(CAI* pAI, int nCapacity)
{
	*(void**) this = g_GAME_EffStreamConstructionVtable;
	*(int*) ((char*) this + 4) = 0x16;
	((GameEffStream*) this)->ResetStateFields();
	*(int*) ((char*) this + 0x2c) = 0x0b;
	*(void**) this = g_LEVEL_EffChunkStreamBaseVtable;
	if (g_pActiveNetworkRuntimeWindow != 0) {
		*(int*) ((char*) this + 0x24) = 1;
		*(int*) ((char*) this + 0x18) += g_cbEffTransportMaxPacketBytes;
	}
	*(void**) this = &g_LINKSCF_CaptureChunkManagerVtable;
	m_pAI3C = pAI;
	m_nCapacity30 = nCapacity;
	m_pObjects38 = 0;
}

// FUNCTION: LEMBALL 0x00427ec0
void CHandManager::Restart(void)
{
	char* pObject;
	int i;
	int nOffset;

	if (m_pObjects38 != 0) {
		i = 0;
		if (m_nCapacity30 > 0) {
			nOffset = 0;
			do {
				pObject = (char*) m_pObjects38 + nOffset;
				++i;
				nOffset += sizeof(CHand);
				((HandObjectProc) (*(void***) pObject)[65])(pObject);
			} while (m_nCapacity30 > i);
		}
	}
}

// FUNCTION: LEMBALL 0x00427ef0
void CHandManager::Initialise(int nCapacity)
{
	CHand* pObject;
	int i;

	m_nCapacity30 = nCapacity;
	m_nObjectCount34 = 0;
	if (nCapacity == 0) {
		m_pObjects38 = 0;
		return;
	}
	if (m_pObjects38 == 0) {
		m_pObjects38 = new CHand[nCapacity];
		i = 0;
		if (m_nCapacity30 > 0) {
			do {
				pObject = &m_pObjects38[i];
				((HandObjectProc) ((void**) pObject->m_pVtable00)[65])(pObject);
				pObject->m_pOwningChunkStream60 = this;
				++i;
			} while (i < m_nCapacity30);
		}
	}
}

// FUNCTION: LEMBALL 0x00427fa0
CHandManager::~CHandManager(void)
{
	*(void**) this = &g_LINKSCF_CaptureChunkManagerVtable;
	if (m_pObjects38 != 0) {
		((HandManagerDeletableChild*) m_pObjects38)->Delete(3);
	}
	((VsNetEffStreamCommon*) this)->VsNetEffStreamCommon::~VsNetEffStreamCommon();
}

// FUNCTION: LEMBALL 0x00428360
int CHandManager::StepOn(const AICOORD& position, CGameObject* pObject)
{
	int i;

	i = 0;
	while (i < m_nObjectCount34) {
		CHand* pHand = &m_pObjects38[i];
		if (pHand->m_fConfigured13C != 0 && pHand->m_fActivated138 == 0 && pHand->m_nPendingState114 == 0 &&
			pHand->StepOn(position, pObject) != 0) {
			return 1;
		}
		++i;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x004283c0
void CHandManager::Process(void)
{
	CHand* pHand;
	int i;

	i = 0;
	if (m_nObjectCount34 > 0) {
		do {
			pHand = &m_pObjects38[i];
			pHand->m_fProcessFrame124 = 1;
			if (pHand->m_fActivated138 != 0 || pHand->m_nPendingState114 != 0) {
				((HandObjectProc) ((void**) pHand->m_pVtable00)[5])(pHand);
			}
			++i;
		} while (i < m_nObjectCount34);
	}
}

// FUNCTION: LEMBALL 0x00428410
int CHandManager::GetViewData(CViewData* pViewData)
{
	CHand* pHand;
	int cViewData;
	int i;

	i = 0;
	cViewData = 0;
	if (m_nObjectCount34 > 0) {
		do {
			pHand = &m_pObjects38[i];
			((HandViewDataProc) ((void**) pHand->m_pVtable00)[3])(pHand, 0, pViewData);
			++cViewData;
			pViewData = (CViewData*) ((char*) pViewData + 0x4c);
			++i;
		} while (i < m_nObjectCount34);
	}
	return cViewData;
}

// FUNCTION: LEMBALL 0x00428460
void CHandManager::Add(unsigned short nSlotId, int x, int y, int z)
{
	AICOORD position;

	if (m_nObjectCount34 < m_nCapacity30) {
		position.x = x << 12;
		position.y = y << 12;
		position.z = z << 12;
		m_pObjects38[m_nObjectCount34].Set(nSlotId, position);
		++m_nObjectCount34;
	}
}

// FUNCTION: LEMBALL 0x004284c0
void CHandManager::LoadLevel(unsigned char* pLevelData, int, unsigned char)
{
	unsigned short* pData = (unsigned short*) pLevelData;
	unsigned short nObjectCount;
	unsigned short nSlotId;

	nObjectCount = *pData++;
	Initialise(nObjectCount);
	while (nObjectCount != 0) {
		if (((HandLevelFormatView*) m_pAI3C)->m_nVersion54 < 2) {
			nSlotId = CGameObject::NextLoadingId();
		}
		else {
			nSlotId = *pData++;
		}
		Add(nSlotId, pData[0], pData[1], pData[2]);
		pData += 3;
		--nObjectCount;
	}
}
