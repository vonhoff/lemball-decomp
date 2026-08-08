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
	int iVar3 = 0;
	if (m_nObjectCount34 > 0) {
		int iVar2 = 0;
		do {
			*(int*) ((char*) m_pObjects38 + 0x124 + iVar2) = 1;
			void* piVar1 = (void*) ((char*) m_pObjects38 + iVar2);
			if (*(int*) ((char*) piVar1 + 0x4e * 4) != 0) {
				(*( void(**)(void)) (*(void***) piVar1 + 0x14 / 4))();
			}
			iVar2 = iVar2 + 0x144;
			iVar3 = iVar3 + 1;
		} while (iVar3 < m_nObjectCount34);
	}
}

// Mac blueprint: CPaintGunManager::GetViewData(CViewData*)
// FUNCTION: LEMBALL 0x0042c520
int CPaintGunManager::GetViewData(CViewData* pViewData)
{
	int nCount = 0;
	int iChild = 0;
	if (m_nObjectCount34 > 0) {
		int iVar2 = 0;
		do {
			void* pChild = (void*) ((char*) m_pObjects38 + iVar2);
			if (*(int*) ((char*) pChild + 0x138) != 0) {
				((void(__fastcall*)(void*, CViewData*)) (*(void***) pChild)[0xc / 4])(pChild, pViewData);
				nCount = nCount + 1;
				pViewData = (CViewData*) ((char*) pViewData + 0x4c);
			}
			iVar2 = iVar2 + 0x144;
			iChild = iChild + 1;
		} while (iChild < m_nObjectCount34);
	}
	return nCount;
}
