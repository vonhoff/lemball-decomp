#include "ENGINE/CORE/VSINIT.H"
#include "LEVEL/LEVELRUNPACKET.H"

// Split from LEVELRUN.CPP to reproduce the original cross-translation-unit forwarding method.

// FUNCTION: LEMBALL 0x00416d00
void ManagedEntityPacket26::WriteEffStreamTaggedHeader(void)
{
	ManagedEntityPacketBase::WriteEffStreamTaggedHeader();
}

// FUNCTION: LEMBALL 0x00416d10
void ManagedEntityPacket26::ReverseEffStreamPayload(void)
{
}
