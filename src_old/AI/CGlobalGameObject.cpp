#define LEMBALL_CGLOBALGAMEOBJECT_SETMESSAGES
#include "AI/CGlobalGameObject.h"
#undef LEMBALL_CGLOBALGAMEOBJECT_SETMESSAGES

#include "Platform/Windows/Mixed/Level/LVPKT.H"
#include "Visos/Generic/Memory.h"

#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif

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

// FUNCTION: LEMBALL 0x00416fe0
void CGlobalGameObject::SetMessages(void)
{
	void* pPacket = AllocateVSMemBlock(0x34);
	if (pPacket != 0) {
		new (pPacket) ManagedEntityPacketBase(0);
		*(void**) pPacket = (void*) 0x493fe8;
	}
	*(void**) 0x49d110 = pPacket;

	pPacket = AllocateVSMemBlock(sizeof(ManagedEntityPacket23));
	if (pPacket != 0) {
		pPacket = new (pPacket) ManagedEntityPacket23;
	}
	*(void**) 0x49d114 = pPacket;

	pPacket = AllocateVSMemBlock(sizeof(ManagedEntityPacket2A));
	if (pPacket != 0) {
		pPacket = new (pPacket) ManagedEntityPacket2A;
	}
	*(void**) 0x49d118 = pPacket;

	pPacket = AllocateVSMemBlock(sizeof(ManagedEntityPacket27));
	if (pPacket != 0) {
		pPacket = new (pPacket) ManagedEntityPacket27;
	}
	*(void**) 0x49d11c = pPacket;

	pPacket = AllocateVSMemBlock(sizeof(ManagedEntityPacket28));
	if (pPacket != 0) {
		pPacket = new (pPacket) ManagedEntityPacket28;
	}
	*(void**) 0x49d120 = pPacket;

	pPacket = AllocateVSMemBlock(sizeof(ManagedEntityStateResetPacket));
	if (pPacket != 0) {
		pPacket = new (pPacket) ManagedEntityStateResetPacket;
	}
	*(void**) 0x49d124 = pPacket;

	pPacket = AllocateVSMemBlock(sizeof(ManagedEntityPacket24));
	if (pPacket != 0) {
		pPacket = new (pPacket) ManagedEntityPacket24;
	}
	*(void**) 0x49d128 = pPacket;

	pPacket = AllocateVSMemBlock(sizeof(ManagedEntityPacket25));
	if (pPacket != 0) {
		pPacket = new (pPacket) ManagedEntityPacket25;
	}
	*(void**) 0x49d12c = pPacket;

	pPacket = AllocateVSMemBlock(sizeof(ManagedEntityPacket26));
	if (pPacket != 0) {
		pPacket = new (pPacket) ManagedEntityPacket26;
	}
	*(void**) 0x49d130 = pPacket;
}

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
		*(int*) ((char*) pMessage + 0x20) = pPacket->m_pvReadCursor;
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
