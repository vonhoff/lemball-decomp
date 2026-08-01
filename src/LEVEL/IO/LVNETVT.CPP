#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/MEDIA/EFFSTRM.H"

extern "C" unsigned long __stdcall timeGetTime(void);

extern int g_nSelectedNetworkLobbyPeerId;
extern void* g_pActiveNetworkRuntimeWindow;
extern void* g_pNetworkLobbyVsnetRuntime;
extern void LEMBALL_FASTCALL MarkNetworkLobbySelectedPeerDisconnected(void* pVsnetRuntime);

struct VsNetSimpleEffTransportRecordSlotTable {
	int GetNextEffTransportRecordClaimFlag(unsigned short nSequence);
};

struct VsNetEffStreamBase {
	void* m_pVtable00;
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

	void WriteEffStreamU16BE(unsigned short nValue);
	int SaveEffStreamToMemoryRange(int nTargetBuffer, int cbRange);
};

struct LevelChunkStreamWriteView : public GameEffStream {
	void AppendLevelChunkStreamPayload(VsNetEffStreamBase* pPayload);
	void FlushLevelChunkStreamWrite(void);
};

struct ManagedEntityPacketStreamView : public VsNetEffStreamBase {
	void* m_pManagedEntity2c;

	void BindManagedEntityPacketTarget(void* pEntity);
};

struct ManagedEntityStateView {
	void** m_pVtable00;
	char m_abReserved04[0xb4];
	int m_nStateIdb8;
	char m_abReservedbc[0x70];
	int m_nRequestedStateId12c;
	char m_abReserved130[4];
	int m_nStateTransitionStatus134;

	void RequestManagedEntityStateId(int nStateId);
};

// Split from the original LEVELVT/TYPE18 source group to preserve MSVC 4.20 code generation.

// GLOBAL: LEMBALL 0x0049d114
ManagedEntityPacketStreamView* g_pManagedEntityPacketBindStream = 0;

// GLOBAL: LEMBALL 0x0049d11c
ManagedEntityPacketStreamView* g_pManagedEntityStateRequestStream = 0;

// FUNCTION: LEMBALL 0x0040ad30
void LevelChunkStreamWriteView::AppendLevelChunkStreamPayload(VsNetEffStreamBase* pPayload)
{
	typedef void(LEMBALL_FASTCALL * ServiceRuntimeProc)(void*);
	GameEffStream* pSource = (GameEffStream*) pPayload;
	LevelChunkStreamWriteView* pTarget = this;
	unsigned long dwStart;
	int cbRequired;

	do {
		if (pTarget->m_fWritePending != 0) {
			dwStart = timeGetTime();
			if (pTarget->m_fWritePending != 0) {
				do {
					if (timeGetTime() - dwStart >= 2000) {
						break;
					}
					((ServiceRuntimeProc) (*(void***) g_pActiveNetworkRuntimeWindow)[12])(
						g_pActiveNetworkRuntimeWindow);
				} while (pTarget->m_fWritePending != 0);
				if (pTarget->m_fWritePending != 0) {
					return;
				}
			}
		}
		if (pTarget->m_cWriteSessions <= 0) {
			pTarget->BeginEffStreamWriteSession();
		}
		cbRequired = (unsigned char*) pTarget->m_pvWriteCursor - (unsigned char*) pTarget->m_pvOwnedBuffer;
		cbRequired += pSource->m_cbSerializedLength;
		cbRequired += 2;
		if ((unsigned int) cbRequired <= (unsigned int) pTarget->m_cbSerializedLength) {
			((VsNetEffStreamBase*) pSource)->SaveEffStreamToMemoryRange(pTarget->m_pvWriteCursor, 0);
			pTarget->m_pvWriteCursor += pSource->m_pvWriteCursor - pSource->m_pvOwnedBuffer;
			return;
		}
		pTarget->FlushLevelChunkStreamWrite();
	} while (1);
}

