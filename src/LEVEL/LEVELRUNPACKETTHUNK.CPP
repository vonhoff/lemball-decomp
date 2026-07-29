#include "ENGINE/CORE/VSINIT.H"

void LEMBALL_FASTCALL SerializeManagedEntityTypeAndSlotId(void* pObject);

// Split from LEVELRUN.CPP to reproduce the original cross-translation-unit forwarding thunk.

// FUNCTION: LEMBALL 0x00416d00
void LEMBALL_FASTCALL SerializeManagedEntityTypeAndSlotIdThunk(void* pObject)
{
	SerializeManagedEntityTypeAndSlotId(pObject);
}
