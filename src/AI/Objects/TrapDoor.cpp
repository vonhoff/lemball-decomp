#include "TrapDoor.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"
#include "ViewData.h"

// GLOBAL: LEMBALL 0x0049cf3c
unsigned int g_dwTrapDoorLocalSfxState = 0;

// GLOBAL: LEMBALL 0x0049cf40
unsigned int g_dwTrapDoorRemoteSfxState = 0;

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
	int width;
	if (x < 0 || y < 0 || (width = map->m_ground.m_width) <= blockX || map->m_ground.m_height <= blockY) {
		z = 0;
	}
	else {
		int groundX = x & 0xf;
		int groundY = y & 0xf;
		z = map->m_ground.m_ground[blockY * width + blockX].GetZ(groundX, groundY);
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
	p_viewData.m_action = m_action;
	p_viewData.m_actionArgument = actionArgument;
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
// FUNCTION: LEMBALL 0x0040c4f0
bool TrapDoor::Process()
{
	if (m_active == 0) {
		return true;
	}
	if (m_isRemoteObject != 0) {
		bool finished = false;
		if (m_pendingAction != m_action) {
			switch (m_action) {
			case 0x1e:
				finished = true;
				break;
			case 0x20:
				if (g_dwTrapDoorRemoteSfxState == 0) {
					g_dwTrapDoorRemoteSfxState = 1;
					SetSndEffect((eSoundEffect) 0x18);
				}
				break;
			case 0x22:
				if (g_dwTrapDoorRemoteSfxState == 1) {
					g_dwTrapDoorRemoteSfxState = 0;
					SetSndEffect((eSoundEffect) 0x18);
				}
				break;
			}
			m_pendingAction = m_action;
		}
		return !finished;
	}
	if (m_mode != 0) {
		return true;
	}
	if (m_actionDeadline <= g_dwGameTick) {
		m_stateTimer = g_dwSimulationTimestamp;
		switch (m_action) {
		case 0x18:
			if (g_dwTrapDoorLocalSfxState == 0) {
				SetSndEffect((eSoundEffect) 0x20);
				g_dwTrapDoorLocalSfxState = 1;
			}
			m_actionDeadline = g_dwGameTick + 0x36;
			Action((eAction) 0x1f);
			break;
		case 0x1f:
			if (g_dwTrapDoorLocalSfxState == 1) {
				g_dwTrapDoorLocalSfxState = 0;
				SetSndEffect((eSoundEffect) 0x18);
			}
			m_actionDeadline = g_dwGameTick + 0x14;
			Action((eAction) 0x20);
			return true;
		case 0x20:
			if (g_dwTrapDoorLocalSfxState == 0) {
				g_dwTrapDoorLocalSfxState = 1;
				SetSndEffect((eSoundEffect) 1);
			}
			m_actionDeadline = g_dwGameTick + 0x50;
			Action((eAction) 0x21);
			return true;
		case 0x21:
			if (g_dwTrapDoorLocalSfxState == 1) {
				g_dwTrapDoorLocalSfxState = 0;
				SetSndEffect((eSoundEffect) 0x18);
			}
			m_actionDeadline = g_dwGameTick + 0x14;
			Action((eAction) 0x22);
			return true;
		case 0x22:
			if (g_dwTrapDoorLocalSfxState == 0) {
				SetSndEffect((eSoundEffect) 0x21);
			}
			m_actionDeadline = g_dwGameTick + 0x36;
			Action((eAction) 0x23);
			return true;
		case 0x23:
			Action((eAction) 0x1e);
			return false;
		}
	}
	return true;
}

// FUNCTION: LEMBALL 0x0040c720
void TrapDoor::SetPositionFromIntegers(int p_x, int p_y, int p_z)
{
	m_position.m_xFixed = p_x << 12;
	m_position.m_yFixed = p_y << 12;
	m_position.m_zFixed = p_z << 12;
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
