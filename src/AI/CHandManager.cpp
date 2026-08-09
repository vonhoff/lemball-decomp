#include "AI/CHandManager.h"

#include "AI/AICoord.h"
#include "AI/CGameObject.h"
#include "AI/CHand.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"
#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"

#include <string.h>

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);

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

// FUNCTION: LEMBALL 0x00427fd0
void CHandManager::Remove(CHand* pHand)
{
	unsigned short nSlot;
	char* pDestination;
	char* pObject;
	char* pSource;
	int i;
	int nOffset;
	int nSourceIndex;

	i = 0;
	nSlot = GetManagedEntitySlotIdThunk((int) (unsigned long) pHand);
	if (m_nObjectCount34 <= 0) {
		return;
	}
	nOffset = 0;
	do {
		pObject = (char*) m_pObjects38 + nOffset;
		if (GetManagedEntitySlotIdThunk((int) (unsigned long) pObject) == nSlot) {
			break;
		}
		nOffset += 0x144;
		++i;
		if (m_nObjectCount34 <= i) {
			return;
		}
	} while (1);

	pObject = (char*) m_pObjects38 + i * 0x144;
	((CGameObject*) pObject)->SetId(0xffff);
	nSourceIndex = i + 1;
	if (nSourceIndex < m_nObjectCount34) {
		nOffset = nSourceIndex * 0x144;
		do {
			pSource = (char*) m_pObjects38 + nOffset;
			pDestination = pSource - 0x144;

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
			++nSourceIndex;
			nOffset += 0x144;
		} while (nSourceIndex < m_nObjectCount34);
	}
	--m_nObjectCount34;
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
