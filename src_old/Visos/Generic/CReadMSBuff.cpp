// Recovered Macintosh logical owner: CReadMSBuff.
// Included by the Windows VSNET translation unit to preserve ABI and link order.

// FUNCTION: LEMBALL 0x004613d0
void* VsNetEffTransportRecordBuffer::ConstructEffTransportRecordBuffer(int fAllocatePayload,
																	   int cbPayload,
																	   unsigned int cbRecord)

{
	VsNetEffTransportPacketHeader* pSerializedHeader;
	int cbSerialized;

	m_pVtable = g_NETWORK_DeleteEffTransportRecordBufferVtable;
	m_fAllocatePayload0c = fAllocatePayload;
	m_nReserved14 = 0;
	m_nReserved10 = 0;
	cbSerialized = (cbRecord & 0xffff) - 0x10;
	m_cbPayload08 = cbSerialized;
	if (fAllocatePayload != 0 && cbSerialized != 0) {
		m_pSerializedBuffer04 = (int) (unsigned long) AllocateVSMemBlock((unsigned int) (cbPayload + 0x10));
		pSerializedHeader = (VsNetEffTransportPacketHeader*) (unsigned long) m_pSerializedBuffer04;
		pSerializedHeader->m_wSequence0a = 0;
	}
	else {
		m_pSerializedBuffer04 = 0;
	}
	return this;
}

// FUNCTION: LEMBALL 0x00461440
void VsNetEffTransportRecordBuffer::DestroyEffTransportRecordBuffer(void)
{
	m_pVtable = g_NETWORK_DeleteEffTransportRecordBufferVtable;
	if (m_pSerializedBuffer04 != 0) {
		FreeVSMemBlock((void*) (unsigned long) m_pSerializedBuffer04);
	}
	m_pVtable = g_NETWORK_EffTransportRecordBufferVtable;
}

// FUNCTION: LEMBALL 0x00461470
void VsNetGlobalPacketRecordBuffer::AppendGlobalPacketIntoRecordBuffer(void)
{
	unsigned int cbPacket;
	unsigned int cbPayload;
	unsigned short nFragment;
	unsigned char* pbSource;
	unsigned char* pbTarget;

	cbPacket = (unsigned int) g_cbEffTransportCurrentPacketBytes;
	nFragment = *(unsigned short*) ((char*) g_pEffTransportPacketBuffer + 0x0c);
	if (nFragment != 0) {
		if (m_cbFragment10 == 0) {
			memcpy(m_pbSerialized04, g_pEffTransportPacketBuffer, 0x10);
			m_cbSerialized14 += 0x10;
		}
		cbPayload = cbPacket - 0x10;
		pbSource = (unsigned char*) g_pEffTransportPacketBuffer + 0x10;
		pbTarget = m_pbSerialized04 + 0x10 + m_cbFragmentStep08 * nFragment;
		memcpy(pbTarget, pbSource, cbPayload);
		m_cbSerialized14 += cbPayload;
		++m_cbFragment10;
		return;
	}
	if (m_cbFragment10 == 0) {
		memcpy(m_pbSerialized04, g_pEffTransportPacketBuffer, cbPacket);
		m_cbSerialized14 += cbPacket;
		++m_cbFragment10;
		return;
	}
	cbPayload = cbPacket - 0x10;
	memcpy(m_pbSerialized04 + 0x10, (unsigned char*) g_pEffTransportPacketBuffer + 0x10, cbPayload);
	m_cbSerialized14 += cbPayload;
	++m_cbFragment10;
}
