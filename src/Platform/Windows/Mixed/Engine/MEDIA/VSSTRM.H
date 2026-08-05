#ifndef LEMBALL_NETWORK_STREAM_H
#define LEMBALL_NETWORK_STREAM_H

#include "Platform/Windows/Mixed/Engine/MEDIA/EFFSTRM.H"

#pragma vtordisp(on)

// Compatibility symbol preserving MSVC 4.20 compiler state.
// ??_GVsNetEffStreamChannelState@@UAEPAXI@Z
// Compatibility symbol preserving MSVC 4.20 compiler state.
// ??_ECNetworkMessage@@UAEPAXI@Z
// VTABLE: LEMBALL 0x00498f30
struct VsNetEffStreamChannelState {
	virtual int ReleaseFileBackedEffMarker(void) = 0; // vtable+0x0
	virtual ~VsNetEffStreamChannelState();
	virtual void ClearNestedEffChannelAsyncStatus(void) = 0; // vtable+0x8
	virtual void ClearPendingWrite(void* pArgument);
#ifdef LEMBALL_DEFINE_EFF_STREAM_CHANNEL_STATE_VTABLE
	static void operator delete(void* pObject);
#endif

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

struct VsNetFileBackedDispatchOffsets;

struct VsNetEffStreamCommon {
	virtual int AcceptEffStreamArgument(void* pArgument);
	virtual int AcceptEffStream(void);
	virtual void ReverseEffStreamPayload(void) = 0;
	virtual void NoopEffStream(void);
	virtual void WriteEffStreamTaggedHeader(void) = 0;
	virtual ~VsNetEffStreamCommon();

	union {
		int m_nReserved04;
		int m_nRecordHeaderOffset04;
	};
	union {
		int m_pPayloadBuffer08;
		int m_nPayloadBaseOffset08;
	};
	union {
		int m_fOwnsPayload0c;
		VsNetFileBackedDispatchOffsets* m_pChannelThunk0c;
	};
	union {
		int m_nReserved10;
		int m_nPendingSlot10;
	};
	int m_nReserved14;
	union {
		int m_nBufferEnd18;
		int m_nAccumulatedStreamLength18;
	};
	union {
		unsigned char* m_nReserved1c;
		int m_nSaveRangeCursor1c;
	};
	union {
		unsigned char* m_nReserved20;
		int m_nLoadSourceCursor20;
	};
	int m_nReserved24;
	int m_fBusy28;

	inline VsNetEffStreamCommon()
	{
		GameEffStream* pStream = (GameEffStream*) this;
		pStream->m_pVtable = (GameEffStreamVtable*) g_GAME_EffStreamConstructionVtable;
		pStream->m_nEventCode = 0;
		pStream->ResetStateFields();
	}
#ifdef LEMBALL_DEFINE_MANAGED_ENTITY_PACKET_CLASSES
	inline VsNetEffStreamCommon(int nEventCode)
	{
		*(void**) this = g_GAME_EffStreamConstructionVtable;
		m_nReserved04 = nEventCode;
		((GameEffStream*) this)->ResetStateFields();
	}
#endif
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
// VTABLE: LEMBALL 0x00498f40
// Macintosh blueprint: Visos (Generic) CNetworkMessage (accepted x86
// correlations 0x45EE80-0x45F2B0; see macintosh-x86-correlations.csv).  The
// binary's mangled symbols name this class CNetworkMessage; the shared
// VsNetEffStreamCommon base + derived family model the proven polymorphic
// stream object.  Mac-only framing methods (CheckMessage/GetHeader/AddHeader)
// are platform_specific and have no x86 equivalent — do not add.
struct CNetworkMessage : VsNetEffStreamCommon {
	CNetworkMessage();
	virtual void ReverseEffStreamPayload(void);    // vtable+0x8
	virtual void WriteEffStreamTaggedHeader(void); // vtable+0x10
	inline void DestroyEffStreamBase(void) { this->~CNetworkMessage(); }
#ifdef LEMBALL_DEFINE_EFF_STREAM_BASE_VTABLE
	static void operator delete(void* pObject);
#endif

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

struct CReadSocketPrimary : CNetworkMessage {
	char m_abReserved38[0x0c];
};

// GLOBAL: LEMBALL 0x00498f58
// CReadSocket::`vbtable'
// VTABLE: LEMBALL 0x00498f60 VsNetEffStreamChannelState
// VTABLE: LEMBALL 0x00498f70 CReadSocketPrimary
struct CReadSocket : CReadSocketPrimary, virtual VsNetEffStreamChannelState {
	CReadSocket();
	virtual ~CReadSocket();
	inline void DestroyDualHandleEffStream(void) { this->~CReadSocket(); }
	virtual void DualStreamNoop(void);
	virtual void WriteGlobalSession(void) = 0;
	virtual void* ClaimRecord(void) = 0;
	virtual void DualStreamRet8(int nArgument0, void* pArgument1);
#ifdef LEMBALL_DEFINE_EFF_STREAM_BASE_VTABLE
	static void operator delete(void* pObject);
#endif

	void* m_pPrimaryHandleArray48;
	void* m_pPrimaryHandleObject4c;
	void* m_pSecondaryHandleArray50;
	void* m_pSecondaryHandleObject54;

