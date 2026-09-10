#include "../../Visos/Network/BaseNetwork.h"
#include "../Base/GlobalGameObject.h"
#include "../Messages/GameStateMessage.h"
#include "../Navigation/Ai.h"
#include "BaseObjectManager.h"
#include "GodManager.h"

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
