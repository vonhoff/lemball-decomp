#include "ENGINE/CORE/VSINIT.H"
#include "LEVEL/LEVELRUNPACKET.H"

extern unsigned short LEMBALL_FASTCALL GetManagedEntitySlotIdThunk(int nManagedEntityObject);
extern void* g_pActiveManagedEntityOwner;

struct ManagedEntityPacketScoreSourceView {
	char m_abReserved00[0x64];
	unsigned int m_nScore64;
};

struct LevelVtSmallFunctionView {
	void AddLevelScoreClamped(int nValue);
};

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

// FUNCTION: LEMBALL 0x00416ca0
void ManagedEntityPacket25::WriteEffStreamTaggedHeader(void)
{
	ManagedEntityPacketBase::WriteEffStreamTaggedHeader();
	((VsNetEffStreamBase*) this)
		->WriteEffStreamU32BE(((ManagedEntityPacketScoreSourceView*) m_pManagedEntity2c)->m_nScore64);
}

// FUNCTION: LEMBALL 0x00416cc0
void ManagedEntityPacket25::ReverseEffStreamPayload(void)
{
	((VsNetEffStreamBase*) this)->ReadEffStreamU32BEValue();
	((LevelVtSmallFunctionView*) g_pActiveManagedEntityOwner)->AddLevelScoreClamped(0x9c4);
}
