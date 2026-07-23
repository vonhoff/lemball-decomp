#include "ENGINE/CORE/SAFEVT.H"
#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/GDI/VSINVIS.H"
#include "ENGINE/MEDIA/VSSTRM.H"
#include "ENGINE/NET/VSNET.H"
#include "GAME.H"

#include <stdlib.h>
#include <string.h>
#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif

// FUNCTION: LEMBALL 0x00462cb0
// VsNetRuntimeChannelCore::`scalar deleting destructor'
// SYNTHETIC: LEMBALL 0x00471b00 symbol
// ??_EVsNetAllocatedTcpipEffTransportComposite@@$4PPPPPPPM@A@AEPAXI@Z
// FUNCTION: LEMBALL 0x00471b10
// VsNetAllocatedTcpipEffTransportComposite::`scalar deleting destructor'
// SYNTHETIC: LEMBALL 0x00471b90 symbol
// ??_EVsNetAllocatedTcpipEffTransportComposite@@$4PPPPPPPM@DA@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x00471b70 symbol
// ?ClearPendingWrite@VsNetAllocatedTcpipEffTransportComposite@@$4PPPPPPPM@A@AEXPAX@Z
// SYNTHETIC: LEMBALL 0x00471cb0 symbol
// ?HandleAsyncMessage@VsNetAllocatedTcpipEffTransportComposite@@$4PPPPPPPM@A@AEHHPAXI@Z
// SYNTHETIC: LEMBALL 0x00471ca0 symbol
// ?DualStreamRet8@VsNetCompositeEffTransportStack@@$4PPPPPPPM@3AEXHPAX@Z
// SYNTHETIC: LEMBALL 0x00471e30 symbol
// ?ClearPendingWrite@VsNetDualSocketWindowEffChannelBundle@@$4PPPPPPPM@A@AEXPAX@Z
// SYNTHETIC: LEMBALL 0x00471dd0 symbol
// ??_EVsNetDualSocketWindowEffChannelBundle@@$4PPPPPPPM@A@AEPAXI@Z
// FUNCTION: LEMBALL 0x00471de0
// VsNetDualSocketWindowEffChannelBundle::`scalar deleting destructor'

// SYNTHETIC: LEMBALL 0x00471a80 symbol
// ??_EVsNetSocketWindowEffChannel@@$4PPPPPPPM@A@AEPAXI@Z
// FUNCTION: LEMBALL 0x00471a90
// VsNetSocketWindowEffChannel::`scalar deleting destructor'
// SYNTHETIC: LEMBALL 0x00471cc0 symbol
// ??_EVsNetTcpipSocketChannelStackLayout@@$4PPPPPPPM@A@AEPAXI@Z
// FUNCTION: LEMBALL 0x00471cd0
// VsNetTcpipSocketChannelStackLayout::`scalar deleting destructor'
// SYNTHETIC: LEMBALL 0x00471d50 symbol
// ??_EVsNetTcpipSocketChannelStackLayout@@$4PPPPPPPM@DA@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x00471e70 symbol
// ??_EVsNetTimedSocketEffChannelBundleLayout@@$4PPPPPPPM@A@AEPAXI@Z
// FUNCTION: LEMBALL 0x00471e80
// VsNetTimedSocketEffChannelBundleLayout::`scalar deleting destructor'
// SYNTHETIC: LEMBALL 0x00471f00 symbol
// ??_EVsNetTimedSocketEffChannelBundleLayout@@$4PPPPPPPM@DA@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x00471f50 symbol
// ??_EVsNetTcpipEffTransportComposite@@$4PPPPPPPM@A@AEPAXI@Z
// FUNCTION: LEMBALL 0x00471f60
// VsNetTcpipEffTransportComposite::`scalar deleting destructor'
// SYNTHETIC: LEMBALL 0x00471fd0 symbol
// ??_EVsNetTcpipEffTransportComposite@@$4PPPPPPPM@DA@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x00471dc0 symbol
// ?ReleaseFileBackedEffMarker@VsNetSocketWindowEffChannel@@WPPPPPPFM@AEHXZ
// SYNTHETIC: LEMBALL 0x00471e20 symbol
// ?ClearNestedEffChannelAsyncStatus@VsNetSocketWindowEffChannel@@WPPPPPPFM@AEXXZ
// SYNTHETIC: LEMBALL 0x00471fb0 symbol
// ?ClearPendingWrite@VsNetTcpipEffTransportComposite@@$4PPPPPPPM@A@AEXPAX@Z
// SYNTHETIC: LEMBALL 0x00471e50 symbol
// ??_EVsNetDualSocketWindowEffChannelBundle@@WCM@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x00471bf0 symbol
// ?CaptureTimedSocketLookup@VsNetTimedSocketEffChannelBundleLayout@@WPPPPPOMA@AEXPAX@Z
// SYNTHETIC: LEMBALL 0x00471c00 symbol
// ?SendTimedSocketPayload@VsNetTimedSocketEffChannelBundleLayout@@WPPPPPOMA@AEHPAXH@Z
// SYNTHETIC: LEMBALL 0x00471c10 symbol
// ?SetTimedChannelWord@VsNetTimedSocketEffChannelBundleLayout@@WPPPPPOMA@AEXG@Z
// SYNTHETIC: LEMBALL 0x00471af0 symbol
// ?ReleaseFileBackedEffMarker@VsNetSocketWindowEffChannel@@WPPPPPONI@AEHXZ
// SYNTHETIC: LEMBALL 0x00471b60 symbol
// ?ClearNestedEffChannelAsyncStatus@VsNetSocketWindowEffChannel@@WPPPPPONI@AEXXZ
// SYNTHETIC: LEMBALL 0x00471c60 symbol
// ??_EVsNetAllocatedTcpipEffTransportComposite@@$4PPPPPPPM@KI@AEPAXI@Z
// SYNTHETIC: LEMBALL 0x00471e60 symbol
// ?ReleaseFileBackedEffMarker@VsNetSocketWindowEffChannel@@WPPPPPPDI@AEHXZ
// SYNTHETIC: LEMBALL 0x00471ec0 symbol
// ?ClearNestedEffChannelAsyncStatus@VsNetSocketWindowEffChannel@@WPPPPPPDI@AEXXZ
// SYNTHETIC: LEMBALL 0x00472000 symbol
// ??_EVsNetTcpipEffTransportComposite@@$4PPPPPPPM@KI@AEPAXI@Z

#ifdef ntohs
#undef ntohs
#endif
#ifdef htons
#undef htons
#endif

extern int PASCAL ReturnTrueVtableCallback(void* pObject);
extern int PASCAL ReturnTrueVtableCallbackThunk(void* pObject);
extern int ReturnTrueVtableCallbackSecondaryThunk(void);
extern void NoopVtableCallbackThunk(void);
extern void ReturnVoidVtableCallback(void);
extern void EffStreamChannelStateRet4Thunk(BYTE fDelete);

struct VsNetConstructionAdjustorVtable {
	void* m_pReserved00;
	int m_nPrimaryOffset;
	int m_nSecondaryOffset;
	int m_nTertiaryOffset;
	int m_nQuaternaryOffset;
	int m_nOffset14;
	int m_nOffset18;
	int m_nOffset1c;
};
extern void* g_NETWORK_TcpipSocketStackDualRet8;
void LEMBALL_FASTCALL TcpipSocketStackDualRet8(void* pObject, void* pUnused, BYTE fDelete, BYTE fReserved);
extern "C" int WINAPI
WSAAsyncGetHostByName(int nSocket, unsigned int nMessage, const char* pszHost, void* pBuffer, int cbBuffer);
struct VsNetPrefixedEffTransportControlView {
	inline void DispatchPrefixedEffTransportControlStream(int nDispatchType, void* pEvent)
	{
		((VsNetRuntimeChannelStack*) this)->VsNetRuntimeChannelStack::DualStreamRet8(nDispatchType, pEvent);
	}
};

struct VsNetAdjustorSubobject {
	int m_nThisDelta;
	void* m_pVtable;
};

struct VsNetCompleteEffTransportPendingWriteView {
	inline void CompleteEffTransportPendingWrite(int fQueueEvent)
	{
		((VsNetCompositeEffTransportStack*) this)
			->VsNetCompositeEffTransportStack::ClearPendingWrite((void*) fQueueEvent);
	}
};

struct VsNetThunkAdjustorGroup {
	VsNetConstructionAdjustorVtable* m_pOffsets;
	VsNetAdjustorSubobject* m_pPrimary;
	VsNetAdjustorSubobject* m_pSecondary;
	VsNetAdjustorSubobject* m_pTertiary;
};

struct VsNetQuadThunkAdjustorGroup {
	VsNetConstructionAdjustorVtable* m_pOffsets;
	VsNetAdjustorSubobject* m_pPrimary;
	VsNetAdjustorSubobject* m_pSecondary;
	VsNetAdjustorSubobject* m_pTertiary;
	VsNetAdjustorSubobject* m_pQuaternary;
};

class VsNetSocketWindowPrimary {
public:
	virtual int HandleAsyncMessage(int nMessage, void* pReserved, unsigned int nEvent) = 0;

	HWND m_hWnd;
	DWORD m_dwReserved0;
	DWORD m_dwReserved1;
};

class VsNetSocketWindowEffChannel :
	public VsNetSocketWindowPrimary,
	virtual public VsNetEffStreamChannelState {
public:
	virtual int HandleAsyncMessage(int nMessage, void* pReserved, unsigned int nEvent);
	virtual ~VsNetSocketWindowEffChannel();
	virtual int ReleaseFileBackedEffMarker(void);
	virtual void ClearNestedEffChannelAsyncStatus(void);

	int m_nSocketHandle;
	int m_nReserved18;

	VsNetSocketWindowEffChannel();
	inline void* ConstructSocketWindowEffChannel(int) { return this; }
};
inline void DestroySocketWindowEffChannel(int) {}

class VsNetSocketAsyncMessageAdapter {
public:
	int ReceiveUdpPayloadFromConnectedSocket(void);
	int ReceiveUdpPayloadFromSenderAddress(void);
	int HandleSocketReceiveAsyncSelectMessage(int nMessage, void* pReserved, unsigned int nEvent);
};

class VsNetSocketAsyncErrorAdapter {
public:
	int HandleSocketAsyncErrorLparam(void* pReserved, unsigned int nEvent, unsigned int* pBuffer);
};

class VsNetTcpipClientAsyncDispatcher {
public:
	virtual int DispatchTcpipClientAsyncMessage(int nMessage, void* pReserved, unsigned int nEvent) = 0;
};

class VsNetTcpipHostPortAsyncDispatcher {
public:
	int DispatchTcpipHostPortAsyncMessage(int nMessage, void* pReserved, unsigned int nEvent);
};

void* DeleteDualSocketWindowChannelStackWrapper(void* pObject, BYTE fFreeMemory);
void* DeleteCompositeTcpipSocketChannelStackWrapper(void* pObject, BYTE fFreeMemory);
void* DeleteTcpipSocketChannelStackWrapper(void* pObject, BYTE fFreeMemory);

struct VsNetTcpipSocketChannelStackWrapperView {
	VsNetEffStreamChannelStateLayout m_ChannelState00;
	unsigned char m_abChannelStateTail2c[4];
	unsigned char m_abTimedStream30[0x78];
	unsigned char m_abDualStreamA8[0x44];
	unsigned char m_abReservedec[0x3c];
	unsigned char m_abSocketWindow128[0x20];
	void* DeleteCompositeTcpipSocketChannelStackWrapper(BYTE fFreeMemory);
	void* DeleteTcpipSocketChannelStackWrapper(BYTE fFreeMemory);
	void* DeleteTcpipEffTransportCompositeWrapper(BYTE fFreeMemory);
};


class VsNetHostLookupCompletionAdapter {
public:
	void HandleRemoteHostLookupComplete(int nError);
	void HandleLocalHostLookupComplete(int nError);
	void HandleServicePortLookupComplete(int nError);
	void HandleRemoteServiceHostLookupComplete(int nError);
	void HandleRemoteServicePortLookupComplete(int nError);
};

class VsNetInetAddressEntry {
public:
	const char* GetAddressString(void);
	void SetAddressMode(int nMode);
	void StoreAddressString(const char* pszAddress);
	void CopyAddressEntry(const VsNetInetAddressEntry* pSource);
	int IsAddressLessOrEqual(const VsNetInetAddressEntry* pSource);
	int IsAddressGreaterOrEqual(const VsNetInetAddressEntry* pSource);
	char m_szAddress[16];
	int m_nAddress;
};

struct VsNetEffDispatchQueue {
	virtual void Reserved00(void) = 0;
	virtual void Reserved04(void) = 0;
	virtual void QueueEvent(void* pEvent) = 0;
};

class VsNetTimedSocketErrorChannel {
public:
	virtual void Reserved00(void) = 0;
	virtual void Reserved04(void) = 0;
	virtual void HandleSocketError(void) = 0;
};

class VsNetSocketChannelControl {
public:
	virtual void Reserved00(void) = 0;
	virtual void Reserved04(void) = 0;
	virtual void HandleSocketError(void) = 0;
	virtual void Reserved0c(void) = 0;
	virtual void Reserved10(void) = 0;
	virtual void Reserved14(void) = 0;
	virtual void Reserved18(void) = 0;
	virtual void Reserved1c(void) = 0;
	virtual void SetRemoteAddress(void* pAddress) = 0;
	virtual void Reserved24(void) = 0;
	virtual void SetAssignedPort(short nPort) = 0;
};

class VsNetTcpipLookupOwner {
public:
	virtual void Reserved00(void) = 0;
	virtual void Reserved04(void) = 0;
	virtual void Reserved08(void) = 0;
	virtual void Reserved0c(void) = 0;
	virtual void BeginPortSetup(void) = 0;
};

class VsNetTimedSocketPacketChannel {
public:
	virtual void Reserved00(void) = 0;
	virtual void Reserved04(void) = 0;
	virtual void Reserved08(void) = 0;
	virtual void Reserved0c(void) = 0;
	virtual void Reserved10(void) = 0;
	virtual void Reserved14(void) = 0;
	virtual void NotifyPacketReady(void) = 0;
};

class VsNetTimedSocketEffChannelBundleLayout :
	virtual public VsNetEffStreamChannelState,
	virtual public VsNetTimedEffStream,
	virtual public VsNetSocketWindowEffChannel {
public:
	short m_nMode04;
	unsigned char m_abReserved06[0x0e];

	VsNetTimedSocketEffChannelBundleLayout();
	virtual ~VsNetTimedSocketEffChannelBundleLayout() {}
	inline void* ConstructTimedSocketEffChannelBundle(int) { return this; }
	virtual void ClearPendingWrite(void* pArgument);
	virtual void CaptureTimedSocketLookup(void* pLookupResult);
	virtual int SendTimedSocketPayload(void* pBuffer, int cbBuffer);
	virtual void SetTimedChannelWord(unsigned short nPort);
	virtual int HandleAsyncMessage(int nMessage, void* pReserved, unsigned int nEvent);
};

struct VsNetTcpipEffTransportCompositeLayout {
	void** m_pVtable00;
	VsNetConstructionAdjustorVtable* m_pOuterOffsets04;
	int m_nRuntimeField08;
	int m_nRuntimeField0c;
	int m_nRuntimeField10;
	int m_nRuntimeField14;
	int m_nRuntimeField18;
	int m_nRuntimeField1c;
	VsNetConstructionAdjustorVtable* m_pTransportOffsets20;
	int m_nSocketWindowHandle24;
	int m_nLookupHandle28;
	int m_nReserved2c;
	VsNetEffStreamChannelStateLayout m_ChannelState30;
	unsigned char m_abTimedStream60[0x44];
	VsNetConstructionAdjustorVtable* m_pDualStreamConstructionOffsetsA4;
	unsigned char m_abTimedStreamA8[0x30];
	unsigned char m_abDualStreamD8[0x44];
	VsNetConstructionAdjustorVtable* m_pDualThunkOffsets11c;
	unsigned char m_abReserved120[0x10];
	VsNetConstructionAdjustorVtable* m_pSocketThunkOffsets130;
	unsigned char m_abReserved134[0x04];
	unsigned char m_abSocketWindow138[0x10];
	VsNetConstructionAdjustorVtable* m_pTimedSocketThunkOffsets148;
	unsigned char m_abSocketWindow14c[0x08];
	VsNetConstructionAdjustorVtable* m_pTimedSocketBundleOffsets154;
	void** m_pTimedSocketBundleConstructionData158;
	unsigned char m_abTimedSocketBundle15c[0x14];

	void* ConstructTcpipEffTransportComposite(int fConstructEmbeddedObjects);
};

struct VsNetTcpipCompositeControlVtableModel {
	void ConfigureRemoteHost(const char* pszHost);
	int StartLocalHostLookup(char* pszHost);
	void EnableAsyncSelect(void);
	void DisableAsyncSelect(void);
};

/* The allocator is a multiple-subobject layout.  Its metadata is not at the
 * offsets used by the standalone 00470270 composite. */
struct VsNetAllocatedTcpipEffTransportCompositeLayout {
	void** m_pVtable00;
	VsNetConstructionAdjustorVtable* m_pOuterOffsets04;
	unsigned char m_abReserved08[0x24];
	VsNetConstructionAdjustorVtable* m_pTransportOffsets2c;
	VsNetEffStreamChannelStateLayout m_ChannelState30;
	unsigned char m_abTimedStream60[0x44];
	unsigned char m_abReservedA4[4];
	VsNetConstructionAdjustorVtable* m_pDualStreamOffsetsA8;
	unsigned char m_abReservedAC[0x2c];
	unsigned char m_abDualStreamD8[0x44];
	unsigned char m_abReserved11c[4];
	VsNetConstructionAdjustorVtable* m_pDualThunkOffsets120;
	unsigned char m_abReserved124[0x10];
	VsNetConstructionAdjustorVtable* m_pSocketThunkOffsets134;
	unsigned char m_abReserved138[4];
	unsigned char m_abSocketWindow13c[0x10];
	VsNetConstructionAdjustorVtable* m_pTimedSocketThunkOffsets14c;
	unsigned char m_abReserved150[8];
	VsNetConstructionAdjustorVtable* m_pTimedSocketBundleOffsets158;
	void** m_pTimedSocketBundleConstructionData15c;
};

