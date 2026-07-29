#include "ENGINE/CORE/VSINIT.H"
#include "LEVEL/LEVELRUNPACKET.H"

// Split from LEVELRUN.CPP to reproduce the original cross-translation-unit forwarding thunk.

// FUNCTION: LEMBALL 0x00416d00
void LEMBALL_FASTCALL SerializeManagedEntityTypeAndSlotIdThunk(void* pObject)
{
	((ManagedEntityPacketBase*) pObject)->ManagedEntityPacketBase::WriteEffStreamTaggedHeader();
}
