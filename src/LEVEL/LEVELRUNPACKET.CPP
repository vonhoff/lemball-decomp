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

struct ManagedEntityPacketStreamView {
	void BindManagedEntityPacketTarget(void* pEntity);
};

struct ManagedEntityPacketPosition {
	int m_nX;
	int m_nY;
	int m_nZ;
};

struct ManagedEntityStateRequestTargetView {
	virtual void ReservedSlot0(void) = 0;
	virtual void ReservedSlot1(void) = 0;
	virtual void ApplyManagedEntityStateId(int nStateId) = 0;
	virtual void ReservedSlot3(void) = 0;
	virtual void ReservedSlot4(void) = 0;
	virtual void ReservedSlot5(void) = 0;
	virtual void ReservedSlot6(void) = 0;
	virtual void ReservedSlot7(void) = 0;
	virtual void ReservedSlot8(void) = 0;
	virtual void ReservedSlot9(void) = 0;
	virtual void ReservedSlot10(void) = 0;
	virtual void ReservedSlot11(void) = 0;
	virtual void ReservedSlot12(void) = 0;
	virtual void ReservedSlot13(void) = 0;
	virtual void ReservedSlot14(void) = 0;
	virtual void ReservedSlot15(void) = 0;
	virtual void ReservedSlot16(void) = 0;
	virtual void ReservedSlot17(void) = 0;
	virtual void ReservedSlot18(void) = 0;
	virtual void ReservedSlot19(void) = 0;
	virtual void ReservedSlot20(void) = 0;
	virtual void ReservedSlot21(void) = 0;
	virtual void ReservedSlot22(void) = 0;
	virtual void ReservedSlot23(void) = 0;
	virtual void ReservedSlot24(void) = 0;
	virtual void ReservedSlot25(void) = 0;
	virtual void ReservedSlot26(void) = 0;
	virtual void ReservedSlot27(void) = 0;
	virtual void ReservedSlot28(void) = 0;
	virtual void ReservedSlot29(void) = 0;
	virtual void ReservedSlot30(void) = 0;
	virtual void ReservedSlot31(void) = 0;
	virtual void ReservedSlot32(void) = 0;
	virtual void ReservedSlot33(void) = 0;
	virtual void ReservedSlot34(void) = 0;
	virtual void ReservedSlot35(void) = 0;
	virtual void ReservedSlot36(void) = 0;
	virtual void ReservedSlot37(void) = 0;
	virtual void ReservedSlot38(void) = 0;
	virtual void ReservedSlot39(void) = 0;
	virtual void ReservedSlot40(void) = 0;
	virtual void ReservedSlot41(void) = 0;
	virtual void ReservedSlot42(void) = 0;
	virtual void ReservedSlot43(void) = 0;
	virtual void ReservedSlot44(void) = 0;
	virtual void ReservedSlot45(void) = 0;
	virtual void ReservedSlot46(void) = 0;
	virtual void ReservedSlot47(void) = 0;
	virtual void ReservedSlot48(void) = 0;
	virtual void ReservedSlot49(void) = 0;
	virtual void ReservedSlot50(void) = 0;
	virtual void ReservedSlot51(void) = 0;
	virtual void ReservedSlot52(void) = 0;
	virtual void ReservedSlot53(void) = 0;
	virtual void ReservedSlot54(void) = 0;
	virtual void ReservedSlot55(void) = 0;
	virtual void ReservedSlot56(void) = 0;
	virtual void ReservedSlot57(void) = 0;
	virtual void ReservedSlot58(void) = 0;
	virtual void ReservedSlot59(void) = 0;
	virtual void ReservedSlot60(void) = 0;
	virtual void ReservedSlot61(void) = 0;
	virtual void ReservedSlot62(void) = 0;
	virtual void ReservedSlot63(void) = 0;
	virtual int CanApplyRequestedState(int nStateId) = 0;
	virtual void ReservedSlot65(void) = 0;
	virtual void ReservedSlot66(void) = 0;
	virtual void CommitManagedEntityState(void) = 0;

	char m_abReserved04[0x34];
	unsigned int m_nPositionSequence38;
	char m_abReserved3c[0x50];
	int m_fStateUpdatePending8c;
	char m_abReserved90[0x0c];
	int m_nPositionX9c;
	int m_nPositionYa0;
	int m_nPositionZa4;
	char m_abReserveda8[0x10];
	int m_nRequestedStateIdb8;
	char m_abReservedbc[0x58];
	int m_fStateTransitionActive114;
	char m_abReserved118[0x14];
	unsigned int m_nSerializedState12c;
	int m_fStateRequestAccepted130;
	int m_fStateTransitionStatus134;
};