typedef VsNetRuntimeChannelCore VsNetTcpipRuntimeChannelFacet;

struct VsNetDualSocketWindowEffChannelBundle :
	virtual VsNetEffStreamChannelState,
	virtual VsNetDualHandleEffStream,
	virtual VsNetSocketWindowEffChannel {
	virtual void ClearPendingWrite(void* pArgument);
};

struct VsNetTcpipSocketChannelStackLayout :
	virtual VsNetEffStreamChannelState,
	virtual VsNetTimedEffStream,
	virtual VsNetDualHandleEffStream,
	virtual VsNetRuntimeChannelCore,
	virtual VsNetSocketWindowEffChannel,
	virtual VsNetDualSocketWindowEffChannelBundle,
	virtual VsNetTimedSocketEffChannelBundleLayout {
	VsNetTcpipSocketChannelStackLayout();
	inline void* ConstructTcpipSocketChannelStack(int) { return this; }
	virtual void ClearPendingWrite(void* pArgument);
	virtual int HandleAsyncMessage(int nMessage, void* pReserved, unsigned int nEvent);
};

/* 00470270 installs a second direct-base vbtable at +0x20.  Its two words
 * occupy +0x24/+0x28 and its seven virtual-base entries mirror the socket
 * stack construction view. */
struct VsNetTcpipEffTransportOwnershipPath :
	virtual VsNetEffStreamChannelState,
	virtual VsNetTimedEffStream,
	virtual VsNetDualHandleEffStream,
	virtual VsNetRuntimeChannelCore,
	virtual VsNetSocketWindowEffChannel,
	virtual VsNetDualSocketWindowEffChannelBundle,
	virtual VsNetTimedSocketEffChannelBundleLayout {
	virtual ~VsNetTcpipEffTransportOwnershipPath() = 0;
	virtual void ClearPendingWrite(void* pArgument) = 0;
	int m_nSocketHandle24;
	int m_nLookupHandle28;
};

inline VsNetTcpipEffTransportOwnershipPath::~VsNetTcpipEffTransportOwnershipPath()
{
}

/* 004715E0 constructs these two nonvirtual bases at +0 and +0x2c.  Their
 * virtual bases are shared by the complete 0x170-byte allocation. */
struct VsNetAllocatedTcpipEffTransportComposite :
	VsNetCompositeEffTransportStack,
	VsNetTcpipSocketChannelStackLayout {
	inline VsNetAllocatedTcpipEffTransportComposite() {}
	virtual int CompositeReserved00(const char* pUnused, const char* pszHost);
	virtual void CompositeReserved08(void);
	virtual void CompositeReserved0c(int nAddress);
	virtual void CompositeReserved10(void);
	virtual void CompositeReserved14(void);
	virtual void ClearPendingWrite(void* pArgument);
	virtual int HandleAsyncMessage(int nMessage, void* pReserved, unsigned int nEvent);
};

/* 00470270 is the complete TCP specialization of the 00460350 runtime
 * stack.  Its two local handles occupy +0x24/+0x28 before the shared
 * virtual-base tail. */
struct VsNetTcpipEffTransportComposite :
	VsNetRuntimeChannelStack,
	VsNetTcpipEffTransportOwnershipPath,
	virtual VsNetTcpipClientAsyncDispatcher {
	VsNetTcpipEffTransportComposite();
	~VsNetTcpipEffTransportComposite();
	virtual void ClearPendingWrite(void* pArgument);
	virtual int DispatchTcpipClientAsyncMessage(int nMessage, void* pReserved, unsigned int nEvent);
};

typedef VsNetCompositeEffTransportStack VsNetCompositeEffTransportStackLayout;

struct VsNetCompositeEffTransportStackWrapperView {
	VsNetEffStreamChannelStateLayout m_ChannelState00;
	unsigned char m_abChannelStateTail2c[4];
	unsigned char m_abTimedStream30[0x78];
	unsigned char m_abDualStreamA8[0x44];
};

struct VsNetEffTransportConnectCallback {
	inline void QueueEffTransportConnectEvent(void)
	{
		((VsNetCompositeEffTransportStack*) this)->VsNetCompositeEffTransportStack::DualStreamNoop();
	}
};

struct VsNetDualSocketWindowChannelStackWrapperView {
	VsNetEffStreamChannelStateLayout m_ChannelState00;
	unsigned char m_abDualStream2c[0x78];
	unsigned char m_abSocketWindowA4[0x20];
	void* DeleteDualSocketWindowChannelStackWrapper(BYTE fFreeMemory);
};

struct VsNetTimedSocketWindowChannelStackWrapperView {
	VsNetEffStreamChannelStateLayout m_ChannelState00;
	unsigned char m_abChannelStateTail2c[4];
	unsigned char m_abTimedStream30[0x98];
	unsigned char m_abSocketWindowC8[0x20];
	void* DeleteTimedSocketWindowChannelStackWrapper(BYTE fFreeMemory);
};

static int g_NETWORK_CompositeEffTransportChannelStateConstructionOffsets[5] = {
	-4,
	0x2c,
	0x5c,
	0xd4,
	0x12c,
};
static int g_NETWORK_CompositeEffTransportTimedStreamConstructionOffsets[5] = {
	-0x44,
	-0x74,
	-4,
	0x2c,
	0x5c,
};
static int g_NETWORK_CompositeEffTransportDualStreamConstructionOffsets[4] = {
	-0x44,
	-0xec,
	-0x44,
	-0x74,
};
static int g_NETWORK_CompositeEffTransportDualThunkConstructionOffsets[4] = {
	0,
	-0x100,
	-0xd0,
	-0x58,
};
void WriteEffTransportTaggedHeader(void* pObject);
void ReverseEffTransportPayload(void* pObject);
inline void LEMBALL_FASTCALL RestoreCompositeEffTransportVtables(int) {}
void DestroyTcpipEffTransportComposite(int nObjectBasePlus0x30);
void* AllocateTcpipInetAddressEntry(void);
void* AllocateTcpipEffTransportComposite(void);
void* AllocateTcpipEffTransportRuntimeOwner(void);
void* DeleteTcpipRuntimeStateWindowAdjustedThunk(void* pObject, BYTE fFreeMemory);
void* DeleteCompositeTcpipSocketChannelStackAdjustedThunk(void* pObject, BYTE fFreeMemory);
void CompleteEffTransportPendingWriteAllocatedAdjustedThunk(void* pObject, int fQueueEvent);
void CompleteEffTransportPendingWriteAllocatedThunkBody(void* pObject, int fQueueEvent);
void* DeleteCompositeTcpipSocketChannelStackTimedAdjustedThunk(void* pObject, BYTE fFreeMemory);
void LEMBALL_FASTCALL WriteCompositeTcpipGlobalSessionBody(void* pObject);
void LEMBALL_FASTCALL WriteCompositeTcpipGlobalSessionAdjustedThunk(void* pObject);
void* DispatchCompositeTcpipAckedPayloadAdjustedThunk(void* pObject);
void AllocatedTcpipCompositeCaptureLookupAdjustedThunk(void* pObject, void* pLookupResult);
int AllocatedTcpipCompositeSendUdpAdjustedThunk(void* pObject, void* pBuffer, int cbBuffer);
void AllocatedTcpipCompositeSetPortAdjustedThunk(void* pObject, short nPort);
void SetTcpipSocketChannelAssignedPort(void* pObject, short nPort);
void* DeleteAllocatedTcpipSocketChannelStackAdjustedThunk(void* pObject, BYTE fFreeMemory);
void LEMBALL_FASTCALL QueueAllocatedEffTransportConnectEventAdjustedThunk(void* pObject);
void LEMBALL_FASTCALL WriteAllocatedEffTransportGlobalSessionAdjustedThunk(void* pObject);
void* DispatchAllocatedAckedEffTransportPayloadAdjustedThunk(void* pObject);
void QueueAllocatedEffTransportPayloadEventAdjustedThunk(void* pObject, int nType, void* pPayload);
int DispatchAllocatedTcpipHostPortAsyncMessageAdjustedThunk(void* pObject,
															int nMessage,
															void* pReserved,
															unsigned int nEvent);
void* DeleteTcpipSocketStackPrimaryAdjustedThunk(void* pObject, BYTE fFreeMemory);
void* DeleteTcpipSocketStackCompositeAdjustedThunk(void* pObject, BYTE fFreeMemory);
void* LEMBALL_FASTCALL ClaimTcpipSocketStackCompositeBody(void* pObject);
void* LEMBALL_FASTCALL ClaimTcpipSocketStackCompositeAdjustedThunk(void* pObject);
void* DeleteTcpipSocketStackDualAdjustedThunk(void* pObject, BYTE fFreeMemory);
void* LEMBALL_FASTCALL ClaimTcpipSocketStackDualAdjustedThunk(void* pObject);
void TimedSocketBundleDispatchAsyncThunk(void* pObject, int nMessage, void* pReserved, unsigned int nEvent);
void LEMBALL_FASTCALL TimedSocketBundlePrimaryCloseThunk(void* pObject);
void* TimedSocketBundlePrimaryDeleteThunk(void* pObject, BYTE fDelete);
void LEMBALL_FASTCALL TimedSocketBundlePrimaryCaptureErrorThunk(void* pObject);
void TimedSocketBundlePrimaryNoopThunk(void* pObject, BYTE fDelete);
void TimedSocketBundlePrimaryNoopBody(BYTE fDelete);
void LEMBALL_FASTCALL TimedSocketBundleCloseSocketThunk(void* pObject);
void* TimedSocketBundleDeleteThunk(void* pObject, BYTE fDelete);
void LEMBALL_FASTCALL TimedSocketBundleCaptureErrorThunk(void* pObject);
void TimedSocketBundleClearPendingWriteThunk(void* pObject, void* pUnused);
void TimedSocketBundleClearPendingWriteBody(void* pObject, void* pUnused);
void* TimedSocketBundleDeleteAdjustedThunk(void* pObject, BYTE fDelete);
void TimedSocketBundleCaptureLookupAdjustedThunk(void* pObject, void* pLookupResult);
int TimedSocketBundleSendUdpAdjustedThunk(void* pObject, void* pBuffer, int cbBuffer);
void TimedSocketBundleSetAssignedPortThunk(void* pObject, short nPort);
void* DeleteDualSocketWindowChannelStackAdjustedThunk(void* pObject, BYTE fDelete);
int TimedSocketBundleConnectAdjustedThunk(void* pObject, int nMessage, void* pReserved, unsigned int nEvent);
void* TimedSocketBundleCompositeDeleteAdjustedThunk(void* pObject, BYTE fDelete);
void* DeleteCompositeTcpipEffTransportTimedAdjustedThunk(void* pObject, BYTE fFreeMemory);
void LEMBALL_FASTCALL WriteCompositeTcpipEffTransportGlobalSessionAdjustedThunk(void* pObject);
void* LEMBALL_FASTCALL ClaimCompositeTcpipEffTransportRecordAdjustedThunk(void* pObject);
void* DeleteCompositeTcpipEffTransportDualAdjustedThunk(void* pObject, BYTE fFreeMemory);
void LEMBALL_FASTCALL WriteCompositeTcpipEffTransportDualGlobalSessionAdjustedThunk(void* pObject);
void* LEMBALL_FASTCALL ClaimCompositeTcpipEffTransportDualRecordAdjustedThunk(void* pObject);
void DispatchCompositeTcpipPrefixedControlAdjustedThunk(void* pObject, int nDispatchType, void* pEvent);
int DispatchTcpipClientAsyncMessageAdjustedThunk(void* pObject, int nMessage, void* pReserved, unsigned int nEvent);

void ReverseEffTransportPayload(void* pObject);
void WriteEffTransportTaggedHeader(void* pObject);
/* Embedded channel-state construction data recovered from 0049a048.
 * Only first/primary offsets are consumed by this layout. */
/* Construction offsets recovered from Ghidra data 0049a078/70/68. */
void* DeleteTimedSocketWindowChannelStackWrapper(void* pObject, BYTE fDelete);
void* DeleteTcpipEffTransportCompositeWrapper(void* pObject, BYTE fDelete);
void* DeleteCompositeTcpipSocketChannelStackWrapper(void* pObject, BYTE fDelete);
struct VsNetCompleteEffTransportPendingWriteView;
struct VsNetDispatchAckedEffTransportPayloadView {
	inline void* DispatchAckedEffTransportPayloadEvent(void)
	{
		return ((VsNetCompositeEffTransportStack*) this)->VsNetCompositeEffTransportStack::ClaimRecord();
	}
};
struct VsNetQueueEffTransportPayloadEventView {
	inline void QueueEffTransportPayloadEvent(int nType, void* pPayload)
	{
		((VsNetCompositeEffTransportStack*) this)
			->VsNetCompositeEffTransportStack::DualStreamRet8(nType, pPayload);
	}
};
struct VsNetAckedEffTransportRecordOwner {
	char m_abUnknown00[0x4c];
	void* m_pRecordTable4c;
};
struct VsNetFileBackedDualSecondaryThunkVtableModel {
	void ReservedRet(void);
};
void AllocatedTcpipCompositeCaptureLookupAdjustedThunk(void* pObject, void* pLookupResult);
int AllocatedTcpipCompositeSendUdpAdjustedThunk(void* pObject, void* pBuffer, int cbBuffer);
void AllocatedTcpipCompositeSetPortAdjustedThunk(void* pObject, short nPort);

void SetTcpipSocketChannelAssignedPort(void* pObject, short nPort);

/* Raw table 0049a090: stream base slots, timed payload slots, then socket
 * adapter slots. */

/* Composite construction groups recovered from Ghidra 0049a138..0049a0d0. */
#if 0
static int g_NETWORK_TcpipCompositeTimedStreamVtable[8] = {
	-4,
	0x2c,
	0x5c,
	0xd4,
	0x12c,
	0x134,
	0x150,
	0x154,
};
static int g_NETWORK_TcpipCompositeTransportStackVtable[8] = {
	0,
	0x10,
	0x40,
	0xb8,
	0x110,
	0x118,
	0x134,
	0x138,
};
static int g_NETWORK_TcpipCompositeDualStreamVtable[4] = {
	-0x44,
	-0x74,
	0,
	0x10,
};
static int g_NETWORK_TcpipCompositeDualThunkVtable[4] = {
	-0x44,
	-0xec,
	-0x44,
	-0x74,
};
static int g_NETWORK_TcpipCompositeSocketWindowVtable[4] = {
	0,
	-0x100,
	-0xd0,
	-0x58,
};
static int g_NETWORK_TcpipCompositeTimedSocketThunkVtable[6] = {
	-0x10,
	-0x118,
	0,
	-0x100,
	-0xd0,
	-0x58,
};
static int g_NETWORK_TcpipCompositeTimedSocketBundleVtable[4] = {
	0,
	-0x124,
	-0x7c,
	-0x1c,
};
static int g_NETWORK_TcpipCompositeTimedSocketBundleDataVtable[4] = {
	0,
	-0x128,
	-0xf8,
	-0x20,
};
static void* g_NETWORK_TcpipCompositeFallbackThunkVtable[1] = {
	(void*) NetworkSafeVtableNoop,
};
/* 0049a158 is a two-base table.  Its suffix at +8 is the dual transport
 * table installed at 0049a160. */
static void* g_NETWORK_TcpipCompositeSocketThunkVtable[12] = {
	(void*) DispatchTcpipClientAsyncMessageAdjustedThunk,
	0,
	(void*) ReturnTrueVtableCallbackThunk,
	(void*) ReturnTrueVtableCallbackSecondaryThunk,
	(void*) ReverseEffTransportPayload,
	(void*) NoopVtableCallbackThunk,
	(void*) WriteEffTransportTaggedHeader,
	(void*) DeleteCompositeTcpipEffTransportDualAdjustedThunk,
	(void*) ReturnVoidVtableCallback,
	(void*) WriteCompositeTcpipEffTransportDualGlobalSessionAdjustedThunk,
	(void*) ClaimCompositeTcpipEffTransportDualRecordAdjustedThunk,
	(void*) DispatchCompositeTcpipPrefixedControlAdjustedThunk,
};
#endif
extern "C" int WINAPI socket(int nAddressFamily, int nType, int nProtocol);
extern "C" int WINAPI ioctlsocket(int nSocket, long nCommand, unsigned long* pValue);
extern "C" int WINAPI bind(int nSocket, const void* pAddress, int cbAddress);
extern "C" int WINAPI WSAAsyncSelect(int nSocket, void* hWnd, unsigned int nMessage, long nEvents);
extern "C" unsigned short WINAPI htons(unsigned short nValue);
extern "C" DWORD WINAPI timeGetTime(void);
extern void* g_pEffTransportRuntimeService;
extern short g_nEffTransportServiceBasePort;
extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);
struct VsNetEffDispatchEvent;

struct VsNetEffTransportRuntimeState {
	void DestroyEffTransportRuntimeGlobals(void);
	int ProcessEffTransportDispatchEvent(VsNetEffDispatchEvent* pEvent);
	void InitializeTcpipNetworkTransport(void);
	void ShutdownTcpipNetworkTransport(void);
	void CleanupEffTransportRuntimeState(void);
	void TcpipLoadFileRangeNoop(void);
	void TcpipWriteFileRangeNoop(void);
	void ServiceTcpipNetworkTransport(void);
	void WaitForFileBasedNetworkMessage(void);
};
int LEMBALL_FASTCALL HandleTcpipRuntimeWindowMessage(void* pObject,
													 void* pUnused,
													 unsigned int nMessage,
													 unsigned int nWParam,
													 unsigned int nLParam);
void* AllocateTcpipEffTransportComposite(void);
void* AllocateTcpipEffTransportRuntimeOwner(void);

/* These are the primary composite slots at 0049a3d8.  Keep them as real
   member functions: the binary calls them with the composite in ECX. */
