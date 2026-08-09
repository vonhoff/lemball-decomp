#include "AI/CPaintGunManager.h"
#include "AI/CPaintGun.h"
#include "Visos/Generic/Memory.h"

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
	typedef unsigned int(__fastcall* FindFreeSlotProc)(void);
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
				(*( void(**)(void)) (*(void***) pObject + 0x14 / 4))();
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
				((void(__fastcall*)(void*, CViewData*)) (*(void***) pChild)[0xc / 4])(pChild, pViewData);
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
		((void(__fastcall*)(void*, int)) (*(void***) ((char*) this + 0x38))[0])(
			*(void**) ((char*) this + 0x38), 3);
	}
	((void(__fastcall*)(void*)) 0x45eea0)(this);
}
