#include "AI/CRocketManager.h"

// FUNCTION: LEMBALL 0x00427050
int CRocketManager::GetViewData(CViewData* pViewData)
{
	int iVar2 = 0;
	int local_4 = 0;
	if (m_nObjectCount34 > 0) {
		int iVar3 = 0;
		do {
			int* piVar1 = (int*) (m_pObjects38 + iVar3);
			int iVar4 = (int) pViewData;
			if (piVar1[0x2e] != 0x18) {
				iVar4 = (int) pViewData + 0x4c;
				(*( void(**)(int)) (*(void***) *piVar1 + 0xc / 4))((int) pViewData);
				local_4 = local_4 + 1;
			}
			iVar3 = iVar3 + 0x144;
			iVar2 = iVar2 + 1;
			pViewData = (CViewData*) iVar4;
		} while (iVar2 < m_nObjectCount34);
	}
	return local_4;
}
// FUNCTION: LEMBALL 0x00427010
void CRocketManager::Process(void)
{
	int iVar3 = 0;
	if (m_nObjectCount34 > 0) {
		int iVar2 = 0;
		do {
			*(int*) (m_pObjects38 + 0x124 + iVar2) = 1;
			void* piVar1 = (void*) (m_pObjects38 + iVar2);
			(*( void(**)(void)) (*(void***) piVar1 + 0x14 / 4))();
			iVar2 = iVar2 + 0x144;
			iVar3 = iVar3 + 1;
		} while (iVar3 < m_nObjectCount34);
	}
}
// FUNCTION: LEMBALL 0x004270b0
void CRocketManager::Add(unsigned short nSlotId, int nWorldX, int nWorldY, int nWorldZ)
{
	if (m_nObjectCount34 < m_nCapacity30) {
		int vec[3];
		vec[0] = nWorldX << 12;
		vec[1] = nWorldY << 12;
		vec[2] = nWorldZ << 12;
		((void(__cdecl*)(void*, unsigned short, void*)) 0x402707)(m_pObjects38 + m_nObjectCount34 * 0x144, nSlotId, vec);
		m_nObjectCount34 = m_nObjectCount34 + 1;
	}
}
// FUNCTION: LEMBALL 0x00426fb0
int CRocketManager::StepOn(const AICOORD& position, CGameObject* pObject)
{
	int i;
	for (i = 0; i < m_nObjectCount34; i++) {
		void* pElem = (void*) (m_pObjects38 + i * 0x144);
		if (*(int*) ((char*) pElem + 0x13c) != 0 && *(int*) ((char*) pElem + 0xb8) == 0x18 && *(int*) ((char*) pElem + 0x12c) == 0x18) {
			if (((int(__cdecl*)(void*, void*, int)) 0x401375)(pElem, (void*) &position, (int) pObject) != 0) {
				return 1;
			}
		}
	}
	return 0;
}
// FUNCTION: LEMBALL 0x00427110
void CRocketManager::LoadLevel(unsigned short* pLevelData, int nLen, unsigned char nFormat)
{
	unsigned short nCount = *pLevelData;
	unsigned int nRemain = (unsigned int) nCount;
	pLevelData++;
	((void(__fastcall*) (void*, int)) 0x403760)(this, nCount);
	if (nCount != 0) {
		do {
			unsigned short uSlot;
			if (*(unsigned short*) ((char*) m_pLevelMode3C + 0x54) < 2) {
				uSlot = (unsigned short) ((int(__fastcall*) ()) 0x40214e)();
			}
			else {
				uSlot = *pLevelData;
				pLevelData++;
			}
			unsigned int x = *(pLevelData);
			pLevelData++;
			unsigned int y = *(pLevelData);
			pLevelData++;
			unsigned int z = *(pLevelData);
			pLevelData++;
			((void(__fastcall*) (void*, unsigned short, int, int, int)) 0x4017b7)(this, uSlot, x, y, z);
			nRemain--;
		} while (nRemain != 0);
	}
	(void) nLen;
	(void) nFormat;
}
