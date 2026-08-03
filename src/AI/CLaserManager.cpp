#include "AI/CLaserManager.h"

#include "AI/CLaser.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"

struct LaserManagerVtableLayout;
extern LaserManagerVtableLayout g_LINKSCF_LasrChunkManagerVtable;
extern void* g_pActiveNetworkRuntimeWindow;
extern int g_cbEffTransportMaxPacketBytes;
extern unsigned short FindFirstFreeManagedEntitySlotIdForwardThunk(void);

struct VsNetEffStreamCommon {
	virtual ~VsNetEffStreamCommon(void);
};

struct LaserManagerDeletableChild {
	virtual void Delete(unsigned char fDelete);
};

typedef char LaserManagerSizeMustMatchLayout[sizeof(CLaserManager) == 0x40 ? 1 : -1];
typedef char LaserSizeMustMatchArrayStride[sizeof(CLaser) == 0x148 ? 1 : -1];

typedef void(LEMBALL_FASTCALL* LaserRestartProc)(void* pObject);
typedef int(LEMBALL_FASTCALL* LaserProcessProc)(void* pObject);

// FUNCTION: LEMBALL 0x00429320
CLaserManager::CLaserManager(CAI* pAI, int nCapacity)
{
	*(void**) this = g_GAME_EffStreamConstructionVtable;
	*(int*) ((char*) this + 4) = 0x15;
	((GameEffStream*) this)->ResetStateFields();
	*(int*) ((char*) this + 0x2c) = 10;
	*(void**) this = g_LEVEL_EffChunkStreamBaseVtable;
	if (g_pActiveNetworkRuntimeWindow != 0) {
		*(int*) ((char*) this + 0x24) = 1;
		*(int*) ((char*) this + 0x18) += g_cbEffTransportMaxPacketBytes;
	}
	*(void**) this = &g_LINKSCF_LasrChunkManagerVtable;
	m_pAI3C = pAI;
	m_cCapacity30 = nCapacity;
	m_pObjects38 = 0;
}

// FUNCTION: LEMBALL 0x00429380
void CLaserManager::Restart(void)
{
	char* pManagerBytes;
	void* pChunkObject;
	int i;
	int nOffset;

	pManagerBytes = (char*) this;
	if (*(char**) (pManagerBytes + 0x38) != 0) {
		i = 0;
		if (*(int*) (pManagerBytes + 0x30) > 0) {
			nOffset = 0;
			do {
				pChunkObject = *(char**) (pManagerBytes + 0x38) + nOffset;
				++i;
				nOffset += 0x148;
				((LaserRestartProc) (*(void***) pChunkObject)[65])(pChunkObject);
			} while (*(int*) (pManagerBytes + 0x30) > i);
		}
	}
}

// FUNCTION: LEMBALL 0x004293b0
void CLaserManager::Initialise(int nCapacity)
{
	char* pObject;
	int i;
	int cbOffset;

	m_cObjects34 = 0;
	if (nCapacity == 0) {
		m_pObjects38 = 0;
		return;
	}
	if (m_pObjects38 == 0) {
		m_pObjects38 = new CLaser[m_cCapacity30];
		cbOffset = 0;
		i = 0;
		if (m_cCapacity30 > 0) {
			do {
				*(CLaserManager**) ((char*) m_pObjects38 + cbOffset + 0x60) = this;
				pObject = (char*) m_pObjects38 + cbOffset;
				cbOffset += 0x148;
				++i;
				((LaserRestartProc) (*(void***) pObject)[65])(pObject);
			} while (i < m_cCapacity30);
		}
	}
}

// FUNCTION: LEMBALL 0x00429450
CLaserManager::~CLaserManager(void)
{
	void* pChild;

	*(void**) this = &g_LINKSCF_LasrChunkManagerVtable;
	pChild = m_pObjects38;
	if (pChild != 0) {
		((LaserManagerDeletableChild*) pChild)->Delete(3);
	}
	((VsNetEffStreamCommon*) this)->VsNetEffStreamCommon::~VsNetEffStreamCommon();
}

// FUNCTION: LEMBALL 0x004297f0
int CLaserManager::StepOn(const AICOORD& position, CGameObject* pObject)
{
	char* pLaser;
	int cbOffset;
	int i;

	cbOffset = 0;
	i = 0;
	while (i < m_cObjects34) {
		pLaser = (char*) m_pObjects38 + cbOffset;
		if (*(int*) (pLaser + 0x13c) != 0 && *(int*) (pLaser + 0x140) == 0 && *(int*) (pLaser + 0xb8) == 0x18 &&
			((CLaser*) pLaser)->StepOn(position, pObject) != 0) {
			return 1;
		}
		cbOffset += 0x148;
		++i;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00429850
void CLaserManager::Process(void)
{
	char* pObject;
	int i;
	int cbOffset;

	i = 0;
	if (m_cObjects34 > 0) {
		cbOffset = 0;
		do {
			*(int*) ((char*) m_pObjects38 + cbOffset + 0x124) = 1;
			pObject = (char*) m_pObjects38 + cbOffset;
			if (*(int*) (pObject + 0x138) != 0 || *(int*) (pObject + 0x114) != 0) {
				((LaserProcessProc) (*(void***) pObject)[5])(pObject);
			}
			cbOffset += 0x148;
			++i;
		} while (i < m_cObjects34);
	}
}

// FUNCTION: LEMBALL 0x004298a0
int CLaserManager::GetViewData(CViewData* pViewData)
{
	char* pObject;
	int i;
	int cbOffset;
	int cViewData;
	int cTotal;

	i = 0;
	cbOffset = 0;
	cTotal = 0;
	if (m_cObjects34 > 0) {
		do {
			pObject = (char*) m_pObjects38 + cbOffset;
			++i;
			cbOffset += 0x148;
			cViewData = ((CLaser*) pObject)->GetViewData(pViewData);
			cTotal += cViewData;
			pViewData = (CViewData*) ((char*) pViewData + cViewData * 0x4c);
		} while (i < m_cObjects34);
	}
	return cTotal;
}

// FUNCTION: LEMBALL 0x004298f0
void CLaserManager::Add(unsigned short nSlot, int x, int y, int z, eObjectType objectType)
{
	AICOORD position;

	if (m_cObjects34 < m_cCapacity30) {
		position.x = x << 12;
		position.y = y << 12;
		position.z = z << 12;
		m_pObjects38[m_cObjects34].Set(nSlot, position, objectType);
		++m_cObjects34;
	}
}

// FUNCTION: LEMBALL 0x00429950
void CLaserManager::LoadLevel(unsigned char* pData, int cbData, unsigned char nVersion)
{
	unsigned short nSlot;
	unsigned short* pRecord;
	int nObjects;

	nObjects = *(unsigned short*) pData;
	pData += 2;
	Initialise(nObjects);
	while (nObjects != 0) {
		pRecord = (unsigned short*) pData;
		if (*(unsigned short*) ((char*) m_pAI3C + 0x54) < 2) {
			nSlot = FindFirstFreeManagedEntitySlotIdForwardThunk();
		}
		else {
			nSlot = *pRecord;
			++pRecord;
		}
		pData = (unsigned char*) (pRecord + 4);
		Add(nSlot, pRecord[1], pRecord[2], pRecord[3], (eObjectType) pRecord[0]);
		--nObjects;
	}
	(void) cbData;
	(void) nVersion;
}