#if 0
static void** g_NETWORK_TcpipCompositeDualRecoveredVtable = g_NETWORK_TcpipCompositeSocketThunkVtable + 2;
static void* g_NETWORK_TcpipCompositeOuterFatalThunkVtable[4] = {
	(void*) CaptureLastWinsockErrorStatusOuterDoubleAdjustedThunk,
	(void*) TimedSocketBundleCompositeDeleteAdjustedThunk,
	(void*) CaptureLastWinsockErrorStatusOuterAdjustedThunk,
	(void*) CloseAdjustedEffTransportPeerByKeyAdjustedThunk,
};
static void* g_NETWORK_TimedSocketBundlePrimaryThunkVtable[5] = {
	(void*) TimedSocketBundlePrimaryCloseThunk,
	(void*) TimedSocketBundlePrimaryDeleteThunk,
	(void*) TimedSocketBundlePrimaryCaptureErrorThunk,
	(void*) TimedSocketBundlePrimaryNoopThunk,
	(void*) TimedSocketBundleDispatchAsyncThunk,
};
/* Raw table 0049a1e4: receive adapter, shared stream prefix, dual-channel
 * methods, then socket-window methods. */
struct VsNetTimedSocketBundleTertiaryThunkVtableModel {
	int (VsNetSocketAsyncMessageAdapter::*m_pHandleReceive)(int, void*, unsigned int);
	void* m_pReturnTrue;
	void* m_pReturnTrueSecondary;
	void* m_pReversePayload;
	void* m_pNoop;
	void* m_pWriteTaggedHeader;
	void* m_pDeleteDual;
	void (VsNetFileBackedDualSecondaryThunkVtableModel::*m_pReservedRet)(void);
	void* m_pFatal0;
	void* m_pFatal1;
	void* m_pRet8;
	void* m_pCloseSocket;
	void* m_pDeletePrimary;
	void* m_pCaptureError;
	void* m_pNoopRet4;
	void* m_pDispatchAsync;
};

static VsNetTimedSocketBundleTertiaryThunkVtableModel g_NETWORK_TimedSocketBundleTertiaryThunkVtableData = {
	&VsNetSocketAsyncMessageAdapter::HandleSocketReceiveAsyncSelectMessage,
	(void*) ReturnTrueVtableCallbackThunk,
	(void*) ReturnTrueVtableCallbackSecondaryThunk,
	(void*) ReverseEffTransportPayload,
	(void*) NoopVtableCallbackThunk,
	(void*) WriteEffTransportTaggedHeader,
	(void*) DeleteDualSocketWindowChannelStackAdjustedThunk,
	&VsNetFileBackedDualSecondaryThunkVtableModel::ReservedRet,
	(void*) _purecall,
	(void*) _purecall,
	(void*) TcpipSocketStackDualRet8,
	(void*) TimedSocketBundlePrimaryCloseThunk,
	(void*) TimedSocketBundlePrimaryDeleteThunk,
	(void*) TimedSocketBundlePrimaryCaptureErrorThunk,
	(void*) TimedSocketBundlePrimaryNoopThunk,
	(void*) TimedSocketBundleDispatchAsyncThunk,
};
static void* g_NETWORK_TimedSocketBundleTertiaryThunkVtable = &g_NETWORK_TimedSocketBundleTertiaryThunkVtableData;
/* 0049a1e8 is suffix of 0049a1e4: skip receive-dispatch slot. */
static void* g_NETWORK_TimedSocketBundleSecondaryThunkVtable =
	(void**) &g_NETWORK_TimedSocketBundleTertiaryThunkVtableData + 1;
/* Socket-stack construction groups recovered from 0049a438..0049a3f0. */
static int g_NETWORK_TcpipSocketStackTimedStreamVtable[4] = {
	-0x44,
	-0x74,
	0,
	8,
};
static int g_NETWORK_TcpipSocketStackDualStreamVtable[4] = {
	-0x44,
	-0xec,
	-0x44,
	-0x74,
};
static int g_NETWORK_TcpipSocketStackBaseVtable[6] = {
	0,
	8,
	0x38,
	0xb0,
	0x108,
	0x110,
};
static int g_NETWORK_TcpipSocketStackDualThunkVtable[4] = {
	0,
	-0x100,
	-0xd0,
	-0x58,
};
static int g_NETWORK_TcpipSocketStackTimedSocketBundleVtable[4] = {
	-0x10,
	-0x118,
	0,
	-0x100,
};
static int g_NETWORK_TcpipSocketStackSocketWindowVtable[4] = {
	0,
	-0x124,
	-0x7c,
	-0x1c,
};
static int g_NETWORK_TcpipSocketStackTimedSocketDataVtable[4] = {
	0,
	-0x128,
	-0xf8,
	-0x20,
};

/* Raw table 0049a250: timed stream view of the TCP/IP socket stack. */
static void* g_NETWORK_TcpipSocketStackTimedThunkVtableData[11] = {
	(void*) ReturnTrueVtableCallbackThunk,
	(void*) ReturnTrueVtableCallbackSecondaryThunk,
	(void*) ReverseEffTransportPayload,
	(void*) NoopVtableCallbackThunk,
	(void*) WriteEffTransportTaggedHeader,
	(void*) DeleteTcpipSocketStackCompositeAdjustedThunk,
	(void*) WriteCompositeTcpipGlobalSessionAdjustedThunk,
	(void*) ClaimTcpipSocketStackCompositeAdjustedThunk,
	(void*) AllocatedTcpipCompositeCaptureLookupAdjustedThunk,
	(void*) AllocatedTcpipCompositeSendUdpAdjustedThunk,
	(void*) AllocatedTcpipCompositeSetPortAdjustedThunk,
};
static void* g_NETWORK_TcpipSocketStackTimedThunkVtable = g_NETWORK_TcpipSocketStackTimedThunkVtableData;

/* Keep this cross-TU table reference constant-initialized.  The original
 * binary's startup initializer consumes the pointer before the other socket
 * translation-unit initializers run; dereferencing the C++ model here leaves
 * a zero BSS value in the reconstructed image. */
struct VsNetTcpipSocketStackDualRet8VtableData {
	void* m_pRet8;
};

static VsNetTcpipSocketStackDualRet8VtableData g_NETWORK_TcpipSocketStackDualRet8VtableData = {
	(void*) TcpipSocketStackDualRet8,
};
void* g_NETWORK_TcpipSocketStackDualRet8 = &g_NETWORK_TcpipSocketStackDualRet8VtableData;
struct VsNetTcpipSocketStackDualDispatchVtableData {
	void* m_apMethods[9];
	void* m_pRet8;
};

static VsNetTcpipSocketStackDualDispatchVtableData g_NETWORK_TcpipSocketStackDualDispatchVtable = {
	{
		(void*) ReturnTrueVtableCallbackThunk,
		(void*) ReturnTrueVtableCallbackSecondaryThunk,
		(void*) ReverseEffTransportPayload,
		(void*) NoopVtableCallbackThunk,
		(void*) WriteEffTransportTaggedHeader,
		(void*) DeleteTcpipSocketStackDualAdjustedThunk,
		(void*) NoopVtableCallbackThunk,
		(void*) WriteAllocatedEffTransportGlobalSessionAdjustedThunk,
		(void*) ClaimTcpipSocketStackDualAdjustedThunk,
	},
	(void*) TcpipSocketStackDualRet8,
};
static void* g_NETWORK_TcpipSocketStackDualThunk = &g_NETWORK_TcpipSocketStackDualDispatchVtable;
static void* g_NETWORK_TcpipSocketStackSocketThunkVtable[2] = {
	(void*) TimedSocketBundleDispatchAsyncThunk,
	0,
};
static void* g_NETWORK_TcpipCompositeTimedThunkRecoveredVtable[12] = {
	(void*) ReturnTrueVtableCallbackThunk,
	(void*) ReturnTrueVtableCallbackSecondaryThunk,
	(void*) ReverseEffTransportPayload,
	(void*) NoopVtableCallbackThunk,
	(void*) WriteEffTransportTaggedHeader,
	(void*) DeleteTcpipSocketStackCompositeAdjustedThunk,
	(void*) WriteCompositeTcpipGlobalSessionAdjustedThunk,
	(void*) ClaimTcpipSocketStackCompositeAdjustedThunk,
	(void*) AllocatedTcpipCompositeCaptureLookupAdjustedThunk,
	(void*) AllocatedTcpipCompositeSendUdpAdjustedThunk,
	(void*) AllocatedTcpipCompositeSetPortAdjustedThunk,
	0,
};
static void* g_NETWORK_TcpipCompositeTimedThunkVtable = g_NETWORK_TcpipCompositeTimedThunkRecoveredVtable;
static void* g_NETWORK_TcpipCompositeOuterTimedThunkVtable = g_NETWORK_TcpipCompositeTimedThunkRecoveredVtable;
static void* g_NETWORK_TcpipCompositeOuterDualThunkVtable = g_NETWORK_TcpipCompositeSocketThunkVtable + 2;
static void* g_NETWORK_TcpipCompositeOuterSocketThunkVtable = g_NETWORK_TcpipCompositeSocketThunkVtable;

/* Allocation-composite construction groups recovered from 0049a348..e0. */
static int g_NETWORK_AllocatedTcpipCompositeTransportVtable[8] = {
	-4,
	0x30,
	0x60,
	0xd8,
	0x130,
	0x138,
	0x154,
	0x158,
};
static int g_NETWORK_AllocatedTcpipCompositeSocketStackVtable[8] = {
	0,
	8,
	0x38,
	0xb0,
	0x108,
	0x110,
	0x12c,
	0x130,
};
static int g_NETWORK_AllocatedTcpipCompositeDualStreamVtable[4] = {
	-0x44,
	-0x74,
	0,
	8,
};
static int g_NETWORK_AllocatedTcpipCompositeDualThunkVtable[4] = {
	-0x44,
	-0xec,
	-0x44,
	-0x74,
};
static int g_NETWORK_AllocatedTcpipCompositeSocketWindowVtable[4] = {
	0,
	-0x100,
	-0xd0,
	-0x58,
};
static int g_NETWORK_AllocatedTcpipCompositeTimedSocketBundleVtable[4] = {
	-0x10,
	-0x118,
	0,
	-0x100,
};
static int g_NETWORK_AllocatedTcpipCompositeTimedSocketThunkVtable[4] = {
	0,
	-0x124,
	-0x7c,
	-0x1c,
};
static int g_NETWORK_AllocatedTcpipCompositeTimedSocketDataVtable[4] = {
	0,
	-0x128,
	-0xf8,
	-0x20,
};
static void* g_NETWORK_AllocatedTcpipCompositeFatalThunkVtable[4] = {
	(void*) CaptureLastWinsockErrorStatusOuterDoubleAdjustedThunk,
	(void*) DeleteCompositeTcpipSocketChannelStackAdjustedThunk,
	(void*) CaptureLastWinsockErrorStatusOuterAdjustedThunk,
	(void*) CompleteEffTransportPendingWriteAllocatedAdjustedThunk,
};
static void* g_NETWORK_AllocatedTcpipCompositeTimedThunkVtable[11] = {
	(void*) ReturnTrueVtableCallbackThunk,
	(void*) ReturnTrueVtableCallbackSecondaryThunk,
	(void*) ReverseEffTransportPayload,
	(void*) NoopVtableCallbackThunk,
	(void*) WriteEffTransportTaggedHeader,
	(void*) DeleteCompositeTcpipSocketChannelStackTimedAdjustedThunk,
	(void*) WriteCompositeTcpipGlobalSessionAdjustedThunk,
	(void*) DispatchCompositeTcpipAckedPayloadAdjustedThunk,
	(void*) AllocatedTcpipCompositeCaptureLookupAdjustedThunk,
	(void*) AllocatedTcpipCompositeSendUdpAdjustedThunk,
	(void*) AllocatedTcpipCompositeSetPortAdjustedThunk,
};
static void* g_NETWORK_AllocatedTcpipCompositeDualThunk[10] = {
	(void*) ReturnTrueVtableCallbackThunk,
	(void*) ReturnTrueVtableCallbackSecondaryThunk,
	(void*) ReverseEffTransportPayload,
	(void*) NoopVtableCallbackThunk,
	(void*) WriteEffTransportTaggedHeader,
	(void*) DeleteAllocatedTcpipSocketChannelStackAdjustedThunk,
	(void*) QueueAllocatedEffTransportConnectEventAdjustedThunk,
	(void*) WriteAllocatedEffTransportGlobalSessionAdjustedThunk,
	(void*) DispatchAllocatedAckedEffTransportPayloadAdjustedThunk,
	(void*) QueueAllocatedEffTransportPayloadEventAdjustedThunk,
};
static void* g_NETWORK_AllocatedTcpipCompositeSocketThunkVtable[2] = {
	(void*) DispatchAllocatedTcpipHostPortAsyncMessageAdjustedThunk,
	(void*) NoopVtableCallbackThunk,
};
#endif

static const char* g_NETWORK_SocketWindowClassName;
static int* g_pfSocketWindowClassRegistered;
struct VsNetInetAddressEntryVtableData {
	const char* (VsNetInetAddressEntry::*m_pGetAddressString)(void);
	void (VsNetInetAddressEntry::*m_pSetAddressMode)(int);
	void (VsNetInetAddressEntry::*m_pStoreAddressString)(const char*);
	void (VsNetInetAddressEntry::*m_pCopyAddressEntry)(const VsNetInetAddressEntry*);
	int (VsNetInetAddressEntry::*m_pIsAddressLessOrEqual)(const VsNetInetAddressEntry*);
	int (VsNetInetAddressEntry::*m_pIsAddressGreaterOrEqual)(const VsNetInetAddressEntry*);
};

static VsNetInetAddressEntryVtableData g_NETWORK_InetAddressEntryVtableData = {
	&VsNetInetAddressEntry::GetAddressString,
	&VsNetInetAddressEntry::SetAddressMode,
	&VsNetInetAddressEntry::StoreAddressString,
	&VsNetInetAddressEntry::CopyAddressEntry,
	&VsNetInetAddressEntry::IsAddressLessOrEqual,
	&VsNetInetAddressEntry::IsAddressGreaterOrEqual,
};
static void* g_NETWORK_InetAddressEntryVtable = &g_NETWORK_InetAddressEntryVtableData;
extern void* g_pEffTransportRuntimeService;
extern "C" const char* WINAPI inet_ntoa(unsigned long nAddress);

/* 0049a280: four-slot socket front, six-slot address suffix, then the
 * fourteen-slot TCP/IP runtime-state suffix. */
struct VsNetTcpipSocketStackRuntimeRecoveredVtableData {
	void* m_pCaptureErrorOuterDouble;
	void* m_pDeletePrimary;
	void* m_pCaptureErrorOuter;
	void* m_pClearPendingWrite;
	const char* (VsNetInetAddressEntry::*m_pGetAddressString)(void);
	void (VsNetInetAddressEntry::*m_pSetAddressMode)(int);
	void (VsNetInetAddressEntry::*m_pStoreAddressString)(const char*);
	void (VsNetInetAddressEntry::*m_pCopyAddressEntry)(const VsNetInetAddressEntry*);
	int (VsNetInetAddressEntry::*m_pIsAddressLessOrEqual)(const VsNetInetAddressEntry*);
	int (VsNetInetAddressEntry::*m_pIsAddressGreaterOrEqual)(const VsNetInetAddressEntry*);
	void* m_pInitializeRenderEntry;
	void* m_pDeleteRuntimeWindow;
	int (VsNetEffTransportRuntimeState::*m_pProcess)(VsNetEffDispatchEvent*);
	void (VsNetEffTransportRuntimeState::*m_pInitializeTransport)(void);
	void (VsNetEffTransportRuntimeState::*m_pShutdownTransport)(void);
	void (VsNetEffTransportRuntimeState::*m_pCleanupRuntimeState)(void);
	void (VsNetEffTransportRuntimeState::*m_pLoadFileRange)(void);
	void (VsNetEffTransportRuntimeState::*m_pWriteFileRange)(void);
	void (VsNetEffTransportRuntimeState::*m_pServiceTransport)(void);
	void* (*m_pAllocateComposite)(void);
	void* (*m_pAllocateRuntimeOwner)(void);
	void* (*m_pAllocateAddress)(void);
	void (VsNetEffTransportRuntimeState::*m_pWaitForTransportMessage)(void);
	void* m_pHandleRuntimeWindowMessage;
};

#if 0
static VsNetTcpipSocketStackRuntimeRecoveredVtableData g_NETWORK_TcpipSocketStackRuntimeRecoveredVtable = {
	(void*) CaptureLastWinsockErrorStatusOuterDoubleAdjustedThunk,
	(void*) DeleteTcpipSocketStackPrimaryAdjustedThunk,
	(void*) CaptureLastWinsockErrorStatusOuterAdjustedThunk,
	(void*) ClearTcpipSocketStackPendingWriteAdjustedThunk,
	&VsNetInetAddressEntry::GetAddressString,
	&VsNetInetAddressEntry::SetAddressMode,
	&VsNetInetAddressEntry::StoreAddressString,
	&VsNetInetAddressEntry::CopyAddressEntry,
	&VsNetInetAddressEntry::IsAddressLessOrEqual,
	&VsNetInetAddressEntry::IsAddressGreaterOrEqual,
	(void*) InitializeNonZrleVariantRenderEntry,
	(void*) DeleteTcpipRuntimeStateWindowAdjustedThunk,
	&VsNetEffTransportRuntimeState::ProcessEffTransportDispatchEvent,
	&VsNetEffTransportRuntimeState::InitializeTcpipNetworkTransport,
	&VsNetEffTransportRuntimeState::ShutdownTcpipNetworkTransport,
	&VsNetEffTransportRuntimeState::CleanupEffTransportRuntimeState,
	&VsNetEffTransportRuntimeState::TcpipLoadFileRangeNoop,
	&VsNetEffTransportRuntimeState::TcpipWriteFileRangeNoop,
	&VsNetEffTransportRuntimeState::ServiceTcpipNetworkTransport,
	AllocateTcpipEffTransportComposite,
	AllocateTcpipEffTransportRuntimeOwner,
	AllocateTcpipInetAddressEntry,
	&VsNetEffTransportRuntimeState::WaitForFileBasedNetworkMessage,
	(void*) HandleTcpipRuntimeWindowMessage,
};
#endif
static const char* g_NETWORK_UdpServiceName;
static const char* g_NETWORK_TftpServiceName;

