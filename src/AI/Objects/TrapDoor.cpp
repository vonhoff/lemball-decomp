#include "TrapDoor.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"
#include "ViewData.h"

// 68K 0x1062116c __ct__9CTrapDoorFR7AICOORDUc
// FUNCTION: LEMBALL 0x0040c2d0
TrapDoor::TrapDoor(AiCoord& p_arg0, unsigned int p_arg1) : BaseGlobalObject(p_arg0, OBJECT_TRAP_DOOR)
{
	m_spawnPosition.m_xFixed = p_arg0.m_xFixed;
	m_spawnPosition.m_yFixed = p_arg0.m_yFixed;
	m_spawnPosition.m_zFixed = p_arg0.m_zFixed;
	m_mode = p_arg1;
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
// FUNCTION: LEMBALL 0x0040c3b0
void TrapDoor::GetViewData(ViewData& p_viewData)
{
	int x = m_position.m_xFixed >> 12;
	int y = m_position.m_yFixed >> 12;
	Map* map = g_pMap;
	int blockX = x >> 4;
	int blockY = y >> 4;
	unsigned short z;
	if (x < 0 || y < 0 || map->m_ground.m_width <= blockX || map->m_ground.m_height <= blockY) {
		z = 0;
	}
	else {
		int groundX = x & 0xf;
		int groundY = y & 0xf;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(groundX, groundY);
	}
	m_position.m_zFixed = (z + 0x4e) << 12;

	p_viewData.m_objectId = m_objectId;
	p_viewData.m_objectType = m_objectType;
	p_viewData.m_playerIndex = 0;
	p_viewData.m_positionX = m_position.m_xFixed >> 12;
	p_viewData.m_positionY = m_position.m_yFixed >> 12;
	p_viewData.m_positionZ = m_position.m_zFixed >> 12;
	p_viewData.m_facingDirection = m_facingDirection;
	unsigned int actionArgument = (unsigned short) m_actionArgument;
	unsigned int stateTimer = m_stateTimer;
	eAction action = m_action;
	p_viewData.m_actionArgument = actionArgument;
	p_viewData.m_action = action;
	p_viewData.m_stateTimer = stateTimer;
	p_viewData.m_statusFlags = 0;
	p_viewData.m_unk0x30 = m_unk0xc0;
	p_viewData.m_auxiliaryPosition.m_xFixed = m_auxiliaryPosition.m_xFixed;
	p_viewData.m_auxiliaryPosition.m_yFixed = m_auxiliaryPosition.m_yFixed;
	p_viewData.m_auxiliaryPosition.m_zFixed = m_auxiliaryPosition.m_zFixed;
	p_viewData.m_soundEffect = m_soundEffect;
	if (m_isRemoteObject != 0) {
		p_viewData.m_animationTime = g_dwNetworkSimulationTimestamp;
	}
	else {
		p_viewData.m_animationTime = g_dwSimulationTimestamp;
	}
	SetSndEffect((eSoundEffect) 0);
	p_viewData.m_transientFlags = m_transientFlags;
	m_transientFlags = 0;
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
