#include "GodManager.h"

#include "../../Visos/Messaging/BasePacketHeader.h"
#include "../../Visos/Messaging/ReadPacket.h"
#include "../../Visos/Network/BaseNetwork.h"
#include "../Base/GlobalGameObject.h"
#include "../Messages/GameStateMessage.h"
#include "../Navigation/Ai.h"
#include "BaseObjectManager.h"

// 68K 0x1060de8a __ct__11CGodManagerFi
// FUNCTION: LEMBALL 0x0040b020
GodManager::GodManager(int p_arg0)
{
	m_capacity = p_arg0;
	m_count = 0;
	m_managers = new BaseObjectManager*[p_arg0];
	m_transportMap = new int[24];
	for (int i = 0; i < 24; i++) {
		m_transportMap[i] = -1;
	}
	GlobalGameObject::SetMessages();
	if (g_pBaseNetwork != 0) {
		g_pBaseNetwork->AttachMessageQueue(this);
	}
	if (g_pActiveAI->m_networkMode != 0) {
		m_gameStateMessage = new GameStateMessage();
	}
	else {
		m_gameStateMessage = 0;
	}
}

// 68K 0x1060e004 Restart__11CGodManagerFv
// FUNCTION: LEMBALL 0x0040b120
void GodManager::Restart()
{
	GodManager* self = this;
	for (int i = 0; i < self->m_count; i++) {
		if (self->m_managers[i] != 0) {
			self->m_managers[i]->Restart();
		}
	}
}

// 68K 0x1060e060 Register__11CGodManagerFP18CBaseObjectManager
// FUNCTION: LEMBALL 0x0040b150
void GodManager::Register(BaseObjectManager* p_manager)
{
	m_transportMap[p_manager->m_messageType] = m_count;
	m_managers[m_count] = p_manager;
	m_count++;
	p_manager->Restart();
}

// 68K 0x1060e0dc ProcessMsg__11CGodManagerFP10tagMESSAGE
// STUB: LEMBALL 0x0040b210
int GodManager::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x1060e190 TransportReceive__11CGodManagerFP11CReadPacket
// FUNCTION: LEMBALL 0x0040b290
bool GodManager::TransportReceive(ReadPacket* p_packet)
{
	unsigned char* data = p_packet->m_data;
	BasePacketHeader* header = (BasePacketHeader*) data;
	if (header->m_messageId < 0xb) {
		return 0;
	}
	BaseObjectManager* manager = m_managers[m_transportMap[header->m_messageId - 0xb]];
	manager->NetworkMessage::Set(data + sizeof(BasePacketHeader));
	p_packet->m_used = 0;
	return 1;
}

// 68K 0x1060e222 GetViewData__11CGodManagerFP9CViewData
// STUB: LEMBALL 0x0040b2e0
int GodManager::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x1060e298 Process__11CGodManagerFv
// STUB: LEMBALL 0x0040b320
void GodManager::Process()
{
}

// 68K 0x1060df5c __dt__11CGodManagerFv
GodManager::~GodManager()
{
}

// GLOBAL: LEMBALL 0x0049cf30
GodManager* g_pGodManager = 0;
