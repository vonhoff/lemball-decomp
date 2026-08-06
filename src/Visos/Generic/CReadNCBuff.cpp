// Recovered Macintosh logical owner: CReadNCBuff.
// Included by the original Windows translation units to preserve ABI and link order.

#ifdef LEMBALL_CREADNCBUFF_VSNET

// FUNCTION: LEMBALL 0x00461560
void* ConstructShiftedLockedRecordSlotTable(void* pObject, int cPacketIds, unsigned int cbRecord)
{
	((CWritePacketTable*) pObject)
		->ConstructLockedEffTransportRecordSlotTable(cPacketIds - 2, cbRecord);
	*(int*) ((char*) pObject + 0x0c) = cPacketIds - 2;
	return pObject;
}

// FUNCTION: LEMBALL 0x00461580
void* VsNetGlobalLockedPacketState::ClaimGlobalPacketLockedRecordSlot(void)
{
	CReadPacket* pSlot;
	unsigned short nPacket;
	unsigned int nIndex;
	unsigned short nSequence;
	int fClaimed;

	nPacket = *(unsigned short*) ((char*) g_pEffTransportPacketBuffer + 8);
	nIndex = nPacket;
	if (nPacket > 2) {
		nIndex -= 3;
	}
	pSlot = m_appSlots08[nIndex];
	fClaimed = 0;
	nSequence = *(unsigned short*) ((char*) pSlot->m_pPayload04 + 0x0a);
	if (nPacket < 3 || (*(unsigned short*) ((char*) g_pEffTransportPacketBuffer + 0x0a) != nSequence &&
						(short) (*(unsigned short*) ((char*) g_pEffTransportPacketBuffer + 0x0a) - nSequence) >= 0)) {
		fClaimed = pSlot->m_fPayloadPresent24 == 0;
		CopyGlobalPacketIntoLockedRecordSlot((int) nIndex);
	}
	if (fClaimed != 0) {
		return m_appSlots08[nIndex];
	}
	return 0;
}
#endif

#ifdef LEMBALL_CREADNCBUFF_EFFSTRM
// FUNCTION: LEMBALL 0x004615f0
void* GameRecordSlotTable::GetShiftedRecordSlotByPacketId(int nPacketId)
{
	if (nPacketId >= 3) {
		nPacketId -= 3;
	}
	return m_apSlots08[nPacketId];
}
#endif
