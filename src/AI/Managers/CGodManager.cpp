#include "CGodManager.h"

#include "../../Visos/Messaging/BasePacketHeader.h"
#include "../../Visos/Messaging/CReadPacket.h"
#include "../../Visos/Network/CBaseNetwork.h"
#include "../../Visos/Network/CConnect.h"
#include "../Base/CGlobalGameObject.h"
#include "../Messages/CGameStateMessage.h"
#include "../Navigation/CAI.h"
#include "../Objects/CViewData.h"
#include "CBaseObjectManager.h"

// FUNCTION: LEMBALL 0x0040b020
CGodManager::CGodManager(int p_capacity)
{
	m_capacity = p_capacity;
	m_count = 0;
	m_managers = new CBaseObjectManager*[p_capacity];
	m_transportMap = new int[24];
	for (int i = 0; i < 24; i++) {
		m_transportMap[i] = -1;
	}
	CGlobalGameObject::SetMessages();
	if (g_pBaseNetwork != 0) {
		g_pBaseNetwork->AttachMessageQueue(this);
	}
	if (g_pActiveAI->m_networkMode != 0) {
		m_gameStateMessage = new CGameStateMessage();
	}
	else {
		m_gameStateMessage = 0;
	}
}

// FUNCTION: LEMBALL 0x0040b0d0
CGodManager::~CGodManager()
{
	if (g_pBaseNetwork != 0) {
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
		if (self->m_managers[i] != 0) {
			self->m_managers[i]->Restart();
		}
	}
}

// FUNCTION: LEMBALL 0x0040b150
void CGodManager::Register(CBaseObjectManager* p_manager)
{
	m_transportMap[p_manager->m_messageType] = m_count;
	m_managers[m_count] = p_manager;
	m_count++;
	p_manager->Restart();
}

// FUNCTION: LEMBALL 0x0040b180
void CGodManager::Unregister(CBaseObjectManager* p_manager)
{
	CBaseObjectManager** managers;
	CBaseObjectManager** item;
	int index;
	int count;

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
		m_transportMap[p_manager->m_messageType] = -1;
		m_managers[index] = 0;
		m_count--;
	}
}

// FUNCTION: LEMBALL 0x0040b1f0
CBaseObjectManager* CGodManager::GetManagerForTransport(int p_transportId)
{
	int index = m_transportMap[p_transportId];
	if (index != -1) {
		return m_managers[index];
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0040b210
int CGodManager::ProcessMsg(Message* p_message)
{
	int code = p_message->code;
	switch (p_message->type) {
	case 5: {
		CReadPacket* packet = (CReadPacket*) p_message->source;
		if (code == 0) {
			if (TransportReceive(packet)) {
				return 1;
			}

			BasePacketHeader* header = (BasePacketHeader*) packet->m_data;
			switch (header->m_messageId) {
			case 10:
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
		return 0;
	}
	CBaseObjectManager* manager = m_managers[m_transportMap[header->m_messageId - 0xb]];
	manager->CNetworkMessage::Set(data + sizeof(BasePacketHeader));
	p_packet->m_used = 0;
	return 1;
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
	if (g_pActiveConnection != 0) {
		for (int i = 0; i < m_count; i++) {
			m_managers[i]->ProcessNetwork();
		}
	}
}

// GLOBAL: LEMBALL 0x0049cf30
CGodManager* g_pGodManager = 0;
