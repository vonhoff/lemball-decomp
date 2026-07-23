#ifndef LEMBALL_NETWORK_STREAM_H
#define LEMBALL_NETWORK_STREAM_H

#include "ENGINE/MEDIA/EFFSTRM.H"

#pragma vtordisp(on)

// Compiler-generated deleting destructors and vtordisp entries proven by the
// original vtables.  Keeping these by-name annotations in the declaration
// unit lets reccmp follow the compiler-owned functions without introducing
// source wrappers.
// SYNTHETIC: LEMBALL 0x004628e0 symbol
// ??_GVsNetEffStreamChannelState@@UAEPAXI@Z
// SYNTHETIC: LEMBALL 0x00462b80 symbol
// ??_EVsNetEffStreamBase@@UAEPAXI@Z
// SYNTHETIC: LEMBALL 0x00462940 symbol
// ??_GVsNetDualHandleEffStream@@UAEPAXI@Z
// SYNTHETIC: LEMBALL 0x00462990 symbol
// ??_EVsNetTimedEffStream@@UAEPAXI@Z
// SYNTHETIC: LEMBALL 0x00462bc0 symbol
// ??_GVsNetRuntimeChannelStack@@UAEPAXI@Z
// SYNTHETIC: LEMBALL 0x00462db0 symbol
// ??_EVsNetCompositeEffTransportStack@@UAEPAXI@Z
// SYNTHETIC: LEMBALL 0x00462bb0 symbol
// ??_EVsNetRuntimeChannelStack@@$4PPPPPPPM@A@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x00462ba0 symbol
// ??_EVsNetDualHandleEffStream@@WFI@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x00462ca0 symbol
// ??_EVsNetRuntimeChannelCore@@$4PPPPPPPM@A@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x00462d20 symbol
// ??_EVsNetRuntimeChannelCore@@$4PPPPPPPM@DA@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x00462d50 symbol
// ??_EVsNetRuntimeChannelCore@@$4PPPPPPPM@KI@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x00462c60 symbol
// ??_EVsNetRuntimeChannelStack@@$4PPPPPPPM@KI@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x00462d80 symbol
// ??_EVsNetTimedEffStream@@$4PPPPPPPM@HI@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x00462da0 symbol
// ??_EVsNetCompositeEffTransportStack@@$4PPPPPPPM@A@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x00462e00 symbol
// ??_EVsNetCompositeEffTransportStack@@$4PPPPPPPM@DA@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x0047be50 symbol
// ??_GVsNetEmbeddedFileBackedEffChannelStack@@UAEPAXI@Z
// SYNTHETIC: LEMBALL 0x0047bf00 symbol
// ??_EVsNetEmbeddedFileBackedEffChannelStack@@$4PPPPPPPM@KI@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x0047b9d0 symbol
// ??_GVsNetFileBackedCompositeEffChannelAdapter@@UAEPAXI@Z
// SYNTHETIC: LEMBALL 0x0047b9c0 symbol
// ??_EVsNetFileBackedCompositeEffChannelAdapter@@$4PPPPPPPM@A@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x0047ba50 symbol
// ?ClearPendingWrite@VsNetEmbeddedFileBackedEffChannelStack@@$4PPPPPPPM@PPPPPOKM@AEXPAX@Z
// SYNTHETIC: LEMBALL 0x0047ba80 symbol
// ??_EVsNetFileBackedCompositeEffChannelAdapter@@$4PPPPPPPM@DA@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x0047bb00 symbol
// ?SendTimedSocketPayload@VsNetEmbeddedFileBackedEffChannelStack@@$4PPPPPPPM@PPPPPOKM@AEHPAXH@Z
// SYNTHETIC: LEMBALL 0x0047b9b0 symbol
// ?ReleaseFileBackedEffMarker@VsNetFileBackedCompositeEffChannelAdapter@@$4PPPPPPPM@A@AEHXZ
// SYNTHETIC: LEMBALL 0x0047bb40 symbol
// ?ReleaseFileBackedEffMarker@VsNetFileBackedCompositeEffChannelAdapter@@$4PPPPPPPM@7AEHXZ
// SYNTHETIC: LEMBALL 0x0047bc40 symbol
// ?SendTimedSocketPayload@VsNetEmbeddedFileBackedEffChannelStack@@$4PPPPPPPM@PPPPPOKI@AEHPAXH@Z
// SYNTHETIC: LEMBALL 0x0047bb60 symbol
// ??_GVsNetGlobalStateFileBackedEffComposite@@UAEPAXI@Z
// SYNTHETIC: LEMBALL 0x0047bf40 symbol
// ??_GVsNetDualSlotTableFileBackedEffComposite@@UAEPAXI@Z
// SYNTHETIC: LEMBALL 0x0047bb10 symbol
// ??_EVsNetFileBackedCompositeEffChannelAdapter@@$4PPPPPPPM@KI@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x0047bc50 symbol
// ??_EVsNetGlobalStateFileBackedEffComposite@@$4PPPPPPPM@KI@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x0047be20 symbol
// ??_EVsNetDualFileBackedEffChannel@@WCM@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x0047c000 symbol
// ??_EVsNetDualSlotTableFileBackedEffComposite@@$4PPPPPPPM@KI@AEPAXI@Z
struct VsNetEffStreamChannelState {
	virtual int ReleaseFileBackedEffMarker(void) = 0;
	virtual ~VsNetEffStreamChannelState();
	virtual void ClearNestedEffChannelAsyncStatus(void) = 0;
	virtual void ClearPendingWrite(void* pArgument) = 0;

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

