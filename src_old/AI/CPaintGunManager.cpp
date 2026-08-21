#include "AI/CPaintGunManager.h"

#include "AI/CGameObject.h"
#include "AI/CPaintGun.h"
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

// Mac blueprint: CPaintGunManager::Add(ushort, int, int, int, int)

// FUNCTION: LEMBALL 0x0042c590
void CPaintGunManager::Add(unsigned short nSlot, int nWorldX, int nWorldY, int nWorldZ, int nSubtype)
{
	CPaintGun* pObject;
	int nLocal[3];

	if (m_nObjectCount34 < m_nCapacity30) {
		nLocal[0] = nWorldX << 12;
		nLocal[1] = nWorldY << 12;
		nLocal[2] = nWorldZ << 12;
		pObject = (CPaintGun*) (m_pObjects38 + m_nObjectCount34 * 0x144);
		pObject->Set(nSlot, nLocal, 0);
		pObject->m_nSubtype140 = nSubtype;
		m_nObjectCount34 = m_nObjectCount34 + 1;
	}
}

// Mac blueprint: CPaintGunManager::LoadLevel(ushort*, int, uchar)
// FUNCTION: LEMBALL 0x0042c610
void CPaintGunManager::LoadLevel(unsigned short* pLevelData, int nLen, unsigned char nFormat)
{
	typedef unsigned int(__fastcall * FindFreeSlotProc)(void);
	unsigned int nCount = (unsigned int) *pLevelData;
	unsigned short nSlot;
	int nX;
	int nY;
	int nZ;
	int nSubtype;

	pLevelData = pLevelData + 1;
	((void(__fastcall*)(void*, unsigned int)) 0x4028d3)(this, nCount);
	while (nCount != 0) {
		if (*(unsigned short*) ((char*) m_pLevelMode3C + 0x54) <= 1) {
			nSlot = (unsigned short) ((FindFreeSlotProc) 0x40214e)();
		}
		else {
			nSlot = *pLevelData++;
		}
		nX = *pLevelData++;
		nY = *pLevelData++;
		nZ = *pLevelData++;
		nSubtype = *pLevelData++;
		Add(nSlot, nX, nY, (short) nZ, nSubtype);
		nCount = nCount - 1;
	}
}

// FUNCTION: LEMBALL 0x0042C150
void CPaintGunManager::Remove(CPaintGun* pPaintGun)
{
	char* pDestination;
	char* pObject;
	char* pSource;
	int i;
	int nOffset;
	int nSourceIndex;

	i = 0;
	if (m_nObjectCount34 <= 0) {
		return;
	}
	pObject = (char*) m_pObjects38;
	while (pObject != (char*) pPaintGun) {
		pObject += 0x144;
		++i;
		if (m_nObjectCount34 <= i) {
			return;
		}
	}

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

// Mac blueprint: CPaintGunManager::Process()
// FUNCTION: LEMBALL 0x0042c4d0
void CPaintGunManager::Process(void)
{
	int iObject = 0;
	if (m_nObjectCount34 > 0) {
		int nObjectOffset = 0;
		do {
			*(int*) ((char*) m_pObjects38 + 0x124 + nObjectOffset) = 1;
			void* pObject = (void*) ((char*) m_pObjects38 + nObjectOffset);
			if (*(int*) ((char*) pObject + 0x4e * 4) != 0) {
				(*(void (**)(void))(*(void***) pObject + 0x14 / 4))();
			}
			nObjectOffset = nObjectOffset + 0x144;
			iObject = iObject + 1;
		} while (iObject < m_nObjectCount34);
	}
}

// Mac blueprint: CPaintGunManager::GetViewData(CViewData*)
// FUNCTION: LEMBALL 0x0042c520
int CPaintGunManager::GetViewData(CViewData* pViewData)
{
	int nCount = 0;
	int iChild = 0;
	if (m_nObjectCount34 > 0) {
		int nObjectOffset = 0;
		do {
			void* pChild = (void*) ((char*) m_pObjects38 + nObjectOffset);
			if (*(int*) ((char*) pChild + 0x138) != 0) {
				((void(__fastcall*)(void*, CViewData*))(*(void***) pChild)[0xc / 4])(pChild, pViewData);
				nCount = nCount + 1;
				pViewData = (CViewData*) ((char*) pViewData + 0x4c);
			}
			nObjectOffset = nObjectOffset + 0x144;
			iChild = iChild + 1;
		} while (iChild < m_nObjectCount34);
	}
	return nCount;
}

// FUNCTION: LEMBALL 0x0042c120
CPaintGunManager::~CPaintGunManager(void)
{
	*(int*) this = 0x496888;
	if (*(void**) ((char*) this + 0x38) != 0) {
		((void(__fastcall*)(void*, int))(*(void***) ((char*) this + 0x38))[0])(*(void**) ((char*) this + 0x38), 3);
	}
	((void(__fastcall*)(void*)) 0x45eea0)(this);
}