// GLOBAL: LEMBALL 0x0049d120
ManagedEntityPacketStreamView* g_pManagedEntityStateResponseStream = 0;

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

// FUNCTION: LEMBALL 0x00416a30
void ManagedEntityPacket27::WriteEffStreamTaggedHeader(void)
{
	ManagedEntityPacketBase::WriteEffStreamTaggedHeader();
	((VsNetEffStreamBase*) this)
		->WriteEffStreamU32BE(((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_nSerializedState12c);
}

// FUNCTION: LEMBALL 0x00416a50
void ManagedEntityPacket27::ReverseEffStreamPayload(void)
{
	int fAccepted;

	((VsNetEffStreamBase*) this)->ReadEffStreamU32BEValue();
	if (((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_fStateTransitionActive114 == 0 &&
		((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_fStateUpdatePending8c == 0 &&
		((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)
				->CanApplyRequestedState(
					((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_nRequestedStateIdb8) != 0) {
		fAccepted = 1;
	}
	else {
		fAccepted = 0;
	}
	((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_fStateRequestAccepted130 = fAccepted;
	((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_fStateTransitionActive114 = fAccepted;
	if (fAccepted != 0) {
		((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_fStateUpdatePending8c = 1;
	}
	g_pManagedEntityStateResponseStream->BindManagedEntityPacketTarget(m_pManagedEntity2c);
}

// FUNCTION: LEMBALL 0x00416ae0
void ManagedEntityPacket28::WriteEffStreamTaggedHeader(void)
{
	ManagedEntityPacketBase::WriteEffStreamTaggedHeader();
	((VsNetEffStreamBase*) this)
		->WriteEffStreamU32BE(((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_fStateRequestAccepted130);
}

// FUNCTION: LEMBALL 0x00416b00
void ManagedEntityPacket28::ReverseEffStreamPayload(void)
{
	((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_fStateRequestAccepted130 =
		((VsNetEffStreamBase*) this)->ReadEffStreamU32BEValue();
	if (((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_fStateRequestAccepted130 != 0) {
		((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_nRequestedStateIdb8 =
			((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_nSerializedState12c;
		((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->CommitManagedEntityState();
		((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)
			->ApplyManagedEntityStateId(
				((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_nSerializedState12c);
		((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_fStateTransitionStatus134 = 2;
	}
	else {
		((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_fStateTransitionStatus134 = 1;
	}
	((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_nSerializedState12c = 0x18;
}

// FUNCTION: LEMBALL 0x00416bd0
void ManagedEntityPacket24::WriteEffStreamTaggedHeader(void)
{
	ManagedEntityPacketBase::WriteEffStreamTaggedHeader();
	((VsNetEffStreamBase*) this)
		->WriteEffStreamU32BE(((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_nPositionX9c >> 12);
	((VsNetEffStreamBase*) this)
		->WriteEffStreamU32BE(((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_nPositionYa0 >> 12);
	((VsNetEffStreamBase*) this)
		->WriteEffStreamU32BE(((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_nPositionZa4 >> 12);
	((VsNetEffStreamBase*) this)
		->WriteEffStreamU32BE(((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_nPositionSequence38);
}

// FUNCTION: LEMBALL 0x00416c30
void ManagedEntityPacket24::ReverseEffStreamPayload(void)
{
	ManagedEntityPacketPosition position;

	position.m_nX = ((VsNetEffStreamBase*) this)->ReadEffStreamU32BEValue() << 12;
	((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_nPositionX9c = position.m_nX;
	position.m_nY = ((VsNetEffStreamBase*) this)->ReadEffStreamU32BEValue() << 12;
	((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_nPositionYa0 = position.m_nY;
	position.m_nZ = ((VsNetEffStreamBase*) this)->ReadEffStreamU32BEValue() << 12;
	((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_nPositionZa4 = position.m_nZ;
	((ManagedEntityStateRequestTargetView*) m_pManagedEntity2c)->m_nPositionSequence38 =
		((VsNetEffStreamBase*) this)->ReadEffStreamU32BEValue();
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
