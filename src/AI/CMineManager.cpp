#include "AI/CMineManager.h"

#include "AI/AICoord.h"
#include "AI/CGameObject.h"
#include "AI/CMine.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"
#include "Visos/Generic/Memory.h"

#include <string.h>

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

struct VsNetEffStreamCommon {
	virtual ~VsNetEffStreamCommon(void);
};

extern MineManagerVtableLayout g_LINKSCF_MineChunkManagerVtable;
extern void* g_pActiveNetworkRuntimeWindow;
extern int g_cbEffTransportMaxPacketBytes;
// FUNCTION: LEMBALL 0x00424020
CMineManager::CMineManager(CAI* pAI, int nCapacity)
{
	*(void**) this = g_GAME_EffStreamConstructionVtable;
	*(int*) ((char*) this + 4) = 0x0d;
	((GameEffStream*) this)->ResetStateFields();
	*(int*) ((char*) this + 0x2c) = 2;
	*(void**) this = g_LEVEL_EffChunkStreamBaseVtable;
	if (g_pActiveNetworkRuntimeWindow != 0) {
		*(int*) ((char*) this + 0x24) = 1;
		*(int*) ((char*) this + 0x18) += g_cbEffTransportMaxPacketBytes;
	}
	*(void**) this = &g_LINKSCF_MineChunkManagerVtable;
	m_pAI30 = pAI;
	m_cCapacity40 = nCapacity;
	m_pObjects34 = 0;
	m_pPositions38 = 0;
}

typedef void(LEMBALL_FASTCALL* MineRestartProc)(void* pObject);

struct MineManagerDeletableChild {
	virtual void Delete(unsigned char fDelete);
};

typedef void(LEMBALL_FASTCALL* MineProcessProc)(void* pObject);

struct MineViewObject {
	virtual void Reserved0(void);
	virtual void Reserved1(void);
	virtual void Reserved2(void);
	virtual void GetViewData(CViewData* pViewData);
};

// FUNCTION: LEMBALL 0x00424080
void CMineManager::Restart(void)
{
	int cbOffset;
	int i;
	char* pObject;

	cbOffset = 0;
	if (m_pObjects34 != 0) {
		i = 0;
		if (m_cCapacity40 > 0) {
			do {
				pObject = (char*) m_pObjects34 + cbOffset;
				++i;
				cbOffset += 0x150;
				((MineRestartProc) (*(void***) pObject)[65])(pObject);
			} while (i < m_cCapacity40);
		}
	}
}

// FUNCTION: LEMBALL 0x004240b0
void CMineManager::Initialise(int nCapacity)
{
	char* pObject;
	int cbOffset;
	int i;

	m_cCapacity40 = nCapacity;
	m_cObjects3C = 0;
	if (nCapacity == 0) {
		m_pObjects34 = 0;
		return;
	}
	if (m_pObjects34 == 0) {
		m_pObjects34 = new CMine[nCapacity];
		cbOffset = 0;
		i = 0;
		if (m_cCapacity40 > 0) {
			do {
				pObject = (char*) m_pObjects34 + cbOffset;
				*(int*) (pObject + 0x14c) = i;
				++i;
				*(CMineManager**) (pObject + 0x60) = this;
				cbOffset += 0x150;
				((MineRestartProc) (*(void***) pObject)[65])(pObject);
			} while (i < m_cCapacity40);
		}
		m_pPositions38 = new MinePosition[m_cCapacity40];
	}
}

// FUNCTION: LEMBALL 0x00424170
CMineManager::~CMineManager(void)
{
	*(void**) this = &g_LINKSCF_MineChunkManagerVtable;
	if (m_pObjects34 != 0) {
		((MineManagerDeletableChild*) m_pObjects34)->Delete(3);
		FreeVSMemBlock(m_pPositions38);
	}
	((VsNetEffStreamCommon*) this)->VsNetEffStreamCommon::~VsNetEffStreamCommon();
}

// FUNCTION: LEMBALL 0x004241a0
void CMineManager::Remove(CMine* pMine)
{
	char* pDestination;
	char* pSource;
	int i;
	int nOffset;
	int nPositionOffset;
	int nSourceIndex;

	i = 0;
	if (m_cObjects3C <= 0) {
		return;
	}
	nOffset = 0;
	while ((char*) m_pObjects34 - (char*) pMine != nOffset) {
		nOffset -= 0x150;
		++i;
		if (m_cObjects3C <= i) {
			return;
		}
	}

	((CGameObject*) ((char*) m_pObjects34 + i * 0x150))->SetId(0xffff);
	nSourceIndex = i + 1;
	if (nSourceIndex < m_cObjects3C) {
		nPositionOffset = nSourceIndex * 6;
		nOffset = nSourceIndex * 0x150;
		do {
			pSource = (char*) m_pObjects34 + nOffset;
			pDestination = pSource - 0x150;

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
			*(int*) (pDestination + 0x40) = *(int*) (pSource + 0x40);
			*(int*) (pDestination + 0x44) = *(int*) (pSource + 0x44);
			*(int*) (pDestination + 0x48) = *(int*) (pSource + 0x48);
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
			pSource = (char*) m_pPositions38 + nPositionOffset;
			pDestination = pSource - 6;
			*(int*) pDestination = *(int*) pSource;
			nPositionOffset += 6;
			*(short*) (pDestination + 4) = *(short*) (pSource + 4);
			++nSourceIndex;
			nOffset += 0x150;
		} while (nSourceIndex < m_cObjects3C);
	}
	--m_cObjects3C;
}

