#include "Key.h"
#include "../../Map/Base/Map.h"
#include "../Navigation/Ai.h"

// 68K 0x1011ad1a Usage__4CKeyFv
// FUNCTION: LEMBALL 0x0041c5f0
int Key::Usage()
{
	return 2;
}

// 68K 0x1011ad36 __dt__4CKeyFv
Key::~Key()
{
}

// 68K 0x10619c40 Process__4CKeyFv
// FUNCTION: LEMBALL 0x0041d480
bool Key::Process()
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
				SetSndEffect((eSoundEffect) 41);
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

// 68K 0x10619d06 Activate__4CKeyFP11CGameObject
// FUNCTION: LEMBALL 0x0041d560
bool Key::Activate(GameObject* p_object)
{
	m_activator = p_object;
	if (m_activator->HasObject(m_objectType) == 0) {
		RequestAction((eAction) 26);
		return 1;
	}
	return 0;
}

// 68K 0x10619d6e DoActivate__4CKeyFv
// FUNCTION: LEMBALL 0x0041d5a0
void Key::DoActivate()
{
	m_activator->AddObject(m_objectType, this);
	SetSndEffect((eSoundEffect) 41);
	g_pAI->AddTime(10);
}

// 68K 0x10619dd0 ActivatePosition__4CKeyFv
// FUNCTION: LEMBALL 0x0041d5d0
AiCoord Key::ActivatePosition()
{
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed - 0x8000;
	return AiCoord(x, y, z);
}