// FUNCTION: LEMBALL 0x0040add0
void LevelChunkStreamWriteView::FlushLevelChunkStreamWrite(void)
{
	typedef void(LEMBALL_FASTCALL * ServiceRuntimeProc)(void*);
	LevelChunkStreamWriteView* pTarget = this;
	char* pRecordOwner;
	unsigned long dwStart;

	if (pTarget->m_cWriteSessions <= 0 || pTarget->m_fWritePending != 0 || g_nSelectedNetworkLobbyPeerId == 0) {
		return;
	}
	((VsNetEffStreamBase*) pTarget)->WriteEffStreamU16BE(0x2f);
	pRecordOwner = (char*) (unsigned long) g_nSelectedNetworkLobbyPeerId +
				   *(int*) (*(int*) ((char*) (unsigned long) g_nSelectedNetworkLobbyPeerId + 4) + 8) + 4;
	if (*(int*) (pRecordOwner + 0x5c) != -1 ||
		((VsNetSimpleEffTransportRecordSlotTable*) *(void**) (pRecordOwner + 0x4c))
				->GetNextEffTransportRecordClaimFlag(*(unsigned short*) (pRecordOwner + 0x30)) == 0) {
		dwStart = timeGetTime();
		do {
			pRecordOwner = (char*) (unsigned long) g_nSelectedNetworkLobbyPeerId +
						   *(int*) (*(int*) ((char*) (unsigned long) g_nSelectedNetworkLobbyPeerId + 4) + 8) + 4;
			if (*(int*) (pRecordOwner + 0x5c) == -1 &&
				((VsNetSimpleEffTransportRecordSlotTable*) *(void**) (pRecordOwner + 0x4c))
						->GetNextEffTransportRecordClaimFlag(*(unsigned short*) (pRecordOwner + 0x30)) != 0) {
				break;
			}
			if (timeGetTime() - dwStart >= 4000) {
				break;
			}
			((ServiceRuntimeProc) (*(void***) g_pActiveNetworkRuntimeWindow)[12])(g_pActiveNetworkRuntimeWindow);
		} while (1);
	}

	pRecordOwner = (char*) (unsigned long) g_nSelectedNetworkLobbyPeerId +
				   *(int*) (*(int*) ((char*) (unsigned long) g_nSelectedNetworkLobbyPeerId + 4) + 8) + 4;
	if (*(int*) (pRecordOwner + 0x5c) == -1 &&
		((VsNetSimpleEffTransportRecordSlotTable*) *(void**) (pRecordOwner + 0x4c))
				->GetNextEffTransportRecordClaimFlag(*(unsigned short*) (pRecordOwner + 0x30)) != 0) {
		pTarget->QueueEffStreamWriteEvent(g_nSelectedNetworkLobbyPeerId);
		pTarget->EndEffStreamWriteSession();
		return;
	}
	MarkNetworkLobbySelectedPeerDisconnected(g_pNetworkLobbyVsnetRuntime);
	pTarget->EndEffStreamWriteSession();
}

// FUNCTION: LEMBALL 0x00416880
void ManagedEntityPacketStreamView::BindManagedEntityPacketTarget(void* pEntity)
{
	m_pManagedEntity2c = pEntity;
	((LevelChunkStreamWriteView*) *(void**) ((char*) pEntity + 0x60))->AppendLevelChunkStreamPayload(this);
}

// FUNCTION: LEMBALL 0x00416e20
void ManagedEntityStateView::RequestManagedEntityStateId(int nStateId)
{
	typedef void(LEMBALL_FASTCALL * NotifyStateProc)(void*);
	if (g_nSelectedNetworkLobbyPeerId != 0) {
		m_nRequestedStateId12c = nStateId;
		g_pManagedEntityStateRequestStream->BindManagedEntityPacketTarget(this);
	}
	else {
		m_nStateIdb8 = nStateId;
		((NotifyStateProc) m_pVtable00[67])(this);
		m_nStateTransitionStatus134 = 2;
	}
}
