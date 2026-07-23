#include "ENGINE/CORE/SAFEVT.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/MEDIA/EFFSTRM.H"
#include "ENGINE/MEDIA/VSSTRM.H"
#include "ENGINE/NET/VSNET.H"
#include "GAME.H"

#include <string.h>

struct VsNetConstructionAdjustorVtable {
	void* m_pReserved00;
	int m_nPrimaryOffset;
	int m_nSecondaryOffset;
	int m_nTertiaryOffset;
	int m_nQuaternaryOffset;
};

struct VsNetAdjustorSubobject {
	int m_nThisDelta;
	void* m_pVtable;
};

struct VsNetAdjustorThunkHeader {
	int m_nThisDelta;
};

struct VsNetThunkAdjustorGroup {
	VsNetConstructionAdjustorVtable* m_pOffsets;
	VsNetAdjustorSubobject* m_pPrimary;
	VsNetAdjustorSubobject* m_pSecondary;
	VsNetAdjustorSubobject* m_pTertiary;
};

struct VsNetEffStreamSerializeVtable {
	void* m_pReserved00;
	void (*m_pLoadFromMemoryHeader)(void* pThis);
	void (*m_pLoadFromMemoryBody)(void* pThis);
	void (*m_pBeginWriteHeader)(void* pThis);
	void (*m_pBeginWriteBody)(void* pThis);
};

struct VsNetChannelStateRuntime {
	virtual void CancelPending(void) = 0;
};

struct VsNetRuntimeService {
	virtual void Reserved00(void) = 0;
	virtual void Reserved04(void) = 0;
	virtual void Reserved08(void) = 0;
	virtual void Invoke(void* pArgument) = 0;
};

struct VsNetRuntimeChannelStackWrapperView {
	unsigned char m_abChannelState00[0x30];
	unsigned char m_abTimedStream30[0x78];
	unsigned char m_abDualStreamA8[0x78];
};

struct VsNetRuntimeWindowInterface {
	virtual void Reserved00(void) = 0;
	virtual void Reserved04(void) = 0;
	virtual void Reserved08(void) = 0;
	virtual void Reserved0C(void) = 0;
	virtual void Reserved10(void) = 0;
	virtual void Reserved14(void) = 0;
	virtual void Reserved18(void) = 0;
	virtual void Reserved1C(void) = 0;
	virtual void Reserved20(void) = 0;
	virtual void Reserved24(void) = 0;
	virtual void Reserved28(void) = 0;
	virtual void* CreateServiceObject(void) = 0;
	virtual void PumpMessages(void) = 0;
};

void EffStreamChannelStateRet4Thunk(BYTE fDelete);

struct VsNetAckedEffTransportRecordOwner {};
extern int PASCAL ReturnTrueVtableCallback(void* pObject);
extern int ReturnTrueVtableCallbackSecondary(void);
extern int PASCAL ReturnTrueVtableCallbackThunk(void* pObject);
extern int ReturnTrueVtableCallbackSecondaryThunk(void);
extern void ReturnVoidVtableCallback(void);
extern void ReverseEffTransportPayload(void* pObject);
extern void NoopVtableCallbackThunk(void);
extern void WriteEffTransportTaggedHeader(void* pObject);
void* DeleteEffStreamBaseWrapper00462900(void* pObject, BYTE fDelete);

