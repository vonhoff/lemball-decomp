#include "AI/CDoorManager.h"

// FUNCTION: LEMBALL 0x0040e080
int CDoorManager::GetViewData(CViewData* pViewData)
{
	int iVar2 = 0;
	int iVar3 = 0;
	if (m_nObjectCount34 > 0) {
		do {
			int* piVar1 = (int*) ((char*) m_pObjects3C + iVar2);
			iVar2 = iVar2 + 0x14c;
			iVar3 = iVar3 + 1;
			(*( void(**)(int)) (*(void***) *piVar1 + 0xc / 4))((int) (void*) pViewData);
			pViewData = (CViewData*) ((char*) pViewData + 0x4c);
		} while (iVar3 < m_nObjectCount34);
	}
	return m_nObjectCount34;
}
// FUNCTION: LEMBALL 0x0040e550
void CDoorManager::Process(void)
{
	int iVar3 = 0;
	if (m_nObjectCount34 > 0) {
		int iVar2 = 0;
		do {
			*(int*) ((char*) m_pObjects3C + 0x124 + iVar2) = 1;
			void* piVar1 = (void*) ((char*) m_pObjects3C + iVar2);
			if (*(int*) ((char*) piVar1 + 0x51 * 4) != 0 || *(int*) ((char*) piVar1 + 0x45 * 4) != 0) {
				(*( void(**)(void)) (*(void***) piVar1 + 0x14 / 4))();
			}
			iVar2 = iVar2 + 0x14c;
			iVar3 = iVar3 + 1;
		} while (iVar3 < m_nObjectCount34);
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
