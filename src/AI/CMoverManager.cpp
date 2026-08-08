#include "AI/CMoverManager.h"
#include "AI/CMover.h"

// FUNCTION: LEMBALL 0x0042f5e0
void CMoverManager::Add(unsigned short nSlotId, unsigned short nFlags, void* pParam3, int nParam4, void* pParam5)
{
	if (m_nCapacity30 > 0) {
		((void(__fastcall*)(void*, unsigned short, unsigned short, void*, int, void*)) 0x40128a)((void*) (m_pObjects38 + m_cObjects34), nSlotId, nFlags, pParam3, nParam4, pParam5);
		m_cObjects34++;
	}
}

// FUNCTION: LEMBALL 0x0042f540
int CMoverManager::GetViewData(CViewData* pViewData)
{
	return ((int(__fastcall*)(void*, CViewData*)) 0x42e590)(this, pViewData);
}

// FUNCTION: LEMBALL 0x0042f680
void CMoverManager::LoadLevel(unsigned short* pLevelData, int nLen, unsigned char nFormat)
{
	unsigned short nCount = *pLevelData;
	((void(__fastcall*) (void*, int)) 0x401811)(this, nCount);
	unsigned int i = 1;
	unsigned short uSlot, uFlags, routeSel, routeCount;
	unsigned int flagOverride;
	while (i < nCount) {
		uSlot = (unsigned int) ((int(__fastcall*) ()) 0x40214e)();
		uFlags = pLevelData[i++];
		flagOverride = (pLevelData[i] & 0x30) != 0;
		routeSel = pLevelData[i++];
		routeCount = pLevelData[i++];
		((void(__fastcall*) (void*, int, int, int, int, int)) 0x401f55)(this, uSlot, uFlags, flagOverride, routeSel, routeCount);
	}
}