void NoopVtableCallbackThunk(void);
int PASCAL ReturnTrueVtableCallbackThunk(void* pObject);
int ReturnTrueVtableCallbackSecondaryThunk(void);
int PASCAL ReturnTrueVtableCallback(void*);
int ReturnTrueVtableCallbackSecondary(void);
void NoopVtableCallback(void);
void* DeleteEffStreamBaseWrapper00462810(void* pObject, BYTE fDelete);
void* DeleteEffStreamBaseWrapper00462830(void* pObject, BYTE fDelete);
void* DeleteEffStreamBaseWrapper00462850(void* pObject, BYTE fDelete);
void* DeleteEffStreamBaseWrapper00462870(void* pObject, BYTE fDelete);
void* DeleteEffStreamBaseWrapper00462890(void* pObject, BYTE fDelete);
void* DeleteEffStreamBaseWrapper004628B0(void* pObject, BYTE fDelete);
void EffStreamChannelStateRet4Thunk(BYTE fDelete);
void* DeleteEffStreamBaseWrapper00462900(void* pObject, BYTE fDelete);
void ReturnVoidVtableCallback(void);
void LEMBALL_FASTCALL WriteEffStreamTwoU16Fields(void* pObject);
void LEMBALL_FASTCALL ReadEffStreamTwoU16Fields(void* pObject);
void LEMBALL_FASTCALL TcpipSocketStackDualRet8(void* pObject, void* pUnused, BYTE fDelete, BYTE fReserved);
int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);


// GLOBAL: LEMBALL 0x004a1e30
void* g_pEffTransportPeerAddressState = 0;
extern int g_nEffTransportAsyncErrorStatus;

extern "C" DWORD WINAPI timeGetTime(void);

extern void* g_pActiveNetworkRuntimeWindow;
extern void* g_pEffTransportRuntimeService;
extern char* g_pszEffTransportBroadcastStatusPayload;
extern char* g_pszFileBasedNetworkLocalHostName;
extern void* g_pEffTransportPacketBuffer;
extern int g_cbEffTransportMaxPacketBytes;

extern void* ConstructShiftedLockedRecordSlotTable(void* pObject, int cPacketIds, unsigned int cbRecord);
extern void* ConstructRangeEffTransportRecordBufferTable(void* pObject,
														 int nFirstPacket,
														 int nLastPacket,
														 int cbPayload,
														 unsigned int cbRecord);
extern void* ConstructRingLockedRecordSlotTable(void* pObject, int cRecords, unsigned int cbRecord);
extern void* ConstructRingEffTransportRecordBufferTable(void* pObject,
														int cRecords,
														int cbPayload,
														unsigned int cbRecord);
struct VsNetSimpleEffTransportRecordSlotTable {
	void* ConstructSimpleRecordSlotTableWrapper(int cRecords, unsigned int cbRecord);
};

struct VsNetVSMemObjectPointerArray {
	int m_cObjects;
	int m_nReserved04;
	int m_pObjectArray08;

	void DestroyVSMemObjectPointerArray(void);
};

// FUNCTION: LEMBALL 0x00401451
void NoopVtableCallbackThunk(void)
{
	NoopVtableCallback();
}

// FUNCTION: LEMBALL 0x00401564
int PASCAL ReturnTrueVtableCallbackThunk(void* pObject)
{
	return ReturnTrueVtableCallback(pObject);
}

// FUNCTION: LEMBALL 0x004026ad
int ReturnTrueVtableCallbackSecondaryThunk(void)
{
	return ReturnTrueVtableCallbackSecondary();
}

// FUNCTION: LEMBALL 0x0040abd0
int PASCAL ReturnTrueVtableCallback(void*)
{
	return 1;
}

// FUNCTION: LEMBALL 0x0040abe0
int ReturnTrueVtableCallbackSecondary(void)
{
	return 1;
}

// FUNCTION: LEMBALL 0x0040abf0
void NoopVtableCallback(void)
{
}

// FUNCTION: LEMBALL 0x0045eea0
VsNetEffStreamCommon::~VsNetEffStreamCommon()
{
	DWORD dwStartTime;

	((GameEffStream*) this)->m_pVtable = (GameEffStreamVtable*) g_GAME_EffStreamConstructionVtable;
	if (m_fBusy28 != 0) {
		dwStartTime = timeGetTime();
		while (m_fBusy28 != 0 && timeGetTime() - dwStartTime < 2000) {
			if (g_pActiveNetworkRuntimeWindow != 0) {
				((VsNetRuntimeWindowInterface*) g_pActiveNetworkRuntimeWindow)->PumpMessages();
			}
		}
	}
	if (m_nReserved14 != 0) {
		FreeVSMemBlock((void*) (unsigned long) m_pPayloadBuffer08);
		m_pPayloadBuffer08 = 0;
	}
}

