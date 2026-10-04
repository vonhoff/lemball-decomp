#include "CDoor.h"

#include "Application/CGame.h"
#include "Application/GameTime.h"
#include "Map/CMap.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Application/SoundEffects.h"

enum {
	DOOR_OPENING_ANIMATION_DURATION_TICKS = 80,
	DOOR_OPEN_HOLD_DURATION_TICKS = 20,
	DOOR_UNLOCK_OPENING_DELAY_TICKS = 20,
	DOOR_LOCKED_FEEDBACK_DURATION_TICKS = 40,
	DOOR_HITBOX_LEFT_OFFSET_PIXELS = -40,
	DOOR_HITBOX_TOP_OFFSET_PIXELS = -8,
	DOOR_HITBOX_WIDTH_PIXELS = 48,
	DOOR_HITBOX_HEIGHT_PIXELS = 16,
	DOOR_UNLOCK_WITH_SWITCH_SCORE_POINTS = 25,
	DOOR_UNLOCK_WITH_KEY_SCORE_POINTS = 75
};

extern unsigned short g_wNextDoorIndex;

enum eDoorLockType {
	DOOR_LOCK_NONE = 0,
	DOOR_LOCK_KEY_1 = 1,
	DOOR_LOCK_KEY_2 = 2,
	DOOR_LOCK_KEY_3 = 3,
	DOOR_LOCK_SWITCH = 4
};

// FUNCTION: LEMBALL 0x0040d470
CDoor::CDoor() : CGlobalGameObject(OBJECT_DOOR_1, 0, 0)
{
}

// FUNCTION: LEMBALL 0x0040d490
void CDoor::Restart()
{
	CGlobalGameObject::Restart();
}

