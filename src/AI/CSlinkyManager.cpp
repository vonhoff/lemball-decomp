#include "AI/CSlinkyManager.h"

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
		((void(__fastcall*)(void*, unsigned short)) 0x402293)((void*) (m_pObjects04 + m_nObjectCount0C * 0x150), (unsigned short) nSlotId);
		((void(__fastcall*)(void*, int, int, int, int)) 0x402892)(
			(void*) (m_pObjects04 + m_nObjectCount0C * 0x150),
			n2, n4, n3, n5);
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
