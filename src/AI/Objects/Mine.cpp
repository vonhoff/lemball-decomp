#include "Mine.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../Managers/MineManager.h"

// GLOBAL: LEMBALL 0x004a7840
short g_mineTerrainOffsets[4];

// 68K 0x10616282 __ct__5CMineFv
// STUB: LEMBALL 0x00423c10
Mine::Mine()
{
}

// 68K 0x106162c2 Restart__5CMineFv
// FUNCTION: LEMBALL 0x00423c30
void Mine::Restart()
{
	GlobalGameObject::Restart();
	Initialise();
}

// 68K 0x106162f2 Initialise__5CMineFv
// FUNCTION: LEMBALL 0x00423c50
void Mine::Initialise()
{
	m_action = (eAction) 0x18;
	m_enabled = 0;
	m_activated = 0;
	m_terrainSet = 0;
	m_triggerPending = 0;
	m_actionDeadline = g_dwGameTick;
	g_mineTerrainOffsets[0] = 10;
	g_mineTerrainOffsets[1] = 17;
	g_mineTerrainOffsets[2] = 48;
	g_mineTerrainOffsets[3] = 8;
}

// 68K 0x10616354 Set__5CMineF7AICOORD
// STUB: LEMBALL 0x00423cb0
void Mine::Set(AiCoord p_position)
{
}

// 68K 0x106163f0 Trigger__5CMineFi
// STUB: LEMBALL 0x00423d40
void Mine::Trigger(int p_delay)
{
}

// 68K 0x1061643c DoActivate__5CMineFv
// FUNCTION: LEMBALL 0x00423d70
void Mine::DoActivate()
{
	m_activated = 1;
	if (m_triggerPending != 0) {
		m_terrainSet = 0;
		m_lastMovementTick = g_dwGameTick + m_triggerDelay;
		((MineManager*) m_manager)->Triggered(this);
		return;
	}
	SetTerrain();
	m_stateTimer = g_dwSimulationTimestamp;
	m_actionDeadline = g_dwGameTick + 20;
}

// 68K 0x106164b0 SetTerrain__5CMineFv
// STUB: LEMBALL 0x00423dd0
void Mine::SetTerrain()
{
}

// 68K 0x10616578 StepOn__5CMineFP11CGameObject
// FUNCTION: LEMBALL 0x00423e70
void Mine::StepOn(GameObject* p_object)
{
	RequestAction(0x1b);
	p_object->HitMine();
}

// 68K 0x106165be IsUsable__5CMineF7eAction
// FUNCTION: LEMBALL 0x00423e90
bool Mine::IsUsable(eAction p_action)
{
	return p_action == (eAction) 8 || p_action == (eAction) 0x18;
}

// 68K 0x106165fa Process__5CMineFv
// STUB: LEMBALL 0x00423eb0
bool Mine::Process()
{
	return 0;
}

// 68K 0x106166fc OnGround__5CMineFv
// STUB: LEMBALL 0x00423fa0
void Mine::OnGround()
{
}

// 68K 0x1011a84c __dt__5CMineFv
Mine::~Mine()
{
}
