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

// FUNCTION: LEMBALL 0x0042c520
int __fastcall CPaintGunManager_GetViewData(void* pThis, int nUnused, int param_1)
{
	int local_4 = 0;
	int iVar2 = 0;
	if (*(int*) ((char*) pThis + 0x34) > 0) {
		int iVar3 = 0;
		do {
			int* piVar1 = (int*) (*(int*) ((char*) pThis + 0x38) + iVar3);
			int iVar4 = param_1;
			if (piVar1[0x4e] != 0) {
				iVar4 = param_1 + 0x4c;
				(*( void(**)(int)) (*(void***) *piVar1 + 0xc / 4))(param_1);
				local_4 = local_4 + 1;
			}
			iVar3 = iVar3 + 0x144;
			iVar2 = iVar2 + 1;
			param_1 = iVar4;
		} while (iVar2 < *(int*) ((char*) pThis + 0x34));
	}
	return local_4;
}
// FUNCTION: LEMBALL 0x0042c4d0
void __fastcall CPaintGunManager_Process(void* param_1)
{
	int iVar3 = 0;
	if (*(int*) ((char*) param_1 + 0x34) > 0) {
		int iVar2 = 0;
		do {
			*(int*) (*(int*) ((char*) param_1 + 0x38) + 0x124 + iVar2) = 1;
			void* piVar1 = (void*) (*(int*) ((char*) param_1 + 0x38) + iVar2);
			if (*(int*) ((char*) piVar1 + 0x4e * 4) != 0) {
				(*( void(**)(void)) (*(void***) piVar1 + 0x14 / 4))();
			}
			iVar2 = iVar2 + 0x144;
			iVar3 = iVar3 + 1;
		} while (iVar3 < *(int*) ((char*) param_1 + 0x34));
	}
}