// FUNCTION: LEMBALL 0x0045ef10
void VsNetEffStreamBase::WriteEffStreamU32BE(unsigned int nValue)
{
	*(unsigned char*) (m_nReserved1c + 3) = (unsigned char) nValue;
	*(unsigned char*) (m_nReserved1c + 2) = (unsigned char) (nValue >> 8);
	*(unsigned char*) (m_nReserved1c + 1) = (unsigned char) (nValue >> 16);
	*(unsigned char*) m_nReserved1c = (unsigned char) (nValue >> 24);
	m_nReserved1c += 4;
}

// FUNCTION: LEMBALL 0x0045ef40
void VsNetEffStreamBase::WriteEffStreamU16BE(unsigned short nValue)
{
	*(unsigned char*) (m_nReserved1c + 1) = (unsigned char) nValue;
	*(unsigned char*) m_nReserved1c = (unsigned char) (nValue >> 8);
	m_nReserved1c += 2;
}

// FUNCTION: LEMBALL 0x0045ef60
void VsNetEffStreamBase::WriteEffStreamU8(unsigned char nValue)
{
	*m_nReserved1c = nValue;
	++m_nReserved1c;
}

// FUNCTION: LEMBALL 0x0045ef70
void VsNetEffStreamBase::WriteEffStreamCString(const char* pszSource)
{
	WriteEffStreamBytes(pszSource, (unsigned int) strlen(pszSource) + 1);
}

// FUNCTION: LEMBALL 0x0045efc0
void VsNetEffStreamBase::WriteEffStreamBytes(const void* pvSource, unsigned int cbWrite)
{
	unsigned int i;
	const unsigned char* pbSource;
	unsigned char* pbTarget;
	pbSource = (const unsigned char*) pvSource;
	pbTarget = (unsigned char*) (unsigned long) m_nReserved1c;

	for (i = cbWrite >> 2; i != 0; --i) {
		*(unsigned int*) pbTarget = *(const unsigned int*) pbSource;
		pbSource += 4;
		pbTarget += 4;
	}

	for (i = cbWrite & 3; i != 0; --i) {
		*pbTarget++ = *pbSource++;
	}

	m_nReserved1c += cbWrite;
}

// FUNCTION: LEMBALL 0x0045eff0
unsigned int VsNetEffStreamBase::ReadEffStreamU32BEValue(void)
{
	unsigned int nValue;

	ReadEffStreamU32BE((unsigned char*) &nValue);
	return nValue;
}

// FUNCTION: LEMBALL 0x0045f010
void VsNetEffStreamBase::ReadEffStreamU32BE(unsigned char* pbTarget)
{
	pbTarget[0] = *(unsigned char*) (m_nReserved20 + 3);
	pbTarget[1] = *(unsigned char*) (m_nReserved20 + 2);
	pbTarget[2] = *(unsigned char*) (m_nReserved20 + 1);
	pbTarget[3] = *(unsigned char*) m_nReserved20;
	m_nReserved20 += 4;
}

// FUNCTION: LEMBALL 0x0045f040
void VsNetEffStreamBase::SwapEffStreamU32BEAndAdvance(void)
{
	unsigned char nByte0;
	unsigned char nByte1;

	nByte0 = m_nReserved20[0];
	nByte1 = m_nReserved20[1];
	m_nReserved20[0] = m_nReserved20[3];
	m_nReserved20[1] = m_nReserved20[2];
	m_nReserved20[2] = nByte1;
	m_nReserved20[3] = nByte0;
	m_nReserved20 += 4;
}

// FUNCTION: LEMBALL 0x0045f070
unsigned short VsNetEffStreamBase::ReadEffStreamU16BEValue(void)
{
	unsigned short nValue;

	ReadEffStreamU16BE((unsigned char*) &nValue);
	return nValue;
}

