#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);
extern void LEMBALL_FASTCALL set_move_chunk_object_trigger_latch(void* pObject);

// Split from the original LEVELVT source group to preserve MSVC 4.20 code generation.

// FUNCTION: LEMBALL 0x0042f620
void LEMBALL_FASTCALL DispatchMoveChunkObjectActionBySlot(void* pManager,
														  void* pUnused,
														  int nAction,
														  unsigned int nSlotId)
{
	char* pManagerBytes = (char*) pManager;
	unsigned int nTargetSlotId = nSlotId;
	int iObject = 0;
	(void) pUnused;
	if (*(int*) (pManagerBytes + 0x34) > 0) {
		int nOffset = 0;
		while (1) {
			if (GetManagedEntitySlotIdThunk((int) (unsigned long) (*(char**) (pManagerBytes + 0x38) + nOffset)) ==
				nTargetSlotId) {
				break;
			}
			nOffset += 0x1a0;
			++iObject;
			if (*(int*) (pManagerBytes + 0x34) <= iObject) {
				return;
			}
		}
		if (nAction == 4) {
			set_move_chunk_object_trigger_latch(*(char**) (pManagerBytes + 0x38) + iObject * 0x1a0);
		}
	}
}
