#include "AI/CInvisibleSwitchManager.h"

#include "AI/CInvisibleSwitch.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

typedef void(LEMBALL_FASTCALL* InvsChunkObjectActivateProc)(void* pObject);

struct InvisibleSwitchManagerResetView {
	unsigned char m_abReserved00[0x30];
	int m_cCapacity30;
	int m_cObjects34;
	unsigned char m_abReserved38[4];
	void* m_pObjects3C;
};

// FUNCTION: LEMBALL 0x0040a270
void CInvisibleSwitchManager::Restart(void)
{
	typedef void(LEMBALL_FASTCALL * ResetProc)(void* pObject);
	InvisibleSwitchManagerResetView* pManager;
	char* pObject;
	int i;
	int cbOffset;

	pManager = (InvisibleSwitchManagerResetView*) this;
	cbOffset = 0;
	if (pManager->m_pObjects3C != 0) {
		for (i = 0; i < pManager->m_cCapacity30; ++i) {
			pObject = (char*) pManager->m_pObjects3C + cbOffset;
			cbOffset += 0x2b8;
			((ResetProc) (*(void***) pObject)[65])(pObject);
		}
	}
}

// FUNCTION: LEMBALL 0x0040a370
void CInvisibleSwitchManager::StepOn(const AICOORD& position, CGameObject* pEntity)
{
	char* pManagerBytes;
	int iObject;

	pManagerBytes = (char*) this;
	iObject = 0;
	if (iObject < *(int*) (pManagerBytes + 0x34)) {
		int nObjectOffset;

		nObjectOffset = 0;
		do {
			CInvisibleSwitch* pObject;

			pObject = (CInvisibleSwitch*) (*(char**) (pManagerBytes + 0x3c) + nObjectOffset);
			pObject->StepOn(position, pEntity);
			nObjectOffset += 0x2b8;
			++iObject;
		} while (iObject < *(int*) (pManagerBytes + 0x34));
	}
}

// FUNCTION: LEMBALL 0x0040a3b0
void CInvisibleSwitchManager::Process(void)
{
	char* pManagerBytes;
	void* pChunkObject;
	int i;
	int nOffset;

	pManagerBytes = (char*) this;
	i = 0;
	if (*(int*) (pManagerBytes + 0x34) > 0) {
		nOffset = 0;
		do {
			pChunkObject = *(char**) (pManagerBytes + 0x3c) + nOffset;
			++i;
			nOffset += 0x2b8;
			((InvsChunkObjectActivateProc) (*(void***) pChunkObject)[5])(pChunkObject);
		} while (*(int*) (pManagerBytes + 0x34) > i);
	}
}
