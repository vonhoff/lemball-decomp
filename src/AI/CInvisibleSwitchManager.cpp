#include "AI/CInvisibleSwitchManager.h"

#include "AI/CInvisibleSwitch.h"
#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

typedef void(LEMBALL_FASTCALL* InvsChunkObjectActivateProc)(void* pObject);

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
