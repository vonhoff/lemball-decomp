#include "AI/CTrampolineManager.h"
#include "AI/CTrampoline.h"
#include "Visos/Generic/Memory.h"

// Mac blueprint: CTrampolineManager::Add(ushort, int, int, int)

// FUNCTION: LEMBALL 0x0042b5a0
void CTrampolineManager::Add(unsigned short nSlot, int nWorldX, int nWorldY, int nWorldZ)
{
	int nLocal[3];

	if (m_nObjectCount34 < m_nCapacity30) {
		nLocal[0] = nWorldX << 12;
		nLocal[1] = nWorldY << 12;
		nLocal[2] = nWorldZ << 12;
		((CTrampoline*) (m_pObjects38 + m_nObjectCount34 * 0x144))->Set(nSlot, nLocal);
		m_nObjectCount34 = m_nObjectCount34 + 1;
	}
}

// Mac blueprint: CTrampolineManager::LoadLevel(ushort*, int, uchar)
// FUNCTION: LEMBALL 0x0042b600
void CTrampolineManager::LoadLevel(unsigned short* pLevelData, int nLen, unsigned char nFormat)
{
	typedef unsigned int(__fastcall* FindFreeSlotProc)(void);
	unsigned int nCount = (unsigned int) *pLevelData;
	unsigned short nSlot;
	int nX;
	int nY;
	int nZ;

	pLevelData = pLevelData + 1;
	((void(__fastcall*)(void*, unsigned int)) 0x4019f6)(this, nCount);
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
		Add(nSlot, nX, nY, nZ);
		nCount = nCount - 1;
	}
}

// FUNCTION: LEMBALL 0x0042b4f0
int CTrampolineManager::GetViewData(int param_1)
{
	int local_4 = 0;
	int iVar2 = 0;
	if (m_nObjectCount34 > 0) {
		int iVar3 = 0;
		do {
			int* piVar1 = (int*) (m_pObjects38 + iVar3);
			int iVar4 = param_1;
			if (piVar1[0x4e] != 0) {
				iVar4 = param_1 + 0x4c;
				(*( void(**)(int)) (*(void***) *piVar1 + 0xc / 4))(param_1);
				local_4 = local_4 + 1;
			}
			iVar3 = iVar3 + 0x144;
			iVar2 = iVar2 + 1;
			param_1 = iVar4;
		} while (iVar2 < m_nObjectCount34);
	}
	return local_4;
}