// FUNCTION: LEMBALL 0x0040d4a0
void CDoor::Set(eObjectType p_objectType, unsigned short p_doorType, int p_x, int p_y, int p_z)
{
	m_objectType = p_objectType;
	m_spawnPosition.m_xFixed = p_x << FIXED_POINT_FRACTION_BITS;
	m_spawnPosition.m_yFixed = p_y << FIXED_POINT_FRACTION_BITS;
	m_spawnPosition.m_zFixed = p_z << FIXED_POINT_FRACTION_BITS;
	m_doorType = p_doorType;
	m_doorIndex = g_wNextDoorIndex++;
	m_action = ACTION_DOOR_CLOSED;
	if (m_doorType != DOOR_LOCK_NONE) {
		m_action = ACTION_DOOR_LOCKED;
	}
	switch ((unsigned short) m_doorType) {
	case DOOR_LOCK_NONE:
		m_actionArgument = (short) OBJECT_INVALID;
		break;
	case DOOR_LOCK_KEY_1:
		m_actionArgument = OBJECT_KEY_1;
		break;
	case DOOR_LOCK_KEY_2:
		m_actionArgument = OBJECT_KEY_2;
		break;
	case DOOR_LOCK_KEY_3:
		m_actionArgument = OBJECT_KEY_3;
		break;
	case DOOR_LOCK_SWITCH:
		m_actionArgument = OBJECT_SWITCH;
		break;
	}
	m_setTick = g_dwGameTick;
	m_position.m_xFixed = m_spawnPosition.m_xFixed;
	m_position.m_yFixed = m_spawnPosition.m_yFixed;
	m_position.m_zFixed = m_spawnPosition.m_zFixed;
	m_activationPending = 0;
	int blockY;
	int blockX = p_x / GROUND_BLOCK_PIXEL_SIZE;
	blockY = p_y / GROUND_BLOCK_PIXEL_SIZE;
	CMap* groundMap;
	switch (m_objectType) {
	case OBJECT_DOOR_1:
		groundMap = g_pMap;
		if (blockX >= 0 && blockY + 1 >= 0 && blockX < groundMap->m_ground.m_width &&
			blockY + 1 < groundMap->m_ground.m_height) {
			groundMap->m_ground.GetGroundCell(blockX, blockY + 1)->m_collision |= GROUND_COLLISION_OBJECT_INTERACTION;
		}
		groundMap = g_pMap;
		if (blockX >= 0 && blockY + 1 >= 0 && blockX < groundMap->m_ground.m_width &&
			blockY + 1 < groundMap->m_ground.m_height) {
			groundMap->m_ground.GetGroundCell(blockX, blockY + 1)->m_collision |= GROUND_COLLISION_BLOCKS_WALKING;
		}
		groundMap = g_pMap;
		if (blockX >= 0 && blockY >= 0 && blockX < groundMap->m_ground.m_width &&
			blockY < groundMap->m_ground.m_height) {
			groundMap->m_ground.GetGroundCell(blockX, blockY)->m_collision |= GROUND_COLLISION_OBJECT_INTERACTION;
		}
		groundMap = g_pMap;
		if (blockX >= 0 && blockY >= 0 && blockX < groundMap->m_ground.m_width &&
			blockY < groundMap->m_ground.m_height) {
			groundMap->m_ground.GetGroundCell(blockX, blockY)->m_collision |= GROUND_COLLISION_BLOCKS_WALKING;
		}
		groundMap = g_pMap;
		if (blockX >= 0 && blockY - 1 >= 0 && blockX < groundMap->m_ground.m_width &&
			blockY - 1 < groundMap->m_ground.m_height) {
			groundMap->m_ground.GetGroundCell(blockX, blockY - 1)->m_collision |= GROUND_COLLISION_OBJECT_INTERACTION;
		}
		groundMap = g_pMap;
		if (blockX >= 0 && --blockY >= 0 && blockX < groundMap->m_ground.m_width &&
			blockY < groundMap->m_ground.m_height) {
			groundMap->m_ground.GetGroundCell(blockX, blockY)->m_collision |= GROUND_COLLISION_BLOCKS_WALKING;
		}
		break;
	case OBJECT_DOOR_2:
		g_pMap->m_ground.SetCollision(blockX + 1, blockY, GROUND_COLLISION_OBJECT_INTERACTION);
		g_pMap->m_ground.SetCollision(blockX + 1, blockY, 1);
		g_pMap->m_ground.SetCollision(blockX, blockY, GROUND_COLLISION_OBJECT_INTERACTION);
		g_pMap->m_ground.SetCollision(blockX, blockY, 1);
		g_pMap->m_ground.SetCollision(--blockX, blockY, GROUND_COLLISION_OBJECT_INTERACTION);
		groundMap = g_pMap;
		if (blockX >= 0 && blockY >= 0 && blockX < groundMap->m_ground.m_width &&
			blockY < groundMap->m_ground.m_height) {
			groundMap->m_ground.GetGroundCell(blockX, blockY)->m_collision |= GROUND_COLLISION_BLOCKS_WALKING;
		}
		break;
	}
}

// FUNCTION: LEMBALL 0x0040d760
void CDoor::Delete()
{
	int blockX = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	int blockY = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;

	switch (m_objectType) {
	case OBJECT_DOOR_1:
		if (blockX >= 0 && blockY + 1 >= 0 && blockX < g_pMap->m_ground.m_width &&
			g_pMap->m_ground.m_height > blockY + 1) {
			g_pMap->m_ground.m_ground[(blockY + 1) * g_pMap->m_ground.m_width + blockX].m_collision &=
				~GROUND_COLLISION_BLOCKS_WALKING;
		}
		if (blockX >= 0 && blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision &=
				~GROUND_COLLISION_BLOCKS_WALKING;
		}
		if (blockX >= 0 && --blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision &=
				~GROUND_COLLISION_BLOCKS_WALKING;
		}
		break;
	case OBJECT_DOOR_2:
		if (blockX + 1 >= 0 && blockY >= 0 && g_pMap->m_ground.m_width > blockX + 1 &&
			g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX + 1].m_collision &=
				~GROUND_COLLISION_BLOCKS_WALKING;
		}
		if (blockX >= 0 && blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision &=
				~GROUND_COLLISION_BLOCKS_WALKING;
		}
		if (blockX - 1 >= 0 && blockY >= 0 && g_pMap->m_ground.m_width > blockX - 1 &&
			g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX - 1].m_collision &=
				~GROUND_COLLISION_BLOCKS_WALKING;
		}
		break;
	}
}

