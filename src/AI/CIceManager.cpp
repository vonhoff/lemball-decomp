#define LEMBALL_CICE_MANAGER_RECONSTRUCTION
#include "AI/CIceManager.h"

#include "AI/CGameObject.h"
#include "AI/CIce.h"
#include "AI/CInvisibleSwitch.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"
#include "Visos/Generic/Memory.h"

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

// FUNCTION: LEMBALL 0x0042d830
void CIceManager::Initialise(int nCount)
{
	CIce* pBase;
	int i;

	m_nCapacity30 = nCount;
	m_nObjectCount34 = 0;
	if (nCount != 0) {
		if (m_pObjects38 == 0) {
			pBase = (CIce*) ((char*) AllocateVSMemBlock((unsigned int) (nCount * sizeof(CIce) + 4)) + 4);
			if (pBase != 0) {
				*(int*) ((char*) pBase - 4) = nCount;
				for (i = nCount - 1; i >= 0; --i) {
					((void(__fastcall*)(char*)) 0x40353f)((char*) pBase + i * sizeof(CIce));
				}
				m_pObjects38 = pBase;
			}
			else {
				m_pObjects38 = 0;
			}
		}
		for (i = 0; i < m_nCapacity30; ++i) {
			((IceObjectProc) (*(void***) ((char*) m_pObjects38 + i * sizeof(CIce)))[65])((char*) m_pObjects38 +
																						 i * sizeof(CIce));
			m_pObjects38[i].m_pOwnerManager60 = this;
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

// FUNCTION: LEMBALL 0x0042dd40
int CIceManager::StepOn(void* pCoord, void* pGameObject)
{
	char* pObject;
	int i;
	int nOffset;

	if (m_nObjectCount34 <= 0) {
		return 0;
	}
	for (i = 0, nOffset = 0; i < m_nObjectCount34; ++i, nOffset += sizeof(CIce)) {
		pObject = (char*) m_pObjects38 + nOffset;
		if (((CIce*) pObject)->StepOn(*(AICOORD*) pCoord, (CGameObject*) pGameObject) != 0) {
			return 1;
		}
	}
	return 0;
}

// MACINTOSH: CIceManager::Add(unsigned short, const tCoord3d&, const tCoord3d&, int, int, unsigned char)
// FUNCTION: LEMBALL 0x0042ddf0
void CIceManager::Add(unsigned short nSlot,
					  const tCoord3d& start,
					  const tCoord3d& end,
					  int nMoveX,
					  int nMoveY,
					  unsigned char fActive)
{
	if (m_nObjectCount34 < m_nCapacity30) {
		m_pObjects38[m_nObjectCount34].Set(nSlot, start, end, nMoveX, nMoveY, fActive);
		++m_nObjectCount34;
	}
}

// MACINTOSH: CIceManager::LoadLevel(unsigned char*, int, unsigned char)
// FUNCTION: LEMBALL 0x0042dea0
void CIceManager::LoadLevel(unsigned char* pData, int cbData, unsigned char nVersion)
{
	unsigned int cObjects = *(unsigned short*) pData;
	unsigned short* pRecord = (unsigned short*) (pData + 2);
	tCoord3d start;
	tCoord3d end;

	Initialise(cObjects);
	while (cObjects != 0) {
		unsigned short nSlot;
		unsigned short nFormatVersion = *(unsigned short*) ((char*) m_pAI3C + 0x54);
		if (nFormatVersion < 2) {
			nSlot = CGameObject::NextId();
		}
		else {
			nSlot = *pRecord;
			++pRecord;
		}
		start.x = pRecord[0];
		start.y = pRecord[1];
		start.z = pRecord[2];
		end.x = pRecord[3];
		end.y = pRecord[4];
		end.z = pRecord[5];
		Add(nSlot,
			start,
			end,
			(short) pRecord[6],
			(short) pRecord[7],
			nFormatVersion > 9 ? (unsigned char) pRecord[8] : 1);
		pRecord += nFormatVersion > 9 ? 9 : 8;
		--cObjects;
	}
	(void) cbData;
	(void) nVersion;
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

// FUNCTION: LEMBALL 0x0042d900 (was "reset_ice_chunk_object_count")
void CIceManager::ResetObjectCount(void)
{
	m_nObjectCount34 = 0;
}