	VsNetEffStreamChannelState();
	inline VsNetEffStreamChannelState* ConstructEffStreamChannelState(void) { return this; }
	inline void DestroyEffStreamChannelState(void) { this->~VsNetEffStreamChannelState(); }
	void SetEffStreamChannelAsyncErrorStatus(int nStatus);
};

/* Raw prefix views remain for record/file layouts that share bytes but are
 * not instances of the virtual-inheritance family. */
struct VsNetEffStreamChannelStateLayout {
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

	inline VsNetEffStreamChannelState* ConstructEffStreamChannelState(void)
	{
		return ((VsNetEffStreamChannelState*) this)->ConstructEffStreamChannelState();
	}
	inline void DestroyEffStreamChannelState(void)
	{
		((VsNetEffStreamChannelState*) this)->DestroyEffStreamChannelState();
	}
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

struct VsNetEffStreamCommon {
	virtual int AcceptEffStreamArgument(void* pArgument);
	virtual int AcceptEffStream(void);
	virtual void ReverseEffStreamPayload(void) = 0;
	virtual void NoopEffStream(void);
	virtual void WriteEffStreamTaggedHeader(void) = 0;
	virtual ~VsNetEffStreamCommon();

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

	inline VsNetEffStreamCommon()
	{
		GameEffStream* pStream = (GameEffStream*) this;
		pStream->m_pVtable = (GameEffStreamVtable*) g_GAME_EffStreamConstructionVtable;
		pStream->m_nEventCode = 0;
		pStream->ResetStateFields();
	}
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

/* Tagged and file-backed streams share the 0x2c-byte polymorphic prefix.
 * Tagged streams append the 0x0c-byte serialization tag tail. */
struct VsNetEffStreamBase : VsNetEffStreamCommon {
	VsNetEffStreamBase();
	virtual void ReverseEffStreamPayload(void);
	virtual void WriteEffStreamTaggedHeader(void);
	inline void DestroyEffStreamBase(void) { this->~VsNetEffStreamBase(); }

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
	void DestroyEffStreamBaseThunk(void);
};

struct VsNetDualHandleEffStreamPrimary : VsNetEffStreamBase {
	char m_abReserved38[0x0c];
};

struct VsNetDualHandleEffStream : VsNetDualHandleEffStreamPrimary, virtual VsNetEffStreamChannelState {
	VsNetDualHandleEffStream();
	virtual ~VsNetDualHandleEffStream();
	inline void DestroyDualHandleEffStream(void) { this->~VsNetDualHandleEffStream(); }
	virtual void DualStreamNoop(void);
	virtual void WriteGlobalSession(void) = 0;
	virtual void* ClaimRecord(void) = 0;
	virtual void DualStreamRet8(int nArgument0, void* pArgument1) = 0;

	void* m_pPrimaryHandleArray48;
	void* m_pPrimaryHandleObject4c;
	void* m_pSecondaryHandleArray50;
	void* m_pSecondaryHandleObject54;

