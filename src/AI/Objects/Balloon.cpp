#include "Balloon.h"
#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"
#include "../Navigation/Ai.h"

// 68K 0x1011ad98 Usage__8CBalloonFv
// FUNCTION: LEMBALL 0x0041c630
int Balloon::Usage()
{
	return 2;
}

// 68K 0x1011adb8 __dt__8CBalloonFv
Balloon::~Balloon()
{
}

// 68K 0x10619e44 Restart__8CBalloonFv
// FUNCTION: LEMBALL 0x0041d600
void Balloon::Restart()
{
	BaseGlobalObject::Restart();
	int randVal = (*g_pSentinel * 0x29 + 0x1f) & 0x7fffff;
	*g_pSentinel = randVal;
	m_stateTimer = g_dwSimulationTimestamp - (randVal % 4096);
}

// 68K 0x10619ea4 Process__8CBalloonFv
// FUNCTION: LEMBALL 0x0041d650
bool Balloon::Process()
{
	int y = m_position.m_yFixed >> 12;
	int x = m_position.m_xFixed >> 12;
	int blockX = x >> 4;
	int blockY = y >> 4;
	if (x >= 0 && y >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		m_position.m_zFixed = g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].GetZ(x & 0xf, y & 0xf) << 12;
	}
	else {
		m_position.m_zFixed = 0;
	}
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			if (m_action == 26) {
				SetSndEffect((eSoundEffect) 42);
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	if (m_action == 26) {
		Action((eAction) 24);
		m_heading = 0;
	}
	return 1;
}

// 68K 0x10619f72 Activate__8CBalloonFP11CGameObject
// FUNCTION: LEMBALL 0x0041d740
bool Balloon::Activate(GameObject* p_object)
{
	m_activator = p_object;
	if (m_activator->HasObject(m_objectType) == 0) {
		RequestAction((eAction) 26);
		return 1;
	}
	return 0;
}

// 68K 0x10619fde DoActivate__8CBalloonFv
// FUNCTION: LEMBALL 0x0041d780
void Balloon::DoActivate()
{
	m_activator->AddObject(m_objectType, this);
	SetSndEffect((eSoundEffect) 42);
	g_pAI->AddTime(50);
}

// 68K 0x1061a044 ActivatePosition__8CBalloonFv
// FUNCTION: LEMBALL 0x0041d7b0
AiCoord Balloon::ActivatePosition()
{
	int z = m_position.m_zFixed;
	int y = m_position.m_yFixed;
	return AiCoord(m_position.m_xFixed, y, z);
}

