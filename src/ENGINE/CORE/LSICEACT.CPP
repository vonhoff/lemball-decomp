#include "ENGINE/CORE/VSINIT.H"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);

struct ManagedEntityStateView {
	void RequestManagedEntityStateId(int nStateId);
};

struct IceChunkObjectActionView : public ManagedEntityStateView {
	void RequestIceChunkObjectToggleActivation(void);
};

// Split from the original LINKSCF source group to preserve MSVC 4.20 code generation.

// FUNCTION: LEMBALL 0x0042d550
void IceChunkObjectActionView::RequestIceChunkObjectToggleActivation(void)
{
	RequestManagedEntityStateId(0x1a);
}

// FUNCTION: LEMBALL 0x0042dd90
void LEMBALL_FASTCALL DispatchIceChunkObjectActionBySlot(void* pManager, void* pUnused, int nAction, unsigned int nSlot)
{
	char* pManagerBytes = (char*) pManager;
	int iObject = 0;
	int nObjectOffset;
	(void) pUnused;

	if (*(int*) (pManagerBytes + 0x34) > 0) {
		nObjectOffset = 0;
		while (GetManagedEntitySlotIdThunk((int) (unsigned long) (*(char**) (pManagerBytes + 0x38) + nObjectOffset)) !=
			   nSlot) {
			nObjectOffset += 0x188;
			++iObject;
			if (*(int*) (pManagerBytes + 0x34) <= iObject) {
				return;
			}
		}
		if (nAction == 5) {
			((IceChunkObjectActionView*) (*(char**) (pManagerBytes + 0x38) + iObject * 0x188))
				->RequestIceChunkObjectToggleActivation();
		}
	}
}