extern "C" DWORD WINAPI timeGetTime(void);
extern "C" int WINAPI
sendto(int nSocket, const char* pBuffer, int cbBuffer, int nFlags, const void* pTo, int nToLength);
extern "C" int WINAPI WSAGetLastError(void);
extern "C" int WINAPI closesocket(int nSocket);
extern "C" int WINAPI recv(int nSocket, char* pBuffer, int cbBuffer, int nFlags);
extern "C" int WINAPI recvfrom(int nSocket, char* pBuffer, int cbBuffer, int nFlags, void* pFrom, int* pcbFromLength);
extern "C" const char* WINAPI inet_ntoa(unsigned long nAddress);
extern "C" unsigned long WINAPI inet_addr(const char* pszAddress);
extern "C" int WINAPI socket(int nAddressFamily, int nType, int nProtocol);
extern "C" int WINAPI
WSAAsyncGetHostByName(int nSocket, unsigned int nMessage, const char* pszHost, void* pBuffer, int cbBuffer);
extern "C" int WINAPI WSACancelAsyncRequest(int nLookupHandle);
extern "C" int WINAPI gethostname(char* pszHost, int cbHost);
extern "C" int WINAPI WSAAsyncGetServByName(int nSocket,
											unsigned int nMessage,
											const char* pszService,
											const char* pszProtocol,
											void* pBuffer,
											int cbBuffer);
extern "C" unsigned short WINAPI ntohs(unsigned short nValue);
extern "C" unsigned short WINAPI htons(unsigned short nValue);
extern "C" int WINAPI setsockopt(int nSocket, int nLevel, int nOption, const char* pValue, int cbValue);
extern "C" int WINAPI bind(int nSocket, const void* pAddress, int cbAddress);
extern "C" int WINAPI WSAAsyncSelect(int nSocket, void* hWnd, unsigned int nMessage, long nEvents);

extern void* g_pEffTransportPeerAddressState;
extern void* g_pEffTransportDispatchQueue;
extern void* g_pEffTransportPacketBuffer;
extern void* g_pEffTransportRuntimeService;
extern int g_fFileBasedNetworkPathConfigured;
extern short g_nEffTransportServiceBasePort;
extern void QueueEffTransportErrorEvent(int nError);
extern int g_cbEffTransportCurrentPacketBytes;
extern int g_cbEffTransportMaxPacketBytes;
extern char* g_pszFileBasedNetworkLocalHostName;
struct VsNetEffTransportPeer {
	void PrepareEffTransportBroadcastStatusPayload(char* pszHostName);
};
static int* g_pEffTransportReceivedPacketBytes;

struct VsNetEffTransportPacketProcessor {
	int ProcessEffTransportPacketHeader(void);
};


// FUNCTION: LEMBALL 0x0045f790
void VsNetEffStreamBase::WriteEffStreamTaggedHeader(void)
{
	WriteEffStreamU32BE(*(unsigned int*) m_pTagBuffer2c);
	WriteEffStreamU32BE(*(unsigned int*) ((char*) m_pTagBuffer2c + 4));
	WriteEffStreamU16BE(*(unsigned short*) ((char*) m_pTagBuffer2c + 8));
	WriteEffStreamU16BE(*(unsigned short*) ((char*) m_pTagBuffer2c + 0xa));
	WriteEffStreamU16BE(*(unsigned short*) ((char*) m_pTagBuffer2c + 0xc));
	*(unsigned char*) (unsigned long) m_nReserved1c = *((unsigned char*) m_pTagBuffer2c + 0xe);
	++m_nReserved1c;
}
// FUNCTION: LEMBALL 0x0045f7f0
void VsNetEffStreamBase::ReverseEffStreamPayload(void)
{
	SwapEffStreamU32BEAndAdvance();
	SwapEffStreamU32BEAndAdvance();
	SwapEffStreamU16BEAndAdvance();
	SwapEffStreamU16BEAndAdvance();
	SwapEffStreamU16BEAndAdvance();
	AdvanceEffStreamReadCursorByte();
}
// FUNCTION: LEMBALL 0x00460a90
VsNetCompositeEffTransportStack::VsNetCompositeEffTransportStack()

{
	m_nField0c = 0;
	m_nField08 = 0;
	m_nField14 = 0;
	m_nField10 = 0;
	m_nField1c = 0;
	m_nField20 = 0;
	m_dwLastActivity24 = 0;
	m_nField18 = 0;
	m_dwServiceTick28 = timeGetTime();
}
// FUNCTION: LEMBALL 0x00460c00
VsNetCompositeEffTransportStack::~VsNetCompositeEffTransportStack()
{
}
// FUNCTION: LEMBALL 0x00462930
void LEMBALL_FASTCALL TcpipSocketStackDualRet8(void* pObject, void* pUnused, BYTE fDelete, BYTE fReserved)
{
	(void) pObject;
	(void) pUnused;
	(void) fDelete;
	(void) fReserved;
}
// FUNCTION: LEMBALL 0x004629d0
void VsNetAllocatedTcpipEffTransportComposite::CompositeReserved14(void)
{
	(void) 0;
}
// FUNCTION: LEMBALL 0x0046f9b0
void VsNetInetAddressEntry::SetAddressMode(int nMode)
{
	void** pVtable;

	if (nMode == 0) {
		pVtable = *(void***) this;
		((void (*)(void*)) pVtable[3])(g_pEffTransportRuntimeService);
		*(unsigned char*) ((char*) this + 0x17) = 0xff;
	}
	else if (nMode == 1) {
		*(int*) ((char*) this + 0x14) = -1;
	}
}
// FUNCTION: LEMBALL 0x0046f9f0
int VsNetInetAddressEntry::IsAddressGreaterOrEqual(const VsNetInetAddressEntry* pSource)

{
	return m_nAddress > pSource->m_nAddress;
}
// FUNCTION: LEMBALL 0x0046fcf0
VsNetSocketWindowEffChannel::VsNetSocketWindowEffChannel()
{
	((VS_InvisibleWindow*) this)->Construct(g_NETWORK_SocketWindowClassName, g_pfSocketWindowClassRegistered);
	m_nSocketHandle = 0;
	m_nReserved18 = 0;
	m_dwReserved0 = 0x440;
	m_dwReserved1 = 0x45f;
}
// FUNCTION: LEMBALL 0x0046fd70
VsNetSocketWindowEffChannel::~VsNetSocketWindowEffChannel()
{
	if (*(int*) ((char*) this - 0x08) != 0) {
		FreeVSMemBlock((void*) (unsigned long) *(int*) ((char*) this - 0x08));
	}
	DestroyInvisibleMessageWindow((VS_InvisibleWindow*) this);
}
// FUNCTION: LEMBALL 0x0046fdb0
int VsNetSocketAsyncErrorAdapter::HandleSocketAsyncErrorLparam(void* pReserved,
															   unsigned int nEvent,
															   unsigned int* pBuffer)

{
	unsigned int nError;
	int nChannelDelta;

	(void) pReserved;
	nError = nEvent >> 16;
	if (nError == 0) {
		return 0;
	}
	if (nError > 11000 && nError < 0x2afd) {
		return 2;
	}
	nChannelDelta = *(int*) (*(int*) ((char*) this + 0x10) + 4);
	((VsNetEffStreamChannelState*) ((char*) this + nChannelDelta + 0x10))
		->SetEffStreamChannelAsyncErrorStatus((int) nError);
	if (*pBuffer != 0) {
		FreeVSMemBlock((void*) (unsigned long) *pBuffer);
	}
	*pBuffer = 0;
	return 0x0e;
}
// FUNCTION: LEMBALL 0x0046fe10
int VsNetSocketAsyncMessageAdapter::ReceiveUdpPayloadFromSenderAddress(void)
{
	unsigned char abFrom[16];
	int cbFrom;
	int nResult;
	int nChannelDelta;
	char* pszAddress;
	VsNetInetAddressEntry* pPeerAddress;
	VsNetSocketChannelControl* pErrorChannel;

	cbFrom = 0x10;
	nChannelDelta = *(int*) (*(int*) this + 4);
	nResult = recvfrom(*(int*) ((char*) this + nChannelDelta + 8),
					   (char*) g_pEffTransportPacketBuffer,
					   g_cbEffTransportMaxPacketBytes,
					   0,
					   abFrom,
					   &cbFrom);
	g_cbEffTransportCurrentPacketBytes = nResult;
	pPeerAddress = (VsNetInetAddressEntry*) g_pEffTransportPeerAddressState;
	if (nResult == -1) {
		g_pErrorOutputStream->AppendCStringToStream("Receive error (after receive from):");
		g_pErrorOutputStream->AppendIntToStream((unsigned int) WSAGetLastError());
		g_pErrorOutputStream->AppendCStringToStream("\n");
		pErrorChannel = (VsNetSocketChannelControl*) ((char*) this + nChannelDelta);
		pErrorChannel->HandleSocketError();
		return 0;
	}
	pPeerAddress->m_nAddress = *(int*) abFrom;
	pszAddress = (char*) inet_ntoa((unsigned long) pPeerAddress->m_nAddress);
	strcpy(pPeerAddress->m_szAddress, pszAddress);
	return ((VsNetEffTransportPacketProcessor*) ((char*) this + *(int*) (*(int*) this + 8)))
		->ProcessEffTransportPacketHeader();
}
// FUNCTION: LEMBALL 0x0046fee0
int VsNetSocketAsyncMessageAdapter::ReceiveUdpPayloadFromConnectedSocket(void)
{
	VsNetTimedSocketErrorChannel* pErrorChannel;
	VsNetTimedSocketPacketChannel* pPacketChannel;
	int nThisDelta;
	int nResult;
	char* pbThis;

	pbThis = (char*) this;
	nThisDelta = *(int*) (*(int*) pbThis + 4);
	nResult =
		recv(*(int*) (pbThis + nThisDelta + 8), (char*) g_pEffTransportPacketBuffer, g_cbEffTransportMaxPacketBytes, 0);
	g_cbEffTransportCurrentPacketBytes = nResult;
	*(DWORD*) (*(int*) (*(int*) pbThis + 8) + (int) (unsigned long) pbThis + 0x3c) = timeGetTime();
	if (nResult == -1) {
		g_pErrorOutputStream->AppendCStringToStream("Receive error (after receive):");
		g_pErrorOutputStream->AppendIntToStream((unsigned int) WSAGetLastError());
		g_pErrorOutputStream->AppendCStringToStream("\n");
		pErrorChannel = (VsNetTimedSocketErrorChannel*) (pbThis + nThisDelta);
		pErrorChannel->HandleSocketError();
		return 0;
	}
	if (*(int*) (pbThis + nThisDelta + 0x10) == 0) {
		pPacketChannel = (VsNetTimedSocketPacketChannel*) (pbThis + *(int*) (*(int*) pbThis + 8));
		pPacketChannel->NotifyPacketReady();
	}
	*g_pEffTransportReceivedPacketBytes += g_cbEffTransportCurrentPacketBytes;
	return ((VsNetEffTransportPacketProcessor*) (pbThis + *(int*) (*(int*) pbThis + 8)))
		->ProcessEffTransportPacketHeader();
}
// FUNCTION: LEMBALL 0x0046ff90
int VsNetSocketAsyncMessageAdapter::HandleSocketReceiveAsyncSelectMessage(int nMessage,
																		  void* pReserved,
																		  unsigned int nEvent)

{
	int nThisDelta;
	unsigned int nError;
	VsNetSocketAsyncMessageAdapter* pReceiveChannel;

	(void) pReserved;
	if (nMessage == 0x443) {
		nThisDelta = *(int*) (*(int*) ((char*) this - 0x8c) + 4);
		if (*(int*) ((char*) this + nThisDelta - 0x84) == -1) {
			return 0;
		}
		nError = nEvent >> 16;
		((VsNetEffStreamChannelState*) ((char*) this + nThisDelta - 0x8c))
			->SetEffStreamChannelAsyncErrorStatus((int) nError);
		if ((nEvent & 0xffff) == 1) {
			pReceiveChannel = (VsNetSocketAsyncMessageAdapter*) ((char*) this - 0x8c);
			if (nError == 0) {
				pReceiveChannel->ReceiveUdpPayloadFromConnectedSocket();
				return 0;
			}
			g_pErrorOutputStream->AppendCStringToStream("Receive error:");
			g_pErrorOutputStream->AppendIntToStream(nError);
			g_pErrorOutputStream->AppendCStringToStream("\n");
			return 0;
		}
	}
	return -1;
}
// FUNCTION: LEMBALL 0x00470030
VsNetTimedSocketEffChannelBundleLayout::VsNetTimedSocketEffChannelBundleLayout()
{
	m_nMode04 = 2;
}
// FUNCTION: LEMBALL 0x004700f0
void VsNetTimedSocketEffChannelBundleLayout::CaptureTimedSocketLookup(void* pLookupResult)
{
	void* pTimedStream;

	pTimedStream =
		(void*) (unsigned long) (*(int*) (*(int*) ((char*) this - 0x48) + 8) + (int) (unsigned long) this - 0x48);
	((VsNetTimedEffStream*) pTimedStream)->InvokeTimedEffStreamServiceCallback(pLookupResult);
	*(int*) ((char*) this - 0x40) = *(int*) ((char*) pLookupResult + 0x14);
}
// FUNCTION: LEMBALL 0x00470120
int VsNetTimedSocketEffChannelBundleLayout::SendTimedSocketPayload(void* pBuffer, int cbBuffer)
{
	int nSocket;
	int nResult;
	VsNetTimedSocketErrorChannel* pChannel;
	char* pbThis;

	pbThis = (char*) this;
	nSocket = *(int*) (*(int*) (pbThis - 0x48) + 4);
	if (*(int*) (pbThis + nSocket - 0x24) == 0) {
		return 0;
	}

	nResult = sendto(*(int*) (pbThis + nSocket - 0x40),
					 (const char*) pBuffer,
					 cbBuffer,
					 0,
					 (const void*) (pbThis - 0x44),
					 0x10);
	*(DWORD*) (*(int*) (*(int*) (pbThis - 0x48) + 8) + (int) (unsigned long) pbThis - 0x0c) = timeGetTime();
	if (nResult == -1) {
		if (WSAGetLastError() == 0x2733) {
			return 0;
		}
		pChannel = (VsNetTimedSocketErrorChannel*) (pbThis + nSocket - 0x48);
		pChannel->HandleSocketError();
		return 0;
	}
	return 1;
}
// FUNCTION: LEMBALL 0x004701a0
int VsNetTimedSocketEffChannelBundleLayout::HandleAsyncMessage(int nMessage,
																			  void* pReserved,
																			  unsigned int nEvent)

{
	int nThisDelta;

	(void) pReserved;
	if (nMessage == 0x443) {
		nThisDelta = *(int*) (*(int*) ((char*) this - 0xc0) + 4);
		if (*(int*) ((char*) this + nThisDelta - 0xb8) == -1) {
			return -1;
		}
		((VsNetEffStreamChannelState*) ((char*) this + nThisDelta - 0xc0))
			->SetEffStreamChannelAsyncErrorStatus((int) (nEvent >> 16));
		if ((nEvent & 0xffff) == 2) {
			if ((nEvent >> 16) == 0) {
				*(int*) (*(int*) (*(int*) ((char*) this - 0xc0) + 4) + (int) (unsigned long) this - 0x9c) = 1;
			}
			return 0;
		}
	}
	return -1;
}
// FUNCTION: LEMBALL 0x00470220
int VsNetTcpipSocketChannelStackLayout::HandleAsyncMessage(int nMessage,
															void* pReserved,
															unsigned int nEvent)

{
	int nTimedDelta;
	int nReceiveDelta;
	int nResult;
	VsNetTimedSocketEffChannelBundleLayout* pTimedChannel;
	VsNetSocketAsyncMessageAdapter* pReceiveChannel;
	char* pbThis;

	pbThis = (char*) this;
	nTimedDelta = *(int*) (*(int*) (pbThis - 0x110) + 0x1c);
	pTimedChannel = (VsNetTimedSocketEffChannelBundleLayout*) (pbThis + nTimedDelta - 0x50);
	nResult = pTimedChannel->HandleAsyncMessage(nMessage, pReserved, nEvent);
	if (nResult == -1) {
		nReceiveDelta = *(int*) (*(int*) (pbThis - 0x110) + 0x18);
		pReceiveChannel = (VsNetSocketAsyncMessageAdapter*) (pbThis + nReceiveDelta - 0x84);
		return pReceiveChannel->HandleSocketReceiveAsyncSelectMessage(nMessage, pReserved, nEvent);
	}
	return nResult;
}
// DISABLED FUNCTION: LEMBALL 0x00470270
#if 0
void* VsNetTcpipEffTransportCompositeLayout::ConstructTcpipEffTransportComposite(int fConstructEmbeddedObjects)

