#include "AI/CLaserManager.h"

#include "AI/CGameObject.h"
#include "AI/CLaser.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"

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

struct LaserManagerVtableLayout;
extern LaserManagerVtableLayout g_LINKSCF_LasrChunkManagerVtable;
extern void* g_pActiveNetworkRuntimeWindow;
extern int g_cbEffTransportMaxPacketBytes;
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

// FUNCTION: LEMBALL 0x00429470
void CLaserManager::Remove(CLaser* pLaser)
{
	char* pDestination;
	char* pObject;
	char* pSource;
	int i;
	int nOffset;
	int nSourceIndex;

	i = 0;
	if (m_cObjects34 <= 0) return;
	pObject = (char*) m_pObjects38;
	while (pObject != (char*) pLaser) {
		pObject += 0x148;
		++i;
		if (m_cObjects34 <= i) return;
	}

	((CGameObject*) pObject)->SetId(0xffff);
	nSourceIndex = i + 1;
	if (nSourceIndex < m_cObjects34) {
		nOffset = nSourceIndex * 0x148;
		do {
			pSource = (char*) m_pObjects38 + nOffset;
			pDestination = pSource - 0x148;

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
			++nSourceIndex;
			nOffset += 0x148;
		} while (nSourceIndex < m_cObjects34);
	}
	--m_cObjects34;
}

// FUNCTION: LEMBALL 0x004297f0
int CLaserManager::StepOn(const AICOORD& position, CGameObject* pObject)
{
	int cbOffset;
	int i;

	cbOffset = 0;
	i = 0;
	while (i < m_cObjects34) {
		CLaser* pLaser = (CLaser*) ((char*) m_pObjects38 + cbOffset);
		if (pLaser->m_anRuntimeState138[1] != 0 && pLaser->m_anRuntimeState138[2] == 0 && pLaser->m_nStateB8 == 0x18 &&
			pLaser->StepOn(position, pObject) != 0) {
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
			if (((CLaser*) pObject)->m_anRuntimeState138[0] != 0 || *(int*) (pObject + 0x114) != 0) {
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
			nSlot = CGameObject::NextId();
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
