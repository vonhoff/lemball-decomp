#include "ENGINE/CORE/VSINIT.H"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);

struct ManagedEntityStateView {
	void RequestManagedEntityStateId(int nStateId);
};

struct DoorChunkObjectActionView : public ManagedEntityStateView {
	void RequestDoorChunkObjectTriggerTransition(void);
};

// Split from the original LINKSCF source group to preserve MSVC 4.20 code generation.

// FUNCTION: LEMBALL 0x0040dd00
void DoorChunkObjectActionView::RequestDoorChunkObjectTriggerTransition(void)
{
	typedef void(__stdcall * SetTriggerProc)(int);
	char* pObject = (char*) this;
	int nStateId = *(int*) (pObject + 0xb8);
	if (nStateId >= 0x1c && nStateId <= 0x1d) {
		*(int*) (pObject + 0xcc) = 0x14;
		((SetTriggerProc) (*(void***) pObject)[13])(0xb);
		RequestManagedEntityStateId(0x20);
	}
}

// FUNCTION: LEMBALL 0x0040e5a0
void LEMBALL_FASTCALL DispatchDoorChunkObjectTriggerBySlot(void* pManager,
														   void* pUnused,
														   int nAction,
														   unsigned int nSlot)
{
	char* pManagerBytes = (char*) pManager;
	int iObject = 0;
	int nObjectOffset;
	(void) pUnused;

	if (*(int*) (pManagerBytes + 0x34) > 0) {
		nObjectOffset = 0;
		while (GetManagedEntitySlotIdThunk((int) (unsigned long) (*(char**) (pManagerBytes + 0x3c) + nObjectOffset)) !=
			   nSlot) {
			nObjectOffset += 0x14c;
			++iObject;
			if (*(int*) (pManagerBytes + 0x34) <= iObject) {
				return;
			}
		}
		if (nAction == 3) {
			((DoorChunkObjectActionView*) (*(char**) (pManagerBytes + 0x3c) + iObject * 0x14c))
				->RequestDoorChunkObjectTriggerTransition();
		}
	}
}
