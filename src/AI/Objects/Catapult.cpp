#include "Catapult.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../Navigation/Ai.h"

// 68K 0x1011a976 Usage__9CCatapultFv
// FUNCTION: LEMBALL 0x0041c3f0
int Catapult::Usage()
{
	return 1;
}

// 68K 0x1011a996 __dt__9CCatapultFv
Catapult::~Catapult()
{
}

// 68K 0x10618a46 Restart__9CCatapultFv
// FUNCTION: LEMBALL 0x0041c700
void Catapult::Restart()
{
	BaseGlobalObject::Restart();
	m_actionArgument = 0;
}

#include "../../Map/Base/Map.h"

// 68K 0x10618a76 Process__9CCatapultFv
// FUNCTION: LEMBALL 0x0041c720
bool Catapult::Process()
{
	int y = m_position.m_yFixed >> 12;
	int x = m_position.m_xFixed >> 12;
	int blockX = x >> 4;
	int blockY = y >> 4;
	unsigned short z;
	if (x >= 0 && y >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		z = g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].GetZ(x & 0xf, y & 0xf);
	}
	else {
		z = 0;
	}
	m_position.m_zFixed = (int) z * 0x1000;
	if (m_isRemoteObject != 0) {
		m_actionArgument = 1;
		if (m_pendingAction != m_action) {
			if (m_action == 27) {
				SetSndEffect((eSoundEffect) 9);
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	m_actionArgument = 0;
	switch (m_action) {
	case 25:
		if (g_dwGameTick <= m_unk0xd0) {
			return 1;
		}
		Action((eAction) 26);
		break;
	case 26: {
		if (g_dwGameTick <= m_unk0xd4) {
			return 1;
		}
		C3DVector pos;
		pos.m_xFixed = m_position.m_xFixed - 0xc000;
		pos.m_yFixed = m_position.m_yFixed - 0xc000;
		pos.m_zFixed = (int) z * 0x1000 + 0x20000;

		int r1 = (*g_pSentinel * 0x29 + 0x1f) & 0x7fffff;
		*g_pSentinel = r1;
		int r2 = (*g_pSentinel * 0x29 + 0x1f) & 0x7fffff;
		*g_pSentinel = r2;
		int r3 = (*g_pSentinel * 0x29 + 0x1f) & 0x7fffff;
		*g_pSentinel = r3;

		C3DVector vel;
		vel.m_xFixed = ((r1 % 32768) * 4096 / 32768) + 0x9000;
		vel.m_yFixed = ((r2 % 32768) * 4096 / 32768);
		vel.m_zFixed = ((r3 % 32768) * 4096 / 32768) + 0xc000;

		m_activator->m_unk0xc0 = 0;
		m_activator->m_action = (eAction) 0;
		m_activator->StartFly(vel, &pos);
		m_activator = 0;
		Action((eAction) 27);
		SetSndEffect((eSoundEffect) 9);
		return 1;
	}
	case 27:
		if (g_dwGameTick <= m_actionDeadline) {
			return 1;
		}
		Action((eAction) 24);
		break;
	}
	return 1;
}

// 68K 0x10618d6a Activate__9CCatapultFP11CGameObject
// FUNCTION: LEMBALL 0x0041c9b0
bool Catapult::Activate(GameObject* p_object)
{
	if (m_action == 24) {
		m_activator = p_object;
		m_stateTimer = g_dwSimulationTimestamp;
		m_unk0xd0 = 32;
		m_unk0xd4 = 46;
		m_actionDeadline = 94;
		RequestAction((eAction) 25);
		return 1;
	}
	return 0;
}

// 68K 0x10618dde DoActivate__9CCatapultFv
// FUNCTION: LEMBALL 0x0041ca00
void Catapult::DoActivate()
{
	m_stateTimer = g_dwSimulationTimestamp;
	m_unk0xd0 += g_dwGameTick;
	m_unk0xd4 += g_dwGameTick;
	m_actionDeadline += g_dwGameTick;
	m_unk0x90 = m_activator->m_objectType;
	m_activator->m_unk0xc0 = 1;
	m_activator->m_action = (eAction) 5;
	g_pAI->AddTime(20);
}

// 68K 0x10618e62 ActivatePosition__9CCatapultFv
// FUNCTION: LEMBALL 0x0041ca60
AiCoord Catapult::ActivatePosition()
{
	int y = m_position.m_yFixed - 0xc000;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed - 0x3c000;
	return AiCoord(x, y, z);
}

