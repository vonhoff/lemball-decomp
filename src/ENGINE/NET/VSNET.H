#ifndef LEMBALL_NETWORK_RUNTIME_H
#define LEMBALL_NETWORK_RUNTIME_H

#include "ENGINE/MEDIA/VSSTRM.H"
#include "ENGINE/CORE/VSMEM.H"

struct VsNetFileBackedChannelRoot {
	VsNetFileBackedChannelRoot();
	int m_nFileBackedRoot00;
	int m_nFileBackedRoot04;
	int m_nFileBackedRoot08;
};

class VsNetWin32FileWrapper {
public:
	inline ~VsNetWin32FileWrapper()
	{
		void* pOwnedPath = *(void**) &m_abFileState04[0];
		if (pOwnedPath != 0) {
			FreeVSMemBlock(pOwnedPath);
		}
	}
	virtual int OpenWithCreateFlag(const char* pszPath, int nAccessMode, int fCreateNew);
	virtual int OpenCreateNew(const char* pszPath, int nAccessMode);
	virtual int ProbeAccess(const char* pszPath);
	virtual int CloseIfOpen(void);
	virtual void FreeOwnedPath(void);
	virtual int Write(const void* pvSource, unsigned long cbWrite);
	virtual int Read(void* pvTarget, unsigned long cbRead);
	virtual int Seek(unsigned long dwOffset);
	virtual unsigned long GetLength(void);
	virtual unsigned long GetOffset(void);
	virtual int LockRange(unsigned long dwOffset, unsigned long cbRange);
	virtual int UnlockRange(unsigned long dwOffset, unsigned long cbRange);
	virtual void* GetOwnedPath(void);

	char m_abFileState04[0x10];
};

struct VsNetFileBackedFatalFacet :
	virtual VsNetEffStreamChannelState,
	virtual VsNetWin32FileWrapper {
	virtual ~VsNetFileBackedFatalFacet() = 0;
	virtual void FatalFileBackedOperation00(void) = 0;
	int m_nFatalFacet08;
};

inline VsNetFileBackedFatalFacet::~VsNetFileBackedFatalFacet() {}

struct VsNetFileBackedEffStream : VsNetEffStreamCommon {
	virtual void ReverseEffStreamPayload(void);
	virtual void WriteEffStreamTaggedHeader(void);

	void WriteEffStreamU16BE(unsigned short nValue)
	{
		((VsNetEffStreamBase*) this)->WriteEffStreamU16BE(nValue);
	}
	void ReadEffStreamU16BE(unsigned char* pbTarget)
	{
		((VsNetEffStreamBase*) this)->ReadEffStreamU16BE(pbTarget);
	}

	unsigned short m_nWord2c;
	unsigned short m_nWord2e;
};

struct VsNetTimedFileBackedEffChannel :
	VsNetFileBackedChannelRoot,
	virtual VsNetEffStreamChannelState,
	virtual VsNetTimedEffStream,
	virtual VsNetFileBackedFatalFacet {
	VsNetTimedFileBackedEffChannel();
	virtual ~VsNetTimedFileBackedEffChannel();
	virtual void ClearPendingWrite(void* pUnused);
	virtual void CaptureTimedSocketLookup(void* pArgument);
	virtual int SendTimedSocketPayload(void* pvPayload, int cbPayload);
	int m_nFileBackedMode10;
};

struct VsNetDualFileBackedEffChannel :
	VsNetFileBackedChannelRoot,
	virtual VsNetEffStreamChannelState,
	virtual VsNetDualHandleEffStream,
	virtual VsNetFileBackedFatalFacet {
	VsNetDualFileBackedEffChannel();
	virtual void ClearPendingWrite(void* pUnused);
	int m_nFileBackedMode10;
};

struct VsNetEmbeddedFileBackedEffChannelStack :
	virtual VsNetRuntimeChannelCore,
	virtual VsNetDualFileBackedEffChannel,
	virtual VsNetTimedFileBackedEffChannel {
	VsNetEmbeddedFileBackedEffChannelStack();
	virtual void ClearNestedEffChannelAsyncStatus(void);
	virtual void ClearPendingWrite(void* pUnused);
	virtual int SendTimedSocketPayload(void* pvPayload, int cbPayload);
};

/* Shared construction owner used by both complete file-backed composites.
 * The declaration edge and base order are significant: MSVC promotes the
 * runtime/composite sibling's vfptr to offset zero while retaining this
 * owner's vbptr and initial stream at +0x20/+0x24 (global) or +0x2c/+0x30
 * (dual-slot). */
struct VsNetFileBackedCompositeEffChannelAdapter :
	virtual VsNetEmbeddedFileBackedEffChannelStack {
	virtual int ReleaseFileBackedEffMarker(void);
	VsNetFileBackedEffStream m_InitialStream04;
};

struct VsNetGlobalStateFileBackedEffComposite :
	VsNetFileBackedCompositeEffChannelAdapter,
	VsNetRuntimeChannelStack {
	VsNetGlobalStateFileBackedEffComposite();
	virtual ~VsNetGlobalStateFileBackedEffComposite();
	virtual short ClaimRuntimeFlag(const char* pszFlags);
	virtual void ClearRuntimeFlag(short nFlag);
	virtual void RuntimeReserved08(void);
	virtual int RuntimeReserved0c(char* pszValue);
	virtual void ServiceRuntimeConnect(void);
	virtual void RuntimeReserved14(void);
	virtual void RuntimeReserved18(void);
	virtual void ClearPendingWrite(void* pUnused);
	virtual void FatalFileBackedOperation00(void);
	int m_fStreamLoaded54;
	DWORD m_dwGlobalStateServiceTick58;
};

struct VsNetDualSlotTableFileBackedEffComposite :
	VsNetFileBackedCompositeEffChannelAdapter,
	VsNetCompositeEffTransportStack {
	VsNetDualSlotTableFileBackedEffComposite();
	virtual ~VsNetDualSlotTableFileBackedEffComposite();
	virtual int CompositeReserved00(const char* pszPath08, const char* pszPath0c);
	virtual void ServicePeer(void);
	virtual void CompositeReserved08(void);
	virtual void CompositeReserved0c(int nArgument);
	virtual void CompositeReserved10(void);
	virtual void CompositeReserved14(void);
	virtual void ClearPendingWrite(void* pUnused);
	virtual void* ClaimRecord(void);
	virtual void FatalFileBackedOperation00(void);
};

struct VsNetPeerPayloadSender {
	virtual void Reserved00(void) = 0;
	virtual void Reserved04(void) = 0;
	virtual void Reserved08(void) = 0;
	virtual void Reserved0C(void) = 0;
	virtual void Reserved10(void) = 0;
	virtual void Reserved14(void) = 0;
	virtual void Reserved18(void) = 0;
	virtual void Reserved1C(void) = 0;
	virtual void WritePeerKey(void* pPeerKey) = 0;
	virtual int SendPayload(void* pvBuffer, int cbBuffer) = 0;
	virtual void SetAssignedPort(short nPort) = 0;

	int SendEffStreamPayloadWithTransportHeader(int nStream);
};

struct VsNetPeerPayloadSenderRaw {
	void* m_pVtable;
};

struct VsNetEffTransportPendingWriteState {};

#endif
