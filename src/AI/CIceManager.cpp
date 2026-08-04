#include "AI/CIceManager.h"

#include "AI/CIce.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"

extern void* g_pActiveNetworkRuntimeWindow;
extern int g_cbEffTransportMaxPacketBytes;
extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);

struct VsNetEffStreamCommon {
	virtual ~VsNetEffStreamCommon(void);
};

struct IceManagerDeletableChild {
	virtual void Delete(unsigned char fDelete);
};

typedef void(LEMBALL_FASTCALL* IceObjectProc)(void* pObject);

// FUNCTION: LEMBALL 0x0042d7a0
CIceManager::CIceManager(CAI* pAI, int nCapacity)
{
	*(void**) this = g_GAME_EffStreamConstructionVtable;
	*(int*) ((char*) this + 4) = 0x19;
	((GameEffStream*) this)->ResetStateFields();
	*(int*) ((char*) this + 0x2c) = 0x0e;
	*(void**) this = g_LEVEL_EffChunkStreamBaseVtable;
	if (g_pActiveNetworkRuntimeWindow != 0) {
		*(int*) ((char*) this + 0x24) = 1;
		*(int*) ((char*) this + 0x18) += g_cbEffTransportMaxPacketBytes;
	}
	*(void**) this = &g_LINKSCF_IceChunkManagerVtable;
	m_pAI3C = pAI;
	m_nCapacity30 = nCapacity;
	m_pObjects38 = 0;
}

// FUNCTION: LEMBALL 0x0042d800
void CIceManager::Restart(void)
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
				nOffset += sizeof(CIce);
				((IceObjectProc) (*(void***) pObject)[65])(pObject);
			} while (m_nCapacity30 > i);
		}
	}
}

// FUNCTION: LEMBALL 0x0042d8e0
CIceManager::~CIceManager(void)
{
	*(void**) this = &g_LINKSCF_IceChunkManagerVtable;
	if (m_pObjects38 != 0) {
		((IceManagerDeletableChild*) m_pObjects38)->Delete(3);
	}
	((VsNetEffStreamCommon*) this)->VsNetEffStreamCommon::~VsNetEffStreamCommon();
}

// FUNCTION: LEMBALL 0x0042dd00
void CIceManager::Process(void)
{
	char* pObject;
	int i;
	int nOffset;

	i = 0;
	if (m_nObjectCount34 > 0) {
		nOffset = 0;
		do {
			pObject = (char*) m_pObjects38 + nOffset;
			++i;
			nOffset += sizeof(CIce);
			((IceObjectProc) (*(void***) pObject)[5])(pObject);
		} while (m_nObjectCount34 > i);
	}
}

// FUNCTION: LEMBALL 0x0042dd30
int CIceManager::GetViewData(CViewData* pViewData)
{
	(void) pViewData;
	return 0;
}

// FUNCTION: LEMBALL 0x0042dd90
void CIceManager::Switch(int nAction, unsigned int nSlot)
{
	int iObject = 0;
	int nObjectOffset;

	if (m_nObjectCount34 > 0) {
		nObjectOffset = 0;
		while (GetManagedEntitySlotIdThunk((int) (unsigned long) ((char*) m_pObjects38 + nObjectOffset)) != nSlot) {
			nObjectOffset += sizeof(CIce);
			++iObject;
			if (m_nObjectCount34 <= iObject) {
				return;
			}
		}
		if (nAction == 5) {
			((CIce*) ((char*) m_pObjects38 + iObject * sizeof(CIce)))->Switch();
		}
	}
}