// FUNCTION: LEMBALL 0x0045f090
void VsNetEffStreamBase::ReadEffStreamU16BE(unsigned char* pbTarget)
{
	pbTarget[0] = *(unsigned char*) (m_nReserved20 + 1);
	pbTarget[1] = *(unsigned char*) m_nReserved20;
	m_nReserved20 += 2;
}

// FUNCTION: LEMBALL 0x0045f0b0
void VsNetEffStreamBase::SwapEffStreamU16BEAndAdvance(void)
{
	unsigned char nByte0;
	unsigned char nByte1;

	nByte1 = m_nReserved20[1];
	nByte0 = m_nReserved20[0];
	m_nReserved20[1] = nByte0;
	m_nReserved20[0] = nByte1;
	m_nReserved20 += 2;
}

// FUNCTION: LEMBALL 0x0045f0d0
unsigned char VsNetEffStreamBase::ReadEffStreamU8Value(void)
{
	unsigned char nValue;

	ReadEffStreamU8(&nValue);
	return nValue;
}

// FUNCTION: LEMBALL 0x0045f0f0
void VsNetEffStreamBase::ReadEffStreamU8(unsigned char* pbTarget)
{
	*pbTarget = *m_nReserved20;
	++m_nReserved20;
}

// FUNCTION: LEMBALL 0x0045f110
void VsNetEffStreamBase::AdvanceEffStreamReadCursorByte(void)
{
	++m_nReserved20;
}

// FUNCTION: LEMBALL 0x0045f120
void VsNetEffStreamBase::ReadEffStreamCString(char** ppszTarget)
{
	*ppszTarget = (char*) (unsigned long) m_nReserved20;
	m_nReserved20 += (int) strlen(*ppszTarget) + 1;
}

// FUNCTION: LEMBALL 0x0045f140
void VsNetEffStreamBase::ReadEffStreamRangePointer(void** ppvTarget, int cbRead)
{
	*ppvTarget = m_nReserved20;
	m_nReserved20 += cbRead;
}

// FUNCTION: LEMBALL 0x0045f160
void VsNetEffStreamBase::ReadEffStreamCStringIntoBuffer(char* pszTarget)
{
	strcpy(pszTarget, (char*) m_nReserved20);
	m_nReserved20 += strlen(pszTarget) + 1;
}

// FUNCTION: LEMBALL 0x0045f1b0
void VsNetEffStreamBase::ReadEffStreamBytes(void* pvTarget, unsigned int cbRead)
{
	unsigned int i;
	unsigned char* pbTarget;
	unsigned char* pbSource;
	pbTarget = (unsigned char*) pvTarget;
	pbSource = (unsigned char*) (unsigned long) m_nReserved20;

	for (i = cbRead >> 2; i != 0; --i) {
		*(unsigned int*) pbTarget = *(unsigned int*) pbSource;
		pbSource += 4;
		pbTarget += 4;
	}

	for (i = cbRead & 3; i != 0; --i) {
		*pbTarget++ = *pbSource++;
	}

	m_nReserved20 += cbRead;
}

// FUNCTION: LEMBALL 0x0045f250
int VsNetEffStreamBase::SaveEffStreamToMemoryRange(int nTargetBuffer, int cbRange)
{
	VsNetEffStreamSerializeVtable* pVtable;
	int (*pfnWriteBody)(void* pThis);

	m_pPayloadBuffer08 = nTargetBuffer;
	pVtable = (VsNetEffStreamSerializeVtable*) *(void***) this;
	m_nReserved1c = (unsigned char*) (unsigned long) (nTargetBuffer + cbRange);
	m_fOwnsPayload0c = m_nBufferEnd18 + nTargetBuffer + cbRange;
	pVtable->m_pBeginWriteHeader(this);
	pfnWriteBody = (int (*)(void*)) pVtable->m_pBeginWriteBody;
	return pfnWriteBody(this);
}

