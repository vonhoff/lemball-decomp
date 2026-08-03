#include "AI/CInvisibleSwitchManager.h"

#include "AI/CInvisibleSwitch.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"

struct InvisibleSwitchManagerVtableLayout;
extern InvisibleSwitchManagerVtableLayout g_LINKSCF_InvsChunkManagerVtable;
extern void* g_pActiveNetworkRuntimeWindow;
extern int g_cbEffTransportMaxPacketBytes;

struct VsNetEffStreamCommon {
	virtual ~VsNetEffStreamCommon(void);
};

typedef void(LEMBALL_FASTCALL* InvsChunkObjectActivateProc)(void* pObject);

struct InvisibleSwitchManagerResetView {
	unsigned char m_abReserved00[0x30];
	int m_cCapacity30;
	int m_cObjects34;
	unsigned char m_abReserved38[4];
	void* m_pObjects3C;
};

struct InvisibleSwitchManagerDeletableChild {
	virtual void Delete(unsigned char fDelete);
};

// FUNCTION: LEMBALL 0x0040a210
CInvisibleSwitchManager::CInvisibleSwitchManager(CAI* pAI, int nCapacity)
{
	*(void**) this = g_GAME_EffStreamConstructionVtable;
	*(int*) ((char*) this + 4) = 0x20;
	((GameEffStream*) this)->ResetStateFields();
	*(int*) ((char*) this + 0x2c) = 0x15;
	*(void**) this = g_LEVEL_EffChunkStreamBaseVtable;
	if (g_pActiveNetworkRuntimeWindow != 0) {
		*(int*) ((char*) this + 0x24) = 1;
		*(int*) ((char*) this + 0x18) += g_cbEffTransportMaxPacketBytes;
	}
	*(void**) this = &g_LINKSCF_InvsChunkManagerVtable;
	*(CAI**) ((char*) this + 0x38) = pAI;
	*(int*) ((char*) this + 0x30) = nCapacity;
	*(void**) ((char*) this + 0x3c) = 0;
}

// FUNCTION: LEMBALL 0x0040a270
void CInvisibleSwitchManager::Restart(void)
{
	typedef void(LEMBALL_FASTCALL * ResetProc)(void* pObject);
	InvisibleSwitchManagerResetView* pManager;
	char* pObject;
	int i;
	int cbOffset;

	pManager = (InvisibleSwitchManagerResetView*) this;
	cbOffset = 0;
	if (pManager->m_pObjects3C != 0) {
		for (i = 0; i < pManager->m_cCapacity30; ++i) {
			pObject = (char*) pManager->m_pObjects3C + cbOffset;
			cbOffset += 0x2b8;
			((ResetProc) (*(void***) pObject)[65])(pObject);
		}
	}
}

// FUNCTION: LEMBALL 0x0040a350
CInvisibleSwitchManager::~CInvisibleSwitchManager(void)
{
	void* pChild;

	*(void**) this = &g_LINKSCF_InvsChunkManagerVtable;
	pChild = *(void**) ((char*) this + 0x3c);
	if (pChild != 0) {
		((InvisibleSwitchManagerDeletableChild*) pChild)->Delete(3);
	}
	((VsNetEffStreamCommon*) this)->VsNetEffStreamCommon::~VsNetEffStreamCommon();
}

// FUNCTION: LEMBALL 0x0040a370
void CInvisibleSwitchManager::StepOn(const AICOORD& position, CGameObject* pEntity)
{
	char* pManagerBytes;
	int iObject;

	pManagerBytes = (char*) this;
	iObject = 0;
	if (iObject < *(int*) (pManagerBytes + 0x34)) {
		int nObjectOffset;

		nObjectOffset = 0;
		do {
			CInvisibleSwitch* pObject;

			pObject = (CInvisibleSwitch*) (*(char**) (pManagerBytes + 0x3c) + nObjectOffset);
			pObject->StepOn(position, pEntity);
			nObjectOffset += 0x2b8;
			++iObject;
		} while (iObject < *(int*) (pManagerBytes + 0x34));
	}
}

// FUNCTION: LEMBALL 0x0040a3b0
void CInvisibleSwitchManager::Process(void)
{
	char* pManagerBytes;
	void* pChunkObject;
	int i;
	int nOffset;

	pManagerBytes = (char*) this;
	i = 0;
	if (*(int*) (pManagerBytes + 0x34) > 0) {
		nOffset = 0;
		do {
			pChunkObject = *(char**) (pManagerBytes + 0x3c) + nOffset;
			++i;
			nOffset += 0x2b8;
			((InvsChunkObjectActivateProc) (*(void***) pChunkObject)[5])(pChunkObject);
		} while (*(int*) (pManagerBytes + 0x34) > i);
	}
}