	void ReleaseEffStreamPrimaryHandleGroup(void);
	void ReleaseEffStreamSecondaryHandleGroup(void);
	void ConfigureEffStreamPrimaryHandleGroup(int nPacketCount, int cbPayload, int nMode);
	void ConfigureEffStreamSecondaryHandleGroup(int nPacketCount, int cbPayload);
};

struct VsNetTimedEffStreamPrimary : VsNetEffStreamBase {
	int m_nReserved38;
	DWORD m_dwLastTick3c;
	int m_nReserved40;
};

struct VsNetTimedEffStream : VsNetTimedEffStreamPrimary, virtual VsNetEffStreamChannelState {
	VsNetTimedEffStream();
	inline VsNetTimedEffStream* ConstructTimedEffStream(int) { return this; }
	virtual ~VsNetTimedEffStream();
	inline void DestroyTimedEffStream(void) { this->~VsNetTimedEffStream(); }
	virtual void ClearPendingWrite(void* pArgument);
	virtual void WriteGlobalSession(void) = 0;
	virtual void* ClaimRecord(void) = 0;
	virtual void CaptureTimedSocketLookup(void* pLookupResult) = 0;
	virtual int SendTimedSocketPayload(void* pBuffer, int cbBuffer) = 0;
	virtual void SetTimedChannelWord(unsigned short nValue);

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

	void ReleaseTimedEffStreamPrimaryHandle(void);
	void ReleaseTimedEffStreamPrimaryHandleThunk(int nUnused0, int nUnused1, int nUnused2);
	void ReleaseTimedEffStreamSecondaryHandles(void);
	void ConfigureTimedEffStreamSecondaryHandle(int nPacketCount);
	void InvokeTimedEffStreamServiceCallback(void* pArgument);
};

struct VsNetRuntimeChannelCore :
	virtual VsNetEffStreamChannelState,
	virtual VsNetTimedEffStream,
	virtual VsNetDualHandleEffStream {
	virtual ~VsNetRuntimeChannelCore() {}
	virtual void ClearPendingWrite(void* pArgument) = 0;
	virtual void WriteGlobalSession(void);
	virtual void* ClaimRecord(void);
};

struct VsNetRuntimeChannelStack :
	virtual VsNetEffStreamChannelState,
	virtual VsNetTimedEffStream,
	virtual VsNetDualHandleEffStream,
	virtual VsNetRuntimeChannelCore {
	VsNetRuntimeChannelStack();
	inline void* ConstructEffTransportRuntimeChannelStack(int) { return this; }
	virtual short ClaimRuntimeFlag(const char* pszFlags);
	virtual void ClearRuntimeFlag(short nFlag);
	virtual void RuntimeReserved08(void) = 0;
	virtual int RuntimeReserved0c(char* pszValue) = 0;
	virtual void ServiceRuntimeConnect(void);
	virtual void RuntimeReserved14(void) = 0;
	virtual void RuntimeReserved18(void) = 0;
	~VsNetRuntimeChannelStack();
	virtual void ClearPendingWrite(void* pArgument);
	virtual void DualStreamRet8(int nDispatchType, void* pEvent);

	int m_nReserved08;
	int m_nReserved0c;
	unsigned char* m_pbRuntimeFlags10;
	int m_nReserved14;
	int m_nReserved18;
	void* m_pRuntimeSideBuffer1c;
};

struct VsNetCompositeEffTransportStack :
	virtual VsNetEffStreamChannelState,
	virtual VsNetTimedEffStream,
	virtual VsNetDualHandleEffStream,
	virtual VsNetRuntimeChannelCore {
	VsNetCompositeEffTransportStack();
	inline void* ConstructCompositeEffTransportStack(int) { return this; }
	virtual int CompositeReserved00(const char* pszValue0, const char* pszValue1) = 0;
	virtual void ServicePeer(void);
	virtual void CompositeReserved08(void) = 0;
	virtual void CompositeReserved0c(int nArgument) = 0;
	virtual void CompositeReserved10(void) = 0;
	virtual void CompositeReserved14(void) = 0;
	~VsNetCompositeEffTransportStack();
	virtual void ClearPendingWrite(void* pArgument);
	virtual void DualStreamNoop(void);
	virtual void* ClaimRecord(void);
	virtual void DualStreamRet8(int nDispatchType, void* pEvent);

	int m_nField08;
	int m_nField0c;
	int m_nField10;
	int m_nField14;
	int m_nField18;
	int m_nField1c;
	int m_nField20;
	DWORD m_dwLastActivity24;
	DWORD m_dwServiceTick28;
};

struct VsNetRuntimeChannelStackReleaseFront {
	inline void Release(void)
	{
		((VsNetRuntimeChannelStack*) ((char*) this - 0x24))->~VsNetRuntimeChannelStack();
	}
};

inline void* ConstructDualHandleEffStream(void* pStream, int)
{
	return pStream;
}

#endif