{
	VsNetTcpipEffTransportCompositeLayout* pComposite;
	VsNetConstructionAdjustorVtable* pOffsets;
	VsNetAdjustorSubobject* pPrimaryThunk;
	VsNetAdjustorSubobject* pSecondaryThunk;
	VsNetAdjustorSubobject* pTertiaryThunk;
	VsNetAdjustorSubobject* pQuaternaryThunk;
	volatile unsigned int nConstructionFlags;

	pComposite = this;
	nConstructionFlags = 0;
	if (fConstructEmbeddedObjects != 0) {
		pComposite->m_pOuterOffsets04 = (VsNetConstructionAdjustorVtable*) g_NETWORK_TcpipCompositeTimedStreamVtable;
		pComposite->m_pTransportOffsets20 =
			(VsNetConstructionAdjustorVtable*) g_NETWORK_TcpipCompositeTransportStackVtable;
		pComposite->m_pDualStreamConstructionOffsetsA4 =
			(VsNetConstructionAdjustorVtable*) g_NETWORK_TcpipCompositeDualStreamVtable;
		pComposite->m_pDualThunkOffsets11c = (VsNetConstructionAdjustorVtable*) g_NETWORK_TcpipCompositeDualThunkVtable;
		pComposite->m_pSocketThunkOffsets130 =
			(VsNetConstructionAdjustorVtable*) g_NETWORK_TcpipCompositeSocketWindowVtable;
		pComposite->m_pTimedSocketThunkOffsets148 =
			(VsNetConstructionAdjustorVtable*) g_NETWORK_TcpipCompositeTimedSocketThunkVtable;
		pComposite->m_pTimedSocketBundleOffsets154 =
			(VsNetConstructionAdjustorVtable*) g_NETWORK_TcpipCompositeTimedSocketBundleVtable;
		pComposite->m_pTimedSocketBundleConstructionData158 =
			(void**) g_NETWORK_TcpipCompositeTimedSocketBundleDataVtable;

		nConstructionFlags |= 1;
		pComposite->m_ChannelState30.ConstructEffStreamChannelState();
		nConstructionFlags |= 2;
		((VsNetTimedEffStream*) pComposite->m_abTimedStream60)->ConstructTimedEffStream(0);
		nConstructionFlags |= 4;
		ConstructDualHandleEffStream(pComposite->m_abDualStreamD8, 0);

		nConstructionFlags |= 8;
		pOffsets = pComposite->m_pSocketThunkOffsets130;
		pPrimaryThunk = (VsNetAdjustorSubobject*) ((char*) pComposite + 0x12c + pOffsets->m_nPrimaryOffset);
		pSecondaryThunk = (VsNetAdjustorSubobject*) ((char*) pComposite + 0x12c + pOffsets->m_nSecondaryOffset);
		pTertiaryThunk = (VsNetAdjustorSubobject*) ((char*) pComposite + 0x12c + pOffsets->m_nTertiaryOffset);
		pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 8;
		pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x38;
		pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xb0;

		nConstructionFlags |= 0x10;
		((VsNetSocketWindowEffChannel*) pComposite->m_abSocketWindow138)->ConstructSocketWindowEffChannel(0);

		nConstructionFlags |= 0x20;
		pOffsets = pComposite->m_pTimedSocketBundleOffsets154;
		pPrimaryThunk = (VsNetAdjustorSubobject*) ((char*) pComposite + 0x150 + pOffsets->m_nPrimaryOffset);
		pSecondaryThunk = (VsNetAdjustorSubobject*) ((char*) pComposite + 0x150 + pOffsets->m_nSecondaryOffset);
		pTertiaryThunk = (VsNetAdjustorSubobject*) ((char*) pComposite + 0x150 + pOffsets->m_nTertiaryOffset);
		pPrimaryThunk->m_pVtable = g_NETWORK_TimedSocketBundlePrimaryThunkVtable;
		pSecondaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleSecondaryThunkVtable;
		pTertiaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleTertiaryThunkVtable;
		pPrimaryThunk->m_nThisDelta = 0;

		((VsNetTimedSocketEffChannelBundleLayout*) &pComposite->m_pTimedSocketBundleConstructionData158)
			->ConstructTimedSocketEffChannelBundle(0);
		nConstructionFlags |= 0x40;
	}

	((VsNetRuntimeChannelStack*) this)->ConstructEffTransportRuntimeChannelStack(0);
	pOffsets = pComposite->m_pTransportOffsets20;
	pPrimaryThunk = (VsNetAdjustorSubobject*) ((char*) pComposite + 0x1c + pOffsets->m_nPrimaryOffset);
	pSecondaryThunk = (VsNetAdjustorSubobject*) ((char*) pComposite + 0x1c + pOffsets->m_nSecondaryOffset);
	pTertiaryThunk = (VsNetAdjustorSubobject*) ((char*) pComposite + 0x1c + pOffsets->m_nTertiaryOffset);
	pQuaternaryThunk = (VsNetAdjustorSubobject*) ((char*) pComposite + 0x1c + pOffsets->m_nOffset14);
	pPrimaryThunk->m_pVtable = (void**) &g_NETWORK_TcpipSocketStackRuntimeRecoveredVtable;
	pSecondaryThunk->m_pVtable = g_NETWORK_TcpipCompositeTimedThunkVtable;
	pTertiaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackDualThunk;
	pQuaternaryThunk->m_pVtable = g_NETWORK_TcpipCompositeSocketThunkVtable;
	pPrimaryThunk->m_nThisDelta = 0;
	pSecondaryThunk->m_nThisDelta = 0;
	pTertiaryThunk->m_nThisDelta = 0;
	pQuaternaryThunk->m_nThisDelta = 0;

	pComposite->m_pVtable00 = (void**) &g_NETWORK_TcpipCompositeVtable;
	pOffsets = pComposite->m_pOuterOffsets04;
	pPrimaryThunk = (VsNetAdjustorSubobject*) ((char*) pComposite + pOffsets->m_nPrimaryOffset);
	pSecondaryThunk = (VsNetAdjustorSubobject*) ((char*) pComposite + pOffsets->m_nSecondaryOffset);
	pTertiaryThunk = (VsNetAdjustorSubobject*) ((char*) pComposite + pOffsets->m_nTertiaryOffset);
	pQuaternaryThunk = (VsNetAdjustorSubobject*) ((char*) pComposite + pOffsets->m_nOffset14);
	pPrimaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterFatalThunkVtable;
	pSecondaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterTimedThunkVtable;
	pTertiaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterDualThunkVtable;
	pQuaternaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterSocketThunkVtable;
	pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x2c;
	pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x5c;
	pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xd4;
	pQuaternaryThunk->m_nThisDelta = pOffsets->m_nOffset14 - 0x134;

	pComposite->m_nSocketWindowHandle24 = 0;
	pComposite->m_nLookupHandle28 = 0;
	return this;
}
#endif
// DISABLED FUNCTION: LEMBALL 0x004704e0
#if 0
void DestroyTcpipEffTransportComposite(int nObjectBasePlus0x30)
{
	VsNetTcpipEffTransportCompositeLayout* pComposite;
	VsNetConstructionAdjustorVtable* pOffsets;
	VsNetAdjustorSubobject* pPrimaryThunk;
	VsNetAdjustorSubobject* pSecondaryThunk;
	VsNetAdjustorSubobject* pTertiaryThunk;
	VsNetAdjustorSubobject* pQuaternaryThunk;
	char* pbObjectBase;

	pbObjectBase = (char*) (unsigned long) (nObjectBasePlus0x30 - 0x30);
	pComposite = (VsNetTcpipEffTransportCompositeLayout*) pbObjectBase;
	pComposite->m_pVtable00 = (void**) &g_NETWORK_TcpipCompositeVtable;
	pOffsets = pComposite->m_pOuterOffsets04;
	pPrimaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + pOffsets->m_nPrimaryOffset);
	pSecondaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + pOffsets->m_nSecondaryOffset);
	pTertiaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + pOffsets->m_nTertiaryOffset);
	pQuaternaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + pOffsets->m_nOffset14);
	pPrimaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterFatalThunkVtable;
	pSecondaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterTimedThunkVtable;
	pTertiaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterDualThunkVtable;
	pQuaternaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterSocketThunkVtable;
	pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x2c;
	pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x5c;
	pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xd4;
	pQuaternaryThunk->m_nThisDelta = pOffsets->m_nOffset14 - 0x134;

	if ((void*) (unsigned long) pComposite->m_nSocketWindowHandle24 != 0) {
		FreeVSMemBlock((void*) (unsigned long) pComposite->m_nSocketWindowHandle24);
	}

	((VsNetRuntimeChannelStackReleaseFront*) (unsigned long) (nObjectBasePlus0x30 - 0xc))->Release();
}
#endif
// FUNCTION: LEMBALL 0x00470580
void VsNetTcpipCompositeControlVtableModel::ConfigureRemoteHost(const char* pszHost)

{
	VsNetTcpipEffTransportCompositeLayout* pComposite;
	VsNetInetAddressEntry* pAddress;
	VsNetConstructionAdjustorVtable* pOffsets;
	VsNetAdjustorSubobject* pErrorChannel;
	int nAddress;
	int nResult;

	pComposite = (VsNetTcpipEffTransportCompositeLayout*) this;
	nAddress = (int) inet_addr(pszHost);
	if (nAddress != -1) {
		pAddress = (VsNetInetAddressEntry*) AllocateVSMemBlock(0x18);
		if (pAddress != 0) {
			*(void***) pAddress = (void**) g_NETWORK_InetAddressEntryVtable;
			pAddress->m_szAddress[0] = '\0';
			pComposite->m_nRuntimeField1c = (int) (unsigned long) pAddress;
		}
		else {
			pComposite->m_nRuntimeField1c = 0;
		}
		pAddress = (VsNetInetAddressEntry*) (unsigned long) pComposite->m_nRuntimeField1c;
		pAddress->StoreAddressString(pszHost);
		pAddress->GetAddressString();
		return;
	}

	if (pComposite->m_nLookupHandle28 != 0) {
		nResult = WSACancelAsyncRequest(pComposite->m_nLookupHandle28);
		if (nResult != 0) {
			pOffsets = pComposite->m_pOuterOffsets04;
			pErrorChannel = (VsNetAdjustorSubobject*) ((char*) pComposite + pOffsets->m_nSecondaryOffset + 4);
			((void (**)(void*)) pErrorChannel->m_pVtable)[2](pErrorChannel);
			return;
		}
		FreeVSMemBlock((void*) (unsigned long) pComposite->m_nSocketWindowHandle24);
		pComposite->m_nSocketWindowHandle24 = 0;
	}

	pComposite->m_nSocketWindowHandle24 = (int) (unsigned long) AllocateVSMemBlock(0x400);
	pOffsets = pComposite->m_pOuterOffsets04;
	pComposite->m_nLookupHandle28 = WSAAsyncGetHostByName(*(int*) ((char*) pComposite + pOffsets->m_nOffset14 + 8),
														  0x441,
														  pszHost,
														  (void*) (unsigned long) pComposite->m_nSocketWindowHandle24,
														  0x400);
}
// FUNCTION: LEMBALL 0x00470650
void VsNetHostLookupCompletionAdapter::HandleRemoteHostLookupComplete(int nError)
{
	char abAddress[20];
	char* pbLookup;
	char* pszAddress;
	VsNetInetAddressEntry* pEntry;
	VsNetEffDispatchQueue* pDispatchQueue;
	short cbAddress;
	unsigned char abEvent[20];

	pbLookup = *(char**) ((char*) this + 0x24);
	if (nError == 0) {
		cbAddress = *(short*) (pbLookup + 0x0a);
		memcpy(abAddress, *(void**) (*(int*) (pbLookup + 0x0c)), cbAddress);
		pEntry = (VsNetInetAddressEntry*) AllocateVSMemBlock(0x18);
		*(void***) pEntry = (void**) g_NETWORK_InetAddressEntryVtable;
		pEntry->m_szAddress[0] = '\0';
		*(char**) ((char*) this + 0x1c) = (char*) pEntry;
		pEntry->m_nAddress = *(int*) abAddress;
		pszAddress = (char*) inet_ntoa((unsigned long) pEntry->m_nAddress);
		strcpy(pEntry->m_szAddress, pszAddress);
		pEntry->GetAddressString();
	}
	else {
		g_pErrorOutputStream->AppendCStringToStream("Specified computer name not found\n");
		if (*(int*) ((char*) this + 0x0c) == 2) {
			*(unsigned short*) abEvent = 0x000d;
			*(int*) (abEvent + 8) = 0x0f;
			pDispatchQueue = (VsNetEffDispatchQueue*) g_pEffTransportDispatchQueue;
			pDispatchQueue->QueueEvent(abEvent);
		}
	}
	FreeVSMemBlock(pbLookup);
	*(int*) ((char*) this + 0x24) = 0;
}
// FUNCTION: LEMBALL 0x00470730
int VsNetTcpipCompositeControlVtableModel::StartLocalHostLookup(char* pszHost)
{
	VsNetTcpipEffTransportCompositeLayout* pComposite;
	VsNetConstructionAdjustorVtable* pOffsets;
	char szHost[0x100];
	int cbHost;
	int nLookupHandle;

	pComposite = (VsNetTcpipEffTransportCompositeLayout*) this;
	((VsNetEffTransportPeer*) this)->PrepareEffTransportBroadcastStatusPayload(pszHost);
	if (gethostname(szHost, sizeof(szHost)) == -1) {
		return 0;
	}

	cbHost = (int) strlen(szHost) + 1;
	if (g_pszFileBasedNetworkLocalHostName != 0) {
		FreeVSMemBlock(g_pszFileBasedNetworkLocalHostName);
	}
	g_pszFileBasedNetworkLocalHostName = (char*) AllocateVSMemBlock((unsigned int) cbHost);
	memcpy(g_pszFileBasedNetworkLocalHostName, szHost, (unsigned int) cbHost);

	pOffsets = pComposite->m_pOuterOffsets04;
	pComposite->m_nRuntimeField18 = (int) (unsigned long) AllocateVSMemBlock(0x400);
	*(int*) ((char*) pComposite + pOffsets->m_nPrimaryOffset + 0x18) = 1;
	nLookupHandle = WSAAsyncGetHostByName(*(int*) ((char*) pComposite + pOffsets->m_nOffset14 + 8),
										  0x440,
										  g_pszFileBasedNetworkLocalHostName,
										  (void*) (unsigned long) pComposite->m_nRuntimeField18,
										  0x400);
	pComposite->m_nLookupHandle28 = nLookupHandle;
	return nLookupHandle != 0;
}
// FUNCTION: LEMBALL 0x00470840
void VsNetHostLookupCompletionAdapter::HandleLocalHostLookupComplete(int nError)
{
	char abAddress[4];
	char* pbLookup;
	char* pszAddress;
	VsNetInetAddressEntry* pLocalAddress;
	VsNetTimedSocketErrorChannel* pErrorChannel;
	int nTimedDelta;
	int nSocketDelta;
	char* pbTimedSocket;
	char* pbSocketChannel;

	if (nError != 0) {
		g_pErrorOutputStream->AppendCStringToStream("Local host name not found\n");
	}
	else {
		nTimedDelta = *(int*) (*(int*) ((char*) this + 4) + 0x14);
		pbLookup = *(char**) ((char*) this + nTimedDelta + 0x18);
		memcpy(abAddress, *(void**) (*(int*) (pbLookup + 0x0c)), *(short*) (pbLookup + 0x0a));
		pLocalAddress = (VsNetInetAddressEntry*) g_pEffTransportRuntimeService;
		pLocalAddress->m_nAddress = *(int*) abAddress;
		pszAddress = (char*) inet_ntoa((unsigned long) pLocalAddress->m_nAddress);
		strcpy(pLocalAddress->m_szAddress, pszAddress);
		pLocalAddress->GetAddressString();
	}

	g_fFileBasedNetworkPathConfigured = 1;
	nTimedDelta = *(int*) (*(int*) ((char*) this + 4) + 0x14);
	FreeVSMemBlock(*(void**) ((char*) this + nTimedDelta + 0x18));
	*(int*) ((char*) this + nTimedDelta + 0x18) = 0;

	nSocketDelta = *(int*) (*(int*) ((char*) this + 4) + 4);
	pbSocketChannel = (char*) this + nSocketDelta;
	*(int*) (pbSocketChannel + 0x0c) = socket(2, 2, 0);
	if (*(int*) (pbSocketChannel + 0x0c) == -1) {
		pErrorChannel = (VsNetTimedSocketErrorChannel*) (pbSocketChannel + 4);
		pErrorChannel->HandleSocketError();
		QueueEffTransportErrorEvent(1);
		return;
	}

	*(int*) (pbSocketChannel + 0x10) = 1;
	pbTimedSocket = (char*) this + nTimedDelta;
	*(int*) (pbTimedSocket + 0x18) = (int) (unsigned long) AllocateVSMemBlock(0x400);
	*(int*) (pbSocketChannel + 0x18) = 1;
	*(int*) (pbTimedSocket + 0x1c) = WSAAsyncGetServByName(*(int*) (pbTimedSocket + 8),
														   0x442,
														   g_NETWORK_TftpServiceName,
														   g_NETWORK_UdpServiceName,
														   (void*) (unsigned long) *(int*) (pbTimedSocket + 0x18),
														   0x400);
	if (*(int*) (pbTimedSocket + 0x1c) == 0) {
		pErrorChannel = (VsNetTimedSocketErrorChannel*) (pbSocketChannel + 4);
		pErrorChannel->HandleSocketError();
		QueueEffTransportErrorEvent(2);
	}
}
// FUNCTION: LEMBALL 0x004709c0
void VsNetHostLookupCompletionAdapter::HandleServicePortLookupComplete(int nError)
{
	unsigned char abAddress[16];
	unsigned char abEvent[20];
	int nOuterDelta;
	int nSocketDelta;
	int nPort;
	int nResult;
	int nSocket;
	int nSocketOption;
	char* pbLookup;
	char* pbTimedSocket;
	char* pbSocketChannel;
	VsNetSocketChannelControl* pControl;
	VsNetEffDispatchQueue* pDispatchQueue;

	nSocketDelta = *(int*) (*(int*) ((char*) this + 4) + 8);
	pControl = (VsNetSocketChannelControl*) ((char*) this + nSocketDelta + 4);
	if (nError == 0) {
		nOuterDelta = *(int*) (*(int*) ((char*) this + 4) + 0x14);
		pbLookup = *(char**) ((char*) this + nOuterDelta + 0x18);
		nPort = (int) (unsigned short) ntohs(*(unsigned short*) (pbLookup + 8));
		pControl->SetAssignedPort((short) (nPort - g_nEffTransportServiceBasePort));
	}
	else {
		g_pErrorOutputStream->AppendCStringToStream("Failed to determine service port number\n");
		pControl->SetAssignedPort(0);
	}

	nOuterDelta = *(int*) (*(int*) ((char*) this + 4) + 0x14);
	FreeVSMemBlock(*(void**) ((char*) this + nOuterDelta + 0x18));
	*(int*) ((char*) this + nOuterDelta + 0x18) = 0;

	nSocketDelta = *(int*) (*(int*) ((char*) this + 4) + 4);
	pbSocketChannel = (char*) this + nSocketDelta;
	nSocket = *(int*) (pbSocketChannel + 0x0c);
	nSocketOption = 1;
	nResult = setsockopt(nSocket, -1, 0x20, (const char*) &nSocketOption, 4);
	if (nResult == -1) {
		((VsNetSocketChannelControl*) (pbSocketChannel + 4))->HandleSocketError();
		QueueEffTransportErrorEvent(3);
		return;
	}

	*(unsigned short*) abAddress = 2;
	*(unsigned short*) (abAddress + 2) = htons(
		(unsigned short) (*(short*) (*(int*) (*(int*) ((char*) this + 4) + 4) + (int) (unsigned long) this + 0x24) +
						  g_nEffTransportServiceBasePort));
	*(int*) (abAddress + 4) = *(int*) ((char*) g_pEffTransportRuntimeService + 0x14);
	nResult = bind(nSocket, abAddress, 0x10);
	if (nResult == -1) {
		((VsNetSocketChannelControl*) (pbSocketChannel + 4))->HandleSocketError();
		QueueEffTransportErrorEvent(4);
		return;
	}

	nOuterDelta = *(int*) (*(int*) ((char*) this + 4) + 0x14);
	pbTimedSocket = (char*) this + nOuterDelta;
	nResult = WSAAsyncSelect(nSocket,
							 (void*) (unsigned long) *(int*) (pbTimedSocket + 8),
							 0x443,
							 *(int*) ((char*) this + 0x14) != 0 ? 3 : 2);
	if (nResult == -1) {
		((VsNetSocketChannelControl*) (pbSocketChannel + 4))->HandleSocketError();
		QueueEffTransportErrorEvent(5);
		return;
	}

	*(int*) (pbSocketChannel + 0x14) = 1;
	*(int*) (pbSocketChannel + 0x18) = 0;
	*(int*) ((char*) this + 0x18) = (int) (timeGetTime() - 1000);
	*(unsigned short*) abEvent = 2;
	*(int*) (abEvent + 8) = 0;
	pDispatchQueue = (VsNetEffDispatchQueue*) g_pEffTransportDispatchQueue;
	pDispatchQueue->QueueEvent(abEvent);
}
// FUNCTION: LEMBALL 0x00470b90
int VsNetTcpipEffTransportComposite::DispatchTcpipClientAsyncMessage(int nMessage,
	void* pReserved,
	unsigned int nEvent)

