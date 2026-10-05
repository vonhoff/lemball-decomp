#include "CTrapDoor.h"

#include "Gameplay/Simulation/GameTime.h"
#include "Map/CMap.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CBaseGlobalObject.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectInteractionStates.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Gameplay/Objects/CViewData.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Application/SoundEffects.h"

enum eTrapDoorSoundState {
	TRAPDOOR_SOUND_NOT_TRIGGERED = 0,
	TRAPDOOR_SOUND_TRIGGERED = 1
};

enum {
	TRAP_DOOR_ARRIVAL_DELAY_TICKS = 54,
	TRAP_DOOR_OPENING_DURATION_TICKS = 20,
	TRAP_DOOR_OPEN_DURATION_TICKS = 80,
	TRAP_DOOR_HEIGHT_ABOVE_GROUND_PIXELS = 78
};

// GLOBAL: LEMBALL 0x0049cf3c
unsigned int g_dwTrapDoorLocalSfxState = TRAPDOOR_SOUND_NOT_TRIGGERED;

// GLOBAL: LEMBALL 0x0049cf40
unsigned int g_dwTrapDoorRemoteSfxState = TRAPDOOR_SOUND_NOT_TRIGGERED;

// FUNCTION: LEMBALL 0x0040c2d0
CTrapDoor::CTrapDoor(AICOORD& p_position, unsigned int p_mode) : CBaseGlobalObject(p_position, OBJECT_TRAP_DOOR)
{
	m_spawnPosition.m_xFixed = p_position.m_xFixed;
	m_spawnPosition.m_yFixed = p_position.m_yFixed;
	m_spawnPosition.m_zFixed = p_position.m_zFixed;
	m_mode = p_mode;
}

// FUNCTION: LEMBALL 0x0040c350
void CTrapDoor::Restart()
{
	CGlobalGameObject::Restart();
	m_position.m_xFixed = m_spawnPosition.m_xFixed;
	m_position.m_yFixed = m_spawnPosition.m_yFixed;
	m_position.m_zFixed = m_spawnPosition.m_zFixed;
	m_action = ACTION_READY;
	m_stateTimer = g_dwSimulationTimestamp;
	unsigned int now = g_dwGameTick;
	m_active = 1;
	m_deadline = 80;
	m_actionDeadline = now;
}

// FUNCTION: LEMBALL 0x0040c3b0
void CTrapDoor::GetViewData(CViewData& p_viewData)
{
	int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	CMap* map = g_pMap;
	int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
	int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
	unsigned short z;
	int width;
	if (x < 0 || y < 0 || (width = map->m_ground.m_width) <= blockX || map->m_ground.m_height <= blockY) {
		z = 0;
	}
	else {
		int groundX = x & GROUND_BLOCK_PIXEL_MASK;
		int groundY = y & GROUND_BLOCK_PIXEL_MASK;
		z = map->m_ground.m_ground[blockY * width + blockX].GetZ(groundX, groundY);
	}
	m_position.m_zFixed = (z + TRAP_DOOR_HEIGHT_ABOVE_GROUND_PIXELS) << FIXED_POINT_FRACTION_BITS;

	p_viewData.m_objectId = m_objectId;
	p_viewData.m_objectType = m_objectType;
	p_viewData.m_playerIndex = 0;
	p_viewData.m_positionX = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	p_viewData.m_positionY = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	p_viewData.m_positionZ = m_position.m_zFixed >> FIXED_POINT_FRACTION_BITS;
	p_viewData.m_facingDirection = m_facingDirection;
	unsigned int actionArgument = (unsigned short) m_actionArgument;
	unsigned int stateTimer = m_stateTimer;
	p_viewData.m_action = m_action;
	p_viewData.m_actionArgument = actionArgument;
	p_viewData.m_stateTimer = stateTimer;
	p_viewData.m_statusFlags = 0;
	p_viewData.m_hidden = m_hidden;
	p_viewData.m_auxiliaryPosition = m_auxiliaryPosition;
	p_viewData.m_soundEffect = m_soundEffect;
	if (m_isRemoteObject != 0) {
		p_viewData.m_animationTime = g_dwNetworkSimulationTimestamp;
	}
	else {
		p_viewData.m_animationTime = g_dwSimulationTimestamp;
	}
	SetSndEffect(SFX_NONE);
	p_viewData.m_transientFlags = m_transientFlags;
	m_transientFlags = 0;
}

