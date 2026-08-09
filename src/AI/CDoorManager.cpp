#include "AI/CDoorManager.h"

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