// FUNCTION: LEMBALL 0x0040d910
void CDoor::SetCollision()
{
	int blockX = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	int blockY = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;

	switch (m_objectType) {
	case OBJECT_DOOR_1:
		if (blockX >= 0) {
			if (blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
				g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision |=
					GROUND_COLLISION_BLOCKS_WALKING;
			}
			if (blockX >= 0 && --blockY >= 0) {
				CMap* map = g_pMap;
				int width = map->m_ground.m_width;
				if (blockX < width && map->m_ground.m_height > blockY) {
					g_pMap->m_ground.m_ground[width * blockY + blockX].m_collision |= GROUND_COLLISION_BLOCKS_WALKING;
				}
			}
		}
		break;
	case OBJECT_DOOR_2:
		if (blockX >= 0 && blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision |=
				GROUND_COLLISION_BLOCKS_WALKING;
		}
		if (blockX - 1 >= 0 && blockY >= 0 && g_pMap->m_ground.m_width > blockX - 1 &&
			g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX - 1].m_collision |=
				GROUND_COLLISION_BLOCKS_WALKING;
		}
		break;
	}
}

// FUNCTION: LEMBALL 0x0040da40
void CDoor::ResetCollision()
{
	int blockX = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	int blockY = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;

	switch (m_objectType) {
	case OBJECT_DOOR_1:
		if (blockX >= 0) {
			if (blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
				g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision &=
					~GROUND_COLLISION_BLOCKS_WALKING;
			}
			if (blockX >= 0 && --blockY >= 0) {
				CMap* map = g_pMap;
				int width = map->m_ground.m_width;
				if (blockX < width && map->m_ground.m_height > blockY) {
					g_pMap->m_ground.m_ground[width * blockY + blockX].m_collision &= ~GROUND_COLLISION_BLOCKS_WALKING;
				}
			}
		}
		break;
	case OBJECT_DOOR_2:
		if (blockX >= 0 && blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision &=
				~GROUND_COLLISION_BLOCKS_WALKING;
		}
		if (blockX - 1 >= 0 && blockY >= 0 && g_pMap->m_ground.m_width > blockX - 1 &&
			g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX - 1].m_collision &=
				~GROUND_COLLISION_BLOCKS_WALKING;
		}
		break;
	}
}

// FUNCTION: LEMBALL 0x0040db80
bool CDoor::Process()
{
	if (m_isRemoteObject) {
		if (m_pendingAction != m_action) {
			switch (m_action) {
			case ACTION_DOOR_OPENING:
				SetSndEffect(SFX_DOOROPEN);
				ResetCollision();
				break;
			case ACTION_DOOR_CLOSING:
				SetCollision();
				break;
			default:
				break;
			}
			m_pendingAction = m_action;
		}
		return true;
	}

	if (!m_activationPending) {
		return true;
	}

	if (m_actionDeadline > g_dwGameTick) {
		return true;
	}

	m_stateTimer = g_dwSimulationTimestamp;
	switch (m_action) {
	case ACTION_DOOR_LOCKED_FEEDBACK:
		Action(ACTION_DOOR_LOCKED);
		m_activationPending = 0;
		break;
	case ACTION_DOOR_OPENING:
		m_stateTimer = g_dwSimulationTimestamp;
		m_actionDeadline = g_dwGameTick + DOOR_OPENING_ANIMATION_DURATION_TICKS;
		ResetCollision();
		Action(ACTION_DOOR_OPEN);
		break;
	case ACTION_DOOR_OPEN:
		if (m_doorType == DOOR_LOCK_NONE) {
			m_stateTimer = g_dwSimulationTimestamp;
			m_actionDeadline = g_dwGameTick + DOOR_OPEN_HOLD_DURATION_TICKS;
			SetCollision();
			SetSndEffect(SFX_DOOROPEN);
			Action(ACTION_DOOR_CLOSING);
			return true;
		}
		m_activationPending = 0;
		break;
	case ACTION_DOOR_CLOSING:
		m_activationPending = 0;
		Action(ACTION_DOOR_CLOSED);
		break;
	default:
		break;
	}

	return true;
}

