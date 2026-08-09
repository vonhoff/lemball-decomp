#include "AI/CDoorManager.h"

#include "AI/CGameObject.h"
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

// FUNCTION: LEMBALL 0x0040e080
int CDoorManager::GetViewData(CViewData* pViewData)
{
	int nObjectOffset = 0;
	int iObject = 0;
	if (m_nObjectCount34 > 0) {
		do {
			int* pObjectWords = (int*) ((char*) m_pObjects3C + nObjectOffset);
			nObjectOffset = nObjectOffset + 0x14c;
			iObject = iObject + 1;
			(*( void(**)(int)) (*(void***) *pObjectWords + 0xc / 4))((int) (void*) pViewData);
			pViewData = (CViewData*) ((char*) pViewData + 0x4c);
		} while (iObject < m_nObjectCount34);
	}
	return m_nObjectCount34;
}
// FUNCTION: LEMBALL 0x0040e140
void CDoorManager::Remove(CDoor* pDoor)
{
	unsigned short nSlot;
	char* pDestination;
	char* pObject;
	char* pSource;
	int i;
	int nOffset;
	int nSourceIndex;

	i = 0;
	nSlot = GetManagedEntitySlotIdThunk((int) (unsigned long) pDoor);
	if (m_nObjectCount34 <= 0) {
		return;
	}
	nOffset = 0;
	do {
		pObject = (char*) m_pObjects3C + nOffset;
		if (GetManagedEntitySlotIdThunk((int) (unsigned long) pObject) == nSlot) {
			break;
		}
		nOffset += 0x14c;
		++i;
		if (m_nObjectCount34 <= i) {
			return;
		}
	} while (1);

	pObject = (char*) m_pObjects3C + i * 0x14c;
	((void(LEMBALL_FASTCALL*)(void*))(*(void***) pObject)[50])(pObject);
	((CGameObject*) pObject)->SetId(0xffff);
	nSourceIndex = i + 1;
	if (nSourceIndex < m_nObjectCount34) {
		nOffset = nSourceIndex * 0x14c;
		do {
			pSource = (char*) m_pObjects3C + nOffset;
			pDestination = pSource - 0x14c;

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
			*(short*) (pDestination + 0x138) = *(short*) (pSource + 0x138);
			*(int*) (pDestination + 0x13c) = *(int*) (pSource + 0x13c);
			*(short*) (pDestination + 0x140) = *(short*) (pSource + 0x140);
			*(int*) (pDestination + 0x144) = *(int*) (pSource + 0x144);
			*(short*) (pDestination + 0x148) = *(short*) (pSource + 0x148);

			++nSourceIndex;
			nOffset += 0x14c;
		} while (nSourceIndex < m_nObjectCount34);
	}
	--m_nObjectCount34;
}

// FUNCTION: LEMBALL 0x0040e550
void CDoorManager::Process(void)
{
	int iObject = 0;
	if (m_nObjectCount34 > 0) {
		int nObjectOffset = 0;
		do {
			*(int*) ((char*) m_pObjects3C + 0x124 + nObjectOffset) = 1;
			void* pObject = (void*) ((char*) m_pObjects3C + nObjectOffset);
			if (*(int*) ((char*) pObject + 0x51 * 4) != 0 || *(int*) ((char*) pObject + 0x45 * 4) != 0) {
				(*( void(**)(void)) (*(void***) pObject + 0x14 / 4))();
			}
			nObjectOffset = nObjectOffset + 0x14c;
			iObject = iObject + 1;
		} while (iObject < m_nObjectCount34);
	}
}


// FUNCTION: LEMBALL 0x0040e060
CDoorManager::~CDoorManager(void)
{
	*(int*) this = 0x493840;
	if (*(void**) ((char*) this + 0x3c) != 0) {
		((void(__fastcall*)(void*, int)) (*(void***) ((char*) this + 0x3c))[0])(
			*(void**) ((char*) this + 0x3c), 3);
	}
	((void(__fastcall*)(void*)) 0x45eea0)(this);
}