{
	char* pbThis;
	int nDelta;
	int nResult;
	VsNetSocketAsyncErrorAdapter* pErrorAdapter;
	VsNetHostLookupCompletionAdapter* pLookupAdapter;
	VsNetSocketAsyncMessageAdapter* pReceiveAdapter;
	VsNetTcpipSocketChannelStackLayout* pSocketDispatcher;

	pbThis = (char*) this;
	switch (nMessage) {
	case 0x440:
		nDelta = *(int*) (*(int*) (pbThis - 0x134) + 0x14);
		pErrorAdapter = (VsNetSocketAsyncErrorAdapter*) (pbThis + nDelta - 0x134);
		nResult =
			pErrorAdapter->HandleSocketAsyncErrorLparam(pReserved, nEvent, (unsigned int*) (pbThis + nDelta - 0x120));
		if (nResult != 0x0e) {
			pLookupAdapter = (VsNetHostLookupCompletionAdapter*) (pbThis - 0x138);
			pLookupAdapter->HandleLocalHostLookupComplete(nResult == 2);
		}
		return 0;

	case 0x441:
		*(int*) (pbThis - 0x110) = 0;
		nDelta = *(int*) (*(int*) (pbThis - 0x134) + 0x14);
		pErrorAdapter = (VsNetSocketAsyncErrorAdapter*) (pbThis + nDelta - 0x134);
		nResult = pErrorAdapter->HandleSocketAsyncErrorLparam(pReserved, nEvent, (unsigned int*) (pbThis - 0x114));
		if (nResult != 0x0e) {
			pLookupAdapter = (VsNetHostLookupCompletionAdapter*) (pbThis - 0x138);
			pLookupAdapter->HandleRemoteHostLookupComplete(nResult == 2);
		}
		return 0;

	case 0x442:
		nDelta = *(int*) (*(int*) (pbThis - 0x134) + 0x14);
		pErrorAdapter = (VsNetSocketAsyncErrorAdapter*) (pbThis + nDelta - 0x134);
		nResult =
			pErrorAdapter->HandleSocketAsyncErrorLparam(pReserved, nEvent, (unsigned int*) (pbThis + nDelta - 0x120));
		if (nResult != 0x0e) {
			pLookupAdapter = (VsNetHostLookupCompletionAdapter*) (pbThis - 0x138);
			pLookupAdapter->HandleServicePortLookupComplete(nResult == 2);
		}
		return 0;

	case 0x443:
		nDelta = *(int*) (*(int*) (pbThis - 0x134) + 4);
		if (*(int*) (pbThis + nDelta - 0x12c) == -1) {
			return 0;
		}
		((VsNetEffStreamChannelState*) (pbThis + nDelta - 0x134))
			->SetEffStreamChannelAsyncErrorStatus((int) (nEvent >> 16));
		if ((nEvent & 0xffff) == 1) {
			if ((nEvent >> 16) == 0) {
				nDelta = *(int*) (*(int*) (pbThis - 0x134) + 0x18);
				pReceiveAdapter = (VsNetSocketAsyncMessageAdapter*) (pbThis + nDelta - 0x134);
				pReceiveAdapter->ReceiveUdpPayloadFromSenderAddress();
			}
			return 0;
		}
		break;
	}

	pSocketDispatcher = (VsNetTcpipSocketChannelStackLayout*) (pbThis - 8);
	return pSocketDispatcher->VsNetTcpipSocketChannelStackLayout::HandleAsyncMessage(
		nMessage, pReserved, nEvent);
}
// FUNCTION: LEMBALL 0x00470d30
void VsNetTcpipCompositeControlVtableModel::EnableAsyncSelect(void)
{
	VsNetTcpipEffTransportCompositeLayout* pComposite;
	VsNetConstructionAdjustorVtable* pOffsets;
	VsNetAdjustorSubobject* pSocketChannel;
	int nResult;

	pComposite = (VsNetTcpipEffTransportCompositeLayout*) this;
	if (pComposite->m_nRuntimeField14 != 0) {
		return;
	}
	pOffsets = pComposite->m_pOuterOffsets04;
	pSocketChannel = (VsNetAdjustorSubobject*) ((char*) pComposite + pOffsets->m_nSecondaryOffset + 4);
	if (*(int*) ((char*) pSocketChannel + 0x14) == 0) {
		return;
	}
	nResult = WSAAsyncSelect(*(int*) ((char*) pSocketChannel + 0x0c),
							 (void*) (unsigned long) *(int*) ((char*) pComposite + pOffsets->m_nOffset14 + 8),
							 0x443,
							 3);
	if (nResult == -1) {
		((void (**)(void*)) pSocketChannel->m_pVtable)[2](pSocketChannel);
		return;
	}
	pComposite->m_nRuntimeField14 = 1;
}
// FUNCTION: LEMBALL 0x00470d80
void VsNetTcpipCompositeControlVtableModel::DisableAsyncSelect(void)
{
	VsNetTcpipEffTransportCompositeLayout* pComposite;
	VsNetConstructionAdjustorVtable* pOffsets;
	VsNetAdjustorSubobject* pSocketChannel;
	int nResult;

	pComposite = (VsNetTcpipEffTransportCompositeLayout*) this;
	if (pComposite->m_nRuntimeField14 == 0) {
		return;
	}
	pOffsets = pComposite->m_pOuterOffsets04;
	pSocketChannel = (VsNetAdjustorSubobject*) ((char*) pComposite + pOffsets->m_nSecondaryOffset + 4);
	if (*(int*) ((char*) pSocketChannel + 0x14) == 0) {
		pComposite->m_nRuntimeField14 = 0;
		return;
	}
	nResult = WSAAsyncSelect(*(int*) ((char*) pSocketChannel + 0x0c),
							 (void*) (unsigned long) *(int*) ((char*) pComposite + pOffsets->m_nOffset14 + 8),
							 0x443,
							 2);
	if (nResult == -1) {
		((void (**)(void*)) pSocketChannel->m_pVtable)[2](pSocketChannel);
		return;
	}
	pComposite->m_nRuntimeField14 = 0;
}
// FUNCTION: LEMBALL 0x00470dd0
int VsNetAllocatedTcpipEffTransportComposite::CompositeReserved00(const char* pUnused,
																  const char* pszHost)
{
	VsNetAllocatedTcpipEffTransportCompositeLayout* pComposite;
	VsNetConstructionAdjustorVtable* pOffsets;
	char* pTimedSocket;
	VsNetSocketChannelControl* pErrorChannel;
	unsigned int cbHost;
	int nLookupHandle;

	(void) pUnused;
	pComposite = (VsNetAllocatedTcpipEffTransportCompositeLayout*) this;
	cbHost = (unsigned int) strlen(pszHost) + 1;
	*(char**) ((char*) pComposite + 0x0c) = (char*) AllocateVSMemBlock(cbHost);
	memcpy(*(char**) ((char*) pComposite + 0x0c), pszHost, cbHost);

	pOffsets = pComposite->m_pOuterOffsets04;
	pTimedSocket = (char*) pComposite + pOffsets->m_nOffset14;
	*(int*) (pTimedSocket + 0x18) = (int) (unsigned long) AllocateVSMemBlock(0x400);
	nLookupHandle = WSAAsyncGetHostByName(*(int*) (pTimedSocket + 8),
										  0x440,
										  pszHost,
										  (void*) (unsigned long) *(int*) (pTimedSocket + 0x18),
										  0x400);
	*(int*) (pTimedSocket + 0x1c) = nLookupHandle;
	if (nLookupHandle == 0) {
		pErrorChannel = (VsNetSocketChannelControl*) ((char*) pComposite + pOffsets->m_nPrimaryOffset + 4);
		pErrorChannel->HandleSocketError();
	}
	return nLookupHandle != 0;
}
// FUNCTION: LEMBALL 0x00470ed0
void VsNetHostLookupCompletionAdapter::HandleRemoteServiceHostLookupComplete(int nError)
{
	char abAddress[20];
	VsNetInetAddressEntry localAddress;
	VsNetSocketChannelControl* pSocketChannel;
	VsNetSocketChannelControl* pErrorChannel;
	VsNetTcpipLookupOwner* pOwner;
	char* pbLookup;
	char* pszAddress;
	int nTimedDelta;
	int nSocketDelta;
	int nPort;
	int nLookupResult;
	short cbAddress;

	if (nError != 0) {
		g_pErrorOutputStream->AppendCStringToStream("Computer specified was not found\n");
		return;
	}

	nTimedDelta = *(int*) (*(int*) ((char*) this + 4) + 0x14);
	pbLookup = *(char**) ((char*) this + nTimedDelta + 0x18);
	cbAddress = *(short*) (pbLookup + 0x0a);
	memcpy(abAddress, *(void**) (*(int*) (pbLookup + 0x0c)), cbAddress);
	*(void***) &localAddress = (void**) g_NETWORK_InetAddressEntryVtable;
	localAddress.m_szAddress[0] = '\0';
	localAddress.m_nAddress = *(int*) abAddress;
	pszAddress = (char*) inet_ntoa((unsigned long) localAddress.m_nAddress);
	strcpy(localAddress.m_szAddress, pszAddress);

	nSocketDelta = *(int*) (*(int*) ((char*) this + 4) + 8);
	pSocketChannel = (VsNetSocketChannelControl*) ((char*) this + nSocketDelta + 4);
	pSocketChannel->SetRemoteAddress(&localAddress);

	nPort = atoi(*(char**) ((char*) this + 0x0c));
	*(short*) ((char*) this + *(int*) (*(int*) ((char*) this + 4) + 4) + 0x24) = (short) nPort;
	pOwner = (VsNetTcpipLookupOwner*) this;
	if (*(short*) ((char*) this + *(int*) (*(int*) ((char*) this + 4) + 4) + 0x24) != 0) {
		pOwner->BeginPortSetup();
		return;
	}

	*(int*) ((char*) this + *(int*) (*(int*) ((char*) this + 4) + 4) + 0x18) = 1;
	nTimedDelta = *(int*) (*(int*) ((char*) this + 4) + 0x14);
	nLookupResult = WSAAsyncGetServByName(*(int*) ((char*) this + nTimedDelta + 8),
										  0x442,
										  *(const char**) ((char*) this + 0x0c),
										  "TCP",
										  (void*) (unsigned long) *(int*) ((char*) this + nTimedDelta + 0x18),
										  0x400);
	*(int*) ((char*) this + nTimedDelta + 0x1c) = nLookupResult;
	if (nLookupResult == 0) {
		nSocketDelta = *(int*) (*(int*) ((char*) this + 4) + 4);
		pErrorChannel = (VsNetSocketChannelControl*) ((char*) this + nSocketDelta + 4);
		pErrorChannel->HandleSocketError();
	}
}
// FUNCTION: LEMBALL 0x00471000
void VsNetHostLookupCompletionAdapter::HandleRemoteServicePortLookupComplete(int nError)
{
	int nOffsets;
	int nTimedDelta;
	int nPort;
	char* pbLookup;
	VsNetSocketChannelControl* pControl;

	nOffsets = *(int*) ((char*) this + 4);
	pControl = (VsNetSocketChannelControl*) ((char*) this + *(int*) (nOffsets + 8) + 4);
	if (nError == 0) {
		nTimedDelta = *(int*) (nOffsets + 0x14);
		pbLookup = *(char**) ((char*) this + nTimedDelta + 0x18);
		nPort = (int) (unsigned short) ntohs(*(unsigned short*) (pbLookup + 8));
		pControl->SetAssignedPort((short) (nPort - g_nEffTransportServiceBasePort));
	}
	else {
		pControl->SetAssignedPort(0);
		g_pErrorOutputStream->AppendCStringToStream("Service port number specified was not found\n");
	}

	nTimedDelta = *(int*) (nOffsets + 0x14);
	FreeVSMemBlock(*(void**) ((char*) this + nTimedDelta + 0x18));
	*(int*) ((char*) this + nTimedDelta + 0x18) = 0;
	if (nError == 0) {
		((VsNetTcpipLookupOwner*) this)->BeginPortSetup();
	}
}
// FUNCTION: LEMBALL 0x00471090
void VsNetAllocatedTcpipEffTransportComposite::CompositeReserved08(void)
{
	VsNetTcpipEffTransportCompositeLayout* pComposite;
	VsNetConstructionAdjustorVtable* pOffsets;
	VsNetAdjustorSubobject* pSocketChannel;
	int nSocket;
	unsigned long nNonblocking;

	pComposite = (VsNetTcpipEffTransportCompositeLayout*) this;
	nSocket = socket(2, 2, 0);
	pOffsets = pComposite->m_pOuterOffsets04;
	pSocketChannel = (VsNetAdjustorSubobject*) ((char*) pComposite + pOffsets->m_nSecondaryOffset + 4);
	*(int*) ((char*) pSocketChannel + 0x0c) = nSocket;
	if (nSocket == -1) {
		((VsNetSocketChannelControl*) pSocketChannel)->HandleSocketError();
		return;
	}
	*(int*) ((char*) pSocketChannel + 0x10) = 1;
	nNonblocking = 1;
	if (ioctlsocket(*(int*) ((char*) pSocketChannel + 0x0c), 0x8004667e, &nNonblocking) == -1) {
		((VsNetSocketChannelControl*) pSocketChannel)->HandleSocketError();
	}
}
// FUNCTION: LEMBALL 0x00471110
void VsNetAllocatedTcpipEffTransportComposite::CompositeReserved0c(int nAddress)
{
	VsNetTcpipEffTransportCompositeLayout* pComposite;
	VsNetConstructionAdjustorVtable* pOffsets;
	VsNetSocketChannelControl* pSocketChannel;
	unsigned char abAddress[16];
	int nSocket;

	pComposite = (VsNetTcpipEffTransportCompositeLayout*) this;
	CompositeReserved08();
	pOffsets = pComposite->m_pOuterOffsets04;
	pSocketChannel = (VsNetSocketChannelControl*) ((char*) this + pOffsets->m_nSecondaryOffset + 4);
	pSocketChannel->SetRemoteAddress((void*) nAddress);
	*(unsigned short*) abAddress = 2;
	*(unsigned short*) (abAddress + 2) =
		htons((unsigned short) (*(short*) ((char*) this + pOffsets->m_nSecondaryOffset + 0x24) +
								g_nEffTransportServiceBasePort));
	*(int*) (abAddress + 4) = *(int*) ((char*) g_pEffTransportRuntimeService + 0x14);
	nSocket = *(int*) ((char*) this + pOffsets->m_nSecondaryOffset + 0x0c);
	if (bind(nSocket, abAddress, 0x10) == -1) {
		pSocketChannel->HandleSocketError();
		return;
	}
	if (WSAAsyncSelect(nSocket,
					   (void*) (unsigned long) *(int*) ((char*) g_pEffTransportRuntimeService + 0x14),
					   0x443,
					   3) == -1) {
		pSocketChannel->HandleSocketError();
		return;
	}
	*(int*) ((char*) this + pOffsets->m_nSecondaryOffset + 0x1c) = 1;
	*(int*) ((char*) this + pOffsets->m_nSecondaryOffset + 0x20) = 1;
	*(int*) ((char*) this + 0x1c) = 0;
	*(int*) ((char*) this + pOffsets->m_nSecondaryOffset + 0x40) = (int) timeGetTime() - 1000;
	*(int*) ((char*) this + pOffsets->m_nSecondaryOffset + 0x44) = (int) timeGetTime();
}
// FUNCTION: LEMBALL 0x00471210
void VsNetAllocatedTcpipEffTransportComposite::CompositeReserved10(void)
{
	unsigned char abAddress[16];
	int nPrimaryDelta;
	int nTimedDelta;
	int nSocket;
	int nResult;
	VsNetSocketChannelControl* pErrorChannel;

	((VsNetCompositeEffTransportStack*) this)->CompositeReserved08();
	nPrimaryDelta = *(int*) (*(int*) ((char*) this + 4) + 4);
	*(unsigned short*) abAddress = 2;
	*(unsigned short*) (abAddress + 2) =
		htons((unsigned short) (*(short*) ((char*) this + nPrimaryDelta + 0x24) + g_nEffTransportServiceBasePort));
	*(int*) (abAddress + 4) = *(int*) ((char*) g_pEffTransportRuntimeService + 0x14);
	nSocket = *(int*) ((char*) this + nPrimaryDelta + 0x0c);
	nResult = bind(nSocket, abAddress, 0x10);
	if (nResult == -1) {
		pErrorChannel = (VsNetSocketChannelControl*) ((char*) this + nPrimaryDelta + 4);
		pErrorChannel->HandleSocketError();
		return;
	}

	nTimedDelta = *(int*) (*(int*) ((char*) this + 4) + 0x14);
	nResult = WSAAsyncSelect(nSocket, (void*) (unsigned long) *(int*) ((char*) this + nTimedDelta + 8), 0x443, 3);
	if (nResult == -1) {
		pErrorChannel = (VsNetSocketChannelControl*) ((char*) this + nPrimaryDelta + 4);
		pErrorChannel->HandleSocketError();
		return;
	}

	*(int*) ((char*) this + nPrimaryDelta + 0x20) = 1;
	*(int*) ((char*) this + 0x1c) = 1;
	*(int*) ((char*) this + *(int*) (*(int*) ((char*) this + 4) + 0x0c) + 0x40) = (int) timeGetTime();
}
// FUNCTION: LEMBALL 0x004712e0
int VsNetAllocatedTcpipEffTransportComposite::HandleAsyncMessage(
	int nMessage, void* pReserved, unsigned int nEvent)

