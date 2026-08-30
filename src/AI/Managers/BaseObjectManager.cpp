#include "BaseObjectManager.h"

// 68K 0x1011977c Restart__18CBaseObjectManagerFv
// FUNCTION: LEMBALL 0x0040ab70
void BaseObjectManager::Restart()
{
}

// 68K 0x10119728 Receive__18CBaseObjectManagerFUsP17CGlobalGameObjectP15CNetworkMessage
// FUNCTION: LEMBALL 0x0040ab80
bool BaseObjectManager::Receive(unsigned short p_messageId, GlobalGameObject* p_object, NetworkMessage* p_message)
{
	return 0;
}

// 68K 0x101197a6 GetViewData__18CBaseObjectManagerFP9CViewData
// FUNCTION: LEMBALL 0x0040ab90
int BaseObjectManager::GetViewData(ViewData* p_viewData)
{
	return 0;
}

// 68K 0x1011806c __dt__18CBaseObjectManagerFv
BaseObjectManager::~BaseObjectManager()
{
}

// 68K 0x1060420e Add__18CBaseObjectManagerFP15CNetworkMessage
// STUB: LEMBALL 0x0040ad30
void BaseObjectManager::Add(NetworkMessage* p_message)
{
}

// 68K 0x106042fe AddData__18CBaseObjectManagerFv
// FUNCTION: LEMBALL 0x0040adc0
void BaseObjectManager::AddData()
{
}

// 68K 0x10604328 ProcessNetwork__18CBaseObjectManagerFv
// STUB: LEMBALL 0x0040add0
void BaseObjectManager::ProcessNetwork()
{
}

// 68K 0x10604496 GetData__18CBaseObjectManagerFv
// STUB: LEMBALL 0x0040aed0
void BaseObjectManager::GetData()
{
}
