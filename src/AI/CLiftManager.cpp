#include "AI/CLiftManager.h"

#include "AI/CLift.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"

extern LiftManagerVtableLayout g_LINKSCF_LiftChunkManagerVtable;
extern void* g_pActiveNetworkRuntimeWindow;
extern int g_cbEffTransportMaxPacketBytes;
extern unsigned short g_nNextLiftObjectId;
extern int g_nLevelFrameClockTimeMs;
extern int g_nNetworkFrameClockTimeMs;
extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);

typedef void(LEMBALL_FASTCALL* LiftRestartProc)(void* pObject);
typedef int(LEMBALL_FASTCALL* LiftProcessProc)(void* pObject);
typedef void(LEMBALL_FASTCALL* LiftViewStateProc)(void* pObject, int nState);

struct LiftManagerDeletableChild {
	virtual void Delete(unsigned char fDelete);
};

struct VsNetEffStreamCommon {
	virtual ~VsNetEffStreamCommon(void);
};

// FUNCTION: LEMBALL 0x00425680
CLiftManager::CLiftManager(CAI* pAI, int nCapacity)
{
	*(void**) this = g_GAME_EffStreamConstructionVtable;
	*(int*) ((char*) this + 4) = 0x12;
	((GameEffStream*) this)->ResetStateFields();
	*(int*) ((char*) this + 0x2c) = 7;
	*(void**) this = g_LEVEL_EffChunkStreamBaseVtable;
	if (g_pActiveNetworkRuntimeWindow != 0) {
		*(int*) ((char*) this + 0x24) = 1;
		*(int*) ((char*) this + 0x18) += g_cbEffTransportMaxPacketBytes;
	}
	*(void**) this = &g_LINKSCF_LiftChunkManagerVtable;
	m_pAI30 = pAI;
	m_cCapacity38 = nCapacity;
	m_pObjects3C = 0;
}

// FUNCTION: LEMBALL 0x004256e0
void CLiftManager::Restart(void)
{
	char* pObject;
	int cbOffset;
	int i;

	g_nNextLiftObjectId = 0;
	cbOffset = 0;
	if (m_pObjects3C != 0) {
		i = 0;
		if (m_cCapacity38 > 0) {
			do {
				pObject = (char*) m_pObjects3C + cbOffset;
				++i;
				cbOffset += 0x190;
				((LiftRestartProc) (*(void***) pObject)[65])(pObject);
			} while (i < m_cCapacity38);
		}
	}
}

// FUNCTION: LEMBALL 0x00425720
void CLiftManager::Initialise(int nCapacity)
{
	char* pObject;
	int cbOffset;
	int i;

	m_cObjects34 = 0;
	if (nCapacity == 0) {
		m_pObjects3C = 0;
		return;
	}
	m_cCapacity38 = nCapacity;
	if (m_pObjects3C == 0) {
		m_pObjects3C = new CLift[nCapacity];
		cbOffset = 0;
		i = 0;
		if (m_cCapacity38 > 0) {
			do {
				*(CLiftManager**) ((char*) m_pObjects3C + cbOffset + 0x60) = this;
				pObject = (char*) m_pObjects3C + cbOffset;
				cbOffset += 0x190;
				++i;
				((LiftRestartProc) (*(void***) pObject)[65])(pObject);
			} while (i < m_cCapacity38);
		}
	}
}

// FUNCTION: LEMBALL 0x004257c0
CLiftManager::~CLiftManager(void)
{
	*(void**) this = &g_LINKSCF_LiftChunkManagerVtable;
	if (m_pObjects3C != 0) {
		((LiftManagerDeletableChild*) m_pObjects3C)->Delete(3);
	}
	((VsNetEffStreamCommon*) this)->VsNetEffStreamCommon::~VsNetEffStreamCommon();
}

// FUNCTION: LEMBALL 0x00425d30
void CLiftManager::Process(void)
{
	CLift* pObject;
	int cbOffset;
	int i;

	cbOffset = 0;
	i = 0;
	if (m_cObjects34 > 0) {
		do {
			pObject = (CLift*) ((char*) m_pObjects3C + cbOffset);
			*(int*) ((char*) pObject + 0x124) = 1;
			((LiftProcessProc) (*(void***) pObject)[5])(pObject);
			cbOffset += 0x190;
			pObject->CheckObjects();
			++i;
		} while (i < m_cObjects34);
	}
}

