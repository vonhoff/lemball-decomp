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

// 68K 0x10618a76 Process__9CCatapultFv
// STUB: LEMBALL 0x0041c720
bool Catapult::Process()
{
	return 0;
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

