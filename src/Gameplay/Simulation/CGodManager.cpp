#include "CGodManager.h"

#include "Engine/Network/Packets/BasePacketHeader.h"
#include "Engine/Network/Packets/CReadPacket.h"
#include "Engine/Network/CBaseNetwork.h"
#include "Engine/Network/CConnect.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Messages/CGameStateMessage.h"
#include "Gameplay/Messages/GameMessageIds.h"
#include "CAI.h"
#include "Gameplay/Objects/CViewData.h"
#include "Gameplay/Objects/CBaseObjectManager.h"

enum {
	OBJECT_TRANSPORT_MAP_CAPACITY = OBJECT_MANAGER_TRANSPORT_PLAYER_LEMMING_GROUPS + 1,
	TRANSPORT_MANAGER_INDEX_UNREGISTERED = -1
};

// FUNCTION: LEMBALL 0x0040b020
CGodManager::CGodManager(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	m_managers = new CBaseObjectManager*[p_capacity];
	m_transportMap = new int[OBJECT_TRANSPORT_MAP_CAPACITY];
	for (int i = 0; i < OBJECT_TRANSPORT_MAP_CAPACITY; i++) {
		m_transportMap[i] = TRANSPORT_MANAGER_INDEX_UNREGISTERED;
	}
	CGlobalGameObject::SetMessages();
	if (g_pBaseNetwork != NULL) {
		g_pBaseNetwork->AttachMessageQueue(this);
	}
	if (g_pActiveAI->m_networkMode != NETWORK_MODE_SINGLE_PLAYER) {
		m_gameStateMessage = new CGameStateMessage();
	}
	else {
		m_gameStateMessage = NULL;
	}
}

// FUNCTION: LEMBALL 0x0040b0d0
CGodManager::~CGodManager()
{
	if (g_pBaseNetwork != NULL) {
		g_pBaseNetwork->DetachMessageQueue();
	}
	delete m_gameStateMessage;
	CGlobalGameObject::DeleteMessages();
	operator delete(m_transportMap);
	operator delete(m_managers);
}

// FUNCTION: LEMBALL 0x0040b120
void CGodManager::Restart()
{
	CGodManager* self = this;
	for (int i = 0; i < self->m_count; i++) {
		if (self->m_managers[i] != NULL) {
			self->m_managers[i]->Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x0040b150
void CGodManager::Register(CBaseObjectManager* p_manager)
{
	m_transportMap[p_manager->m_transportId] = m_count;
	m_managers[m_count] = p_manager;
	m_count++;
	p_manager->Restart();
}

// FUNCTION: LEMBALL 0x0040b180
void CGodManager::Unregister(CBaseObjectManager* p_manager)
{
	CBaseObjectManager** managers;
	CBaseObjectManager** item;
	int count;
	int index;

	index = 0;
	count = m_count;
	if (count > index) {
		managers = m_managers;
		do {
			if (*managers == p_manager) {
				goto managerFound;
			}
			managers++;
			index++;
		} while (count > index);
		return;
	managerFound:
		if (index < count - 1) {
			do {
				item = m_managers + index;
				index++;
				*item = item[1];
			} while (index < m_count - 1);
		}
		m_transportMap[p_manager->m_transportId] = TRANSPORT_MANAGER_INDEX_UNREGISTERED;
		m_managers[index] = NULL;
		m_count--;
	}
}

// FUNCTION: LEMBALL 0x0040b1f0
CBaseObjectManager* CGodManager::GetManagerForTransport(int p_transportId)
{
	int index = m_transportMap[p_transportId];
	if (index != TRANSPORT_MANAGER_INDEX_UNREGISTERED) {
		return m_managers[index];
	}
	return NULL;
}

// FUNCTION: LEMBALL 0x0040b210
int CGodManager::ProcessMsg(Message* p_message)
{
	int code = p_message->m_code;
	switch (p_message->m_type) {
	case NETWORK_EVENT_CRITICAL_PACKET_READY: {
		CReadPacket* packet = (CReadPacket*) p_message->m_source;
		if (code == 0) {
			if (TransportReceive(packet)) {
				return 1;
			}

			BasePacketHeader* header = (BasePacketHeader*) packet->m_data;
			switch (header->m_messageId) {
			case MESSAGE_GAME_STATE:
				m_gameStateMessage->Set(packet->m_data + sizeof(BasePacketHeader));
				packet->m_used = 0;
				g_pActiveAI->RemoteGameState(m_gameStateMessage);
				return 1;
			}
		}
		else {
			return 1;
		}
		break;
	}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0040b290
bool CGodManager::TransportReceive(CReadPacket* p_packet)
{
	unsigned char* data = p_packet->m_data;
	BasePacketHeader* header = (BasePacketHeader*) data;
	if (header->m_messageId < 0xb) {
		return false;
	}
	CBaseObjectManager* manager = m_managers[m_transportMap[header->m_messageId - 0xb]];
	manager->CNetworkMessage::Set(data + sizeof(BasePacketHeader));
	p_packet->m_used = 0;
	return true;
}

// FUNCTION: LEMBALL 0x0040b2e0
int CGodManager::GetViewData(CViewData* p_viewData)
{
	int i = 0;
	int total = 0;
	if (m_count > 0) {
		do {
			total += m_managers[i]->GetViewData(p_viewData + total);
			i++;
		} while (i < m_count);
	}
	return total;
}

// FUNCTION: LEMBALL 0x0040b320
void CGodManager::Process()
{
	for (int i = 0; i < m_count; i++) {
		m_managers[i]->Process();
	}
	if (g_pActiveConnection != NULL) {
		for (int i = 0; i < m_count; i++) {
			m_managers[i]->ProcessNetwork();
		}
	}
}

// GLOBAL: LEMBALL 0x0049cf30
CGodManager* g_pGodManager = NULL;