// FUNCTION: LEMBALL 0x0045f680
VsNetEffStreamChannelState::VsNetEffStreamChannelState()
{
	m_nSelectedPeer08 = -1;
	m_nSelectedPort20 = 0xffff;
	m_nReserved10 = 0;
	m_nReserved0c = 0;
	m_nReserved14 = 0;
	m_nReserved18 = 0;
	m_nReserved1c = 0;
	m_nReserved24 = 0;
	m_nReserved04 = 0;
	m_pSideBuffer28 = AllocateVSMemBlock(0x10);
}

// FUNCTION: LEMBALL 0x0045f6c0
VsNetEffStreamChannelState::~VsNetEffStreamChannelState()
{
	FreeVSMemBlock(m_pSideBuffer28);
}

// FUNCTION: LEMBALL 0x0045f6e0
void VsNetEffStreamChannelState::SetEffStreamChannelAsyncErrorStatus(int nStatus)
{
	m_nReserved04 = nStatus;
	g_nEffTransportAsyncErrorStatus = nStatus;
	if (nStatus != 0 && m_nReserved0c != 0) {
		((VsNetChannelStateRuntime*) this)->CancelPending();
		m_nReserved10 = 0;
		m_nReserved0c = 0;
		m_nSelectedPeer08 = -1;
		m_nReserved14 = 0;
	}
}

// FUNCTION: LEMBALL 0x0045f750
VsNetEffStreamBase::VsNetEffStreamBase()
{
	m_nWord34 = 0;
	m_nBufferEnd18 += 0x10;
	m_nWord36 = 0;
	m_nWord30 = 0;
	m_nWord32 = 0;
}

// FUNCTION: LEMBALL 0x0045f820
VsNetDualHandleEffStream::VsNetDualHandleEffStream()
{
	m_pPrimaryHandleArray48 = 0;
	m_pPrimaryHandleObject4c = 0;
	m_pSecondaryHandleArray50 = 0;
	m_pSecondaryHandleObject54 = 0;
	m_pTagBuffer2c = 0;

	if (g_pEffTransportPeerAddressState == 0) {
		g_pEffTransportPeerAddressState =
			((VsNetRuntimeWindowInterface*) g_pActiveNetworkRuntimeWindow)->CreateServiceObject();
	}

}

// FUNCTION: LEMBALL 0x0045f8a0
VsNetDualHandleEffStream::~VsNetDualHandleEffStream()
{
	ReleaseEffStreamPrimaryHandleGroup();
	ReleaseEffStreamSecondaryHandleGroup();
}

// FUNCTION: LEMBALL 0x0045f8d0
void VsNetDualHandleEffStream::ReleaseEffStreamPrimaryHandleGroup(void)
{
	void* pHandleArray;
	void* pHandleObject;

	pHandleArray = m_pPrimaryHandleArray48;
	if (pHandleArray != 0) {
		((VsNetVSMemObjectPointerArray*) pHandleArray)->DestroyVSMemObjectPointerArray();
		FreeVSMemBlock(pHandleArray);
	}

	pHandleObject = m_pPrimaryHandleObject4c;
	if (pHandleObject != 0) {
		((void (*)(int)) * *(void***) pHandleObject)(1);
	}
}

// FUNCTION: LEMBALL 0x0045f900
void VsNetDualHandleEffStream::ReleaseEffStreamSecondaryHandleGroup(void)
{
	void* pHandleArray;
	void* pHandleObject;

	pHandleArray = m_pSecondaryHandleArray50;
	if (pHandleArray != 0) {
		((VsNetVSMemObjectPointerArray*) pHandleArray)->DestroyVSMemObjectPointerArray();
		FreeVSMemBlock(pHandleArray);
	}

	pHandleObject = m_pSecondaryHandleObject54;
	if (pHandleObject != 0) {
		((void (*)(int)) * *(void***) pHandleObject)(1);
	}
}

