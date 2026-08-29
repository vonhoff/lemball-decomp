#include "GodManager.h"
#include "BaseObjectManager.h"

// 68K 0x1060de8a __ct__11CGodManagerFi
// STUB: LEMBALL 0x0040b020
GodManager::GodManager(int p_arg0)
{
}

// 68K 0x1060e004 Restart__11CGodManagerFv
// FUNCTION: LEMBALL 0x0040b120
void GodManager::Restart()
{
	for (int i = 0; i < m_count; i++) {
		if (m_managers[i] != 0) {
			m_managers[i]->Restart();
		}
	}
}

// 68K 0x1060e060 Register__11CGodManagerFP18CBaseObjectManager
// STUB: LEMBALL 0x0040b150
void GodManager::Register(BaseObjectManager* p_manager)
{
}

// 68K 0x1060e0dc ProcessMsg__11CGodManagerFP10tagMESSAGE
// STUB: LEMBALL 0x0040b210
int GodManager::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x1060e190 TransportReceive__11CGodManagerFP11CReadPacket
// STUB: LEMBALL 0x0040b290
bool GodManager::TransportReceive(ReadPacket* p_packet)
{
	return 0;
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