// FUNCTION: LEMBALL 0x0040c4f0
bool CTrapDoor::Process()
{
	if (m_active == 0) {
		return true;
	}
	if (m_isRemoteObject != 0) {
		bool finished = false;
		if (m_pendingAction != m_action) {
			switch (m_action) {
			case ACTION_DOOR_CLOSED:
				finished = true;
				break;
			case ACTION_DOOR_OPENING:
				if (g_dwTrapDoorRemoteSfxState == TRAPDOOR_SOUND_NOT_TRIGGERED) {
					g_dwTrapDoorRemoteSfxState = TRAPDOOR_SOUND_TRIGGERED;
					SetSndEffect(SFX_TRAPDOOR);
				}
				break;
			case ACTION_DOOR_CLOSING:
				if (g_dwTrapDoorRemoteSfxState == TRAPDOOR_SOUND_TRIGGERED) {
					g_dwTrapDoorRemoteSfxState = TRAPDOOR_SOUND_NOT_TRIGGERED;
					SetSndEffect(SFX_TRAPDOOR);
				}
				break;
			}
			m_pendingAction = m_action;
		}
		return !finished;
	}
	if (m_mode != TRAPDOOR_MODE_LOCAL_AUTOMATIC) {
		return true;
	}
	if (m_actionDeadline <= g_dwGameTick) {
		m_stateTimer = g_dwSimulationTimestamp;
		switch (m_action) {
		case ACTION_READY:
			if (g_dwTrapDoorLocalSfxState == TRAPDOOR_SOUND_NOT_TRIGGERED) {
				SetSndEffect(SFX_DOORAPPR);
				g_dwTrapDoorLocalSfxState = TRAPDOOR_SOUND_TRIGGERED;
			}
			m_actionDeadline = g_dwGameTick + TRAP_DOOR_ARRIVAL_DELAY_TICKS;
			Action(ACTION_ARRIVING);
			break;
		case ACTION_ARRIVING:
			if (g_dwTrapDoorLocalSfxState == TRAPDOOR_SOUND_TRIGGERED) {
				g_dwTrapDoorLocalSfxState = TRAPDOOR_SOUND_NOT_TRIGGERED;
				SetSndEffect(SFX_TRAPDOOR);
			}
			m_actionDeadline = g_dwGameTick + TRAP_DOOR_OPENING_DURATION_TICKS;
			Action(ACTION_DOOR_OPENING);
			return true;
		case ACTION_DOOR_OPENING:
			if (g_dwTrapDoorLocalSfxState == TRAPDOOR_SOUND_NOT_TRIGGERED) {
				g_dwTrapDoorLocalSfxState = TRAPDOOR_SOUND_TRIGGERED;
				SetSndEffect(SFX_LETSGO);
			}
			m_actionDeadline = g_dwGameTick + TRAP_DOOR_OPEN_DURATION_TICKS;
			Action(ACTION_DOOR_OPEN);
			return true;
		case ACTION_DOOR_OPEN:
			if (g_dwTrapDoorLocalSfxState == TRAPDOOR_SOUND_TRIGGERED) {
				g_dwTrapDoorLocalSfxState = TRAPDOOR_SOUND_NOT_TRIGGERED;
				SetSndEffect(SFX_TRAPDOOR);
			}
			m_actionDeadline = g_dwGameTick + TRAP_DOOR_OPENING_DURATION_TICKS;
			Action(ACTION_DOOR_CLOSING);
			return true;
		case ACTION_DOOR_CLOSING:
			if (g_dwTrapDoorLocalSfxState == TRAPDOOR_SOUND_NOT_TRIGGERED) {
				SetSndEffect(SFX_DOORGO);
			}
			m_actionDeadline = g_dwGameTick + TRAP_DOOR_ARRIVAL_DELAY_TICKS;
			Action(ACTION_LEAVING);
			return true;
		case ACTION_LEAVING:
			Action(ACTION_DOOR_CLOSED);
			return false;
		}
	}
	return true;
}

// FUNCTION: LEMBALL 0x0040c720
void CTrapDoor::SetPositionFromIntegers(int p_x, int p_y, int p_z)
{
	m_position.m_xFixed = p_x << FIXED_POINT_FRACTION_BITS;
	m_position.m_yFixed = p_y << FIXED_POINT_FRACTION_BITS;
	m_position.m_zFixed = p_z << FIXED_POINT_FRACTION_BITS;
}

// FUNCTION: LEMBALL 0x0040ce80
int CTrapDoor::Usage()
{
	return GROUP_OBJECT_USAGE_SINGLE;
}

// FUNCTION: LEMBALL 0x0040ce90
void CTrapDoor::DoActivate()
{
}