// FUNCTION: LEMBALL 0x0045f930
void VsNetDualHandleEffStream::ConfigureEffStreamPrimaryHandleGroup(int nPacketCount, int cbPayload, int nMode)

{
	void* pHandleArray;
	void* pHandleObject;

	ReleaseEffStreamPrimaryHandleGroup();
	pHandleArray = AllocateVSMemBlock(0x10);
	if (pHandleArray == 0) {
		m_pPrimaryHandleArray48 = 0;
	}
	else {
		m_pPrimaryHandleArray48 = ConstructShiftedLockedRecordSlotTable(pHandleArray,
																		nPacketCount,
																		(unsigned int) g_cbEffTransportMaxPacketBytes);
	}

	pHandleObject = AllocateVSMemBlock(0x28);
	if (pHandleObject == 0) {
		m_pPrimaryHandleObject4c = 0;
	}
	else {
		m_pPrimaryHandleObject4c =
			ConstructRangeEffTransportRecordBufferTable(pHandleObject,
														nPacketCount + 1,
														cbPayload,
														nMode,
														(unsigned int) g_cbEffTransportMaxPacketBytes);
	}
}

// FUNCTION: LEMBALL 0x0045f9b0
void VsNetDualHandleEffStream::ConfigureEffStreamSecondaryHandleGroup(int nPacketCount, int cbPayload)

{
	void* pHandleArray;
	void* pHandleObject;

	ReleaseEffStreamSecondaryHandleGroup();
	pHandleArray = AllocateVSMemBlock(0x14);
	if (pHandleArray == 0) {
		m_pSecondaryHandleArray50 = 0;
	}
	else {
		m_pSecondaryHandleArray50 = ConstructRingLockedRecordSlotTable(pHandleArray,
																	   nPacketCount,
																	   (unsigned int) g_cbEffTransportMaxPacketBytes);
	}

	pHandleObject = AllocateVSMemBlock(0x20);
	if (pHandleObject == 0) {
		m_pSecondaryHandleObject54 = 0;
	}
	else {
		m_pSecondaryHandleObject54 =
			ConstructRingEffTransportRecordBufferTable(pHandleObject,
													   nPacketCount,
													   cbPayload,
													   (unsigned int) g_cbEffTransportMaxPacketBytes);
	}

	if (g_pEffTransportPacketBuffer == 0) {
		g_pEffTransportPacketBuffer = AllocateVSMemBlock((unsigned int) g_cbEffTransportMaxPacketBytes);
	}
}

// FUNCTION: LEMBALL 0x0045fce0
VsNetTimedEffStream::VsNetTimedEffStream()
{
	DWORD dwNow;
	int* pHandleArray;

	dwNow = timeGetTime();
	m_dwLastTick3c = dwNow - 1000;

	pHandleArray = (int*) AllocateVSMemBlock(0x10);
	m_pTagBuffer2c = (VsNetEffStreamTag*) pHandleArray;
	*pHandleArray = 0x56533039;

	m_pPrimaryHandleArray48 = 0;
	m_pSecondaryHandleArray4c = 0;
	m_pTertiaryHandleArray50 = 0;
	m_nReserved54 = 0;
	m_nHandleIndex5c = -1;
	m_pRuntimeService70 =
		((VsNetRuntimeWindowInterface*) g_pActiveNetworkRuntimeWindow)->CreateServiceObject();
}

// FUNCTION: LEMBALL 0x0045fd80
VsNetTimedEffStream::~VsNetTimedEffStream()
{
	FreeVSMemBlock(m_pRuntimeService70);
	m_pRuntimeService70 = 0;
	FreeVSMemBlock(m_pTagBuffer2c);
	ReleaseTimedEffStreamPrimaryHandle();
	ReleaseTimedEffStreamSecondaryHandles();
}

