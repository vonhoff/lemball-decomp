#include "Ammo.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"
#include "../Navigation/Ai.h"

// 68K 0x1011a9fe Usage__5CAmmoFv
// FUNCTION: LEMBALL 0x0041c430
int Ammo::Usage()
{
	return 2;
}

// 68K 0x1011aa1a __dt__5CAmmoFv
Ammo::~Ammo()
{
}

// 68K 0x10618ede Restart__5CAmmoFv
// FUNCTION: LEMBALL 0x0041ca90
void Ammo::Restart()
{
	BaseGlobalObject::Restart();
	m_ammo = 0;
}

// 68K 0x10618f0a Process__5CAmmoFv
// FUNCTION: LEMBALL 0x0041cab0
bool Ammo::Process()
{
	int y = m_position.m_yFixed >> 12;
	int x = m_position.m_xFixed >> 12;
	int blockX = x >> 4;
	int blockY = y >> 4;
	if (x >= 0 && y >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		m_position.m_zFixed =
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].GetZ(x & 0xf, y & 0xf) << 12;
	}
	else {
		m_position.m_zFixed = 0;
	}
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			if (m_action == 26) {
				SetSndEffect((eSoundEffect) 17);
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	switch (m_action) {
	case 26:
		if (m_unk0xd4 >= g_dwGameTick) {
			return 1;
		}
		if (m_ammo == 0) {
			m_heading = 0;
			return 1;
		}
		m_actionDeadline = g_dwGameTick + (m_ammo * 1000) / 50;
		RequestAction((eAction) 27);
		break;
	case 27:
		if (m_actionDeadline >= g_dwGameTick) {
			return 1;
		}
		RequestAction((eAction) 24);
		break;
	}
	return 1;
}

// 68K 0x10619030 Activate__5CAmmoFP11CGameObject
// FUNCTION: LEMBALL 0x0041cbe0
bool Ammo::Activate(GameObject* p_object)
{
	if (m_action == 24 && p_object->HasObject(m_objectType) == 0) {
		m_unk0xd4 = 8;
		m_activator = p_object;
		RequestAction((eAction) 26);
		return 1;
	}
	return 0;
}

// 68K 0x106190a4 DoActivate__5CAmmoFv
// FUNCTION: LEMBALL 0x0041cc30
void Ammo::DoActivate()
{
	m_stateTimer = g_dwSimulationTimestamp;
	m_unk0xd4 += g_dwGameTick;
	SetSndEffect((eSoundEffect) 0x11);
	m_activator->PickUpAmmo(25);
	g_pAI->AddTime(50);
}

// 68K 0x10619118 ActivatePosition__5CAmmoFv
// FUNCTION: LEMBALL 0x0041cc70
AiCoord Ammo::ActivatePosition()
{
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed;
	return AiCoord(x, y, z);
}
