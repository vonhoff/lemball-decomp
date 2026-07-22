#ifndef LEMBALL_NETWORK_STREAM_H
#define LEMBALL_NETWORK_STREAM_H

struct VsNetEffStreamChannelState {
	void** m_pVtable;
	int m_nReserved04;
	int m_nSelectedPeer08;
	int m_nReserved0c;
	int m_nReserved10;
	int m_nReserved14;
	int m_nReserved18;
	int m_nReserved1c;
	unsigned short m_nSelectedPort20;
	unsigned short m_nReserved22;
	int m_nReserved24;
	void* m_pSideBuffer28;

	VsNetEffStreamChannelState* ConstructEffStreamChannelState(void);
	void DestroyEffStreamChannelState(void);
	void SetEffStreamChannelAsyncErrorStatus(int nStatus);
};

/* Serialized tag copied by the stream header virtual.  The binary reads
 * exactly these fields at +0, +4, +8, +a, +c, and +e. */
struct VsNetEffStreamTag {
	unsigned int m_nWord00;
	unsigned int m_nWord04;
	unsigned short m_nWord08;
	unsigned short m_nWord0a;
	unsigned short m_nWord0c;
	unsigned char m_bByte0e;
};

struct VsNetEffStreamCore {
	void** m_pVtable;
	int m_nReserved04;
	int m_pPayloadBuffer08;
	int m_fOwnsPayload0c;
	int m_nReserved10;
	int m_nReserved14;
	int m_nBufferEnd18;
	unsigned char* m_nReserved1c;
	unsigned char* m_nReserved20;
	int m_nReserved24;
	int m_fBusy28;
};

/*
 * 0045F750 first installs the shared GameEffStream-compatible table
 * (004932C8), calls GameEffStream::ResetStateFields with unchanged this,
 * then installs the EFF stream table (00498F40).  This proves a shared ABI
 * prefix, not a direct C++ base edge: file-backed record slots reuse the
 * prefix but place marker data at +2c.  The common prefix is modeled as
 * VsNetEffStreamCore; the GameEffStream class edge remains unproven.
 */
struct VsNetEffStreamBase : VsNetEffStreamCore {
	VsNetEffStreamTag* m_pTagBuffer2c;
	unsigned short m_nWord30;
	unsigned short m_nWord32;
	unsigned short m_nWord34;
	unsigned short m_nWord36;

	void WriteEffStreamU32BE(unsigned int nValue);
	void WriteEffStreamU16BE(unsigned short nValue);
	void WriteEffStreamU8(unsigned char nValue);
	void WriteEffStreamBytes(const void* pvSource, unsigned int cbWrite);
	void WriteEffStreamCString(const char* pszSource);
	void ReadEffStreamU32BE(unsigned char* pbTarget);
	unsigned int ReadEffStreamU32BEValue(void);
	void SwapEffStreamU32BEAndAdvance(void);
	void ReadEffStreamU16BE(unsigned char* pbTarget);
	unsigned short ReadEffStreamU16BEValue(void);
	void SwapEffStreamU16BEAndAdvance(void);
	void ReadEffStreamU8(unsigned char* pbTarget);
	unsigned char ReadEffStreamU8Value(void);
	void AdvanceEffStreamReadCursorByte(void);
	void ReadEffStreamRangePointer(void** ppvTarget, int cbRead);
	void ReadEffStreamBytes(void* pvTarget, unsigned int cbRead);
	void ReadEffStreamCString(char** ppszTarget);
	void ReadEffStreamCStringIntoBuffer(char* pszTarget);
	int SaveEffStreamToMemoryRange(int nTargetBuffer, int cbRange);
	VsNetEffStreamBase* ConstructEffStreamBase(void);
	void DestroyEffStreamBase(void);
	void* DeleteEffStreamBaseWrapper(BYTE fDeleteFlags);
	void DestroyEffStreamBaseThunk(void);
};

struct VsNetDualHandleEffStream : VsNetEffStreamBase {
	char m_abReserved38[0x0c];
	void** m_pChannelStateConstructionOffsets44;
	void* m_pPrimaryHandleArray48;
	void* m_pPrimaryHandleObject4c;
	void* m_pSecondaryHandleArray50;
	void* m_pSecondaryHandleObject54;
	unsigned char m_abChannelState58[0x2c];

	void ReleaseEffStreamPrimaryHandleGroup(void);
	void ReleaseEffStreamSecondaryHandleGroup(void);
	void ConfigureEffStreamPrimaryHandleGroup(int nPacketCount, int cbPayload, int nMode);
	void ConfigureEffStreamSecondaryHandleGroup(int nPacketCount, int cbPayload);
	void DestroyDualHandleEffStream(void);
};

struct VsNetTimedEffStream : VsNetEffStreamBase {
	int m_nReserved38;
	DWORD m_dwLastTick3c;
	int m_nReserved40;
	void** m_pChannelStateConstructionOffsets44;
	void* m_pPrimaryHandleArray48;
	void* m_pSecondaryHandleArray4c;
	void* m_pTertiaryHandleArray50;
	int m_nReserved54;
	int m_nReserved58;
	int m_nHandleIndex5c;
	int m_nReserved60;
	int m_nReserved64;
	int m_nReserved68;
	int m_nReserved6c;
	void* m_pRuntimeService70;
	unsigned char m_abChannelState78[0x2c];

	void ReleaseTimedEffStreamPrimaryHandle(void);
	void ReleaseTimedEffStreamPrimaryHandleThunk(int nUnused0, int nUnused1, int nUnused2);
	void ReleaseTimedEffStreamSecondaryHandles(void);
	void ConfigureTimedEffStreamSecondaryHandle(int nPacketCount);
	void InvokeTimedEffStreamServiceCallback(void* pArgument);
	VsNetTimedEffStream* ConstructTimedEffStream(int fConstructChannelState);
	void DestroyTimedEffStream(void);
};

/* Runtime stack contains three embedded stream subobjects.  Keep offsets
 * explicit: channel state +0x24, timed stream +0x54, dual stream +0xcc. */
struct VsNetRuntimeChannelStack {
	void** m_pVtable;
	void** m_pOuterConstructionOffsets04;
	int m_nReserved08;
	int m_nReserved0c;
	unsigned char* m_pbRuntimeFlags10;
	int m_nReserved14;
	int m_nReserved18;
	void* m_pRuntimeSideBuffer1c;
	int m_nReserved20;
	unsigned char m_abChannelState24[0x30];
	unsigned char m_abTimedStream54[0x78];
	unsigned char m_abDualStreamcc[0x58];
	void** m_pEmbeddedConstructionOffsets124;
	unsigned char m_abDualStreamTail128[0x2c];

	void* ConstructEffTransportRuntimeChannelStack(int fConstructEmbeddedObjects);
	short ClaimRuntimeFlag(const char* pszFlags);
	void ClearRuntimeFlag(short nFlag);
};

struct VsNetRuntimeChannelStackReleaseFront {
	void Release(void);
};

struct VsNetRuntimeChannelStackDualStreamVtableData {
	void* m_pReturnTrue;
	void* m_pReturnTrueSecondary;
	void* m_pReversePayload;
	void* m_pNoop;
	void* m_pWriteTaggedHeader;
	void* m_pDelete;
	void* m_pDeleteRet;
	void* m_pWriteGlobalSession;
	void* m_pClaimRecord;
	void* m_pDeleteRet8;
};

extern void* g_NETWORK_RuntimeChannelStackTimedStreamVtable[10];
extern VsNetRuntimeChannelStackDualStreamVtableData g_NETWORK_RuntimeChannelStackDualStreamVtable;

#endif
