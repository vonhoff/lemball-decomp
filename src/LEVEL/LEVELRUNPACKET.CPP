#include "ENGINE/CORE/VSINIT.H"
#include "LEVEL/LEVELRUNPACKET.H"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);

// Split from LEVELRUN.CPP to preserve compiler state in the original translation unit.

// FUNCTION: LEMBALL 0x004168a0
void ManagedEntityPacketBase::WriteEffStreamTaggedHeader(void)
{
	((VsNetEffStreamBase*) this)->WriteEffStreamU16BE(m_nReserved04);
	((VsNetEffStreamBase*) this)
		->WriteEffStreamU16BE(GetManagedEntitySlotIdThunk((int) (unsigned long) m_pManagedEntity2c));
}

// FUNCTION: LEMBALL 0x004168c0
void ManagedEntityPacketBase::ReverseEffStreamPayload(void)
{
}