{
	char* pbThis;
	int nDelta;
	int nResult;
	VsNetSocketAsyncErrorAdapter* pErrorAdapter;
	VsNetHostLookupCompletionAdapter* pLookupAdapter;
	VsNetTcpipSocketChannelStackLayout* pSocketDispatcher;

	pbThis = (char*) this;
	if (*(int*) (pbThis - 0x11c) != 0) {
		return -1;
	}
	if (nMessage != 0x440 && nMessage != 0x442) {
		pSocketDispatcher = (VsNetTcpipSocketChannelStackLayout*) this;
		return pSocketDispatcher->VsNetTcpipSocketChannelStackLayout::HandleAsyncMessage(
			nMessage, pReserved, nEvent);
	}
	else {
		nDelta = *(int*) (*(int*) (pbThis - 0x138) + 0x14);
		pErrorAdapter = (VsNetSocketAsyncErrorAdapter*) (pbThis + nDelta - 0x138);
		nResult =
			pErrorAdapter->HandleSocketAsyncErrorLparam(pReserved, nEvent, (unsigned int*) (pbThis + nDelta - 0x124));
		if (nResult != 0x0e) {
			pLookupAdapter = (VsNetHostLookupCompletionAdapter*) (pbThis - 0x13c);
			if (nMessage == 0x440) {
				pLookupAdapter->HandleRemoteServiceHostLookupComplete(nResult == 2);
			}
			else {
				pLookupAdapter->HandleRemoteServicePortLookupComplete(nResult == 2);
			}
		}
		return 0;
	}
}
// FUNCTION: LEMBALL 0x004714f0
int VsNetSocketWindowEffChannel::HandleAsyncMessage(int nMessage, void* pReserved, unsigned int nEvent)
{
	(void) nMessage;
	(void) pReserved;
	(void) nEvent;
	return -1;
}
// FUNCTION: LEMBALL 0x00471500
void VsNetInetAddressEntry::CopyAddressEntry(const VsNetInetAddressEntry* pSource)

{
	char* pszAddress;

	pszAddress = (char*) inet_ntoa((unsigned long) pSource->m_nAddress);
	m_nAddress = pSource->m_nAddress;
	strcpy(m_szAddress, pszAddress);
}
// FUNCTION: LEMBALL 0x00471550
void VsNetInetAddressEntry::StoreAddressString(const char* pszAddress)
{
	m_nAddress = (int) inet_addr(pszAddress);
	strcpy(m_szAddress, pszAddress);
}
// FUNCTION: LEMBALL 0x00471590
int VsNetInetAddressEntry::IsAddressLessOrEqual(const VsNetInetAddressEntry* pSource)

{
	return m_nAddress <= pSource->m_nAddress;
}
// FUNCTION: LEMBALL 0x004715b0
const char* VsNetInetAddressEntry::GetAddressString(void)
{
	return m_szAddress;
}
// FUNCTION: LEMBALL 0x004715c0
void* AllocateTcpipInetAddressEntry(void)
{
	VsNetInetAddressEntry* pObject;

	pObject = (VsNetInetAddressEntry*) AllocateVSMemBlock(0x18);
	if (pObject == 0) {
		return 0;
	}
	*(void***) pObject = (void**) g_NETWORK_InetAddressEntryVtable;
	pObject->m_szAddress[0] = 0;
	return pObject;
}
// FUNCTION: LEMBALL 0x004715e0
void* AllocateTcpipEffTransportComposite(void)
{
#if 0
	VsNetAllocatedTcpipEffTransportCompositeLayout* pComposite;
	VsNetConstructionAdjustorVtable* pOffsets;
	VsNetAdjustorSubobject* pPrimaryThunk;
	VsNetAdjustorSubobject* pSecondaryThunk;
	VsNetAdjustorSubobject* pTertiaryThunk;
	VsNetAdjustorSubobject* pQuaternaryThunk;
	char* pbObjectBase;

	pComposite = (VsNetAllocatedTcpipEffTransportCompositeLayout*) AllocateVSMemBlock(0x170);
	if (pComposite == 0) {
		return 0;
	}

	pbObjectBase = (char*) pComposite;
	pComposite->m_pOuterOffsets04 = (VsNetConstructionAdjustorVtable*) g_NETWORK_AllocatedTcpipCompositeTransportVtable;
	pComposite->m_pTransportOffsets2c =
		(VsNetConstructionAdjustorVtable*) g_NETWORK_AllocatedTcpipCompositeSocketStackVtable;
	pComposite->m_pDualStreamOffsetsA8 =
		(VsNetConstructionAdjustorVtable*) g_NETWORK_AllocatedTcpipCompositeDualStreamVtable;
	pComposite->m_pDualThunkOffsets120 =
		(VsNetConstructionAdjustorVtable*) g_NETWORK_AllocatedTcpipCompositeDualThunkVtable;
	pComposite->m_pSocketThunkOffsets134 =
		(VsNetConstructionAdjustorVtable*) g_NETWORK_AllocatedTcpipCompositeSocketWindowVtable;
	pComposite->m_pTimedSocketBundleOffsets158 =
		(VsNetConstructionAdjustorVtable*) g_NETWORK_AllocatedTcpipCompositeTimedSocketBundleVtable;
	pComposite->m_pTimedSocketThunkOffsets14c =
		(VsNetConstructionAdjustorVtable*) g_NETWORK_AllocatedTcpipCompositeTimedSocketThunkVtable;
	pComposite->m_pTimedSocketBundleConstructionData15c =
		(void**) g_NETWORK_AllocatedTcpipCompositeTimedSocketDataVtable;

	pComposite->m_ChannelState30.ConstructEffStreamChannelState();
	((VsNetTimedEffStream*) pComposite->m_abTimedStream60)->ConstructTimedEffStream(0);
	ConstructDualHandleEffStream(pComposite->m_abDualStreamD8, 0);

	pOffsets = pComposite->m_pSocketThunkOffsets134;
	pPrimaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + 0x134 + pOffsets->m_nPrimaryOffset - 4);
	pSecondaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + 0x134 + pOffsets->m_nSecondaryOffset - 4);
	pTertiaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + 0x134 + pOffsets->m_nTertiaryOffset - 4);
	pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 8;
	pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x38;
	pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xb0;

	((VsNetSocketWindowEffChannel*) pComposite->m_abSocketWindow13c)->ConstructSocketWindowEffChannel(0);

	pOffsets = pComposite->m_pTimedSocketBundleOffsets158;
	pPrimaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + 0x154 + pOffsets->m_nPrimaryOffset - 4);
	pSecondaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + 0x154 + pOffsets->m_nSecondaryOffset - 4);
	pTertiaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + 0x154 + pOffsets->m_nTertiaryOffset - 4);
	pPrimaryThunk->m_pVtable = g_NETWORK_TimedSocketBundlePrimaryThunkVtable;
	pSecondaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleSecondaryThunkVtable;
	pTertiaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleTertiaryThunkVtable;
	pPrimaryThunk->m_nThisDelta = 0;

	((VsNetTimedSocketEffChannelBundleLayout*) &pComposite->m_pTimedSocketBundleConstructionData15c)
		->ConstructTimedSocketEffChannelBundle(0);
	((VsNetCompositeEffTransportStackLayout*) pComposite)->ConstructCompositeEffTransportStack(0);
	((VsNetTcpipSocketChannelStackLayout*) ((char*) pComposite + 0x2c))->ConstructTcpipSocketChannelStack(0);

	pComposite->m_pVtable00 = (void**) &g_NETWORK_AllocatedTcpipCompositeVtable;
	pOffsets = pComposite->m_pOuterOffsets04;
	pPrimaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + pOffsets->m_nPrimaryOffset);
	pSecondaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + pOffsets->m_nSecondaryOffset);
	pTertiaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + pOffsets->m_nTertiaryOffset);
	pQuaternaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + pOffsets->m_nOffset14);
	pPrimaryThunk->m_pVtable = g_NETWORK_AllocatedTcpipCompositeFatalThunkVtable;
	pSecondaryThunk->m_pVtable = g_NETWORK_AllocatedTcpipCompositeTimedThunkVtable;
	pTertiaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackDualThunk;
	pQuaternaryThunk->m_pVtable = g_NETWORK_AllocatedTcpipCompositeSocketThunkVtable;
	pPrimaryThunk->m_nThisDelta = 0;
	pSecondaryThunk->m_nThisDelta = 0;
	pTertiaryThunk->m_nThisDelta = 0;
	pQuaternaryThunk->m_nThisDelta = 0;
	return pComposite;
#else
	void* pStorage = AllocateVSMemBlock(0x170);
	if (pStorage == 0) {
		return 0;
	}
	return new (pStorage) VsNetAllocatedTcpipEffTransportComposite;
#endif
}
// FUNCTION: LEMBALL 0x00471810
void* AllocateTcpipEffTransportRuntimeOwner(void)
{
	void* pStorage;

	pStorage = AllocateVSMemBlock(0x16c);
	if (pStorage == 0) {
		return 0;
	}
	return pStorage;
}
// FUNCTION: LEMBALL 0x00471830
VsNetTcpipSocketChannelStackLayout::VsNetTcpipSocketChannelStackLayout()
{
}

// FUNCTION: LEMBALL 0x00470270
VsNetTcpipEffTransportComposite::VsNetTcpipEffTransportComposite()
{
	m_nSocketHandle24 = 0;
	m_nLookupHandle28 = 0;
}

// FUNCTION: LEMBALL 0x004704e0
VsNetTcpipEffTransportComposite::~VsNetTcpipEffTransportComposite()
{
	if (m_nSocketHandle24 != 0) {
		FreeVSMemBlock((void*) (unsigned long) m_nSocketHandle24);
	}
}

// FUNCTION: LEMBALL 0x00471fc0
void VsNetTcpipEffTransportComposite::ClearPendingWrite(void* pArgument)
{
	VsNetRuntimeChannelStack::ClearPendingWrite(pArgument);
}

#if 0
void* VsNetTcpipSocketChannelStackLayout::ConstructTcpipSocketChannelStack(int fConstructEmbeddedObjects)

{
	VsNetTcpipSocketChannelStackLayout* pStack;
	VsNetConstructionAdjustorVtable* pOffsets;
	VsNetAdjustorSubobject* pPrimaryThunk;
	VsNetAdjustorSubobject* pSecondaryThunk;
	VsNetAdjustorSubobject* pTertiaryThunk;
	VsNetAdjustorSubobject* pQuaternaryThunk;
	char* pbObjectBase;

	pStack = this;
	pbObjectBase = (char*) this;

	if (fConstructEmbeddedObjects != 0) {
		pStack->m_pTimedStreamConstructionOffsets7c =
			(VsNetConstructionAdjustorVtable*) g_NETWORK_TcpipSocketStackTimedStreamVtable;
		pStack->m_pDualStreamConstructionOffsetsf4 =
			(VsNetConstructionAdjustorVtable*) g_NETWORK_TcpipSocketStackDualStreamVtable;
		pStack->m_pBaseOffsets00 = (VsNetConstructionAdjustorVtable*) g_NETWORK_TcpipSocketStackBaseVtable;
		pStack->m_pDualThunkOffsets108 = (VsNetConstructionAdjustorVtable*) g_NETWORK_TcpipSocketStackDualThunkVtable;
		pStack->m_pTimedSocketBundleConstructionData130 = (void**) g_NETWORK_TcpipSocketStackTimedSocketDataVtable;
		pStack->m_pTimedSocketBundleConstructionOffsets120 =
			(VsNetConstructionAdjustorVtable*) g_NETWORK_TcpipSocketStackTimedSocketBundleVtable;
		pStack->m_pSocketWindowConstructionOffsets12c =
			(VsNetConstructionAdjustorVtable*) g_NETWORK_TcpipSocketStackSocketWindowVtable;

		pStack->m_ChannelState08.ConstructEffStreamChannelState();
		((VsNetTimedEffStream*) pStack->m_abTimedStream38)->ConstructTimedEffStream(0);
		ConstructDualHandleEffStream(pStack->m_abDualStreamB0, 0);

		pOffsets = pStack->m_pDualThunkOffsets108;
		pPrimaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + 0x104 + pOffsets->m_nPrimaryOffset);
		pSecondaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + 0x104 + pOffsets->m_nSecondaryOffset);
		pTertiaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + 0x104 + pOffsets->m_nTertiaryOffset);
		pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
		pSecondaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackTimedStreamVtable;
		pTertiaryThunk->m_pVtable = (void**) &g_NETWORK_RuntimeChannelStackDualStreamVtable;
		pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 8;
		pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x38;
		pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xb0;

		((VsNetSocketWindowEffChannel*) pStack->m_abSocketWindow110)->ConstructSocketWindowEffChannel(0);

		pOffsets = pStack->m_pSocketWindowConstructionOffsets12c;
		pPrimaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + 0x128 + pOffsets->m_nPrimaryOffset);
		pSecondaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + 0x128 + pOffsets->m_nSecondaryOffset);
		pTertiaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + 0x128 + pOffsets->m_nTertiaryOffset);
		pPrimaryThunk->m_pVtable = g_NETWORK_TimedSocketBundlePrimaryThunkVtable;
		pSecondaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleSecondaryThunkVtable;
		pTertiaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleTertiaryThunkVtable;
		pPrimaryThunk->m_nThisDelta = 0;

		((VsNetTimedSocketEffChannelBundleLayout*) &pStack->m_pTimedSocketBundleConstructionData130)
			->ConstructTimedSocketEffChannelBundle(0);
	}

	pOffsets = pStack->m_pBaseOffsets00;
	pPrimaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + pOffsets->m_nPrimaryOffset - 4);
	pSecondaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + pOffsets->m_nSecondaryOffset - 4);
	pTertiaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + pOffsets->m_nTertiaryOffset - 4);
	pQuaternaryThunk = (VsNetAdjustorSubobject*) (pbObjectBase + pOffsets->m_nOffset14 - 4);
	pPrimaryThunk->m_pVtable = (void**) &g_NETWORK_TcpipSocketStackRuntimeRecoveredVtable;
	pSecondaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackTimedThunkVtable;
	pTertiaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackDualThunk;
	pQuaternaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackSocketThunkVtable;
	pPrimaryThunk->m_nThisDelta = 0;
	pSecondaryThunk->m_nThisDelta = 0;
	pTertiaryThunk->m_nThisDelta = 0;
	pQuaternaryThunk->m_nThisDelta = 0;
	return this;
}
#endif
// FUNCTION: LEMBALL 0x00471a10
void* DeleteTcpipRuntimeStateWindowAdjustedThunk(void* pObject, BYTE fFreeMemory)

{
	char* pWindowBase;

	pWindowBase = (char*) pObject - 0x10;
	((VsNetEffTransportRuntimeState*) pWindowBase)->DestroyEffTransportRuntimeGlobals();
	DestroyInvisibleMessageWindow((VS_InvisibleWindow*) pWindowBase);
	if ((fFreeMemory & 1) != 0) {
		FreeVSMemBlock(pWindowBase);
	}
	return pWindowBase;
}
// FUNCTION: LEMBALL 0x00471a60
int VsNetSocketWindowEffChannel::ReleaseFileBackedEffMarker(void)
{
	int nDelta;

	nDelta = *(int*) (*(int*) ((char*) this - 0x10) + 4);
	return closesocket(*(int*) ((char*) this + nDelta - 8));
}
// FUNCTION: LEMBALL 0x00471ad0
void VsNetSocketWindowEffChannel::ClearNestedEffChannelAsyncStatus(void)
{
	int nDelta;

	nDelta = *(int*) (*(int*) ((char*) this - 0x10) + 4);
	((VsNetEffStreamChannelState*) ((char*) this + nDelta - 0x10))
		->SetEffStreamChannelAsyncErrorStatus(WSAGetLastError());
}
// DISABLED FUNCTION: LEMBALL 0x00471b00
#if 0
void* DeleteCompositeTcpipSocketChannelStackAdjustedThunk(void* pObject, BYTE fFreeMemory)

{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4);
	return ((VsNetTcpipSocketChannelStackWrapperView*) pObject)
		->DeleteCompositeTcpipSocketChannelStackWrapper(fFreeMemory);
}
// DISABLED FUNCTION: LEMBALL 0x00471b10
void* VsNetTcpipSocketChannelStackWrapperView::DeleteCompositeTcpipSocketChannelStackWrapper(BYTE fFreeMemory)

{
	VsNetTcpipSocketChannelStackWrapperView* pView;
	char* pbAllocationBase;
	void* pObject;

	pObject = (void*) this;
	pView = this;
	pbAllocationBase = (char*) this - 0x34;
	RestoreCompositeEffTransportVtables((int) (unsigned long) pObject - 4);
	DestroySocketWindowEffChannel((int) (unsigned long) pView->m_abSocketWindow128);
	((VsNetDualHandleEffStream*) pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
	((VsNetTimedEffStream*) pView->m_abTimedStream30)->DestroyTimedEffStream();
	pView->m_ChannelState00.DestroyEffStreamChannelState();
	if ((fFreeMemory & 1) != 0) {
		FreeVSMemBlock(pbAllocationBase);
	}
	return pbAllocationBase;
}
#endif
// DISABLED FUNCTION: LEMBALL 0x00471b70
#if 0
void CompleteEffTransportPendingWriteAllocatedAdjustedThunk(void* pObject, int fQueueEvent)

{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4);
	CompleteEffTransportPendingWriteAllocatedThunkBody(pObject, fQueueEvent);
}
#endif
// FUNCTION: LEMBALL 0x00471b80
void VsNetAllocatedTcpipEffTransportComposite::ClearPendingWrite(void* pArgument)