// FUNCTION: LEMBALL 0x0045fde0
void VsNetTimedEffStream::InvokeTimedEffStreamServiceCallback(void* pArgument)
{
	((VsNetRuntimeService*) m_pRuntimeService70)->Invoke(pArgument);
}

// FUNCTION: LEMBALL 0x0045fdf0
void VsNetTimedEffStream::ReleaseTimedEffStreamPrimaryHandle(void)
{
	void* pHandleArray;

	pHandleArray = m_pPrimaryHandleArray48;
	if (pHandleArray != 0) {
		((VsNetVSMemObjectPointerArray*) pHandleArray)->DestroyVSMemObjectPointerArray();
		FreeVSMemBlock(pHandleArray);
	}
}

// FUNCTION: LEMBALL 0x0045fe10
void VsNetTimedEffStream::ReleaseTimedEffStreamSecondaryHandles(void)
{
	void* pHandleArray;

	pHandleArray = m_pSecondaryHandleArray4c;
	if (pHandleArray != 0) {
		((VsNetVSMemObjectPointerArray*) pHandleArray)->DestroyVSMemObjectPointerArray();
		FreeVSMemBlock(pHandleArray);
	}

	pHandleArray = m_pTertiaryHandleArray50;
	if (pHandleArray != 0) {
		((VsNetVSMemObjectPointerArray*) pHandleArray)->DestroyVSMemObjectPointerArray();
		FreeVSMemBlock(pHandleArray);
	}
}

// FUNCTION: LEMBALL 0x0045fe50
void VsNetTimedEffStream::ReleaseTimedEffStreamPrimaryHandleThunk(int nUnused0, int nUnused1, int nUnused2)

{
	(void) nUnused0;
	(void) nUnused1;
	(void) nUnused2;
	ReleaseTimedEffStreamPrimaryHandle();
}

// FUNCTION: LEMBALL 0x0045fe60
void VsNetTimedEffStream::ConfigureTimedEffStreamSecondaryHandle(int nPacketCount)
{
	void* pHandleArray;

	ReleaseTimedEffStreamSecondaryHandles();
	pHandleArray = AllocateVSMemBlock(0x0c);
	if (pHandleArray == 0) {
		m_pSecondaryHandleArray4c = 0;
	}
	else {
		m_pSecondaryHandleArray4c =
			((VsNetSimpleEffTransportRecordSlotTable*) pHandleArray)
				->ConstructSimpleRecordSlotTableWrapper(nPacketCount, (unsigned int) g_cbEffTransportMaxPacketBytes);
	}
}

// FUNCTION: LEMBALL 0x00460350
VsNetRuntimeChannelStack::VsNetRuntimeChannelStack()
{
	int i;

	m_nReserved14 = 0;
	m_nReserved08 = 0;
	m_nReserved0c = 0;
	m_pRuntimeSideBuffer1c = 0;

	g_pEffTransportRuntimeService =
		((VsNetRuntimeWindowInterface*) g_pActiveNetworkRuntimeWindow)->CreateServiceObject();
	m_pbRuntimeFlags10 = (unsigned char*) AllocateVSMemBlock(0x200);
	i = 0;
	do {
		++i;
		m_pbRuntimeFlags10[i - 1] = 0;
	} while (i < 0x200);

}

// FUNCTION: LEMBALL 0x004604e0
VsNetRuntimeChannelStack::~VsNetRuntimeChannelStack()
{
	void* pRuntimeService;

	if (m_pRuntimeSideBuffer1c != 0) {
		FreeVSMemBlock(m_pRuntimeSideBuffer1c);
	}
	FreeVSMemBlock(m_pbRuntimeFlags10);
	pRuntimeService = g_pEffTransportRuntimeService;
	FreeVSMemBlock(pRuntimeService);
	g_pEffTransportRuntimeService = 0;
	if (g_pszFileBasedNetworkLocalHostName != 0) {
		FreeVSMemBlock(g_pszFileBasedNetworkLocalHostName);
		g_pszFileBasedNetworkLocalHostName = 0;
	}
	if (g_pszEffTransportBroadcastStatusPayload != 0) {
		FreeVSMemBlock(g_pszEffTransportBroadcastStatusPayload);
		g_pszEffTransportBroadcastStatusPayload = 0;
	}
}