// FUNCTION: LEMBALL 0x00424560
void CMineManager::Triggered(CMine* pMine)
{
	Trigger(*(int*) ((char*) pMine + 0x14c), *(int*) ((char*) pMine + 0x148));
}

// FUNCTION: LEMBALL 0x00424580
void CMineManager::Trigger(int nIndex, int nDelay)
{
	MinePosition* pOrigin;
	MinePosition* pPosition;
	CMine* pMine;
	int cbObject;
	int cbPosition;
	int dx;
	int dy;
	int dz;
	int i;

	pOrigin = (MinePosition*) ((char*) m_pPositions38 + nIndex * 6);
	cbObject = 0;
	i = 0;
	if (m_cObjects3C > 0) {
		cbPosition = 0;
		do {
			if (nIndex != i) {
				pMine = (CMine*) ((char*) m_pObjects34 + cbObject);
				if (pMine->m_nStateB8 == 0x18) {
					pPosition = (MinePosition*) ((char*) m_pPositions38 + cbPosition);
					dz = pPosition->z - pOrigin->z;
					dy = pPosition->y - pOrigin->y;
					dx = pPosition->x - pOrigin->x;
					if (dz * dz + dy * dy + dx * dx < 0x801) {
						pMine->Trigger(nDelay + 6);
					}
				}
			}
			cbObject += 0x150;
			++i;
			cbPosition += 6;
		} while (i < m_cObjects3C);
	}
}

// FUNCTION: LEMBALL 0x00424630
void CMineManager::StepOn(const AICOORD& position, CGameObject* pObject)
{
	MinePosition* pMinePosition;
	CMine* pMine;
	int maxX;
	int maxY;
	int minX;
	int minY;
	int minZ;
	int i;

	minX = (position.x >> 12) - 8;
	minY = (position.y >> 12) - 8;
	minZ = (position.z >> 12) - 8;
	maxX = minX + 15;
	maxY = minY + 15;
	i = 0;
	if (m_cObjects3C > 0) {
		pMine = (CMine*) ((char*) m_pObjects34 + 0x138);
		do {
			if (*(int*) pMine != 0 && *(int*) ((char*) pMine + 4) == 0) {
				pMinePosition = &m_pPositions38[i];
				if (minX < pMinePosition->x && pMinePosition->x < maxX && minY < pMinePosition->y &&
					pMinePosition->y < maxY && minZ < pMinePosition->z && pMinePosition->z < maxY) {
					((CMine*) ((char*) m_pObjects34 + i * 0x150))->StepOn(pObject);
					Trigger(i, 0);
					return;
				}
			}
			pMine = (CMine*) ((char*) pMine + 0x150);
			++i;
		} while (i < m_cObjects3C);
	}
}

// FUNCTION: LEMBALL 0x00424710
void CMineManager::Add(unsigned short nId, AICOORD position)
{
	CMine* pMine;

	if (nId != 0xffff && m_cObjects3C < m_cCapacity40) {
		pMine = (CMine*) ((char*) m_pObjects34 + m_cObjects3C * 0x150);
		((CGameObject*) pMine)->SetId(nId);
		pMine->Set(position);
		m_pPositions38[m_cObjects3C].x = (short) (position.x >> 12);
		m_pPositions38[m_cObjects3C].y = (short) (position.y >> 12);
		m_pPositions38[m_cObjects3C].z = (short) (position.z >> 12);
		++m_cObjects3C;
	}
}

// FUNCTION: LEMBALL 0x004247b0
void CMineManager::Process(void)
{
	CMine* pMine;
	int i;

	i = 0;
	if (m_cObjects3C > 0) {
		pMine = m_pObjects34;
		do {
			pMine->OnGround();
			*(int*) ((char*) pMine + 0x124) = 1;
			if (pMine->m_anRuntimeState138[0] != 0) {
				((MineProcessProc) (*(void***) pMine)[5])(pMine);
			}
			pMine = (CMine*) ((char*) pMine + 0x150);
			++i;
		} while (i < m_cObjects3C);
	}
}

// FUNCTION: LEMBALL 0x00424800
int CMineManager::GetViewData(CViewData* pViewData)
{
	int cbObject;
	int cViewData;
	int i;

	cbObject = 0;
	cViewData = 0;
	i = 0;
	if (m_cObjects3C > 0) {
		do {
			((MineViewObject*) ((char*) m_pObjects34 + cbObject))->GetViewData(pViewData);
			cbObject += 0x150;
			++i;
			++cViewData;
			pViewData = (CViewData*) ((char*) pViewData + 0x4c);
		} while (i < m_cObjects3C);
	}
	return cViewData;
}

// FUNCTION: LEMBALL 0x00424850
void CMineManager::LoadLevel(unsigned char* pData, int cbData, unsigned char nVersion)
{
	AICOORD position;
	unsigned short* pRecord;
	unsigned short nId;
	unsigned int cObjects;

	cObjects = *(unsigned short*) pData;
	pData += 2;
	Initialise(cObjects);
	while (cObjects != 0) {
		pRecord = (unsigned short*) pData;
		if (*(unsigned short*) ((char*) m_pAI30 + 0x54) < 2) {
			nId = CGameObject::NextId();
		}
		else {
			nId = *pRecord;
			++pRecord;
		}
		position.x = (unsigned int) pRecord[0] << 12;
		position.y = (unsigned int) pRecord[1] << 12;
		position.z = (unsigned int) pRecord[2] << 12;
		pData = (unsigned char*) (pRecord + 3);
		Add(nId, position);
		--cObjects;
	}
	(void) cbData;
	(void) nVersion;
}
