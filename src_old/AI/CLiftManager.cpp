#include "AI/CLiftManager.h"

#include "AI/CGameObject.h"
#include "AI/CLift.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"

#include <string.h>

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

struct LevelThreeDwordPoint {
	int m_anValues[3];
	LevelThreeDwordPoint* CopyThunk(const LevelThreeDwordPoint* pSource);
};

struct LevelThreeDwordPointVariant {
	int m_anValues[3];
	LevelThreeDwordPointVariant* CopyThunk(const LevelThreeDwordPointVariant* pSource);
};

struct FourDwordValue {
	int m_anValues[4];
	FourDwordValue* CopyFrom(const int* pSource);
};

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

// FUNCTION: LEMBALL 0x00425890
void CLiftManager::Remove(CLift* pLift)
{
	int i;
	int j;
	int nOffset;
	int nSourceIndex;
	char* pDestination;
	char* pSource;

	i = 0;
	if (m_cObjects34 <= 0) {
		return;
	}
	nOffset = 0;
	while ((char*) m_pObjects3C - (char*) pLift != nOffset) {
		nOffset -= sizeof(CLift);
		++i;
		if (m_cObjects34 <= i) {
			return;
		}
	}

	((CGameObject*) ((char*) m_pObjects3C + i * sizeof(CLift)))->SetId(0xffff);
	nSourceIndex = i + 1;
	if (nSourceIndex < m_cObjects34) {
		nOffset = nSourceIndex * sizeof(CLift);
		do {
			pSource = (char*) m_pObjects3C + nOffset;
			pDestination = pSource - sizeof(CLift);

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
			((FourDwordValue*) (pDestination + 0x78))->CopyFrom((int*) (pSource + 0x78));
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
			*(short*) (pDestination + 0x138) = *(short*) (pSource + 0x138);
			*(int*) (pDestination + 0x13a) = *(int*) (pSource + 0x13a);
			*(short*) (pDestination + 0x13e) = *(short*) (pSource + 0x13e);
			*(int*) (pDestination + 0x140) = *(int*) (pSource + 0x140);
			*(short*) (pDestination + 0x144) = *(short*) (pSource + 0x144);
			*(int*) (pDestination + 0x148) = *(int*) (pSource + 0x148);
			*(int*) (pDestination + 0x14c) = *(int*) (pSource + 0x14c);
			*(int*) (pDestination + 0x150) = *(int*) (pSource + 0x150);
			*(short*) (pDestination + 0x154) = *(short*) (pSource + 0x154);
			*(int*) (pDestination + 0x158) = *(int*) (pSource + 0x158);
			*(int*) (pDestination + 0x15c) = *(int*) (pSource + 0x15c);
			*(int*) (pDestination + 0x160) = *(int*) (pSource + 0x160);
			*(int*) (pDestination + 0x164) = *(int*) (pSource + 0x164);
			*(int*) (pDestination + 0x168) = *(int*) (pSource + 0x168);
			*(int*) (pDestination + 0x16c) = *(int*) (pSource + 0x16c);
			for (j = 0; j < 8; ++j) {
				*(int*) (pDestination + 0x170 + j * 4) = *(int*) (pSource + 0x170 + j * 4);
			}

			++nSourceIndex;
			nOffset += sizeof(CLift);
		} while (nSourceIndex < m_cObjects34);
	}
	--m_cObjects34;
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
			*(int*) (pEntry + 0x04) = ((CLift*) pObject)->m_WorldPosition9C.x >> 12;
			*(int*) (pEntry + 0x08) = ((CLift*) pObject)->m_WorldPosition9C.y >> 12;
			*(int*) (pEntry + 0x0c) = ((CLift*) pObject)->m_WorldPosition9C.z >> 12;
			*(short*) pEntry = (short) *(int*) (pObject + 0xb4);
			*(int*) (pEntry + 0x18) = ((CLift*) pObject)->m_nStateB8;
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

// FUNCTION: LEMBALL 0x00425f90
unsigned short CLiftManager::Id(int iObject)
{
	if (m_cObjects34 <= iObject) {
		return 0xffff;
	}
	return GetManagedEntitySlotIdThunk((int) (unsigned long) ((char*) m_pObjects3C + iObject * 0x190));
}

// FUNCTION: LEMBALL 0x00425fc0
void CLiftManager::LoadLevel(unsigned char* pData, int cbData, unsigned char nVersion)
{
	unsigned short* pRecord;
	unsigned short nSlotId;
	unsigned int cObjects;
	CLift* pObject;
	tCoord3d start;
	tCoord3d end;

	cObjects = *(unsigned short*) pData;
	pData += 2;
	Initialise(cObjects);
	if (*(unsigned short*) ((char*) m_pAI30 + 0x54) > 2 && cObjects != 0) {
		do {
			pRecord = (unsigned short*) pData;
			if (*(unsigned short*) ((char*) m_pAI30 + 0x54) < 2) {
				nSlotId = CGameObject::NextId();
			}
			else {
				nSlotId = *pRecord;
				++pRecord;
			}
			pObject = (CLift*) ((char*) m_pObjects3C + m_cObjects34 * 0x190);
			((CGameObject*) pObject)->SetId(nSlotId);
			if (*(unsigned short*) ((char*) m_pAI30 + 0x54) < 5) {
				pData = (unsigned char*) (pRecord + 8);
				pObject->Set(pRecord[2],
							 pRecord[3],
							 pRecord[4],
							 (short) pRecord[7],
							 (short) pRecord[5],
							 (short) pRecord[6],
							 pRecord[1],
							 (unsigned char) pRecord[0]);
			}
			else {
				start.x = pRecord[2];
				start.y = pRecord[3];
				start.z = pRecord[4];
				end.x = pRecord[5];
				end.y = pRecord[6];
				end.z = pRecord[7];
				pData = (unsigned char*) (pRecord + 11);
				pObject->Set(start,
							 end,
							 (short) pRecord[10],
							 (short) pRecord[8],
							 (short) pRecord[9],
							 pRecord[1],
							 (unsigned char) pRecord[0]);
			}
			++m_cObjects34;
			--cObjects;
		} while (cObjects != 0);
	}
	(void) cbData;
	(void) nVersion;
}

// FUNCTION: LEMBALL 0x00425ce0
void CLiftManager::AppendLiftChunkObjectBetweenEndpoints(unsigned short nSlotId,
														 const tCoord3d& start,
														 const tCoord3d& end)
{
	if (m_cObjects34 < m_cCapacity38) {
		((CGameObject*) ((char*) m_pObjects3C + m_cObjects34 * 0x190))->SetId(nSlotId);
		((CLift*) ((char*) m_pObjects3C + m_cObjects34 * 0x190))
			->Set((tCoord3d&) start, (tCoord3d&) end, 1, -1, 0x30, (eLiftActivateType) 2, 1);
		m_cObjects34++;
	}
}

// FUNCTION: LEMBALL 0x00425c80
void CLiftManager::AppendLegacyLiftChunkObject(unsigned short nSlotId, int nParam1, int nParam2, int nParam3)
{
	if (m_cObjects34 < m_cCapacity38) {
		((CGameObject*) ((char*) m_pObjects3C + m_cObjects34 * 0x190))->SetId(nSlotId);
		((CLift*) ((char*) m_pObjects3C + m_cObjects34 * 0x190))
			->Set(nParam1, nParam2, nParam3, 1, -1, 0x30, (eLiftActivateType) 2, 1);
		m_cObjects34++;
	}
}
