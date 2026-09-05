#include "TrapDoor.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"

// 68K 0x1062116c __ct__9CTrapDoorFR7AICOORDUc
// FUNCTION: LEMBALL 0x0040c2d0
TrapDoor::TrapDoor(AiCoord& p_arg0, unsigned char p_arg1) : BaseGlobalObject(p_arg0, OBJECT_TRAP_DOOR)
{
	m_spawnPosition.m_xFixed = p_arg0.m_xFixed;
	m_spawnPosition.m_yFixed = p_arg0.m_yFixed;
	m_spawnPosition.m_zFixed = p_arg0.m_zFixed;
	m_mode = *(unsigned int*) &p_arg1;
}

// 68K 0x10621212 Restart__9CTrapDoorFv
// FUNCTION: LEMBALL 0x0040c350
void TrapDoor::Restart()
{
	GlobalGameObject::Restart();
	m_position.m_xFixed = m_spawnPosition.m_xFixed;
	m_position.m_yFixed = m_spawnPosition.m_yFixed;
	m_position.m_zFixed = m_spawnPosition.m_zFixed;
	m_action = (eAction) 0x18;
	m_stateTimer = g_dwSimulationTimestamp;
	m_active = 1;
	m_deadline = 80;
	m_actionDeadline = g_dwGameTick;
}

// 68K 0x1062127c GetViewData__9CTrapDoorFR9CViewData
// STUB: LEMBALL 0x0040c3b0
void TrapDoor::GetViewData(ViewData& p_viewData)
{
}

// 68K 0x106213e8 Process__9CTrapDoorFv
// STUB: LEMBALL 0x0040c4f0
bool TrapDoor::Process()
{
	return 0;
}

// 68K 0x1011b660 Usage__9CTrapDoorFv
// FUNCTION: LEMBALL 0x0040ce80
int TrapDoor::Usage()
{
	return 2;
}

// 68K 0x1011b680 DoActivate__9CTrapDoorFv
// FUNCTION: LEMBALL 0x0040ce90
void TrapDoor::DoActivate()
{
}

// 68K 0x1011b5f8 __dt__9CTrapDoorFv
TrapDoor::~TrapDoor()
{
}
