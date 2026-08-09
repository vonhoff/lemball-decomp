#include "AI/CSlinkyManager.h"

#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

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

struct CSlinky {
	void Set(int nSlinkX, int nSlinkY, int nSlinkZ, int nSlinkW);
};

struct CGameObject {
	void SetId(unsigned short nSlotId);
};

// FUNCTION: LEMBALL 0x0040ba30
void CSlinkyManager::Remove(void* pSlinky)
{
	char* pDestination;
	char* pSource;
	int i;
	int nOffset;
	int nSourceIndex;

	i = 0;
	if (m_nObjectCount0C <= 0) {
		return;
	}
	nOffset = 0;
	while (GetManagedEntitySlotIdThunk(m_pObjects04 + nOffset) !=
		   GetManagedEntitySlotIdThunk((int) (unsigned long) pSlinky)) {
		nOffset += 0x150;
		++i;
		if (m_nObjectCount0C <= i) {
			return;
		}
	}

	((CGameObject*) (m_pObjects04 + i * 0x150))->SetId(0xffff);
	nSourceIndex = i + 1;
	if (nSourceIndex < m_nObjectCount0C) {
		nOffset = nSourceIndex * 0x150;
		do {
			pSource = (char*) m_pObjects04 + nOffset;
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

			++nSourceIndex;
			nOffset += 0x150;
		} while (nSourceIndex < m_nObjectCount0C);
	}
	--m_nObjectCount0C;
}

// FUNCTION: LEMBALL 0x0040be20
void CSlinkyManager::Process(void)
{
	int i;
	if (m_nObjectCount0C > 0) {
		for (i = 0; i < m_nObjectCount0C; i++) {
			(*( void(**)(void)) (*(void***) (m_pObjects04 + i * 0x150) + 0x14 / 4))();
		}
	}
}
// FUNCTION: LEMBALL 0x0040b9e0
void CSlinkyManager::Add(int nSlotId, int n2, int n3, int n4, int n5)
{
	if (m_nObjectCount0C < m_nCapacity08) {
		((CGameObject*) (m_pObjects04 + m_nObjectCount0C * 0x150))->SetId((unsigned short) nSlotId);
		((CSlinky*) (m_pObjects04 + m_nObjectCount0C * 0x150))->Set(n2, n4, n3, n5);
		m_nObjectCount0C = m_nObjectCount0C + 1;
	}
}
// FUNCTION: LEMBALL 0x0040be50
void CSlinkyManager::LoadLevel(unsigned short* pLevelData, int nLen, unsigned char nFormat)
{
	unsigned short nCount = *pLevelData;
	pLevelData++;
	((void(__fastcall*) (void*, int, int)) 0x403396)(this, 0, nCount);
	if (nCount != 0) {
		do {
			unsigned short uSlot;
			if (*(unsigned short*) ((char*) m_vtable00 + 0x54) < 2) {
				uSlot = (unsigned short) ((int(__fastcall*) ()) 0x40214e)();
			}
			else {
				uSlot = *pLevelData;
				pLevelData++;
			}
			unsigned int word1 = *(pLevelData);
			pLevelData++;
			unsigned int word2 = *(pLevelData);
			pLevelData++;
			unsigned int word3 = *(pLevelData);
			pLevelData++;
			unsigned int word4 = *(pLevelData);
			pLevelData++;
			Add(uSlot, word1, word2, word3, word4);
			nCount--;
		} while (nCount != 0);
	}
	(void) nLen;
	(void) nFormat;
}
