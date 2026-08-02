#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);

// Split from the original LINKSCF source group to preserve MSVC 4.20 code generation.

// MACINTOSH: CLift::Activate()
// FUNCTION: LEMBALL 0x00425640
int LEMBALL_FASTCALL RequestLiftChunkObjectActivationState(void* pObject)
{
	typedef void(__stdcall * SetStateProc)(int);
	*(int*) ((char*) pObject + 0x164) = 1;
	((SetStateProc) (*(void***) pObject)[2])(0x19);
	return 1;
}

// MACINTOSH: CLift::ActivateDeactivate()
// FUNCTION: LEMBALL 0x00425660
void LEMBALL_FASTCALL ToggleLiftChunkObjectActivationState(void* pObject)
{
	typedef void(__stdcall * SetStateProc)(int);
	if (*(int*) ((char*) pObject + 0x164) == 0) {
		RequestLiftChunkObjectActivationState(pObject);
	}
	else {
		((SetStateProc) (*(void***) pObject)[2])(8);
	}
}

// MACINTOSH: CLiftManager::Switch(swMessage, int, int, int)
// FUNCTION: LEMBALL 0x00425f10
void LEMBALL_FASTCALL DispatchLiftChunkObjectActionBySlot(void* pManager,
														  void* pUnusedEdx,
														  int nAction,
														  unsigned int nSlotId,
														  int nUnused3,
														  int nUnused4)
{
	char* pManagerBytes = (char*) pManager;
	int iObject = 0;
	int nOffset = 0;
	(void) pUnusedEdx;
	(void) nUnused3;
	(void) nUnused4;

	if (*(int*) (pManagerBytes + 0x34) > 0) {
		while (GetManagedEntitySlotIdThunk((int) (unsigned long) (*(char**) (pManagerBytes + 0x3c) + nOffset)) !=
			   nSlotId) {
			nOffset += 0x190;
			++iObject;
			if (*(int*) (pManagerBytes + 0x34) <= iObject) {
				return;
			}
		}
		if (nAction == 1) {
			char* pObject = *(char**) (pManagerBytes + 0x3c) + iObject * 0x190;
			if (*(int*) (pObject + 0x15c) != 3) {
				if (*(int*) (pObject + 0x15c) == 0) {
					ToggleLiftChunkObjectActivationState(pObject);
					return;
				}
			}
			else if (*(int*) (pObject + 0x16c) == 0) {
				RequestLiftChunkObjectActivationState(pObject);
			}
		}
	}
}