// FUNCTION: LEMBALL 0x00425d80
void CLiftManager::StepOn(const AICOORD& coord, CGameObject* pObject)
{
	int cbOffset;
	int i;

	i = 0;
	if (m_cObjects34 > 0) {
		cbOffset = 0;
		do {
			((CLift*) ((char*) m_pObjects3C + cbOffset))->StepOn(coord, pObject);
			cbOffset += 0x190;
			++i;
		} while (i < m_cObjects34);
	}
}

// FUNCTION: LEMBALL 0x00425df0
int CLiftManager::GetViewData(CViewData* pViewData)
{
	char* pEntry;
	char* pObject;
	int cbOffset;
	int cEntries;
	int i;

	cEntries = 0;
	i = 0;
	if (m_cObjects34 > 0) {
		cbOffset = 0;
		pEntry = (char*) pViewData;
		do {
			pObject = (char*) m_pObjects3C + cbOffset;
			*(short*) (pEntry + 0x2c) = *(short*) (pObject + 0x6a);
			*(int*) (pEntry + 0x28) = *(int*) (pObject + 0x64);
			*(short*) (pEntry + 0x2e) = 0;
			*(int*) (pEntry + 0x04) = *(int*) (pObject + 0x9c) >> 12;
			*(int*) (pEntry + 0x08) = *(int*) (pObject + 0xa0) >> 12;
			*(int*) (pEntry + 0x0c) = *(int*) (pObject + 0xa4) >> 12;
			*(short*) pEntry = (short) *(int*) (pObject + 0xb4);
			*(int*) (pEntry + 0x18) = *(int*) (pObject + 0xb8);
			*(short*) (pEntry + 0x1c) = (short) *(int*) (pObject + 0xbc);
			*(int*) (pEntry + 0x20) = *(int*) (pObject + 0x94);
			*(int*) (pEntry + 0x14) = 0;
			*(int*) (pEntry + 0x30) = *(int*) (pObject + 0xc0);
			*(int*) (pEntry + 0x38) = *(int*) (pObject + 0xe4);
			*(int*) (pEntry + 0x3c) = *(int*) (pObject + 0xe8);
			*(int*) (pEntry + 0x40) = *(int*) (pObject + 0xec);
			*(int*) (pEntry + 0x44) = *(int*) (pObject + 0x98);
			*(int*) (pEntry + 0x24) =
				*(int*) (pObject + 0x114) != 0 ? g_nNetworkFrameClockTimeMs : g_nLevelFrameClockTimeMs;
			((LiftViewStateProc) (*(void***) pObject)[13])(pObject, 0);
			*(int*) (pEntry + 0x34) = *(int*) (pObject + 0x10);
			*(int*) (pObject + 0x10) = 0;
			cbOffset += 0x190;
			pEntry += 0x4c;
			++cEntries;
			++i;
		} while (i < m_cObjects34);
	}
	return cEntries;
}

// FUNCTION: LEMBALL 0x00425f10
void CLiftManager::Switch(swMessage message, int nSlotId, int nRangeEnd, int nUnused4)
{
	int iObject;
	int nOffset;
	(void) nRangeEnd;
	(void) nUnused4;

	iObject = 0;
	nOffset = 0;
	if (m_cObjects34 > 0) {
		while (GetManagedEntitySlotIdThunk((int) (unsigned long) ((char*) m_pObjects3C + nOffset)) != nSlotId) {
			nOffset += 0x190;
			++iObject;
			if (m_cObjects34 <= iObject) {
				return;
			}
		}
		if (message == 1) {
			char* pObject = (char*) m_pObjects3C + iObject * 0x190;
			if (*(int*) (pObject + 0x15c) != 3) {
				if (*(int*) (pObject + 0x15c) == 0) {
					((CLift*) pObject)->ActivateDeactivate();
					return;
				}
			}
			else if (*(int*) (pObject + 0x16c) == 0) {
				((CLift*) pObject)->Activate();
			}
		}
	}
}