// FUNCTION: LEMBALL 0x004605d0
short VsNetRuntimeChannelStack::ClaimRuntimeFlag(const char* pszFlags)
{
	VsNetRuntimeChannelStack* pStack = this;
	short i;

	for (i = 0; i < 0x200; ++i) {
		if (pszFlags[i] == 0 && pStack->m_pbRuntimeFlags10[i] == 0) {
			pStack->m_pbRuntimeFlags10[i] = 1;
			return i;
		}
	}
	return -1;
}

// FUNCTION: LEMBALL 0x00462810
void* DeleteEffStreamBaseWrapper00462810(void* pObject, BYTE fDelete)
{
	((VsNetEffStreamBase*) pObject)->~VsNetEffStreamBase();
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x00462830
void* DeleteEffStreamBaseWrapper00462830(void* pObject, BYTE fDelete)
{
	((VsNetEffStreamBase*) pObject)->~VsNetEffStreamBase();
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x00462850
void* DeleteEffStreamBaseWrapper00462850(void* pObject, BYTE fDelete)
{
	((VsNetEffStreamBase*) pObject)->~VsNetEffStreamBase();
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x00462870
void* DeleteEffStreamBaseWrapper00462870(void* pObject, BYTE fDelete)
{
	((VsNetEffStreamBase*) pObject)->~VsNetEffStreamBase();
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x00462890
void* DeleteEffStreamBaseWrapper00462890(void* pObject, BYTE fDelete)
{
	((VsNetEffStreamBase*) pObject)->~VsNetEffStreamBase();
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x004628b0
void* DeleteEffStreamBaseWrapper004628B0(void* pObject, BYTE fDelete)
{
	((VsNetEffStreamBase*) pObject)->~VsNetEffStreamBase();
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x004628d0
void EffStreamChannelStateRet4Thunk(BYTE fDelete)
{
	(void) fDelete;
}

// FUNCTION: LEMBALL 0x00462900
void* DeleteEffStreamBaseWrapper00462900(void* pObject, BYTE fDelete)
{
	((VsNetEffStreamBase*) pObject)->~VsNetEffStreamBase();
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

// FUNCTION: LEMBALL 0x004629c0
void VsNetRuntimeChannelStack::ClearRuntimeFlag(short nFlag)
{
	m_pbRuntimeFlags10[nFlag] = 0;
}

// FUNCTION: LEMBALL 0x00462b00
void ReturnVoidVtableCallback(void)
{
}

// FUNCTION: LEMBALL 0x00462b40
void LEMBALL_FASTCALL WriteEffStreamTwoU16Fields(void* pObject)
{
	((VsNetEffStreamBase*) pObject)->WriteEffStreamU16BE(*(unsigned short*) ((char*) pObject + 0x2c));
	((VsNetEffStreamBase*) pObject)->WriteEffStreamU16BE(*(unsigned short*) ((char*) pObject + 0x2e));
}

// FUNCTION: LEMBALL 0x00462b60
void LEMBALL_FASTCALL ReadEffStreamTwoU16Fields(void* pObject)
{
	((VsNetEffStreamBase*) pObject)->ReadEffStreamU16BE((unsigned char*) ((char*) pObject + 0x2c));
	((VsNetEffStreamBase*) pObject)->ReadEffStreamU16BE((unsigned char*) ((char*) pObject + 0x2e));
}

// FUNCTION: LEMBALL 0x00462ed0
int WINAPI InitializeNonZrleVariantRenderEntry(int nValue)
{
	return nValue;
}

// FUNCTION: LEMBALL 0x0047b860
void VsNetEffStreamBase::DestroyEffStreamBaseThunk(void)
{
	this->~VsNetEffStreamBase();
}
