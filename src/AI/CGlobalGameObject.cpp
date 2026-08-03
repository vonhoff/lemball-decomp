#include "AI/CGlobalGameObject.h"

struct ManagedEntityPacketStreamView {
	void BindManagedEntityPacketTarget(void* pEntity);
};

extern ManagedEntityPacketStreamView* g_pManagedEntityPacket2ATargetStream;
extern ManagedEntityPacketStreamView* g_pManagedEntityStateResetPacketStream;

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