	void ReleaseEffStreamPrimaryHandleGroup(void);
	void ReleaseEffStreamSecondaryHandleGroup(void);
	void ConfigureEffStreamPrimaryHandleGroup(int nPacketCount, int cbPayload, int nMode);
	void ConfigureEffStreamSecondaryHandleGroup(int nPacketCount, int cbPayload);
};

struct VsNetTimedEffStreamPrimary : CNetworkMessage {
	int m_nReserved38;
	DWORD m_dwLastTick3c;
	int m_nReserved40;
};

// GLOBAL: LEMBALL 0x00498f98
// VsNetTimedEffStream::`vbtable'
// VTABLE: LEMBALL 0x00498fa0 VsNetEffStreamChannelState
struct VsNetTimedEffStream : VsNetTimedEffStreamPrimary, virtual VsNetEffStreamChannelState {
	VsNetTimedEffStream();
	inline VsNetTimedEffStream* ConstructTimedEffStream(int) { return this; }
	virtual ~VsNetTimedEffStream();
	inline void DestroyTimedEffStream(void) { this->~VsNetTimedEffStream(); }
	virtual void ClearPendingWrite(void* pArgument);                     // vtable+0xc
	virtual void WriteGlobalSession(void) = 0;                           // vtable+0x10
	virtual void* ClaimRecord(void) = 0;                                 // vtable+0x14
	virtual void CaptureTimedSocketLookup(void* pLookupResult) = 0;      // vtable+0x18
	virtual int SendTimedSocketPayload(void* pBuffer, int cbBuffer) = 0; // vtable+0x1c
	virtual void SetTimedChannelWord(unsigned short nValue);             // vtable+0x20
#ifdef LEMBALL_DEFINE_EFF_STREAM_BASE_VTABLE
	static void operator delete(void* pObject);
#endif

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
#ifdef LEMBALL_DEFINE_TIMED_EFF_STREAM_SECONDARY_HANDLE_ABI
	void ConfigureTimedEffStreamSecondaryHandle(int nPacketCount, int nMode);
#else
	void ConfigureTimedEffStreamSecondaryHandle(int nPacketCount);
#endif
	void InvokeTimedEffStreamServiceCallback(void* pArgument);
};

// VTABLE: LEMBALL 0x00499038 VsNetRuntimeChannelCore
struct VsNetRuntimeChannelCore : virtual VsNetEffStreamChannelState,
								 virtual VsNetTimedEffStream,
								 virtual CReadSocket {
	virtual ~VsNetRuntimeChannelCore() {}
	virtual void ClearPendingWrite(void* pArgument);
	virtual void WriteGlobalSession(void);
	virtual void* ClaimRecord(void);
#ifdef LEMBALL_DEFINE_EFF_STREAM_BASE_VTABLE
	static void operator delete(void* pObject);
#endif
};

// GLOBAL: LEMBALL 0x00499048
// VsNetRuntimeChannelStack::`vbtable'{for `VsNetRuntimeChannelCore'}
// GLOBAL: LEMBALL 0x00499058
// VsNetRuntimeChannelStack::`vbtable'{for `CReadSocket'}
// GLOBAL: LEMBALL 0x00499060
// VsNetRuntimeChannelStack::`vbtable'{for `VsNetTimedEffStream'}
// GLOBAL: LEMBALL 0x00499068
// VsNetRuntimeChannelStack::`vbtable'{for `VsNetRuntimeChannelStack'}
// VTABLE: LEMBALL 0x004990d8 VsNetRuntimeChannelCore
// VTABLE: LEMBALL 0x004990e8 VsNetRuntimeChannelStack
struct VsNetRuntimeChannelStack : virtual VsNetRuntimeChannelCore {
	VsNetRuntimeChannelStack();
	inline void* ConstructEffTransportRuntimeChannelStack(int) { return this; }
	virtual short ClaimRuntimeFlag(const char* pszFlags);
	virtual void ClearRuntimeFlag(short nFlag);
	virtual void RuntimeReserved08(void* pUnused) = 0;
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

// GLOBAL: LEMBALL 0x00499108
// VsNetCompositeEffTransportStack::`vbtable'{for `VsNetRuntimeChannelCore'}
// GLOBAL: LEMBALL 0x00499118
// VsNetCompositeEffTransportStack::`vbtable'{for `CReadSocket'}
// GLOBAL: LEMBALL 0x00499120
// VsNetCompositeEffTransportStack::`vbtable'{for `VsNetTimedEffStream'}
// GLOBAL: LEMBALL 0x00499128
// VsNetCompositeEffTransportStack::`vbtable'{for `VsNetCompositeEffTransportStack'}
// VTABLE: LEMBALL 0x00499198 VsNetRuntimeChannelCore
// VTABLE: LEMBALL 0x004991a8 VsNetCompositeEffTransportStack
struct VsNetCompositeEffTransportStack : virtual VsNetRuntimeChannelCore {
protected:
	VsNetCompositeEffTransportStack();
	inline void* ConstructCompositeEffTransportStack(int) { return this; }

public:
	virtual int CompositeReserved00(const char* pszValue0, const char* pszValue1) = 0;
	virtual void ServicePeer(void);
	virtual void CompositeReserved08(void) = 0;
	virtual void CompositeReserved0c(int nArgument) = 0;
	virtual void CompositeReserved10(void) = 0;
	virtual void CompositeReserved14(void);
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
	inline void Release(void) { ((VsNetRuntimeChannelStack*) ((char*) this - 0x24))->~VsNetRuntimeChannelStack(); }
};

inline void* ConstructDualHandleEffStream(void* pStream, int)
{
	return pStream;
}

#endif
