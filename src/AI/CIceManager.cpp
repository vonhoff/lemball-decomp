#define LEMBALL_CICE_MANAGER_RECONSTRUCTION
#include "AI/CIceManager.h"

#include "AI/CGameObject.h"
#include "AI/CIce.h"
#include "AI/CInvisibleSwitch.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"
#include "Visos/Generic/Memory.h"

#include <string.h>

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

struct LevelThreeDwordPoint {
	int m_anValues[3];
	LevelThreeDwordPoint* CopyThunk(const LevelThreeDwordPoint* pSource);
};

struct LevelThreeDwordPointVariant {
	int m_anValues[3];
	LevelThreeDwordPointVariant* CopyThunk(const LevelThreeDwordPointVariant* pSource);
};

struct LevelTwoDwordPair {
	int m_anValues[2];
	LevelTwoDwordPair* CopyThunk(const LevelTwoDwordPair* pSource);
};

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

// FUNCTION: LEMBALL 0x0042d910
void CIceManager::Remove(CIce* pObject)
{
	int i;
	int j;
	int nOffset;
	int nSourceIndex;
	char* pDestination;
	char* pSource;

	i = 0;
	if (m_nObjectCount34 <= 0) {
		return;
	}
	nOffset = 0;
	while ((char*) m_pObjects38 - (char*) pObject != nOffset) {
		nOffset -= sizeof(CIce);
		++i;
		if (m_nObjectCount34 <= i) {
			return;
		}
	}

	((CGameObject*) ((char*) m_pObjects38 + i * sizeof(CIce)))->SetId(0xffff);
	nSourceIndex = i + 1;
	if (nSourceIndex < m_nObjectCount34) {
		nOffset = nSourceIndex * sizeof(CIce);
		do {
			pSource = (char*) m_pObjects38 + nOffset;
			pDestination = pSource - sizeof(CIce);

			*(short*) (pDestination + 4) = *(short*) (pSource + 4);
			*(int*) (pDestination + 8) = *(int*) (pSource + 8);
			*(int*) (pDestination + 0x0c) = *(int*) (pSource + 0x0c);
			*(int*) (pDestination + 0x10) = *(int*) (pSource + 0x10);
			memcpy(pDestination + 0x14, pSource + 0x14, 0x18);
			*(int*) (pDestination + 0x2c) = *(int*) (pSource + 0x2c);
			*(int*) (pDestination + 0x30) = *(int*) (pSource + 0x30);
			*(int*) (pDestination + 0x34) = *(int*) (pSource + 0x34);
			*(int*) (pDestination + 0x38) = *(int*) (pSource + 0x38);
			*(int*) (pDestination + 0x3c) = *(int*) (pSource + 0x3c);
			((LevelThreeDwordPointVariant*) (pDestination + 0x40))
				->CopyThunk((LevelThreeDwordPointVariant*) (pSource + 0x40));
			((LevelThreeDwordPoint*) (pDestination + 0x4c))->CopyThunk((LevelThreeDwordPoint*) (pSource + 0x4c));
			*(int*) (pDestination + 0x58) = *(int*) (pSource + 0x58);
			*(int*) (pDestination + 0x5c) = *(int*) (pSource + 0x5c);
			*(int*) (pDestination + 0x60) = *(int*) (pSource + 0x60);
			*(int*) (pDestination + 0x64) = *(int*) (pSource + 0x64);
			*(short*) (pDestination + 0x68) = *(short*) (pSource + 0x68);
			*(short*) (pDestination + 0x6a) = *(short*) (pSource + 0x6a);
			*(short*) (pDestination + 0x6c) = *(short*) (pSource + 0x6c);
			*(short*) (pDestination + 0x6e) = *(short*) (pSource + 0x6e);
			*(int*) (pDestination + 0x70) = *(int*) (pSource + 0x70);
			*(int*) (pDestination + 0x74) = *(int*) (pSource + 0x74);
			((LevelTwoDwordPair*) (pDestination + 0x78))->CopyThunk((LevelTwoDwordPair*) (pSource + 0x78));
			((LevelTwoDwordPair*) (pDestination + 0x80))->CopyThunk((LevelTwoDwordPair*) (pSource + 0x80));
			*(int*) (pDestination + 0x88) = *(int*) (pSource + 0x88);
			*(int*) (pDestination + 0x8c) = *(int*) (pSource + 0x8c);
			*(int*) (pDestination + 0x90) = *(int*) (pSource + 0x90);
			*(int*) (pDestination + 0x94) = *(int*) (pSource + 0x94);
			*(int*) (pDestination + 0x98) = *(int*) (pSource + 0x98);
			((LevelThreeDwordPointVariant*) (pDestination + 0x9c))
				->CopyThunk((LevelThreeDwordPointVariant*) (pSource + 0x9c));
			((LevelThreeDwordPointVariant*) (pDestination + 0xa8))
				->CopyThunk((LevelThreeDwordPointVariant*) (pSource + 0xa8));
			*(short*) (pDestination + 0xb4) = *(short*) (pSource + 0xb4);
			*(short*) (pDestination + 0xb6) = *(short*) (pSource + 0xb6);
			*(int*) (pDestination + 0xb8) = *(int*) (pSource + 0xb8);
			*(short*) (pDestination + 0xbc) = *(short*) (pSource + 0xbc);
			*(int*) (pDestination + 0xc0) = *(int*) (pSource + 0xc0);
			*(short*) (pDestination + 0xc4) = *(short*) (pSource + 0xc4);
			*(int*) (pDestination + 0xc8) = *(int*) (pSource + 0xc8);
			*(int*) (pDestination + 0xcc) = *(int*) (pSource + 0xcc);
			*(int*) (pDestination + 0xd0) = *(int*) (pSource + 0xd0);
			*(int*) (pDestination + 0xd4) = *(int*) (pSource + 0xd4);
			((LevelThreeDwordPoint*) (pDestination + 0xd8))->CopyThunk((LevelThreeDwordPoint*) (pSource + 0xd8));
			((LevelThreeDwordPoint*) (pDestination + 0xe4))->CopyThunk((LevelThreeDwordPoint*) (pSource + 0xe4));
			*(int*) (pDestination + 0xf0) = *(int*) (pSource + 0xf0);
			((LevelThreeDwordPointVariant*) (pDestination + 0xf4))
				->CopyThunk((LevelThreeDwordPointVariant*) (pSource + 0xf4));
			*(int*) (pDestination + 0x100) = *(int*) (pSource + 0x100);
			*(int*) (pDestination + 0x104) = *(int*) (pSource + 0x104);
			*(int*) (pDestination + 0x108) = *(int*) (pSource + 0x108);
			*(int*) (pDestination + 0x10c) = *(int*) (pSource + 0x10c);
			*(int*) (pDestination + 0x110) = *(int*) (pSource + 0x110);
			*(int*) (pDestination + 0x114) = *(int*) (pSource + 0x114);
			*(int*) (pDestination + 0x118) = *(int*) (pSource + 0x118);
			*(int*) (pDestination + 0x11c) = *(int*) (pSource + 0x11c);
			*(short*) (pDestination + 0x120) = *(short*) (pSource + 0x120);
			*(int*) (pDestination + 0x124) = *(int*) (pSource + 0x124);
			*(int*) (pDestination + 0x128) = *(int*) (pSource + 0x128);
			*(int*) (pDestination + 0x12c) = *(int*) (pSource + 0x12c);
			*(int*) (pDestination + 0x130) = *(int*) (pSource + 0x130);
			*(int*) (pDestination + 0x134) = *(int*) (pSource + 0x134);
			*(int*) (pDestination + 0x138) = *(int*) (pSource + 0x138);
			*(int*) (pDestination + 0x13c) = *(int*) (pSource + 0x13c);
			*(int*) (pDestination + 0x140) = *(int*) (pSource + 0x140);
			*(int*) (pDestination + 0x144) = *(int*) (pSource + 0x144);
			*(int*) (pDestination + 0x148) = *(int*) (pSource + 0x148);
			*(int*) (pDestination + 0x14c) = *(int*) (pSource + 0x14c);
			*(int*) (pDestination + 0x150) = *(int*) (pSource + 0x150);
			for (j = 0; j < 10; ++j) {
				*(int*) (pDestination + 0x154 + j * 4) = *(int*) (pSource + 0x154 + j * 4);
			}
			*(int*) (pDestination + 0x17c) = *(int*) (pSource + 0x17c);
			*(short*) (pDestination + 0x180) = *(short*) (pSource + 0x180);
			*(int*) (pDestination + 0x182) = *(int*) (pSource + 0x182);
			*(short*) (pDestination + 0x186) = *(short*) (pSource + 0x186);

			++nSourceIndex;
			nOffset += sizeof(CIce);
		} while (nSourceIndex < m_nObjectCount34);
	}
	--m_nObjectCount34;
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
