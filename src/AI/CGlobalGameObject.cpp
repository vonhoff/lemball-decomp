#include "AI/CGlobalGameObject.h"

struct GameEffStream {
	void* m_pVtable00;
	int m_nEventCode04;
	int m_pvOwnedBuffer08;
	int m_pvBufferEnd0c;
	int m_cWriteSessions10;
	int m_fOwnsBuffer14;
	int m_cbSerializedLength18;
	int m_pvWriteCursor1c;
	int m_pvReadCursor20;
	int m_fHasPayload24;
	int m_fWritePending28;

	int LoadEffStreamFromMemory(int nSourceBuffer);
};

struct ManagedEntityPacketStreamView {
	void BindManagedEntityPacketTarget(void* pEntity);
};

struct ManagedEntityPacketReceiveView : public GameEffStream {
	void* m_pManagedEntity2c;
};

extern ManagedEntityPacketStreamView* g_pManagedEntityPacketBindStream;
extern ManagedEntityPacketStreamView* g_pManagedEntityStateRequestStream;
extern ManagedEntityPacketStreamView* g_pManagedEntityStateResponseStream;
extern ManagedEntityPacketStreamView* g_pManagedEntityPacket2ATargetStream;
extern ManagedEntityPacketStreamView* g_pManagedEntityStateResetPacketStream;

// FUNCTION: LEMBALL 0x00416e90
int CGlobalGameObject::Receive(unsigned short nMessage, CNetworkMessage* pMessage)
{
	ManagedEntityPacketReceiveView* pPacket;
	switch (nMessage) {
	case 0x23:
		pPacket = (ManagedEntityPacketReceiveView*) g_pManagedEntityPacketBindStream;
		break;
	case 0x24:
		pPacket = *(ManagedEntityPacketReceiveView**) 0x49d128;
		break;
	case 0x25:
		pPacket = *(ManagedEntityPacketReceiveView**) 0x49d12c;
		break;
	case 0x27:
		pPacket = (ManagedEntityPacketReceiveView*) g_pManagedEntityStateRequestStream;
		break;
	case 0x28:
		pPacket = (ManagedEntityPacketReceiveView*) g_pManagedEntityStateResponseStream;
		break;
	case 0x29:
		CancelRequest();
		return 1;
	default:
		return 0;
	}
	pPacket->m_pManagedEntity2c = this;
	if (pPacket->LoadEffStreamFromMemory(*(int*) ((char*) pMessage + 0x20)) != 0) {
		*(int*) ((char*) pMessage + 0x20) = pPacket->m_pvReadCursor20;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00416d50
void CGlobalGameObject::Restart(void)
{
	CGameObject::Restart();
	m_fResetPending130 = 0;
	m_nSerializedState12C = 0x18;
	m_nStateResetCode128 = 0x18;
	m_nUsableState134 = 0;
	m_fNetworkStateUpdateActive124 = 1;
}

// FUNCTION: LEMBALL 0x00416d90
int CGlobalGameObject::UsableState(void)
{
	int nState = m_nUsableState134;
	if (nState != 0) {
		m_nUsableState134 = 0;
	}
	return nState;
}

// FUNCTION: LEMBALL 0x00416e70
void CGlobalGameObject::CancelRequest(void)
{
	if (m_fResetPending130 != 0) {
		m_nPendingState114 = 0;
		m_fResetPending130 = 0;
		m_nRuntimeState8C = 0;
	}
}

// FUNCTION: LEMBALL 0x00416fc0
void CGlobalGameObject::SendRemove(void)
{
	g_pManagedEntityPacket2ATargetStream->BindManagedEntityPacketTarget(this);
}

// FUNCTION: LEMBALL 0x00416fd0
void CGlobalGameObject::SendCancel(void)
{
	g_pManagedEntityStateResetPacketStream->BindManagedEntityPacketTarget(this);
}

// FUNCTION: LEMBALL 0x0041d350
void CGlobalGameObject::AppendGmobChunkType14Action(int nType, unsigned short nParam)
{
	if (m_cActionCount15C < 0x20) {
		m_aActions160[m_cActionCount15C].m_nType = nType;
		m_aActions160[m_cActionCount15C].m_nParam = nParam;
		m_cActionCount15C++;
	}
}