// FUNCTION: LEMBALL 0x0040dd00
void CDoor::Unlock()
{
	if (m_action >= ACTION_DOOR_LOCKED_FEEDBACK && m_action <= ACTION_DOOR_LOCKED) {
		m_actionDeadline = DOOR_UNLOCK_OPENING_DELAY_TICKS;
		SetSndEffect(SFX_DOOROPEN);
		RequestAction(ACTION_DOOR_OPENING);
	}
}

// FUNCTION: LEMBALL 0x0040dd30
bool CDoor::IsUsable(eAction p_action)
{
	return p_action == ACTION_READY || (p_action >= ACTION_DOOR_LOCKED && p_action <= ACTION_DOOR_CLOSED);
}

// FUNCTION: LEMBALL 0x0040dd50
bool CDoor::TryBeginActivation()
{
	unsigned int tick;

	if (m_activationPending != 0) {
		return false;
	}
	tick = g_dwGameTick;
	m_activationPending = 1;
	m_setTick = tick;
	return true;
}

// FUNCTION: LEMBALL 0x0040dd80
int CDoor::Hits(const AICOORD& p_position, CGameObject* p_object)
{
	int x = p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int y = p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int doorX = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int doorY = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	doorX += DOOR_HITBOX_LEFT_OFFSET_PIXELS;
	doorY += DOOR_HITBOX_TOP_OFFSET_PIXELS;
	int maxX = doorX + DOOR_HITBOX_WIDTH_PIXELS;
	int maxY = doorY + DOOR_HITBOX_HEIGHT_PIXELS;
	if (doorX <= x && maxX >= x && doorY <= y && maxY >= y) {
		switch (m_action) {
		case ACTION_DOOR_LOCKED_FEEDBACK:
		case ACTION_DOOR_LOCKED:
			if (p_object->HasObject((eObjectType) (unsigned short) m_actionArgument)) {
				m_actionDeadline = DOOR_UNLOCK_OPENING_DELAY_TICKS;
				SetSndEffect(SFX_DOOROPEN);
				RequestAction(ACTION_DOOR_OPENING);
				return 1;
			}
			m_actionDeadline = DOOR_LOCKED_FEEDBACK_DURATION_TICKS;
			RequestAction(ACTION_DOOR_LOCKED_FEEDBACK);
			return 0;
		case ACTION_DOOR_CLOSED:
			m_actionDeadline = DOOR_UNLOCK_OPENING_DELAY_TICKS;
			SetSndEffect(SFX_DOOROPEN);
			RequestAction(ACTION_DOOR_OPENING);
			return 1;
		case ACTION_DOOR_OPENING:
			return 1;
		case ACTION_DOOR_OPEN:
			return 0;
		case ACTION_DOOR_CLOSING:
			return 1;
		default:
			return 1;
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0040dec0
void CDoor::DoActivate()
{
	m_activationPending = 1;
	m_stateTimer = g_dwSimulationTimestamp;
	m_actionDeadline += g_dwGameTick;
	if (m_action != ACTION_DOOR_LOCKED_FEEDBACK) {
		int actionArgument = (unsigned short) m_actionArgument;
		int score;
		switch (actionArgument) {
		case OBJECT_SWITCH:
			score = DOOR_UNLOCK_WITH_SWITCH_SCORE_POINTS;
			break;
		case OBJECT_KEY_1:
		case OBJECT_KEY_2:
		case OBJECT_KEY_3:
			score = DOOR_UNLOCK_WITH_KEY_SCORE_POINTS;
			break;
		default:
			score = DOOR_UNLOCK_WITH_SWITCH_SCORE_POINTS;
			break;
		}
		g_pAI->Score(score);
	}
}