{
	VsNetCompositeEffTransportStack::ClearPendingWrite(pArgument);
}
// DISABLED FUNCTION: LEMBALL 0x00471b90
#if 0
void* DeleteCompositeTcpipSocketChannelStackTimedAdjustedThunk(void* pObject, BYTE fFreeMemory)

{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4) - 0x30;
	return ((VsNetTcpipSocketChannelStackWrapperView*) pObject)
		->DeleteCompositeTcpipSocketChannelStackWrapper(fFreeMemory);
}
#endif
// FUNCTION: LEMBALL 0x00471ba0
void LEMBALL_FASTCALL WriteCompositeTcpipGlobalSessionAdjustedThunk(void* pObject)
{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4);
	WriteCompositeTcpipGlobalSessionBody(pObject);
}
// FUNCTION: LEMBALL 0x00471bb0
void LEMBALL_FASTCALL WriteCompositeTcpipGlobalSessionBody(void* pObject)
{
	char* pAdjusted;
	int* pOffsets;

	pAdjusted = (char*) pObject;
	pOffsets = *(int**) (pAdjusted - 0x38);
	pAdjusted += pOffsets[4];
	pOffsets = *(int**) (pAdjusted - 0x38);
	((VsNetRuntimeChannelCore*) (pAdjusted + pOffsets[2] - 0x38))
		->VsNetRuntimeChannelCore::WriteGlobalSession();
}
// FUNCTION: LEMBALL 0x00471bd0
void* DispatchCompositeTcpipAckedPayloadAdjustedThunk(void* pObject)
{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4) - 4;
	return ((VsNetDispatchAckedEffTransportPayloadView*) pObject)->DispatchAckedEffTransportPayloadEvent();
}
// LEGACY THUNK: LEMBALL 0x00471bf0
#if 0
void AllocatedTcpipCompositeCaptureLookupAdjustedThunk(void* pObject, void* pLookupResult)

{
	((VsNetTimedSocketEffChannelBundleLayout*) ((char*) pObject + 0x140))
		->CaptureTimedSocketLookup(pLookupResult);
}
// LEGACY THUNK: LEMBALL 0x00471c00
int AllocatedTcpipCompositeSendUdpAdjustedThunk(void* pObject, void* pBuffer, int cbBuffer)

{
	return ((VsNetTimedSocketEffChannelBundleLayout*) ((char*) pObject + 0x140))
		->SendTimedSocketPayload(pBuffer, cbBuffer);
}
// LEGACY THUNK: LEMBALL 0x00471c10
void AllocatedTcpipCompositeSetPortAdjustedThunk(void* pObject, short nPort)

{
	((VsNetTimedSocketEffChannelBundleLayout*) ((char*) pObject + 0x140))->SetTimedChannelWord(nPort);
}
#endif
// FUNCTION: LEMBALL 0x00471c20
void VsNetTimedSocketEffChannelBundleLayout::SetTimedChannelWord(unsigned short nPort)
{
	char* pbObject;
	int nOffsets;
	int nChannelDelta;
	int nFieldDelta;
	int nBaseDelta;
	unsigned short nSocketPort;

	pbObject = (char*) this;
	nOffsets = *(int*) (pbObject - 0x48);
	nChannelDelta = *(int*) (nOffsets + 8);
	nFieldDelta = *(int*) (*(int*) (pbObject + nChannelDelta - 4) + 4);
	*(short*) (pbObject + nFieldDelta + nChannelDelta - 0x48 + 0x64) = nPort;

	nBaseDelta = *(int*) (nOffsets + 4);
	nSocketPort = *(unsigned short*) (pbObject + nBaseDelta - 0x28);
	*(unsigned short*) (pbObject - 0x42) = htons((unsigned short) (nSocketPort + g_nEffTransportServiceBasePort));
}
// DISABLED FUNCTION: LEMBALL 0x00471c60
#if 0
void* DeleteAllocatedTcpipSocketChannelStackAdjustedThunk(void* pObject, BYTE fFreeMemory)
{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4) - 0xa8;
	return ((VsNetTcpipSocketChannelStackWrapperView*) pObject)
		->DeleteCompositeTcpipSocketChannelStackWrapper(fFreeMemory);
}
#endif
// DISABLED FUNCTION: LEMBALL 0x00471c70
#if 0
#pragma optimize("g", on)
void LEMBALL_FASTCALL QueueAllocatedEffTransportConnectEventAdjustedThunk(void* pObject)
{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4);
	pObject = (char*) pObject - 4;
	((VsNetEffTransportConnectCallback*) pObject)->QueueEffTransportConnectEvent();
}
#endif
#pragma optimize("", on)
// FUNCTION: LEMBALL 0x00471c80
#pragma optimize("g", on)
void LEMBALL_FASTCALL WriteAllocatedEffTransportGlobalSessionAdjustedThunk(void* pObject)
{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4);
	pObject = (char*) pObject - 0x78;
	WriteCompositeTcpipGlobalSessionBody(pObject);
}
#pragma optimize("", on)
// FUNCTION: LEMBALL 0x00471c90
void* DispatchAllocatedAckedEffTransportPayloadAdjustedThunk(void* pObject)
{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4) - 0x7c;
	return ((VsNetDispatchAckedEffTransportPayloadView*) pObject)->DispatchAckedEffTransportPayloadEvent();
}
// DISABLED FUNCTION: LEMBALL 0x00471ca0
#if 0
void QueueAllocatedEffTransportPayloadEventAdjustedThunk(void* pObject, int nType, void* pPayload)

{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4) - 4;
	((VsNetQueueEffTransportPayloadEventView*) pObject)->QueueEffTransportPayloadEvent(nType, pPayload);
}
#endif
// DISABLED FUNCTION: LEMBALL 0x00471cb0
#if 0
int DispatchAllocatedTcpipHostPortAsyncMessageAdjustedThunk(void* pObject,
															int nMessage,
															void* pReserved,
															unsigned int nEvent)

{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4);
	return ((VsNetTcpipHostPortAsyncDispatcher*) pObject)
		->DispatchTcpipHostPortAsyncMessage(nMessage, pReserved, nEvent);
}
#endif
// DISABLED FUNCTION: LEMBALL 0x00471cc0
#if 0
void* DeleteTcpipSocketStackPrimaryAdjustedThunk(void* pObject, BYTE fFreeMemory)

{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4);
	return ((VsNetTcpipSocketChannelStackWrapperView*) pObject)->DeleteTcpipSocketChannelStackWrapper(fFreeMemory);
}
// DISABLED FUNCTION: LEMBALL 0x00471cd0
void* VsNetTcpipSocketChannelStackWrapperView::DeleteTcpipSocketChannelStackWrapper(BYTE fFreeMemory)

{
	VsNetTcpipSocketChannelStackWrapperView* pView;
	char* pbAllocationBase;
	void* pObject;

	pObject = (void*) this;
	pView = this;
	pbAllocationBase = (char*) this - 8;
	DestroySocketWindowEffChannel((int) (unsigned long) pView->m_abSocketWindow128);
	((VsNetDualHandleEffStream*) pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
	((VsNetTimedEffStream*) pView->m_abTimedStream30)->DestroyTimedEffStream();
	pView->m_ChannelState00.DestroyEffStreamChannelState();
	if ((fFreeMemory & 1) != 0) {
		FreeVSMemBlock(pbAllocationBase);
	}
	return pbAllocationBase;
}
#endif
// FUNCTION: LEMBALL 0x00471d30
void VsNetTcpipSocketChannelStackLayout::ClearPendingWrite(void* pUnused)

{
	char* pAdjusted;
	int* pOffsets;

	pAdjusted = (char*) this + 8;
	pOffsets = *(int**) (pAdjusted - 8);
	pAdjusted += pOffsets[4];
	pOffsets = *(int**) (pAdjusted - 8);
	((VsNetTimedEffStream*) (unsigned long) (pAdjusted + pOffsets[2] - 8))
		->VsNetTimedEffStream::ClearPendingWrite(pUnused);
}
// DISABLED FUNCTION: LEMBALL 0x00471d50
#if 0
void* DeleteTcpipSocketStackCompositeAdjustedThunk(void* pObject, BYTE fFreeMemory)

{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4) - 0x30;
	return ((VsNetTcpipSocketChannelStackWrapperView*) pObject)->DeleteTcpipSocketChannelStackWrapper(fFreeMemory);
}
#endif
// FUNCTION: LEMBALL 0x00471d60
void* LEMBALL_FASTCALL ClaimTcpipSocketStackCompositeAdjustedThunk(void* pObject)
{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4);
	return ClaimTcpipSocketStackCompositeBody(pObject);
}
// FUNCTION: LEMBALL 0x00471d70
void* LEMBALL_FASTCALL ClaimTcpipSocketStackCompositeBody(void* pObject)
{
	char* pAdjusted;
	int* pOffsets;

	pAdjusted = (char*) pObject;
	pOffsets = *(int**) (pAdjusted - 0x38);
	pAdjusted += pOffsets[4];
	pOffsets = *(int**) (pAdjusted - 0x38);
	return ((VsNetRuntimeChannelCore*) (pAdjusted + pOffsets[2] - 0x38))
		->VsNetRuntimeChannelCore::ClaimRecord();
}
// DISABLED FUNCTION: LEMBALL 0x00471d90
#if 0
void* DeleteTcpipSocketStackDualAdjustedThunk(void* pObject, BYTE fFreeMemory)
{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4) - 0xa8;
	return ((VsNetTcpipSocketChannelStackWrapperView*) pObject)->DeleteTcpipSocketChannelStackWrapper(fFreeMemory);
}
#endif
// FUNCTION: LEMBALL 0x00471da0
#pragma optimize("g", on)
void* LEMBALL_FASTCALL ClaimTcpipSocketStackDualAdjustedThunk(void* pObject)
{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4);
	pObject = (char*) pObject - 0x78;
	return ClaimTcpipSocketStackCompositeBody(pObject);
}
#pragma optimize("", on)
// DISABLED FUNCTION: LEMBALL 0x00471db0
#if 0
void TimedSocketBundleDispatchAsyncThunk(void* pObject, int nMessage, void* pReserved, unsigned int nEvent)

{
	VsNetTimedSocketAsyncMessageAdapter* pAdapter;

	pAdapter = (VsNetTimedSocketAsyncMessageAdapter*) ((char*) pObject - *(int*) ((char*) pObject - 4));
	pAdapter->HandleAsyncMessage(nMessage, pReserved, nEvent);
}
#endif
// LEGACY THUNK: LEMBALL 0x00471dc0
#if 0
void LEMBALL_FASTCALL TimedSocketBundlePrimaryCloseThunk(void* pObject)
{
	((VsNetSocketWindowEffChannel*) ((char*) pObject + 0xa4))->ReleaseFileBackedEffMarker();
}
#endif
// DISABLED FUNCTION: LEMBALL 0x00471dd0
#if 0
void* TimedSocketBundlePrimaryDeleteThunk(void* pObject, BYTE fDelete)
{
	char* pAdjustedObject = (char*) pObject - *(int*) ((char*) pObject - 4);
	return ((VsNetDualSocketWindowChannelStackWrapperView*) pAdjustedObject)
		->DeleteDualSocketWindowChannelStackWrapper(fDelete);
}
// DISABLED FUNCTION: LEMBALL 0x00471de0
void* VsNetDualSocketWindowChannelStackWrapperView::DeleteDualSocketWindowChannelStackWrapper(BYTE fFreeMemory)

{
	VsNetDualSocketWindowChannelStackWrapperView* pView;
	char* pbAllocationBase;
	void* pObject;

	pObject = (void*) this;
	pView = this;
	pbAllocationBase = (char*) this - 8;
	DestroySocketWindowEffChannel((int) (unsigned long) pView->m_abSocketWindowA4);
	((VsNetDualHandleEffStream*) pView->m_abDualStream2c)->DestroyDualHandleEffStream();
	pView->m_ChannelState00.DestroyEffStreamChannelState();
	if ((fFreeMemory & 1) != 0) {
		FreeVSMemBlock(pbAllocationBase);
	}
	return pbAllocationBase;
}
#endif
// LEGACY THUNK: LEMBALL 0x00471e20
#if 0
void LEMBALL_FASTCALL TimedSocketBundlePrimaryCaptureErrorThunk(void* pObject)
{
	((VsNetSocketWindowEffChannel*) ((char*) pObject + 0xa4))->ClearNestedEffChannelAsyncStatus();
}
#endif
// DISABLED FUNCTION: LEMBALL 0x00471e30
#if 0
void TimedSocketBundlePrimaryNoopThunk(void* pObject, BYTE fDelete)
{
	(void) pObject;
	TimedSocketBundlePrimaryNoopBody(fDelete);
}
#endif
// FUNCTION: LEMBALL 0x00471e40
void VsNetDualSocketWindowEffChannelBundle::ClearPendingWrite(void* pArgument)
{
	(void) pArgument;
}
// DISABLED FUNCTION: LEMBALL 0x00471e50
#if 0
void* DeleteDualSocketWindowChannelStackAdjustedThunk(void* pObject, BYTE fDelete)
{
	return ((VsNetDualSocketWindowChannelStackWrapperView*) ((char*) pObject - 0x2c))
		->DeleteDualSocketWindowChannelStackWrapper(fDelete);
}
#endif
// FUNCTION: LEMBALL 0x00471ee0
void VsNetTimedSocketEffChannelBundleLayout::ClearPendingWrite(void* pUnused)
{
	int nPendingDelta = *(int*) (*(int*) ((char*) this - 0x18) + 8);
	((VsNetTimedEffStream*) ((char*) this + nPendingDelta + 0x60))
		->VsNetTimedEffStream::ClearPendingWrite(pUnused);
}
// DISABLED FUNCTION: LEMBALL 0x00471f50
#if 0
void* TimedSocketBundleCompositeDeleteAdjustedThunk(void* pObject, BYTE fDelete)
{
	char* pAdjustedObject = (char*) pObject - *(int*) ((char*) pObject - 4);
	return ((VsNetTcpipSocketChannelStackWrapperView*) pAdjustedObject)
		->DeleteTcpipEffTransportCompositeWrapper(fDelete);
}
// DISABLED FUNCTION: LEMBALL 0x00471f60
void* VsNetTcpipSocketChannelStackWrapperView::DeleteTcpipEffTransportCompositeWrapper(BYTE fFreeMemory)

{
	VsNetTcpipSocketChannelStackWrapperView* pView;
	char* pbAllocationBase;
	void* pObject;

	pObject = (void*) this;
	pView = this;
	pbAllocationBase = (char*) this - 0x30;
	DestroyTcpipEffTransportComposite((int) (unsigned long) pObject);
	DestroySocketWindowEffChannel((int) (unsigned long) pView->m_abSocketWindow128);
	((VsNetDualHandleEffStream*) pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
	((VsNetTimedEffStream*) pView->m_abTimedStream30)->DestroyTimedEffStream();
	pView->m_ChannelState00.DestroyEffStreamChannelState();
	if ((fFreeMemory & 1) != 0) {
		FreeVSMemBlock(pbAllocationBase);
	}
	return pbAllocationBase;
}
#endif
// DISABLED FUNCTION: LEMBALL 0x00471fd0
#if 0
void* DeleteCompositeTcpipEffTransportTimedAdjustedThunk(void* pObject, BYTE fFreeMemory)

{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4) - 0x30;
	return ((VsNetTcpipSocketChannelStackWrapperView*) pObject)->DeleteTcpipEffTransportCompositeWrapper(fFreeMemory);
}
#endif
// FUNCTION: LEMBALL 0x00471fe0
#pragma optimize("g", on)
void LEMBALL_FASTCALL WriteCompositeTcpipEffTransportGlobalSessionAdjustedThunk(void* pObject)
{
	WriteCompositeTcpipGlobalSessionBody((char*) pObject - *(int*) ((char*) pObject - 4) - 8);
}
#pragma optimize("", on)
// FUNCTION: LEMBALL 0x00471ff0
#pragma optimize("g", on)
void* LEMBALL_FASTCALL ClaimCompositeTcpipEffTransportRecordAdjustedThunk(void* pObject)
{
	return ClaimTcpipSocketStackCompositeBody((char*) pObject - *(int*) ((char*) pObject - 4) - 8);
}
#pragma optimize("", on)
// DISABLED FUNCTION: LEMBALL 0x00472000
#if 0
void* DeleteCompositeTcpipEffTransportDualAdjustedThunk(void* pObject, BYTE fFreeMemory)

{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4) - 0xa8;
	return ((VsNetTcpipSocketChannelStackWrapperView*) pObject)->DeleteTcpipEffTransportCompositeWrapper(fFreeMemory);
}
#endif
// FUNCTION: LEMBALL 0x00472010
#pragma optimize("g", on)
void LEMBALL_FASTCALL WriteCompositeTcpipEffTransportDualGlobalSessionAdjustedThunk(void* pObject)
{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4);
	pObject = (char*) pObject - 0x80;
	WriteCompositeTcpipGlobalSessionBody(pObject);
}
// FUNCTION: LEMBALL 0x00472020
void* LEMBALL_FASTCALL ClaimCompositeTcpipEffTransportDualRecordAdjustedThunk(void* pObject)
{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4);
	pObject = (char*) pObject - 0x80;
	return ClaimTcpipSocketStackCompositeBody(pObject);
}
#pragma optimize("", on)
// DISABLED FUNCTION: LEMBALL 0x00472030
#if 0
void DispatchCompositeTcpipPrefixedControlAdjustedThunk(void* pObject, int nDispatchType, void* pEvent)

{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4) - 0x0c;
	((VsNetPrefixedEffTransportControlView*) pObject)->DispatchPrefixedEffTransportControlStream(nDispatchType, pEvent);
}
#endif
// DISABLED FUNCTION: LEMBALL 0x00472040
#if 0
int DispatchTcpipClientAsyncMessageAdjustedThunk(void* pObject, int nMessage, void* pReserved, unsigned int nEvent)

{
	pObject = (char*) pObject - *(int*) ((char*) pObject - 4);
	return ((VsNetTcpipClientAsyncDispatcher*) pObject)->DispatchTcpipClientAsyncMessage(nMessage, pReserved, nEvent);
}
#endif
