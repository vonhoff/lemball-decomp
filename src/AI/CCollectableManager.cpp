#include "Platform/Windows/Mixed/Engine/CORE/LINKSCF.H"

// FUNCTION: LEMBALL 0x00422420
void CCollectableManager::Restart(void)
{
	if (m_apObjects34 != 0 && m_nObjectCount38 > 0) {
		int i;
		for (i = 0; i < m_nObjectCount38; i++) {
			void* pElem = m_apObjects34[i];
			if (pElem != 0) {
				(*( void(**)(void)) (*(void***) pElem + 0x104 / 4))();
			}
		}
	}
}
// FUNCTION: LEMBALL 0x00422790
void CCollectableManager::LoadLevel(unsigned short* pLevelData, int nFormat)
{
	unsigned short nCount = *pLevelData;
	pLevelData++;
	if (nFormat == 0) {
		((void(__fastcall*) (void*, int, int)) 0x40187f)(this, 0, nCount);
	}
	if (nCount != 0) {
		do {
			unsigned short uSlot;
			if (*(unsigned short*) ((char*) m_pLevelMode30 + 0x54) < 2) {
				uSlot = (unsigned short) ((int(__fastcall*) ()) 0x40214e)();
			}
			else {
				uSlot = *pLevelData;
				pLevelData++;
			}
			unsigned int subtype = *(pLevelData);
			pLevelData++;
			unsigned int coord2 = *(pLevelData);
			pLevelData++;
			unsigned int coord3 = *(pLevelData);
			pLevelData++;
			unsigned int coord4 = *(pLevelData);
			pLevelData++;
			int appendType = (int) subtype;
			if (nFormat == 0) {
				void* pBase = m_pLevelMode30;
				if ((subtype == 0xc || subtype == 0xb) && *(int*) ((char*) pBase + 0x64) == 1) {
					if (*(int*) ((char*) pBase + 0x7c) == 1) {
						if (subtype == 0xc) appendType = 0xc; else appendType = 0xb;
					}
					else {
						if (subtype == 0xc) appendType = 0xb; else appendType = 0xc;
					}
				}
			}
			((void(__fastcall*) (void*, int, int, int, int, int, int)) 0x401afa)(m_pLevelMode30, 0, uSlot, coord2, coord3, coord4, appendType);
			nCount--;
		} while (nCount != 0);
	}
}
