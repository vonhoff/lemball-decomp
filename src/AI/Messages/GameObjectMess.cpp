#include "GameObjectMess.h"

#include "../Base/GlobalGameObject.h"
#include "../Managers/BaseObjectManager.h"

GameObjectMess::GameObjectMess()
{
}

// 68K 0x1060b23a __ct__15CGameObjectMessFUl
// FUNCTION: LEMBALL 0x00416850
GameObjectMess::GameObjectMess(unsigned long p_messageId) : NetworkMessage(p_messageId)
{
	m_headerEnabled = 1;
	m_payloadCapacity += 4;
}

// 68K 0x1060b292 Send__15CGameObjectMessFP17CGlobalGameObject
// FUNCTION: LEMBALL 0x00416880
void GameObjectMess::Send(GlobalGameObject* p_object)
{
	m_object = p_object;
	p_object->m_manager->Add(this);
}

// 68K 0x1060b2ea AddData__15CGameObjectMessFv
// FUNCTION: LEMBALL 0x004168a0
void GameObjectMess::AddData()
{
	Add((unsigned short) m_messageId);
	Add((unsigned short) m_object->GetId());
}

// 68K 0x1060b33e GetData__15CGameObjectMessFv
// FUNCTION: LEMBALL 0x004168c0
void GameObjectMess::GetData()
{
}

// 68K 0x10117fd4 __dt__15CGameObjectMessFv
GameObjectMess::~GameObjectMess()
{
}
